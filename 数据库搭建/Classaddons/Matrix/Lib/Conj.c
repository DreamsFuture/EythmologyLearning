/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Conj.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: Conj.c,v $
 * Revision 1.5  1993/06/10  13:59:27  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 1.5  1993/06/10  13:59:27  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 1.4  1993/05/11  15:42:00  tiller
 * Added support for memory leak traking...Everything looks good for this
 * versions.
 *
 * Revision 1.4  1993/05/11  15:42:00  tiller
 * Added support for memory leak traking...Everything looks good for this
 * versions.
 *
 * Revision 1.3  1993/03/07  17:37:41  tiller
 * Various changes to get Non-Linear Optimization Homework working
 *
 * Revision 1.2  1993/02/20  19:03:12  tiller
 * Added RCS header information
 *
 */

#include	<Conj.h>
#include	<Matrix.h>

#ifdef __GNUC__
#pragma implementation
#endif

#define	DEBUG	0
//Conj::Conj(SparseMatrix *mat) : Solver(mat)
Conj::Conj(Matrix& mat) : Solver(mat)
{
	eps = 1e-3;
}

void
Conj::Print()
{
	matrix->Print();
}

Vector
Conj::Solve(const Vector& b, double& time)
{
	Timer	timer;
	Vector	r, p, x;
	double	rho, oldrho, l;
	double	cval;
	double	Anorm;
	int	i, k, neq;
	Vector	Ap;
//	SparseMatrix&	A = (*sm);
	Matrix&	A = (*matrix);
	
//	A.Sort();
	cval = eps*b.norm();

	neq = A.NumberOfRows();

	for(i=1;i<=neq;i++) x[i] = 0.0;
	r = b;
	rho = r*r;
	oldrho = 1.0;

	timer.SetTimer();
	for(k=0;sqrt(rho)>cval;k++)
	{
		if (k==0)
			p = r;
		else
			p = r+((rho)/(oldrho))*p;

		Ap = A*p;
		Anorm = p*Ap;
#if	0
		printf("p = \n");
		p.Print();
		printf("Ap = \n");
		Ap.Print();
		printf("Anorm = %g\n", Anorm);
#endif
		l = rho/Anorm;
		x += l*p;
		r -= l*(Ap);
		oldrho = rho;
		rho = r*r;
	}
	time = timer.Elapsed();
	return(x);
}

NonSymConj::NonSymConj(Matrix& mat) : Conj(*new SymEquiv(mat))
{
	eps = 1e-3;
}

BiConj::BiConj(Matrix& mat) : Conj(mat)
{
	eps = 1e-3;
}

Vector
BiConj::Solve(const Vector& b, double& time)
{
	Vector	r, rbar, p, pbar, z, D;
	Vector	oldr, oldrbar, x;
	double	alpha, beta, cval;
	int	neq, i, iter;
	Matrix&	A = (*matrix);
	
	cval = eps*b.norm();
	neq = A.NumberOfRows();

	for(i=1;i<=neq;i++) x[i] = 0.0;

	r = rbar = p = pbar = b;
	for(iter=0;(r.norm()>cval && iter < 2*neq) || iter==0; iter++)
	{
		alpha = (rbar*r)/(pbar*A*p);
		oldr = r;
		oldrbar = rbar;
		r -= alpha*(A*p);
		rbar -= alpha*(pbar*A);
#if	DEBUG
		printf("alpha = %g\n", alpha);
		printf("||r|| = %g\n", r.norm());
		printf("  _  \n");
		printf("||r|| = %g\n", rbar.norm());
#endif
		x += alpha*p;
		beta = (rbar*r)/(oldrbar*oldr);
#if	DEBUG
		printf("x = ");
		x.Print();
		printf("beta = %g\n", beta);
#endif
		p = r+beta*p;
		pbar = rbar+beta*pbar;
	}
	printf("iters = %d\n", iter);
	printf("r.norm() [%g] = %g\n", cval, r.norm());
	return(x);
}

PreBiConj::PreBiConj(Matrix& mat) : BiConj(mat)
{
	eps = 1e-3;
}

Vector
PreBiConj::Solve(const Vector& b, double& time)
{
	Vector	r, rbar, p, pbar;
	Vector	oldr, oldrbar, x;
	double	alpha, beta, cval;
	int	neq, i, iter;
	Matrix&	A = (*matrix);
	
	cval = eps*b.norm();
	neq = A.NumberOfRows();

	for(i=1;i<=neq;i++) x[i] = 0.0;

	r = rbar = p = pbar = b;
	for(iter=0;(r.norm()>cval && iter < 2*neq) || iter==0; iter++)
	{
		alpha = (rbar*r)/(pbar*A*p);
		oldr = r;
		oldrbar = rbar;
		r -= alpha*(A*p);
		rbar -= alpha*(pbar*A);
#if	DEBUG
		printf("alpha = %g\n", alpha);
		printf("||r|| = %g\n", r.norm());
		printf("  _  \n");
		printf("||r|| = %g\n", rbar.norm());
#endif
		x += alpha*p;
		beta = (rbar*r)/(oldrbar*oldr);
#if	DEBUG
		printf("x = ");
		x.Print();
		printf("beta = %g\n", beta);
#endif
		p = r+beta*p;
		pbar = rbar+beta*pbar;
	}
	printf("iters = %d\n", iter);
	printf("r.norm() [%g] = %g\n", cval, r.norm());
	return(x);
}

