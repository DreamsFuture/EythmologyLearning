#include	<stdlib.h>
#include	<math.h>
#include	<iostream.h>
#include	<assert.h>
#include	"Trans.h"
#include	"Node.h"
#include	"Element.h"
#include	"Basis.h"
#include	"GaussContext.h"
#include	"GPInfo.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Trans.c,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#ifdef	USE_MCHUNK
#include	"MemChunk.h"
#endif
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

#define	EPS	(.0001)

// This function assumes that jac is defined and calculates ijac and dj

void
Transformation::Calc()
{
	switch(ndim)
	{
		case	1:
			dj = jac[0][0];
			ijac[0][0] = 1.0/jac[0][0];
			break;
		case	2:
			dj = jac[0][0]*jac[1][1]-jac[1][0]*jac[0][1];
			ijac[0][0] = jac[1][1]/dj;
			ijac[0][1] = -jac[0][1]/dj;
			ijac[1][0] = -jac[1][0]/dj;
			ijac[1][1] = jac[0][0]/dj;
			break;
		case	3:
			dj = jac[0][0]*jac[1][1]*jac[2][2]-jac[0][1]*jac[1][0]*jac[2][2]-
			     jac[0][0]*jac[1][2]*jac[2][1]+jac[0][2]*jac[1][0]*jac[2][1]+
			     jac[0][1]*jac[1][2]*jac[2][0]-jac[0][2]*jac[1][1]*jac[2][0];
			ijac[0][0] = (jac[1][1]*jac[2][2]-jac[1][2]*jac[2][1])/dj;
			ijac[0][1] = (jac[0][2]*jac[2][1]-jac[0][1]*jac[2][2])/dj;
			ijac[0][2] = (jac[0][1]*jac[1][2]-jac[0][2]*jac[1][1])/dj;
			ijac[1][0] = (jac[1][2]*jac[2][0]-jac[1][0]*jac[2][2])/dj;
			ijac[1][1] = (jac[0][0]*jac[2][2]-jac[0][2]*jac[2][0])/dj;
			ijac[1][2] = (jac[0][2]*jac[1][0]-jac[0][0]*jac[1][2])/dj;
			ijac[2][0] = (jac[1][0]*jac[2][1]-jac[1][1]*jac[2][0])/dj;
			ijac[2][1] = (jac[0][1]*jac[2][0]-jac[0][0]*jac[2][1])/dj;
			ijac[2][2] = (jac[0][0]*jac[1][1]-jac[0][1]*jac[1][0])/dj;
			break;
	}
}


// This subroutine can assume that jac,ijac,dj and pj are all defined
// and it should calculate pij and pdj

