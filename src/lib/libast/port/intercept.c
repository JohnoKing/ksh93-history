/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*          Copyright (c) 1985-2013 AT&T Intellectual Property          *
*                      and is licensed under the                       *
*                 Eclipse Public License, Version 1.0                  *
*                    by AT&T Intellectual Property                     *
*                                                                      *
*                A copy of the License is available at                 *
*          http://www.eclipse.org/org/documents/epl-v10.html           *
*         (with md5 checksum b35adb5213ca9657e911e9befb180842)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                     Phong Vo <phongvo@gmail.com>                     *
*                                                                      *
***********************************************************************/
#pragma prototyped

#define _AST_INTERCEPT_IMPLEMENT		2

#include "astlib.h"

#include <aso.h>
#include <error.h>
#include <ast_ioctl.h>
#include <sys/socket.h>

static const char	dot[] = ".";

Ast_global_t		ast_global =
{
	"libast",
	20130717,
	0,
	0,
};

#define LOCAL(f)	(ast.f)		/* to thread or not		*/
#define GLOBAL(f)	(ast_global.f)	/* process global		*/

#define RESTART(r,f)	\
	do { \
		uint32_t	serial; \
		do \
		{ \
			serial = asoget32(&GLOBAL(restart)); \
		} while ((r = f) == -1 && errno == EINTR && serial == astserial(AST_SERIAL_RESTART, AST_SERIAL_get)); \
	} while (0)

/* ast global/local data support */

uint32_t
astserial(int serial, uint32_t op)
{
	uint32_t	r;
	uint32_t*	v;

	switch (serial)
	{
	case AST_SERIAL_ENVIRON:
		v = &LOCAL(env_serial);
		break;
	case AST_SERIAL_LOCALE:
		v = &LOCAL(locale.serial);
		break;
	case AST_SERIAL_RESTART:
		v = &GLOBAL(restart);
		break;
	default:
		return 0;
	}
	r = asoget32(v);
	switch (op)
	{
	case AST_SERIAL_get:
		switch (r)
		{
		case AST_SERIAL_except:
			asocas32(v, r, AST_SERIAL_always);
			break;
		}
		break;
	case AST_SERIAL_inc:
		switch (r)
		{
		case AST_SERIAL_always:
			break;
		case AST_SERIAL_except:
			asocas32(v, r, AST_SERIAL_always);
			r = asoget32(v);
			break;
		case AST_SERIAL_max:
			if (asocas32(v, r, 1) == r)
			{
				r = asoget32(v);
				break;
			}
			/*FALLTHROUGH*/
		default:
			r = asoinc32(v) + 1;
			break;
		}
		break;
	case AST_SERIAL_always:
	case AST_SERIAL_except:
		asocas32(v, r, op);
		break;
	}
	return r;
}

/* *at() intercepts */

int
ast_faccessat(int cwd, const char* path, int mode, int flags)
{
	int	r;

	RESTART(r, faccessat(cwd, path, mode, flags));
	return r;
}

int
ast_fchmodat(int cwd, const char* path, mode_t mode, int flags)
{
	int	r;

	RESTART(r, fchmodat(cwd, path, mode, flags));
	return r;
}

int
ast_fchownat(int cwd, const char* path, uid_t owner, gid_t group, int flags)
{
	int	r;

	RESTART(r, fchownat(cwd, path, owner, group, flags));
	return r;
}

int
ast_fstatat(int cwd, const char* path, struct stat* st, int flags)
{
	int	r;

	RESTART(r, fstatat(cwd, path, st, flags));
	return r;
}

int
ast_linkat(int cwd, const char* path, int lwd, const char* linkpath, int flags)
{
	int	r;

	RESTART(r, linkat(cwd, path, lwd, linkpath, flags));
	return r;
}

int
ast_mkdirat(int cwd, const char* path, mode_t mode)
{
	int	r;

	RESTART(r, mkdirat(cwd, path, mode));
	return r;
}

