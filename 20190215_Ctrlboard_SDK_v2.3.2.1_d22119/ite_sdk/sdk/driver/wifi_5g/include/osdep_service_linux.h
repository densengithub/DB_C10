/******************************************************************************
 *
 * Copyright(c) 2007 - 2013 Realtek Corporation. All rights reserved.
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
 *
 ******************************************************************************/
#include <autoconf.h>
#include "ports/ite_port.h" // eason

#ifdef PLATFORM_LINUX
#ifndef __OSDEP_LINUX_SERVICE_H_
#define __OSDEP_LINUX_SERVICE_H_

#if 1
    #include <stdio.h>
	#include <linux/spinlock.h>
	#include <linux/os.h>
	#include <linux/list.h>
    #include <linux/atomic.h>
	#include <ports/ite_skbuf.h>
    #include <ports/ite_ndis.h>
	//#include <linux/timer.h>
#else
	#include <linux/version.h>
	#include <linux/spinlock.h>
	#include <linux/compiler.h>
	#include <linux/kernel.h>
	#include <linux/errno.h>
	#include <linux/init.h>
	#include <linux/slab.h>
	#include <linux/module.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,5))
	#include <linux/kref.h>
#endif
	//#include <linux/smp_lock.h>
	#include <linux/netdevice.h>
	#include <linux/skbuff.h>
	#include <linux/circ_buf.h>
	#include <asm/uaccess.h>
	#include <asm/byteorder.h>
	#include <asm/atomic.h>
	#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26))
	#include <asm/semaphore.h>
#else
	#include <linux/semaphore.h>
#endif
	#include <linux/sem.h>
	#include <linux/sched.h>
	#include <linux/etherdevice.h>
	#include <linux/wireless.h>
	#include <net/iw_handler.h>
	#include <linux/if_arp.h>
	#include <linux/rtnetlink.h>
	#include <linux/delay.h>
	#include <linux/interrupt.h>	// for struct tasklet_struct
	#include <linux/ip.h>
	#include <linux/kthread.h>
	#include <linux/list.h>
	#include <linux/vmalloc.h>

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,5,41))
	#include <linux/tqueue.h>
#endif

#ifdef RTK_DMP_PLATFORM
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,12))
	#include <linux/pageremap.h>
#endif
	#include <asm/io.h>
#endif

#ifdef CONFIG_NET_RADIO
	#define CONFIG_WIRELESS_EXT
#endif

	/* Monitor mode */
	#include <net/ieee80211_radiotap.h>

#ifdef CONFIG_IOCTL_CFG80211
/*	#include <linux/ieee80211.h> */
	#include <net/cfg80211.h>
#endif //CONFIG_IOCTL_CFG80211

#ifdef CONFIG_TCP_CSUM_OFFLOAD_TX
	#include <linux/in.h>
	#include <linux/udp.h>
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
	#include <linux/earlysuspend.h>
#endif //CONFIG_HAS_EARLYSUSPEND

#ifdef CONFIG_EFUSE_CONFIG_FILE
	#include <linux/fs.h>
#endif

#ifdef CONFIG_USB_HCI
	#include <linux/usb.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21))
	#include <linux/usb_ch9.h>
#else
	#include <linux/usb/ch9.h>
#endif
#endif
#endif


#if 0//def CONFIG_BT_COEXIST_SOCKET_TRX
	#include <net/sock.h>
	#include <net/tcp.h>
	#include <linux/udp.h>
	#include <linux/in.h>
	#include <linux/netlink.h>
#endif //CONFIG_BT_COEXIST_SOCKET_TRX

#if 1
#define   KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
/* emulate a modern version */
#define LINUX_VERSION_CODE KERNEL_VERSION(2, 6, 35)
#endif

#ifdef CONFIG_USB_HCI
	typedef struct usb_device *  PUSB_DEV;
	typedef struct usb_driver *  PUSB_DRIVER;
	typedef struct usb_interface *  PUSB_INTF;
	typedef struct urb *  PURB;
#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,22))
#ifdef CONFIG_USB_SUSPEND
#define CONFIG_AUTOSUSPEND	1
#endif
#endif
#endif

#define mod_timer(t, expire)    ite_mod_timer((t), (expire-jiffies))

#if 1
	#define _sema	sem_t
#else
	typedef struct 	semaphore _sema;
#endif

	typedef	spinlock_t	_lock;
#if 1//(LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37))
	struct s_mutex {
		pthread_mutex_t mutex;
	};

	typedef struct s_mutex 		_mutex;
#else
	typedef struct semaphore	_mutex;
#endif

	typedef unsigned long	osdepTickType;
	typedef void*	_timerHandle;
	typedef struct timer_list _timer;

