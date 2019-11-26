#ifndef _LINUX_TTY_H
#define _LINUX_TTY_H



struct tty_struct {
    int index;
    void *driver_data;
    spinlock_t lock;

    int wakeup : 1;
    int throttled : 1;
};

/* circular buffer */
struct tty_buf {
    unsigned		buf_size;
    char			*buf_buf;
    char			*buf_get;
    char			*buf_put;
};

struct tty_port {
    struct tty_struct *tty;
    struct tty_buf tty_read_buf;
    int count;
    int avail_data;
    
    spinlock_t lock;
    wait_queue_head_t	read_wait;
};

void tty_port_init(struct tty_port *port);
void tty_port_destroy(struct tty_port *port);


struct tty_operations {
    int(*is_ready)(struct tty_struct * tty);  // Irene Lin
    int(*open)(struct tty_struct * tty);
    void(*close)(struct tty_struct * tty);
    int(*write)(struct tty_struct * tty,
        const unsigned char *buf, int count);
    int(*put_char)(struct tty_struct *tty, unsigned char ch);
    void(*flush_chars)(struct tty_struct *tty);
    int(*write_room)(struct tty_struct *tty);
    int(*chars_in_buffer)(struct tty_struct *tty);
    int(*break_ctl)(struct tty_struct *tty, int state);
	void (*unthrottle)(struct tty_struct * tty);
};

struct tty_driver {
    unsigned int num;
#if 0
    struct tty_struct **ttys;
#endif
    const struct tty_operations *ops;
};

int tty_register_driver(struct tty_driver *driver);
int tty_unregister_driver(struct tty_driver *driver);

static inline void tty_set_operations(struct tty_driver *driver,
    const struct tty_operations *op)
{
    driver->ops = op;
}

static inline void tty_wakeup(struct tty_struct *tty)
{
    tty->wakeup = 1;
}

void tty_hangup(struct tty_struct *tty);

#define tty_throttled(tty)  (tty->throttled)


int tty_insert_flip_string(struct tty_port *port,
    const unsigned char *chars, size_t size);

void tty_flip_buffer_push(struct tty_port *port);


#endif
