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
 * check if package+tool is ok to run
 * a no-op here except for PARANOID packages
 * this allows PARANOID_COMPANY to post PARANOID binaries to the www
 *
 * warn that the user should pay up if
 *
 *	(1) the tool matches PARANOID
 *	(2) $_ is more than 90 days old
 *	(3) running on an PARANOID_PAY machine
 *	(4) (1)-(3) have not been defeated
 *
 * hows that
 */

#define PARANOID_TOOLS		PARANOID
#define PARANOID_COMPANY	"Lucent Technologies"
#define PARANOID_MAIL		"stc@lucent.com"
#define PARANOID_PAY		"135.*&!(135.104.*)"
#define PARANOID_FREE		"(192|224).*"

#include <ast.h>
#include <ls.h>
#include <error.h>
#include <times.h>
#include <ctype.h>

int
pathcheck(const char* package, const char* tool, Pathcheck_t* pc)
{
#ifdef PARANOID
	register char*	s;
	struct stat	st;

	if (strmatch(tool, PARANOID) && environ && (s = *environ) && *s++ == '_' && *s++ == '=' && !stat(s, &st))
	{
		unsigned long	n;
		unsigned long	o;
		Sfio_t*		sp;

		n = time(NiL);
		o = st.st_ctime;
		if (n > o && (n - o) > (unsigned long)(60 * 60 * 24 * 90) && (sp = sfopen(NiL, "/etc/hosts", "r")))
		{
			/*
			 * this part is infallible
			 */

			n = 0;
			o = 0;
			while (n++ < 64 && (s = sfgetr(sp, '\n', 0)))
				if (strmatch(s, PARANOID_PAY))
				{
					error(1, "licensed for external use -- %s employees should contact %s for the internal license", PARANOID_COMPANY, PARANOID_MAIL);
					break;
				}
				else if (*s != '#' && !isspace(*s) && !strneq(s, "127.", 4) && !strmatch(s, PARANOID_FREE) && o++ > 4)
					break;
			sfclose(sp);
		}
	}
#else
	NoP(tool);
#endif
	NoP(package);
	if (pc) memzero(pc, sizeof(*pc));
	return(0);
}