typedef void*	_xqueue;

	struct	__queue	{
		struct	list_head	queue;
		_lock	lock;
	};

	typedef	struct sk_buff	_pkt;
	typedef unsigned char	_buffer;

	typedef struct	__queue	_queue;
	typedef struct	list_head	_list;
	typedef	int	_OS_STATUS;
	//typedef u32	_irqL;
	typedef unsigned long _irqL;
	typedef	struct	net_device * _nic_hdl;

	typedef void*		_thread_hdl_;
	//typedef int		thread_return;
    typedef void*		thread_return;
	typedef void*	thread_context;

#if 1
#define rtw_thread_exit()   do { return NULL; } while(0)
#else
	#define rtw_thread_exit() complete_and_exit(NULL, 0)
#endif

	typedef void timer_hdl_return;
	typedef void* timer_hdl_context;

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,41))
	typedef struct work_struct _workitem;
#else
	typedef struct tq_struct _workitem;
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
	#define DMA_BIT_MASK(n) (((n) == 64) ? ~0ULL : ((1ULL<<(n))-1))
#endif

#if 1//(LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
// Porting from linux kernel, for compatible with old kernel.
static inline unsigned char *skb_tail_pointer(const struct sk_buff *skb)
{
	return skb->tail;
}

static inline void skb_reset_tail_pointer(struct sk_buff *skb)
{
	skb->tail = skb->data;
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
	skb->tail = skb->data + offset;
}

static inline unsigned char *skb_end_pointer(const struct sk_buff *skb)
{
	return skb->end;
}
#endif

__inline static _list *get_next(_list	*list)
{
	return list->next;
}

__inline static _list	*get_list_head(_queue	*queue)
{
	return (&(queue->queue));
}


#define LIST_CONTAINOR(ptr, type, member) \
        ((type *)((char *)(ptr)-(SIZE_T)(&((type *)0)->member)))


__inline static void _enter_critical(_lock *plock, _irqL *pirqL)
{
	spin_lock_irqsave(plock, *pirqL);
}

__inline static void _exit_critical(_lock *plock, _irqL *pirqL)
{
	spin_unlock_irqrestore(plock, *pirqL);
}

__inline static void _enter_critical_ex(_lock *plock, _irqL *pirqL)
{
	spin_lock_irqsave(plock, *pirqL);
}

__inline static void _exit_critical_ex(_lock *plock, _irqL *pirqL)
{
	spin_unlock_irqrestore(plock, *pirqL);
}

__inline static void _enter_critical_bh(_lock *plock, _irqL *pirqL)
{
	spin_lock_bh(plock);
}

__inline static void _exit_critical_bh(_lock *plock, _irqL *pirqL)
{
	spin_unlock_bh(plock);
}

__inline static int _enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	int ret = 0;
#if 0//(LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37))
	//mutex_lock(pmutex);
	ret = mutex_lock_interruptible(pmutex);
#else
	//ret = down_interruptible(pmutex);
	ret = mutex_lock(pmutex); //eason edit
#endif
	return ret;
}


__inline static void _exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
#if 0//(LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37))
		mutex_unlock(pmutex);
#else
		//up(pmutex);
		mutex_unlock(pmutex); //eason edit
#endif
}

__inline static void rtw_list_delete(_list *plist)
{
	list_del_init(plist);
}

#define RTW_TIMER_HDL_ARGS void *FunctionContext

__inline static void _init_timer(_timer *ptimer,_nic_hdl nic_hdl,void *pfunc,void* cntx)
{
	//setup_timer(ptimer, pfunc,(u32)cntx);
	ptimer->function = pfunc;
	ptimer->data = (unsigned long)cntx;
	init_timer(ptimer);
}

/*__inline static void _set_timer(_timer *ptimer,u32 delay_time)
{
	mod_timer(ptimer , (jiffies+(delay_time*HZ/1000)));
}*/

__inline static void _set_timer(_timer *ptimer,u32 delay_time)
{
	if(!ptimer)
	printk("ptimer is NULL pointer...\n");
	//mod_timer(ptimer , (jiffies+(delay_time*HZ/1000)));	 //James: need implement
	del_timer_sync(ptimer);
	ptimer->expires = jiffies;//(jiffies+delay_time);
	ptimer->delayT = delay_time;
	add_timer(ptimer);
}

__inline static void _cancel_timer(_timer *ptimer,u8 *bcancelled)
{
	del_timer_sync(ptimer);
	*bcancelled=  _TRUE;//TRUE ==1; FALSE==0
}


#if 0 //eason
__inline static void _init_workitem(_workitem *pwork, void *pfunc, PVOID cntx)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
	INIT_WORK(pwork, pfunc);
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,41))
	INIT_WORK(pwork, pfunc,pwork);
