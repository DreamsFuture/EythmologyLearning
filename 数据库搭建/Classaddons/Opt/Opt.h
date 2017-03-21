#ifndef	_OPT_H_
#define	_OPT_H_
#include	<stdio.h>
#include	<iostream.h>

#define	NEW(t,n)	new t[n]
#define	DELETE(p)	delete [] p

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

// 
// Class : Optimizable
//
//	An optimizable class is one which can be optimized.  This is basically
// an abstract class definition.  The only pure virtual functions are
// the number of design variables, the cost function and the gradient
// of the cost function.
//

typedef	double	*DesignVec;
typedef	double	*GradVec;
typedef	double	*SearchVec;

class	Optimizable
{
	public:
		virtual	int		NumVars() const = 0;
		virtual	double	Function(DesignVec) = 0;
		virtual	void	Gradient(DesignVec, GradVec) = 0;
		double			GradMag(DesignVec);
		virtual	void	D2(DesignVec, GradVec, int);
		virtual	int		isSecondOrder() const { return 0; }
		virtual	~Optimizable() { }
};

class	LineFunc : public Optimizable
{
	private:
		// call this 'Copy Protection'
		LineFunc();
		LineFunc(const LineFunc&);
		void operator=(const LineFunc &);
	protected:
		int	nv;
		SearchVec	s;
		DesignVec	x0;
		DesignVec	scr1;
		DesignVec	scr2;
		Optimizable	&opt;
	public:
		LineFunc(Optimizable&, SearchVec, DesignVec);
		void	ReOrient(SearchVec);
		void	RePosition(DesignVec);
		int	NumVars() const;
		double	Function(DesignVec);
		void	Gradient(DesignVec, GradVec);
		double	Function(double);
		double	Gradient(double);
		double	GradMag(double);
		void	D2(DesignVec, GradVec, int);

		Optimizable	&GetOpt() { return opt; }
		int	isSecondOrder() const
		{
			return opt.isSecondOrder();
		}
		void	GetPos(double, DesignVec);

		double	operator()(double b) { return Function(b); }
		virtual	~LineFunc();
};

class	Optimizer
{
	private:
		// call this 'Copy Protection'
		Optimizer(const Optimizer&);
		void	operator=(const Optimizer&);
	protected:
		int		nv;
		DesignVec	pos;
		Optimizable	&opt;

		double	**MakeMatrix(int);
		void	DeleteMatrix(double **,int);
		double	*MakeVector();
		virtual	void Reposition(DesignVec);
	public:
		Optimizer(Optimizable &, DesignVec);

		virtual	int	Iterate(int, int &) = 0;  // Return 1 if done
		virtual	int	Optimize();
		virtual	int	Optimize(int max);

		double	&operator[](int);
		void	GetPos(DesignVec);
		virtual	~Optimizer();
};

class	LineMin
{
	private:
		// call this 'Copy Protection'
		LineMin();
		LineMin(const LineMin&);
		void	operator=(const LineMin&);

	protected:
		LineFunc	&func;
		double	alpha;
	public:
		LineMin(LineFunc&);

		// Find Min should return 1 one success
		virtual	int	FindMin(double) = 0;
		virtual	int	FindMin() { return FindMin(0.0); }

		LineFunc	&GetFunc() { return func; }
		double	GetPos();
};

class	Poly2	: public Optimizable
{
	protected:
		double	a0, a1, a2;
	public:
		Poly2(double,double,double);
		Poly2(Optimizable &, double);
		int	NumVars() const { return 1; }
		double	Function(DesignVec);
		void	Gradient(DesignVec, GradVec);
};

class	Poly3	: public Optimizable
{
	protected:
		double	a0, a1, a2, a3;
	public:
		Poly3(double,double,double,double);
		Poly3(Optimizable &, double);
		int	NumVars() const { return 1; }
		double	Function(DesignVec);
		void	Gradient(DesignVec, GradVec);
};

class	Poly5	: public Optimizable
{
	protected:
		double	a0, a1, a2, a3, a4, a5;
	public:
		Poly5(double,double,double,double,double,double);
		Poly5(Optimizable &, double, double);
		int	NumVars() const { return 1; }
		double	Function(DesignVec);
		void	Gradient(DesignVec, GradVec);
};

#endif

