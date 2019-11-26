#ifndef ITP_UNISTD_H
#define ITP_UNISTD_H

#include <sys/unistd.h>
#include <getopt.h>
#include <io.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define access _access

#ifdef CFG_WIN32_FS_HW
    int chmod(const char *path, mode_t mode);
#else
    #define chmod _chmod
#endif // CFG_WIN32_FS_HW

int	getdtablesize(void);
int ftruncate(int file, off_t length);
pid_t setsid(void);
int setuid(uid_t uid);
int usleep(useconds_t useconds);
int fchmod(int fildes, mode_t mode);
int fsync(int fd);
int fdatasync(int fd);

#ifdef __cplusplus
}
#endif

#endif // ITP_UNISTD_H
