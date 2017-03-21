/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Solver.h,v 1.6 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: Solver.h,v $
 * Revision 1.6  1993/06/10  21:00:33  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 1.5  1993/06/10  13:59:00  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 1.4  1993/05/11  15:42:32  tiller
 * Added support for tracking memory leaks.  Everything in this versions
 * looks good.
 *
 * Revision 1.4  1993/05/11  15:42:32  tiller
 * Added support for tracking memory leaks.  Everything in this versions
 * looks good.
 *
 * Revision 1.3  1993/03/07  17:36:43  tiller
 * Various Changes to get Non-Linear Optimization homework working.
 *
 * Revision 1.3  1993/03/07  17:36:43  tiller
 * Various Changes to get Non-Linear Optimization homework working.
 *
 * Revision 1.2  1993/02/20  18:59:28  tiller
 * Added RCS header info
 *
 */

#ifndef	_SOLVER_H_
#define	_SOLVER_H_

#ifdef	__GNUC__
#pragma interface
#endif

#include	<Set/SimpleSet.h>
#include	<Entry.h>
#include	<Vector.h>
#include	<Misc/Timer.h>
//#include	<Matrix.h>

class	Matrix;

// A solver is any number of classes capable of solving a matrix in sparse storage

class	Solver 
{
	protected:
		Matrix	*matrix;
	public:
		Solver(Matrix&);
		virtual Vector	Solve(const Vector&, double&) = 0;

// Wish list:
//		virtual long	Memory();
};

#endif
