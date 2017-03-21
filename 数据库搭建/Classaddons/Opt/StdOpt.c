#include	"StdOpt.h"
#include	<math.h>
#include	<stdio.h>
#include	<iostream.h>

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

static	const	double	GOLD=1.618034;
static	const	double	RGOLD=.61803399;

#define	SHIFT4(a,b,c,d)	(a)=(b); (b)=(c); (c)=(d)
#define	SHIFT3(a,b,c)	(a)=(b); (b)=(c);

ostream&
operator<<(ostream& o, const Bracket& br)
{
	o << "Bracket = [" <<
		br.a << " (" << br.fa << "), " <<
		br.b << " (" << br.fb << "), " <<
		br.c << " (" << br.fc << ") ]" << endl;

	return o;
}

const double	Bracket::GOLD = 1.618034;
const double	Bracket::GLIMIT = 100.0;
const double	Bracket::TINY = 1.0e-20;

double 
Bracket::Sign(const double& a, const double &b)
{
	if (b>0.0)
		return fabs(a);
	else
		return -fabs(a);
}

double
Bracket::Max(const double &a, const double &b)
{
	if (a>b)
		return a;
	else
		return b;
}


int
Bracket::Find(LineFunc& o, double a0, double b0)
{
	double	ulim, u, r, q, fu, dum;

	a = a0;
	b = b0;
	fa = o.Function(a);
	fb = o.Function(b);

	if (fb > fa)
	{
		SHIFT4(dum, a, b, dum);
		SHIFT4(dum, fa, fb, dum);
	}
	c = b+GOLD*(b-a);
	fc = o.Function(c);
	while(fb > fc)
	{
		r = (b-a)*(fb-fc);
		q = (b-c)*(fb-fa);
		u = b-((b-c)*q-(b-a)*r)/(2.0*Sign(Max(fabs(q-r),TINY),q-r));

		// Set a limit on how far to move
		ulim = b+GLIMIT*(c-b);

		if ((b-u)*(u-c) > 0.0)
		{
			fu = o.Function(u);
			if (fu < fc)
			{
				a = b;
				b = u;
				fa = fb;
				fb = fu;
				return 1;
			}
			else
			{
				if (fu > fb)
				{
					c = u;
					fc = fu;
					return 1;
				}
			}
			u = c+GOLD*(c-b);
			fu = o.Function(u);
		}
		else
		{
			if ((c-u)*(u-ulim) > 0.0)
			{
				fu = o.Function(u);
				if (fu < fc)
				{
					SHIFT4(b, c, u, c+GOLD*(c-b));
					SHIFT4(fb,fc,fu,o.Function(u));
				}
			}
			else
			{
				if ((u-ulim)*(ulim-c) >= 0.0)
				{
					u = ulim;
					fu = o.Function(u);
				}
				else
				{
					u = c+GOLD*(c-b);
					fu = o.Function(u);
				}
			}
		}
		SHIFT4(a,b,c,u);
		SHIFT4(fa, fb, fc, fu);
	}
	return 1;
}

Parabolic::Parabolic(LineFunc &f, double b, double e) : LineMin(f)
{
	x2 = b;
	eps = e;
}

int
Parabolic::FindMin(double alpha0)
{
	double	f1,df1,f2;
	double	a0,a1,a2,st2;

	x1 = alpha0;

	do
	{
		f1 = func.Function(x1);
		f2 = func.Function(x2);
		df1 = func.Gradient(x1);
	
		st2 = (x2-x1)*(x2-x1);
		a0 = (f2*x1*x1-2*f1*x1*x2+df1*x1*x1*x2+f1*x2*x2-df1*x1*x2*x2)/st2;
		a1 = (2*f1*x1-2*f2*x1-df1*x1*x1+df1*x2*x2)/st2;
		a2 = (-f1+f2+df1*x1-df1*x2)/st2;

		alpha = (-a1/(2*a2));
		df1 = func.Gradient(alpha);
	
		if (df1 < 0)
			x1 = alpha;
		else
			x2 = alpha;
	
#ifdef	DEBUG
		printf("[%g %g] Min at %g, gradient = %g (%g)\n",
			x1, x2, alpha, df1, eps);
#endif
	
	} while(func.GradMag(alpha) > eps);
	return 1;
}

