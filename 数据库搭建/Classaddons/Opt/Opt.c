#include	"Opt.h"
#include	<math.h>
#include	<stdlib.h>
#include	<iostream.h>
#include	<assert.h>

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

Optimizer::Optimizer(Optimizable &o, DesignVec x0) : opt(o)
{
	int	i;

	nv = opt.NumVars();
	pos = NEW(double,nv);
	for(i=0;i<nv;i++)
		pos[i] = x0[i];
}

double *
Optimizer::MakeVector()
{
	int	i;
	double	*ret;

	ret = NEW(double,nv);
	for(i=0;i<nv;i++)
		ret[i] = 0.0;
	return(ret);
}

double **
Optimizer::MakeMatrix(int n)
{
	int	i, j;
	double	**ret;

	ret = NEW(double *,n);
	for(i=0;i<n;i++)
		ret[i] = NEW(double,n);

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			ret[i][j] = 0.0;
	
	return ret;
}

void
Optimizer::DeleteMatrix(double **x0, int n)
{
	int	i;

	for(i=0;i<n;i++)
		DELETE(x0[i]);
	DELETE(x0);
}

double	&
Optimizer::operator[](int i)
{
	assert(i<=nv && i>=1);
	return(pos[i-1]);
}

void
Optimizer::GetPos(double *nx)
{
	int	i;

	for(i=0;i<nv;i++)
		nx[i] = pos[i];
}

int
Optimizer::Optimize()
{
	int count, i;

	for(i=0;!Iterate(1,count);i+=count);

	return(i);
}

int
Optimizer::Optimize(int max)
{
	int count, i;

	for(i=0;!Iterate(1,count) && i<max; i+=count);

	return(i);
}


void
Optimizer::Reposition(DesignVec x0)
{ 
	int	i;

	for(i=0;i<nv;i++)
		pos[i] = x0[i];
}

Optimizer::~Optimizer()
{
	DELETE(pos);
}

double
Optimizable::GradMag(DesignVec x)
{
	int	i, n = NumVars();
	double	mag;
	double *g = NEW(double,NumVars());

	Gradient(x,g);
	for(i=0,mag=0.0;i<n;i++)
		mag+=g[i]*g[i];
	mag = sqrt(mag);

	DELETE(g);
	return mag;
}

void
Optimizable::D2(DesignVec, GradVec g, int)
{
	int	i;
	cerr << "Warning, second derivative information requested for an optimizable" << endl;
	cerr << "that isn't second order.   Returning all zeros" << endl;

	for(i=0;i<NumVars();i++)
		g[i] = 0.0;

	return;
}

LineFunc::LineFunc(Optimizable &o, SearchVec s0, DesignVec xp) : opt(o)
{
	int	i;
	nv = opt.NumVars();

	x0 = NEW(double,nv);
	s = NEW(double,nv);
	scr1 = NEW(double,nv);
	scr2 = NEW(double,nv);

	for(i=0;i<nv;i++)
	{
		x0[i] = xp[i];
		s[i] = s0[i];
	}
}

void
LineFunc::D2(double *g, double *alpha, int dummy)
{
	int	i, j;
	double	*nx = scr1;		// Scratch Vectors
	double	*ng = scr2;

	for(i=0;i<nv;i++)
		nx[i] = x0[i] + alpha[0]*s[i];

	g[0]=0.0;
	for(i=0;i<nv;i++)
	{
		opt.D2(ng,nx,i);
		for(j=0;j<nv;j++)
			g[0] += s[i]*ng[j]*s[j];
	}
}

int
LineFunc::NumVars() const
{
	return 1;
}

void
LineFunc::ReOrient(SearchVec ns)
{
	int	i;

	for(i=0;i<nv;i++)
		s[i] = ns[i];
}

void
LineFunc::RePosition(DesignVec nx)
{
	int	i;

	for(i=0;i<nv;i++)
		x0[i] = nx[i];
}

double
LineFunc::Function(DesignVec alpha)
{
	int	i;
	double	*nx = scr1;
	double	ret;

	for(i=0;i<nv;i++)
		nx[i] = x0[i] + alpha[0]*s[i];

	ret = opt.Function(nx);

	return	ret;
}

