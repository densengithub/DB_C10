#include <sys/ioctl.h>
#include <assert.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include "ite/itp.h"
#include "project.h"
#include "external.h"
#include "scene.h"

sem_t ExternaltaskSem;
static mqd_t extInQueue = -1;
static mqd_t extOutQueue = -1;
static mqd_t extQueue = -1;
static pthread_t extTask;
static volatile bool extQuit;
ExternalEvent_layer ev_layer2 = {0};
bool light_set_flag = false;

static void* ExternalCheckTask(void* arg)
{
	ExternalEvent ev_user = {0};
	ExternalEvent ev = {0};
	int ret = 0, num = 0, j = 0, i = 0;

	printf("Run ExternalCheckTask!\n");
	// while (!SceneReadReady())
	// {
	// 	usleep(1 * 1000);
	// }
	while (1)
	{
		//  sem_wait(&ExternaltaskSem);
		 ret = ExternalReceive(&ev_user);
		 if (ret)
		 {
			 //  printf("ev_user=%d..\t\n",ev_user.type);
			 // if (ev_layer2.type == EXTERNAL_UPDATA)
			 if (ev_user.type == EXTERNAL_UPDATA)
			 {				
				//  printf("sizeof(ScooterInfo_Typedef)=%d..\t\n", sizeof(ScooterInfo_Typedef));
				//  niu_data_read_value_loc(NIU_DB, NIU_ID_DB_LOC_STATUS, (ev_layer2.layer_displaydata.DisplayDatabuffer), sizeof(ScooterInfo_Typedef));
				//  for (i = 0; i < (NIU_ID_DB_LOC_MAX - NIU_ID_DB_LOC_STATUS); i++)
				//  {
				// 	 num = niu_data_get_datalen_loc(NIU_DB, NIU_ID_DB_LOC_STATUS + i);
				// 	 // printf("rd_id + i=%d,num=%d....\n", NIU_ID_DB_LOC_STATUS + i, num);
				// 	 niu_data_read_value_loc(NIU_DB, NIU_ID_DB_LOC_STATUS + i, (ev_layer2.layer_displaydata.DisplayDatabuffer + j), num);
				// 	 j += num;
				//  }
				#if 0
				if (SceneReadReady())
				{
					if (memcmp(ev_layer2.layer_displaydata.DisplayDatabuffer, local_niu_value_loc.db_loc.buff, sizeof(NIU_DATA_BODY_DB_LOC)) != 0)
					{
						memcpy(ev_layer2.layer_displaydata.DisplayDatabuffer, niu_data_get_pdata_loc(NIU_DB, NIU_ID_DB_LOC_STATUS), sizeof(NIU_DATA_BODY_DB_LOC));

						ev.type = EXTERNAL_UPDATA;
						if (Layer_ExternalSend(&ev) == -1)
						{
							// printf("ExternalSend(&ev) fail...\n");
						}
						else
						{
							// printf("ExternalSend(&ev) success...\n");
						}
					}
				}
				#else

				if (memcmp(ev_layer2.layer_displaydata.DisplayDatabuffer, local_niu_value_loc.db_loc.buff, sizeof(NIU_DATA_BODY_DB_LOC)) != 0)
				{
					memcpy(ev_layer2.layer_displaydata.DisplayDatabuffer, niu_data_get_pdata_loc(NIU_DB, NIU_ID_DB_LOC_STATUS), sizeof(NIU_DATA_BODY_DB_LOC));
					
					if (SceneReadReady())
					{
						ev.type = EXTERNAL_UPDATA;
						if (Layer_ExternalSend(&ev) == -1)
						{
							printf("ExternalSend(&ev) fail...\n");
						}
						// else
						// {
						// 	printf("ExternalSend(&ev) success...\n");
						// }
					}
				}

#endif

				//  for (i = 0; i < sizeof(ScooterInfo_Typedef); i++)
				//  {
				//      printf("Disp[%d]=0x%02x\t\n", i, ev_layer2.layer_displaydata.DisplayDatabuffer[i]);
				//  }
				 light_set_flag = true;
				//  printf("light=%d\t\n", ev_layer2.layer_displaydata.ScooterInfo.light_percent);
				//  ScreenSetBrightness(100 - ev_layer2.layer_displaydata.ScooterInfo.light_percent);
			 }
			 else if(ev_user.type == EXTERNAL_TESTMODE)
			 {
				 
				 memcpy(&ev_layer2.layer_displaydata.DisplayDatabuffer[2], ev_user.buf1, (sizeof(DashBoardInfo_typedef)-2));
				//  for (i = 0; i < (sizeof(DashBoardInfo_typedef)-2); i++)
				//  {
				// 	 printf("buf1[%d] = 0x%02x...", i, ev_user.buf1[i]);
				// 	 printf("Databuffer[%d] = 0x%02x...\t\n", 2 + i, ev_layer2.layer_displaydata.DisplayDatabuffer[2 + i]);
				//  }
				 if (SceneReadReady())
				 {
					 ev.type = EXTERNAL_TESTMODE;
					 if (Layer_ExternalSend(&ev) == -1)
					 {
						  printf("Layer_ev fail...\t\n");
					 }
					//  else
					//  {
					// 	  printf("Layer_ev success...\t\n");
					//  }
				 }	
				 light_set_flag = true;			
			 }
		 }
		//  else
		//  {
		// 	 printf("Layer_ExternalReceive no data!\n");
		//  }
		 if (light_set_flag)
		 {
			//  printf("light=%d\t\n", ev_layer2.layer_displaydata.ScooterInfo.light_percent);
			//  ScreenSetBrightness(100 - ev_layer2.layer_displaydata.ScooterInfo.light_percent);
			 ScreenSetBrightness(ev_layer2.layer_displaydata.ScooterInfo.light_percent);
		 }
		 usleep(33 * 1000);
	 }
	 return NULL;
}

