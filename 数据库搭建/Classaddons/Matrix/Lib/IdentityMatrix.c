/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: IdentityMatrix.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: IdentityMatrix.c,v $
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
#include	<IdentityMatrix.h>

#ifdef __GNUC__
#pragma implementation
#endif

void
IdentityMatrix::Print()
{
	if (size!=-1)
		printf("{ %dx%d Identity matrix }\n", size, size);
	else
		printf("{ Identity matrix }\n");
}

int
IdentityMatrix::isSym() const { return 1; }

Vector		IdentityMatrix::Solve(const Vector &v)
{
	if (v.Size()<=size || size==-1)
		return	v;

	fprintf(stderr, "Solving %dx%d IdentityMatrix using %d length vector\n",
		size,size,v.Size());
	exit(1);
}

int
IdentityMatrix::BandWidth() const
{
	return	1;
}

Vector
IdentityMatrix::Pre(const Vector&v) const
{
	if (v.Size()<=size || size==-1)
		return	v;

	fprintf(stderr, "Multiplying %dx%d IdentityMatrix by %d length vector\n",
		size,size,v.Size());
	exit(1);
}
Vector
IdentityMatrix::Post(const Vector&v)	const
{
	if (v.Size()<=size || size==-1)
		return	v;

	fprintf(stderr, "Multiplying %dx%d IdentityMatrix by %d length vector\n",
		size,size,v.Size());
	exit(1);
}