double
LineFunc::Function(double alpha)
{
	double	x[1];
	double	ret;

	x[0] = alpha;
	ret = Function(x);
	return ret;
}

void
LineFunc::Gradient(DesignVec alpha, GradVec g)
{
	int	i;
	double	*nx = scr1;
	double	*ng = scr2;

	for(i=0;i<nv;i++)
		nx[i] = x0[i] + alpha[0]*s[i];

	opt.Gradient(nx,ng);

	for(i=0,g[0]=0.0;i<nv;i++)
		g[0]+=ng[i]*s[i];
}

double
LineFunc::Gradient(double alpha)
{
	int	i;
	double	*nx = scr1;
	double	*ng = scr2;
	double	g;

	for(i=0;i<nv;i++)
		nx[i] = x0[i] + alpha*s[i];

	opt.Gradient(nx,ng);

	for(i=0,g=0.0;i<nv;i++)
		g+=ng[i]*s[i];

	return g;
}

double
LineFunc::GradMag(double alpha)
{
	return Gradient(alpha);
}

void
LineFunc::GetPos(double alpha, DesignVec r)
{
	int	i;

	for(i=0;i<nv;i++)
		r[i] = x0[i]+alpha*s[i];
}

LineFunc::~LineFunc()
{
	DELETE(x0);
	DELETE(s);
	DELETE(scr1);
	DELETE(scr2);
}

LineMin::LineMin(LineFunc &f) : func(f)
{
	alpha = 0.0;
}

double
LineMin::GetPos()
{
	return alpha;
}

Poly2::Poly2(double A0, double A1, double A2)
{
	a0 = A0; a1 = A1; a2 = A2;
}

Poly2::Poly2(Optimizable &o, double step)
{
	double	x1, x2, f1, f2, df1, st2;

	// Error, Trying to fit a polynomial to a
	// function of more than 1 variable
	assert(o.NumVars()==1);

	x1 = 0.0;
	x2 = step;
	f1 = o.Function(&x1);
	f2 = o.Function(&x2);
	o.Gradient(&x1,&df1);

	st2 = (x2-x1)*(x2-x1);
	a0 = (f2*x1*x1-2*f1*x1*x2+df1*x1*x1*x2+f1*x2*x2-df1*x1*x2*x2)/st2;
	a1 = (2*f1*x1-2*f2*x1-df1*x1*x1+df1*x2*x2)/st2;
	a2 = (-f1+f2+df1*x1-df1*x2)/st2;
}

double
Poly2::Function(DesignVec x)
{
	double	&X = *x;
	return a0+a1*X+a2*X*X;
}

void
Poly2::Gradient(DesignVec x, GradVec g)
{
	double	&X = *x;
	
	*g = a1+2.0*a2*X;
}

Poly3::Poly3(double A0, double A1, double A2, double A3)
{
	a0 = A0; a1 = A1; a2 = A2; a3 = A3;
}

Poly3::Poly3(Optimizable &o, double step)
{
	double	x1, x2, f1, f2, df1, df2, den;

	// Error, trying to fit a polynomial to a function of 
	// more than one variable
	assert(o.NumVars()==1);

	x1 = 0.0;
	x2 = step;
	f1 = o.Function(&x1);
	f2 = o.Function(&x2);
	o.Gradient(&x1,&df1);
	o.Gradient(&x2,&df2);

	den = -(x2-x1)*(x2-x1)*(x2-x1);
	a0 = (f2*x1*x1*x1-3*f2*x1*x1*x2-df2*x1*x1*x1*x2+
		3*f1*x1*x2*x2-df1*x1*x1*x2*x2+
		df2*x1*x1*x2*x2-f1*x2*x2*x2+df1*x1*x2*x2*x2)/den;
	a1 = (df2*x1*x1*x1-6*f1*x1*x2+6*f2*x1*x2+
		2*df1*x1*x1*x2+df2*x1*x1*x2-df1*x1*x2*x2-
		2*df2*x1*x2*x2-df1*x2*x2*x2)/den;
	a2 = (3*f1*x1-3*f2*x1-df1*x1*x1-2*df2*x1*x1+3*f1*x2-
		3*f2*x2-df1*x1*x2+df2*x1*x2+2*df1*x2*x2+df2*x2*x2)/den;
	a3 = -(2*f1-2*f2-df1*x1-df2*x1+df1*x2+df2*x2)/den;
}