int
ast_mkfifoat(int cwd, const char* path, mode_t mode)
{
	int	r;

	RESTART(r, mkfifoat(cwd, path, mode));
	return r;
}

int
ast_mknodat(int cwd, const char* path, mode_t mode, dev_t dev)
{
	int	r;

	RESTART(r, mknodat(cwd, path, mode, dev));
	return r;
}

int
ast_openat(int cwd, const char* path, int flags, ...)
{
	int	r;
#if _ast_O_LOCAL && O_CLOEXEC >= _ast_O_LOCAL
	int	o_cloexec;
#endif
#if _ast_O_LOCAL && O_DIRECTORY >= _ast_O_LOCAL
	int	o_directory;
#endif
	mode_t	mode;
	va_list	ap;

	va_start(ap, flags);
	mode = (flags & O_CREAT) ? (mode_t)va_arg(ap, int) : (mode_t)0;
	va_end(ap);
#if _ast_O_LOCAL && O_CLOEXEC >= _ast_O_LOCAL
	if (flags & O_CLOEXEC)
	{
		flags &= ~O_CLOEXEC;
		o_cloexec = 1;
	}
	else
		o_cloexec = 0;
#endif
#if _ast_O_LOCAL && O_DIRECTORY >= _ast_O_LOCAL
	if (flags & O_DIRECTORY)
	{
		flags &= ~O_DIRECTORY;
		o_directory = 1;
	}
	else
		o_directory = 0;
#endif
	if (!path)
		path = dot;
 whither_kiss:
	if (flags & O_INTERCEPT)
		RESTART(r, openat(cwd, path, flags&~O_INTERCEPT, mode));
	else
		RESTART(r, pathopen(cwd, path, NiL, 0, 0, flags|O_INTERCEPT, mode));
#if _ast_O_LOCAL && O_CLOEXEC >= _ast_O_LOCAL
	if (o_cloexec && r >= 0)
		RESTART(o_cloexec, fcntl(r, F_SETFD, FD_CLOEXEC));
#endif

	/*
	 * the O_DIRECTORY+O_SEARCH dance covers filesystems with files
	 * that sometimes act like directories -- this consistently favors
	 * ENOTDIR over EACCESS
	 */

#if _ast_O_LOCAL && O_DIRECTORY >= _ast_O_LOCAL
	if (o_directory)
	{
		if (r >= 0)
		{
			struct stat	st;

			if (fstat(r, &st))
			{
				close(r);
				r = -1;
			}
			else if (!S_ISDIR(st.st_mode))
			{
				close(r);
				r = -1;
				errno = ENOTDIR;
			}
		}
		else if (errno == EACCES && !(flags & O_SEARCH) && O_SEARCH && O_SEARCH < _ast_O_LOCAL)
		{
			flags |= O_SEARCH|O_INTERCEPT;
			goto whither_kiss;
		}
	}
#else
	if (r < 0 && (flags & (O_DIRECTORY|O_SEARCH)) == O_DIRECTORY && O_SEARCH && O_SEARCH < _ast_O_LOCAL)
	{
		flags |= O_SEARCH|O_INTERCEPT;
		goto whither_kiss;
	}
#endif
	return r;
}

int
ast_readlinkat(int cwd, const char* path, char* buf, size_t size)
{
	int	r;

	RESTART(r, readlinkat(cwd, path, buf, size));
	return r;
}

int
ast_symlinkat(const char* path, int cwd, const char* linkpath)
{
	int	r;

	RESTART(r, symlinkat(path, cwd, linkpath));
	return r;
}

int
ast_unlinkat(int cwd, const char* path, int flags)
{
	int	r;

	RESTART(r, unlinkat(cwd, path, flags));
	return r;
}

/* RESTART() intercepts */

int
ast_access(const char* path, int mode)
{
	int	r;

	RESTART(r, faccessat(LOCAL(pwd), path, mode, 0));
	return r;
}

