/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *******************************************************************************/
#define _SDIO_OPS_LINUX_C_

#include <drv_types.h>
#include <linux/current.h>
#include <linux/mmc/errorno.h>

static bool rtw_sdio_claim_host_needed(struct sdio_func *func)
{
	struct dvobj_priv *dvobj = sdio_get_drvdata(func);
	PSDIO_DATA sdio_data = &dvobj->intf_data;

	if (sdio_data->sys_sdio_irq_thd && sdio_data->sys_sdio_irq_thd == current)
		return _FALSE;
	return _TRUE;
}

inline void rtw_sdio_set_irq_thd(struct dvobj_priv *dvobj, _thread_hdl_ thd_hdl)
{
	PSDIO_DATA sdio_data = &dvobj->intf_data;

	sdio_data->sys_sdio_irq_thd = thd_hdl;
}

u8 sd_f0_read8(struct intf_hdl *pintfhdl,u32 addr, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	u8 v=0;
	struct sdio_func *func;
	bool claim_needed;

_func_enter_;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return v;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	v = sdio_f0_readb(func, addr, err);
	if (claim_needed)
		sdio_release_host(func);
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);

_func_exit_;

	return v;
}

void sd_f0_write8(struct intf_hdl *pintfhdl, u32 addr, u8 v, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	sdio_f0_writeb(func, v, addr, err);
	if (claim_needed)
		sdio_release_host(func);
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x val=0x%02x\n", __func__, *err, addr, v);

_func_exit_;
}

/*
 * Return:
 *	0		Success
 *	others	Fail
 */
s32 _sd_cmd52_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	int err=0, i;
	struct sdio_func *func;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}

	func = psdio->func;

	for (i = 0; i < cnt; i++) {
		pdata[i] = sdio_readb(func, addr+i, &err);
		if (err) {
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x\n", __func__, err, addr+i);
			break;
		}
	}

_func_exit_;

	return err;
}

/*
 * Return:
 *	0		Success
 *	others	Fail
 */
s32 sd_cmd52_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	int err=0, i;
	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_cmd52_read(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);

_func_exit_;

	return err;
}

/*
 * Return:
 *	0		Success
 *	others	Fail
 */
s32 _sd_cmd52_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	int err=0, i;
	struct sdio_func *func;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}

	func = psdio->func;

	for (i = 0; i < cnt; i++) {
		sdio_writeb(func, pdata[i], addr+i, &err);
		if (err) {
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x val=0x%02x\n", __func__, err, addr+i, pdata[i]);
			break;
		}
	}

_func_exit_;

	return err;
}

/*
 * Return:
 *	0		Success
 *	others	Fail
 */
s32 sd_cmd52_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	int err=0, i;
	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_cmd52_write(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);

_func_exit_;

	return err;
}

u8 _sd_read8(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	u8 v=0;
	struct sdio_func *func;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return v;
	}

	func = psdio->func;

	v = sdio_readb(func, addr, err);

	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);

_func_exit_;

	return v;
}

u8 sd_read8(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	u8 v=0;
	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return v;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	v = sdio_readb(func, addr, err);
	if (claim_needed)
		sdio_release_host(func);
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);

_func_exit_;

	return v;
}

u16 sd_read16(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	u16 v=0;
	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return v;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	v = sdio_readw(func, addr, err);
	if (claim_needed)
		sdio_release_host(func);
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x\n", __func__, *err, addr);

_func_exit_;

	return  v;
}

u32 _sd_read32(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	u32 v=0;
	struct sdio_func *func;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return v;
	}

	func = psdio->func;

	v = sdio_readl(func, addr, err);

	if (err && *err)
	{
		int i;

		DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x\n", __func__, *err, addr, v);

		*err = 0;
		for(i=0; i<SD_IO_TRY_CNT; i++)
		{
			//sdio_claim_host(func);
			v = sdio_readl(func, addr, err);
			//sdio_release_host(func);
			if (*err == 0){
				rtw_reset_continual_io_error(psdiodev);
				break;
			}
			else{
				DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);
				if(( -ESHUTDOWN == *err ) || ( -ENODEV == *err))
				{
					padapter->bSurpriseRemoved = _TRUE;
				}

				if(rtw_inc_and_chk_continual_io_error(psdiodev) == _TRUE ){
					padapter->bSurpriseRemoved = _TRUE;
					break;
				}

			}
		}

		if (i==SD_IO_TRY_CNT)
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);
		else
			DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);

	}

