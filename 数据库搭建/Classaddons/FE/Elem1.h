#ifndef	_ELEM1_H_
#define	_ELEM1_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Elem1.h,v 3.4 1993/05/25 21:31:08 tiller Exp $
 */

#include	"Finite.h"
#include	"Gauss.h"
#include	"Basis.h"
#include	"Trans.h"

class	IsoCondElem42 : public CondElem
{
	public:
		static	const BF42 e1b;
		static	ParamTran e1t;
		static	const GenGauss e1g;
	public:
		IsoCondElem42(const MatProp &, const Const& , Node *,
						Node *, Node *, Node *);
		void	Register(LocalDomain&);
		void	Stiff(BIMatrix &b, LocalDomain &d);
		void	TanStiff(BIMatrix &b, LocalDomain &d);
		void	ParStiff(BIMatrix &b, LocalDomain &d, int bn);
};

#endif

