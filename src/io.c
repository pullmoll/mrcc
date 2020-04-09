// mrcc - A C Compiler system on MapReduce
// Zhiqiang Ma, https://www.ericzma.com

#include <stdarg.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include <fcntl.h>

#include <sys/fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>

#include <signal.h>

#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/poll.h>

#include "utils.h"
#include "io.h"
#include "trace.h"

/**
 * @brief Calls select() to block until the specified fd becomes writeable
 * or has an error condition, or the timeout expires.
 * @param fd file descriptor.
 * @param timeout timeout in seconds.
 * @return 0 on success, or EXIT_IO_ERROR.
 */
int
select_for_write(int fd, int timeout)
{
    fd_set write_fds;
    fd_set except_fds;
    int rs;

    struct timeval tv;

    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    while (1) {
        FD_ZERO(&write_fds);
        FD_ZERO(&except_fds);
        FD_SET(fd, &write_fds);
        FD_SET(fd, &except_fds);
        rs_trace("select for write on fd%d", fd);

        rs = select(fd + 1, NULL, &write_fds, &except_fds, &tv);

        if (rs == -1 && EINTR == errno) {
            rs_trace("select was interrupted");
            continue;
        }

        if (rs == -1) {
            rs_log_error("select failed: %s", strerror(errno));
            return EXIT_IO_ERROR;
        }

        if (rs == 0) {
            rs_log_error("IO timeout");
            return EXIT_IO_ERROR;

            if (FD_ISSET(fd, &except_fds)) {
                rs_trace("error condition on fd%d", fd);
                /*
           * Don't fail here; we couldn't give a good error
           * message, because we don't know what the error
           * condition is.  Instead just return 0 (success),
           * indicating that the select has successfully finished.
           * The next call to write() for that fd will fail but
           * will also set errno properly so that we can give a
           * good error message at that point.
           */
            }
            return 0;
        }
    }
}

/**
 * @brief Write bytes to an fd.
 * Keep writing until we're all done or something goes wrong.
 * @param fd file descriptor.
 * @param buf buffer to write.
 * @param len number of bytes to write;
 * @return 0 or exit code.
 */
int writex(int fd, const void *buf, size_t len)
{
/*
 * Timeout for all IO other than opening connections.  Much longer, because
 * compiling files can take a long time. *
 */
    const int _io_timeout = 300; /* seconds */

    ssize_t r;
    int ret;

    while (len > 0) {
        r = write(fd, buf, len);

        if (r == -1 && EAGAIN == errno) {
            if ((ret = select_for_write(fd, _io_timeout)))
                return ret;
            continue;
        }
        if (r == -1 && EINTR == errno) {
            continue;
        }
        if (r == -1) {
            rs_log_error("failed to write: %s", strerror(errno));
            return EXIT_IO_ERROR;
        }
        buf = &((char *) buf)[r];
        len -= r;
    }

    return 0;
}


int mrcc_close(int fd)
{
    if (close(fd) != 0) {
        rs_log_error("failed to close fd%d: %s", fd, strerror(errno));
        return EXIT_IO_ERROR;
    }
    return 0;
}



/**
 * Open a file for read, and also put its size into @p fsize.
 *
 * If the file does not exist, then returns 0, but @p ifd is -1 and @p
 * fsize is zero.  If @p fsize is zero, the caller should not try to
 * read from the file.
 *
 * This strange behaviour for ENOENT is useful because if there is
 * e.g. no output file from the compiler, we don't want to abort, but
 * rather just send nothing.  The receiver has the corresponding
 * behaviour of not creating zero-length files.
 *
 * Using fstat() helps avoid a race condition -- not a security issue,
 * but possibly a failure.  Shouldn't be very likely though.
 *
 * The caller is responsible for closing @p ifd.
 */
int open_read(const char *fname, int *ifd, off_t *fsize)
{
    struct stat buf;

    *ifd = open(fname, O_RDONLY|O_BINARY);
    if (*ifd == -1) {
        int save_errno = errno;
        if (ENOENT == save_errno) {
            /* that's OK, just assume it's empty */
            *fsize = 0;
            return 0;
        } else {
            rs_log_error("failed to open %s: %s", fname, strerror(save_errno));
            return EXIT_IO_ERROR;
        }
    }

    if (fstat(*ifd, &buf) == -1) {
        rs_log_error("fstat %s failed: %s", fname, strerror(errno));
        close(*ifd);
        return EXIT_IO_ERROR;
    }

    *fsize = buf.st_size;

    return 0;
}

/**
 * @brief Copy a file's contents to a file descriptor.
 * @param in_fname filename to open.
 * @param out_fd unused.
 * @return 0 on success, or EXIT_IO_ERROR.
 */
int
copy_file_to_fd(const char *in_fname, int out_fd)
{
    off_t len;
    int ifd;
    int ret;

    ret = open_read(in_fname, &ifd, &len);
    if (ret)
        return ret;

#ifdef HAVE_SENDFILE
    ret = pump_sendfile(out_fd, ifd, (size_t) len);
#else
    // deleted here
    // ret = pump_readwrite(out_fd, ifd, (size_t) len);
#endif

    if (ret) {
        close(ifd);
        return ret;
    }
    return 0;
}


