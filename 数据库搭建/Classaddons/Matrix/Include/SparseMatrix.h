/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: SparseMatrix.h,v 1.5 1993/06/10 13:59:00 tiller Exp $
 * ChangeLog:
 *	$Log: SparseMatrix.h,v $
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

#ifndef	_SPARSE_MATRIX_H_
#define	_SPARSE_MATRIX_H_
#ifdef	__GNUC__
#pragma interface
#endif

#include	<Matrix.h>

// Various Macros

#define		EqualAt(i)	( (i+1)<matrix.Size() && *Elem(i)==*Elem(i+1) )
#define		IsZero(i)	( i<matrix.Size() && fabs(Elem(i)->v) < eps )

enum	ListStatus	{ Normal, Sorted, EmptyList };
enum	SolverType	{ BandedSolver, ConjugateGradient, ConjugateJacobi,
					  JacobiMethod, GaussSeidel, SparseDirect, OverRelaxation };
enum	EigenMethod	{ LanczosMethod, Householder };

// General Storage Class for Sparse Matrices
class   SparseMatrix : public Matrix
{
	protected:
		SimpleSet<Entry *> matrix;			// The list of enties
		Solver	*solver;
		ListStatus		status;	// Status of the original List
		double	eps;
		double	time;			// After an operation, the time it took to do it is here.
		int	symmetric;

		Solver	*GetSolver(SolverType);

	public:

		SparseMatrix(SolverType st=SparseDirect, int issym=0);	// Constructor
		SparseMatrix(const SparseMatrix&);
	
		void			SetSolver(SolverType);
		virtual	void	Print();				// Print
		void			Sort();					// Group duplicate entries
		void			Zero();

		Entry			*Elem(int i) const	{ return(matrix[i]); }
		inline	void	Free()				{ Zero(); status = EmptyList; }
		inline	int		isSym()	const		{ return(symmetric); }
		inline	double	Time()				{ return(time); }

		void SetEntry(int, int, double);
		void operator+=(Entry *);
		void operator()(int, int, double);
		Vector	Solve(const Vector &);
		Vector	Solve(Vector, SolverType);
		int	BandWidth() const;
		int	NumberOfRows() const;
		int	NumberOfCols() const;
		long Memory() const;
		void Reduce(int, double, Vector& b);
		inline Vector	Pre(const Vector& b) const		//	Pre-multiply
		{
			Vector	ret;
			int	k;

			for(k=0;k<Size();k++)
				ret[Elem(k)->i]+=
					Elem(k)->v*b[Elem(k)->j];
			return(ret);
		}
		inline Vector	Post(const Vector& b) const		// Post-multiply
		{
			Vector	ret;
			int	k;

			for(k=0;k<Size();k++)
				ret[Elem(k)->j]+=
					Elem(k)->v*b[Elem(k)->i];
			return(ret);
		}
		int	Size() const;
//		inline	void	SetBlockSize(int b) { matrix.SetBlockSize(b); }

		virtual ~SparseMatrix();
};

SparseMatrix*	ReadMatrixFromFile(FILE *);
#endif
