/*******************************************************************
*                                                                  *
*             This software is part of the ast package             *
*                Copyright (c) 1990-2000 AT&T Corp.                *
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
*                                                                  *
*******************************************************************/
#pragma prototyped
/*
 * Glenn Fowler
 * AT&T Research
 *
 * sync all outstanding file operations for file opened on fd
 * if file==0 then fd used
 * if fd<0 then file used
 * if mode<0 then fd not created
 *
 * NOTE: this is an unfortunate NFS workaround that should be done by fsync()
 */

#include "colib.h"

#include <ls.h>

#include "FEATURE/nfsd"

int
cosync(Coshell_t* co, const char* file, int fd, int mode)
{
#if defined(_cmd_nfsd)
	if (!co || (co->flags & CO_SERVER))
	{
		char	tmp[PATH_MAX];

		if (file && *file)
		{
			register const char*	s;
			register char*		t;
			register char*		b;
			int			td;

			/*
			 * writing to a dir apparently flushes the
			 * attribute cache for all entries in the dir
			 */

			s = file;
			b = t = tmp;
			while (t < &tmp[sizeof(tmp) - 1])
			{
				if (!(*t = *s++)) break;
				if (*t++ == '/') b = t;
			}
			s = "..nfs..botch..";
			t = b;
			while (t < &tmp[sizeof(tmp) - 1] && (*t++ = *s++));
			*t = 0;
			if ((td = open(tmp, O_WRONLY|O_CREAT|O_TRUNC, 0)) >= 0) close(td);
			unlink(tmp);
			if (fd >= 0 && mode >= 0)
			{
				if ((td = open(file, mode)) < 0) return(-1);
				close(fd);
				dup2(td, fd);
				close(td);
			}
		}
#if defined(F_SETLK)
		else
		{
			int		clean = 0;
			struct flock	lock;

			if (fd < 0)
			{
				if (!file || mode < 0 || (fd = open(file, O_RDONLY)) < 0) return(-1);
				clean = 1;
			}

			/*
			 * this sets the VNOCACHE flag across NFS
			 */

			lock.l_type = F_RDLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 1;
			if (!fcntl(fd, F_SETLK, &lock))
			{
				lock.l_type = F_UNLCK;
				fcntl(fd, F_SETLK, &lock);
			}
			if (clean) close(fd);

			/*
			 * 4.1 has a bug that lets VNOCACHE linger after unlock
			 * VNOCACHE inhibits mapping which kills exec
			 * the double rename flushes the incore vnode (and VNOCACHE)
			 *
			 * this kind of stuff doesn't happen with *real* file systems
			 */

			if (file && *file)
			{
				strcpy(tmp, file);
				fd = strlen(tmp) - 1;
				tmp[fd] = (tmp[fd] == '*') ? '?' : '*';
				if (!rename(file, tmp)) rename(tmp, file);
			}
		}
#endif
	}
#endif
	return(0);
}
