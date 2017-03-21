#ifndef	_MIZER_H_
#define	_MIZER_H_

#include	"Opt.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

//
// Bracket related stuff
//

class	Bracket
{
	protected:
		static	const double	GOLD;
		static	const double	GLIMIT;
		static	const double	TINY;
	public:
		double	a, b, c;
		double	fa, fb, fc;
		int		Find(LineFunc& o, double, double);
		static	double	Sign(const double &, const double &);
		static	double	Max(const double &, const double &);
};

ostream& operator<<(ostream& o, const Bracket&);

//
// Searchers
//

class	Parabolic : public LineMin
{
	protected:
		double	eps;
		double	x1, x2;
	public:
		Parabolic(LineFunc &f, double, double e=1e-3);
		int	FindMin(double);
};

class	Cubic : public LineMin
{
	protected:
		double	eps;
		double	x1, x2;
	public:
		Cubic(LineFunc &f, double, double e=1e-3);
		int	FindMin(double);
};

class	Section : public LineMin
{
	protected:
		static	const	double	Ratio;
		static	const	double	Complement;
		double	eps;
		Bracket	br;
	public:
		Section(LineFunc &f, double e=1e-8);
		int	FindMin(double);
};

#if	0
class	Quintic : public LineMin
{
	protected:
		double	eps, step0, step1;
		int		maxiters;
	public:
		Quintic(LineFunc &f, double s=1.0,int mi=25,double e=1e-3);
		int	FindMin(double);
};
#endif

//
// Optimizers
//

class	Steep : public Optimizer
{
	protected:
		double		eps;
		double		*scr1;
		LineMin		&search;
	public:
		Steep(LineMin&, DesignVec x0, double mingrad=1e-3);
		int	Iterate(int, int &);
		~Steep();
};

class	BFGS : public Optimizer
{
	protected:
		double		eps;
		double		**B, **Bp;
		double		*y, *p, *df, *dfp, *s, *xp, *By;
		LineMin		&search;

	public:
		BFGS(LineMin& s, DesignVec x0, double mingrad=1e-3);
		int	Iterate(int, int &);
		BFGS::~BFGS();
};

#if	0
class	Newton : public Optimizer
{
	protected:
		double	eps;
		double	**D2;
		int	maxiters, iters;
	public:
		Newton(double mingrad=1e-3, int mi=25);
		int	Iterate(int, int &);
};
#endif

#endif
