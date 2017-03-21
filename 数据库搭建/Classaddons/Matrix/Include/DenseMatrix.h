/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: DenseMatrix.h,v 1.6 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: DenseMatrix.h,v $
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

#ifndef	_DENSE_MATRIX_H_
#define	_DENSE_MATRIX_H_

#ifdef	__GNUC__
#pragma interface
#endif

#include	<Matrix.h>
#ifdef	USE_SET
#include	<Set/SimpleSet.h>
#else
#include	<Gen/Array.h>
#endif

class   DenseMatrix	: public Matrix	// General Storage Class for Matrices
{
	protected:
#ifdef	USE_SET
		SimpleSet<Vector>	rows;
#else
		Array<Vector>		rows;
#endif
	public:
		DenseMatrix() { };
		DenseMatrix(const DenseMatrix&);
	
		// Matrix virtuals
		void		Print();			// Print
		void		Zero();				// Make a Zero Matrix
		int			isSym() const;
		Vector		Solve(const Vector &);
		int			BandWidth()			const;	// Bandwidth
		int			NumberOfRows()		const;	// Number of Rows
		int			NumberOfCols()		const;	// Number of Rows
		Vector		Pre(const Vector&)	const;	// Pre-multiply
		Vector		Post(const Vector&)	const;	// Post-multiply
		long		Memory()			const;  // Memory used

		// Non-inherited methods
		Vector	&operator[](int);
		const	Vector	&operator()(int) const;
		void	PreRot(double,int,int);			// Premultiply by rotation matrix
		void	PostRot(double,int,int);		// Postmultiply by rotation matrix
		void	PreRot(double,double,int,int);	// Premultiply by rotation matrix
		void	PostRot(double,double,int,int);	// Postmultiply by rotation matrix
		Matrix&	Transpose();					// Transpose the Matrix

		~DenseMatrix() { };
};

#endif
