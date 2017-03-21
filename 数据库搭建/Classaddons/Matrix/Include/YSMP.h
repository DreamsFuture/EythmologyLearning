/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: YSMP.h,v 1.5 1993/06/10 13:59:00 tiller Exp $
 * ChangeLog:
 *	$Log: YSMP.h,v $
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

#ifndef	_YSMP_H_
#define	_YSMP_H_

#ifdef	__GNUC__
#pragma interface
#endif

#include	<stdlib.h>
extern	"C"	{
#if	0
#include	<malloc.h>
#endif
};
#include	<SparseMatrix.h>
#include	<Solver.h>

// Various Macros

extern	"C"
{
	// The Fortran Subroutine SDRV
	void	sdrv_(int *n, int *p, int *ip, int *i, int *j, double *v,
				double *b, double *x, int *nsp, int *isp, double *rsp,
				int *esp, int *path, int *flag);
};

class   YSMP : public Solver		// A Solver
{
	private:
		int		reduced;	// Is this matrix reduced?
		int		analyzed;	// Is this matrix analyzed?

		// YSMP stuff
		int		*ia, *ja, *p, *ip, *isp;
		double	*va, *x, *rsp;
		int		neq, nsp, k, esp, path, flag;
		double	eps;
		SparseMatrix	*sm;

	public:
		YSMP(SparseMatrix *);
	
		// Calculate data for SDRV and allocate
		void	Analyze();
		void	Reduce();
		void	Print();
		Vector	Solve(const Vector& b, double&);
		void HandleError(int f);
		void	Free();

		virtual ~YSMP();
};

#endif
