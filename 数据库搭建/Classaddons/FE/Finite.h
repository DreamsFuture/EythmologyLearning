#ifndef	_FINITE_ELEM_
#define	_FINITE_ELEM_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Finite.h,v 3.4 1993/05/25 21:31:08 tiller Exp $
 */


#include	"Element.h"
class	GenGauss;
class	BasisFunctions;
class	Transformation;
class	Const;
class	MatProp;
class	BIMatrix;
class	SVector;
class	MatCache;
class	GPInfo;

class	Finite : public Element
{
	protected:
	public:
		virtual	void Stiff(BIMatrix&, LocalDomain&) = 0;
		virtual	void TanStiff(BIMatrix&, LocalDomain&);
		virtual	void ParStiff(BIMatrix&, LocalDomain&, int);

		virtual	void Force(SVector&, LocalDomain&) = 0;
		virtual	void TanForce(BIMatrix&, LocalDomain&);
		virtual	void ParForce(SVector&, LocalDomain&, int);
};

class	CondElem : public Finite
{
	protected:
		const BasisFunctions&	bf;
		Transformation&	tran;
		const GenGauss&	ggw;
		const Const& dof;
		const MatProp &mp1;

		protected:
			void Stiff(MatCache&, const GPInfo&, double *);
			void ParStiff(MatCache&, const GPInfo&, double *, int);
	public:
		CondElem(const BasisFunctions& b, Transformation &t, const Const&d,
		         const GenGauss& g, const MatProp &m);
		virtual	void Stiff(BIMatrix&, LocalDomain&);
		virtual	void TanStiff(BIMatrix&, LocalDomain&);
		virtual	void ParStiff(BIMatrix&, LocalDomain&, int);

		virtual	void Force(SVector&, LocalDomain&);
//		virtual	void TanForce(BIMatrix&, Domain&, const Vector&);
//		virtual	void ParForce(SVector&, Domain&, int);
};

class	ConvElem : public Finite
{
	protected:
		const BasisFunctions&	bf;
		Transformation&	tran;
		const GenGauss&	ggw;
		const Const& dof;
		const MatProp &htc;
		const MatProp &tinf;

		void Stiff(MatCache&, const GPInfo&, double *);
		void ParStiff(MatCache&, const GPInfo&, double *, int);
	public:
		ConvElem(const BasisFunctions& b, Transformation &t, const Const&d,
		         const GenGauss& g, const MatProp &h, const MatProp &T);
		virtual	void Stiff(BIMatrix&, LocalDomain&);
		virtual	void TanStiff(BIMatrix&, LocalDomain&);
		virtual	void ParStiff(BIMatrix&, LocalDomain&, int);

		virtual	void Force(SVector&, LocalDomain&);
//		virtual	void TanForce(BIMatrix&, Domain&, const Vector&);
		virtual	void ParForce(SVector&, LocalDomain&, int);
};

#endif