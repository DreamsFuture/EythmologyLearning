/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Tridiagonal.h,v 1.5 1993/06/10 13:59:00 tiller Exp $
 * ChangeLog:
 *	$Log: Tridiagonal.h,v $
 * Revision 1.5  1993/06/10  13:59:00  tiller
 * Everything seems to be working under gcc-2.3.3
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

#ifndef	_TRIDIAGONAL_H_
#define	_TRIDIAGONAL_H_
#ifdef	__GNUC__
#pragma interface
#endif

#include	<Matrix.h>

class   Tridiagonal		// General Storage Class for Matrices
{
	protected:
		Vector	*diag;
		Vector	*sup;
		Vector	*sub;
	public:
		Tridiagonal(int issym=1);
		Tridiagonal(const Vector& d, const Vector &od);
		Tridiagonal(const Vector& d, const Vector &sp, const Vector& sb);
		Tridiagonal(const Tridiagonal& T);
	
		void		Print();			// Print
		void		Zero();				// Make a Zero Matrix
		int			isSym() const;

		int			Length() const;
		Vector		Solve(const Vector &);
		int			BandWidth()			const;	// Bandwidth
		int			NumberOfRows()		const;	// Number of Rows
		int			NumberOfCols()		const;	// Number of Rows
		Vector		Pre(const Vector&)	const;	// Pre-multiply
		Vector		Post(const Vector&)	const;	// Post-multiply
		long		Memory()			const;  // Memory used
		double&		Sub(int);
		double&		Sup(int);
		double&		Diag(int);
		Vector&		Sub();
		Vector&		Sup();
		Vector&		Diag();
		Vector&		Sub() const;
		Vector&		Sup() const;
		Vector&		Diag() const;
//		Vector		CopySub() const;
//		Vector		CopySup() const;
//		Vector		CopyDiag() const;

		~Tridiagonal();
};
#endif
