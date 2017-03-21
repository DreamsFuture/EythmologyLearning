/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Banded.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: Banded.c,v $
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

#include	<SparseMatrix.h>
#include	<Banded.h>
#include	<time.h>
#include	<prof.h>
#include	<Matrix.h>

//	Some handy macros

#define	COL(i, j)	((i)-(diag-(j))+1) //	Calculate Column
#define	LHBW(i)		( (i)>=HBW ? HBW : (i) ) // Half Bandwidth to the left
#define	RHBW(i)		( ((i)+HBW)< nrows ? HBW : nrows-(i)-1 ) // Half Bandwidth to the right

#ifdef	OLD
#define	INDEX(m, i, j)	(m[i][j])
#else
#define	INDEX(m, i, j)	m(i,j)

#endif
#ifdef __GNUC__
#pragma implementation
#endif


Banded::Banded(SparseMatrix *mat) : Solver(*mat)
{
	sm = mat;
	inited = 0;
}

void
Banded::Init(int nr, int hbw, int sym)
{
	int	i, j;

	nrows = nr;				// Store number of rows
	BW = 2*hbw+1;			// Calculate bandwidth
	HBW = hbw;				// Store Half Bandwidth
	symmetric = sym;		// Tells whether it is symmetric
	reduced = 0;			// Not reduced yet.
	if (!symmetric)
		diag = hbw+1;			// Store diag
	else
		diag = 0;				// Store diag

#ifdef	OLD
	m = new double *[nr];	// Allocate rows
	for(i=0;i<nr;i++)
	{
		m[i] = new double[diag+HBW+1];	// Allocate columns for each row
		for(j=0;j<diag+HBW;j++)			// Initialize to identity matrix
			m[i][j] = 0.0;
	}
#else
	m.Init(nr, diag+HBW+1, 0.0);
#endif
	inited = 1;
}

Vector
Banded::Solve(const Vector& bi, double& time)	// Given a vector 'bi' Solve
{
	Vector	a;				// Solution Vector
	Vector	b;
	Timer	timer;

	a[bi.Size()] = 0.0;	// Allocate now
	b[bi.Size()] = 0.0;	// Allocate now
	sm->Sort();
	if (!inited)
	{
		int	k, temp1, temp2, temp3;
		sm->Sort();
		temp1 = sm->NumberOfRows();
		temp2 = sm->BandWidth()-1;
		temp3 = sm->isSym();
		Init(temp1, temp2, temp3);
		if (symmetric)
			for(k=0;k<sm->Size();k++)
				Banded::Assign(sm->Elem(k)->i, sm->Elem(k)->j, sm->Elem(k)->v);
		else
			for(k=0;k<sm->Size();k++)
				Banded::Assign(sm->Elem(k)->i, sm->Elem(k)->j, sm->Elem(k)->v);

		timer.SetTimer();
		Reduce();	// If not already reduced, do so
	}
	if (!reduced)
	{
		timer.SetTimer();
		Reduce();	// If not already reduced, do so
	}

	ForwardSubstitute(b, bi);	// Forward Substitute
	BackSubstitute(a, b);		// Back Substitute
	time = timer.Elapsed();

	return(a);
}

void
Banded::Reduce()	// Tell compiler how to reduce a Banded
{
	register	int	i, j, k;
	int	k1, d1;
	register	int	row, col;
	int	rhbw, rhbw1;
	double	*t;				// Temporary storage

	if (reduced)			// Don't do anything if already reduced
		return;

	t = new double[HBW+1];	// Allocate temporary storage

	d1 = diag+1;
	if (symmetric)
		for(i=0;i<nrows;i++)
		{
			rhbw = RHBW(i);
			rhbw1 = RHBW(i+1);

			// Calculate new Column in Lower matrix
			for(j=1;j<=rhbw;j++)
				t[j] = INDEX(m,i,j)/INDEX(m,i,diag);

			// Subtract row 'i' from next HBW rows
			for(k=1;k<=rhbw;k++)
				for(row=i+k, k1=k-1, col=diag-k1, j=k1;j<=rhbw1;j++)
					INDEX(m,row,col+j)-=t[k]*INDEX(m,i,d1+j);

			for(j=1;j<=rhbw;j++)
				INDEX(m,i,diag+j) = t[j];
		}
	else
		for(i=0;i<nrows;i++)
		{
			rhbw = RHBW(i);
			rhbw1 = RHBW(i+1);

			// Calculate New Column in Lower matrix
			for(j=1;j<=rhbw;j++)
				t[j] = INDEX(m,i+j,diag-j)/INDEX(m,i,diag);

			// Subtract row 'i' from next HBW rows
			for(k=1;k<=rhbw;k++)
				for(row=i+k, k1=k-1, col=diag-k1, j=0;j<=rhbw1;j++)
					INDEX(m,row,col+j)-=t[k]*INDEX(m,i,d1+j);

			for(j=1;j<=rhbw;j++)
				INDEX(m,i+j,diag-j)=t[j];
		}

	delete	t;	/* Free allocated storage */
	reduced = 1;
}