const	double	Section::Ratio = 0.61803399;
const	double	Section::Complement = (1.0-Section::Ratio);

Section::Section(LineFunc &f, double e) : LineMin(f)
{
	eps = e;
}

int
Section::FindMin(double alpha0)
{
	double	f1, f2, x0, x1, x2, x3;

	br.Find(func, alpha0, alpha0+1.0);

	cout << br << endl;

	if (br.a > br.c)
	{
		x0 = br.c;
		x3 = br.a;
	}
	else
	{
		x0 = br.a;
		x3 = br.c;
	}

	if (fabs(br.c-br.b) > fabs(br.b-br.a))
	{
		x1 = br.b;
		x2 = br.b+Complement*(br.c-br.b);
	}
	else
	{
		x2 = br.b;
		x1 = br.b-Complement*(br.b-br.a);
	}
	f1 = func(x1);
	f2 = func(x2);
	while(fabs(x3-x0) > eps*(fabs(x1)+fabs(x2)))
	{
#ifdef	DEBUG
		cout.form("%g | %g [%g] | %g [%g] | %g", x0, x1, f1, x2, f2, x3) << endl;
#endif
		if (f2 < f1)
		{
			SHIFT4(x0,x1,x2,Ratio*x1+Complement*x3);
			SHIFT3(f1,f2,func(x2));
		}
		else
		{
			SHIFT4(x3,x2,x1,Ratio*x2+Complement*x0);
			SHIFT3(f2,f1,func(x1));
		}
	}

	if (f1 < f2)
		alpha = x1;
	else
		alpha = x2;

	return 1;
}

Cubic::Cubic(LineFunc &f, double b, double e) : LineMin(f)
{
	x2 = b;
	eps = e;
}

int
Cubic::FindMin(double alpha0)
{
	double	f1,df1,f2,df2;
	double	a0,a1,a2,a3,den;
	double	r1, r2;

	x1 = alpha0;

	do
	{
		f1 = func.Function(x1);
		f2 = func.Function(x2);
		df1 = func.Gradient(x1);
		df2 = func.Gradient(x2);
	
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
	
		// Assumes no complex roots...minimum better be in the interval
		r1 = (-a2+sqrt(a2*a2-3*a1*a3))/(3*a3);
		r2 = -(a2+sqrt(a2*a2-3*a1*a3))/(3*a3);
	
		if (isnan(r1) && isnan(r2))
			return	0;

#ifdef	DEBUG
		printf("a0 = %g, a1 = %g, a2 = %g, a3 = %g\n", a0, a1, a2, a3);
		printf("x1 = %g, x2 = %g\n", x1, x2);
		printf("r1 = %g (%g), r2 = %g (%g)\n",
			r1, func.Function(r1),
			r2, func.Function(r2));
#endif
	
		if (func.Function(r1) > func.Function(r2))
			alpha = r2;
		else
			alpha = r1;
	
		df1 = func.Gradient(alpha);
		if (df1 < 0)
			x1 = alpha;
		else
			x2 = alpha;
	
		df1 = func.GradMag(alpha);
	} while(df1 < eps);
	return 1;
}

#if	0
Quintic::Quintic(LineFunc &f, double s1, int mi, double e) : LineMin(f)
{
	eps = e;
	step1 = s1;
	maxiters = mi;
}

int
Quintic::Iterate(int n, int &c)
{
	Poly5	p5(*opt,step0,step1);
	Bisect	bisect(func, step1, eps);
	double	x0 = 0.0;
	int	stat;
	
	c = 1;
	bisect.Attach(p5, &x0);
	stat = bisect.Optimize();
#ifdef	DEBUG
	printf("Bisect::Optimize returned %d\n", stat);
#endif
	if (stat==0)
	{
		c = 0;
		return 1;
	}
	*x = bisect.GetPos();

	return	1;
}
#endif

