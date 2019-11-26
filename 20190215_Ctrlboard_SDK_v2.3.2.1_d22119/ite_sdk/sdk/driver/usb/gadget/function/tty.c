#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <linux/os.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/util.h>
#include <linux/wait.h>

#include "tty.h"

#define TTY_READ_BUF_SIZE       16*1024

static struct tty_struct g_tty[4];
static struct tty_driver *g_tty_driver;

int tty_register_driver(struct tty_driver *driver)
{
    g_tty_driver = driver;
    return 0;
}

int tty_unregister_driver(struct tty_driver *driver)
{
    g_tty_driver = NULL;
    return 0;
}

/*-------------------------------------------------------------------------*/

/* Circular Buffer */

/*
* tty_buf_alloc
*
* Allocate a circular buffer and all associated memory.
*/
static int tty_buf_alloc(struct tty_buf *tb, unsigned size)
{
    tb->buf_buf = kmalloc(size, GFP_KERNEL);
    if (tb->buf_buf == NULL)
        return -ENOMEM;

    tb->buf_size = size;
    tb->buf_put = tb->buf_buf;
    tb->buf_get = tb->buf_buf;

    return 0;
}

/*
* tty_buf_free
*
* Free the buffer and all associated memory.
*/
static void tty_buf_free(struct tty_buf *tb)
{
    kfree(tb->buf_buf);
    tb->buf_buf = NULL;
}

/*
* tty_buf_clear
*
* Clear out all data in the circular buffer.
*/
static void tty_buf_clear(struct tty_buf *tb)
{
    tb->buf_get = tb->buf_put;
    /* equivalent to a get of all data available */
}

/*
* tty_buf_data_avail
*
* Return the number of bytes of data written into the circular
* buffer.
*/
static unsigned tty_buf_data_avail(struct tty_buf *tb)
{
    return (tb->buf_size + tb->buf_put - tb->buf_get) % tb->buf_size;
}

/*
* tty_buf_space_avail
*
* Return the number of bytes of space available in the circular
* buffer.
*/
static unsigned tty_buf_space_avail(struct tty_buf *tb)
{
    return (tb->buf_size + tb->buf_get - tb->buf_put - 1) % tb->buf_size;
}

/*
* tty_buf_put
*
* Copy data data from a user buffer and put it into the circular buffer.
* Restrict to the amount of space available.
*
* Return the number of bytes copied.
*/
static unsigned
tty_buf_put(struct tty_buf *tb, const char *buf, unsigned count)
{
    unsigned len;

    len = tty_buf_space_avail(tb);
    if (count > len)
        count = len;

    if (count == 0)
        return 0;

    len = tb->buf_buf + tb->buf_size - tb->buf_put;
    if (count > len) {
        memcpy(tb->buf_put, buf, len);
        memcpy(tb->buf_buf, buf + len, count - len);
        tb->buf_put = tb->buf_buf + count - len;
    }
    else {
        memcpy(tb->buf_put, buf, count);
        if (count < len)
            tb->buf_put += count;
        else /* count == len */
            tb->buf_put = tb->buf_buf;
    }

    return count;
}

/*
* tty_buf_get
*
* Get data from the circular buffer and copy to the given buffer.
* Restrict to the amount of data available.
*
* Return the number of bytes copied.
*/
static unsigned
tty_buf_get(struct tty_buf *tb, char *buf, unsigned count)
{
    unsigned len;

    len = tty_buf_data_avail(tb);
    if (count > len)
        count = len;

    if (count == 0)
        return 0;

    len = tb->buf_buf + tb->buf_size - tb->buf_get;
    if (count > len) {
        memcpy(buf, tb->buf_get, len);
        memcpy(buf + len, tb->buf_buf, count - len);
        tb->buf_get = tb->buf_buf + count - len;
    }
    else {
        memcpy(buf, tb->buf_get, count);
        if (count < len)
            tb->buf_get += count;
        else /* count == len */
            tb->buf_get = tb->buf_buf;
    }

    return count;
}

/*-------------------------------------------------------------------------*/

void tty_port_init(struct tty_port *port)
{
    memset(port, 0, sizeof(*port));
    if (tty_buf_alloc(&port->tty_read_buf, TTY_READ_BUF_SIZE))
        printf("tty buffer alloc fail!! %s:%d\n", __func__, __LINE__);
    spin_lock_init(&port->lock);
    init_waitqueue_head(&port->read_wait);
}

