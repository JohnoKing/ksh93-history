/*******************************************************************
*                                                                  *
*             This software is part of the ast package             *
*                Copyright (c) 1992-2001 AT&T Corp.                *
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
*******************************************************************/
#pragma prototyped
/*
 * David Korn
 * Glenn Fowler
 * AT&T Research
 *
 * uname
 */

static const char usage[] =
"[-?\n@(#)$Id: uname (AT&T Labs Research) 2001-05-01 $\n]"
USAGE_LICENSE
"[+NAME?uname - identify the current system ]"
"[+DESCRIPTION?By default \buname\b writes the operating system name to"
"	standard output. When options are specified, one or more"
"	system characteristics are written to standard output, space"
"	separated, on a single line. When more than one option is specifed"
"	the output is in the order specfied by the \b-A\b option below."
"	Unsupported option values are listed as \a[option]]\a. If any unknown"
"	options are specified then the local \b/usr/bin/uname\b is called.]"
"[a:all?Equivalent to \b-snrvm\b.]"
"[d:domain?The domain name returned by \agetdomainname\a(2).]"
"[h:host-id|id?The host id in hex.]"
"[i:implementation|platform?The hardware implementation (platform);"
"	this is \b--host-id\b on some systems.]"
"[m:machine?The name of the hardware type the system is running on.]"
"[n:nodename?The hostname or nodename.]"
"[p:processor?The name of the processor instruction set architecture.]"
"[r:release?The release level of the operating system implementation.]"
"[s:os|system|sysname?The operating system name. This is the default.]"
"[v:version?The operating system implementation version level.]"
"[A:everything?Equivalent to \b-snrvmphCdtbiRX\b.]"
"[R:extended-release?The extended release name.]"
"[S:sethost?Set the hostname or nodename to \aname\a. No output is"
"	written to standard output.]:[name]"

"[+SEE ALSO?\bhostname\b(1), \bgetconf\b(1), \buname\b(2), \bsysconf\b(2)]"
;

#if defined(__STDPP__directive) && defined(__STDPP__hide)
__STDPP__directive pragma pp:hide getdomainname gethostid gethostname sethostname
#else
#define getdomainname	______getdomainname
#define gethostid	______gethostid
#define gethostname	______gethostname
#define sethostname	______sethostname
#endif

#include <cmdlib.h>
#include <ctype.h>

#include "FEATURE/utsname"

#define MAXHOSTNAME	64

#if _lib_uname && _sys_utsname

#include <sys/utsname.h>

#endif

#if _lib_syssgi && _sys_syssgi

#include <sys/syssgi.h>

#endif

#if _sys_systeminfo
#if !_lib_systeminfo && _lib_syscall && _sys_syscall
#include <sys/syscall.h>
#if defined(SYS_systeminfo)
#define _lib_systeminfo		1
#define systeminfo(a,b,c)	syscall(SYS_systeminfo,a,b,c)
#endif
#endif
#if _lib_systeminfo
#if !defined(SYS_NMLEN)
#define SYS_NMLEN	9
#endif
#include <sys/systeminfo.h>
#endif
#endif

#if defined(__STDPP__directive) && defined(__STDPP__hide)
__STDPP__directive pragma pp:nohide getdomainname gethostid gethostname sethostname
#else
#undef	getdomainname
#undef	gethostid
#undef	gethostname
#undef	sethostname
#endif

#if _lib_getdomainname
extern int	getdomainname(char*, size_t);
#endif
#if _lib_gethostid
extern int	gethostid(void);
#endif
#if _lib_gethostname
extern int	gethostname(char*, size_t);
#endif
#if _lib_sethostname
extern int	sethostname(const char*, size_t);
#endif

#ifndef HOSTTYPE
#define HOSTTYPE	"unknown"
#endif

static char	hosttype[] = HOSTTYPE;

#if !_lib_uname || !_sys_utsname

#if defined(__STDPP__)
#define SYSNAME		#(getprd machine)
#define RELEASE		#(getprd release)
#define VERSION		#(getprd version)
#define MACHINE		#(getprd architecture)
#else
#define SYSNAME		""
#define RELEASE		""
#define VERSION		""
#define MACHINE		""
#endif

struct utsname
{
	char*	sysname;
	char	nodename[MAXHOSTNAME];
	char*	release;
	char*	version;
	char*	machine;
};

