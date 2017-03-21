#ifndef	_BASIS_H_
#define	_BASIS_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Basis.h,v 3.4 1993/06/10 20:59:29 tiller Exp tiller $
 */

#include	<space/coord3d.h>
#include	<Set/SimpleSet.h>

class	GPInfo;
class	GaussContext;
class	BasisFunctions
{
	public:

		virtual	int	NumNodes() const = 0;
		virtual	int	NumDim() const = 0;
		virtual	GaussContext *Eval(const Coord3D& c) const = 0;
		virtual void Eval(const FixedSet<Coord3D>&, GPInfo& con) const;
};

class	BF21 : public BasisFunctions
{
	public:
		int	NumNodes() const { return 2; }
		int	NumDim() const { return 1; }
		GaussContext *Eval(const Coord3D& c) const;
};

class	BF42 : public BasisFunctions
{
	public:
		int	NumNodes() const { return 4; }
		int	NumDim() const { return 2; }
		GaussContext *Eval(const Coord3D& c) const;
};


#endif