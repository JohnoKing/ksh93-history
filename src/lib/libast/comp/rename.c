/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*                  Copyright (c) 1985-2004 AT&T Corp.                  *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                            by AT&T Corp.                             *
*                                                                      *
*                A copy of the License is available at                 *
*            http://www.opensource.org/licenses/cpl1.0.txt             *
*         (with md5 checksum 059e8cd6165cb4c31e351f2b69388fd9)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                   Phong Vo <kpv@research.att.com>                    *
*                                                                      *
***********************************************************************/
#pragma prototyped

#include <ast.h>

#if _lib_rename

NoN(rename)

#else

#include <error.h>
#include <proc.h>

#ifdef EPERM

static int
mvdir(const char* from, const char* to)
{
	char*			argv[4];
	int			oerrno;

	static const char	mvdir[] = "/usr/lib/mv_dir";

	oerrno = errno;
	if (!eaccess(mvdir, X_OK))
	{
		argv[0] = mvdir;
		argv[1] = from;
		argv[2] = to;
		argv[3] = 0;
		if (!procrun(argv[0], argv))
		{
			errno = oerrno;
			return 0;
		}
	}
	errno = EPERM;
	return -1;
}

#endif

int
rename(const char* from, const char* to)
{
	int	oerrno;
	int	ooerrno;

	ooerrno = errno;
	while (link(from, to))
	{
#ifdef EPERM
		if (errno == EPERM)
		{
			errno = ooerrno;
			return mvdir(from, to);
		}
#endif
		oerrno = errno;
		if (unlink(to))
		{
#ifdef EPERM
			if (errno == EPERM)
			{
				errno = ooerrno;
				return mvdir(from, to);
			}
#endif
			errno = oerrno;
			return -1;
		}
	}
	errno = ooerrno;
	return unlink(from);
}

#endif
