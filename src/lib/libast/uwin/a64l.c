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
#ifndef _UWIN

void _STUB_a64l(){}

#else

#define a64l	______a64l
#define l64a	______l64a

#include	<stdlib.h>
#include	<string.h>

#undef	a64l
#undef	l64a

#if defined(__EXPORT__)
#define extern		__EXPORT__
#endif

static char letter[65] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

extern long a64l(const char *str)
{
	register unsigned long ul = 0;
	register int n = 6;
	register int c;
	register char *cp;
	for(n=0; n <6; n++)
	{
		if((c= *str++)==0)
			break;
		if(!(cp=strchr(letter,c)))
			break;
		ul |= (cp-letter)<< (6*n);
	}
	return((long)ul);
}

extern char *l64a(long l)
{
	static char buff[7];
	unsigned ul = ((unsigned long)l & 0xffffffff);
	register char *cp = buff;
	while(ul>0)
	{
		*cp++ = letter[ul&077];
		ul >>= 6;
	}
	*cp = 0;
	return(buff);
}

#endif
