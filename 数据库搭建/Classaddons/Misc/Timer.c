/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

#ifdef	__GNUC__
#pragma implementation
#endif

#include	"Timer.h"

void 
Timer::SetTimer()
{
	if (gettimeofday(&start, (struct timezone *) NULL)==-1)
		fprintf(stderr, "Error in Timer::Set (gettimeofday)!\n");
}
