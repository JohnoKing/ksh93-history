/*******************************************************************
*                                                                  *
*             This software is part of the ast package             *
*                Copyright (c) 1985-2001 AT&T Corp.                *
*        and it may only be used by you under license from         *
*                       AT&T Corp. ("AT&T")                        *
*         A copy of the Source Code Agreement is available         *
*                at the AT&T Internet web site URL                 *
*                                                                  *
*       http://www.research.att.com/sw/license/ast-open.html       *
*                                                                  *
*        If you have copied this software without agreeing         *
*        to the terms of the license you are infringing on         *
*           the license and copyright and are violating            *
*               AT&T's intellectual property rights.               *
*                                                                  *
*                 This software was created by the                 *
*                 Network Services Research Center                 *
*                        AT&T Labs Research                        *
*                         Florham Park NJ                          *
*                                                                  *
*               Glenn Fowler <gsf@research.att.com>                *
*                David Korn <dgk@research.att.com>                 *
*                 Phong Vo <kpv@research.att.com>                  *
*******************************************************************/
#pragma prototyped

/*
 * omissions and other workarounds
 */

#include <ast.h>
#include <error.h>

#include "FEATURE/omitted"

#if _win32_botch

#include <ls.h>
#include <utime.h>

/*
 * these workarounds assume each system call foo() has an _foo() entry
 * which is true for __CYGWIN__ which is currently the only _WIN32 system
 * that needs the workarounds
 *
 * the workarounds handle .exe suffix omissions and the fixed /bin/sh
 * reference in execve() and bogus getpagesize() return values
 *
 * NOTE: __CYGWIN__ _win32_botch_* workarounds have been moved to
 *	 cygwin1.dll by Karsten Fleischer <K.Fleischer@omnium.de>.
 *	 Thanks to Karsten for working with gsf through umpteen
 *	 emails to get these issues resolved -- now real unix
 *	 commands and makefiles can build out of the box.
 *	 And yes, not all workarounds can be handled by the unix
 *	 syscall interface. In particular, { ksh nmake } have 
 *	 workarounds for case-ignorant filesystems and { libast }
 *	 has workarounds for win32 locale info.
 */

#define ic(x,c)	(((x)|040)==(c))

extern int	_access(const char*, int);
extern int	_chmod(const char*, mode_t);
extern int	_close(int);
extern int	_execve(const char*, char*const[], char*const[]);
extern int	_link(const char*, const char*);
extern int	_open(const char*, int, ...);
extern long	_pathconf(const char*, int);
extern ssize_t	_read(int, void*, size_t);
extern int	_rename(const char*, const char*);
extern int	_stat(const char*, struct stat*);
extern int	_unlink(const char*);
extern int	_utime(const char*, struct utimbuf*);
extern ssize_t	_write(int, const void*, size_t);

#undef pathconf
#undef stat

#if defined(__EXPORT__)
#define extern	__EXPORT__
#endif

static char*
suffix(register const char* path)
{
	register const char*	s = path + strlen(path);
	register int		c;

	while (s > path)
		if ((c = *--s) == '.')
			return (char*)s + 1;
		else if (c == '/' || c == '\\')
			break;
	return 0;
}

#define MAGIC_mode		0
#define MAGIC_exec		1

/*
 * return 0 if path is magic, -1 otherwise
 * op==MAGIC_exec retains errno for -1 return
 */

static int
magic(const char* path, int op)
{
	int		fd;
	int		r;
	int		oerrno;
	unsigned char	buf[2];

	oerrno = errno;
	if ((fd = _open(path, O_RDONLY, 0)) >= 0)
	{
		r = _read(fd, buf, 2) == 2 && (buf[1] == 0x5a && (buf[0] == 0x4c || buf[0] == 0x4d) || op == MAGIC_exec && buf[0] == '#' && buf[1] == '!') ? 0 : -1;
		close(fd);
		if (r && op == MAGIC_exec)
			oerrno = ENOEXEC;
	}
	else if (op != MAGIC_exec)
		r = -1;
	else if (errno == ENOENT)
	{
		oerrno = errno;
		r = -1;
	}
	else
		r = 0;
	errno = oerrno;
	return r;
}

