#ifndef	_GAUSSPOINTS_H_
#define	_GAUSSPOINTS_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Gauss.h,v 3.4 1993/06/10 20:59:29 tiller Exp tiller $
 */

#ifdef	__GNUC__
#pragma interface
#endif

#include <math.h>
#include <space/coord3d.h>
#include <Set/SimpleSet.h>

struct	GaussWeight
{
	double w, x;
	GaussWeight() { w = 0.0; x = 0.0; }
	GaussWeight(const GaussWeight& gpw) { w=gpw.w; x=gpw.x; }
};

struct	GaussWeight2
{
	double w, x, y;
	GaussWeight2();
	GaussWeight2(const GaussWeight2& gpw);
	GaussWeight2(double, double, double);
};

struct	GenGaussWeight
{
	Coord3D	c;
	double	w;
	GenGaussWeight();
	GenGaussWeight(const GenGaussWeight& gpw);
	GenGaussWeight(const Coord3D& C, double W);
};

ostream& operator<<(ostream&, const GenGaussWeight& ggw);

class	GaussPoints : public FixedSet<GaussWeight>
{
	protected:
		SimpleSet<GaussWeight>	weights;
	public:
		GaussPoints(double x1, double x2, int n);
		int	Size() const { return weights.Size(); }
		int	Get(int p, GaussWeight& t) { return weights.Get(p,t); }
		int	Get(int p, GaussWeight& t) const
		{
			return weights.Get(p,t);
		}
};

class	GaussSurface : public FixedSet<GaussWeight2>
{
	protected:
		SimpleSet<GaussWeight2>	weights;
	public:	
		GaussSurface(double, double, double, double, int, int);
		int	Size() const { return weights.Size(); }
		int	Get(int p, GaussWeight2& t) { return weights.Get(p,t); }
		int	Get(int p, GaussWeight2& t) const
		{
			return weights.Get(p,t);
		}
};

class	GenGauss : public FixedSet<GenGaussWeight>, public FixedSet<Coord3D>
{
	protected:
		SimpleSet<GenGaussWeight> weights;
		int	num_dim;
	public:
		GenGauss(const Coord3D &p1, const Coord3D &p2,
			int nx = 2, int ny = 1, int nz = 1);
		int	Size() const { return weights.Size(); }
		
		int	Get(int p, GenGaussWeight& t)
		{
			return weights.Get(p,t);
		}
		inline int NumDim() { return num_dim; }
		int	Get(int p, GenGaussWeight& t) const
		{
			return weights.Get(p,t);
		}
		int	Get(int p, Coord3D& c)
		{
			int	ret;
			GenGaussWeight	w;
			ret = Get(p, w);
			c = w.c;
			return ret;
		}
		int	Get(int p, Coord3D& c) const
		{
			int	ret;
			GenGaussWeight	w;
			ret = Get(p, w);
			c = w.c;
			return ret;
		}
};

extern	const	GaussSurface	TwoByTwo;
#endif