int
uname(register struct utsname* ut)
{
#ifdef HOSTTYPE
	char*		sys = 0;
	char*		arch = 0;

	if (*hosttype)
	{
		sys = hosttype;
		if (arch = strchr(sys, '.'))
		{
			*arch++ = 0;
			if (!*arch)
				arch = 0;
		}
		if (!*sys)
			sys = 0;
	}
#endif
#ifdef _lib_gethostname
	if (gethostname(ut->nodename, sizeof(ut->nodename) - 1))
		return -1;
#else
	strncpy(ut->nodename, "local", sizeof(ut->nodename) - 1);
#endif
#ifdef HOSTTYPE
	if (!(ut->sysname = sys))
#endif
	if (!*(ut->sysname = SYSNAME))
		ut->sysname = ut->nodename;
#ifdef HOSTTYPE
	if (!(ut->machine = arch))
#endif
	ut->machine = MACHINE;
	ut->release = RELEASE;
	ut->version = VERSION;
	return 0;
}

#endif

#define OPT_system		(1<<0)
#define OPT_nodename		(1<<1)
#define OPT_release		(1<<2)
#define OPT_version		(1<<3)
#define OPT_machine		(1<<4)

#define OPT_ALL			5

#define OPT_processor		(1<<5)
#define OPT_hostid		(1<<6)
#define OPT_vendor		(1<<7)
#define OPT_domain		(1<<8)
#define OPT_machine_type	(1<<9)
#define OPT_base		(1<<10)
#define OPT_implementation	(1<<11)
#define OPT_extended_release	(1<<12)
#define OPT_extra		(1<<13)

#define OPT_TOTAL		14

#define OPT_all			(1L<<29)
#define OPT_total		(1L<<30)

#ifndef MACHINE
#if defined(__STDPP__)
#define MACHINE			#(getprd architecture)
#else
#define MACHINE			""
#endif
#endif

#ifndef HOSTTYPE
#define HOSTTYPE		"unknown"
#endif

#define extra(m)        do \
			{ \
				if ((char*)&ut.m[sizeof(ut.m)] > last) \
					last = (char*)&ut.m[sizeof(ut.m)]; \
			} while(0)

#define output(f,v,u)	do \
			{ \
				if ((flags&(f))&&(*(v)||!(flags&OPT_total))) \
				{ \
					if (sep) \
						sfputc(sfstdout, ' '); \
					else \
						sep = 1; \
					if (*(v)) \
						sfputr(sfstdout, v, -1); \
					else \
						sfprintf(sfstdout, "[%s]", u); \
				} \
			} while (0)

