/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*                  Copyright (c) 1982-2004 AT&T Corp.                  *
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
*                  David Korn <dgk@research.att.com>                   *
*                                                                      *
***********************************************************************/
#pragma prototyped
#include	<ast.h>
#include	<signal.h>
#include	"FEATURE/options"
#include	"FEATURE/dynamic"
#include	"shtable.h"
#include	"name.h"

/*
 * This is the table of built-in aliases.  These should be exported.
 */

const struct shtable2 shtab_aliases[] =
{
#if SHOPT_FS_3D
	"2d",		NV_NOFREE,	"set -f;_2d",
#endif /* SHOPT_FS_3D */
	"autoload",	NV_NOFREE,	"typeset -fu",
	"command",	NV_NOFREE,	"command ",
	"fc",		NV_NOFREE,	"hist",
	"float",	NV_NOFREE,	"typeset -E",
	"functions",	NV_NOFREE,	"typeset -f",
	"hash",		NV_NOFREE,	"alias -t --",
	"history",	NV_NOFREE,	"hist -l",
	"integer",	NV_NOFREE,	"typeset -i",
	"nameref",	NV_NOFREE,	"typeset -n",
	"nohup",	NV_NOFREE,	"nohup ",
	"r",		NV_NOFREE,	"hist -s",
	"redirect",	NV_NOFREE,	"command exec",
	"source",	NV_NOFREE,	"command .",
#ifdef SIGTSTP
	"stop",		NV_NOFREE,	"kill -s STOP",
	"suspend", 	NV_NOFREE,	"kill -s STOP $$",
#endif /*SIGTSTP */
	"times",	NV_NOFREE,	"{ { time;} 2>&1;}",
	"type",		NV_NOFREE,	"whence -v",
	"",		0,		(char*)0
};

