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
#include	"sfhdr.h"

/*	Seek function that knows discipline
**
**	Written by Kiem-Phong Vo.
*/
#if __STD_C
Sfoff_t sfsk(reg Sfio_t* f, Sfoff_t addr, reg int type, Sfdisc_t* disc)
#else
Sfoff_t sfsk(f,addr,type,disc)
reg Sfio_t*	f;
Sfoff_t		addr;
reg int		type;
Sfdisc_t*	disc;
#endif
{
	Sfoff_t		p;
	reg Sfdisc_t*	dc;
	reg ssize_t	s;
	reg int		local, mode;

	SFMTXSTART(f, (Sfoff_t)(-1));

	GETLOCAL(f,local);
	if(!local && !(f->bits&SF_DCDOWN))
	{	if((mode = f->mode&SF_RDWR) != (int)f->mode && _sfmode(f,mode,0) < 0)
			SFMTXRETURN(f, (Sfoff_t)(-1));
		if(SFSYNC(f) < 0)
			SFMTXRETURN(f, (Sfoff_t)(-1));
#ifdef MAP_TYPE
		if(f->mode == SF_READ && (f->bits&SF_MMAP) && f->data)
		{	SFMUNMAP(f, f->data, f->endb-f->data);
			f->data = NIL(uchar*);
		}
#endif
		f->next = f->endb = f->endr = f->endw = f->data;
	}

	if((type &= (SEEK_SET|SEEK_CUR|SEEK_END)) > SEEK_END)
		SFMTXRETURN(f, (Sfoff_t)(-1));

	for(;;)
	{	dc = disc;
		if(f->flags&SF_STRING)
		{	SFSTRSIZE(f);
			if(type == SEEK_SET)
				s = (ssize_t)addr;
			else if(type == SEEK_CUR)
				s = (ssize_t)(addr + f->here);
			else	s = (ssize_t)(addr + f->extent);
		}
		else
		{	SFDISC(f,dc,seekf);
			if(dc && dc->seekf)
			{	SFDCSK(f,addr,type,dc,p);
			}
			else
			{	p = syslseekf(f->file,(sfoff_t)addr,type);
			}
			if(p >= 0)
				SFMTXRETURN(f,p);
			s = -1;
		}

		if(local)
			SETLOCAL(f);
		switch(_sfexcept(f,SF_SEEK,s,dc))
		{
		case SF_EDISC:
		case SF_ECONT:
			if(f->flags&SF_STRING)
				SFMTXRETURN(f, (Sfoff_t)s);
			goto do_continue;
		default:
			SFMTXRETURN(f, (Sfoff_t)(-1));
		}

	do_continue:
		for(dc = f->disc; dc; dc = dc->disc)
			if(dc == disc)
				break;
		disc = dc;
	}
}