int
ast_chdir(const char* path)
{
	int	r;
	int	fd;

	if (path && *path != '/')
	{
		RESTART(fd, openat(LOCAL(pwd), path, O_SEARCH));
		if (fd < 0)
			return fd;
		RESTART(r, fchdir(fd));
		close(fd);
	}
	else
		RESTART(r, chdir(path));
	return r;
}

int
ast_chmod(const char* path, mode_t mode)
{
	int	r;

	RESTART(r, fchmodat(LOCAL(pwd), path, mode, 0));
	return r;
}

int
ast_chown(const char* path, uid_t owner, gid_t group)
{
	int	r;

	RESTART(r, fchownat(LOCAL(pwd), path, owner, group, 0));
	return r;
}

int
ast_close(int fd)
{
	int	r;

	RESTART(r, close(fd));
	return r;
}

int
ast_dup(int fd)
{
	int	r;

	RESTART(r, dup(fd));
	return r;
}

int
ast_dup2(int ff, int ft)
{
	int	r;

	RESTART(r, dup2(ff, ft));
	return r;
}

int
ast_eaccess(const char* path, int mode)
{
	int	r;

	RESTART(r, faccessat(LOCAL(pwd), path, mode, AT_EACCESS));
	return r;
}

int
ast_fchdir(int fd)
{
	int	r;

	RESTART(r, fchdir(fd));
	return r;
}

int
ast_fchmod(int fd, mode_t mode)
{
        int     r;

	RESTART(r, fchmod(fd, mode));
	return r;
}

int
ast_fchown(int fd, uid_t owner, gid_t group)
{
        int     r;

	RESTART(r, fchown(fd, owner, group));
	return r;
}

/*
 * XXX: we at least cover the ops used by ast -- others may get the wrong 3rd arg
 */

int
ast_fcntl(int fd, int op, ...)
{
	void*	p;
	long	l;
	int	r;
	int	c;
	va_list	ap;

	va_start(ap, op);
	switch (op)
	{

	case F_GETFD:
	case F_GETFL:
#ifdef F_GETLEASE
	case F_GETLEASE:
#endif
#ifdef F_GETOWN
	case F_GETOWN:
#endif
#ifdef F_GETSIG
	case F_GETSIG:
#endif
#ifdef F_GETXFL
	case F_GETXFL:
#endif
		RESTART(r, fcntl(fd, op));
		break;

#ifdef F_DUPFD_CLOEXEC
	case F_DUPFD_CLOEXEC:
#if _ast_F_LOCAL && F_DUPFD_CLOEXEC >= _ast_F_LOCAL
		l = va_arg(ap, long);
		RESTART(r, fcntl(fd, F_DUPFD, l));
		if (r >= 0)
			RESTART(c, fcntl(r, F_SETFD, FD_CLOEXEC));
		break;
#endif
#endif
	case F_DUPFD:
#ifdef F_NOTIFY
	case F_NOTIFY:
#endif
	case F_SETFL:
#ifdef F_SETLEASE
	case F_SETLEASE:
#endif
#ifdef F_SETOWN
	case F_SETOWN:
#endif
#ifdef F_SETSIG
	case F_SETSIG:
#endif
		l = va_arg(ap, long);
		RESTART(r, fcntl(fd, op, l));
		break;

	default:
		p = va_arg(ap, void*);
		RESTART(r, fcntl(fd, op, p));
		break;

	}
	va_end(ap);
	return r;
}

int
ast_fstat(int fd, struct stat* st)
{
	int	r;

	RESTART(r, fstat(fd, st));
	return r;
}

int
ast_ftruncate(int fd, off_t size)
{
	int	r;

	RESTART(r, ftruncate(fd, size));
	return r;
}

/*
 * unlike fcntl() the 3rd ioctl() arg must be a pointer so void* always works
 */

