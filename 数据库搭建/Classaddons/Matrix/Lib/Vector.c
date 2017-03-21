/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Vector.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: Vector.c,v $
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

#include	<Vector.h>
#include	<assert.h>
#include	<ctype.h>
#include	<iostream.h>
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

#if	0
externn	"C"	{
extern	int	atoi(char *);
extern	double	atof(char *);
};

#endif

#ifdef __GNUC__
#pragma implementation
#endif


Vector::Vector()
{
	eps = 1e-15;
	size = 0;
    v = NULL;
}
Vector::Vector(double *b, int s)
{
	int	i;

	SetSize(s);
	for(i=0;i<s;i++)
		v[i] = b[i];
}
Vector::Vector(const Vector& V2)
{
#ifdef	MDEBUG
	malloc_enter("Vector::Vector");
#endif
	int	i;

	size = V2.size;
	v = new double[size];
	for(i=0;i<size;i++)
		v[i] = V2.v[i];
#ifdef	MDEBUG
	malloc_leave("Vector::Vector");
#endif
}
void
Vector::SetSize(int	s)
{
#ifdef	MDEBUG
	malloc_enter("Vector::SetSize");
#endif
	double	*temp;
	int	i;

	if (size==0)
	{
		assert(v==NULL);
		v = new double[s];
		for(i=0;i<s;i++)
			v[i] = 0.0;
	}
	else
	{
		temp = new double [s];
		if (temp!=v)
		{
			for(i=0;i<s;i++)
				if (i<size)
					temp[i] = v[i];
				else
					temp[i] = 0.0;
			delete [] v;
			v = temp;
		}
	}
	size = s;
#ifdef	MDEBUG
	malloc_leave("Vector::SetSize");
#endif
}

const double&
Vector::operator[](int i) const
{
	static double	zero = 0.0;
	if (i>size)
	{
		cerr << "Error, access out of range in Vector::operator[] const\n";
		return(zero);
	}
	else
		return(v[i-1]);
}

double
Vector::dot(const Vector &b) const
{
	int i, max;
	double	ret=0.0;

	max = ( size > b.size ? size : b.size );
	for(i=1;i<=max;i++)
		ret += b[i]*(*this)[i];
	return(ret);
}

Vector
Vector::operator+(const Vector &b)
{
	int     i, max;
	Vector	ret;

	max = (size > b.size ? size : b.size);
	ret.SetSize(max);
	for(i=1;i<=max;i++)
		ret[i] = (*this)[i]+b[i];
	return(ret);
}

void
Vector::operator+=(const Vector &b)
{
	int	i;

	if (size < b.size)
		SetSize(b.size);
	for(i=1;i<=b.size;i++)
		v[i-1] += b[i];
}

void
Vector::operator-=(const Vector &b)
{
	int	i;

	if (size<b.size)
		SetSize(b.size);
	for(i=1;i<=b.size;i++)
		v[i-1] -= b[i];
}

Vector 
Vector::operator-(const Vector &b)
{
	int     i, max;
	Vector	ret;

	max = (size > b.size ? size : b.size);
	ret.SetSize(max);
	ret = (*this);
	for(i=1;i<=b.size;i++)
		ret[i] -= b[i];
	return(ret);
}

Vector
Vector::operator-()
{
	int     i;
	Vector	ret;

	ret.SetSize(size);
	for(i=1;i<=size;i++)
		ret[i] = -v[i-1];
	return(ret);
}

Vector operator*(double a, Vector& b)
{
	int     i, size;
	Vector	ret;

	size = b.Size();
	if (size==0)
		return(ret);
	ret[size] = 0.0;
	for(i=1;i<=size;i++)
		ret[i] = a*b[i];
	return(ret);
}
void Vector::Free()
{
	if (size>0)
		delete[] v;
	else
		assert(v==NULL);

	size = 0;
}
Vector::~Vector()
{
	Free();
}
void
Vector::Load(double *b, int s)
{
	int	i;

	if (size>0)
		delete [] v;

	SetSize(s);
	for(i=0;i<s;i++)
		v[i] = b[i];
}
Vector&
Vector::operator=(const Vector& V2)
{
#ifdef	MDEBUG
	malloc_enter("Vector::operator=");
#endif
	int	i;

	size = V2.size;

	if (v!=NULL)
		delete [] v;

	v = new double[size];
	for(i=0;i<size;i++)
		v[i] = V2.v[i];
#ifdef	MDEBUG
	malloc_leave("Vector::operator=");
#endif
	return(*this);
}
void
Vector::Print()
{
	int	i;

	cout << endl << "{";
	for(i=1;i<=size;i++)
		cout << v[i-1] << "\t";
	cout << "}" << endl;
}
double
Vector::operator*(Vector &b)
{
	double	ret;

	ret = dot(b);
	return(ret);
}
double
Vector::operator!()
{
	return(norm());
}

Vector
ReadVectorFromFile(istream& ifile) return ret
{
	int	eq;
	
	while(ifile.good() && !ifile.eof())
	{
		ifile >> eq;
		ifile >> ret[eq];
	}
	return	ret;
}