void
Transformation::ParCalc()
{
	switch(ndim)
	{
		case	1:
			pdj = pj[0][0];
			pji[0][0] = -pj[0][0]/(jac[0][0]*jac[0][0]);
			break;
		case	2:
			pdj = jac[1][1]*pj[0][0]-jac[1][0]*pj[0][1]-
			      jac[0][1]*pj[1][0]+jac[0][0]*pj[1][1];
			pji[0][0] = -(ijac[0][0]*pj[0][0]*ijac[0][0]+
			              ijac[0][1]*pj[1][0]*ijac[0][0]+
			              ijac[0][0]*pj[0][1]*ijac[1][0]+
			              ijac[0][1]*pj[1][1]*ijac[1][0]);
			pji[0][1] = -(ijac[0][0]*pj[0][0]*ijac[0][1]+
			              ijac[0][1]*pj[1][0]*ijac[0][1]+
			              ijac[0][0]*pj[0][1]*ijac[1][1]+
			              ijac[0][1]*pj[1][1]*ijac[1][1]);
			pji[1][0] = -(ijac[1][0]*pj[0][0]*ijac[0][0]+
			              ijac[1][1]*pj[1][0]*ijac[0][0]+
			              ijac[1][0]*pj[0][1]*ijac[1][0]+
			              ijac[1][1]*pj[1][1]*ijac[1][0]);
			pji[1][1] = -(ijac[1][0]*pj[0][0]*ijac[0][1]+
			              ijac[1][1]*pj[1][0]*ijac[0][1]+
			              ijac[1][0]*pj[0][1]*ijac[1][1]+
			              ijac[1][1]*pj[1][1]*ijac[1][1]);
			break;
		case	3:
			pdj = -jac[1][2]*jac[2][1]*pj[0][0]+jac[1][1]*jac[2][2]*pj[0][0]+
			       jac[1][2]*jac[2][0]*pj[0][1]-jac[1][0]*jac[2][2]*pj[0][1]-
			       jac[1][1]*jac[2][0]*pj[0][2]+jac[1][0]*jac[2][1]*pj[0][2]+
			       jac[0][2]*jac[2][1]*pj[1][0]-jac[0][1]*jac[2][2]*pj[1][0]-
			       jac[0][2]*jac[2][0]*pj[1][1]+jac[0][0]*jac[2][2]*pj[1][1]+
			       jac[0][1]*jac[2][0]*pj[1][2]-jac[0][0]*jac[2][1]*pj[1][2]-
			       jac[0][2]*jac[1][1]*pj[2][0]+jac[0][1]*jac[1][2]*pj[2][0]+
			       jac[0][2]*jac[1][0]*pj[2][1]-jac[0][0]*jac[1][2]*pj[2][1]-
			       jac[0][1]*jac[1][0]*pj[2][2]+jac[0][0]*jac[1][1]*pj[2][2];

			pji[0][0] = -(ijac[0][0]*pj[0][0]*ijac[0][0]+
			              ijac[0][1]*pj[1][0]*ijac[0][0]+
			              ijac[0][2]*pj[2][0]*ijac[0][0]+
			              ijac[0][0]*pj[0][1]*ijac[1][0]+
			              ijac[0][1]*pj[1][1]*ijac[1][0]+
			              ijac[0][2]*pj[2][1]*ijac[1][0]+
			              ijac[0][0]*pj[0][2]*ijac[2][0]+
			              ijac[0][1]*pj[1][2]*ijac[2][0]+
			              ijac[0][2]*pj[2][2]*ijac[2][0]);
			pji[1][0] = -(ijac[1][0]*pj[0][0]*ijac[0][0]+
			              ijac[1][1]*pj[1][0]*ijac[0][0]+
			              ijac[1][2]*pj[2][0]*ijac[0][0]+
			              ijac[1][0]*pj[0][1]*ijac[1][0]+
			              ijac[1][1]*pj[1][1]*ijac[1][0]+
			              ijac[1][2]*pj[2][1]*ijac[1][0]+
			              ijac[1][0]*pj[0][2]*ijac[2][0]+
			              ijac[1][1]*pj[1][2]*ijac[2][0]+
			              ijac[1][2]*pj[2][2]*ijac[2][0]);
			pji[2][0] = -(ijac[2][0]*pj[0][0]*ijac[0][0]+
			              ijac[2][1]*pj[1][0]*ijac[0][0]+
			              ijac[2][2]*pj[2][0]*ijac[0][0]+
			              ijac[2][0]*pj[0][1]*ijac[1][0]+
			              ijac[2][1]*pj[1][1]*ijac[1][0]+
			              ijac[2][2]*pj[2][1]*ijac[1][0]+
			              ijac[2][0]*pj[0][2]*ijac[2][0]+
			              ijac[2][1]*pj[1][2]*ijac[2][0]+
			              ijac[2][2]*pj[2][2]*ijac[2][0]);
			pji[0][1] = -(ijac[0][0]*pj[0][0]*ijac[0][1]+
			              ijac[0][1]*pj[1][0]*ijac[0][1]+
			              ijac[0][2]*pj[2][0]*ijac[0][1]+
			              ijac[0][0]*pj[0][1]*ijac[1][1]+
			              ijac[0][1]*pj[1][1]*ijac[1][1]+
			              ijac[0][2]*pj[2][1]*ijac[1][1]+
			              ijac[0][0]*pj[0][2]*ijac[2][1]+
			              ijac[0][1]*pj[1][2]*ijac[2][1]+
			              ijac[0][2]*pj[2][2]*ijac[2][1]);
			pji[1][1] = -(ijac[1][0]*pj[0][0]*ijac[0][1]+
			              ijac[1][1]*pj[1][0]*ijac[0][1]+
			              ijac[1][2]*pj[2][0]*ijac[0][1]+
			              ijac[1][0]*pj[0][1]*ijac[1][1]+
			              ijac[1][1]*pj[1][1]*ijac[1][1]+
			              ijac[1][2]*pj[2][1]*ijac[1][1]+
			              ijac[1][0]*pj[0][2]*ijac[2][1]+
			              ijac[1][1]*pj[1][2]*ijac[2][1]+
			              ijac[1][2]*pj[2][2]*ijac[2][1]);
			pji[2][1] = -(ijac[2][0]*pj[0][0]*ijac[0][1]+
			              ijac[2][1]*pj[1][0]*ijac[0][1]+
			              ijac[2][2]*pj[2][0]*ijac[0][1]+
			              ijac[2][0]*pj[0][1]*ijac[1][1]+
			              ijac[2][1]*pj[1][1]*ijac[1][1]+
			              ijac[2][2]*pj[2][1]*ijac[1][1]+
			              ijac[2][0]*pj[0][2]*ijac[2][1]+
			              ijac[2][1]*pj[1][2]*ijac[2][1]+
			              ijac[2][2]*pj[2][2]*ijac[2][1]);
			pji[0][2] = -(ijac[0][0]*pj[0][0]*ijac[0][2]+
			              ijac[0][1]*pj[1][0]*ijac[0][2]+
			              ijac[0][2]*pj[2][0]*ijac[0][2]+
			              ijac[0][0]*pj[0][1]*ijac[1][2]+
			              ijac[0][1]*pj[1][1]*ijac[1][2]+
			              ijac[0][2]*pj[2][1]*ijac[1][2]+
			              ijac[0][0]*pj[0][2]*ijac[2][2]+
			              ijac[0][1]*pj[1][2]*ijac[2][2]+
			              ijac[0][2]*pj[2][2]*ijac[2][2]);
			pji[1][2] = -(ijac[1][0]*pj[0][0]*ijac[0][2]+
			              ijac[1][1]*pj[1][0]*ijac[0][2]+
			              ijac[1][2]*pj[2][0]*ijac[0][2]+
			              ijac[1][0]*pj[0][1]*ijac[1][2]+
			              ijac[1][1]*pj[1][1]*ijac[1][2]+
			              ijac[1][2]*pj[2][1]*ijac[1][2]+
			              ijac[1][0]*pj[0][2]*ijac[2][2]+
			              ijac[1][1]*pj[1][2]*ijac[2][2]+
			              ijac[1][2]*pj[2][2]*ijac[2][2]);
			pji[2][2] = -(ijac[2][0]*pj[0][0]*ijac[0][2]+
			              ijac[2][1]*pj[1][0]*ijac[0][2]+
			              ijac[2][2]*pj[2][0]*ijac[0][2]+
			              ijac[2][0]*pj[0][1]*ijac[1][2]+
			              ijac[2][1]*pj[1][1]*ijac[1][2]+
			              ijac[2][2]*pj[2][1]*ijac[1][2]+
			              ijac[2][0]*pj[0][2]*ijac[2][2]+
			              ijac[2][1]*pj[1][2]*ijac[2][2]+
			              ijac[2][2]*pj[2][2]*ijac[2][2]);
			break;
	}
}

