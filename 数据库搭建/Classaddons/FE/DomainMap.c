/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: DomainMap.c,v 1.1 1993/06/13 18:37:32 tiller Exp tiller $
 */

#include	<assert.h>
#include	"DomainMap.h"

StdDomainMap::StdDomainMap(int nd)
{
	int	i;

	for(i=1;i<=nd;i++)
		maps[i] = new DomainEqMap;
}

long &
StdDomainMap::Eq(int dom, long eq)
{
	int	msize = maps.Size();
	assert(dom>=1 && dom<=msize);

	DomainEqMap &dmap = *(maps[dom]);

	return dmap[eq];
}

void
StdDomainMap::Inverse(long eq, SimpleSet<DomainRef> &ref)
{
	// This is a brute force search...pretty ugly
	int	i, j;

	foreach(i,maps)
	{
		DomainEqMap &map = *(maps[i]);
		foreach(j,map)
			if (map[j]==eq)
				ref+=DomainRef(i,j);
	}
}
