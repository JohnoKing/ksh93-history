####################################################################
#                                                                  #
#             This software is part of the ast package             #
#                Copyright (c) 1985-2000 AT&T Corp.                #
#        and it may only be used by you under license from         #
#                       AT&T Corp. ("AT&T")                        #
#         A copy of the Source Code Agreement is available         #
#                at the AT&T Internet web site URL                 #
#                                                                  #
#       http://www.research.att.com/sw/license/ast-open.html       #
#                                                                  #
#        If you have copied this software without agreeing         #
#        to the terms of the license you are infringing on         #
#           the license and copyright and are violating            #
#               AT&T's intellectual property rights.               #
#                                                                  #
#                 This software was created by the                 #
#                 Network Services Research Center                 #
#                        AT&T Labs Research                        #
#                         Florham Park NJ                          #
#                                                                  #
#               Glenn Fowler <gsf@research.att.com>                #
#                David Korn <dgk@research.att.com>                 #
#                 Phong Vo <kpv@research.att.com>                  #
#                                                                  #
####################################################################
ok=0
for i in \
	-x /lib/ld.so /lib/ld-*.so /usr/lib/ld.so /lib/rld \
	-f /usr/shlib/libc.so /shlib/libc.so /usr/lib/libc.so \
	-r /usr/shlib/libc.so /shlib/libc.so
do	case $i in
	-*)	op=$i; continue ;;
	esac
	if	test $op $i
	then	ok=1
		break
	fi
	set x $i.[0-9]*
	if	test $op $2
	then	ok=1
		break
	fi
done
if	test "0" != "$ok"
then	libpath=lib:LD_LIBRARY_PATH
	if	test -d /lib32
	then	libpath=lib32:LD_LIBRARYN32_PATH,$libpath
	fi
	if	test -d /lib64
	then	libpath=lib64:LD_LIBRARY64_PATH,$libpath
	fi
elif	test -x /lib/dld.sl
then	libpath=lib:SHLIB_PATH
elif	test -x /usr/lib/dyld
then	libpath=lib:DYLD_LIBRARY_PATH
elif	test -x /bin/xlc -o -x /bin/tso -a -d /bin/IBM
then	libpath=LIBPATH
else	libpath=
fi
if	test "" != "$libpath"
then	echo "#define _LIBPATH		\"$libpath\""
fi
