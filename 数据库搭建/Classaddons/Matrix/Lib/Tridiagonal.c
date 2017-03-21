/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Tridiagonal.c,v 1.5 1993/06/10 21:01:04 tiller Exp tiller $
 * ChangeLog:
 *	$Log: Tridiagonal.c,v $
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


#include	<Tridiagonal.h>
#ifdef __GNUC__
#pragma implementation
#endif

Tridiagonal::Tridiagonal(int issym)
{
	diag = new Vector;
	sup = new Vector;
	if (issym)
		sub = sup;
	else
		sub = new Vector;
}

Tridiagonal::Tridiagonal(const Tridiagonal &T)
{
	if (T.isSym())
	{
		diag = new Vector;
		sup = new Vector;
		sub = sup;
		*diag = *T.diag;
		*sup = *T.sup;
	}
	else
	{
		diag = new Vector;
		sup = new Vector;
		sub = new Vector;
		*diag = *T.diag;
		*sup = *T.sup;
		*sub = *T.sub;
	}
}
Tridiagonal::Tridiagonal(const Vector& d, const Vector& od)
{
	diag = new Vector;
	sup = new Vector;
	sub = sup;

	*diag = d;
	*sup = od;
}

Tridiagonal::Tridiagonal(const Vector& d, const Vector& sp, const Vector& sb)
{
	diag = new Vector;
	sup = new Vector;
	sub = new Vector;
	
	*diag = d;
	*sup = sp;
	*sub = sb;
}

double& Tridiagonal::Sub(int i) { return(sub->operator[](i)); }
double& Tridiagonal::Sup(int i) { return(sup->operator[](i)); }
double& Tridiagonal::Diag(int i) { return(diag->operator[](i)); }

Vector& Tridiagonal::Sub() { return(*sub); }
Vector& Tridiagonal::Sup() { return(*sup); }
Vector& Tridiagonal::Diag() { return(*diag); }

Vector& Tridiagonal::Sub() const { return(*sub); }
Vector& Tridiagonal::Sup() const { return(*sup); }
Vector& Tridiagonal::Diag() const { return(*diag); }

//Vector Tridiagonal::CopySub() const { return(*sub); }
//Vector Tridiagonal::CopySup() const { return(*sup); }
//Vector Tridiagonal::CopyDiag() const { return(*diag); }

int
Tridiagonal::Length() const
{
#if	1
	return(diag->Size());
#else
	int	s1 = diag->Size(),
	    s2 = sup->Size(),
	    s3 = sub->Size();

	if (s3 >= s2 && s3 >= s1)
		return(s3+1);
	if (s2 >= s3 && s2 >= s1)
		return(s2+1);
	if (s1 >= s2 && s1 >= s3)
		return(s1);

	// Should NEVER get here
	fprintf(stderr, "Error in Tridiagonal::Length()\n");
	return(s1);
#endif
}

void
Tridiagonal::Print()
{
	int	i, s = Length();

	printf("Row 1 = %g	%g	%g\n", 0.0, Diag(1), Sup(1));
	for(i=2;i<s;i++)
		printf("Row %d = %g	%g	%g\n", i, Sub(i-1), Diag(i), Sup(i));
	printf("Row %d = %g	%g	%g\n", s, Sub(s-1), Diag(s), 0.0);
}

void
Tridiagonal::Zero()
{
	delete diag;
	delete sup;
	if (sup!=sub)
		delete sub;

}

int
Tridiagonal::isSym() const { return(sup==sub); }

Vector
Solve(const Vector &)
{
	fprintf(stderr,
		"Sorry, Tridiagonal::Solve(const Vector &) not implemented\n");
	return(Vector());
}

int
Tridiagonal::BandWidth() const
{
	return(2);
}

int
Tridiagonal::NumberOfCols() const
{
	return(Length());
}

int
Tridiagonal::NumberOfRows() const
{
	return(Length());
}

Vector
Tridiagonal::Post(const Vector &b) const
{
	int	i, s = Length();
	Vector	res;

	if (b.Size() > s)
	{
		fprintf(stderr, "Incompatible Size in Tridiagonal::Post\n");
		return(res);
	}

	for(i=1;i<=s;i++)
	{
		res[i] = (*diag)[i]*b[i];
		if (i<s)
			res[i] += (*sup)[i]*b[i+1];
		if (i>1)
			res[i] += (*sub)[i-1]*b[i-1];
	}
	return(res);
}

Vector
Tridiagonal::Pre(const Vector &b) const
{
	int	i, s = Length();
	Vector	res;

	if (b.Size() > s)
	{
		fprintf(stderr, "Incompatible Size in Tridiagonal::Pre\n");
		return(res);
	}

	for(i=1;i<=s;i++)
	{
		res[i] = (*diag)[i]*b[i];
		if (i<s)
			res[i] += (*sup)[i]*b[i];
		if (i>1)
			res[i] += (*sub)[i-1]*b[i];
	}
	return(res);
}

long
Tridiagonal::Memory() const
{
	return(Length()*3*sizeof(double));
}

Tridiagonal::~Tridiagonal()
{
#if	0
	int	issym = (sup==sub);

	delete diag;
	delete sup;
	if (sup!=sub)
		delete sub;

	diag = new Vector;
	sup = new Vector;
	if (issym)
		sub = sup;
	else
		sub = new Vector;
#endif
}
