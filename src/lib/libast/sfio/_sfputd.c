/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*                  Copyright (c) 1985-2004 AT&T Corp.                  *
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
*                   Phong Vo <kpv@research.att.com>                    *
*                                                                      *
***********************************************************************/
#include	"sfhdr.h"

/*	Write out a floating point value in a portable format
**
**	Written by Kiem-Phong Vo.
*/

#if __STD_C
int _sfputd(Sfio_t* f, Sfdouble_t v)
#else
int _sfputd(f,v)
Sfio_t*		f;
Sfdouble_t	v;
#endif
{
#define N_ARRAY		(16*sizeof(Sfdouble_t))
	reg ssize_t	n, w;
	reg uchar	*s, *ends;
	int		exp;
	uchar		c[N_ARRAY];
	Sfdouble_t	x;

	SFMTXSTART(f,-1);

	if(f->mode != SF_WRITE && _sfmode(f,SF_WRITE,0) < 0)
		SFMTXRETURN(f, -1);
	SFLOCK(f,0);

	/* get the sign of v */
	if(v < 0.)
	{	v = -v;
		n = 1;
	}
	else	n = 0;

	/* make the magnitude of v < 1 */
	if(v != 0.)
		v = frexpl(v,&exp);
	else	exp = 0;

	/* code the sign of v and exp */
	if((w = exp) < 0)
	{	n |= 02;
		w = -w;
	}

	/* write out the signs and the exp */
	SFOPEN(f,0);
	if(sfputc(f,n) < 0 || (w = sfputu(f,w)) < 0)
		SFMTXRETURN(f, -1);
	SFLOCK(f,0);
	w += 1;

	s = (ends = &c[0])+sizeof(c);
	while(s > ends)
	{	/* get 2^SF_PRECIS precision at a time */
		n = (int)(x = ldexpl(v,SF_PRECIS));
		*--s = n|SF_MORE;
		v = x-n;
		if(v <= 0.)
			break;
	}

	/* last byte is not SF_MORE */
	ends = &c[0] + sizeof(c) -1;
	*ends &= ~SF_MORE;

	/* write out coded bytes */
	n = ends - s + 1;
	w = SFWRITE(f,(Void_t*)s,n) == n ? w+n : -1;

	SFOPEN(f,0);
	SFMTXRETURN(f,w);
}