#if _win32_botch_access

extern int
access(const char* path, int op)
{
	int	r;
	int	oerrno;
	char	buf[PATH_MAX];

	oerrno = errno;
	if ((r = _access(path, op)) && errno == ENOENT && !suffix(path))
	{
		snprintf(buf, sizeof(buf), "%s.exe", path);
		errno = oerrno;
		r = _access(buf, op);
	}
	return r;
}

#endif

#if _win32_botch_chmod

extern int
chmod(const char* path, mode_t mode)
{
	int	r;
	int	oerrno;
	char	buf[PATH_MAX];

	if ((r = _chmod(path, mode)) && errno == ENOENT && !suffix(path))
	{
		snprintf(buf, sizeof(buf), "%s.exe", path);
		errno = oerrno;
		return _chmod(buf, mode);
	}
	if (!(r = _chmod(path, mode)) &&
	    (mode & (S_IXUSR|S_IXGRP|S_IXOTH)) &&
	    !suffix(path) &&
	    (strlen(path) + 4) < sizeof(buf))
	{
		oerrno = errno;
		if (!magic(path, MAGIC_mode))
		{
			snprintf(buf, sizeof(buf), "%s.exe", path);
			_rename(path, buf);
		}
		errno = oerrno;
	}
	return r;
}

#endif

#if _win32_botch_execve

#define DEBUG		1
#define DEBUG_init	(1<<0)
#define DEBUG_enoexec	(1<<1)
#define DEBUG_trace	(1<<2)

extern int
execve(const char* path, char* const argv[], char* const envv[])
{
	register char*	s;
	register char**	p;
	register char**	v;
	int		n;
	int		oerrno;
	struct stat	st;
	struct stat	ss;
	char		buf[PATH_MAX];
	char		cmd[PATH_MAX];

#if DEBUG
	static int	debug;
#endif

#if DEBUG
	if (!debug)
	{
		debug |= DEBUG_init;
		if (getenv("_AST_execve_ENOEXEC"))
			debug |= DEBUG_enoexec;
		if (getenv("_AST_execve_trace"))
			debug |= DEBUG_trace;
	}
#endif
	v = 0;
	oerrno = errno;
	s = suffix(path);
	if (_stat(path, &st))
		return -1;
	if (!s)
	{
		snprintf(buf, sizeof(buf), "%s.exe", path);
		if (!_stat(buf, &ss) && st.st_dev == ss.st_dev && st.st_ino == ss.st_ino)
		{
			path = (const char*)buf;
			goto exec;
		}
		errno = oerrno;
	}
	if (!S_ISREG(st.st_mode) || !(st.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH)))
	{
		errno = EACCES;
		return -1;
	}
	if (s)
		switch (strlen(s))
		{
		case 2:
			if (ic(s[0], 's') && ic(s[1], 'h'))
				goto shell;
			break;
		case 3:
			if (ic(s[0], 'k') && ic(s[1], 's') && ic(s[2], 'h'))
				goto shell;
			if (ic(s[0], 'e') && ic(s[1], 'x') && ic(s[2], 'e'))
				goto exec;
			break;
		}
	if (!magic(path, MAGIC_exec))
		goto exec;
#if DEBUG
	if (debug & DEBUG_enoexec)
	{
		errno = ENOEXEC;
		return -1;
	}
