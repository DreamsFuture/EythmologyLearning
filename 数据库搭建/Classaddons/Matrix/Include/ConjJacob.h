/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: ConjJacob.h,v 1.6 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: ConjJacob.h,v $
 * Revision 1.6  1993/06/10  21:00:33  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 1.5  1993/05/26  15:45:56  tiller
 * Modified to handle const Vectors in the operator versions of
 * Solve, PreMult and PostMult.
 *
 * Revision 1.5  1993/05/26  15:45:56  tiller
 * Modified to handle const Vectors in the operator versions of
 * Solve, PreMult and PostMult.
 *
 * Revision 1.4  1993/05/10  15:11:15  tiller
 * *** empty log message ***
 *
 * Revision 1.4  1993/05/10  15:11:15  tiller
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/07  17:36:43  tiller
 * Various Changes to get Non-Linear Optimization homework working.
 *
 * Revision 1.2  1993/02/20  18:59:28  tiller
 * Added RCS header info
 *
 */

#ifndef	_CONJJACOB_H_
#define	_CONJJACOB_H_

#ifdef	__GNUC__
#pragma interface
#endif

class	SparseMatrix;
#include	<Solver.h>

class	ConjJacob : public Solver
{
	private:
		double	eps;
		SparseMatrix	*sm;

	public:
		ConjJacob(SparseMatrix *);

		void	Print();
		Vector	Solve(const Vector& b, double &);
};

#endif
