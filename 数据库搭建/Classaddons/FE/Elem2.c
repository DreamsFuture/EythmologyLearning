#include	"Domain.h"
#include	"Elem2.h"
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
 * $Id: Elem2.c,v 3.4 1993/05/25 21:31:08 tiller Exp $
 */

const	BF21 IsoConvElem21::e1b;
ParamTran	IsoConvElem21::e1t(IsoConvElem21::e1b);
const GenGauss	IsoConvElem21::e1g(Coord3D(-1,0,0),Coord3D(1,0,0),2);

IsoConvElem21::IsoConvElem21(const MatProp &h, const MatProp &T, const Const& d,
			 Node *n1, Node *n2)
	: ConvElem(e1b, e1t, d, e1g, h, T)
{
	nlist+=n1;
	nlist+=n2;
#if	0
	n1->AddDOF(dof);
	n2->AddDOF(dof);
#endif
}

void
IsoConvElem21::Register(LocalDomain& d)
{
	int	i;

	foreach(i,nlist)
		d.AddDOF(nlist[i],dof);
}

void
IsoConvElem21::Stiff(BIMatrix &b, LocalDomain &d)
{
	e1t.Attach(*this);
	ConvElem::Stiff(b, d);
}

void
IsoConvElem21::TanStiff(BIMatrix &b, LocalDomain &d)
{
	e1t.Attach(*this);
	ConvElem::TanStiff(b, d);
}

void
IsoConvElem21::ParStiff(BIMatrix &b, LocalDomain &d, int bn)
{
	e1t.Attach(*this);
	ConvElem::ParStiff(b, d, bn);
}

