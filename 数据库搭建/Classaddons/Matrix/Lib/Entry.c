/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Entry.c,v 1.5 1993/06/10 13:59:27 tiller Exp $
 * ChangeLog:
 *	$Log: Entry.c,v $
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


#include	<Entry.h>

#ifdef __GNUC__
#pragma implementation
#endif

AllocGCImplement(Entry,200);

Entry::Entry(int argi, int argj, double argv)
{
	i = argi;
	j = argj;
	v = argv;
}

Entry::Entry(const Entry& e)
{
	i = e.i;
	j = e.j;
	v = e.v;
}

Entry::~Entry()
{
	fprintf(stderr, "~Entry called!\n");
}
