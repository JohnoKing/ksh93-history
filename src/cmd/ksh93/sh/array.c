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
/*
 * Array processing routines
 *
 *   David Korn
 *   AT&T Labs
 *   dgk@research.att.com
 *
 */

#include	"defs.h"
#include	<stak.h>
#include	"name.h"

#define NUMSIZE	(4+(ARRAY_MAX>999)+(ARRAY_MAX>9999)+(ARRAY_MAX>99999))
#define is_associative(ap)	array_assoc((Namarr_t*)(ap))

struct index_array
{
        Namarr_t        header;
        int		cur;    /* index of current element */
        int		maxi;   /* maximum index for array */
        union Value	val[1]; /* array of value holders */
};


/*
 * replace discipline with new one
 */
static void array_setptr(register Namval_t *np, struct index_array *old, struct index_array *new)
{
	register Namfun_t **fp = &np->nvfun;
	while(*fp && *fp!= &old->header.hdr)
		fp = &((*fp)->next);
	if(*fp)
	{
		new->header.hdr.next = (*fp)->next;
		*fp = &new->header.hdr;
	}
	else sfprintf(sfstderr,"discipline not replaced\n");
}

/*
 *   Calculate the amount of space to be allocated to hold an
 *   indexed array into which <maxi> is a legal index.  The number of
 *   elements that will actually fit into the array (> <maxi>
 *   but <= ARRAY_MAX) is returned.
 *
 */
static int	arsize(register int maxi)
{
	register int i = roundof(maxi,ARRAY_INCR);
	return (i>ARRAY_MAX?ARRAY_MAX:i);
}

/* return index of highest element of an array */
int array_maxindex(Namval_t *np)
{
	register struct index_array *ap = (struct index_array*)nv_arrayptr(np);
	register int i = ap->maxi;
	if(is_associative(ap))
		return(-1);
	while(i>0 && ap->val[--i].cp==0);
	return(i+1);
}

/*
 * Get the Value pointer for an array.
 * Delete space as necessary if flag is ARRAY_DELETE
 * After the lookup is done the last @ or * subscript is incremented
 */
static union Value *array_find(Namval_t *np,Namarr_t *arp, int flag)
{
	register struct index_array *ap = (struct index_array*)arp;
	register union Value *up;
	register unsigned dot=0;
	if(is_associative(ap))
		up = (union Value*)((*ap->header.fun)(np,NIL(char*),0));
	else
	{
		if((dot=ap->cur) >= ap->maxi)
			errormsg(SH_DICT,ERROR_exit(1),e_subscript, nv_name(np));
		up = &(ap->val[dot]);
	}
	if(flag==ARRAY_DELETE)
	{
		if(up->cp)
		{
			ap->header.nelem--;
			if(is_associative(ap))
			{
				(*ap->header.fun)(np, NIL(char*), NV_ADELETE);
				if(!(ap->header.nelem&ARRAY_SCAN))
					return(up);
			}
		}
		if(array_elem(&ap->header)==0 && ((ap->header.nelem&ARRAY_SCAN) || !is_associative(ap)))
		{
			const char *cp = up->cp;
			if(cp && is_associative(ap))
				(*ap->header.fun)(np, NIL(char*), NV_AFREE);
			nv_offattr(np,NV_ARRAY);
			if(nv_isattr(np,NV_TABLE))
			{
				np->nvfun = up->funp;
				up->cp = 0;
				up = (union Value*)&np->nvfun;
			}
			else
			{
				np->nvalue.cp = cp;
				up->cp = 0;
				up = &np->nvalue;
			}
		}
	}
	else if(!up->cp)
	{
		if(flag==ARRAY_LOOKUP)
			return(0);
		ap->header.nelem++;
	}
	return(up);
}

static Namfun_t *array_clone(Namval_t *np, Namval_t *mp, int flags, Namfun_t *fp)
{
	Namarr_t *ap = (Namarr_t*)fp;
	char *name, *sub=0;
	int nelem = ap->nelem;
	if(array_assoc(ap))
		nv_setarray(mp,ap->fun);
	else
		nv_putsub(mp,NIL(char*),ap->nelem);
	if(!(nelem&(ARRAY_SCAN|ARRAY_UNDEF)) && (sub=nv_getsub(np)))
		sub = strdup(sub);
	nv_putsub(np,NIL(char*),ARRAY_SCAN);
	do
	{
	        if(array_assoc(ap))
			name = (char*)((*ap->fun)(np,NIL(char*),NV_ANAME));
		else
			name = nv_getsub(np);
		nv_putsub(mp,name,ARRAY_ADD);
		if(nv_isattr(np,NV_INTEGER))
		{
			Sfdouble_t d= nv_getnum(np);
			nv_putval(mp,(char*)&d,NV_LONG|NV_DOUBLE|NV_INTEGER);
		}
		else
			nv_putval(mp,nv_getval(np),NV_RDONLY);
	}
	while(nv_nextsub(np));
	if(sub)
	{
		nv_putsub(np,sub,0L);
		free((void*)sub);
	}
	ap->nelem = nelem;
	((Namarr_t*)mp->nvfun)->nelem = nelem;
	return(nv_stack(mp,(Namfun_t*)0));
}