_func_exit_;

	return  v;
}

u32 sd_read32(struct intf_hdl *pintfhdl, u32 addr, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	u32 v=0;
	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return v;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	v = sdio_readl(func, addr, err);
	if (claim_needed)
		sdio_release_host(func);

	if (err && *err)
	{
		int i;

		DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x\n", __func__, *err, addr, v);

		*err = 0;
		for(i=0; i<SD_IO_TRY_CNT; i++)
		{
			if (claim_needed) sdio_claim_host(func);
			v = sdio_readl(func, addr, err);
			if (claim_needed) sdio_release_host(func);

			if (*err == 0){
				rtw_reset_continual_io_error(psdiodev);
				break;
			}else{
				DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);
				if(( -ESHUTDOWN == *err ) || ( -ENODEV == *err)){
					padapter->bSurpriseRemoved = _TRUE;
				}

				if(rtw_inc_and_chk_continual_io_error(psdiodev) == _TRUE ){
					padapter->bSurpriseRemoved = _TRUE;
					break;
				}
			}
		}

		if (i==SD_IO_TRY_CNT)
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);
		else
			DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);

	}

_func_exit_;

	return  v;
}

void sd_write8(struct intf_hdl *pintfhdl, u32 addr, u8 v, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	struct sdio_func *func;
	bool claim_needed;

_func_enter_;

	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return ;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	sdio_writeb(func, v, addr, err);
	if (claim_needed)
		sdio_release_host(func);
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x val=0x%02x\n", __func__, *err, addr, v);

_func_exit_;
}

void sd_write16(struct intf_hdl *pintfhdl, u32 addr, u16 v, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return ;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	sdio_writew(func, v, addr, err);
	if (claim_needed)
		sdio_release_host(func);
	if (err && *err)
		DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x val=0x%04x\n", __func__, *err, addr, v);

_func_exit_;
}

void _sd_write32(struct intf_hdl *pintfhdl, u32 addr, u32 v, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	struct sdio_func *func;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return ;
	}

	func = psdio->func;

	sdio_writel(func, v, addr, err);

	if (err && *err)
	{
		int i;

		DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x val=0x%08x\n", __func__, *err, addr, v);

		*err = 0;
		for(i=0; i<SD_IO_TRY_CNT; i++)
		{
			sdio_writel(func, v, addr, err);
			if (*err == 0){
				rtw_reset_continual_io_error(psdiodev);
				break;
			}else{
				DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);
				if(( -ESHUTDOWN == *err ) || ( -ENODEV == *err)){
					padapter->bSurpriseRemoved = _TRUE;
				}

				if(rtw_inc_and_chk_continual_io_error(psdiodev) == _TRUE ){
					padapter->bSurpriseRemoved = _TRUE;
					break;
				}
			}
		}

		if (i==SD_IO_TRY_CNT)
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x val=0x%08x, try_cnt=%d\n", __func__, *err, addr, v, i);
		else
			DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x val=0x%08x, try_cnt=%d\n", __func__, *err, addr, v, i);

	}

_func_exit_;
}

void sd_write32(struct intf_hdl *pintfhdl, u32 addr, u32 v, s32 *err)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;
	struct sdio_func *func;
	bool claim_needed;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return ;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	sdio_writel(func, v, addr, err);
	if (claim_needed)
		sdio_release_host(func);

	if (err && *err)
	{
		int i;

		DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x val=0x%08x\n", __func__, *err, addr, v);

		*err = 0;
		for(i=0; i<SD_IO_TRY_CNT; i++)
		{
			if (claim_needed) sdio_claim_host(func);
			sdio_writel(func, v, addr, err);
			if (claim_needed) sdio_release_host(func);
			if (*err == 0){
				rtw_reset_continual_io_error(psdiodev);
				break;
			}else{
				DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x, val=0x%x, try_cnt=%d\n", __func__, *err, addr, v, i);
				if(( -ESHUTDOWN == *err ) || ( -ENODEV == *err)){
					padapter->bSurpriseRemoved = _TRUE;
				}

				if(rtw_inc_and_chk_continual_io_error(psdiodev) == _TRUE ){
					padapter->bSurpriseRemoved = _TRUE;
					break;
				}
			}
		}

		if (i==SD_IO_TRY_CNT)
			DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x val=0x%08x, try_cnt=%d\n", __func__, *err, addr, v, i);
		else
			DBG_871X(KERN_ERR "%s: (%d) addr=0x%05x val=0x%08x, try_cnt=%d\n", __func__, *err, addr, v, i);
	}

