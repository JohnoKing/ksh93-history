/*
 * post stuff to WWWDIR for web access
 * index generated from *.mm
 */

WWWDIR = public_html wwwfiles
WWWSAVE =
WWWSTYLE =
WWWTYPES =

.WWW.semaphore : .SEMAPHORE

.EXPORT : WWWSTYLE WWWTYPES

/*
 * item :WWW: [style=frame] [save=pattern] file ...
 *
 *	`item'.mm generates index.html
 *	other files copied to $(WWWDIR)/`item'
 *	sets up www.bin
 */

":WWW:" : .MAKE .OPERATOR
	local A B D I J L X E P R M
	WWWDIR := $(HOME:X=$(WWWDIR):T=F:O=1)
	B := $(*:N=*.mm::O=1:B)
	D := $(WWWDIR)/$(B)
	M := $(WWWDIR)/man/man1
	R := $(>:N!=*=*)
	for I $(>:N=*=*)
		A := WWW$(I:/=.*//:F=%(upper)S)
		$(A) := $(I:/.*=//)
	end
	(html_info) : $$(MM2HTMLINFO) $$(MM2HTMLINIT)
	if WWWSTYLE == "frame"
		%.html %-index.html %-head.html %-body.html : %.mm (html_info)
			$(MM2HTML) $(MM2HTMLFLAGS) -f $(%) -x -o WWWTYPES=$(WWWTYPES:@Q:@Q) $(>)
	else
		%.html : %.mm (html_info)
			$(MM2HTML) $(MM2HTMLFLAGS) -o WWWTYPES=$(WWWTYPES:@Q:@Q) $(>) > $(<)
	end
	%.html : %.1 (html_info)
		$(MM2HTML) $(MM2HTMLFLAGS) $(>) > $(<)
	%-man.html : $(BINDIR)/% (html_info)
		ignore $(>) --html 2> $(<)
	.DO.WWW.MAN : .USE
		if	test '' = '$(*)'
		then	if	[[ "$( $(<:B) '--???html' -- 2>&1 )" == version=[1-9]* ]]
			then	( $(<:B) '--??html' -- 2>$(<) ) || true
			fi
		elif	strings $(*) | egrep -q '\[\+NAME\?|libcmd\.|cmd[0-9][0-9]\.'
		then	$(*) '--??html' -- 2>$(<) || true
		fi
	$(M)/%.html : .DONTCARE $(INSTALLROOT)/bin/%
		$(@.DO.WWW.MAN)
	if "$(<)"
		D := $(<)
	elif ! "$(R)"
		return
	end
	.WWW .WWW.BIN : $(D) $(M) -
	$(D) $(M) :
		$(SILENT) test -d $(<) || mkdir $(<)
	if ( J = "$(R:N=*.mm)" )
		for I $(J:G=%.html)
			if I == "*-index.html"
				O := $(D)/index.html
			else
				O := $(I:D=$(D):B:S)
			end
			.WWW : $(O)
			$(O) :COPY: $(I)
		end
	end
	.WWW.req : .FUNCTION
		return $(*$(%:T=SR):N=-l*:T=F:P=B:N!=-l*|/*)
	.WWW.LOCAL .WWW.REMOTE : .CLEAR
	A = 0
	for I $(R:N!=*.mm)
		if I == "-"
			let A = ! A
		elif I == "-l*"
			L := $(I:/-l//)
			if J = "$(.DLL.NAME. $(L) $($(L).VERSION):T=F)"
				X += $(J)
			end
		elif A || "$(I:A=.COMMAND|.ARCHIVE)" || "$(I:D:D:N=$(INSTALLROOT))" || "$(I:N=*-www)"
			X += $(I)
			if "$(I:A=.COMMAND)"
				X += $$(.WWW.req $(I))
				J := $(I:/-www$//)
				eval
				.WWW : $(J:D=$(M):B:S=.html)
				$(J:D=$(M):B:S=.html) : $(I) $(I:B:S=.1:T=F:?$(I:B:S=.1)??)
					if	strings $$(*:O=1) | egrep -q '\[\+NAME\?|libcmd\.|cmd[0-9][0-9]\.'
					then	$$(IGNORE) $$(*:O=1) '--??html' -- 2>&1
					elif	test '' != '$$(*:N=*.1)'
					then	$$(MM2HTML) $$(*:N=*.1)
					fi > $$(<)
				end
			end
		else
			if I == "*.html"
				$(I) : .TERMINAL
			end
			.WWW : $(D)/$(I)
			$(D)/$(I) :COPY: $(I)
		end
	end
	if "$(X:V)"
		.WWW.EDIT. : .FUNCTION
			local E I J
			for I $(.INSTALL.LIST.:C,^$(INSTALLROOT)/,,:N!=lib/lib/*)
				for J $(%)
					if "$(I:B:S)" == "$(J:B:S)"
						E += -s ',^$(J)$,$(I),'
					end
				end
			end
			return $(E)
		.WWW.LIST. : .FUNCTION
			local E I J
			for I $(.INSTALL.LIST.:C,^$(INSTALLROOT)/,,:N!=lib/lib/*)
				for J $(%)
					if "$(I:B:S)" == "$(J:B:S)"
						E += $(I)
					end
				end
			end
			return $(E)
		.WWW .WWW.BIN : $(D)/$(B)-$(CC.HOSTTYPE).tar.gz
		$(D)/$(B)-$(CC.HOSTTYPE).tar.gz : $(X:V)
			cat > X.$(tmp).X <<!
			This archive contains $(CC.HOSTTYPE) binaries for
				$(.WWW.LIST. $(*))
			Add the bin directory to PATH and the lib directory
			to LD_LIBRARY_PATH or its equivalent for your system.
			Use the --?help and --man options for online help,
			documentation and contact info.
			!
			$(PAX) -wvf $(<) -x tar:gzip -s "/X.$(tmp).X/README/" $(.WWW.EDIT. $(*)) -s ',\(.*\)-www$,bin/\1,' -s ',.*/lib/,lib/,' X.$(tmp).X $(*:N!=-l*)
			$(RM) -f X.$(tmp).X
	end

/*
 * item ... :WWWBIN: index.mm file ... host:arch ...
 *
 *	home page control
 *	`host' of type `arch' for www.bin files
 */

":WWWBIN:" : .MAKE .OPERATOR
	local HOST ITEM ARCH BINS DIRS G
	.WWW.NOMAN. += $(<)
	for HOST $(>)
		TYPE := $(HOST:/.*://)
		HOST := $(HOST:/:.*//)
		WWWTYPES += $(TYPE)
		ARCH := $(PWD:D:C,/$(CC.HOSTTYPE)/,/$(TYPE)/)
		BINS :=
		DIRS :=
		for ITEM $(<)
			if TYPE == "$(CC.HOSTTYPE)"
				G := $("index.mm":G=%.html:D=$(WWWDIR)/$(ITEM):B:S)
				.WWW.LOCAL : $(G)
				eval
				$(G) : .JOINT $(ARCH)/$(ITEM)/$(ITEM).mm (html_info) .WWW.semaphore .FORCE
					cd $$(*:D)
					$$(MAKE) $$(-) $$(=) www
				end
			else
				BINS += $(WWWDIR)/$(ITEM)/$(ITEM)-$(TYPE).tar.gz
				DIRS += $(ARCH)/$(ITEM)
			end
		end
		.WWW.REMOTE : $(BINS)
		ARCH := $(ARCH:C,/src/.*,,)
		eval
		$(BINS) :JOINT: .FORCE .WWW.semaphore
			rsh $(HOST) "
				eval \"\`bin/package debug use\`\"
				PATH=\$PATH:$(PATH)
				umask 022
				for dir in $(DIRS)
				do	cd \$dir
					$(MAKE) $(-) $(=) --errorid=\$dir www.bin
				done
				"
		end
	end
	.WWW.ALL : .WWW.REMOTE - .WWW.LOCAL

/*
 * :WWWPOST: host:dir
 *
 *	post local $(WWWDIR) to `host':`dir'
 */

":WWWPOST:" : .MAKE .OPERATOR
	:ALL: delta.pax
	eval
	.POST : .VIRTUAL base.pax delta.pax
		case "$$(>)" in
		'')	;;
		*)	rcp $$(>) $(>)
			rsh $(>:C/:.*//) '
				umask 022
				PATH=$HOME/bin:$PATH
				cd $(>:C/.*://)
				pax -rvf delta.pax -z base.pax
			'
			;;
		esac
	end
	base.pax :
		cd $(WWWDIR)
		pax -wvf $(<:P=A) .
	.base.list. : .FUNCTION
		local X
		X := $(sh pax -f $(%:N=*.pax):C,$("\n"), ,G:C,^,$$(WWWDIR)/,)
		$(X) : .DONTCARE
		return $(X)
	delta.pax : .WWW.ALL base.pax $$(.base.list. $$(*))
		cd $(WWWDIR)
		pax -wvf $(<:P=A) -z $(*:N=*.pax:P=A) .

/*
 * [ dir ] :WWWPAGE: [ source ... ] file.mm file
 *
 *	*.mm generates *.html
 *	other files copied to $(WWWDIR)[/dir]
 *	files after - (toggle) are just asserted on ::
 */

":WWWPAGE:" : .MAKE .OPERATOR
	local B D I J O X G A
	A = 0
	if D = "$(<:O=1)"
		B := $(D:B)
		if D != "/*"
			D := $(WWWDIR)/$(D)
			$(D) :INSTALLDIR:
			.WWW.LOCAL : $(D)
		end
		for I $(<:B)
			.WWW.LOCAL : $(WWWDIR)/man/man1/$(I).html
			$(WWWDIR)/man/man1/$(I).html : .DONTCARE
		end
		for I $(>)
			if I == "-"
				let A = !A
				continue
			end
			if A || I == "$(WWWSAVE)"
				:: $(I)
				continue
			end
			if "$(I:T=FD)"
				.SOURCE : $(I)
				continue
			end
			if I == "*.html"
				$(I) : .TERMINAL
				O := $(I)
				X := $(I)
			elif ( G = "$(I:G=%.html)" )
				$(G) : .IMPLICIT $(I)
				if $(G:O) > 1
					for J $(G)
						if J == "*-index.html" && ! X
							O := index.html
						else
							O := $(J)
						end
						.WWW.LOCAL : $(D)/$(O)
						$(D)/$(O) :INSTALL: $(J)
					end
					continue
				end
				if X
					X := $(I)
				else
					X := index
				end
				I := $(I:B:S=.html)
				O := $(X:B:S=.html)
			else
				O := $(I)
			end
			$(D)/$(O) :INSTALL: $(I)
			.WWW.LOCAL : $(D)/$(O)
		end
	else
		for I $(>)
			if I == "-"
				let A = !A
				continue
			end
			if A || I == "$(WWWSAVE)"
				:: $(I)
				continue
			end
			if "$(I:T=FD)"
				.SOURCE : $(I)
				continue
			end
			if I == "*.html"
				$(I) : .TERMINAL
				O := $(I)
			elif ( O = "$(I:G=%.html)" )
				$(O) : .IMPLICIT $(I)
			end
			for J $(O)
				if J == "*-index.html"
					X := index.html
				else
					X := $(J)
				end
				X := $(WWWDIR)/$(X)
				.WWW.LOCAL : $(X)
				$(X) :COPY: $(J)
			end
		end
	end

/*
 * rhs done by default
 */

":WWWALL:" : .MAKE .OPERATOR
	.WWW.ALL : $(>)

":WWWMAN:" : .MAKE .OPERATOR
	.INIT : .WWW.MAN
	.WWW.MAN. := $(>)
	.WWW.MAN : .MAKE .FORCE
		local H I
		for I $(.WWW.MAN.)
			.WWW.LOCAL : $(WWWDIR)/man/man1/$(I:B).html
			$(WWWDIR)/man/man1/$(I:B).html : .DO.WWW.MAN $(I)
		end
		for I $(sh builtin:B)
			.WWW.LOCAL : $(WWWDIR)/man/man1/$(I).html
			$(WWWDIR)/man/man1/$(I).html : .DO.WWW.MAN -
		end
		for I $("$(BINDIR)/*([!-.])":P=G:B)
			if I != "*_*"
				H := $(WWWDIR)/man/man1/$(I).html
				if ! "$(*$(H))" && I != "$(.WWW.NOMAN.:/ /|/G)"
					.WWW.LOCAL : $(H)
				end
			elif "$(PATH:/:/ /G:X=$(I:/.*_//):T=F:O=1)"
				H := $(WWWDIR)/man/man1/$(I:/.*_//).html
				.WWW.LOCAL : $(H)
				$(H) : .DO.WWW.MAN $(BINDIR)/$(I)
			end
		end

.WWW.SED. : .FUNCTION
	local E T
	E = s/^\(\.xx.link=.*\)%HOSTTYPE%\(.*\)%HOSTTYPE%\(.*\)/
	for T $(%)
		E := $(E:V)\$$("\n").LI\$$("\n")\1$(T)\2$(T)\3
	end
	return $(E:V)/

/*
 * mm scan support
 */

.SCAN.mm : .SCAN
	O|S|
	I|.sn %|A.DONTCARE|M$$(%)|
	I|.so %|A.DONTCARE|M$$(%)|

.ATTRIBUTE.%.mm : .SCAN.mm
