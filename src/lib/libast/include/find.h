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
 * Glenn Fowler
 * AT&T Research
 *
 * fast find interface definitions
 */

#ifndef _FIND_H
#define _FIND_H

#define FIND_VERSION	19980301L

#ifndef FIND_CODES
#define FIND_CODES	"lib/find/codes"
#endif

#define FIND_CODES_ENV	"FINDCODES"

#define FIND_GENERATE	(1<<0)		/* generate new codes		*/
#define FIND_ICASE	(1<<1)		/* ignore case in match		*/
#define FIND_GNU	(1<<2)		/* generate gnu format codes	*/
#define FIND_OLD	(1<<3)		/* generate old format codes	*/
#define FIND_TYPE	(1<<4)		/* generate type with codes	*/
#define FIND_VERIFY	(1<<5)		/* verify the dir hierarchy	*/

#define FIND_USER	(1L<<16)	/* first user flag bit		*/

struct Find_s;
struct Finddisc_s;

typedef int (*Finderror_f)(struct Find_s*, struct Finddisc_s*, int, const char*, ...);
typedef int (*Findverify_f)(struct Find_s*, const char*, size_t, struct Finddisc_s*);

typedef struct Finddisc_s
{
	unsigned long	version;	/* interface version		*/
	unsigned long	flags;		/* FIND_* flags			*/
	Finderror_f	errorf;		/* error function		*/
	Findverify_f	verifyf;	/* dir verify function		*/
	char**		dirs;		/* dir prefixes to search	*/
} Finddisc_t;

typedef struct Find_s
{
	const char*	id;		/* library id string		*/
	unsigned long	stamp;		/* codes time stamp		*/

#ifdef _FIND_PRIVATE_
	_FIND_PRIVATE_
#endif

} Find_t;

#if _BLD_ast && defined(__EXPORT__)
#define extern		__EXPORT__
#endif

extern Find_t*		findopen(const char*, const char*, const char*, Finddisc_t*);
extern char*		findread(Find_t*);
extern int		findwrite(Find_t*, const char*, size_t, const char*);
extern int		findclose(Find_t*);

#undef	extern

#endif
