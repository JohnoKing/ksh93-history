# regression tests for the iffe command

TEST 01 'command line basics'

	EXEC	-r -v - hdr stdio
		OUTPUT - $'/* : : generated by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes'

	EXEC	-r -v -s bsh - hdr stdio

	EXEC	-r -v - hdr stdio,limits
		OUTPUT - $'/* : : generated by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _hdr_limits	1	/* #include <limits.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is limits.h a header ... yes'

	EXEC	-r -v -s bsh - hdr stdio,limits

	EXEC	-r -v - hdr,lib no_foo_bar,no_bar_foo stdio.h
		OUTPUT - $'/* : : generated by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is no_foo_bar.h a header ... no
iffe: test: is no_bar_foo.h a header ... no
iffe: test: is no_foo_bar a library function ... no
iffe: test: is no_bar_foo a library function ... no'

	EXEC	-r -v -s bsh - hdr,lib no_foo_bar,no_bar_foo stdio.h

	EXEC	-r -v - hdr no_foo_bar,no_bar_foo stdio.h : lib no_foo_bar,no_bar_foo stdio.h

	EXEC	-r -v -s bsh - hdr no_foo_bar,no_bar_foo stdio.h : lib no_foo_bar,no_bar_foo stdio.h

TEST 02 'file input basics'

	EXEC	-r -v - t1.iffe
		INPUT t1.iffe $'hdr stdio'
		OUTPUT - $'/* : : generated from t1.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes'

	EXEC	-r -v -s bsh - t1.iffe

	EXEC	-r -v - t2.iffe
		INPUT t2.iffe $'hdr stdio,limits'
		OUTPUT - $'/* : : generated from t2.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _hdr_limits	1	/* #include <limits.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is limits.h a header ... yes'

	EXEC	-r -v -s bsh - t2.iffe

	EXEC	-r -v - t3.iffe
		INPUT t3.iffe $'hdr,lib no_foo_bar,no_bar_foo stdio.h'
		OUTPUT - $'/* : : generated from t3.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is no_foo_bar.h a header ... no
iffe: test: is no_bar_foo.h a header ... no
iffe: test: is no_foo_bar a library function ... no
iffe: test: is no_bar_foo a library function ... no'

	EXEC	-r -v -s bsh - t3.iffe

	EXEC	-r -v - t3.iffe
		INPUT t3.iffe $'hdr no_foo_bar,no_bar_foo stdio.h
lib no_foo_bar,no_bar_foo stdio.h'

	EXEC	-r -v -s bsh - t3.iffe

TEST 03 'nested if'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if hdr stdio
	if lib open {
		HIT 1
	}
	elif lib close {
		HIT 2
	}
	else {
		HIT 3
	}
	endif
