/*******************************************************************
*                                                                  *
*             This software is part of the ast package             *
*                Copyright (c) 1985-2002 AT&T Corp.                *
*        and it may only be used by you under license from         *
*                       AT&T Corp. ("AT&T")                        *
*         A copy of the Source Code Agreement is available         *
*                at the AT&T Internet web site URL                 *
*                                                                  *
*       http://www.research.att.com/sw/license/ast-open.html       *
*                                                                  *
*    If you have copied or used this software without agreeing     *
*        to the terms of the license you are infringing on         *
*           the license and copyright and are violating            *
*               AT&T's intellectual property rights.               *
*                                                                  *
*            Information and Software Systems Research             *
*                        AT&T Labs Research                        *
*                         Florham Park NJ                          *
*                                                                  *
*               Glenn Fowler <gsf@research.att.com>                *
*                David Korn <dgk@research.att.com>                 *
*                 Phong Vo <kpv@research.att.com>                  *
*                                                                  *
*******************************************************************/
#include	"FEATURE/vmalloc"

#if _std_malloc || _BLD_INSTRUMENT || cray || defined(_UWIN)
void _STUB_malloc(){}
#else

/*	malloc compatibility functions.
**	These are aware of debugging/profiling and driven by the environment variables:
**	VMETHOD: select an allocation method by name.
**
**	VMPROFILE: if is a file name, write profile data to it.
**	VMTRACE: if is a file name, write trace data to it.
**	The pattern %p in a file name will be replaced by the process ID.
**
**	VMDEBUG:
**		a:			abort on any warning
**		[decimal]:		period to check arena.
**		0x[hexadecimal]:	address to watch.
**
**	Written by Kiem-Phong Vo, kpv@research.att.com, 01/16/94.
*/

#include	"vmhdr.h"

#if _hdr_stat
#include	<stat.h>
#else
#if _sys_stat
#include	<sys/stat.h>
#endif
#endif
#include	<fcntl.h>

#ifdef S_IRUSR
#define CREAT_MODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#else
#define CREAT_MODE	0644
#endif

#undef malloc
#undef free
#undef realloc
#undef calloc
#undef cfree
#undef memalign
#undef valloc

#if __STD_C
static Vmulong_t atou(char** sp)
#else
static Vmulong_t atou(sp)
char**	sp;
#endif
{
	char*		s = *sp;
	Vmulong_t	v = 0;

	if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X') )
	{	for(s += 2; *s; ++s)
		{	if(*s >= '0' && *s <= '9')
				v = (v << 4) + (*s - '0');
			else if(*s >= 'a' && *s <= 'f')
				v = (v << 4) + (*s - 'a') + 10;
			else if(*s >= 'A' && *s <= 'F')
				v = (v << 4) + (*s - 'A') + 10;
			else break;
		}
	}
	else
	{	for(; *s; ++s)
		{	if(*s >= '0' && *s <= '9')
				v = v*10 + (*s - '0');
			else break;
		}
	}

	*sp = s;
	return v;
}

static int		_Vmflinit = 0;
static Vmulong_t	_Vmdbcheck = 0;
static Vmulong_t	_Vmdbtime = 0;
static int		_Vmpffd = -1;
#define VMFLINIT() \
	{ if(!_Vmflinit)	vmflinit(); \
	  if(_Vmdbcheck && (++_Vmdbtime % _Vmdbcheck) == 0 && \
	     Vmregion->meth.meth == VM_MTDEBUG) \
		vmdbcheck(Vmregion); \
	}

#if __STD_C
static char* insertpid(char* begs, char* ends)
#else
static char* insertpid(begs,ends)
char*	begs;
char*	ends;
#endif
{	int	pid;
	char*	s;

	if((pid = getpid()) < 0)
		return NIL(char*);

	s = ends;
	do
	{	if(s == begs)
			return NIL(char*);
		*--s = '0' + pid%10;
	} while((pid /= 10) > 0);
	while(s < ends)
		*begs++ = *s++;

	return begs;
}