_func_exit_;
}

/*
 * Use CMD53 to read data from SDIO device.
 * This function MUST be called after sdio_claim_host() or
 * in SDIO ISR(host had been claimed).
 *
 * Parameters:
 *	psdio	pointer of SDIO_DATA
 *	addr	address to read
 *	cnt		amount to read
 *	pdata	pointer to put data, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *	0		Success
 *	others	Fail
 */
s32 _sd_read(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	int err= -EPERM;
	struct sdio_func *func;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}

	func = psdio->func;

	if (unlikely((cnt==1) || (cnt==2)))
	{
		int i;
		u8 *pbuf = (u8*)pdata;

		for (i = 0; i < cnt; i++)
		{
			*(pbuf+i) = sdio_readb(func, addr+i, &err);

			if (err) {
				DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x\n", __func__, err, addr);
				break;
			}
		}
		return err;
	}

	err = sdio_memcpy_fromio(func, pdata, addr, cnt);
	if (err) {
		DBG_871X(KERN_ERR "%s: FAIL(%d)! ADDR=%#x Size=%d\n", __func__, err, addr, cnt);
	}

_func_exit_;

	return err;
}

/*
 * Use CMD53 to read data from SDIO device.
 *
 * Parameters:
 *	psdio	pointer of SDIO_DATA
 *	addr	address to read
 *	cnt		amount to read
 *	pdata	pointer to put data, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *	0		Success
 *	others	Fail
 */
s32 sd_read(struct intf_hdl * pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	struct sdio_func *func;
	bool claim_needed;
	s32 err= -EPERM;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}
	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_read(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);
_func_exit_;
	return err;
}

/*
 * Use CMD53 to write data to SDIO device.
 * This function MUST be called after sdio_claim_host() or
 * in SDIO ISR(host had been claimed).
 *
 * Parameters:
 *	psdio	pointer of SDIO_DATA
 *	addr	address to write
 *	cnt		amount to write
 *	pdata	data pointer, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *	0		Success
 *	others	Fail
 */
s32 _sd_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	struct sdio_func *func;
	u32 size;
	s32 err=-EPERM;

_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}

	func = psdio->func;
//	size = sdio_align_size(func, cnt);

	if (unlikely((cnt==1) || (cnt==2)))
	{
		int i;
		u8 *pbuf = (u8*)pdata;

		for (i = 0; i < cnt; i++)
		{
			sdio_writeb(func, *(pbuf+i), addr+i, &err);
			if (err) {
				DBG_871X(KERN_ERR "%s: FAIL!(%d) addr=0x%05x val=0x%02x\n", __func__, err, addr, *(pbuf+i));
				break;
			}
		}

		return err;
	}

	size = cnt;
	err = sdio_memcpy_toio(func, addr, pdata, size);
	if (err) {
		DBG_871X(KERN_ERR "%s: FAIL(%d)! ADDR=%#x Size=%d(%d)\n", __func__, err, addr, cnt, size);
	}

_func_exit_;

	return err;
}

/*
 * Use CMD53 to write data to SDIO device.
 *
 * Parameters:
 *  psdio	pointer of SDIO_DATA
 *  addr	address to write
 *  cnt		amount to write
 *  pdata	data pointer, this should be a "DMA:able scratch buffer"!
 *
 * Return:
 *  0		Success
 *  others	Fail
 */