Steep::Steep(LineMin &s, DesignVec x0, double mingrad)
	: Optimizer(s.GetFunc().GetOpt(), x0), search(s)
{
	eps = mingrad;
	scr1 = NEW(double,nv);
	pos = NEW(double,nv);

	Reposition(x0);
	search.GetFunc().RePosition(x0);
}

int
Steep::Iterate(int n, int &c)
{
	GradVec		g = scr1;
	double	mag;
	int	i;

#ifdef	DEBUG
	cout << "Position is {";
	for(i=0;i<nv;i++)
		cout << pos[i] << (i==nv-1 ? ' ' : ',');
	cout << "}" << endl;
#endif

	opt.Gradient(pos, g);
	mag = opt.GradMag(pos);

#ifdef	DEBUG
	cout << "Gradient Magnitude = " << mag << endl;
	cout << "Gradient is {";
	for(i=0;i<nv;i++)
		cout << g[i] << (i==nv-1 ? ' ' : ',');
	cout << "}" << endl;
#endif
	for(i=0;i<nv;i++)
		g[i] /= -mag;

#ifdef	DEBUG
	cout << "Search direction is {";
	for(i=0;i<nv;i++)
		cout << g[i] << (i==nv-1 ? ' ' : ',');
	cout << "}" << endl;
#endif

	search.GetFunc().ReOrient(g);
	if (!search.FindMin())
	{
		cerr << "Unable to improve!" << endl;
		return 1;
	}
	cout << "Alpha = " << search.GetPos() << endl;
	search.GetFunc().GetPos(search.GetPos(), pos);

	printf("# Solution is {");
	for(i=0;i<nv;i++)
		printf("%g%c", pos[i], (i==nv-1 ? ' ' : ','));
	printf("}\n");

	search.GetFunc().RePosition(pos);
	if (opt.GradMag(pos) < eps)
		return 1;
	else
		return 0;
}

Steep::~Steep()
{
	DELETE(scr1);
	DELETE(pos);
}

BFGS::BFGS(LineMin &lm, DesignVec x0, double mingrad)
	: Optimizer(lm.GetFunc().GetOpt(), x0), search(lm)
{
	int	i;

	eps = mingrad;
	B = MakeMatrix(nv);
	Bp = MakeMatrix(nv);

	pos = NEW(double,nv);
	y = MakeVector();
	p = MakeVector();
	df = MakeVector();
	dfp = MakeVector();
	s = MakeVector();
	xp = MakeVector();
	By = MakeVector();

	for(i=0;i<nv;i++)
		pos[i] = x0[i];
}

BFGS::~BFGS()
{
	DeleteMatrix(B, nv);
	DeleteMatrix(Bp, nv);
	DELETE(By);
	DELETE(xp);
	DELETE(pos);
	DELETE(s);
	DELETE(dfp);
	DELETE(df);
	DELETE(p);
	DELETE(y);
}