#if __STD_C
static int createfile(char* file)
#else
static int createfile(file)
char*	file;
#endif
{
	char	buf[1024];
	char	*next, *endb;

	next = buf;
	endb = buf + sizeof(buf);
	while(*file)
	{	if(*file == '%')
		{	switch(file[1])
			{
			case 'p' :
				if(!(next = insertpid(next,endb)) )
					return -1;
				file += 2;
				break;
			default :
				goto copy;
			}
		}
		else
		{ copy:
			*next++ = *file++;
		}

		if(next >= endb)
			return -1;
	}

	*next = '\0';
#if _PACKAGE_ast
	return open(buf,O_WRONLY|O_CREAT|O_TRUNC,CREAT_MODE);
#else
	return creat(buf,CREAT_MODE);
#endif
}

#if __STD_C
static void pfprint(void)
#else
static void pfprint()
#endif
{
	if(Vmregion->meth.meth == VM_MTPROFILE)
		vmprofile(Vmregion,_Vmpffd);
}

#if __STD_C
static int vmflinit(void)
#else
static int vmflinit()
#endif
{
	char*		env;
	Vmalloc_t*	vm;
	int		fd;
	Vmulong_t	addr;
	char*		file;
	int		line;
	Void_t*		func;

	/* this must be done now to avoid any inadvertent recursion (more below) */
	_Vmflinit = 1;
	VMFLF(Vmregion,file,line,func);

	/* if getenv() calls malloc(), the eventual region may not see this */
	vm = NIL(Vmalloc_t*);
	if((env = getenv("VMETHOD")) )
	{	if(strcmp(env,"Vmdebug") == 0 || strcmp(env,"vmdebug") == 0)
			vm = vmopen(Vmdcsbrk,Vmdebug,0);
		else if(strcmp(env,"Vmprofile") == 0 || strcmp(env,"vmprofile") == 0 )
			vm = vmopen(Vmdcsbrk,Vmprofile,0);
		else if(strcmp(env,"Vmlast") == 0 || strcmp(env,"vmlast") == 0 )
			vm = vmopen(Vmdcsbrk,Vmlast,0);
		else if(strcmp(env,"Vmbest") == 0 || strcmp(env,"vmbest") == 0 )
			vm = Vmheap;
	}

	if((!vm || vm->meth.meth == VM_MTDEBUG) &&
	   (env = getenv("VMDEBUG")) && env[0] )
	{	if(vm || (vm = vmopen(Vmdcsbrk,Vmdebug,0)) )
		{	reg int	setcheck = 0;

			while(*env)
			{	if(*env == 'a')
					vmset(vm,VM_DBABORT,1);

				if(*env < '0' || *env > '9')
					env += 1;
				else if(env[0] == '0' &&
					(env[1] == 'x' || env[1] == 'X') )
				{	if((addr = atou(&env)) != 0)
						vmdbwatch((Void_t*)addr);
				}
				else
				{	_Vmdbcheck = atou(&env);
					setcheck = 1;
				}
			}
			if(!setcheck)
				_Vmdbcheck = 1;
		}
	}

	if((!vm || vm->meth.meth == VM_MTPROFILE) &&
	   (env = getenv("VMPROFILE")) && env[0] )
	{	_Vmpffd = createfile(env);
		if(!vm)
			vm = vmopen(Vmdcsbrk,Vmprofile,0);
	}

	/* slip in the new region now so that malloc() will work fine */
	if(vm)
		Vmregion = vm;

	/* turn on tracing if requested */
	if((env = getenv("VMTRACE")) && env[0] && (fd = createfile(env)) >= 0)
	{	vmset(Vmregion,VM_TRACE,1);
		vmtrace(fd);
	}

	/* make sure that profile data is output upon exiting */
	if(vm && vm->meth.meth == VM_MTPROFILE)
	{	if(_Vmpffd < 0)
			_Vmpffd = 2;
		/* this may wind up calling malloc(), but region is ok now */
		atexit(pfprint);
	}
	else if(_Vmpffd >= 0)
	{	close(_Vmpffd);
		_Vmpffd = -1;
	}

	/* reset file and line number to correct values for the call */
	Vmregion->file = file;
	Vmregion->line = line;
	Vmregion->func = func;

	return 0;
}