s32 sd_write(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, void *pdata)
{
	PADAPTER padapter;
	struct dvobj_priv *psdiodev;
	PSDIO_DATA psdio;

	struct sdio_func *func;
	bool claim_needed;
	s32 err=-EPERM;
_func_enter_;
	padapter = pintfhdl->padapter;
	psdiodev = pintfhdl->pintf_dev;
	psdio = &psdiodev->intf_data;

	if(padapter->bSurpriseRemoved){
		//DBG_871X(" %s (padapter->bSurpriseRemoved ||adapter->pwrctrlpriv.pnp_bstop_trx)!!!\n",__FUNCTION__);
		return err;
	}

	func = psdio->func;
	claim_needed = rtw_sdio_claim_host_needed(func);

	if (claim_needed)
		sdio_claim_host(func);
	err = _sd_write(pintfhdl, addr, cnt, pdata);
	if (claim_needed)
		sdio_release_host(func);
_func_exit_;
	return err;
}

//porting from Irene Lin's project
#if 1
/**
 *	sdio_register_driver - register a function driver
 *	@drv: SDIO function driver
 */
extern int sdio_register_driver(struct sdio_driver *);
/**
 *	sdio_unregister_driver - unregister a function driver
 *	@drv: SDIO function driver
 */
extern void sdio_unregister_driver(struct sdio_driver *);

/*
 * SDIO I/O operations
 */
 /**
 *	sdio_claim_host - exclusively claim a bus for a certain SDIO function
 *	@func: SDIO function that will be accessed
 *
 *	Claim a bus for a set of operations. The SDIO function given
 *	is used to figure out which bus is relevant.
 */
extern void sdio_claim_host(struct sdio_func *func);
/**
 *	sdio_release_host - release a bus for a certain SDIO function
 *	@func: SDIO function that was accessed
 *
 *	Release a bus, allowing others to claim the bus for their
 *	operations.
 */
extern void sdio_release_host(struct sdio_func *func);
/**
 *	sdio_enable_func - enables a SDIO function for usage
 *	@func: SDIO function to enable
 *
 *	Powers up and activates a SDIO function so that register
 *	access is possible.
 */
extern int sdio_enable_func(struct sdio_func *func);
/**
 *	sdio_disable_func - disable a SDIO function
 *	@func: SDIO function to disable
 *
 *	Powers down and deactivates a SDIO function. Register access
 *	to this function will fail until the function is reenabled.
 */
extern int sdio_disable_func(struct sdio_func *func);
/**
 *	sdio_set_block_size - set the block size of an SDIO function
 *	@func: SDIO function to change
 *	@blksz: new block size or 0 to use the default.
 *
 *	The default block size is the largest supported by both the function
 *	and the host, with a maximum of 512 to ensure that arbitrarily sized
 *	data transfer use the optimal (least) number of commands.
 *
 *	A driver may call this to override the default block size set by the
 *	core. This can be used to set a block size greater than the maximum
 *	that reported by the card; it is the driver's responsibility to ensure
 *	it uses a value that the card supports.
 *
 *	Returns 0 on success, -EINVAL if the host does not support the
 *	requested block size, or -EIO (etc.) if one of the resultant FBR block
 *	size register writes failed.
 *
 */
extern int sdio_set_block_size(struct sdio_func *func, unsigned blksz);
/**
 *	sdio_claim_irq - claim the IRQ for a SDIO function
 *	@func: SDIO function
 *	@handler: IRQ handler callback
 *
 *	Claim and activate the IRQ for the given SDIO function. The provided
 *	handler will be called when that IRQ is asserted.  The host is always
 *	claimed already when the handler is called so the handler must not
 *	call sdio_claim_host() nor sdio_release_host().
 */
extern int sdio_claim_irq(struct sdio_func *func, sdio_irq_handler_t *handler);
/**
 *	sdio_release_irq - release the IRQ for a SDIO function
 *	@func: SDIO function
 *
 *	Disable and release the IRQ for the given SDIO function.
 */
extern int sdio_release_irq(struct sdio_func *func);
/**
 *	sdio_align_size - pads a transfer size to a more optimal value
 *	@func: SDIO function
 *	@sz: original transfer size
 *
 *	Pads the original data size with a number of extra bytes in
 *	order to avoid controller bugs and/or performance hits
 *	(e.g. some controllers revert to PIO for certain sizes).
 *
 *	If possible, it will also adjust the size so that it can be
 *	handled in just a single request.
 *
 *	Returns the improved size, which might be unmodified.
 */