int
ast_ioctl(int fd, int op, ...)
{
	void*	p;
	int	r;
	va_list	ap;

	va_start(ap, op);
	p = va_arg(ap, void*);
	va_end(ap);
	RESTART(r, ioctl(fd, op, p));
	return r;
}

int
ast_lchmod(const char* path, mode_t mode)
{
	int	r;

	RESTART(r, fchmodat(LOCAL(pwd), path, mode, AT_SYMLINK_NOFOLLOW));
	return r;
}

int
ast_lchown(const char* path, uid_t owner, gid_t group)
{
	int	r;

	RESTART(r, fchownat(LOCAL(pwd), path, owner, group, AT_SYMLINK_NOFOLLOW));
	return r;
}

int
ast_link(const char* path, const char* linkpath)
{
	int	r;

	RESTART(r, linkat(LOCAL(pwd), path, LOCAL(pwd), linkpath, 0));
	return r;
}

int
ast_lstat(const char* path, struct stat* st)
{
	int	r;

	RESTART(r, fstatat(LOCAL(pwd), path, st, AT_SYMLINK_NOFOLLOW));
	return r;
}

int
ast_mkdir(const char* path, mode_t mode)
{
	int	r;

	RESTART(r, mkdirat(LOCAL(pwd), path, mode));
	return r;
}

int
ast_mkfifo(const char* path, mode_t mode)
{
	int	r;

	RESTART(r, mkfifoat(LOCAL(pwd), path, mode));
	return r;
}

int
ast_mknod(const char* path, mode_t mode, dev_t dev)
{
	int	r;

	RESTART(r, mknodat(LOCAL(pwd), path, mode, dev));
	return r;
}

int
ast_open(const char* path, int flags, ...)
{
	int	r;
	mode_t	mode;
	va_list	ap;

	va_start(ap, flags);
	mode = (flags & O_CREAT) ? (mode_t)va_arg(ap, int) : (mode_t)0;
	va_end(ap);
	return ast_openat(LOCAL(pwd), path, flags, mode);
}

int
ast_creat(const char* path, mode_t mode)
{
	return ast_open(path, O_CREAT|O_WRONLY|O_TRUNC, mode);
}

int
ast_pipe(int fds[2])
{
	int	r;

	RESTART(r, pipe(fds));
	return r;
}

int
ast_pipe2(int fds[2], int flags)
{
	int	r;
#if _lib_pipe2
	RESTART(r, pipe2(fds, flags));
#else
	int	c;

	RESTART(r, pipe(fds));
	if ((flags & O_CLOEXEC) && r >= 0)
	{
		RESTART(c, fcntl(fds[0], F_SETFD, FD_CLOEXEC));
		RESTART(c, fcntl(fds[1], F_SETFD, FD_CLOEXEC));
	}
#ifdef O_NONBLOCK
	if (flags & O_NONBLOCK)
	{
		int	f;

		RESTART(f, fcntl(fds[0], F_GETFL));
		if (f >= 0)
			RESTART(c, fcntl(fds[0], F_SETFL, f|O_NONBLOCK));
		RESTART(f, fcntl(fds[1], F_GETFL));
		if (f >= 0)
			RESTART(c, fcntl(fds[1], F_SETFL, f|O_NONBLOCK));
	}
#endif
#endif
	return r;
}

int
ast_readlink(const char* path, char* buf, size_t size)
{
	int	r;

	RESTART(r, readlinkat(LOCAL(pwd), path, buf, size));
	return r;
}

int
ast_remove(const char* path)
{
	int	r;

	RESTART(r, unlinkat(LOCAL(pwd), path, 0));
	return r;
}

int
ast_rename(const char* oldpath, const char* newpath)
{
	int	r;

	RESTART(r, renameat(LOCAL(pwd), oldpath, LOCAL(pwd), newpath));
	return r;
}

