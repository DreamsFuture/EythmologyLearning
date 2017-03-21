/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */
/*
 * $Id: DOTOpt.c,v 1.2 1993/06/10 21:03:58 tiller Exp tiller $
 * ChangeLog:
 *	$Log: DOTOpt.c,v $
 * Revision 1.2  1993/06/10  21:03:58  tiller
 * *** empty log message ***
 *
 * Revision 1.1  1993/05/09  19:00:00  tiller
 * Initial revision
 *
 * Revision 2.7  1993/05/04  22:51:02  tiller
 * Made some radical changes to make Domain abstract and to modularize
 * things a little more.  The System heirarchy is probably in a state of
 * flux.
 *
 * Revision 2.6  1993/04/20  18:47:21  tiller
 * Too many changes to list...mostly just testing shape sensitivity!
 *
 * Revision 2.6  1993/04/20  18:47:21  tiller
 * Too many changes to list...mostly just testing shape sensitivity!
 *
 * Revision 2.5  1993/03/30  20:00:51  tiller
 * I seem to have fixed some bugs in how boundary conditions are handled.
 * In addition, Everything seems to be working...next I have to check
 * for some phantom sensitivities...hopefully they will be gone.  There
 * does seem to be a problem with the sensitivities be permuted by the
 * System::Direct method.
 *
 * Revision 2.5  1993/03/30  20:00:51  tiller
 * I seem to have fixed some bugs in how boundary conditions are handled.
 * In addition, Everything seems to be working...next I have to check
 * for some phantom sensitivities...hopefully they will be gone.  There
 * does seem to be a problem with the sensitivities be permuted by the
 * System::Direct method.
 *
 * Revision 2.4  1993/03/23  23:15:45  tiller
 * Tangent terms appears to be correct.  They have been tested on a ~200
 * node problem and appear to work correctly.
 *
 * Revision 2.4  1993/03/23  23:15:45  tiller
 * Tangent terms appears to be correct.  They have been tested on a ~200
 * node problem and appear to work correctly.
 *
 * Revision 2.3  1993/03/15  14:30:23  tiller
 * The 'Finite' abstraction seems to be working.
 *
 * Revision 2.3  1993/03/15  14:30:23  tiller
 * The 'Finite' abstraction seems to be working.
 *
 * Revision 2.2  1993/03/07  17:54:00  tiller
 * Things seem to work for the Non-Linear Heat Transfer examples.
 * I'm about to change the interfaces for some optimization reasons.
 * For now, everything seems stable.
 *
 * Revision 2.2  1993/03/07  17:54:00  tiller
 * Things seem to work for the Non-Linear Heat Transfer examples.
 * I'm about to change the interfaces for some optimization reasons.
 * For now, everything seems stable.
 *
 * Revision 2.1  1993/02/04  00:18:59  tiller
 * Minor Changes
 *
 */

#include	"DOTOpt.h"
#include	<stdlib.h>

#ifdef	__GNUC__
#pragma implementation
#endif

extern	"C"
{
void
MAIN_()
{
	fprintf(stderr, "Stupid run-time FORTRAN crap\n");
	exit(1);
}
}

DOT::DOT(Optimizable& o, DesignVec x0, int meth, int pr,
	int NRWK, int NRIWK) : Optimizer(o, x0), opt(o)
{
	int	i;

	// Initialize Sizes
	ncon = 0;
	nrwk = NRWK;
	nriwk = NRIWK;

	// Allocate vectors on the fly
	xl = new double[nv];
	xu = new double[nv];

	// Allocate more vectors
	wk = new double[nrwk];
	rprm = new double[20];
	iwk = new int[NRIWK];
	iprm = new int[20];

	// Miscellaneous
	iprint=pr;
	minmax=-1;
	method = meth;
	instruction = Done;

	// Initialize vectors
	for(i=0;i<20;i++)
	{
		rprm[i]=0.0;
		iprm[i]=0;
	}

	iprm[0] = 1;
	for(i=0;i<nv;i++)
	{
		xl[i] = -1.0E+20;
		xu[i] = 1.0E+20;
	}

	Init();
}

void
DOT::Init()
{
	obj = opt.Function(pos);

	instruction = Done;
	dot_(&instruction, &method, &iprint, &nv, &ncon, pos, xl, xu, &obj,
	     &minmax, NULL, rprm, iprm, wk, &nrwk, iwk, &nriwk);
}

int
DOT::Iterate(int n, int &c)
{
	dot_(&instruction, &method, &iprint, &nv, &ncon, pos, xl, xu, &obj,
	     &minmax, NULL, rprm, iprm, wk, &nrwk, iwk, &nriwk);

	if (instruction == GetObj)
		obj = opt.Function(pos);

	if (instruction == GetGrad)
		opt.Gradient(pos, wk);

	if (instruction==Done)
	{
		c = 0;
		return 1;
	}

	c = 1;
	return 0;
}

#if	0
void
DOT::GetGradients(ScVector& B)
{
	int	ngt = iprm[19];
	int	i, j;

	ScVector	Grad;

	opt.Gradient(Grad, 1, B);
	for(i=0;i<nv;i++)
		wk[i] = Grad[i+1];

	if (ngt == 0)
		return;

	for(i=1;i<=ngt;i++)
	{
		int	con = iwk[i-1];
		opt.Gradient(Grad, con+1, B);

		for(j=1;j<=nv;j++)
			wk[i*nv+j-1] = Grad[j].Value();
	}
}
#endif

DOT::~DOT()
{
	delete[] xl;
	delete[] xu;

	delete[] wk;
	delete[] rprm;
	delete[] iwk;
	delete[] iprm;
}