#if __STD_C
Void_t* malloc(reg size_t size)
#else
Void_t* malloc(size)
reg size_t	size;
#endif
{
	VMFLINIT();
	return (*Vmregion->meth.allocf)(Vmregion,size);
}

#if __STD_C
Void_t* realloc(reg Void_t* data, reg size_t size)
#else
Void_t* realloc(data,size)
reg Void_t*	data;	/* block to be reallocated	*/
reg size_t	size;	/* new size			*/
#endif
{
	VMFLINIT();

#if _PACKAGE_ast
	if(data && !(Vmregion->data->mode&VM_TRUST) &&
	   (*Vmregion->meth.addrf)(Vmregion,data) != 0 )
	{	Void_t*	newdata;
		if((newdata = (*Vmregion->meth.allocf)(Vmregion,size)) )
			memcpy(newdata,data,size);
		return newdata;
	}
#endif

	return (*Vmregion->meth.resizef)(Vmregion,data,size,VM_RSCOPY|VM_RSMOVE);
}

#if __STD_C
void free(reg Void_t* data)
#else
void free(data)
reg Void_t*	data;
#endif
{
	VMFLINIT();

#if _PACKAGE_ast
	if(data && !(Vmregion->data->mode&VM_TRUST) &&
	   (*Vmregion->meth.addrf)(Vmregion,data) != 0 )
		return;
#endif

	(void)(*Vmregion->meth.freef)(Vmregion,data);
}

#if __STD_C
Void_t* calloc(reg size_t n_obj, reg size_t s_obj)
#else
Void_t* calloc(n_obj, s_obj)
reg size_t	n_obj;
reg size_t	s_obj;
#endif
{
	VMFLINIT();
	return (*Vmregion->meth.resizef)(Vmregion,NIL(Void_t*),n_obj*s_obj,VM_RSZERO);
}

#if __STD_C
void cfree(reg Void_t* data)
#else
void cfree(data)
reg Void_t*	data;
#endif
{
	VMFLINIT();

#if _PACKAGE_ast
	if(data && !(Vmregion->data->mode&VM_TRUST) &&
	   (*Vmregion->meth.addrf)(Vmregion,data) != 0 )
		return;
#endif

	(void)(*Vmregion->meth.freef)(Vmregion,data);
}

#if __STD_C
Void_t* memalign(reg size_t align, reg size_t size)
#else
Void_t* memalign(align, size)
reg size_t	align;
reg size_t	size;
#endif
{
	VMFLINIT();
	return (*Vmregion->meth.alignf)(Vmregion,size,align);
}

#if __STD_C
Void_t* valloc(reg size_t size)
#else
Void_t* valloc(size)
reg size_t	size;
#endif
{
	VMFLINIT();
	GETPAGESIZE(_Vmpagesize);
	return (*Vmregion->meth.alignf)(Vmregion,size,_Vmpagesize);
}

#if __STD_C
Void_t* pvalloc(reg size_t size)
#else
Void_t* pvalloc(size)
reg size_t	size;
#endif
{
	VMFLINIT();
	GETPAGESIZE(_Vmpagesize);
	return (*Vmregion->meth.alignf)(Vmregion,ROUND(size,_Vmpagesize),_Vmpagesize);
}

#if !_lib_alloca || _mal_alloca
#ifndef _stk_down
#define _stk_down	0
#endif
typedef struct _alloca_s	Alloca_t;
union _alloca_u
{	struct
	{	char*		addr;
		Alloca_t*	next;
	} head;
	char	array[ALIGN];
};
struct _alloca_s
{	union _alloca_u	head;
	Vmuchar_t	data[1];
};

