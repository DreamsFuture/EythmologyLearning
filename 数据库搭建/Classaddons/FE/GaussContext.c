#include	<stdlib.h>
#include	<math.h>
#include	<iostream.h>
#include	<assert.h>

#include	"GaussContext.h"
#include	"Node.h"
#include	"Element.h"
#include	"Trans.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: GaussContext.c,v 3.4 1993/06/10 20:59:29 tiller Exp tiller $
 */

#ifdef	USE_MCHUNK
#include	"MemChunk.h"
#endif
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

#ifdef	GC_IS_GC
AllocGCImplement(GaussContext,50);
#endif

#ifdef	USE_MCHUNK
GaussContext::GaussContext(int nn, int nd, const Coord3D& c)
	: chunk(nn*(nd+1))
#else
GaussContext::GaussContext(int nn, int nd, const Coord3D& c)
#endif
{
	nnodes = nn;
	ndim = nd;
#ifdef	USE_MCHUNK
//	chunk = new DoubleChunk(nnodes*(ndim+1));
#endif
	p = c;
	Allocate();
}

#ifdef	USE_MCHUNK	
GaussContext::GaussContext(const GaussContext& c)
	: chunk(c.nnodes*(c.ndim+1))
#else
GaussContext::GaussContext(const GaussContext& c)
#endif
{
	nnodes = c.nnodes;
	ndim = c.ndim;
#ifdef	USE_MCHUNK
//	chunk = new DoubleChunk(nnodes*(ndim+1));
#endif
	p = c.p;
	int	i, j;

	Allocate();
	for(i=1;i<=nnodes;i++)
	{
		N(i) = c.N(i);
		for(j=1;j<=ndim;j++)
			B(j,i) = c.B(j,i);
	}
}

void
GaussContext::Allocate()
{
#ifdef	MDEBUG
	malloc_enter("GaussContext::Allocate");
#endif

#ifdef	USE_MCHUNK
	n = chunk.Vec(nnodes*(ndim+1));
	b = NULL;
#else
	n = new double[nnodes*(ndim+1)]; // Allocate N and B
	b = new double*[ndim];
#endif
	dj = 1.0;
	int	i;

#ifndef	USE_MCHUNK
	for(i=1;i<=ndim;i++)
		b[i-1] = n+nnodes*i;
#endif

#ifdef	MDEBUG
	malloc_leave("G aussContext::Allocate");
#endif
}

void
GaussContext::DeAllocate()
{
#ifdef	USE_MCHUNK
//	delete chunk;
#else
	delete[] b;
	delete[] n;
#endif
}

void
GaussContext::Transform(Transformation& t)
{
	if (t.NumDim()!=NumDim())
	{
		cerr << "Error in GaussContext::Transform" << endl;
		cerr << "Transforming a GaussContext with incompatible dimensions!" << endl;
		cerr << "Transformation is of " << t.NumDim() << " Dimensions" << endl;
		cerr << "GaussContext is of " << NumDim() << " Dimensions" << endl;
		assert(t.NumDim()==NumDim());
	}
		
	int	i, j, k;
	GaussContext	c(*this);			// Copy current context

	for(i=1;i<=nnodes;i++)
		for(j=1;j<=ndim;j++)
			B(j,i) = 0.0;		// Zero terms

	t.At(p);					// Get Transformation at p
	for(i=1;i<=nnodes;i++)		// Matrix Multiply
		for(j=1;j<=ndim;j++)
			for(k=1;k<=ndim;k++)
				B(j,i)+=c.B(k,i)*t.Ji(k,j);
//	cout << "dj = " << c.detJ() << " * " << t.detJ() << endl;
	dj = c.detJ()*t.detJ();
}

int
GaussContext::ParTransform(Transformation& t, int bn)
{
	if (t.NumDim()!=NumDim())
	{
		cerr << "Error in GaussContext::Transform" << endl;
		cerr << "Transforming a GaussContext with incompatible dimensions!" << endl;
		cerr << "Transformation is of " << t.NumDim() << " Dimensions" << endl;
		cerr << "GaussContext is of " << NumDim() << " Dimensions" << endl;
		assert(t.NumDim()==NumDim());
	}
		
	int	i, j, k, ic;
	GaussContext	c(*this);	// Copy current context

	for(i=1;i<=nnodes;i++)		// dNdb = 0.0
		N(i) = 0.0;

	for(i=1;i<=nnodes;i++)
		for(j=1;j<=ndim;j++)
			B(j,i) = 0.0;		// Zero terms

	ic = t.ParAt(p, bn);		// Get Transformation at p
#if	0
	cout << "ParTransform" << endl;
	cout << t << endl;
#endif
		
	for(i=1;i<=nnodes;i++)		// Matrix Multiply
		for(j=1;j<=ndim;j++)
			for(k=1;k<=ndim;k++)
				B(j,i)+=c.B(k,i)*t.ParJi(k,j);

//	dj = c.detJ()*t.PardetJ();
	dj = t.PardetJ();			// I don't think I want the c.detJ()
	return ic;
}

GaussContext::~GaussContext()
{
	DeAllocate();
}

ostream& operator<<(ostream& o, const GaussContext& gc)
{
	int	i, j;
	Coord3D	c;
	o << "Number of Nodes : " << gc.NumNodes() << endl;
	o << "Number of Dimensions : " << gc.NumDim() << endl;
	c = gc.X();
	o << "Position : (" << c.x << ", " << c.y << ", " << c.z << ") " << endl;
	o << "N = [ ";
	for(i=1;i<=gc.NumNodes();i++) o << gc.N(i) << " ";
	o << "]" << endl;

	o << "B = [";
	for(i=1;i<=gc.NumNodes();i++)
	{
		o << " [ ";
		for(j=1;j<=gc.NumDim();j++)
			o << gc.B(j,i) << " ";
		o << " ] ";
	}
	o << " ] " << endl;
	o << "|J| = " << gc.detJ();
	return o;
}


