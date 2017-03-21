/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: IdentityMatrix.h,v 1.5 1993/06/10 13:59:00 tiller Exp $
 * ChangeLog:
 *	$Log: IdentityMatrix.h,v $
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

#ifndef	_ID_MATRIX_H_
#define	_ID_MATRIX_H_
#ifdef	__GNUC__
#pragma interface
#endif

#include	<Matrix.h>

class   IdentityMatrix : public Matrix		// The Identity Matrix
{
	protected:
		int	size;
	public:
		IdentityMatrix(int s) { size = s; }
		IdentityMatrix() { size = -1; }
	
		void		Print();								// Print
		void		Zero() { };								// Make a Zero Matrix
		int			isSym() const;

		Vector		Solve(const Vector &);
		int			BandWidth() const;							// Bandwidth
		int			NumberOfRows() const { return size; }	// Number of Rows
		int			NumberOfCols() const { return size; }	// Number of Cols
		Vector		Pre(const Vector&)	const;				// Pre-multiply
		Vector		Post(const Vector&)	const;				// Post-multiply
		long		Memory() const { return sizeof(int); }		// Memory used

		~IdentityMatrix() { };
};

#endif
