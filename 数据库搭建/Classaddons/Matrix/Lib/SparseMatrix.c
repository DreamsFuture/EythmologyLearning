/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: SparseMatrix.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: SparseMatrix.c,v $
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

#include	<stdlib.h>
#include	<SparseMatrix.h>
#include	<YSMP.h>
#include	<Jacobi.h>
#include	<Banded.h>
#include	<Gauss.h>
#include	<SOR.h>
#include	<ConjJacob.h>
#include	<Conj.h>
#include	<ctype.h>
#include	<math.h>
#include	"alleigen.h"

extern	"C" {
extern	int	atoi(const char *);
extern	double	atof(const char *);

};

#ifdef __GNUC__
#pragma implementation
#endif


double	SOR::omega=1.0;

SparseMatrix::SparseMatrix(SolverType st, int issym) : Matrix()	// Constructor
{
	status = EmptyList;
	if (solver!=NULL)
		delete solver;
	solver = GetSolver(st);
	symmetric = issym;
	eps = 1e-15;
	time = 0.0;
}

SparseMatrix::SparseMatrix(const SparseMatrix &M)
{
	fprintf(stderr, "Warning, copying a matrix (not a good idea)!\n");
	status = EmptyList;
	solver = M.solver;
	matrix = M.matrix;
	symmetric = M.symmetric;
	eps = 1e-15;
	time = 0.0;
}
	
void
SparseMatrix::Reduce(int pos, double val, Vector& b)
{
	int	i;
	
	Sort();
	for(i=0;i<Size();i++)
	{
		if (Elem(i)->i == pos && Elem(i)->j == pos)
		{
			if (&b!=NULL)
				b[Elem(i)->i] = val;
			Elem(i)->v = 1.0;
		}
		if (Elem(i)->i == pos && Elem(i)->j != pos)
			Elem(i)->v = 0.0;
		if (Elem(i)->i != pos && Elem(i)->j == pos)
		{
			if (&b!=NULL)
				b[Elem(i)->i] -= Elem(i)->v*val;
			Elem(i)->v = 0.0;
		}
	}
	Sort();
}
Solver	*
SparseMatrix::GetSolver(SolverType st)
{
	switch(st)
	{
		case	SparseDirect:
			return(new YSMP(this));
		case	BandedSolver:
			return(new Banded(this));
		case	JacobiMethod:
			return(new Jacobi(this));
		case	GaussSeidel:
			return(new Gauss(this));
		case	OverRelaxation:
			return(new SOR(this));
		case	ConjugateGradient:
			return(new Conj(*this));
		case	ConjugateJacobi:
			return(new ConjJacob(this));
		default:
			fprintf(stderr, "Unimplemented Solver!\n");
			return(NULL);
	}
}

void
SparseMatrix::SetSolver(SolverType st)
{
	if (solver!=NULL)
		delete solver;
	solver=GetSolver(st);
}

void SparseMatrix::Print()
{
	int	i;
	foreach(i,matrix)
		matrix[i]->Print();
}

void	SparseMatrix::Zero()
{
	matrix.Empty();
	status = EmptyList;
}

void
SparseMatrix::Sort()			// Group duplicate entries
{
	int i;

	if (status==Sorted)
		return;
	PtrSetSorter<Entry>	sorter(matrix);
	sorter.QuickSort();
	for(i=0;i<matrix.Size()-1;i++)
	{
		while(EqualAt(i) || IsZero(i))
		{
			if (EqualAt(i))
			{
				Elem(i)->v+=Elem(i+1)->v;
				matrix[i+1]->unref();
				matrix.Remove(i+1);
			}
			if (IsZero(i))
			{
				matrix[i]->unref();
				matrix.Remove(i);
			}
		}
	}
	status = Sorted;
}
void
SparseMatrix::SetEntry(int i, int j, double val)
{
	if (symmetric && (i > j) )
		return;

	int	i;
	Entry	*elem = new Entry(i,j,val);

	foreach(i,matrix)
		while(*matrix[i] == *elem && i<=matrix.UBound())
		{
			matrix[i]->unref();
			matrix.Remove(i);
		}
	matrix+=elem;
	status = Normal;
}
void
SparseMatrix::operator+=(Entry *b)
{
	if (symmetric && (b->i > b->j) )
		return;

	matrix+=b;
	status = Normal;
}
void
SparseMatrix::operator()(int i, int j, double val)
{
	if (symmetric && i > j )
		return;

	matrix+=new Entry(i, j, val);
	status = Normal;
}
Vector
SparseMatrix::Solve(const Vector& b)
{
	if (solver==NULL)
		SetSolver(SparseDirect);
	return(solver->Solve(b, time));
}
Vector
SparseMatrix::Solve(const Vector b, SolverType st)
{
	SetSolver(st);
	return(Solve(b));
}

int
SparseMatrix::NumberOfRows() const
{
	int	i, s = matrix.Size();
	int	rows = 0;

	for(i=0;i<s;i++)
		if (Elem(i)->i > rows)
			rows = Elem(i)->i;
	return(rows);
}

int
SparseMatrix::NumberOfCols() const
{
	int	i, s = matrix.Size();
	int	cols = 0;

	for(i=0;i<s;i++)
		if (Elem(i)->i > cols)
			cols = Elem(i)->i;
	return(cols);
}
int
SparseMatrix::BandWidth() const
{
	int	k, s = matrix.Size();
	int	bw = 0, i, j, d;

	for(k=0;k<s;k++)
	{
		i = Elem(k)->i;
		j = Elem(k)->j;
		d = ( i>j ? i-j : j-i );
		if (bw < d)
			bw = d;
	}
	return(bw+1);
}
SparseMatrix::~SparseMatrix()
{
	Free();
}

SparseMatrix *
ReadMatrixFromFile(FILE *fp)
{
	SparseMatrix	*ret = new SparseMatrix;
	int	i, j;
	char	*ip, *jp, *vp;
	double	v;
	char	line[100];

	while(fgets(line, 99, fp)!=NULL)
	{
		ip = line;
		for(jp=ip;isspace(*jp);jp++);
		for(;!isspace(*jp);jp++);
		for(;isspace(*jp);jp++);
		for(vp=jp;isspace(*vp);vp++);
		for(;!isspace(*vp);vp++);
		for(;isspace(*vp);vp++);
		i = atoi(ip);
		j = atoi(jp);
		v = atof(vp);
		printf("Parsed (%d,%d,%g)\n", i, j, v);
		(*ret)(i, j, v);
	}
	return(ret);
}

int
SparseMatrix::Size() const
{
	return(matrix.Size());
}

long
SparseMatrix::Memory() const
{
	return(sizeof(Entry)*Size());
}
