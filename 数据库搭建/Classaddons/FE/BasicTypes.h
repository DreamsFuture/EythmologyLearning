#ifndef	_BASIC_TYPES_H_
#define	_BASIC_TYPES_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: BasicTypes.h,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#ifdef	__GNUC__
#pragma interface
#endif

#include	"Const.h"

class	Displacement
{
	public:
		static	const	Const	XDisp;
		static	const	Const	YDisp;
		static	const	Const	ZDisp;
};

class	Misc
{
	public:
		static	const	Const	Temp;
};


#endif
