/*******************************************************************
*                                                                  *
*             This software is part of the ast package             *
*                Copyright (c) 1985-2000 AT&T Corp.                *
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
*                                                                  *
*******************************************************************/
#pragma prototyped
/*
 * dirname(3) implementation
 */

#include <libgen.h>

char *dirname(register char *pathname)
{
	register char  *last;
	/* go to end of path */
	for(last=pathname; *last; last++);
	/* back over trailing '/' */
	while(last>pathname && *--last=='/');
	/* back over non-slash chars */
	for(;last>pathname && *last!='/';last--);
	if(last==pathname)
	{
		/* all '/' or "" */
		if(*pathname!='/')
			last = pathname = ".";
		/* preserve // */
		else if(pathname[1]=='/')
			last++;
	}
	else
	{
		/* back over trailing '/' */
		for(;*last=='/' && last > pathname; last--);
		/* preserve // */
		if(last==pathname && *pathname=='/' && pathname[1]=='/')
			last++;
	}
	*last = 0;
	return(pathname);
}