#else
	INIT_TQUEUE(pwork, pfunc,pwork);
#endif
}

__inline static void _set_workitem(_workitem *pwork)
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,41))
	schedule_work(pwork);
#else
	schedule_task(pwork);
#endif
}

__inline static void _cancel_workitem_sync(_workitem *pwork)
{
#if 0//(LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,22))
	cancel_work_sync(pwork);
//#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,41))
	flush_scheduled_work();
//#else
	flush_scheduled_tasks();
#endif
}
#endif
//
// Global Mutex: can only be used at PASSIVE level.
//

#define ACQUIRE_GLOBAL_MUTEX(_MutexCounter)                              \
{                                                               \
	while (atomic_inc_return((atomic_t *)&(_MutexCounter)) != 1)\
	{                                                           \
		atomic_dec((atomic_t *)&(_MutexCounter));        \
		msleep(10);                          \
	}                                                           \
}

#define RELEASE_GLOBAL_MUTEX(_MutexCounter)                              \
{                                                               \
	atomic_dec((atomic_t *)&(_MutexCounter));        \
}

static inline int rtw_netif_queue_stopped(struct net_device *pnetdev)
{
#if 0//(LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,35))
	return (netif_tx_queue_stopped(netdev_get_tx_queue(pnetdev, 0)) &&
		netif_tx_queue_stopped(netdev_get_tx_queue(pnetdev, 1)) &&
		netif_tx_queue_stopped(netdev_get_tx_queue(pnetdev, 2)) &&
		netif_tx_queue_stopped(netdev_get_tx_queue(pnetdev, 3)) );
#else
	return netif_queue_stopped(pnetdev);
#endif
}

static inline void rtw_netif_wake_queue(struct net_device *pnetdev)
{
#if 0//(LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,35))
	netif_tx_wake_all_queues(pnetdev);
#else
	netif_wake_queue(pnetdev);
#endif
}

static inline void rtw_netif_start_queue(struct net_device *pnetdev)
{
#if 0//(LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,35))
	netif_tx_start_all_queues(pnetdev);
#else
	netif_start_queue(pnetdev);
#endif
}

static inline void rtw_netif_stop_queue(struct net_device *pnetdev)
{
#if 0//(LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,35))
	netif_tx_stop_all_queues(pnetdev);
#else
	netif_stop_queue(pnetdev);
#endif
}

static inline void rtw_merge_string(char *dst, int dst_len, char *src1, char *src2)
{
	int	len = 0;
	len += snprintf(dst+len, dst_len - len, "%s", src1);
	len += snprintf(dst+len, dst_len - len, "%s", src2);
}

#if 0//(LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27))
#define rtw_signal_process(pid, sig) kill_pid(find_vpid((pid)),(sig), 1)
#else //(LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27))
#define rtw_signal_process(pid, sig) kill_proc((pid), (sig), 1)
#endif //(LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27))


// Suspend lock prevent system from going suspend
#ifdef CONFIG_WAKELOCK
#include <linux/wakelock.h>
#elif defined(CONFIG_ANDROID_POWER)
#include <linux/android_power.h>
#endif

// limitation of path length
#define PATH_LENGTH_MAX PATH_MAX

//Atomic integer operations
#define ATOMIC_T atomic_t

#define rtw_netdev_priv(netdev) ( ((struct rtw_netdev_priv_indicator *)netdev_priv(netdev))->priv )

#define NDEV_FMT "%s"
#define NDEV_ARG(ndev) ndev->name
#define ADPT_FMT "%s"
#define ADPT_ARG(adapter) adapter->pnetdev->name
#define FUNC_NDEV_FMT "%s(%s)"
#define FUNC_NDEV_ARG(ndev) __func__, ndev->name
#define FUNC_ADPT_FMT "%s(%s)"
#define FUNC_ADPT_ARG(adapter) __func__, adapter->pnetdev->name

struct rtw_netdev_priv_indicator {
	void *priv;
	u32 sizeof_priv;
};
struct net_device *rtw_alloc_etherdev_with_old_priv(int sizeof_priv, void *old_priv);
extern struct net_device * rtw_alloc_etherdev(int sizeof_priv);

#define STRUCT_PACKED __attribute__ ((packed))

//useless for Linux OS
#define XMIT_STACKSIZE			0
#define CMD_STACKSIZE			0
#define HALXMIT_STACKSIZE		0

#define TASK_PRORITY_LOW  				0
#define TASK_PRORITY_MIDDLE   			0
#define TASK_PRORITY_HIGH    			0
#define TASK_PRORITY_SUPER    			0
#endif
#endif