extern unsigned int sdio_align_size(struct sdio_func *func, unsigned int sz);
/**
 *	sdio_readb - read a single byte from a SDIO function
 *	@func: SDIO function to access
 *	@addr: address to read
 *	@err_ret: optional status value from transfer
 *
 *	Reads a single byte from the address space of a given SDIO
 *	function. If there is a problem reading the address, 0xff
 *	is returned and @err_ret will contain the error code.
 */
extern u8 sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret);
/**
 *	sdio_readw - read a 16 bit integer from a SDIO function
 *	@func: SDIO function to access
 *	@addr: address to read
 *	@err_ret: optional status value from transfer
 *
 *	Reads a 16 bit integer from the address space of a given SDIO
 *	function. If there is a problem reading the address, 0xffff
 *	is returned and @err_ret will contain the error code.
 */
extern u16 sdio_readw(struct sdio_func *func, unsigned int addr, int *err_ret);
/**
 *	sdio_readl - read a 32 bit integer from a SDIO function
 *	@func: SDIO function to access
 *	@addr: address to read
 *	@err_ret: optional status value from transfer
 *
 *	Reads a 32 bit integer from the address space of a given SDIO
 *	function. If there is a problem reading the address,
 *	0xffffffff is returned and @err_ret will contain the error
 *	code.
 */
extern u32 sdio_readl(struct sdio_func *func, unsigned int addr, int *err_ret);
/**
 *	sdio_memcpy_fromio - read a chunk of memory from a SDIO function
 *	@func: SDIO function to access
 *	@dst: buffer to store the data
 *	@addr: address to begin reading from
 *	@count: number of bytes to read
 *
 *	Reads from the address space of a given SDIO function. Return
 *	value indicates if the transfer succeeded or not.
 */
extern int sdio_memcpy_fromio(struct sdio_func *func, void *dst,
	unsigned int addr, int count);
/**
 *	sdio_readsb - read from a FIFO on a SDIO function
 *	@func: SDIO function to access
 *	@dst: buffer to store the data
 *	@addr: address of (single byte) FIFO
 *	@count: number of bytes to read
 *
 *	Reads from the specified FIFO of a given SDIO function. Return
 *	value indicates if the transfer succeeded or not.
 */
extern int sdio_readsb(struct sdio_func *func, void *dst,
	unsigned int addr, int count);
/**
 *	sdio_writeb - write a single byte to a SDIO function
 *	@func: SDIO function to access
 *	@b: byte to write
 *	@addr: address to write to
 *	@err_ret: optional status value from transfer
 *
 *	Writes a single byte to the address space of a given SDIO
 *	function. @err_ret will contain the status of the actual
 *	transfer.
 */
extern void sdio_writeb(struct sdio_func *func, u8 b,
	unsigned int addr, int *err_ret);
/**
 *	sdio_writew - write a 16 bit integer to a SDIO function
 *	@func: SDIO function to access
 *	@b: integer to write
 *	@addr: address to write to
 *	@err_ret: optional status value from transfer
 *
 *	Writes a 16 bit integer to the address space of a given SDIO
 *	function. @err_ret will contain the status of the actual
 *	transfer.
 */
extern void sdio_writew(struct sdio_func *func, u16 b,
	unsigned int addr, int *err_ret);
/**
 *	sdio_writel - write a 32 bit integer to a SDIO function
 *	@func: SDIO function to access
 *	@b: integer to write
 *	@addr: address to write to
 *	@err_ret: optional status value from transfer
 *
 *	Writes a 32 bit integer to the address space of a given SDIO
 *	function. @err_ret will contain the status of the actual
 *	transfer.
 */
extern void sdio_writel(struct sdio_func *func, u32 b,
	unsigned int addr, int *err_ret);
/**
 *	sdio_writeb_readb - write and read a byte from SDIO function
 *	@func: SDIO function to access
 *	@write_byte: byte to write
 *	@addr: address to write to
 *	@err_ret: optional status value from transfer
 *
 *	Performs a RAW (Read after Write) operation as defined by SDIO spec -
 *	single byte is written to address space of a given SDIO function and
 *	response is read back from the same address, both using single request.
 *	If there is a problem with the operation, 0xff is returned and
 *	@err_ret will contain the error code.
 */
extern u8 sdio_writeb_readb(struct sdio_func *func, u8 write_byte,
	unsigned int addr, int *err_ret);