int
ast_rmdir(const char* path)
{
	int	r;

	RESTART(r, unlinkat(LOCAL(pwd), path, AT_REMOVEDIR));
	return r;
}

int
ast_stat(const char* path, struct stat* st)
{
	int	r;

	RESTART(r, fstatat(LOCAL(pwd), path, st, 0));
	return r;
}

int
ast_symlink(const char* path, const char* linkpath)
{
	int	r;

	RESTART(r, symlinkat(path, LOCAL(pwd), linkpath));
	return r;
}

int
ast_truncate(const char* path, off_t size)
{
	int	r;
	int	fd;

	if (path && *path != '/')
	{
		RESTART(fd, openat(LOCAL(pwd), path, O_WRONLY));
		if (fd < 0)
			return -1;
		RESTART(r, ftruncate(fd, size));
		close(fd);
	}
	else
		RESTART(r, truncate(path, size));
	return r;
}

int
ast_unlink(const char* path)
{
	int	r;

	RESTART(r, unlinkat(LOCAL(pwd), path, 0));
	return r;
}

/* socket intercepts */

#if _ast_SOCK_CLOEXEC || _ast_SOCK_NONBLOCK && defined(O_NONBLOCK)

static void
socktype(int type, int fd1, int fd2)
{
	int	c;

#if _ast_SOCK_CLOEXEC
	if (type & SOCK_CLOEXEC)
	{
		RESTART(c, fcntl(fd1, F_SETFD, FD_CLOEXEC));
		if (fd2 >= 0)
			RESTART(c, fcntl(fd2, F_SETFD, FD_CLOEXEC));
	}
#endif
#if _ast_SOCK_NONBLOCK && defined(O_NONBLOCK)
	if (type & O_NONBLOCK)
	{
		RESTART(c, fcntl(fd1, F_GETFL));
		if (c >= 0)
			RESTART(c, fcntl(fd1, F_SETFL, c|O_NONBLOCK));
		RESTART(c, fcntl(fd2, F_GETFL));
		if (c >= 0)
			RESTART(c, fcntl(fd2, F_SETFL, c|O_NONBLOCK));
	}
#endif
}

#define SOCKTYPE(r,t,f1,f2)	do { if ((r) >= 0) socktype(t, f1, f2); } while (0)

#else

#define SOCKTYPE(r,t,f1,f2)

#endif

int
ast_accept(int fd, struct sockaddr* addr, socklen_t* len)
{
	int	r;

	RESTART(r, accept(fd, addr, len));
	return r;
}

int
ast_accept4(int fd, struct sockaddr* addr, socklen_t* len, int flags)
{
	int	r;

#if _lib_accept4
	RESTART(r, accept4(fd, addr, len, flags));
#else
	RESTART(r, accept(fd, addr, len));
	SOCKTYPE(r, flags, r, -1);
#endif
	return r;
}

int
ast_connect(int fd, struct sockaddr* addr, socklen_t len)
{
	int	r;

	RESTART(r, connect(fd, addr, len));
	return r;
}

int
ast_socket(int domain, int type, int protocol)
{
	int	r;
	int	t;

	t = type;
#if _ast_SOCK_CLOEXEC
	t &= ~SOCK_CLOEXEC;
#endif
#if _ast_SOCK_NONBLOCK
	t &= ~SOCK_NONBLOCK;
#endif
	RESTART(r, socket(domain, t, protocol));
	SOCKTYPE(r, type, r, -1);
	return r;
}

int
ast_socketpair(int domain, int type, int protocol, int fds[2])
{
	int	r;
	int	t;

	t = type;
#if _ast_SOCK_CLOEXEC
	t &= ~SOCK_CLOEXEC;
#endif
#if _ast_SOCK_NONBLOCK
	t &= ~SOCK_NONBLOCK;
#endif
	RESTART(r, socketpair(domain, t, protocol, fds));
	SOCKTYPE(r, type, fds[0], fds[1]);
	return r;
}
