/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Matrix.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: Matrix.c,v $
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
#include	"Matrix.h"
#include	<stddef.h>

#ifdef __GNUC__
#pragma implementation
#endif

void
SymEquiv::Print()
{
	printf("Symmetric product of\n");
	A.Print();
}

void
SymEquiv::Zero()
{
	A.Zero();
}

Vector
SymEquiv::Solve(const Vector&)
{
	fprintf(stderr, "Vector SymEquiv::Solve Not implemented!\n");
	exit(1);
}

int
SymEquiv::BandWidth() const 
{
	return A.BandWidth();	// I don't think this can be true
}

int
SymEquiv::NumberOfRows() const
{
	return A.NumberOfRows();
}

int
SymEquiv::NumberOfCols() const
{
	return A.NumberOfCols();
}

Vector
SymEquiv::Pre(const Vector& v) const
{
	return A.Post(A.Pre(v));
}

Vector
SymEquiv::Post(const Vector& v) const
{
	return A.Pre(A.Post(v));
}

long
SymEquiv::Memory() const 
{
	return A.Memory();
}