void tty_port_destroy(struct tty_port *port)
{
    tty_buf_free(&port->tty_read_buf);
}

/* in critical section */
int tty_insert_flip_string(struct tty_port *port,
    const unsigned char *chars, size_t size)
{
    int copied = 0;
    do {
        int goal = min_t(size_t, size - copied, TTY_READ_BUF_SIZE);
        int space = tty_buf_space_avail(&port->tty_read_buf);

        if (unlikely(space == 0)) {
            port->tty->throttled = 1;
			//ithPrintf("Rx throttled! \n");
            break;
        }
        space = tty_buf_put(&port->tty_read_buf, chars, goal);
        copied += space;
        chars += space;
    } while (unlikely(size > copied));
    return copied;
}

void tty_flip_buffer_push(struct tty_port *port)
{
    port->avail_data = tty_buf_data_avail(&port->tty_read_buf);
	
    if (port->avail_data)
        wake_up(&port->read_wait);
}

void tty_hangup(struct tty_struct *tty)
{
    struct tty_port *port = tty->driver_data;

    tty->wakeup = 0;
	
	tty_buf_clear(&port->tty_read_buf);
	port->avail_data = -1;
	wake_up(&port->read_wait);
}

/*-------------------------------------------------------------------------*/
#define check_status(x)  { if(status) printf(" %s() error code: %d \n", __func__ , status); }

int iteAcmTtyIsReadyEx(int index)
{
    struct tty_struct *tty = &g_tty[index];

    if (!g_tty_driver)
        return 0;

    tty->index = index;
    return g_tty_driver->ops->is_ready(tty);
}

int iteAcmTtyOpenEx(int index)
{
    int status = 0;
    struct tty_struct *tty = &g_tty[index];

    if (!g_tty_driver) {
        status = -1;
        goto end;
    }

    tty->index = index;
    spin_lock_init(&tty->lock);
    status = g_tty_driver->ops->open(tty);

end:
    check_status(status);
    return status;
}

int iteAcmTtyCloseEx(int index)
{
    int status = 0;
    struct tty_struct *tty = &g_tty[index];

    if (!g_tty_driver) {
        status = -1;
        goto end;
    }

    g_tty_driver->ops->close(tty);

end:
    check_status(status);
    return status;
}

int iteAcmTtyReadEx(int index, char *buf, int len)
{
    int count;
    struct tty_struct *tty = &g_tty[index];
    struct tty_port *port = tty->driver_data;
    
    if (!g_tty_driver)
        return 0;

    wait_event(port->read_wait, (port->avail_data || (port->tty == NULL)));

    if (port->tty == NULL)
		return 0;

	if (port->avail_data == -1) /* hang up */
		return 0;

    spin_lock_irq(&tty->lock);
    count = tty_buf_data_avail(&port->tty_read_buf);
    if (count > 0) {
        if (count > len)
            count = len;

        count = tty_buf_get(&port->tty_read_buf, buf, count);
		if (tty->throttled) {
			tty->throttled = 0;
			g_tty_driver->ops->unthrottle(tty);
		}
    }
    port->avail_data = tty_buf_data_avail(&port->tty_read_buf);
    spin_unlock_irq(&tty->lock);

    return count;
}

int iteAcmTtyWriteEx(int index, const unsigned char *buf, int len)
{
    int count = 0;
    struct tty_struct *tty = &g_tty[index];

    if (!g_tty_driver)
        return -1;

    count = g_tty_driver->ops->write(tty, buf, len);

    return count;
}

int iteAcmTtyGetWriteRoomEx(int index)
{
    int room = 0;
    struct tty_struct *tty = &g_tty[index];

    if (!g_tty_driver)
        return -1;

    room = g_tty_driver->ops->write_room(tty);

    return room;
}

int iteAcmTtyPutCharEx(int index, unsigned char c)
{
    int status = 0;
    struct tty_struct *tty = &g_tty[index];

    if (!g_tty_driver)
        return -1;

    status = g_tty_driver->ops->put_char(tty, c);

    return status;
}

void iteAcmTtyFlushCharsEx(int index)
{
    struct tty_struct *tty = &g_tty[index];

    if (!g_tty_driver)
        return;

    g_tty_driver->ops->flush_chars(tty);

    return;
}
