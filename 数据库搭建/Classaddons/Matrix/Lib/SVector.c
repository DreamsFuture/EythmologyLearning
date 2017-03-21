/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: SVector.c,v 1.2 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: SVector.c,v $
 * Revision 1.2  1993/06/10  13:59:27  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 1.2  1993/06/10  13:59:27  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 1.1  1993/05/11  15:42:00  tiller
 * Added support for memory leak traking...Everything looks good for this
 * versions.
 *
 * Revision 1.1  1993/05/11  15:42:00  tiller
 * Added support for memory leak traking...Everything looks good for this
 * versions.
 *
 * Revision 1.0  1993/03/08  19:54:05  tiller
 * Initial revision
 *
 */

#include	<SVector.h>
#include	<Vector.h>

void operator+=(SVector &sv, const Vector &v)
{
	int	i;

	for(i=1;i<=v.Size();i++)
		sv[i]+=v[i];
}

void operator-=(SVector &sv, const Vector &v)
{
	int	i;

	for(i=1;i<=v.Size();i++)
		sv[i]-=v[i];
}

void operator+=(Vector &v, const SVector &sv)
{
	int	i, row;
	double	val;

	for(i=1;i<=sv.Size();i++)
		if (sv.Get(i,row,val))
			v[row]+=val;
}

void operator-=(Vector &v, const SVector &sv)
{
	int	i, row;
	double	val;

	for(i=1;i<=sv.Size();i++)
		if (sv.Get(i,row,val))
			v[row]-=val;
}