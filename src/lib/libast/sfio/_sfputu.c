/*******************************************************************
*                                                                  *
*             This software is part of the ast package             *
*                Copyright (c) 1985-2002 AT&T Corp.                *
*        and it may only be used by you under license from         *
*                       AT&T Corp. ("AT&T")                        *
*         A copy of the Source Code Agreement is available         *
*                at the AT&T Internet web site URL                 *
*                                                                  *
*       http://www.research.att.com/sw/license/ast-open.html       *
*                                                                  *
*    If you have copied or used this software without agreeing     *
*        to the terms of the license you are infringing on         *
*           the license and copyright and are violating            *
*               AT&T's intellectual property rights.               *
*                                                                  *
*            Information and Software Systems Research             *
*                        AT&T Labs Research                        *
*                         Florham Park NJ                          *
*                                                                  *
*               Glenn Fowler <gsf@research.att.com>                *
*                David Korn <dgk@research.att.com>                 *
*                 Phong Vo <kpv@research.att.com>                  *
*                                                                  *
*******************************************************************/
#include	"sfhdr.h"

/*	Write out an unsigned long value in a portable format.
**
**	Written by Kiem-Phong Vo.
*/

#if __STD_C
int _sfputu(reg Sfio_t* f, Sfulong_t v)
#else
int _sfputu(f,v)
reg Sfio_t*	f;	/* write a portable ulong to this stream */
Sfulong_t	v;	/* the unsigned value to be written */
#endif
{
#define N_ARRAY		(2*sizeof(Sfulong_t))
	reg uchar	*s, *ps;
	reg ssize_t	n, p;
	uchar		c[N_ARRAY];

	SFMTXSTART(f, -1);

	if(f->mode != SF_WRITE && _sfmode(f,SF_WRITE,0) < 0)
		SFMTXRETURN(f, -1);
	SFLOCK(f,0);

	/* code v as integers in base SF_UBASE */
	s = ps = &(c[N_ARRAY-1]);
	*s = (uchar)SFUVALUE(v);
	while((v >>= SF_UBITS) )
		*--s = (uchar)(SFUVALUE(v) | SF_MORE);
	n = (ps-s)+1;

	if(n > 8 || SFWPEEK(f,ps,p) < n)
		n = SFWRITE(f,(Void_t*)s,n); /* write the hard way */
	else
	{	switch(n)
		{
		case 8 : *ps++ = *s++;
		case 7 : *ps++ = *s++;
		case 6 : *ps++ = *s++;
		case 5 : *ps++ = *s++;
		case 4 : *ps++ = *s++;
		case 3 : *ps++ = *s++;
		case 2 : *ps++ = *s++;
		case 1 : *ps++ = *s++;
		}
		f->next = ps;
	}

	SFOPEN(f,0);
	SFMTXRETURN(f, (int)n);
}