static Namarr_t *array_check(Namval_t*, Namarr_t*, int);

static char *array_getval(Namval_t *np, Namfun_t *disc)
{
	register Namarr_t *ap = (Namarr_t*)disc;
	register union Value *up;
	ap = array_check(np,ap,ARRAY_LOOKUP);
	if(!(up = array_find(np,ap,ARRAY_LOOKUP)))
		 return (NIL(char*));
	if(nv_isattr(np,NV_TABLE))
		np->nvfun = up->funp;
	else
		np->nvalue.cp = up->cp;
	return(nv_getv(np,&ap->hdr));
}

static Sfdouble_t array_getnum(Namval_t *np, Namfun_t *disc)
{
	register Namarr_t *ap = (Namarr_t*)disc;
	register union Value *up;
	ap = array_check(np,ap,ARRAY_LOOKUP);
	if(!(up = array_find(np,ap,ARRAY_LOOKUP)))
		 return(0);
	if(nv_isattr(np,NV_TABLE))
		np->nvfun = up->funp;
	else
		np->nvalue.cp = up->cp;
	return(nv_getn(np,&ap->hdr));
}

static void array_putval(Namval_t *np, const char *string, int flags, Namfun_t *dp)
{
	register Namarr_t *ap = (Namarr_t*)dp;
	register union Value *up;
	ap = array_check(np,ap,string?ARRAY_ASSIGN:ARRAY_DELETE);
	do
	{
		if(!(up = array_find(np,ap,string?ARRAY_ASSIGN:ARRAY_DELETE)))
			continue;
		if(nv_isattr(np,NV_TABLE))
			np->nvfun = up->funp;
		else
			np->nvalue.cp = up->cp;
		nv_putv(np,string,flags,&ap->hdr);
		if(nv_isattr(np,NV_TABLE))
			up->funp = np->nvfun;
		else
			up->cp = np->nvalue.cp;
	}
	while(!string && nv_nextsub(np));
	if(!string && !nv_isattr(np,NV_ARRAY))
	{
		Namfun_t *nfp;
		nv_stack(np,(Namfun_t*)ap);
		nfp = nv_stack(np,0);
		if(nfp == (Namfun_t*)ap)
			free((void*)ap);
		else errormsg(SH_DICT,ERROR_warn(0),"%s: variable discipline not top level",nv_name(np));
	}
}

static const Namdisc_t array_disc =
{
	sizeof(Namarr_t),
	array_putval,
	array_getval,
	array_getnum,
	0,
	0,
	array_clone
};

/*
 *        Increase the size of the indexed array of elements in <arp>
 *        so that <maxi> is a legal index.  If <arp> is 0, an array
 *        of the required size is allocated.  A pointer to the 
 *        allocated Namarr_t structure is returned.
 *        <maxi> becomes the current index of the array.
 */
static struct index_array *array_grow(Namval_t *np, register struct index_array *arp,int maxi)
{
	register struct index_array *ap;
	register int i=0;
	register int newsize = arsize(maxi+1);
	if (maxi >= ARRAY_MAX)
		errormsg(SH_DICT,ERROR_exit(1),e_subscript, fmtbase((long)maxi,10,0));
	ap = new_of(struct index_array,(newsize-1)*sizeof(union Value*));
	memset((void*)ap,0,sizeof(*ap));
	ap->maxi = newsize;
	ap->cur = maxi;
	if(arp)
	{
		ap->header = arp->header;
		for(;i < arp->maxi;i++)
			ap->val[i].cp = arp->val[i].cp;
		array_setptr(np,arp,ap);
		free((void*)arp);
	}
	else
	{
		ap->header.fun = 0;
		if(nv_isattr(np,NV_TABLE))
		{
			ap->val[0].funp = np->nvfun;
			i++;
		}
		else if((ap->val[0].cp=np->nvalue.cp))
			i++;
		ap->header.nelem = i;
		ap->header.hdr.nofree = 1;
		ap->header.hdr.disc = &array_disc;
		nv_disc(np,(Namfun_t*)ap, NV_LAST);
	}
	for(;i < newsize;i++)
		ap->val[i].cp = 0;
	return(ap);
}

