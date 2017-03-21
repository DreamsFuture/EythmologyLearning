/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Entry.h,v 1.6 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: Entry.h,v $
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

#ifndef		_ENTRY_H_
#define		_ENTRY_H_

#ifdef	__GNUC__
#pragma interface
#endif

#include	<stdio.h>
#include	<Misc/GC.h>

// An entry in the matrix (see Sparse.h & GC.h)

class	Entry : public AllocGC
{
	public:
		AllocGCInterface;

		int	i, j;
		double	v;
		Entry(int, int, double);
		Entry(const Entry &);
		~Entry();

		inline void Print() { printf("i = %d, j = %d, v = %g\n", i, j, v); }

		inline	int	operator> (Entry &b)
		{
			if (i > b.i)
				return(1);
			if (b.i == i && j > b.j)
				return(1);
			return(0);
		}

		inline	int	operator< (Entry &b)
		{
			if (i < b.i)
				return(1);
			if (b.i == i && j < b.j)
				return(1);
			return(0);
		}

		inline	int	operator== (Entry &b)
		{
			if (b.i == i && j == b.j)
				return(1);
			return(0);
		}

		friend class Matrix;
		friend class Solver;
		friend class YSMP;
		friend class Banded;
		friend class Jacobi;
		friend class Gauss;
		friend class Conj;
		friend class SOR;
		friend class ConjJacob;
};

#endif
