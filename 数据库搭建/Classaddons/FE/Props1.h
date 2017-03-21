#ifndef	_PROPS_1_
#define	_PROPS_1_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * File: $Id: Props1.h,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#include	"MatProp.h"

class	GPInfo;

class	NonLinCond1 : public MatProp
{
	protected:
		double	k;
		const	Const& dof;
	public:
		NonLinCond1(double mp, const Const& d);
		const Const& DOF() const { return dof; }
		void	Eval(double *, LocalDomain&,
						Element&, const GPInfo&) const;
		int	TanEval(double *, const Const&, LocalDomain&,
						Element&, const GPInfo&) const;
		int	ParEval(double *, int bn, LocalDomain&,
						Element&, const GPInfo&) const;
};

class	RadConv1 : public MatProp
{
	protected:
		double	h;
		const	Const& dof;
		const	MatProp&	Ti;
		static	const	double	sfb;
	public:
		RadConv1(double mp, const Const& d, const MatProp& t);
		const Const& DOF() const { return dof; }
		void	Eval(double *, LocalDomain&,
						Element&, const GPInfo&) const;
		int	TanEval(double *, const Const&, LocalDomain&,
						Element&, const GPInfo&) const;
		int	ParEval(double *, int bn, LocalDomain&,
						Element&, const GPInfo&) const;
};

#endif
