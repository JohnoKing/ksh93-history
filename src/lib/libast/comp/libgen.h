/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*           Copyright (c) 1985-2006 AT&T Knowledge Ventures            *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                      by AT&T Knowledge Ventures                      *
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
#pragma prototyped
/*
 * libgen interface definitions
 */

#ifndef _LIBGEN_H
#define _LIBGEN_H

#if _BLD_ast && defined(__EXPORT__)
#define extern		__EXPORT__
#endif
#if !_BLD_ast && defined(__IMPORT__)
#define extern		extern __IMPORT__
#endif

extern char*	__loc1;

#undef	extern

#if _BLD_ast && defined(__EXPORT__)
#define extern		__EXPORT__
#endif

extern char*	regcmp(const char*, ...);
extern char*	regex(const char*, const char*, ...);
extern char*	basename(char*);
extern char*	dirname(char*);

#undef	extern

#endif
