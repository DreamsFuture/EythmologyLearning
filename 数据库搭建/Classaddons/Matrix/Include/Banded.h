/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Banded.h,v 1.6 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: Banded.h,v $
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

#ifndef	_BMATRIX_H_
#define	_BMATRIX_H_

#ifdef	__GNUC__
#pragma interface
#endif

#ifndef	OLD
#include	<Gen/FArray.h>
#endif
#include	<Solver.h>

//	Some handy macros

#define	COL(i, j)	((i)-(diag-(j))+1) //	Calculate Column
#define	LHBW(i)		( (i)>=HBW ? HBW : (i) ) // Half Bandwidth to the left
#define	RHBW(i)		( ((i)+HBW)< nrows ? HBW : nrows-(i)-1 ) // Half Bandwidth to the right

// Define a Banded --> Banded Matrix 
class	Banded	: public Solver
{
	// Members
	public:
		int	inited;
		int	BW, HBW;		// BandWidth and HalfBandWidth 
		short	reduced;		// Is it reduced?
		short	symmetric;		// Is it symmetric

	private:
		int	nrows;			// Number of Rows 
		int	diag;			// What col is the diagonal on
#ifdef	OLD
		double	**m;		// The internal (compressed) matrix
#else
		FArray<double>	m;
#endif
		SparseMatrix	*sm;


	// Methods
	public:

		// Tell compiler how to make a Banded 
		//		nr = number of rows
		//		hbw = half bandwidth
		//		sym = 1 if symmetric

		Banded(SparseMatrix *);
		void	Init(int, int, int);

		Vector	Solve(const Vector& bi, double&);

		void	Print();
		virtual ~Banded();

	private:

		void	Reduce();
		void	ForwardSubstitute(Vector& a, const Vector& b);
		void	BackSubstitute(Vector& a, Vector& b);
		void	Assign(int, int, double);

	// Misc
	public:
		double&	operator()(int i, int j);
};
#endif
