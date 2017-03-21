/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Matrix.h,v 1.6 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: Matrix.h,v $
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

#ifndef	_MATRIX_H_
#define	_MATRIX_H_
#ifdef	__GNUC__
#pragma interface
#endif

#include	<stdio.h>
#include	<Vector.h>
#include	<Set/SimpleSet.h>
#include	<Entry.h>

extern	"C"	{
#if	0
#include	<malloc.h>
#endif
double	fabs(double);
};

class   Matrix		// General Storage Class for Matrices
{
	public:
		Matrix() { };
	
		virtual	void		Print() = 0;			// Print
		virtual	void		Zero() = 0;				// Make a Zero Matrix
		virtual	int			isSym() const = 0;

		virtual	Vector		Solve(const Vector &) = 0;
		virtual	int			BandWidth()			const = 0;	// Bandwidth
		virtual	int			NumberOfRows()		const = 0;	// Number of Rows
		virtual	int			NumberOfCols()		const = 0;	// Number of Rows
		virtual	Vector		Pre(const Vector&)	const = 0;	// Pre-multiply
		virtual	Vector		Post(const Vector&)	const = 0;	// Post-multiply
		virtual	long		Memory()			const = 0;  // Memory used

		virtual ~Matrix() { };
};

class	SymEquiv : public Matrix
{
	protected:
		Matrix	&A;
	public:
		SymEquiv(Matrix &m) : A(m) { };
		void	Print();
		void	Zero();
		int		isSym() const { return 1; }
		Vector	Solve(const Vector&);
		int		BandWidth() const;
		int		NumberOfRows() const;	
		int		NumberOfCols() const;
		Vector	Pre(const Vector&) const;
		Vector	Post(const Vector&) const;
		long	Memory() const;
};

// Short-Hand
inline Vector operator/(const Vector& b, Matrix& A)
{
	return(A.Solve(b));
}
inline	Vector	operator*(const Vector& b, const Matrix& A)
{
	return(A.Post(b));
}
inline	Vector	operator*(const Vector& b, Matrix *A)
{
	return(A->Post(b));
}
inline	Vector	operator*(const Matrix& A, const Vector& b)
{
	return(A.Pre(b));
}
inline	Vector	operator*(Matrix *A, const Vector& b)
{
	return(A->Pre(b));
}

#endif
