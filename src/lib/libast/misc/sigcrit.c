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
 * Glenn Fowler
 * AT&T Research
 *
 * signal critical region support
 */

#include <ast.h>
#include <sig.h>

static struct
{
	int	sig;
	int	op;
}
signals[] =		/* held inside critical region	*/
{
	SIGINT,		SIG_REG_EXEC,
#ifdef SIGPIPE
	SIGPIPE,	SIG_REG_EXEC,
#endif
#ifdef SIGQUIT
	SIGQUIT,	SIG_REG_EXEC,
#endif
#ifdef SIGHUP
	SIGHUP,		SIG_REG_EXEC,
#endif
#ifdef SIGCLD
	SIGCLD,		SIG_REG_PROC,
#endif
#ifdef SIGCHLD
	SIGCHLD,	SIG_REG_PROC,
#endif
#ifdef SIGTSTP
	SIGTSTP,	SIG_REG_TERM,
#endif
#ifdef SIGTTIN
	SIGTTIN,	SIG_REG_TERM,
#endif
#ifdef SIGTTOU
	SIGTTOU,	SIG_REG_TERM,
#endif
};

#ifndef SIG_SETMASK
#undef	_lib_sigprocmask
#endif

#if !_lib_sigprocmask && !_lib_sigsetmask

static long	hold;			/* held signal mask		*/

/*
 * hold last signal for later delivery
 */

static void
interrupt(int sig)
{
	signal(sig, interrupt);
	hold |= sigmask(sig);
}

#endif

/*
 * critical signal region handler
 *
 * op>0		new region according to SIG_REG_*, return region level
 * op==0	pop region, return region level
 * op<0		return non-zero if any signals held in current region
 *
 * signals[] held until region popped
 */

int
sigcritical(int op)
{
	register int		i;
	static int		region;
	static int		level;
#if _lib_sigprocmask
	static sigset_t		mask;
	sigset_t		nmask;
#else
#if _lib_sigsetmask
	static long		mask;
#else
	static Sig_handler_t	handler[elementsof(signals)];
#endif
#endif

	if (op > 0)
	{
		if (!level++)
		{
			region = op;
			if (op & SIG_REG_SET)
				level--;
#if _lib_sigprocmask
			sigemptyset(&nmask);
			for (i = 0; i < elementsof(signals); i++)
				if (op & signals[i].op)
					sigaddset(&nmask, signals[i].sig);
			sigprocmask(SIG_BLOCK, &nmask, &mask);
#else
#if _lib_sigsetmask
			mask = 0;
			for (i = 0; i < elementsof(signals); i++)
				if (op & signals[i].op)
					mask |= sigmask(signals[i].sig);
			mask = sigblock(mask);
#else
			hold = 0;
			for (i = 0; i < elementsof(signals); i++)
				if (op & signals[i].op)
				{
					if ((handler[i] = signal(signals[i].sig, interrupt)) == SIG_IGN)
					{
						signal(signals[i].sig, SIG_IGN);
						hold &= ~sigmask(signals[i].sig);
					}
				}
#endif
#endif
		}
		return(level);
	}
	else if (op < 0)
	{
#if _lib_sigprocmask
		sigpending(&nmask);
		for (i = 0; i < elementsof(signals); i++)
			if (region & signals[i].op)
			{
				if (sigismember(&nmask, signals[i].sig))
					return(1);
			}
		return(0);
#else
#if _lib_sigsetmask
		/* no way to get pending signals without installing handler */
		return(0);
#else
		return(hold != 0);
#endif
#endif
	}
	else
	{
		/*
		 * a vfork() may have intervened so we
		 * allow apparent nesting mismatches
		 */

		if (--level <= 0)
		{
			level = 0;
#if _lib_sigprocmask
			sigprocmask(SIG_SETMASK, &mask, NiL);
#else
#if _lib_sigsetmask
			sigsetmask(mask);
#else
			for (i = 0; i < elementsof(signals); i++)
				if (region & signals[i].op)
					signal(signals[i].sig, handler[i]);
			if (hold)
			{
				for (i = 0; i < elementsof(signals); i++)
					if (region & signals[i].op)
					{
						if (hold & sigmask(signals[i].sig))
							kill(getpid(), signals[i].sig);
					}
				pause();
			}
#endif
#endif
		}
		return(level);
	}
}
