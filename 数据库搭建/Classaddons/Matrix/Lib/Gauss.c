/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Gauss.c,v 1.5 1993/06/10 21:01:04 tiller Exp tiller $
 * ChangeLog:
 *	$Log: Gauss.c,v $
 * Revision 1.5  1993/06/10  21:01:04  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 1.4  1993/05/26  15:45:16  tiller
 * *** empty log message ***
 *
 * Revision 1.4  1993/05/26  15:45:16  tiller
 * *** empty log message ***
 *
 * Revision 1.3  1993/03/07  17:37:41  tiller
 * Various changes to get Non-Linear Optimization Homework working
 *
 * Revision 1.3  1993/03/07  17:37:41  tiller
 * Various changes to get Non-Linear Optimization Homework working
 *
 * Revision 1.2  1993/02/20  19:03:12  tiller
 * Added RCS header information
 *
 */


#include	<Gauss.h>

#ifdef __GNUC__
#pragma implementation
#endif

Gauss::Gauss(SparseMatrix *mat) : Solver(*mat)
{
	sm = mat;
	eps = 1e-5;
}

Vector
Gauss::Solve(const Vector& b, double& time)
{
	Timer	timer;
	int	nrows;
	Vector	guess;
	double	err = 1e5;
	int		*first;
	int	i, cr;
	int	last;
	double	sum, diag, perr, temp;
	double	gv, mv;

	sm->Sort();
	nrows = sm->NumberOfRows();

	first = new int[nrows+1];
	// Collect Info
	for(cr=1,i=0;i<sm->Size();cr++)
		for(first[cr]=i; i<sm->Size() && cr==sm->Elem(i)->i; i++)
			if (sm->Elem(i)->i == sm->Elem(i)->j)
				guess[sm->Elem(i)->i] = b[sm->Elem(i)->i]/sm->Elem(i)->v;

	timer.SetTimer();
	for(;err>eps;)
	{
		err=0.0;
		for(cr=1;cr<=nrows;cr++)
		{
			sum=b[cr];
			last = (cr==nrows ? sm->Size() : first[cr+1]);
			for(i=first[cr]; i<last ;i++)		// Multiply by row cr
				if (sm->Elem(i)->i==sm->Elem(i)->j)
					diag = sm->Elem(i)->v;
				else
				{
					gv = guess[sm->Elem(i)->j];
					mv = sm->Elem(i)->v;
					sum = sum - mv*gv;
				}
			temp = sum/diag;
			perr = fabs(guess[cr]-temp);
			if (perr > err)
				err = perr;
			guess[cr] = temp;
		}
	}
	time = timer.Elapsed();
	return(guess);
}