/**
 *	sdio_memcpy_toio - write a chunk of memory to a SDIO function
 *	@func: SDIO function to access
 *	@addr: address to start writing to
 *	@src: buffer that contains the data to write
 *	@count: number of bytes to write
 *
 *	Writes to the address space of a given SDIO function. Return
 *	value indicates if the transfer succeeded or not.
 */
extern int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr,
	void *src, int count);
/**
 *	sdio_writesb - write to a FIFO of a SDIO function
 *	@func: SDIO function to access
 *	@addr: address of (single byte) FIFO
 *	@src: buffer that contains the data to write
 *	@count: number of bytes to write
 *
 *	Writes to the specified FIFO of a given SDIO function. Return
 *	value indicates if the transfer succeeded or not.
 */
extern int sdio_writesb(struct sdio_func *func, unsigned int addr,
	void *src, int count);
/**
 *	sdio_f0_readb - read a single byte from SDIO function 0
 *	@func: an SDIO function of the card
 *	@addr: address to read
 *	@err_ret: optional status value from transfer
 *
 *	Reads a single byte from the address space of SDIO function 0.
 *	If there is a problem reading the address, 0xff is returned
 *	and @err_ret will contain the error code.
 */
extern unsigned char sdio_f0_readb(struct sdio_func *func,
	unsigned int addr, int *err_ret);
/**
 *	sdio_f0_writeb - write a single byte to SDIO function 0
 *	@func: an SDIO function of the card
 *	@b: byte to write
 *	@addr: address to write to
 *	@err_ret: optional status value from transfer
 *
 *	Writes a single byte to the address space of SDIO function 0.
 *	@err_ret will contain the status of the actual transfer.
 *
 *	Only writes to the vendor specific CCCR registers (0xF0 -
 *	0xFF) are permiited; @err_ret will be set to -EINVAL for *
 *	writes outside this range.
 */
extern void sdio_f0_writeb(struct sdio_func *func, unsigned char b,
	unsigned int addr, int *err_ret);
/**
 *	sdio_get_host_pm_caps - get host power management capabilities
 *	@func: SDIO function attached to host
 *
 *	Returns a capability bitmask corresponding to power management
 *	features supported by the host controller that the card function
 *	might rely upon during a system suspend.  The host doesn't need
 *	to be claimed, nor the function active, for this information to be
 *	obtained.
 */
extern mmc_pm_flag_t sdio_get_host_pm_caps(struct sdio_func *func);
/**
 *	sdio_set_host_pm_flags - set wanted host power management capabilities
 *	@func: SDIO function attached to host
 *
 *	Set a capability bitmask corresponding to wanted host controller
 *	power management features for the upcoming suspend state.
 *	This must be called, if needed, each time the suspend method of
 *	the function driver is called, and must contain only bits that
 *	were returned by sdio_get_host_pm_caps().
 *	The host doesn't need to be claimed, nor the function active,
 *	for this information to be set.
 */
extern int sdio_set_host_pm_flags(struct sdio_func *func, mmc_pm_flag_t flags);

int rtw_sdio_set_drvdata(struct sdio_func *func, void *data){
	sdio_set_drvdata(func, data);
	return 0;
}

void *rtw_sdio_get_drvdata(struct sdio_func *func){
	return sdio_get_drvdata(func);
}

int rtw_sdio_set_block_size(struct sdio_func *func, unsigned int blksz){
	return sdio_set_block_size(func, blksz);
}

const SDIO_BUS_OPS rtw_sdio_bus_ops = {
// 0
	NULL, //bus_probe
	NULL, //bus_remove
	sdio_enable_func,//enable_func
	sdio_disable_func,//disable_func
	sdio_register_driver,//reg_driver
	sdio_unregister_driver,//unreg_driver
// 6
	sdio_claim_irq,//claim_irq
	sdio_release_irq,//release_irq
	sdio_claim_host,//claim_host
	sdio_release_host,//release_host
// 10
	sdio_readb,//readb
	sdio_readw,//readw
	sdio_readl,//readl
// 13
	sdio_writeb,//writeb
	sdio_writew,//writew
	sdio_writel,//writel
// 16
	sdio_memcpy_fromio,//memcpy_fromio
	sdio_memcpy_toio,//memcpy_toio
	sdio_f0_readb, //f0_readb
	sdio_f0_writeb//f0_writeb
};
#endif

