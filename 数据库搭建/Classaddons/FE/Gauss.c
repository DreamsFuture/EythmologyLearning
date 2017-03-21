#include	"Gauss.h"
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

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
 * $Id: Gauss.c,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#if	0
#define	HAVE_NR
#endif

#ifdef	HAVE_NR
void	gauleg(double,double,double*,double*,int);
#define	GAUSSROUTINE(x1,x2,x,w,n) gauleg(x1,x2,x-1,w-1,n)
#else
void	mygauleg(int,double,double,double*,double*);
#define	GAUSSROUTINE(x1,x2,x,w,n) mygauleg(n,x1,x2,x,w)
#endif

GaussPoints::GaussPoints(double x1, double x2, int n)
{
#ifdef	MDEBUG
	malloc_enter("GaussPoints::GaussPoints");
#endif
	double	*w;
	double	*x;
	int	i;

	w = new double[n];
	x = new double[n];

	weights.SetSize(n);
	GAUSSROUTINE(x1, x2, x, w, n);
	for(i=1;i<=n;i++)
	{
		weights[i].w = w[i-1];
		weights[i].x = x[i-1];
	}

	delete [] x;
	delete [] w;
#ifdef	MDEBUG
	malloc_leave("GaussPoints::GaussPoints");
#endif
}

GaussWeight2::GaussWeight2()
{
	w = 0.0;
	x = y = 0.0;
}

GaussWeight2::GaussWeight2(const GaussWeight2& gpw)
{
	w = gpw.w;
	x = gpw.x;
	y = gpw.y;
}

GaussWeight2::GaussWeight2(double X, double Y, double W)
{
	w = W;
	x = X;
	y = Y;
}

GenGaussWeight::GenGaussWeight()
{
	w = c.x = c.y = c.z = 0.0;
}

GenGaussWeight::GenGaussWeight(const GenGaussWeight& ggw)
{
	w = ggw.w;
	c = ggw.c;
}

GenGaussWeight::GenGaussWeight(const Coord3D& C, double W)
{
	w = W;
	c = C;
}

ostream& operator<<(ostream& o, const GenGaussWeight& ggw)
{
	o << "(" << ggw.c.x << ", " << ggw.c.y << ", " << ggw.c.z << ") . "
			<< ggw.w << endl;
	return o;
}

const	GaussSurface	TwoByTwo(-1.0,1.0,-1.0,1.0,2,2);

GaussSurface::GaussSurface(double x1, double x2,
                         double y1, double y2, int nx, int ny)
{
	int	i, j;
	GaussPoints	x(x1,x2,nx);
	GaussPoints y(y1,y2,ny);
	GaussWeight	wx, wy;

	weights.SetSize(nx*ny);	// An optimization hack
	foreach(i,x)
	{
		x.Get(i,wx);
		foreach(j,y)
		{
			y.Get(j,wy);
			weights[(i-1)*2+j] = GaussWeight2(wx.x, wy.x, wx.w*wy.w);
		}
	}
}

GenGauss::GenGauss(const Coord3D &p1, const Coord3D &p2,
                   int nx, int ny, int nz)
{
	int	i, j, k;
	GaussPoints	x(p1.x,p2.x,nx);
	GaussPoints y(p1.y,p2.y,ny);
	GaussPoints z(p1.z,p2.z,nz);
	GaussWeight	wx, wy, wz;

	num_dim=0;
	if (nx>1) num_dim++;
	if (ny>1) num_dim++;
	if (nz>1) num_dim++;
	if (num_dim==0) num_dim++;

	foreach(i,x)
	{
		x.Get(i,wx);
		foreach(j,y)
		{
			y.Get(j,wy);
			foreach(k,z)
			{
				z.Get(k,wz);
				Coord3D	c(wx.x,wy.x,wz.x);
				double	w=1.0;

				if (nx > 1) w*=wx.w;
				if (ny > 1) w*=wy.w;
				if (nz > 1) w*=wz.w;

				weights+=GenGaussWeight(c, w);
			}
		}
	}
}

