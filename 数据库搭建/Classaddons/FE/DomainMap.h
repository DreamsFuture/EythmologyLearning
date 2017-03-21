#ifndef	_DOMAIN_MAP_H_
#define	_DOMAIN_MAP_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: DomainMap.h,v 1.1 1993/06/13 18:37:32 tiller Exp tiller $
 */

//
// The class is intended to provide a map from sub-domains to their
// parent (sub)domain.  In other words, it maps equations of sub-systems
// to the approriate equations in the larger system.
//

#include	<Set/SimpleSet.h>

struct	DomainRef
{
	DomainRef() { dom = -1; eq = -1; }
	DomainRef(int d, long e) { dom = d; eq = e; }
	int		dom;
	long	eq;
};

class	DomainMap
{
	public:
		virtual	long	&Eq(int dom, long eq) = 0;
		virtual	void Inverse(long eq, SimpleSet<DomainRef>&) = 0;
};

typedef	SimpleSet<long>	DomainEqMap;

class	StdDomainMap : public DomainMap
{
	private:
		StdDomainMap();
		StdDomainMap(const StdDomainMap&);
		StdDomainMap& operator=(const StdDomainMap&);
	protected:
		SimpleSet<DomainEqMap *>	maps;
	public:
		StdDomainMap(int);
		long &Eq(int dom, long eq);
		void Inverse(long eq, SimpleSet<DomainRef>&);
};

#endif
