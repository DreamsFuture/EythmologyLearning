/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: YSMP.c,v 1.5 1993/06/10 21:01:04 tiller Exp tiller $
 * ChangeLog:
 *	$Log: YSMP.c,v $
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


#include	<YSMP.h>
#ifdef __GNUC__
#pragma implementation
#endif

YSMP::YSMP(SparseMatrix *mat) : Solver(*mat)		// Constructor
{
	sm = mat;
	reduced = 0;
	analyzed = 0;
	eps = 1e-15;
	flag = 0;
}
	
void
YSMP::Analyze()
{
	int	i, j, store;
	int	size;
	
	// This is a hack which basically doubles the necessary memory :-(
	if (sm->isSym())
	{
		size = sm->Size();
		for(i=0;i<size;i++)
			if (sm->Elem(i)->i < sm->Elem(i)->j)
				sm->SetEntry(sm->Elem(i)->j, sm->Elem(i)->i, sm->Elem(i)->v);
	}

	sm->Sort();

	neq = sm->Elem(sm->Size()-1)->i;	// Last Row

	for(k=0,i=0; i<sm->Size(); i++)	// Calculate K
		if (sm->Elem(i)->j > sm->Elem(i)->i)
			k++;
	k+=neq;

	// Allocate
	va = new double[sm->Size()];		// Initialize va
	ja = new int[sm->Size()];		// Initialize ja
	ia = new int[neq+1];				// Initialize ia

	p = new int[neq];					// Init Permutations
	ip = new int[neq];

	x = new double[neq];				// Allocate solution
	store = 3*neq+2*k;
	nsp = store*5;

	rsp = new double[nsp];
	isp = (int *)rsp;		// Equivalence

	// Set arrays
	for(i=0;i<neq;i++)
		p[i] = ip[i] = i+1;
	for(i=0;i<sm->Size();i++)
	{
		ja[i] = sm->Elem(i)->j;
		va[i] = sm->Elem(i)->v;
	}

	ia[0]=1;
	ia[neq]=sm->Size()+1;
	for(i=1, j=1;i<sm->Size();i++)
		if (sm->Elem(i)->i > sm->Elem(i-1)->i)
		{
			ia[j] = i+1;
			j++;
		}
	
	analyzed = 1;
}
void
YSMP::Print()
{
	int	i;
//	int	neq;

	printf("ia = {");
	for(i=0;i<=neq;i++)
		printf("%d ", ia[i]);
	printf("}\n");

	printf("ja = {");
	for(i=0;i<sm->Size();i++)
		printf("%d ", ja[i]);
	printf("}\n");

	printf("va = {");
	for(i=0;i<sm->Size();i++)
		printf("%g ", va[i]);
	printf("}\n");
}
void
YSMP::Reduce()		// Do LU Decomposition
{
	if (!analyzed)
		Analyze();

	path = 5;
	sdrv_(&neq, p, ip, ia, ja, va, x, x,
		&nsp, isp, rsp, &esp, &path, &flag);
	HandleError(flag);
	reduced = 1;
}
Vector
YSMP::Solve(const Vector& b, double& time)		// Solve
{
	Vector	ret;
	Timer	timer;
	double	*bd;
	int	i;

	if (!analyzed)
		Analyze();
	if (!reduced)
		Reduce();

	bd = new double[neq];
	for(i=0;i<neq;i++)
		bd[i] = b[i+1];

	path = 3;
	timer.SetTimer();
	sdrv_(&neq, p, ip, ia, ja, va, bd, x,
		&nsp, isp, rsp, &esp, &path, &flag);
	time = timer.Elapsed();

	ret.Load(x, neq);	// Make vector
	HandleError(flag);

	delete [] bd;
	return(ret);
}
void
YSMP::HandleError(int f)
{
    int	N, K;

    if (f==0)
		return;
    N = f/neq;
    K = f % neq;
    switch(N)
    {
		case 2:
		    fprintf(stderr, "Duplicate Entry in row %d\n", K);
		    return;
		case 6:
		    fprintf(stderr,
			"Insufficient Storage in SSF @ row %d\n", K);
		    return;
		case 7:
		    fprintf(stderr, "Insufficient Storage in SNF\n");
		    return;
		case 8:
		    fprintf(stderr, "Zero Pivot in row %d\n", K);
		    return;
		case 10:
		    fprintf(stderr, "Insufficient Storage in SDRV\n");
		    return;
		case 11:
		    fprintf(stderr, "Illegal Path specification\n");
		    return;
		default:
		    fprintf(stderr, "Unknown Error %d\n", f);
		    return;
    }
}
void
YSMP::Free()	// Release Scratch
{
	if (!analyzed)
		return;
	delete [] va;
	delete [] ia;
	delete [] ja;
	delete [] p;
	delete [] ip;
	delete [] rsp;
	analyzed = 0;
}

YSMP::~YSMP()
{
	Free();
}
