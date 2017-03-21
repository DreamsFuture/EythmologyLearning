#ifndef	_COUPLER_H_
#define	_COUPLER_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Coupler.h,v 1.1 1993/06/13 18:32:27 tiller Exp tiller $
 */

#include	"GenSystem.h"
#include	<Set/SimpleSet.h>
#include	"DomainMap.h"

class	Coupler : public GenSystem
{
	public:
		virtual const FixedSet<GenSystem *>& Systems() = 0;
};

class	StdCoupler : public Coupler
{
	protected:
		SimpleSet<GenSystem *> systems;
	public:
		const FixedSet<GenSystem *>& Systems();
};

class	UniCoupler : public StdCoupler
{
	private:
		UniCoupler(const UniCoupler&);
		UniCoupler& operator=(const UniCoupler&);
	public:
		UniCoupler();
		void	SetSystem(GenSystem *);

		// This is to satisfy the GenSystem interface
		void Tangent(BIMatrix &);
		void TanPreMult(const Vector &, Vector &);
		void TanPostMult(const Vector &, Vector &);
		int Num();
		double Get(int);
		void Set(int, double);
		void Residual(Vector&);
		void LocalReduce(BIMatrix&, Vector&, ReducType);
		void DistReduce(Vector &, ReducType);
};

class	MultiCoupler : public StdCoupler
{
	private:
		MultiCoupler(const MultiCoupler&);
		MultiCoupler& operator=(const MultiCoupler&);
	protected:
		int	nsys;
		int	num;
		StdDomainMap	map;
	public:
		MultiCoupler(int nd, int neq);

		// Unique to MultiCouplers
		int	NumSystems();
		void SetSystem(int n, GenSystem *sys);
		DomainMap &Map();

		// This is to satisfy the GenSystem interface
		void Tangent(BIMatrix &);
		void TanPreMult(const Vector &, Vector &);
		void TanPostMult(const Vector &, Vector &);
		int Num();
		double Get(int);
		void Set(int, double);
		void Residual(Vector&);
		void LocalReduce(BIMatrix&, Vector&, ReducType);
		void DistReduce(Vector &, ReducType);
};

#endif
