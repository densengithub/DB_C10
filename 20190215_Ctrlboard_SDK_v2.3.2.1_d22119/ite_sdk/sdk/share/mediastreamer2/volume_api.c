#include "volume_api.h"

static const float max_e = (32768* 0.3);/*is RMS factor */
static const float coef = 0.2; /* floating averaging coeff. for energy */
//static const float gain_k = 0.02; /* floating averaging coeff. for gain */
static const float vol_upramp = 0.4;
static const float vol_downramp = 0.45;   /* not yet runtime parameterizable */
static const float en_weight=25.0;
static const float noise_thres=0.1;
static const float transmit_thres=4;
static const float min_ng_floorgain=0.005;
static const float agc_threshold=0.5;

static inline int16_t saturate(int val) {
	return (val>32767) ? 32767 : ( (val<-32767) ? -32767 : val);
}

static void update_energy(int16_t *signal, int numsamples, Volume *v) {
	int i;
	float acc = 0;
	float en;
	int lp = 0, pk = 0;
		
	for (i=0;i<numsamples;++i){
		int s=signal[i];
		acc += s * s;

		lp = abs(s);
		if (lp > pk) pk = lp;
	}
	en = (sqrt(acc / numsamples)+1) / v->max_energy;
	v->energy = (en * coef) + v->energy * (1.0 - coef);
	v->level_pk = (float)pk / v->max_energy;
	v->instant_energy = en;// currently non-averaged energy seems better (short artefacts)
}

static void apply_gain(Volume *v, mblk_t *m, float tgain) {
	int16_t *sample;
	int dc_offset = 0;
	int32_t intgain;
	float gain;

	/* ramps with factors means linear ramps in logarithmic domain */
	
	if (v->gain < tgain) {
		if (v->gain<v->ng_floorgain) v->gain=v->ng_floorgain;
        
		v->gain *= 1 + (v->fast_upramp ? v->vol_fast_upramp : v->vol_upramp);
		if (v->gain > tgain) v->gain = tgain;
        
	}else if (v->gain > tgain) {
		v->gain *= 1 - v->vol_downramp;
        
		if (v->gain < tgain) v->gain = tgain;
		v->fast_upramp=FALSE;
	}

	gain=v->gain * v->ng_gain;
	intgain = gain* 4096;


	//if (v->peer) ms_message("MSVolume:%p Applying gain %5f, v->gain=%5f, tgain=%5f, ng_gain=%5f",v,gain,v->gain,tgain,v->ng_gain); 

	if (v->remove_dc){
		for (sample=(int16_t*)m->b_rptr;sample<(int16_t*)m->b_wptr;++sample){
			dc_offset+= *sample;
			*sample = saturate(((*sample - v->dc_offset) * intgain) / 4096);
		}
		/* offset smoothing */
		v->dc_offset = (v->dc_offset*7 + dc_offset*2/(m->b_wptr - m->b_rptr)) / 8;
	}else if (gain!=1){
		for (sample=(int16_t*)m->b_rptr;sample<(int16_t*)m->b_wptr;++sample){
			*sample = saturate(((*sample) * intgain) / 4096);
		}
	}
}

static void volume_noise_gate_process(Volume *v , float energy, mblk_t *om){
	static int counter;
	float tgain = v->ng_floorgain;  /* start with floorgain */
	int nsamples=((om->b_wptr-om->b_rptr)/2);
	if (energy > v->ng_threshold) {
		v->ng_noise_dur = v->ng_cut_time;
		tgain = 1.0;
	}
	else {
		if (v->ng_noise_dur > 0) {
			v->ng_noise_dur -= (nsamples * 1000) / v->sample_rate;
			tgain = 1.0;
		}
	}
	/* simple averaging computation is adequate here: fast rise, relatively slower decrease */
	/* of gain - ears impression */
	v->ng_gain = v->ng_gain*0.75 + tgain*0.25;
	//if (!(++counter % 10))
	//	ms_debug("%d: nglevel=%f, energy=%f, tgain=%f, ng_gain=%f", (v->peer!=NULL)?1:0, energy, v->energy, tgain, v->ng_gain);
}


Volume* VOICE_INIT(void){
	Volume *v=(Volume*)ms_new0(Volume,1);
	v->energy=0;
	v->level_pk = 0;
	v->static_gain = v->gain = v->target_gain = 1;
	v->dc_offset = 0;
	v->vol_upramp = vol_upramp;
	v->vol_fast_upramp=vol_upramp*3;
	v->vol_downramp = vol_downramp;
	v->ea_thres = noise_thres;
	v->ea_transmit_thres=transmit_thres;
	v->force=en_weight;
	v->sustain_time=200;
	v->sustain_dur = 0;
	v->agc_enabled=FALSE;
	v->buffer=ms_bufferizer_new();
	v->sample_rate=8000;
	v->nsamples=80;
	v->noise_gate_enabled=TRUE;
	v->ng_cut_time = 400;/*TODO: ng_sustain (milliseconds)*/
	v->ng_noise_dur=0;
	v->ng_threshold=0.05;
	v->ng_floorgain=min_ng_floorgain;
	v->ng_gain = 1;
	v->remove_dc=FALSE;
    v->max_energy=max_e;

    //preprocess
    if(v->ng_floorgain < min_ng_floorgain) v->ng_floorgain = min_ng_floorgain;
	if (v->noise_gate_enabled) v->gain = v->target_gain = v->ng_floorgain; // start with floorgain (soft start)
    
    v->nsamples=(int)(0.01*(float)v->sample_rate);
	v->fast_upramp=TRUE;
    return v;
}


void VOICE_applyProcess(Volume *v,mblk_t *TxIn){

	float target_gain;

	/* Important notice: any processes called herein can modify v->target_gain, at
	 * end of this function apply_gain() is called, thus: later process calls can
	 * override this target gain, and order must be well thought out
	 */

		/*light processing: no agc. Work in place in the input buffer*/

			update_energy((int16_t*)TxIn->b_rptr, (TxIn->b_wptr - TxIn->b_rptr) / 2, v);
			target_gain = v->static_gain;

			if (v->noise_gate_enabled)
				volume_noise_gate_process(v, v->instant_energy, TxIn);
			apply_gain(v, TxIn, target_gain);

}

void VOICE_UNINIT(Volume *v){
    ms_free(v);  
}