void
Banded::Print()			// Tell compiler how to print a Banded
{
	int	i, j, offd;
	double	val;
	int	rhbw, lhbw;

	printf("\n");
	for(i=1;i<=nrows;i++)
	{
		rhbw = RHBW(i-1);
		lhbw = LHBW(i-1);
		for(j=1;j<=nrows;j++)
		{
			val = 0.0;
			offd = j-i;

			if (!symmetric)
			{
				if (offd >= -lhbw && offd <= rhbw)
					val = INDEX(m,i-1,diag+offd); 
			}
			else
			{
				if (offd >= -lhbw && offd <=0)
					val = INDEX(m,i-1+offd,diag-offd); 
				if (offd <= rhbw && offd >0)
					val = INDEX(m,i-1,diag+offd); 
			}

			printf("%e\t", val);
		}
		printf("\n");
	}
}
	
Banded::~Banded()
{
	int	i;

#ifdef	OLD
	for(i=0;i<nrows;i++)
		delete m[i];
	delete m;
#else
	m.Clean();
#endif
}
void
Banded::ForwardSubstitute(Vector& a, const Vector& b)
{
	int	i, j;
	int	lhbw;

	if (symmetric)
		for(i=0;i<nrows;i++)		// Perform forward substitution
			for(lhbw = LHBW(i), a[i+1]=b[i+1], j=diag-lhbw; j < diag; j++)
				a[i+1] -= INDEX(m,i+j,-j)*a[COL(i, j)];
	else
		for(i=0;i<nrows;i++)		// Perform forward substitution
			for(lhbw = LHBW(i), a[i+1]=b[i+1], j=diag-lhbw; j < diag; j++)
				a[i+1] -= INDEX(m,i,j)*a[COL(i, j)];
}
void
Banded::BackSubstitute(Vector& a, Vector& b)
{
	int	i, j;
	int	rhbw;

	if (symmetric)
		for(i=nrows-1;i>=0;a[i+1]=(a[i+1])/INDEX(m,i,diag), i--)
			for(rhbw = RHBW(i), a[i+1]=b[i+1], j=diag+1; j<=diag+rhbw; j++)
				a[i+1] -= INDEX(m,i,j)*INDEX(m,i,diag)*a[COL(i, j)]; // This is an Error I think
	else
		for(i=nrows-1;i>=0;a[i+1]=(a[i+1])/INDEX(m,i,diag), i--)
			for(rhbw = RHBW(i), a[i+1]=b[i+1], j=diag+1; j<=diag+rhbw; j++)
				a[i+1] -= INDEX(m,i,j)*a[COL(i, j)];
}

void
Banded::Assign(int i, int j, double v)
{
//	static	double	dummy = 0.0;
	int	offd = j-i;

	if ( offd >= -LHBW(i-1) && offd <= RHBW(i-1) )
	{
		if (i-1 < 0 || i-1 >= nrows ||
			diag+offd < 0 ||
			diag+offd >= diag+HBW+1 )
			fprintf(stderr, "Access Error @(%d,%d) [%d]!\n",
					i, j, offd);
		INDEX(m,i-1,diag+offd)=v;
		return;
	}
	fprintf(stderr, "Access Error @(%d,%d) [%d]!\n", i, j, offd);
}
double&
Banded::operator()(int i, int j)
{
	static	double	dummy = 0.0;
	int	offd = j-i;

	if ( offd >= -LHBW(i-1) && offd <= RHBW(i-1) )
	{
		if (i-1 < 0 || i-1 >= nrows ||
			diag+offd < 0 ||
			diag+offd >= diag+HBW+1 )
			fprintf(stderr, "Access Error @(%d,%d) [%d]!\n",
					i, j, offd);
		return(INDEX(m,i-1,diag+offd));
	}

	dummy = 0.0;
	fprintf(stderr, "Access Error @(%d,%d) [%d]!\n", i, j, offd);
	return(dummy);
}
