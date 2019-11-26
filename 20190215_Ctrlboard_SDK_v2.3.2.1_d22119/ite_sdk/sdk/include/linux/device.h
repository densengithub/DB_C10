#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <linux/os.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct module;

struct device_type {
	const char *name;
};

struct device_driver {
	const char		*name;
	struct module		*owner;
};

struct device {
    const char *name;
	struct device_driver *driver;
	void *driver_data;
    void (*release)(struct device *dev);
};

static inline int dev_set_name(struct device *dev, const char *name)
{
    dev->name = name;
	return 0;
}

static inline const char *dev_name(const struct device *dev)
{
    return dev->name;
}

static inline void *dev_get_drvdata(const struct device *dev)
{
	return dev->driver_data;
}

static inline void dev_set_drvdata(struct device *dev, void *data)
{
	dev->driver_data = data;
}

static inline void device_initialize(struct device *dev)
{
}

static inline int device_add(struct device *dev)
{
    return 0;
}

static inline void device_unregister(struct device *dev)
{
    if (dev->release)
        dev->release(dev);
}

static inline int dev_to_node(struct device *dev)
{
	return -1;
}
static inline void set_dev_node(struct device *dev, int node)
{
}

static inline void device_lock(struct device *dev)
{
}

static inline int device_trylock(struct device *dev)
{
}

static inline void device_unlock(struct device *dev)
{
}

#define device_del(x)   do {} while(0)
#define put_device(x)   do {} while(0)
#ifndef get_device
#define get_device(x)	(x)
#endif




#ifdef __cplusplus
}
#endif

#endif // _DEVICE_H_