/*
 * Change ARRAY_UNDEF as appropriate
 * Allocate the space if necessary, if flag is ARRAY_ASSIGN
 * Check for bounds violation for indexed array
 */
static Namarr_t *array_check(Namval_t *np, Namarr_t *arp, int flag)
{
	register struct index_array *ap = (struct index_array*)arp;
	if(ap->header.nelem&ARRAY_UNDEF)
	{
		ap->header.nelem &= ~ARRAY_UNDEF;
		/* delete array is the same as delete array[@] */
		if(flag&ARRAY_DELETE)
		{
			nv_putsub(np, NIL(char*), ARRAY_SCAN);
			ap->header.nelem |= ARRAY_SCAN;
		}
		else /* same as array[0] */
		{
			if(is_associative(ap))
				(*ap->header.fun)(np,"0",flag==ARRAY_ASSIGN?NV_AADD:0);
			else
				ap->cur = 0;
		}
	}
	if(!is_associative(ap))
	{
		if(!(ap->header.nelem&ARRAY_SCAN) && ap->cur >= ap->maxi)
			ap = array_grow(np, ap, (int)ap->cur);
		if(ap->cur>=ap->maxi)
			errormsg(SH_DICT,ERROR_exit(1),e_subscript, nv_name(np));
	}
	return((Namarr_t*)ap);
}

Namarr_t *nv_arrayptr(register Namval_t *np)
{
	if(nv_isattr(np,NV_ARRAY))
		return((Namarr_t*)nv_hasdisc(np, &array_disc));
	return(0);
}

/*
 * Verify that argument is an indexed array and convert to associative,
 * freeing relevant storage
 */
static Namarr_t *nv_changearray(Namval_t *np, void *(*fun)(Namval_t*,const char*,int))
{
	register Namarr_t *ap;
	char numbuff[NUMSIZE+1];
	unsigned dot, digit, n;
	union Value *up;
	struct index_array *save_ap;
	register char *string_index=&numbuff[NUMSIZE];
	numbuff[NUMSIZE]='\0';

	if(!fun || !(ap = nv_arrayptr(np)) || is_associative(ap))
		return(NIL(Namarr_t*));

	nv_stack(np,&ap->hdr);
	save_ap = (struct index_array*)nv_stack(np,0);
	ap = (Namarr_t*)((*fun)(np, NIL(char*), NV_AINIT));
	ap->nelem = 0;
	ap->fun = fun;
	nv_onattr(np,NV_ARRAY);

	for(dot = 0; dot < (unsigned)save_ap->maxi; dot++)
	{
		if(save_ap->val[dot].cp)
		{
			if ((digit = dot)== 0)
				*--string_index = '0';
			else while( n = digit )
			{
				digit /= 10;
				*--string_index = '0' + (n-10*digit);
			}
			nv_putsub(np, string_index, ARRAY_ADD);
			up=array_find(np,ap,0);
			up->cp = save_ap->val[dot].cp;
			save_ap->val[dot].cp = 0;
		}
		string_index = &numbuff[NUMSIZE];
	}
	free((void*)save_ap);
	return(ap);
}

/*
 * set the associative array processing method for node <np> to <fun>
 * The array pointer is returned if sucessful.
 */
Namarr_t *nv_setarray(Namval_t *np, void *(*fun)(Namval_t*,const char*,int))
{
	register Namarr_t *ap;
	char *value;
	if(fun && (ap = nv_arrayptr(np)) && !is_associative(ap))
	{
		/*
		 * if it's already an indexed array, convert to 
		 * associative structure
		 */
		ap = nv_changearray(np, fun);
		return(ap);
	}
	value = nv_getval(np);
	if(fun && !nv_arrayptr(np) && (ap = (Namarr_t*)((*fun)(np, NIL(char*), NV_AINIT))))
	{
		/* check for preexisting initialization and save */
		ap->nelem = 0;
		ap->fun = fun;
		nv_onattr(np,NV_ARRAY);
		if(value)
		{
			nv_putsub(np, "0", ARRAY_ADD);
			nv_putval(np, value, 0);
		}
		return(ap);
	}
	return(NIL(Namarr_t*));
}

/*
 * This routine sets subscript of <np> to the next element, if any.
 * The return value is zero, if there are no more elements
 * Otherwise, 1 is returned.
 */
