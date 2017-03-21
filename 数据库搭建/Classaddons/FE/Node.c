#include	<assert.h>
#include	"Domain.h"
#include	"Node.h"

#ifdef	__GNUC__
#pragma implementation
#endif

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * File : $Id: Node.c,v 3.3 1993/05/25 21:31:08 tiller Exp $
*/

//Domain	*Node::current = NULL;

Node::Node(double X)
{
	pos.x = X;
	pos.y = pos.z = 0.0;
	number = -1;
}

Node::Node(double X, double Y)
{
	pos.x = X;
	pos.y = Y;
	pos.z = 0.0;
	number = -1;
}

Node::Node(double X, double Y, double Z)
{
	pos.x = X;
	pos.y = Y;
	pos.z = Z;
	number = -1;
}

Node::Node(const Coord3D& c)
{
	pos.x = c.x;
	pos.y = c.y;
	pos.z = c.z;
	number = -1;
}

#if	 0
void
Node::AddDOF(short idx)
{
	assert(current!=NULL);
	current->AddDOF(this, idx);
}
#endif

TestNode::TestNode(double cx,double cy,double cz,double dx,double dy,double dz)
	: Node(cx, cy, cz)
{
	par.x = dx;
	par.y = dy;
	par.z = dz;
}

Coord3D
TestNode::Par(int bn)
{
	return par;
}

StdNode::StdNode(double cx, double cy, double cz)
	: Node(cx,cy,cz)
{
}

extern "C"
{
	void	nodedinfo(int,double*,int);
}

Coord3D
StdNode::Par(int bn)
{
	double	dx[3];
	Coord3D	ret;

	nodedinfo(number, dx, bn);

	ret.x = dx[0];
	ret.y = dx[1];
	ret.z = dx[2];

	return ret;
}

