#ifndef	_GAUSSCONTEXT_H_
#define	_GAUSSCONTEXT_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: GaussContext.h,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#include	<space/coord3d.h>
class	Transformation;

#if	1
#define		GC_IS_GC
#endif

#ifdef	GC_IS_GC
#include	<Misc/GC.h>
#endif

#ifdef	USE_MCHUNK
#include	"MemChunk.h"
#endif

#ifdef	GC_IS_GC
struct	GaussContext : public AllocGC
#else
struct	GaussContext
#endif
{
	private:
		GaussContext();
		void operator=(const GaussContext&);
	protected:
#ifdef	USE_MCHUNK
		DoubleChunk	chunk;
#endif
		double	*n;
		double	**b;
		double	dj;
		Coord3D	p;
		int	nnodes;
		int	ndim;

#ifdef	GC_IS_GC
		// Memory Allocations stuff
		AllocGCInterface;
#endif

		void Allocate();
		void DeAllocate();
	public:
		GaussContext(int nn, int nd, const Coord3D&c);
		GaussContext(const GaussContext& c);
		inline	const	int&	NumNodes() const { return nnodes; }
		inline	const	int&	NumDim() const { return ndim; }
		inline	double&	N(int i) { return n[i-1]; }
#ifdef	USE_MCHUNK
		inline	double&	B(int i, int j) { return (n+nnodes*i)[j-1]; }
#else
		inline	double&	B(int i, int j) { return b[i-1][j-1]; }
#endif
		inline	double&	detJ() { return dj; }
		inline	const Coord3D&	X() const { return p; }
		inline	const double&	N(int i) const { return n[i-1]; }
#ifdef	USE_MCHUNK
		inline	const double&	B(int i, int j) const
		{ return (n+nnodes*i)[j-1]; }
#else
		inline	const double&	B(int i, int j) const { return b[i-1][j-1]; }
#endif
		inline	const double&	detJ() const { return dj; }
		void	Transform(Transformation&);
		int		ParTransform(Transformation&, int bn);
		~GaussContext();
};

ostream& operator<<(ostream& o, const GaussContext& gc);

#endif