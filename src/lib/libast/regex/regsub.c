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
 * posix regex ed-style substitute
 */

#include "reglib.h"

/*
 * do a single substitution
 */

static int
sub(register Sfio_t* dp, const char* op, register const char* sp, size_t nmatch, register regmatch_t* match, register regflags_t flags, int sre)
{
	register int	c;
	char*		s;
	char*		e;
	const char*	b;
	regflags_t	f;

	f = flags &= (REG_SUB_LOWER|REG_SUB_UPPER);
	for (;;)
	{
		switch (c = *sp++)
		{
		case 0:
			return 0;
		case '~':
			if (!sre || *sp != '(')
			{
				sfputc(dp, c);
				continue;
			}
			b = sp - 1;
			sp++;
			break;
		case '\\':
			if (sre)
			{
				sfputc(dp, chresc(sp - 1, &s));
				sp = (const char*)s;
				continue;
			}
			if (*sp == '&')
			{
				c = *sp++;
				sfputc(dp, c);
				continue;
			}
			break;
		case '&':
			if (sre)
			{
				sfputc(dp, c);
				continue;
			}
			sp--;
			break;
		default:
			switch (flags)
			{
			case REG_SUB_UPPER:
				if (islower(c))
					c = toupper(c);
				break;
			case REG_SUB_LOWER:
				if (isupper(c))
					c = tolower(c);
				break;
			case REG_SUB_UPPER|REG_SUB_LOWER:
				if (isupper(c))
					c = tolower(c);
				else if (islower(c))
					c = toupper(c);
				break;
			}
			sfputc(dp, c);
			continue;
		}
		switch (c = *sp++)
		{
		case 0:
			sp--;
			continue;
		case '&':
			c = 0;
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			c -= '0';
			if (sre)
				while (isdigit(*sp))
					c = c * 10 + *sp++ - '0';
			break;
		case 'l':
			if (sre && *sp != ')')
			{
				c = -1;
				break;
			}
			if (c = *sp)
			{
				sp++;
				if (isupper(c))
					c = tolower(c);
				sfputc(dp, c);
			}
			continue;
		case 'u':
			if (sre)
			{
				if (*sp != ')')
				{
					c = -1;
					break;
				}
				sp++;
			}
			if (c = *sp)
			{
				sp++;
				if (islower(c))
					c = toupper(c);
				sfputc(dp, c);
			}
			continue;
		case 'E':
			if (sre)
			{
				if (*sp != ')')
				{
					c = -1;
					break;
				}
				sp++;
			}
			flags = f;
			continue;
		case 'L':
			if (sre)
			{
				if (*sp != ')')
				{
					c = -1;
					break;
				}
				sp++;
			}
			f = flags;
			flags = REG_SUB_LOWER;
			continue;
		case 'U':
			if (sre)
			{
				if (*sp != ')')
				{
					c = -1;
					break;
				}
				sp++;
			}
			f = flags;
			flags = REG_SUB_UPPER;
			continue;
		default:
			if (!sre)
			{
				sfputc(dp, chresc(sp - 2, &s));
				sp = (const char*)s;
				continue;
			}
			sp--;
			c = -1;
			break;
		}
		if (sre)
		{
			if (c < 0 || *sp != ')')
			{
				for (; b < sp; b++)
					sfputc(dp, *b);
				continue;
			}
			sp++;
		}
		if (c >= nmatch)
			return REG_ESUBREG;
		s = (char*)op + match[c].rm_so;
		e = (char*)op + match[c].rm_eo;
		while (s < e)
		{
			c = *s++;
			switch (flags)
			{
			case REG_SUB_UPPER:
				if (islower(c))
					c = toupper(c);
				break;
			case REG_SUB_LOWER:
				if (isupper(c))
					c = tolower(c);
				break;
			case REG_SUB_UPPER|REG_SUB_LOWER:
				if (isupper(c))
					c = tolower(c);
				else if (islower(c))
					c = toupper(c);
				break;
			}
			sfputc(dp, c);
		}
	}
}

/*
 * ed(1) style substitute using matches from last regexec()
 */

int
regsub(const regex_t* p, Sfio_t* dp, const char* op, const char* sp, size_t nmatch, regmatch_t* match, regflags_t flags)
{
	int	r;
	int	sre;

	if ((p->env->flags & REG_NOSUB) || !nmatch)
		return REG_BADPAT;
	sre = !!(p->env->flags & REG_SHELL);
	do
	{
		sfwrite(dp, op, match->rm_so);
		if (r = sub(dp, op, sp, nmatch, match, flags, sre))
			return fatal(p->env->disc, r, NiL);
		op += match->rm_eo;
	} while ((flags & REG_SUB_ALL) && *op && match->rm_so != match->rm_eo && !(r = regexec(p, op, nmatch, match, p->env->flags)));
	sfputr(dp, op, -1);
	return r == REG_NOMATCH ? 0 : r;
}
