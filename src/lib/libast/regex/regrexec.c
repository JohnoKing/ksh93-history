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
 * posix regex record executor
 * multiple record sized-buffer interface
 */

#include "reglib.h"

/*
 * call regnexec() on records selected by Boyer-Moore
 */

int
regrexec(const regex_t* p, const char* s, size_t len, size_t nmatch, regmatch_t* match, regflags_t flags, int sep, void* handle, regrecord_t record)
{
	register unsigned char*	buf = (unsigned char*)s;
	register unsigned char*	beg;
	register unsigned char*	l;
	register unsigned char*	r;
	register unsigned char*	x;
	register size_t*	skip;
	register size_t*	fail;
	register Bm_mask_t**	mask;
	register size_t		index;
	register int		n;
	unsigned char*		end;
	size_t			mid;
	int			complete;
	int			exactlen;
	int			leftlen;
	int			rightlen;
	int			inv;
	Bm_mask_t		m;
	Env_t*			env;
	Rex_t*			e;

	if (!s || !p || !(env = p->env) || (e = env->rex)->type != REX_BM)
		return REG_BADPAT;
#if __OBSOLETE__ && __OBSOLETE__ < 20030101
	/*
	 * repeat 1000x: sharing bits is never worth it
	 */

	if (flags & REG_MULTIPLE)
	{
		flags &= ~REG_MULTIPLE;
		flags |= REG_INVERT;
	}
	if (flags & REG_DELIMITED)
	{
		flags &= ~REG_DELIMITED;
		flags |= REG_STARTEND;
	}
#endif
	inv = (flags & REG_INVERT) != 0;
	buf = beg = (unsigned char*)s;
	end = buf + len;
	mid = (len < e->re.bm.right) ? 0 : (len - e->re.bm.right);
	skip = e->re.bm.skip;
	fail = e->re.bm.fail;
	mask = e->re.bm.mask;
	complete = e->re.bm.complete && !nmatch;
	exactlen = e->re.bm.size;
	leftlen = e->re.bm.left + exactlen;
	rightlen = exactlen + e->re.bm.right;
	env->rex = e->next;
	index = leftlen++;
	for (;;)
	{
		while ((index += skip[buf[index]]) < mid);
		if (index < HIT)
			goto impossible;
		index -= HIT;
		m = mask[n = exactlen - 1][buf[index]];
		do
		{
			if (!n--)
				goto possible;
		} while (m &= mask[n][buf[--index]]);
		if ((index += fail[n + 1]) < len)
			continue;
 impossible:
		if (inv)
		{
			l = r = buf + len;
			goto invert;
		}
		n = 0;
		goto done;
 possible:
		r = (l = buf + index) + exactlen;
		while (l > beg)
			if (*--l == sep)
			{
				l++;
				break;
			}
		if ((r - l) < leftlen)
			goto spanned;
		while (r < end && *r != sep)
			r++;
		if ((r - (buf + index)) < rightlen)
			goto spanned;
		if (complete || !(n = regnexec(p, (char*)l, r - l, nmatch, match, flags)))
		{
			if (inv)
			{
 invert:
				x = beg;
				while (beg < l)
				{
					while (x < l && *x != sep)
						x++;
					if (n = (*record)(handle, (char*)beg, x - beg))
						goto done;
					beg = ++x;
				}
			}
			else if (n = (*record)(handle, (char*)l, r - l))
				goto done;
			if ((index = (r - buf) + leftlen) >= len)
			{
				n = (inv && (++r - buf) < len) ? (*record)(handle, (char*)r, (buf + len) - r): 0;
				goto done;
			}
			beg = r + 1;
		}
		else if (n != REG_NOMATCH)
			goto done;
		else
		{
 spanned:
			if ((index += exactlen) >= mid)
				goto impossible;
		}
	}
 done:
	env->rex = e;
	return n;
}
