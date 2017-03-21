#include	"Domain.h"
#include	"Elem1.h"
#include	"Node.h"
#include	"Const.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Elem1.c,v 3.4 1993/05/25 21:31:08 tiller Exp $
 */

const	BF42 IsoCondElem42::e1b;
ParamTran	IsoCondElem42::e1t(IsoCondElem42::e1b);
const GenGauss	IsoCondElem42::e1g(Coord3D(-1,-1,0),Coord3D(1,1,0),2,2);

IsoCondElem42::IsoCondElem42(const MatProp &m, const Const& d,
			 Node *n1, Node *n2, Node *n3, Node *n4)
	: CondElem(e1b, e1t, d, e1g, m)
{
	nlist+=n1;
	nlist+=n2;
	nlist+=n3;
	nlist+=n4;
#if	0
	n1->AddDOF(dof);
	n2->AddDOF(dof);
	n3->AddDOF(dof);
	n4->AddDOF(dof);
#endif
}

void
IsoCondElem42::Register(LocalDomain& d)
{
	int	i;

	foreach(i,nlist)
		d.AddDOF(nlist[i],dof);
}

void
IsoCondElem42::Stiff(BIMatrix &b, LocalDomain &d)
{
	e1t.Attach(*this);
	CondElem::Stiff(b, d);
}

void
IsoCondElem42::TanStiff(BIMatrix &b, LocalDomain &d)
{
	e1t.Attach(*this);
	CondElem::TanStiff(b, d);
}

void
IsoCondElem42::ParStiff(BIMatrix &b, LocalDomain &d, int bn)
{
	e1t.Attach(*this);
	CondElem::ParStiff(b, d, bn);
}

