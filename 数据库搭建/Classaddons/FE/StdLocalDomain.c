#include	<string.h>
#include	<stdlib.h>
#include	<BIMatrix.h>
#include	<assert.h>
#include	"StdLocalDomain.h"
#include	"Element.h"
#include	"Design.h"
#include	"Objective.h"
#include	"Essential.h"
#include	"Const.h"
#include	"Node.h"
#include	"Integrator.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: StdLocalDomain.c,v 1.1 1993/06/13 18:45:47 tiller Exp tiller $
 */

#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

NoObjective		StdLocalDomain::no_obj;

StdLocalDomain::StdLocalDomain(Integrator &in, Objective *o, DesignSpec *sp)
	: LocalDomain(in),
	  obj( *(o==NULL ? &no_obj : o) ),
	  spec( *(sp==NULL ? &desvec : sp) )
{
	prev = time = 0.0;
	numDOFS = 0;
}

int
StdLocalDomain::Solve(int senses)
{
	cerr << "StdLocalDomain::Solve needs to be implemented!" << endl;
	return 0;
}

void
StdLocalDomain::NodalReport(const Vector &, char *, ostream &)
{
	cerr << "Warning, StdLocalDomain::NodalReport not implemented!" << endl;
}

void
StdLocalDomain::SetTime(double t)
{
	prev = time;
	time = t;
}

int
StdLocalDomain::NumNodes() const
{
	return	nodes.Size();
}

int
StdLocalDomain::NumEqs() const
{
	return	numDOFS;
}

int
StdLocalDomain::NumNodalDOFS(int n) const
{
	return nodes[n]->Map().Size();
}

int
StdLocalDomain::NodalDOF(int n, int idx) const
{
	Node	*node = nodes[n];

	assert(idx<=node->Map().Size() && idx >=1);
	return node->Map().GetKey(idx);
}

long
StdLocalDomain::EqNum(int n,short dof) const
{
	return nodes[n]->Map()[dof];
}

void
StdLocalDomain::AddNode(Node *node)
{
	nodes+=node;
	node->SetNumber(nodes.Size());
}

void
StdLocalDomain::AddElem(Element *elem)
{
	elems+=elem;
	elem->Register(*this);
}

void
StdLocalDomain::AddBC(Essential *bc)
{
	bcs+=bc;
}

void
StdLocalDomain::AddDOF(Node *node, short type)
{
	if (!node->Map().HasKey(type))
	{
		numDOFS++;
		node->Map()[type] = numDOFS;
	}
}

const FixedSet<Node *>&
StdLocalDomain::NodeSet() const
{
	return nodes;
}

const FixedSet<Element *>&
StdLocalDomain::ElemSet() const
{
	return elems;
}

const FixedSet<Essential *>&
StdLocalDomain::BCSSet() const
{
	return bcs;
}

DesignSpec&
StdLocalDomain::GetSpec()
{
	return	spec;
}

Objective&
StdLocalDomain::GetObj()
{
	return	obj;
}

double
StdLocalDomain::Time() const
{
	return time;
}

double
StdLocalDomain::GetSol(int i) const
{
	return last[i];
}

void
StdLocalDomain::SetSol(int i, double v)
{
	last[i] = v;
}

void
StdLocalDomain::GetSol(Vector &v) const
{
	v = last;
}

void
StdLocalDomain::NodalCoords(int n, double &x, double &y, double &z)
{
	x = nodes[n]->Pos().x;
	y = nodes[n]->Pos().y;
	z = nodes[n]->Pos().z;
}

StdLocalDomain::~StdLocalDomain()
{
}




