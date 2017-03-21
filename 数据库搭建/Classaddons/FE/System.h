#ifndef	_SYSTEM_H_
#define	_SYSTEM_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 *	File : $Id: System.h,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#ifdef	__GNUC__
#pragma interface
#endif

class	Vector;
class	SVector;
class	BIMatrix;
class	Domain;
class	StdDomain;

class	System
{
	public:
		// When asked to reduce the system because of boundary conditions,
		// one of these should be specified.
		enum ReducType { zero, res, mat, rhsonly, rhszero };

		// These three functions take the place of a 'StateVector'
		virtual	int		Num() = 0;
		virtual	double	Get(int) = 0;
		virtual	void	Set(int, double) = 0;

		// I'm not sure what this is going to be used for
		virtual	void Update();
		virtual ~System();
};

class	MatSystem : public virtual System
{
	public:
		virtual void Vec(Vector&) = 0;
		virtual	void Mat(BIMatrix&) = 0;
};

class	MatSysSolver
{
	protected:
		MatSystem	*sys;
	public:
		MatSysSolver(MatSystem *s) { sys = s; }
		virtual	void	Solve(double, int) = 0;
};

#endif