#if __STD_C
Void_t* alloca(size_t size)
#else
Void_t* alloca(size)
size_t	size;
#endif
{	char		array[ALIGN];
	char*		file;
	int		line;
	Void_t*		func;
	reg Alloca_t*	f;
	static Alloca_t* Frame;

	VMFLINIT();
	VMFLF(Vmregion,file,line,func);
	while(Frame)
	{	if(( _stk_down && &array[0] > Frame->head.head.addr) ||
		   (!_stk_down && &array[0] < Frame->head.head.addr) )
		{	f = Frame;
			Frame = f->head.head.next;
			(void)(*Vmregion->meth.freef)(Vmregion,f);
		}
		else	break;
	}

	Vmregion->file = file;
	Vmregion->line = line;
	Vmregion->func = func;
	f = (Alloca_t*)(*Vmregion->meth.allocf)(Vmregion,size+sizeof(Alloca_t)-1);

	f->head.head.addr = &array[0];
	f->head.head.next = Frame;
	Frame = f;

	return (Void_t*)f->data;
}
#endif /*!_lib_alloca || _mal_alloca*/

#if _libc_malloc /* GNU __libc_* functions */
#if __STD_C
Void_t*	__libc_calloc(size_t n, size_t m) { return calloc(n, m); }
Void_t	__libc_cfree(Void_t* p) { cfree(p); }
Void_t	__libc_free(Void_t* p) { free(p); }
Void_t*	__libc_malloc(size_t n) { return malloc(n); }
Void_t*	__libc_memalign(size_t a, size_t n) { return memalign(a, n); }
Void_t*	__libc_pvalloc(size_t n) { return pvalloc(n); }
Void_t*	__libc_realloc(Void_t* p, size_t n) { return realloc(p, n); }
Void_t*	__libc_valloc(size_t n) { return valloc(n); }
#else
Void_t*	__libc_calloc(n,m) size_t n; size_t m; { return calloc(n, m); }
Void_t	__libc_cfree(p) Void_t* p; { cfree(p); }
Void_t	__libc_free(p) Void_t* p; { free(p); }
Void_t*	__libc_malloc(n) size_t n; { return malloc(n); }
Void_t*	__libc_memalign(a,n) size_t a; size_t n; { return memalign(a, n); }
Void_t*	__libc_pvalloc(n) size_t n; { return pvalloc(n); }
Void_t*	__libc_realloc(p,n) Void_t* p; size_t n; { return realloc(p, n); }
Void_t*	__libc_valloc(n) size_t n; { return valloc(n); }
#endif
#endif /* _libc_malloc */


#if _hdr_malloc /* need the mallint interface for statistics, etc. */

#define calloc	______calloc
#define free	______free
#define malloc	______malloc
#define realloc	______realloc

#include	<malloc.h>

#if _lib_mallopt
#if __STD_C
int mallopt(int cmd, int value)
#else
int mallopt(cmd, value)
int	cmd;
int	value;
#endif
{
	VMFLINIT();
	return 0;
}
#endif /*_lib_mallopt*/

#if _lib_mallinfo
#if __STD_C
struct mallinfo mallinfo(void)
#else
struct mallinfo mallinfo()
#endif
{
	Vmstat_t	sb;
	struct mallinfo	mi;

	VMFLINIT();
	memset(&mi,0,sizeof(mi));
	if(vmstat(Vmregion,&sb) >= 0)
	{	mi.arena = sb.extent;
		mi.ordblks = sb.n_busy+sb.n_free;
		mi.uordblks = sb.s_busy;
		mi.fordblks = sb.s_free;
	}
	return mi;
}
#endif /* _lib_mallinfo */

#if _lib_mstats
#if __STD_C
struct mstats mstats(void)
#else
struct mstats mstats()
#endif
{
	Vmstat_t	sb;
	struct mstats	ms;

	VMFLINIT();
	memset(&ms,0,sizeof(ms));
	if(vmstat(Vmregion,&sb) >= 0)
	{	ms.bytes_total = sb.extent;
		ms.chunks_used = sb.n_busy;
		ms.bytes_used = sb.s_busy;
		ms.chunks_free = sb.n_free;
		ms.bytes_free = sb.s_free;
	}
	return ms;
}
#endif /*_lib_mstats*/

#endif/*_hdr_malloc*/

#endif /*_std_malloc || _BLD_INSTRUMENT || cray || defined(_UWIN) */