int nv_nextsub(Namval_t *np)
{
	register struct index_array *ap = (struct index_array*)nv_arrayptr(np);
	register unsigned dot;
	if(!ap || !(ap->header.nelem&ARRAY_SCAN))
		return(0);
	if(is_associative(ap))
	{
		if((*ap->header.fun)(np,NIL(char*),NV_ANEXT))
			return(1);
		ap->header.nelem &= ~ ARRAY_SCAN;
		return(0);
	}
	for(dot=ap->cur+1; dot <  (unsigned)ap->maxi; dot++)
	{
		if(ap->val[dot].cp)
		{
			ap->cur = dot;
			return(1);
		}
	}
	ap->header.nelem &= ~ ARRAY_SCAN;
	ap->cur = 0;
	return(0);
}

/*
 * Set an array subscript for node <np> given the subscript <sp>
 * An array is created if necessary.
 * <mode> can be a number, plus or more of symbolic constants
 *    ARRAY_SCAN, ARRAY_UNDEF, ARRAY_ADD
 * The node pointer is returned which can be NULL if <np> is
 *    not already array and the ARRAY_ADD bit of <mode> is not set.
 */
Namval_t *nv_putsub(Namval_t *np,register char *sp,register long mode)
{
	register struct index_array *ap = (struct index_array*)nv_arrayptr(np);
	register int size = (mode&ARRAY_MASK);
	if(!ap || !ap->header.fun)
	{
		if(sp)
			size = (int)sh_arith((char*)sp);
		if(size >= ARRAY_MAX || (size < 0))
		{
			errormsg(SH_DICT,ERROR_exit(1),e_subscript, nv_name(np));
			return(NIL(Namval_t*));
		}
		if(!ap || size>=ap->maxi)
		{
			if(size==0)
				return(NIL(Namval_t*));
			if(sh.subshell)
				np = sh_assignok(np,1);
			ap = array_grow(np, ap,size);
			nv_onattr(np,NV_ARRAY);
		}
		ap->header.nelem &= ~(ARRAY_SCAN|ARRAY_UNDEF);
		ap->header.nelem |= (mode&(ARRAY_SCAN|ARRAY_UNDEF));
		ap->cur = size;
		if((mode&ARRAY_SCAN) && !ap->val[size].cp && nv_nextsub(np))
			np = 0;
		if(mode&ARRAY_FILL)
		{
			int n;
			for(n=0; n < size; n++)
				ap->val[n].cp = "";
			ap->header.nelem = n|(ap->header.nelem&(ARRAY_SCAN|ARRAY_UNDEF));
			if(n=ap->maxi-ap->maxi)
				memset(&ap->val[size],0,n*sizeof(union Value));
		}
		return((Namval_t*)np);
	}
	ap->header.nelem &= ~(ARRAY_SCAN|ARRAY_UNDEF);
	ap->header.nelem |= (mode&(ARRAY_SCAN|ARRAY_UNDEF));
	if(sp)
		ap = (struct index_array*)(*ap->header.fun)(np, sp, (mode&ARRAY_ADD)?NV_AADD:0);
	else if(mode&ARRAY_SCAN)
		(*ap->header.fun)(np,(char*)np,0);
	else if(mode&ARRAY_UNDEF)
		(*ap->header.fun)(np, "",0);
	if((mode&ARRAY_SCAN) && !nv_nextsub(np))
		np = 0;
	return(np);
}

/*
 * process an array subscript for node <np> given the subscript <cp>
 * returns pointer to character after the subscript
 */
char *nv_endsubscript(Namval_t *np, register char *cp, int mode)
{
	register int count=1, quoted=0, c;
	register char *sp = cp+1;
	/* first find matching ']' */
	while(count>0 && (c= *++cp))
	{
		if(c=='\\' && (!(mode&NV_SUBQUOTE) || (c=cp[1])=='[' || c==']' || c=='\\'))
		{
			quoted=1;
			cp++;
		}
		else if(c=='[')
			count++;
		else if(c==']')
			count--;
	}
	*cp = 0;
	if(quoted)
	{
		/* strip escape characters */
		count = staktell();
		stakwrite(sp,1+cp-sp);
		sh_trim(sp=stakptr(count));
	}
	if(mode && np)
		nv_putsub(np, sp, ARRAY_ADD);
	if(quoted)
		stakseek(count);
	*cp = c;
	if(*++cp == '[' && np)
		errormsg(SH_DICT,ERROR_exit(1),e_subscript, nv_name(np));
	return(cp);
}


Namval_t *nv_opensub(Namval_t* np)
{
	register struct index_array *ap = (struct index_array*)nv_arrayptr(np);
	if(ap && is_associative(ap))
		return((Namval_t*)((*ap->header.fun)(np,NIL(char*),NV_ACURRENT)));
	return(NIL(Namval_t*));
}

