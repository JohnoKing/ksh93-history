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
#include	"sfhdr.h"

/*	Read n bytes from a stream into a buffer
**
**	Written by Kiem-Phong Vo.
*/

#if __STD_C
ssize_t sfread(reg Sfio_t* f, Void_t* buf, reg size_t n)
#else
ssize_t sfread(f,buf,n)
reg Sfio_t*	f;	/* read from this stream. 	*/
Void_t*		buf;	/* buffer to read into		*/
reg size_t	n;	/* number of bytes to be read. 	*/
#endif
{
	reg uchar	*s, *begs;
	reg ssize_t	r;
	reg int		local, justseek;

	SFMTXSTART(f, (ssize_t)(-1));

	GETLOCAL(f,local);
	justseek = f->bits&SF_JUSTSEEK; f->bits &= ~SF_JUSTSEEK;

	if(!buf)
		SFMTXRETURN(f, (ssize_t)(n == 0 ? 0 : -1) );

	/* release peek lock */
	if(f->mode&SF_PEEK)
	{	if(!(f->mode&SF_READ) )
			SFMTXRETURN(f, (ssize_t)(-1));

		if(f->mode&SF_GETR)
		{	if(((uchar*)buf + f->val) != f->next &&
			   (!f->rsrv || f->rsrv->data != (uchar*)buf) )
				SFMTXRETURN(f, (ssize_t)(-1));
			f->mode &= ~SF_PEEK;
			SFMTXRETURN(f, 0);
		}
		else
		{	if((uchar*)buf != f->next)
				SFMTXRETURN(f, (ssize_t)(-1));
			f->mode &= ~SF_PEEK;
			if(f->mode&SF_PKRD)
			{	/* actually read the data now */
				f->mode &= ~SF_PKRD;
				if(n > 0)
					n = (r = read(f->file,f->data,n)) < 0 ? 0 : r;
				f->endb = f->data+n;
				f->here += n;
			}
			f->next += n;
			f->endr = f->endb;
			SFMTXRETURN(f, n);
		}
	}

	s = begs = (uchar*)buf;
	for(;; f->mode &= ~SF_LOCK)
	{	/* check stream mode */
		if(SFMODE(f,local) != SF_READ && _sfmode(f,SF_READ,local) < 0)
		{	n = s > begs ? s-begs : (size_t)(-1);
			SFMTXRETURN(f, (ssize_t)n);
		}

		SFLOCK(f,local);

		if((r = f->endb - f->next) > 0) /* has buffered data */
		{	if(r > (ssize_t)n)
				r = (ssize_t)n;
			if(s != f->next)
				memcpy(s, f->next, r);
			f->next += r;
			s += r;
			n -= r;
		}

		if(n <= 0)	/* all done */
			break;

		if(!(f->flags&SF_STRING) && !(f->bits&SF_MMAP) )
		{	f->next = f->endb = f->data;

			/* exact IO is desirable for these cases */
			if(SFDIRECT(f,n) ||
			   ((f->flags&SF_SHARE) && f->extent < 0) )
				r = (ssize_t)n;
			else if(justseek && n <= f->iosz && f->iosz <= f->size)
				r = f->iosz;	/* limit buffering */
			else	r = f->size;	/* full buffering */

			/* if read almost full size, then just do it direct */
			if(r > (ssize_t)n && (r - r/8) <= (ssize_t)n)
				r = (ssize_t)n;

			/* read directly to user's buffer */
			if(r == (ssize_t)n && (r = SFRD(f,s,r,f->disc)) >= 0)
			{	s += r;
				n -= r;
				if(r == 0 || n == 0) /* eof or eob */ 
					break;
			}
			else	goto do_filbuf;
		}
		else
		{ do_filbuf:
			if(justseek)
				f->bits |= SF_JUSTSEEK;
			if(SFFILBUF(f,-1) <= 0)
				break;
		}
	}

	SFOPEN(f,local);
	r = s-begs;
	SFMTXRETURN(f, r);
}
