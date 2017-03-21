/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * File : $Id: Element.c,v 3.4 1993/05/25 21:31:08 tiller Exp $
*/

#include	<assert.h>
#include	<stdlib.h>
#include	"Element.h"
#include	"Domain.h"
#include	"Node.h"

const FixedSet<Node *>&
Element::nodes() const
{
	return nlist;
}

long
Element::EqNum(int n, short idx)
{
	Node	*node = nlist[n];
	long	eq;

	eq = node->dof(idx);
	return eq;
}