int
b_uname(int argc, char** argv, void* context)
{
	register long	flags = 0;
	register int	sep = 0;
	register int	n;
	register char*	s;
	char*		t;
	char*		sethost = 0;
	struct utsname	ut;
	char		buf[257];

	NoP(argc);
	cmdinit(argv, context, ERROR_CATALOG);
	for (;;)
	{
		switch (optget(argv, usage))
		{
		case 'a':
			flags |= OPT_all|((1L<<OPT_ALL)-1);
			continue;
		case 'b':
			flags |= OPT_base;
			continue;
		case 'c':
			flags |= OPT_vendor;
			continue;
		case 'd':
			flags |= OPT_domain;
			continue;
		case 'h':
			flags |= OPT_hostid;
			continue;
		case 'i':
			flags |= OPT_implementation;
			continue;
		case 'm':
			flags |= OPT_machine;
			continue;
		case 'n':
			flags |= OPT_nodename;
			continue;
		case 'p':
			flags |= OPT_processor;
			continue;
		case 'r':
			flags |= OPT_release;
			continue;
		case 's':
			flags |= OPT_system;
			continue;
		case 't':
			flags |= OPT_machine_type;
			continue;
		case 'v':
			flags |= OPT_version;
			continue;
		case 'x':
			flags |= OPT_extra;
			continue;
		case 'A':
			flags |= OPT_total|((1L<<OPT_TOTAL)-1);
			continue;
		case 'R':
			flags |= OPT_extended_release;
			continue;
		case 'S':
			sethost = opt_info.arg;
			continue;
		case ':':
			argv[0] = "/usr/bin/uname";
			if (!access(argv[0], X_OK) || !access(argv[0]+=4, X_OK))
				execv(argv[0], argv);
			error(2, "%s", opt_info.arg);
			break;
		case '?':
			error(ERROR_usage(2), "%s", opt_info.arg);
			break;
		}
		break;
	}
	argv += opt_info.index;
	if (error_info.errors || *argv || sethost && flags)
		error(ERROR_usage(2), "%s", optusage(NiL));
	if (sethost)
	{
#if _lib_sethostname
		if (sethostname(sethost, strlen(sethost) + 1))
#else
#ifdef	ENOSYS
		errno = ENOSYS;
#else
		errno = EPERM;
#endif
#endif
		error(ERROR_system(1), "%s: cannot set host name", sethost);
	}
	else
	{
		s = buf;
		if (!flags)
			flags = OPT_system;
		memzero(&ut, sizeof(ut));
		if (uname(&ut) < 0)
			error(ERROR_usage(2), "information unavailable");
		output(OPT_system, ut.sysname, "sysname");
		if (flags & OPT_nodename)
		{
#if !_mem_nodeext_utsname && _lib_gethostname
			if (sizeof(ut.nodename) > 9 || gethostname(s, sizeof(buf)))
#endif
			s = ut.nodename;
			output(OPT_nodename, s, "nodename");
		}
		output(OPT_release, ut.release, "release");
		output(OPT_version, ut.version, "version");
		output(OPT_machine, ut.machine, "machine");
		if (flags & OPT_processor)
		{
#if defined(SI_ARCHITECTURE)
			if ((n = systeminfo(SI_ARCHITECTURE, s, sizeof(buf) - 1)) > 0)
				s[n] = 0;
			else
#endif
			{
				if (t = strchr(hosttype, '.'))
					t++;
				else
					t = hosttype;
				strncpy(s, t, sizeof(buf) - 1);
			}
			output(OPT_processor, s, "processor");
		}
		if (flags & OPT_implementation)
		{
#if defined(SI_PLATFORM)
			if ((n = systeminfo(SI_PLATFORM, s, sizeof(buf) - 1)) > 0)
				s[n] = 0;
			else
#endif
				*s = 0;
			output(OPT_implementation, s, "implementation");
		}
		if (flags & OPT_extended_release)
		{
#if _lib_syssgi && defined(SGI_RELEASE_NAME)
			if ((n = syssgi(SGI_RELEASE_NAME, sizeof(buf), s)) < 0)
#endif
				*s = 0;
			output(OPT_extended_release, s, "extended-release");
		}
#if _mem_idnumber_utsname
		output(OPT_hostid, ut.idnumber, "hostid");
#else
		if (flags & OPT_hostid)
		{
#if _lib_gethostid
			sfsprintf(s, sizeof(buf), "%08x", gethostid());
#else
#if _lib_gethostid || defined(SI_HW_SERIAL)
			if ((n = systeminfo(SI_HW_SERIAL, s, sizeof(buf) - 1)) > 0)
				s[n] = 0;
			else
#endif
				*s = 0;
#endif
			output(OPT_hostid, s, "hostid");
		}
#endif
		if (flags & OPT_vendor)
		{
#if defined(SI_HW_PROVIDER)
			if ((n = systeminfo(SI_HW_PROVIDER, s, sizeof(buf) - 1)) > 0)
				s[n] = 0;
			else
#endif
				*s = 0;
			output(OPT_vendor, s, "vendor");
		}
		if (flags & OPT_domain)
		{
#if _lib_getdomainname
			if (getdomainname(s, sizeof(buf)) < 0)
#else
#if defined(SI_SRPC_DOMAIN)
			if ((n = systeminfo(SI_SRPC_DOMAIN, s, sizeof(buf) - 1)) > 0)
				s[n] = 0;
			else
#endif
#endif
				*s = 0;
			output(OPT_domain, s, "domain");
		}
#if _mem_m_type_utsname
		output(OPT_machine_type, ut.m_type, "m_type");
#else
		*s = 0;
		output(OPT_machine_type, s, "m_type");
#endif
#if _mem_base_rel_utsname
		output(OPT_base, ut.base_rel, "base_rel");
#else
		*s = 0;
		output(OPT_base, s, "base_rel");
#endif
		if (flags & OPT_extra)
		{
			char*	last = (char*)&ut;

			extra(sysname);
			extra(nodename);
			extra(release);
			extra(version);
			extra(machine);
#if _mem_idnumber_utsname
			extra(idnumber);
#endif
#if _mem_m_type_utsname
			extra(m_type);
#endif
#if _mem_base_rel_utsname
			extra(base_rel);
#endif
			if (last < ((char*)(&ut + 1)))
			{
				s = t = last;
				while (s < (char*)(&ut + 1))
				{
					if (!(n = *s++))
					{
						if ((s - t) > 1)
						{
							if (sep)
								sfputc(sfstdout, ' ');
							else
								sep = 1;
							sfputr(sfstdout, t, -1);
						}
						t = s;
					}
					else if (!isprint(n))
						break;
				}
			}
		}
		if (sep)
			sfputc(sfstdout, '\n');
	}
	return error_info.errors;
}
