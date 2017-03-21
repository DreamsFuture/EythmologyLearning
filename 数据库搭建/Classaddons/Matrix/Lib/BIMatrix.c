/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: BIMatrix.c,v 1.6 1993/06/10 21:01:04 tiller Exp tiller $
 * ChangeLog:
 *	$Log: BIMatrix.c,v $
 * Revision 1.6  1993/06/10  21:01:04  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 1.5  1993/06/10  13:59:27  tiller
 * *** empty log message ***
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

#include	<BIMatrix.h>

#ifdef __GNUC__
#pragma implementation
#endif

void
BIMatrix::Print()
{
	int	i, j;
	int	col=0;
	double	val=0;

	for(i=1;i<=mat.Size();i++)
		for(j=1;mat[i].Get(j,col,val);j++)
			printf("(%d, %d, %g)\n", i, col, val);
}

void
BIMatrix::ScalarMult(double s)
{
	int	i, j, col=0;
	double	val = 0;

	for(i=1;i<=mat.Size();i++)
		for(j=1;mat[i].Get(j,col,val);j++)
			mat[i][col]*=s;
}

Vector
BIMatrix::Eliminate(int p, double obj)
{
	SVector	dummy;
	int	i, j, col;
	double	val;
	Vector	ret;

	dummy[p] = 1.0;
	mat[p] = dummy;
	for(i=1;i<=mat.Size();i++)
		if (i!=p)
#ifdef	USE_SET
			for(j=1;mat[i].Get(j,col,val);j++)
#else
			for(j=1;mat(i).Get(j,col,val);j++)
#endif
				if (col==p)
				{
					ret[i] -= obj*val;
					mat[i][col] = 0.0;
				}
	return ret;
}

void
BIMatrix::Zero()
{
	mat.Empty();
}

Vector
BIMatrix::Solve(const Vector &b)
{
	Vector	ret;
	fprintf(stderr, "BIMatrix::Solve not implemented!\n");
	
	return(ret);
}

int
BIMatrix::BandWidth() const
{
	fprintf(stderr, "BIMatrix::BandWidth not implemented!\n");
	return(mat.Size());
}

int
BIMatrix::NumberOfRows() const
{
	return(mat.Size());
}

int
BIMatrix::NumberOfCols() const
{
	fprintf(stderr, "BIMatrix::NumberOfCols not implemented!\n");
	return(mat.Size());
}

Vector
BIMatrix::Pre(const Vector &v) const
{
	int	i, j, col=0;
	double	val=0.0;
	Vector	ret;

#ifdef	USE_SET
	for(i=1;i<=mat.Size();i++)
		for(j=1;mat[i].Get(j,col,val);j++)
			ret[i]+=v[col]*val;
#else
	for(i=1;i<=mat.Size();i++)
		for(j=1;mat(i).Get(j,col,val);j++)
			ret[i]+=v[col]*val;
#endif

	return ret;
}

Vector
BIMatrix::Post(const Vector &v) const
{
	int	i, j, col=0;
	double	val=0.0;
	Vector	ret;

#ifdef	USE_SET
	for(i=1;i<=mat.Size();i++)
		for(j=1;mat[i].Get(j,col,val);j++)
			ret[col]+=v[i]*val;
#else
	for(i=1;i<=mat.Size();i++)
		for(j=1;mat(i).Get(j,col,val);j++)
			ret[col]+=v[i]*val;
#endif
	return ret;
}

SVector&
BIMatrix::operator[](int p)
{
	return mat[p];
}

long
BIMatrix::Memory() const
{
	fprintf(stderr, "BIMatrix::Memory not implemented!\n");
	return 0;
}

void
BIMatrix::operator+=(const BIMatrix &m)
{
	int	i, j, col=0;
	double	val=0.0;

#ifdef	USE_SET
	for(i=1;i<=m.mat.Size();i++)
		for(j=1;m.mat[i].Get(j,col,val);j++)
			mat[i][col]+=val;
#else
	for(i=1;i<=m.mat.Size();i++)
		for(j=1;m.mat(i).Get(j,col,val);j++)
			mat[i][col]+=val;
#endif
}

void
BIMatrix::operator-=(const BIMatrix &m)
{
	int	i, j, col=0;
	double	val=0.0;

#ifdef	USE_SET
	for(i=1;i<=m.mat.Size();i++)
		for(j=1;m.mat[i].Get(j,col,val);j++)
			mat[i][col]-=val;
#else
	for(i=1;i<=m.mat.Size();i++)
		for(j=1;m.mat(i).Get(j,col,val);j++)
			mat[i][col]-=val;
#endif
}
