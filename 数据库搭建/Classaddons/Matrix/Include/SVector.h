/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: SVector.h,v 1.5 1993/06/10 21:00:33 tiller Exp tiller $
 * ChangeLog:
 *	$Log: SVector.h,v $
 * Revision 1.5  1993/06/10  21:00:33  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 1.4  1993/05/26  15:45:56  tiller
 * Modified to handle const Vectors in the operator versions of
 * Solve, PreMult and PostMult.
 *
 * Revision 1.4  1993/05/26  15:45:56  tiller
 * Modified to handle const Vectors in the operator versions of
 * Solve, PreMult and PostMult.
 *
 * Revision 1.3  1993/05/10  15:11:15  tiller
 * *** empty log message ***
 *
 * Revision 1.3  1993/05/10  15:11:15  tiller
 * *** empty log message ***
 *
 * Revision 1.2  1993/02/20  18:59:28  tiller
 * Added RCS header info
 *
 * Revision 1.2  1993/02/20  18:59:28  tiller
 * Added RCS header info
 *
 */

#ifndef	_SVECTOR_H_
#define	_SVECTOR_H_

#ifdef	USE_SET
#include	<Set/AssocSet.h>
#else
#include	<Gen/AArray.h>
#endif

#ifdef	USE_SET
class	SVector : public AssocSet<int,double>
#else
class	SVector : public AArray<int,double>
#endif
{
	public:
#ifdef	USE_SET
		SVector() : AssocSet<int,double>() { SetDef(0.0); }
		SVector(const SVector& s) : AssocSet<int,double>(s) { }
#else
		SVector() : AArray<int,double>() { SetDef(0.0); }
		SVector(const SVector& s) : AArray<int,double>(s) { }
#endif
};

class	Vector;
void operator+=(SVector &sv, const Vector &v);
void operator+=(Vector &v, const SVector &sv);
void operator-=(SVector &sv, const Vector &v);
void operator-=(Vector &v, const SVector &sv);

#endif