#endif
 	errno = oerrno;
 shell:
	p = (char**)argv;
	while (*p++);
	if (!(v = (char**)malloc((p - (char**)argv + 2) * sizeof(char*))))
	{
		errno = EAGAIN;
		return -1;
	}
	p = v;
	*p++ = "sh";
	*p++ = (char*)path;
	path = (const char*)pathshell();
	if (!suffix(path))
	{
		snprintf(cmd, sizeof(cmd), "%s.exe", path);
		path = (const char*)cmd;
	}
	if (*argv)
		argv++;
	while (*p++ = (char*)*argv++);
	argv = (char*const*)v;
 exec:
#if DEBUG
	if (debug & DEBUG_trace)
	{
		sfprintf(sfstderr, "_execve %s [", path);
		for (n = 0; argv[n]; n++)
			sfprintf(sfstderr, " '%s'", argv[n]);
		sfprintf(sfstderr, " ] [");
		for (n = 0; envv[n]; n++)
			sfprintf(sfstderr, " '%s'", envv[n]);
		sfprintf(sfstderr, " ]\n");
	}
#endif
	if (n = _execve(path, argv, envv))
		free(v);
	return n;
}

#endif

#if _win32_botch_getpagesize

extern size_t
getpagesize(void)
{
	return 64 * 1024;
}

#endif

#if _win32_botch_link

extern int
link(const char* fp, const char* tp)
{
	int	r;
	int	oerrno;
	char	fb[PATH_MAX];
	char	tb[PATH_MAX];

	oerrno = errno;
	if ((r = _link(fp, tp)) && errno == ENOENT && !suffix(fp))
	{
		snprintf(fb, sizeof(fb), "%s.exe", fp);
		if (!suffix(tp))
		{
			snprintf(tb, sizeof(tb), "%s.exe", tp);
			tp = tb;
		}
		errno = oerrno;
		r = _link(fb, tp);
	}
	return r;
}

#endif

#if _win32_botch_open || _win32_botch_copy

#if _win32_botch_copy

/*
 * this should intercept the important cases
 * dup*() and exec*() fd's will not be intercepted
 */

typedef struct Exe_test_s
{
	int		test;
	int		magic;
	ino_t		ino;
	char		path[PATH_MAX+1];
} Exe_test_t;

static Exe_test_t	exe[16];

extern int
close(int fd)
{
	int		r;
	int		oerrno;
	struct stat	st;
	char		buf[PATH_MAX];

	if (fd >= 0 && fd < elementsof(exe))
	{
		if (exe[fd].magic && !fstat(fd, &st) && st.st_ino == exe[fd].ino)
		{
			exe[fd].test = exe[fd].magic = 0;
			if (r = _close(fd))
				return r;
			oerrno = errno;
			if (stat(exe[fd].path, &st) && st.st_ino == exe[fd].ino)
			{
				snprintf(buf, sizeof(buf), "%s.exe", exe[fd].path);
				_rename(exe[fd].path, buf);
			}
			errno = oerrno;
			return 0;
		}
		exe[fd].test = exe[fd].magic = 0;
	}
	return _close(fd);
}

extern ssize_t
write(int fd, const void* buf, size_t n)
{
	if (fd >= 0 && fd < elementsof(exe) && exe[fd].test)
	{
		exe[fd].test = 0;
		exe[fd].magic = n >= 2 && ((unsigned char*)buf)[1] == 0x5a && (((unsigned char*)buf)[0] == 0x4c || ((unsigned char*)buf)[0] == 0x4d) && !lseek(fd, (off_t)0, SEEK_CUR);
	}
	return _write(fd, buf, n);
}

#endif

