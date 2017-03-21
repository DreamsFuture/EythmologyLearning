/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: BIMatrix.h,v 1.6 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: BIMatrix.h,v $
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

#ifndef	_BIMATRIX_H_
#define	_BIMATRIX_H_

#ifdef	__GNUC__
#pragma interface
#endif

#include	<Matrix.h>
#ifdef	USE_SET
#include	<Set/SimpleSet.h>
#else
#include	<Gen/Array.h>
#endif
#include	<SVector.h>

class	BIMatrix : public Matrix
{
	public:
	protected:
		int	sym_flag;
#ifdef	USE_SET
		SimpleSet<SVector> mat;
#else
		Array<SVector>	mat;
#endif

	public:
		BIMatrix(int s=0) { sym_flag = s; }
		void		Print();
		void		Zero();
		int			isSym() const { return sym_flag; }
		Vector		Solve(const Vector &);
		int			BandWidth() const;
		int			NumberOfRows() const;
		int			NumberOfCols() const;
		Vector		Pre(const Vector &) const;
		Vector		Post(const Vector &) const;
		SVector&	operator[](int p);
		long		Memory() const;
		void		ScalarMult(double s);
		Vector		Eliminate(int,double);
		void		operator+=(const BIMatrix &m);
		void		operator-=(const BIMatrix &m);
};

#endif
