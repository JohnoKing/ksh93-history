/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*                  Copyright (c) 1992-2004 AT&T Corp.                  *
*                      and is licensed under the                       *
*          Common Public License, Version 1.0 (the "License")          *
*                        by AT&T Corp. ("AT&T")                        *
*      Any use, downloading, reproduction or distribution of this      *
*      software constitutes acceptance of the License.  A copy of      *
*                     the License is available at                      *
*                                                                      *
*         http://www.research.att.com/sw/license/cpl-1.0.html          *
*         (with md5 checksum 8a5e0081c856944e76c69a1cf29c2e8b)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                                                                      *
***********************************************************************/
#pragma prototyped
/*
 * use tw to recurse on argc,argv with pfxc,pfxv prefix args
 */

#include <cmdlib.h>
#include <proc.h>
#include <ftwalk.h>

int
cmdrecurse(int argc, char** argv, int pfxc, char** pfxv)
{
	register char**	v;
	register char**	a;
	int		resolve = 'L';
	char		arg[16];

	if (!(a = (char**)stakalloc((argc + pfxc + 4) * sizeof(char**))))
		error(ERROR_exit(1), "out of space");
	v = a;
	*v++ = "tw";
	*v++ = arg;
	*v++ = *(argv - opt_info.index);
	while (*v = *pfxv++)
	{
		if (streq(*v, "-H"))
			resolve = 'H';
		else if (streq(*v, "-P"))
			resolve = 'P';
		v++;
	}
	while (*v++ = *argv++);
	sfsprintf(arg, sizeof(arg), "-%cc%d", resolve, pfxc + 2);
	procopen(*a, a, NiL, NiL, PROC_OVERLAY);
	return(-1);
}