char	*nv_getsub(Namval_t* np)
{
	static char numbuff[NUMSIZE];
	register struct index_array *ap;
	register unsigned dot, n;
	register char *cp = &numbuff[NUMSIZE];
	ap = (struct index_array*)nv_arrayptr(np);
	if(!np || !ap)
		return(NIL(char*));
	if(is_associative(ap))
		return((char*)((*ap->header.fun)(np,NIL(char*),NV_ANAME)));
	if((dot = ap->cur)==0)
		*--cp = '0';
	else while(n=dot)
	{
		dot /= 10;
		*--cp = '0' + (n-10*dot);
	}
	return(cp);
}

/*
 * If <np> is an indexed array node, the current subscript index
 * returned, otherwise returns -1
 */
int nv_aindex(register Namval_t* np)
{
	Namarr_t *ap = nv_arrayptr(np);
	if(!ap || is_associative(ap))
		return(-1);
	return(((struct index_array*)(ap))->cur&ARRAY_MASK);
}

struct assoc_array
{
	Namarr_t	header;
	Dt_t		*table;
	Namval_t	*pos;
	Namval_t	*nextpos;
	Namval_t	*cur;
};


/*
 *  This is the default implementation for associate arrays
 */
void *nv_associative(register Namval_t *np,const char *sp,int mode)
{
	register struct assoc_array *ap = (struct assoc_array*)nv_arrayptr(np);
	register int type;
	switch(mode)
	{
	    case NV_AINIT:
		if(ap = (struct assoc_array*)calloc(1,sizeof(struct assoc_array)))
		{
			ap->table = dtopen(&_Nvdisc,Dtbag);
			ap->cur = 0;
			ap->pos = 0;
			ap->header.hdr.disc = &array_disc;
			ap->header.hdr.nofree = 1;
			nv_disc(np,(Namfun_t*)ap, NV_LAST);
		}
		return((void*)ap);
	    case NV_ADELETE:
		if(ap->cur)
		{
			if(nv_isattr(ap->cur,NV_NOFREE))
				nv_offattr(ap->cur,NV_NOFREE);
			else
			{
				dtdelete(ap->table,(void*)ap->cur);
				free((void*)ap->cur);
				ap->cur = 0;
			}
		}
		return((void*)ap);
	    case NV_AFREE:
		ap->pos = 0;
		dtclose(ap->table);
		return((void*)ap);
	    case NV_ANEXT:
		if(!ap->pos)
		{
			if(!(ap->pos=ap->cur))
				ap->pos = (Namval_t*)dtfirst(ap->table);
		}
		else
			ap->pos = ap->nextpos;
		for(;ap->cur=ap->pos; ap->pos=ap->nextpos)
		{
			ap->nextpos = (Namval_t*)dtnext(ap->table,ap->pos);
			if(ap->cur->nvalue.cp)
				return((void*)ap);
		}
		return(NIL(void*));
	    case NV_ACURRENT:
			return((void*)ap->cur);
	    case NV_ANAME:
		if(ap->cur)
			return((void*)nv_name(ap->cur));
		return(NIL(void*));
	    default:
		if(sp)
		{
			if(sp==(char*)np)
			{
				ap->cur = 0;
				return(0);
			}
			ap->pos = 0;
			type = nv_isattr(np,NV_PUBLIC&~NV_ARRAY);
			if((np=nv_search(sp,ap->table,mode?NV_ADD:0)) && nv_isnull(np))
				nv_onattr(np,type);
			ap->cur = np;
		}
		if(ap->cur)
			return((void*)(&ap->cur->nvalue));
		else
			return((void*)(&ap->cur));
	}
}

/*
 * Assign values to an array
 */
void nv_setvec(register Namval_t *np,int append,register int argc,register char *argv[])
{
	int arg0=0;
	struct index_array *ap=0;
	if(nv_isarray(np))
	{
		ap = (struct index_array*)nv_arrayptr(np);
		if(is_associative(ap))
			errormsg(SH_DICT,ERROR_exit(1),"cannot append index array to associate array %s",nv_name(np));
	}
#if SHOPT_APPEND
	if(append)
	{
		if(ap)
		{
			arg0 = ap->maxi;
			while(--arg0>0 && ap->val[arg0].cp==0);
			arg0++;
		}
		else if(!nv_isnull(np))
			arg0=1;
	}
#endif /* SHOPT_APPEND */
	while(--argc >= 0)
	{
		if((argc+arg0)>0  || nv_isattr(np,NV_ARRAY))
			nv_putsub(np,NIL(char*),(long)argc+arg0);
		nv_putval(np,argv[argc],0);
	}
}