int
BFGS::Iterate(int n, int &c)
{
	int	i, j;
	double	sigma, tau, theta=1.0, mag;

	c = 1;
	for(i=0;i<nv;i++) xp[i] = pos[i];

	opt.Gradient(xp, dfp);
#ifdef	DEBUG
	printf("Starting from {");
	for(i=0;i<nv;i++)
		printf("%g%c", xp[i], (i==nv-1 ? ' ' : ','));
	printf("}\n");
	printf("Gradient is {");
	for(i=0;i<nv;i++)
		printf("%g%c", dfp[i], (i==nv-1 ? ' ' : ','));
	printf("}\n");
#endif

	for(i=0;i<nv;i++)
		for(j=0,s[i]=0.0;j<nv;j++)
			s[i] += -B[i][j]*dfp[j];
	for(i=0,mag=0.0;i<nv;i++)
		mag+=s[i]*s[i];
	for(i=0;i<nv;i++)
		s[i]/=sqrt(mag);

#ifdef	DEBUG
	printf("Search Direction is {");
	for(i=0;i<nv;i++)
		printf("%g%c", s[i], (i==nv-1 ? ' ' : ','));
	printf("}\n");
#endif

	search.GetFunc().ReOrient(s);
	if (!search.FindMin())
	{
		cerr << "Unable to improve!" << endl;
		return 1;
	}
	search.GetFunc().GetPos(search.GetPos(), pos);
	search.GetFunc().RePosition(pos);

#ifdef	DEBUG
	printf("Minimized at {");
	for(i=0;i<nv;i++)
		printf("%g%c", pos[i], (i==nv-1 ? ' ' : ','));
	printf("}\n");
#endif
	opt.Gradient(pos, df);

	for(i=0;i<nv;i++) y[i] = df[i] - dfp[i];
	for(i=0;i<nv;i++) p[i] = pos[i] - xp[i];
	for(i=0;i<nv;i++) By[i] = 0.0;
	for(i=0;i<nv;i++)
		for(j=0;j<nv;j++)
		{
			By[i] += Bp[i][j]*y[j];
			Bp[i][j] = B[i][j];
		}

	for(i=0,sigma=0.0;i<nv;i++) sigma += y[i]*p[i];
	for(i=0,tau=0.0;i<nv;i++) tau += y[i]*By[i];

#if	0
	printf("sigma = %g\n", sigma);
	printf("tau = %g\n", tau);
	printf("p = {%g, %g}\n", p[0], p[1]);
	printf("y = {%g, %g}\n", y[0], y[1]);
	printf("By = {%g, %g}\n", By[0], By[1]);
#endif

	double	v;
	for(i=0;i<nv;i++)
		for(j=0;j<nv;j++)
		{
			v = ((sigma+theta*tau)/(sigma*sigma))*p[i]*p[j];
			B[i][j] += v;
			v = ((theta-1.0)/tau)*(By[i]*By[j]);
			B[i][j] += v;
			v = -(theta/sigma)*(By[i]*p[j]);
			B[i][j] += v;
			v = -(theta/sigma)*(p[i]*By[j]);
			B[i][j] += v;
		}

	printf("Gradient Mag = %g (%g)\n", opt.GradMag(pos), eps);

	printf("# Solution is {");
	for(i=0;i<nv;i++)
		printf("%g%c", pos[i], (i==nv-1 ? ' ' : ','));
	printf("}\n");

	if (tau==0.0 || sigma==0.0)
		return 1;

	if (opt.GradMag(pos) < eps)
		return 1;
	else
		return 0;
}

#if	0
Newton::Newton(double e, int mi)
{
	eps = e;
	maxiters = mi;
}

void
Newton::Attach(Optimizable &o, double *x0)
{
	iters = 0;
	Optimizer::Attach(o, x0);
}

int
Newton::Iterate(int, int& c)
{
	double	m[2][2];
	double	dx[2], df[2], den;
	c = 1;

	if (opt->NumVars()!=2)
	{
		c = 0;
		fprintf(stderr, "Sorry, Newton not implemented for !=2 design vars!\n");
		return 1;
	}

	if (iters==maxiters)
	{
		printf("Max Iterations exceeded in Newton-Raphson!\n");
		c = 0;
		return 1;
	}

	opt->Gradient(x, df);
	opt->D2(m[0], x, 1);
	opt->D2(m[1], x, 2);

	printf("D2 = {{ %g, %g }, { %g, %g }}\n", m[0][0], m[0][1], m[1][0], m[1][1]);
	den = m[0][0]*m[1][1]-m[0][1]*m[1][0];
	dx[0] = -(m[1][1]*df[0]-m[0][1]*df[1])/den;
	dx[1] = -(m[0][0]*df[1]-m[1][0]*df[0])/den;
	printf("dx = { %g, %g }\n", dx[0], dx[1]);
	pos[0]+=dx[0];
	pos[1]+=dx[1];
	printf("x = { %g, %g }\n", pos[0], pos[1]);

	iters++;

	if (opt->GradMag(pos) < eps)
		return 1;
	else
		return 0;
}

#endif
