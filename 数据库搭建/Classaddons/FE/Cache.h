#ifndef	_CACHE_H_
#define	_CACHE_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Cache.h,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

//
// Class : Cache
//
//	Since BIMatrix is a list of entries, it isn't optimal to keep adding
// little bits a pieces of data into, you want to 'cache' that data and
// then put it in.  Another issue is the access speed for members of
// a BIMatrix.  The lookup speed is pretty slow.  This class is designed
// to cache all additions to a BIMatrix and deal with them in the fastest
// possible way, then write the total contribution.
//

class	BIMatrix;
class	Vector;
class	SVector;
class	Element;
class	Const;

class	MatCache
{
	protected:
		BIMatrix	&mat;
		const Const &dof;
		Element	&elem;
		double	**cache;
		int		*map;
	public:
		MatCache(BIMatrix &, const Const &, Element &);
		inline double& operator()(int i, int j) { return cache[i-1][j-1]; }
		~MatCache();
};

class	VecCache
{
	protected:
		SVector	&vec;
		const Const &dof;
		Element	&elem;
		double	*cache;
		int		*map;
	public:
		VecCache(SVector &, const Const &, Element &);
		inline double& operator()(int i) { return cache[i-1]; }
		~VecCache();
};

#endif
