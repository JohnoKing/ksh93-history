/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*          Copyright (c) 1985-2008 AT&T Intellectual Property          *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                    by AT&T Intellectual Property                     *
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

static const char id[] = "\n@(#)$Id: ast (AT&T Research) 2008-04-01 $\0\n";

#include <ast.h>

#undef	strcmp

_Ast_info_t	_ast_info =
{
	"libast",
	{ 0, 0 },
	0, 0, 0, 0, 0,
	strcmp,
	0, 0,
	1
};

__EXTERN__(_Ast_info_t, _ast_info);