UnitTrans::UnitTrans(int nd)
{
	ndim = nd;
	jac[0][1] = jac[1][0] = jac[0][2] = 0.0;
	jac[2][0] = jac[1][2] = jac[2][1] = 0.0;
	jac[0][0] = jac[1][1] = jac[2][2] = 1.0;
	Calc();
}

ostream&
operator<<(ostream &o, Transformation &t)
{
	int	i, j;
	o << "Transformation Dimension : " << t.NumDim() << endl;
	for(i=1;i<=t.NumDim();i++)
	{
		o << "| ";
		for(j=1;j<=t.NumDim();j++)
			o << t.J(i,j) << "   ";
		o << " |" << endl;
	}
	for(i=1;i<=t.NumDim();i++)
	{
		o << "| ";
		for(j=1;j<=t.NumDim();j++)
			o << t.Ji(i,j) << "   ";
		o << " |" << endl;
	}
	o << "|J| = " << t.detJ() << endl;
	o << endl;
	for(i=1;i<=t.NumDim();i++)
	{
		o << "| ";
		for(j=1;j<=t.NumDim();j++)
			o << t.ParJ(i,j) << "   ";
		o << " |" << endl;
	}
	for(i=1;i<=t.NumDim();i++)
	{
		o << "| ";
		for(j=1;j<=t.NumDim();j++)
			o << t.ParJi(i,j) << "   ";
		o << " |" << endl;
	}
	o << "d|J| = " << t.PardetJ() << endl;
	return o;
}

