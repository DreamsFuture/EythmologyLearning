#include	<stdlib.h>
#include	<assert.h>
#include	<math.h>
#include	<iostream.h>
#include	"Basis.h"
#include	"Node.h"
#include	"Element.h"
#include	"GPInfo.h"
#include	"GaussContext.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Basis.c,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#ifdef	USE_MCHUNK
#include	"MemChunk.h"
#endif
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

void
BasisFunctions::Eval(const FixedSet<Coord3D>& p, GPInfo &con) const 
{
	int	i;
	Indexer<Coord3D>	pset(p);

	foreach(i,p)
		con.Append(Eval(pset[i]));
}

GaussContext *
BF21::Eval(const Coord3D& c) const 
{
#ifdef	MDEBUG
	malloc_enter("BF21::Eval");
#endif
	GaussContext	*cp;

	cp = new GaussContext(NumNodes(),NumDim(),c);
#ifdef	GC_IS_GC
	cp->ref();
#endif
	cp->N(1) = (c.x+1.0)/2.0;
	cp->N(2) = (1.0-c.x)/2.0;
	cp->B(1,1) = .5;
	cp->B(1,2) = -.5;
#ifdef	MDEBUG
	malloc_leave("BF21::Eval");
#endif
	return cp;
}

GaussContext *
BF42::Eval(const Coord3D& c) const 
{
#ifdef	MDEBUG
	malloc_enter("BF42::Eval");
#endif
	GaussContext	*cp;

	cp = new GaussContext(NumNodes(),NumDim(),c);
#ifdef	GC_IS_GC
	cp->ref();
#endif
	cp->N(1) = .25*(1.0-c.x)*(1.0-c.y);
	cp->N(2) = .25*(1.0+c.x)*(1.0-c.y);
	cp->N(3) = .25*(1.0+c.x)*(1.0+c.y);
	cp->N(4) = .25*(1.0-c.x)*(1.0+c.y);
	cp->B(1,1) = -.25*(1.0-c.y);
	cp->B(1,2) = .25*(1.0-c.y);
	cp->B(1,3) = .25*(1.0+c.y);
	cp->B(1,4) = -.25*(1.0+c.y);
	cp->B(2,1) = -.25*(1.0-c.x);
	cp->B(2,2) = -.25*(1.0+c.x);
	cp->B(2,3) = .25*(1.0+c.x);
	cp->B(2,4) = .25*(1.0-c.x);
#ifdef	MDEBUG
	malloc_leave("BF42::Eval");
#endif
	return cp;
}