void ExternalInit(void)
{
	int res = 0;
	struct mq_attr qattr;
	sem_init(&ExternaltaskSem, 0, 0);
	qattr.mq_flags = 0;
	qattr.mq_maxmsg = EXT_MAX_QUEUE_SIZE;
	qattr.mq_msgsize = sizeof(ExternalEvent);	
	sem_init(&ExternaltaskSem, 0, 0);
	#ifndef DEMO_BUG_RUN
    write_loc_data_key = xSemaphoreCreateMutex();
	#endif
	extQueue = mq_open("external_com", O_CREAT | O_NONBLOCK, 0644, &qattr);
    assert(extQueue != -1);
	if(extQueue == -1)
	{
		printf("Create external_in fail!.......\n");
	}
	// else
	// {
	// 	printf("Create external_in success!.......\n");
	// }

    // extInQueue = mq_open("external_in", O_CREAT | O_NONBLOCK, 0644, &qattr);
    // assert(extInQueue != -1);
	// if(extInQueue == -1)
	// {
	// 	printf("Create external_in fail!.......\n");
	// }
	// else
	// {
	// 	printf("Create external_in sucess!.......\n");
	// }
	

    extOutQueue = mq_open("external_out", O_CREAT | O_NONBLOCK, 0644, &qattr);
    assert(extOutQueue != -1);

	extQuit = false;

	res = pthread_create(&extTask, NULL, ExternalCheckTask, NULL);
	if (res)
	{
		printf("[TouchPanel]%s() L#%ld: ERROR, create ExternalCheckTask() thread fail! res=%ld\n", res);
	}
   return;
}


void ExternalExit(void)
{
    extQuit = true;

    pthread_join(extTask, NULL);

    mq_close(extOutQueue);
    extOutQueue = -1;
	 mq_close(extQueue);
    extQueue = -1;    
}


int ExternalReceive(ExternalEvent* ev)
{
    assert(ev);	

    // if (extQuit)
    //     return 0;  

    if (mq_receive(extQueue, (char*)ev, sizeof(ExternalEvent), 0) > 0)
        return 1;

    return 0;
}

int ExternalSend(ExternalEvent* ev)
{
    assert(ev);

    // if (extQuit)
    //     return -1;

    // return mq_send(extOutQueue, (char*)ev, sizeof(ExternalEvent), 0);
	return mq_send(extQueue, (const char*)ev, sizeof(ExternalEvent), 0);
}

int Layer_ExternalSend(ExternalEvent* ev)
{
    assert(ev);

    // if (extQuit)
    //     return -1;
   
	return mq_send(extOutQueue, (const char*)ev, sizeof(ExternalEvent), 0);
}

int Layer_ExternalReceive(ExternalEvent* ev)
{
    assert(ev);	

    // if (extQuit)
    //     return 0;  

    if (mq_receive(extOutQueue, (char*)ev, sizeof(ExternalEvent), 0) > 0)
        return 1;

    return 0;
}