ParamTran::ParamTran(const BasisFunctions& b)
	: bf(b)
{
	ndim = bf.NumDim();
	nodes = NULL;
}

void 
ParamTran::Attach(const FixedSet<Node *>& n)
{
	nodes = &n;
	if (nodes->Size()!=bf.NumNodes())
	{
		cerr << "Error in ParamTran::ParamTran" << endl;
		cerr << "  Element has " << nodes->Size() << " nodes, but ";
		cerr << "there are only " << bf.NumNodes() << " basis functions" << endl;
		assert(nodes->Size()==bf.NumNodes());
	}
}

void 
ParamTran::Attach(const Element& e)
{
	nodes = &e.nodes();
	if (nodes->Size()!=bf.NumNodes())
	{
		cerr << "Error in ParamTran::ParamTran" << endl;
		cerr << "  Element has " << nodes->Size() << " nodes, but ";
		cerr << "there are only " << bf.NumNodes() << " basis functions" << endl;
		assert(nodes->Size()==bf.NumNodes());
	}
}

void
ParamTran::At(const Coord3D& c)
{
#ifdef	MDEBUG
	malloc_enter("ParamTran::At");
#endif
	GaussContext	*cp;
	int	i, j, k;
	double	x[3];
	Indexer<Node *>	nl(*nodes);

	if (nodes==NULL)
	{
		cerr << "Error in ParamTran::At" << endl;
		cerr << "ParamTran never attached to anything!" << endl;
		assert(nodes!=NULL);
	}
	for(i=0;i<ndim;i++)
		for(j=0;j<ndim;j++)
			jac[i][j] = 0.0;
	cp = bf.Eval(c);
	for(k=1;k<=bf.NumNodes();k++)
	{
		x[0] = nl[k]->Pos().x;
		x[1] = nl[k]->Pos().y;
		x[2] = nl[k]->Pos().z;
		for(i=1;i<=ndim;i++)
			for(j=1;j<=ndim;j++)
				jac[i-1][j-1] += cp->B(j,k)*x[i-1];
	}
#ifdef	GC_IS_GC
	cp->unref();
#else
	delete cp;
#endif
	Calc();
#ifdef	MDEBUG
	malloc_leave("ParamTran::At");
#endif
}

int
ParamTran::ParAt(const Coord3D& c, int bn)
{
#ifdef	MDEBUG
	malloc_enter("ParamTran::ParAt");
#endif
	At(c);
	GaussContext	*cp;
	int	i, j, k;
	double	x[3];
	Indexer<Node *>	nl(*nodes);

	if (nodes==NULL)
	{
		cerr << "Error in ParamTran::At" << endl;
		cerr << "ParamTran never attached to anything!" << endl;
		assert(nodes!=NULL);
	}
	for(i=0;i<ndim;i++)
		for(j=0;j<ndim;j++)
			pj[i][j] = 0.0;
	cp = bf.Eval(c);
	for(k=1;k<=bf.NumNodes();k++)
	{
		x[0] = nl[k]->Par(bn).x;
		x[1] = nl[k]->Par(bn).y;
		x[2] = nl[k]->Par(bn).z;
		for(i=1;i<=ndim;i++)
			for(j=1;j<=ndim;j++)
				pj[i-1][j-1] += cp->B(j,k)*x[i-1];
	}
#ifdef	GC_IS_GC
	cp->unref();
#else
	delete cp;
#endif

#ifdef	MDEBUG
	malloc_leave("ParamTran::ParAt");
#endif
	ParCalc();
	if (fabs(pdj) < EPS)
		return 0;
	return 1;
}