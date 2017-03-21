#include	"Cache.h"
#include	"BasicTypes.h"
#include	"Element.h"
#include	<iostream.h>
#include	<BIMatrix.h>
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Cache.c,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

MatCache::MatCache(BIMatrix& b, const Const &d, Element &e)
	: mat(b), dof(d), elem(e)
{
#ifdef	MDEBUG
	malloc_enter("MatCache::MatCache");
#endif
	Indexer<Node *>	nodes(elem.nodes());
	const	int	ns = nodes.Size();
	int	i;

	cache = new double*[ns];
	map = new int[ns];

	cache[0] = new double[ns*ns];
	for(i=1;i<ns;i++)
		cache[i] = cache[0]+i*ns;
	for(i=0;i<ns*ns;i++) cache[0][i] = 0.0;

	for(i=0;i<ns;i++)
		map[i] = elem.EqNum(i+1,dof);
#ifdef	MDEBUG
	malloc_leave("MatCache::MatCache");
#endif
}

MatCache::~MatCache()
{
	int	i, j, eqi;
	const	int	ns = elem.nodes().Size();

	// Flush cache
	for(i=0;i<ns;i++)
	{
		eqi = map[i];
		for(j=0;j<ns;j++)
			mat[eqi][map[j]]+=cache[i][j];
	}
	delete [] map;
	delete [] cache[0];
	delete [] cache;
}

VecCache::VecCache(SVector& sv, const Const &d, Element &e)
	: vec(sv), dof(d), elem(e)
{
#ifdef	MDEBUG
	malloc_enter("VecCache::VecCache");
#endif
	Indexer<Node *>	nodes(elem.nodes());
	const	int	ns = nodes.Size();
	int	i;

	cache = new double[ns];
	map = new int[ns];

	for(i=0;i<ns;i++) cache[i] = 0.0;

	for(i=0;i<ns;i++)
		map[i] = elem.EqNum(i+1,dof);
#ifdef	MDEBUG
	malloc_leave("VecCache::VecCache");
#endif
}

VecCache::~VecCache()
{
	int	i;
	const	int	ns = elem.nodes().Size();

	// Flush cache
	for(i=0;i<ns;i++)
		vec[map[i]]+=cache[i];

	delete [] map;
	delete [] cache;
}
