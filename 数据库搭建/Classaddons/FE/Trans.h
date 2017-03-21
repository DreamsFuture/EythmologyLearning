#ifndef	_TRANS_H_
#define	_TRANS_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Trans.h,v 3.4 1993/06/10 20:59:29 tiller Exp tiller $
 */

#include	<space/coord3d.h>
#include	<Set/SimpleSet.h>

class Node;
class	Element;
class	BasisFunctions;
class	Transformation
{
	private:
		Transformation(const Transformation&);
		Transformation& operator=(const Transformation&);
	protected:
		int		ndim;				// Number of usable dimensions
		double	jac[3][3];			// Jacobian
		double	ijac[3][3];			// Inverse Jacobian
		double	dj;

		double	pj[3][3];
		double	pji[3][3];
		double	pdj;

		void	Calc();		// Given j, calculate ij and dj
		void	ParCalc();		// Given j, calculate ij and dj
	public:
		Transformation() { ndim = 3; }
		virtual	void At(const Coord3D& c) = 0;
		virtual int ParAt(const Coord3D& c, int bn) = 0;
		inline	int NumDim() const { return ndim; }
		inline	const double& J(int i, int k) const { return jac[i-1][k-1]; }
		inline	const double& Ji(int i, int k) const { return ijac[i-1][k-1]; }
		inline	const double& detJ() const { return dj; }
		inline	const double& ParJ(int i, int k) const
			{ return pj[i-1][k-1]; }
		inline	const double& ParJi(int i, int k) const
			{ return pji[i-1][k-1]; }
		inline	const double& PardetJ() const
			{ return pdj; }
};

ostream& operator<<(ostream &, Transformation &);
class	UnitTrans : public Transformation
{
	public:
		UnitTrans(int nd);
		void At(const Coord3D& c) { }
};

class	ParamTran : public Transformation
{
	protected:
		const FixedSet<Node *> *nodes;
		const BasisFunctions& bf;
	public:
		ParamTran(const BasisFunctions&);
		void Attach(const Element&);
		void Attach(const FixedSet<Node *>&);
		void At(const Coord3D&);
		int ParAt(const Coord3D&, int bn);
};

#endif