/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: ConjJacob.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: ConjJacob.c,v $
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

#include	<new.h>

#include	<ConjJacob.h>
#include	<Matrix.h>
#include	<SparseMatrix.h>

#ifdef __GNUC__
#pragma implementation
#endif

ConjJacob::ConjJacob(SparseMatrix *mat) : Solver(*mat)
{
	sm = mat;
	eps = 1e-5;
}

void
ConjJacob::Print()
{
	matrix->Print();
}

Vector
ConjJacob::Solve(const Vector& b, double &)
{
	Vector	r, rbar, p, D, I, x;
	double	beta, newbeta, l, n, alpha;
	int	i, k, neq;
	
	sm->Sort();

	for(i=0;i<sm->Size();i++)	// Grab Diagonal and store in vector
		if (sm->Elem(i)->i == sm->Elem(i)->j)
			D[sm->Elem(i)->i] = sm->Elem(i)->v;

	neq = D.Size();

	for(i=1;i<=neq;i++) x[i] = 0.0;
	r = b;
	for(i=1;i<=neq;i++) rbar[i] = r[i]/D[i];
	p = rbar;
	for(beta=0.0,i=1;i<=neq;i++) beta+=rbar[i]*D[i]*rbar[i];

	for(k=0;beta>.001;k++)
	{
		I = matrix*p;
		n = p*I;
		l = beta/n;
		x = x + l*p;
		r = r - l*I;
		for(i=1;i<=neq;i++) rbar[i] = r[i]/D[i];
		for(newbeta=0.0,i=1;i<=neq;i++) newbeta+=rbar[i]*D[i]*rbar[i];
		alpha = newbeta/beta;
		p = rbar + alpha*p;
		beta = newbeta;
	}
	return(x);
}
