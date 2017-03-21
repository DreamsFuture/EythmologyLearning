/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: DenseMatrix.c,v 1.6 1993/06/10 21:01:04 tiller Exp tiller $
 * ChangeLog:
 *	$Log: DenseMatrix.c,v $
 * Revision 1.6  1993/06/10  21:01:04  tiller
 * Added support for USE_SET and tested it.  It appears to work.
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

#include	<stdlib.h>
#include	<DenseMatrix.h>

#ifdef __GNUC__
#pragma implementation
#endif

DenseMatrix::DenseMatrix(const DenseMatrix& d)
{
	int	i, s = d.rows.Size();

#ifdef	USE_SET
	for(i=1;i<=s;i++)
		rows[i] = d.rows[i];
#else
	for(i=1;i<=s;i++)
		rows[i] = d.rows(i);
#endif
}
void
DenseMatrix::Print()
{
	int	i, s = rows.Size();

	for(i=1;i<=s;i++)
	{
		printf("Row %d -> ", i);
		rows[i].Print();
	}
}

void
DenseMatrix::Zero()
{
	rows.Clear();
}

int
DenseMatrix::isSym() const
{
	return 0;			// Practically impossible to check for
}

Vector
DenseMatrix::Solve(const Vector &)
{
	fprintf(stderr, "Sorry, DenseMatrix::Solve not implemented!\n");
	exit(1);
}

int
DenseMatrix::BandWidth() const
{
	if (NumberOfRows() > NumberOfCols())
		return(NumberOfRows()/2);
	else
		return(NumberOfCols()/2);
}

int
DenseMatrix::NumberOfRows() const
{
	return(rows.Size());
}

int
DenseMatrix::NumberOfCols() const
{
	int	i, s = rows.Size();
	int	max = 0;

#ifdef	USE_SET
	for(i=1;i<=s;i++)
		if (rows[i].Size() > max)
			max = rows[i].Size();
#else
	for(i=1;i<=s;i++)
		if (rows(i).Size() > max)
			max = rows(i).Size();
#endif

	return(max);
}

Vector
DenseMatrix::Post(const Vector& v) const
{
	Vector	ret;
	int	i, j, nc = NumberOfCols(), nr = NumberOfRows(), l;

	if (v.Size() > nc)
		fprintf(stderr,
			"Warning, pre-multiplying %dx%d matrix by %d length vector\n",
				nr, nc, v.Size());

#ifdef	USE_SET	
	for(i=1;i<=nr;i++)
		for(l=rows[i].Size(),j=1;j<=l;j++)
			ret[j] += v[i]*rows[i][j];
#else
	for(i=1;i<=nr;i++)
		for(l=rows(i).Size(),j=1;j<=l;j++)
			ret[j] += v[i]*rows[i][j];
#endif

	return(ret);
}

Vector
DenseMatrix::Pre(const Vector& v) const
{
	Vector	ret;
	int	i, nr = NumberOfRows(), nc = NumberOfCols();

	if (v.Size() > nc)
		fprintf(stderr,
			"Warning, post-multiplying %dx%d matrix by %d length vector\n",
				nr, nc, v.Size());

#ifdef	USE_SET
	for(i=1;i<=nr;i++)
		ret[i] = rows[i]*v;
#else
	for(i=1;i<=nr;i++)
		ret[i] = rows(i)*v;
#endif

	return(ret);
}

long
DenseMatrix::Memory() const
{
	return(sizeof(double)*NumberOfRows()*NumberOfCols());
}

const Vector	&
DenseMatrix::operator()(int i) const
{
#ifdef	USE_SET
	return(rows[i]);
#else
	return(rows(i));
#endif
}

Vector	&
DenseMatrix::operator[](int i)
{
	return(rows[i]);
}

void
DenseMatrix::PreRot(double a, int r1, int r2)
{
	double	c = cos(a), s = sin(a);

	PreRot(c, s, r1, r2);
}

void
DenseMatrix::PreRot(double c, double s, int r1, int r2)
{
	int	i, nc = NumberOfCols();
#ifdef	USE_SET
	Vector	row1 = rows[r1], row2 = rows[r2];
#else
	Vector	row1 = rows(r1), row2 = rows(r2);
#endif

	for(i=1;i<=nc;i++)
		rows[r1][i] = row1[i]*c - row2[i]*s;
	for(i=1;i<=nc;i++)
		rows[r2][i] = row1[i]*s + row2[i]*c;
}

void
DenseMatrix::PostRot(double a, int c1, int c2)
{
	double	c = cos(a), s = sin(a);
	PostRot(c, s, c1, c2);
}

void
DenseMatrix::PostRot(double c, double s, int c1, int c2)
{
	int	i, nr = NumberOfRows();
	double	col1, col2;

	for(i=1;i<=nr;i++)
	{
		col1 = rows[i][c1];
		col2 = rows[i][c2];

		rows[i][c1] =  col1*c+col2*s;
		rows[i][c2] = -col1*s+col2*c;
	}
}

Matrix &
DenseMatrix::Transpose()
{
	int	i, j;
	int	nr = NumberOfRows();
	int	nc = NumberOfCols();
	double	t;

	if (nc > nr)
	{
		for(i=1;i<=nr;i++)
			for(j=i;j<=nc;j++)
			{
				t = rows[i][j];
				rows[i][j] = rows[j][i];
				rows[j][i] = t;
			}
		for(i=1;i<=nc;i++)
			rows[i].SetSize(nr);
	}
	else
	{
		for(i=1;i<=nc;i++)
			for(j=i;j<=nr;j++)
			{
				t = rows[i][j];
				rows[i][j] = rows[j][i];
				rows[j][i] = t;
			}
		rows.SetSize(nc);
	}

	return *this;
}
