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
#pragma prototyped
/*
 * Glenn Fowler
 * AT&T Research
 *
 * virtual db file directory entry constants
 */

#ifndef VDB_MAGIC

#define VDB_MAGIC	"vdb"

#define VDB_DIRECTORY	"DIRECTORY"
#define VDB_UNION	"UNION"
#define VDB_DATE	"DATE"
#define VDB_MODE	"MODE"

#define VDB_DELIMITER	';'
#define VDB_IGNORE	'_'
#define VDB_FIXED	10
#define VDB_LENGTH	((int)sizeof(VDB_DIRECTORY)+2*(VDB_FIXED+1))
#define VDB_OFFSET	((int)sizeof(VDB_DIRECTORY))
#define VDB_SIZE	(VDB_OFFSET+VDB_FIXED+1)

#endif