extern int
open(const char* path, int flags, ...)
{
	int		fd;
	int		mode;
	int		oerrno;
	char		buf[PATH_MAX];
#if _win32_botch_copy
	struct stat	st;
#endif
	va_list		ap;

	va_start(ap, flags);
	mode = (flags & O_CREAT) ? va_arg(ap, int) : 0;
	oerrno = errno;
	if ((fd = _open(path, flags, mode)) < 0)
#if _win32_botch_open
		if (errno == ENOENT && !suffix(path))
		{
			snprintf(buf, sizeof(buf), "%s.exe", path);
			errno = oerrno;
			fd = _open(buf, flags, mode);
		}
#else
		/*nop*/;
#endif
#if _win32_botch_copy
	else if (fd < elementsof(exe))
	{
		if ((mode & 0111) && !suffix(path) && strlen(path) <= PATH_MAX && !fstat(fd, &st))
		{
			exe[fd].test = 1;
			exe[fd].magic = 0;
			exe[fd].ino = st.st_ino;
			strcpy(exe[fd].path, path);
		}
		else
			exe[fd].test = exe[fd].magic = 0;
		errno = oerrno;
	}
#endif
	va_end(ap);
	return fd;
}

#endif

#if _win32_botch_pathconf

extern long
pathconf(const char* path, int op)
{
	if (_access(path, F_OK))
		return -1;
	return _pathconf(path, op);
}

#endif

#if _win32_botch_rename

extern int
rename(const char* fp, const char* tp)
{
	int	r;
	int	oerrno;
	char	fb[PATH_MAX];
	char	tb[PATH_MAX];

	oerrno = errno;
	if ((r = _rename(fp, tp)) && errno == ENOENT && !suffix(fp))
	{
		snprintf(fb, sizeof(fb), "%s.exe", fp);
		if (!suffix(tp))
		{
			snprintf(tb, sizeof(tb), "%s.exe", tp);
			tp = tb;
		}
		errno = oerrno;
		r = _rename(fb, tp);
	}
	return r;
}

#endif

#if _win32_botch_stat

extern int
stat(const char* path, struct stat* st)
{
	int	r;
	int	oerrno;
	char	buf[PATH_MAX];

	oerrno = errno;
	if ((r = _stat(path, st)) && errno == ENOENT && !suffix(path))
	{
		snprintf(buf, sizeof(buf), "%s.exe", path);
		errno = oerrno;
		r = _stat(buf, st);
	}
	return r;
}

#endif

#if _win32_botch_truncate

extern int
truncate(const char* path, off_t offset)
{
	int	r;
	int	oerrno;
	char	buf[PATH_MAX];

	oerrno = errno;
	if ((r = _truncate(path, offset)) && errno == ENOENT && !suffix(path))
	{
		snprintf(buf, sizeof(buf), "%s.exe", path);
		errno = oerrno;
		r = _truncate(buf, offset);
	}
	return r;
}

#endif

#if _win32_botch_unlink

extern int
unlink(const char* path)
{
	int	r;
	int	oerrno;
	char	buf[PATH_MAX];

	oerrno = errno;
	if ((r = _unlink(path)) && errno == ENOENT && !suffix(path))
	{
		snprintf(buf, sizeof(buf), "%s.exe", path);
		errno = oerrno;
		r = _unlink(buf);
	}
	return r;
}

#endif

#if _win32_botch_utime

extern int
utime(const char* path, struct utimbuf* ut)
{
	int	r;
	int	oerrno;
	char	buf[PATH_MAX];

	oerrno = errno;
	if ((r = _utime(path, ut)) && errno == ENOENT && !suffix(path))
	{
		snprintf(buf, sizeof(buf), "%s.exe", path);
		errno = oerrno;
		r = _utime(buf, ut);
	}
	return r;
}

#endif

#else

#if sun || _sun || __sun

/*
 * sun misses a few functions required by its own bsd-like macros
 */

#ifdef	_SC_PAGESIZE
#undef	PAGESIZE
#define PAGESIZE	(int)sysconf(_SC_PAGESIZE)
#else
#ifndef PAGESIZE
#define PAGESIZE	4096
#endif
#endif

void
bzero(void* b, size_t n)
{
	memset(b, 0, n);
}

int
getpagesize()
{
	return PAGESIZE;
}

int
killpg(pid_t pgrp, int sig)
{
	return kill(-pgrp, sig);
}

#else

NoN(omitted)

#endif

#endif