double
Poly3::Function(DesignVec x)
{
	double	&X = *x;
	return a0+a1*X+a2*X*X+a3*X*X*X;
}

void
Poly3::Gradient(DesignVec x, GradVec g)
{
	double	&X = *x;
	
	*g = a1+2.0*a2*X+3.0*a3*X*X;
}

Poly5::Poly5(double A0, double A1, double A2, double A3, double A4, double A5)
{
	a0 = A0; a1 = A1; a2 = A2;
	a3 = A3; a4 = A4; a5 = A5;
}

Poly5::Poly5(Optimizable &o, double step1, double step2)
{
	double	x1, xm, x2, f1, fm, f2, df1, dfm, df2;

	if (o.NumVars()!=1)
	{
		fprintf(stderr,
			"Error, Trying to fit a polynomial to a function of %d variables!\n",
			o.NumVars());
		a0 = a1 = a2 = a3 = a4 = a5 = 0.0;
	}

	x1 = 0.1;
	xm = step1;
	x2 = step2;
	f1 = o.Function(&x1);
	fm = o.Function(&xm);
	f2 = o.Function(&x2);
	o.Gradient(&x1, &df1);
	o.Gradient(&xm, &dfm);
	o.Gradient(&x2, &df2);

#ifdef	DEBUG
	printf("x1 = %g, xm = %g, x2 = %g\n", x1, xm, x2);
	printf("f1 = %g, fm = %g, f2 = %g\n", f1, fm, f2);
#endif

	FILE	*p = popen("math > /dev/null", "w");
	char	line[1024];

	fprintf(p, "f = {%g, %g, %g, %g, %g, %g}\n", f1, fm, f2, df1, dfm, df2);
	fprintf(p, "m = {{1, x1, x1^2, x1^3, x1^4, x1^5},\n");
	fprintf(p, "     {1, x2, x2^2, x2^3, x2^4, x2^5},\n");
	fprintf(p, "     {1, x3, x3^2, x3^3, x3^4, x3^5},\n");
	fprintf(p, "     {0, 1, 2 x1, 3 x1^2, 4 x1^3, 5 x1^4},\n");
	fprintf(p, "     {0, 1, 2 x2, 3 x2^2, 4 x2^3, 5 x2^4},\n");
	fprintf(p, "     {0, 1, 2 x3, 3 x3^2, 4 x3^3, 5 x3^4}}\n");
	fprintf(p, "x1 = %g\n", x1);
	fprintf(p, "x2 = %g\n", xm);
	fprintf(p, "x3 = %g\n", x2);
	fprintf(p, "sol = Inverse[m].f\n");
	fprintf(p, "sol[[1]] >> math.sol1\n");
	fprintf(p, "sol[[2]] >> math.sol2\n");
	fprintf(p, "sol[[3]] >> math.sol3\n");
	fprintf(p, "sol[[4]] >> math.sol4\n");
	fprintf(p, "sol[[5]] >> math.sol5\n");
	fprintf(p, "sol[[6]] >> math.sol6\n");
	fprintf(p, "Quit\n");
	pclose(p);

	system("cat math.sol? > math.sol");
	p = fopen("math.sol", "r");
	a0 = atof(fgets(line,1023,p));
	a1 = atof(fgets(line,1023,p));
	a2 = atof(fgets(line,1023,p));
	a3 = atof(fgets(line,1023,p));
	a4 = atof(fgets(line,1023,p));
	a5 = atof(fgets(line,1023,p));
	fclose(p);
	system("rm math.sol*");

#ifdef	DEBUG
	printf("a = {%g, %g, %g, %g, %g, %g}\n", a0, a1, a2, a3, a4, a5);
#endif
}

double
Poly5::Function(DesignVec x)
{
	double	&X = *x;
	return a0+a1*X+a2*X*X+a3*X*X*X+a4*X*X*X*X+a5*X*X*X*X*X;
}

void
Poly5::Gradient(DesignVec x, GradVec g)
{
	double	&X = *x;
	
	*g = a1+2.0*a2*X+3.0*a3*X*X+4.0*a4*X*X*X+5.0*a5*X*X*X*X;
}

