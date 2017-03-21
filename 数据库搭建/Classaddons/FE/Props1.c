#include	"Props1.h"
#include	"GPInfo.h"
#include	"Const.h"
#include	<math.h>
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

const	double	RadConv1::sfb = 5.67e-11;

NonLinCond1::NonLinCond1(double mp, const Const& d) : k(mp), dof(d)
{
}

void
NonLinCond1::Eval(double *v, LocalDomain& d, Element& e, const GPInfo& c) const
{
#ifdef	MDEBUG
	malloc_enter("NonLinCond1::Eval");
#endif
	double	*u = new double[c.Size()];
	int	i;

	Recover1(d, dof, e, u, c);

	foreach(i,c)
		v[i-1] = k*(1.0+.75e-8*pow(u[i-1], 3.0));

	delete [] u;
#ifdef	MDEBUG
	malloc_leave("NonLinCond1::Eval");
#endif
}

int
NonLinCond1::TanEval(double *v, const Const& df, LocalDomain& d, 
				 Element& e, const GPInfo& c) const
{
#ifdef	MDEBUG
	malloc_enter("NonLinCond1::TanEval");
#endif
	int	i;
	if ((int)df != (int)dof)
	{
		foreach(i,c) v[i-1] = 0.0;
		return 0;
	}
	double	*u = new double[c.Size()];

	Recover1(d, dof, e, u, c);

	foreach(i,c)
		v[i-1] = 3.0*k*.75e-8*pow(u[i-1], 2.0);

	delete [] u;
#ifdef	MDEBUG
	malloc_leave("NonLinCond1::TanEval");
#endif
	return 1;
}

int
NonLinCond1::ParEval(double *v, int bn, LocalDomain& d, Element& e,
	const GPInfo& c) const
{
	int	i;

	foreach(i,c)
		v[i-1] = 0.0;
	return 1;
}

RadConv1::RadConv1(double mp, const Const& d, const MatProp& ti)
	: h(mp), dof(d), Ti(ti)
{
}

void
RadConv1::Eval(double *v, LocalDomain& d, Element& e, const GPInfo& c) const
{
#ifdef	MDEBUG
	malloc_enter("RadConv1::Eval");
#endif
	double	*u = new double[c.Size()*2];
	double	*T = u+c.Size();
	int	i;

	Recover1(d, dof, e, u, c);
	Ti.Eval(T, d, e, c);

	foreach(i,c)
		v[i-1] = h*sfb*(u[i-1]*u[i-1]+T[i-1]*T[i-1])*(u[i-1]+T[i-1]);

	delete [] u;
#ifdef	MDEBUG
	malloc_leave("RadConv1::Eval");
#endif
}

int
RadConv1::TanEval(double *v, const Const& df, LocalDomain& d,
					Element& e, const GPInfo& c) const
{
#ifdef	MDEBUG
	malloc_enter("RadConv1::TanEval");
#endif
	int	i;
	if ((int)df != (int)dof)
	{
		cout << "Wrong DOF" << endl;
		foreach(i,c) v[i-1] = 0.0;
		return 0;
	}

	double	*u = new double[c.Size()*2];
	double	*T = u+c.Size();

	Recover1(d, dof, e, u, c);
	Ti.Eval(T, d, e, c);

	foreach(i,c)
		v[i-1] = h*sfb*(3*u[i-1]*u[i-1]+2*u[i-1]*T[i-1]+T[i-1]*T[i-1]);

	delete [] u;
#ifdef	MDEBUG
	malloc_leave("RadConv1::TanEval");
#endif
	return	1;
}

int
RadConv1::ParEval(double *v, int bn, LocalDomain& d, Element& e,
	const GPInfo& c) const
{
#ifdef	MDEBUG
	malloc_enter("RadConv1::ParEval");
#endif
	double	*u = new double[c.Size()*3];
	double	*T = u+c.Size();
	double	*dT = T+c.Size();
	int	i;

	Recover1(d, dof, e, u, c);
	Ti.Eval(T, d, e, c);
	Ti.ParEval(dT, bn, d, e, c);

	foreach(i,c)
		v[i-1] = h*sfb*(3*T[i-1]*T[i-1]+2*u[i-1]*T[i-1]+u[i-1]*u[i-1])*dT[i-1];

	delete [] u;
#ifdef	MDEBUG
	malloc_leave("RadConv1::ParEval");
#endif
	return	1;
}