elif hdr limits {
	HIT 4
}
else {
	HIT 5
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _lib_open	1	/* open() in default lib(s) */
	HIT 1

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is open a library function ... yes'

	EXEC	-r -v -s bsh - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _lib_open	1	/* open() in default lib(s) */
HIT 1

#endif'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if hdr _XXX_stdio
	if lib open {
		HIT 1
	}
	elif lib close {
		HIT 2
	}
	else {
		HIT 3
	}
	endif
elif hdr limits {
	HIT 4
}
else {
	HIT 5
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_limits	1	/* #include <limits.h> ok */
HIT 4

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is _XXX_stdio.h a header ... no
iffe: test: is limits.h a header ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if hdr _XXX_stdio
	if lib open {
		HIT 1
	}
	elif lib close {
		HIT 2
	}
	else {
		HIT 3
	}
	endif
elif hdr _XXX_limits {
	HIT 4
}
else {
	HIT 5
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
HIT 5

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is _XXX_stdio.h a header ... no
iffe: test: is _XXX_limits.h a header ... no'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if hdr stdio
	if lib _XXX_open {
		HIT 1
	}
	elif lib close {
		HIT 2
	}
	else {
		HIT 3
	}
	endif
elif hdr limits {
	HIT 4
}
else {
	HIT 5
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _lib_close	1	/* close() in default lib(s) */
	HIT 2

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is _XXX_open a library function ... no
iffe: test: is close a library function ... yes'

	EXEC	-r -v -s bsh - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _lib_close	1	/* close() in default lib(s) */
HIT 2

#endif'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if hdr stdio
	if lib _XXX_open {
		HIT 1
	}
	elif lib _XXX_close {
		HIT 2
	}
	else {
		HIT 3
	}
	endif
elif hdr limits {
	HIT 4
}
else {
	HIT 5
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
	HIT 3

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is _XXX_open a library function ... no
iffe: test: is _XXX_close a library function ... no'

	EXEC	-r -v -s bsh - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
HIT 3

#endif'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if mem stat.st_atime sys/stat.h {
	#define ATIME	1
}
elif mem stat.st_ctime sys/stat.h {
	#define CTIME	1
}
elif mem stat.st_mtime sys/stat.h {
	#define MTIME	1
}
else pass{ no_stat_time=1 }end {
	#define NOTIME	1
}
endif
if ( !no_stat_time ) {
	#define YESTIME	1
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _sys_stat	1	/* #include <sys/stat.h> ok */
#define _mem_st_atime_stat	1	/* st_atime is a member of struct stat */
#define ATIME	1

#define YESTIME	1

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is sys/stat.h a header ... yes
iffe: test: is stat a type or typedef ... no
iffe: test: is st_atime a member of struct stat ... yes
iffe: test: is ( !no_stat_time ) true ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if mem foo_stat.st_atime sys/stat.h {
	#define ATIME	1
}
elif mem stat.st_ctime sys/stat.h {
	#define CTIME	1
}
elif mem stat.st_mtime sys/stat.h {
	#define MTIME	1
}
else pass{ no_stat_time=1 }end {
	#define NOTIME	1
}
endif
if ( !no_stat_time ) {
	#define YESTIME	1
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _sys_stat	1	/* #include <sys/stat.h> ok */
#define _mem_st_ctime_stat	1	/* st_ctime is a member of struct stat */
#define CTIME	1

#define YESTIME	1

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is sys/stat.h a header ... yes
iffe: test: is foo_stat a type or typedef ... no
iffe: test: is st_atime a member of struct foo_stat ... no
iffe: test: is stat a type or typedef ... no
iffe: test: is st_ctime a member of struct stat ... yes
iffe: test: is ( !no_stat_time ) true ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if mem foo_stat.st_atime sys/stat.h {
	#define ATIME	1
}
elif mem foo_stat.st_ctime sys/stat.h {
	#define CTIME	1
}
elif mem stat.st_mtime sys/stat.h {
	#define MTIME	1
}
else pass{ no_stat_time=1 }end {
	#define NOTIME	1
}
endif
if ( !no_stat_time ) {
	#define YESTIME	1
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _sys_stat	1	/* #include <sys/stat.h> ok */
#define _mem_st_mtime_stat	1	/* st_mtime is a member of struct stat */
#define MTIME	1

#define YESTIME	1

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is sys/stat.h a header ... yes
iffe: test: is foo_stat a type or typedef ... no
iffe: test: is st_atime a member of struct foo_stat ... no
iffe: test: is st_ctime a member of struct foo_stat ... no
iffe: test: is stat a type or typedef ... no
iffe: test: is st_mtime a member of struct stat ... yes
iffe: test: is ( !no_stat_time ) true ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff ifelse
if mem foo_stat.st_atime sys/stat.h {
	#define ATIME	1
}
elif mem foo_stat.st_ctime sys/stat.h {
	#define CTIME	1
}
elif mem foo_stat.st_mtime sys/stat.h {
	#define MTIME	1
}
else pass{ no_stat_time=1 }end {
	#define NOTIME	1
}
endif
if ( !no_stat_time ) {
	#define YESTIME	1
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _ifelse_H
#define _ifelse_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _sys_stat	1	/* #include <sys/stat.h> ok */
#define NOTIME	1

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is sys/stat.h a header ... yes
iffe: test: is foo_stat a type or typedef ... no
iffe: test: is st_atime a member of struct foo_stat ... no
iffe: test: is st_ctime a member of struct foo_stat ... no
iffe: test: is st_mtime a member of struct foo_stat ... no
iffe: test: is ( !no_stat_time ) true ... no'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'set	explicit
iff	previous
hdr	stdio
if hdr stdio {
	OK
}
else {
	OK
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _previous_H
#define _previous_H	1
#define _hdr_stdio	1	/* #include <stdio.h> ok */
OK

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes'

	EXEC	-r -v -s bsh - t.iffe

TEST 04 'test variable/macro override'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
HAVE_STDIO = hdr stdio'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _macro_H
#define _macro_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define HAVE_STDIO	1	/* #include <stdio.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
if hdr - stdio {
	#define HIT 1
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _macro_H
#define _macro_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define HIT 1

#endif'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
if - hdr stdio {
	#define HIT 1
}
endif'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
if ? hdr stdio {
	#define HIT 1
}
endif'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
if hdr - stdio {
	#define HIT 1
}
endif'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
if HAVE_STDIO = hdr stdio {
	#define HIT 1
}
endif
if ( HAVE_STDIO ) {
	#define TOO ALSO
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _macro_H
#define _macro_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define HAVE_STDIO	1	/* #include <stdio.h> ok */
#define HIT 1

#define TOO ALSO

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is ( HAVE_STDIO ) true ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
if HAVE_STDIO = hdr stdio {
	#define HIT 1
}
endif
exp ALSO HAVE_STDIO'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _macro_H
#define _macro_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define HAVE_STDIO	1	/* #include <stdio.h> ok */
#define HIT 1

#define ALSO	1	/* HAVE_STDIO is true */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is HAVE_STDIO true ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
if HAVE_STDIO = hdr stdio {
	#define HIT 1
}
endif
ALSO = ( HAVE_STDIO )'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _macro_H
#define _macro_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define HAVE_STDIO	1	/* #include <stdio.h> ok */
#define HIT 1

#define ALSO	1	/* ( HAVE_STDIO ) is true */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: is ( HAVE_STDIO ) true ... yes'

	EXEC	-r -v -s bsh - t.iffe

TEST 05 'test code option sequence'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
tst - -DN=1 stdio.h - -DN=2 note{ long int type }end output{
	#include <stdio.h>
	#if N == 1
	#define t		long
	#define s		"long"
	#endif
	#if N == 2
	#define t		error
	#define s		"error"
	#endif
	t n = 0;
	int
	main()
	{
		printf("#define t %s\\n", s);
		return 0;
	}
}end'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _macro_H
#define _macro_H	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define t long
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is stdio.h a header ... yes
iffe: test: long int type ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'iff macro
tst - -DN=2 stdio.h - -DN=1 note{ long int type }end output{
	#include <stdio.h>
	#if N == 1
	#define t		long
	#define s		"long"
	#endif
	#if N == 2
	#define t		error
	#define s		"error"
	#endif
	t n = 0;
	int
	main()
	{
		printf("#define t %s\\n", s);
		return 0;
	}
}end'

	EXEC	-r -v -s bsh - t.iffe

TEST 06 'block side effects'

	EXEC	-r - t.iffe
		INPUT t.iffe $'iff -
tst	output{
	int
	main()
	{
		printf("HIT\\n");
		return 0;
	}
}end'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#define _sys_types	1	/* #include <sys/types.h> ok */
HIT'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'iff
tst	- output{
	int
	main()
	{
		printf("HIT\\n");
		return 0;
	}
}end'

	EXEC	-r -s bsh - t.iffe

TEST 07 'diagnostics'

	EXEC	-r - t.iffe
		INPUT t.iffe $'tst foo'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#endif'
		ERROR - $'iffe: t.iffe:1: tst: unknown feature test'
		EXIT 1

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'if (1)'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */'
		ERROR - $'iffe: t.iffe:1: missing endif'
		EXIT 2

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'if'
		OUTPUT -
		ERROR - $'iffe: t.iffe:1: missing endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'endif'
		ERROR - $'iffe: t.iffe:1: endif: no matching if'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'if {
}end'
		ERROR - $'iffe: t.iffe:2: missing }'

	EXEC	-r -s bsh - t.iffe

TEST 08 'negation consternation'

	EXEC	-r - t.iffe
		INPUT t.iffe $'npt fopen,fooon stdio.h'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdlib	1	/* #include <stdlib.h> ok */
#define _hdr_unistd	1	/* #include <unistd.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _npt_fooon	1	/* fooon() needs a prototype */
#endif'

	EXEC	-r -u - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdlib	1	/* #include <stdlib.h> ok */
#define _hdr_unistd	1	/* #include <unistd.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#undef	_npt_fopen		/* fopen() does not need a prototype */
#define _npt_fooon	1	/* fooon() needs a prototype */
#endif'

	EXEC	-r -a - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdlib	1	/* #include <stdlib.h> ok */
#define _hdr_unistd	1	/* #include <unistd.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define _npt_fopen	0	/* fopen() does not need a prototype */
#define _npt_fooon	1	/* fooon() needs a prototype */
#endif'

	EXEC	-r -C - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define HAVE_SYS_TYPES_H	1	/* #include <sys/types.h> ok */
#define HAVE_STDLIB_H	1	/* #include <stdlib.h> ok */
#define HAVE_UNISTD_H	1	/* #include <unistd.h> ok */
#define HAVE_STDIO_H	1	/* #include <stdio.h> ok */
#define HAVE_FOPEN_DECL	1	/* fopen() does not need a prototype */
#undef	HAVE_FOOON_DECL		/* fooon() needs a prototype */
#endif'

	EXEC	-r - t.iffe
		INPUT t.iffe $'NEED_FOPEN = npt fopen stdio.h
HAVE_FOPEN = ! npt fopen stdio.h
NEED_FOOON = npt fooon stdio.h
HAVE_FOOON = ! npt fooon stdio.h'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdlib	1	/* #include <stdlib.h> ok */
#define _hdr_unistd	1	/* #include <unistd.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#define HAVE_FOPEN	1	/* fopen() does not need a prototype */
#define NEED_FOOON	1	/* fooon() needs a prototype */
#endif'

	EXEC	-r -u - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_stdlib	1	/* #include <stdlib.h> ok */
#define _hdr_unistd	1	/* #include <unistd.h> ok */
#define _hdr_stdio	1	/* #include <stdio.h> ok */
#undef	NEED_FOPEN		/* fopen() does not need a prototype */
#define HAVE_FOPEN	1	/* fopen() does not need a prototype */
#define NEED_FOOON	1	/* fooon() needs a prototype */
#undef	HAVE_FOOON		/* fooon() needs a prototype */
#endif'

	EXEC	-r -C - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define HAVE_SYS_TYPES_H	1	/* #include <sys/types.h> ok */
#define HAVE_STDLIB_H	1	/* #include <stdlib.h> ok */
#define HAVE_UNISTD_H	1	/* #include <unistd.h> ok */
#define HAVE_STDIO_H	1	/* #include <stdio.h> ok */
#undef	NEED_FOPEN		/* fopen() does not need a prototype */
#define HAVE_FOPEN	1	/* fopen() does not need a prototype */
#define NEED_FOOON	1	/* fooon() needs a prototype */
#undef	HAVE_FOOON		/* fooon() needs a prototype */
#endif'

TEST 09 'exp vs. if'

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_false {
	ONE
}
exp	_tst_hit !_tst_hit&_tst_true pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&_tst_true {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#define _tst_hit	1	/* !_tst_false is true */
/* !_tst_false */
ONE

#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_true {
	ONE
}
exp	_tst_hit !_tst_hit&_tst_true pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&_tst_true {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#define _tst_hit	1	/* !_tst_hit&_tst_true is true */
TWO 0 1
#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_true {
	ONE
}
exp	_tst_hit !_tst_hit&_tst_false pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&_tst_true {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#define _tst_hit	1	/* !_tst_hit&_tst_true is true */
/* !_tst_hit&_tst_true */
THREE

#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_true {
	ONE
}
exp	_tst_hit !_tst_hit&_tst_false pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&_tst_false {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_false {
	ONE
}
exp	_tst_hit !_tst_hit&&_tst_true pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&&_tst_true {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#define _tst_hit	1	/* !_tst_false is true */
/* !_tst_false */
ONE

#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_true {
	ONE
}
exp	_tst_hit !_tst_hit&&_tst_true pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&&_tst_true {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#define _tst_hit	1	/* !_tst_hit&&_tst_true is true */
TWO 0 1
#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_true {
	ONE
}
exp	_tst_hit !_tst_hit&&_tst_false pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&&_tst_true {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#define _tst_hit	1	/* !_tst_hit&&_tst_true is true */
/* !_tst_hit&&_tst_true */
THREE

#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
exp	_tst_hit !_tst_true {
	ONE
}
exp	_tst_hit !_tst_hit&&_tst_false pass{
cat <<!
	TWO $_tst_false $_tst_true
!
}end
exp	_tst_hit !_tst_hit&&_tst_false {
	THREE
}'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
if ( ! _tst_false ) {
	ONE
}
elif ( _tst_true ) pass{
cat <<!
	TWO
!
}end
else {
	THREE
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
ONE

#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
if ( ! _tst_true ) {
	ONE
}
elif ( _tst_true ) pass{
cat <<!
	TWO
!
}end
else {
	THREE
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
TWO
#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
if ( ! _tst_true ) {
	ONE
}
elif ( _tst_false ) pass{
cat <<!
	TWO
!
}end
else {
	THREE
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
THREE

#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
if ( ! _tst_true ) yes{
	typedef struct
	{
		int		dd_fd;		/* file descriptor		*/
	} DIR;
}end
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
if ( ! _tst_true ) {
	typedef struct
	{
		int		dd_fd;		/* file descriptor		*/
	} DIR;
}
else {
	OK
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _tst_true	1	/* ( 1 ) is true */
OK

#endif'

	EXEC	-r -s bsh - t.iffe

	EXEC	-r - t.iffe
		INPUT t.iffe $'_tst_false = ( 0 )
_tst_true = ( 1 )
if ( ! _tst_true ) {
	typedef struct
	{
		int		dd_fd;		/* file descriptor		*/
	};
}
else {
	OK
}
endif'

	EXEC	-r -s bsh - t.iffe

TEST 10 'exp details'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'_str = "string"
_hdr = <header>
_aaa = ( 0 )
_zzz = ( 1 )
( _str )
( ! _str )
( _hdr )
( ! _hdr )
( _aaa )
( ! _aaa )
( _zzz )
( ! _zzz )'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _str	"string"
#define _hdr	<header>
#define _zzz	1	/* ( 1 ) is true */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is ( 0 ) true ... no
iffe: test: is ( 1 ) true ... yes
iffe: test: is ( _str ) true ... yes
iffe: test: is ( ! _str ) true ... no
iffe: test: is ( _hdr ) true ... yes
iffe: test: is ( ! _hdr ) true ... no
iffe: test: is ( _aaa ) true ... no
iffe: test: is ( ! _aaa ) true ... yes
iffe: test: is ( _zzz ) true ... yes
iffe: test: is ( ! _zzz ) true ... no'

	EXEC	-r -v -s bsh - t.iffe

TEST 11 'set [no]define'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'set nodefine
mem stat.st_mtime sys/stat.h
set define
mem stat.st_mode sys/stat.h
if ( _mem_st_mtime_stat ) {
	1
}
endif
if ( _mem_st_mode_stat ) {
	2
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _mem_st_mode_stat	1	/* st_mode is a member of struct stat */
1

2

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is sys/stat.h a header ... yes
iffe: test: is stat a type or typedef ... no
iffe: test: is st_mtime a member of struct stat ... yes
iffe: test: is st_mode a member of struct stat ... yes
iffe: test: is ( _mem_st_mtime_stat ) true ... yes
iffe: test: is ( _mem_st_mode_stat ) true ... yes'

	EXEC	-r -v -s bsh - t.iffe

TEST 12 'non-opaque mem'

	EXEC	-r -v - mem OPAQUE -I. t.h
		INPUT t.h $'typedef struct opaque OPAQUE;'
		OUTPUT - $'/* : : generated by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_t	1	/* #include <t.h> ok */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is t.h a header ... yes
iffe: test: is OPAQUE a type or typedef ... no
iffe: test: is struct OPAQUE a non-opaque struct ... no'

	EXEC	-r -v - mem NONOPAQUE -I. t.h
		INPUT t.h $'struct nonopaque { int pad; };
typedef struct nonopaque NONOPAQUE;'
		OUTPUT - $'/* : : generated by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _hdr_t	1	/* #include <t.h> ok */
#define _mem_NONOPAQUE	1	/* NONOPAQUE is a non-opaque struct */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is t.h a header ... yes
iffe: test: is NONOPAQUE a type or typedef ... yes
iffe: test: is NONOPAQUE a non-opaque struct ... yes'

TEST 13 'key states'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'key	int
key	const =
key	foo
key	bar =
key	aha = huh = int
key	chr = char = int'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _key_int	1	/* int is a reserved keyword */
#define _key_const	1	/* const is a reserved keyword */
#define bar	 	/* default for reserved keyword bar */
#define aha	int	/* default for reserved keyword aha */
#define _key_char	1	/* char is a reserved keyword */
#define chr	char	/* alternate for reserved keyword chr */
#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is int a reserved keyword ... yes
iffe: test: is const a reserved keyword ... yes
iffe: test: is foo a reserved keyword ... no
iffe: test: is bar a reserved keyword ... no
iffe: test: is aha a reserved keyword ... no
iffe: test: is huh a reserved keyword ... no
iffe: test: is chr a reserved keyword ... no
iffe: test: is char a reserved keyword ... yes'

	EXEC	-r -v -s bsh - t.iffe

	EXEC	-u -r -v - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _key_int	1	/* int is a reserved keyword */
#define _key_const	1	/* const is a reserved keyword */
#undef	_key_foo		/* foo is not a reserved keyword */
#undef	_key_bar		/* bar is not a reserved keyword */
#define bar	 	/* default for reserved keyword bar */
#undef	_key_huh		/* huh is not a reserved keyword */
#define aha	int	/* default for reserved keyword aha */
#define _key_char	1	/* char is a reserved keyword */
#define chr	char	/* alternate for reserved keyword chr */
#endif'

	EXEC	-u -r -v -s bsh - t.iffe

	EXEC	-a -r -v - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define _key_int	1	/* int is a reserved keyword */
#define _key_const	1	/* const is a reserved keyword */
#define _key_foo	0	/* foo is not a reserved keyword */
#define _key_bar	0	/* bar is not a reserved keyword */
#define bar	 	/* default for reserved keyword bar */
#define _key_huh	0	/* huh is not a reserved keyword */
#define aha	int	/* default for reserved keyword aha */
#define _key_char	1	/* char is a reserved keyword */
#define chr	char	/* alternate for reserved keyword chr */
#endif'

	EXEC	-a -r -v -s bsh - t.iffe

	EXEC	-C -r -v - t.iffe
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define HAVE_SYS_TYPES_H	1	/* #include <sys/types.h> ok */
#define HAVE_INT_RESERVED	1	/* int is a reserved keyword */
#define HAVE_CONST_RESERVED	1	/* const is a reserved keyword */
#undef	HAVE_FOO_RESERVED		/* foo is not a reserved keyword */
#undef	HAVE_BAR_RESERVED		/* bar is not a reserved keyword */
#define bar	 	/* default for reserved keyword bar */
#undef	HAVE_HUH_RESERVED		/* huh is not a reserved keyword */
#define aha	int	/* default for reserved keyword aha */
#define HAVE_CHAR_RESERVED	1	/* char is a reserved keyword */
#define chr	char	/* alternate for reserved keyword chr */
#endif'

	EXEC	-C -r -v -s bsh - t.iffe

TEST 14 'include file'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'include t_lib.h
if ( bar_foo ) {
	#define all 1
}
elif ( _foo_bar ) {
	#define some 1
}
endif'
		INPUT t_lib.h '#define bar_foo ALL
#define _foo_bar SOME'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define some 1

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is ( bar_foo ) true ... no
iffe: test: is ( _foo_bar ) true ... yes'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'include t_lib.h .
if ( bar_foo ) {
	#define all 1
}
elif ( _foo_bar ) {
	#define ok 1
}
endif'
		OUTPUT - $'/* : : generated from t.iffe by iffe version 1995-03-19 : : */
#ifndef _REGRESS
#define _REGRESS	1
#define _sys_types	1	/* #include <sys/types.h> ok */
#define all 1

#endif'
		ERROR - $'iffe: test: is sys/types.h a header ... yes
iffe: test: is ( bar_foo ) true ... yes'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'include t_lib.h . ?
if ( bar_foo ) {
	#define all 1
}
elif ( _foo_bar ) {
	#define ok 1
}
endif'
		ERROR - $'iffe: t.iffe:1: warning: ?: include operands ignored
iffe: test: is sys/types.h a header ... yes
iffe: test: is ( bar_foo ) true ... yes'

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'include foo_lib.h'
		OUTPUT -
		ERROR - $'iffe: t.iffe:1: foo_lib.h: include file not found'
		EXIT 2

	EXEC	-r -v - t.iffe
		INPUT t.iffe $'include'
		ERROR - $'iffe: t.iffe:1: include path expected'
