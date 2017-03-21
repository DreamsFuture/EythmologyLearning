#include	<iostream.h>
#include	<assert.h>
#include	"Domain.h"
#include	"Design.h"
#include	"MatProp.h"
#include	"Element.h"
#include	"BasicTypes.h"
#include	"GPInfo.h"
#include	"GaussContext.h"

#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

void
Recover1(LocalDomain &d, const Const &dof, Element &e,
		 double *ssd, const GPInfo &c)
{
#ifdef	MDEBUG
	malloc_enter("Recover1");
#endif
	int	i, j;
	double	*tprev = new double[e.nodes().Size()];
	const	FixedSet<Node *>&	nodes = e.nodes();
	GaussContext	*gcp;
	long	eq;
	short	val;
	
	foreach(j,nodes)
	{
		val = dof;
		eq = e.EqNum(j,val);
		tprev[j-1] = d.GetSol(eq);
	}

	foreach(i, c)
	{
		gcp = c[i];
		ssd[i-1] = 0.0;
		foreach(j, nodes)
			ssd[i-1] += gcp->N(j)*tprev[j-1];
	}
	delete [] tprev;
#ifdef	MDEBUG
	malloc_leave("Recover1");
#endif
}

int	
MatProp::TanEval(double*v, const Const&, LocalDomain&,
	Element&,const GPInfo &c) const
{
	int	i;

	foreach(i,c)	// just in case
		v[i-1] = 0.0;
	return 0;
}

int	
MatProp::ParEval(double* v, int, LocalDomain&,
	Element&,const GPInfo& c) const
{
	int	i;

	foreach(i,c)	// just in case
		v[i-1] = 0.0;
	return 0;
}

StdMatProp::StdMatProp(int type, const Const& d, int i1, int i2, int i3)
	: id(type), dof(d)
{
	I[0] = i1;
	I[1] = i2;
	I[2] = i3;
}

void
StdMatProp::Eval(double *v, LocalDomain& d, Element& e, const GPInfo&c) const
{
#ifdef	MDEBUG
	malloc_enter("StdMatProp::Eval");
#endif
	double	*u = new double[c.Size()];
	double	A[3], X[3], time = d.Time();
	int	i, error;

	Recover1(d, dof, e, u, c);

	foreach(i,c)
	{
		// Note, 'X' not implemented!
		A[0] = u[i-1];
		error = 0;
		matinfo((int *)&id,&v[i-1],A,X,&time,I,&error);
		assert(error==0);
	}
#ifdef	MDEBUG
	malloc_leave("StdMatProp::Eval");
#endif
}

int
StdMatProp::TanEval(double *v, const Const& df, LocalDomain& d,
	Element& e, const GPInfo&c) const
{
#ifdef	MDEBUG
	malloc_enter("StdMatProp::TanEval");
#endif
	int	i;

	if ((int)df != (int)dof)
	{
		foreach(i,c) v[i-1] = 0.0;
		return 0;
	}

	double	*u = new double[c.Size()];
	double	A[3], X[3];
	int	error;
	static	const	int	one = 1;
	double	time = d.Time();

	Recover1(d, dof, e, u, c);

	foreach(i,c)
	{
		// Note, 'X' not implemented!
		A[0] = u[i-1];
		error = 0;
		matdinfo((int *)&id,&v[i-1],(int *)&one,A,X,&time,I,&error);
		assert(error==0);
	}

	delete [] u;

#ifdef	MDEBUG
	malloc_leave("StdMatProp::TanEval");
#endif
	return 1;
}

int
StdMatProp::ParEval(double *v, int bn, LocalDomain& d,
	Element& e, const GPInfo &c) const
{
#ifdef	MDEBUG
	malloc_enter("StdMatProp::ParEval");
#endif
	double	*u = new double[c.Size()];
	double	A[3], X[3], time = d.Time();
	int	i, error, nbn=-bn;

	Recover1(d, dof, e, u, c);

	foreach(i,c)
	{
		// Note, 'X' not implemented!
		A[0] = u[i-1];
		error = 0;
		matdinfo((int *)&id,&v[i-1],&nbn,A,X,&time,I,&error);
		assert(error==0);
	}
	delete [] u;

#ifdef	MDEBUG
	malloc_leave("StdMatProp::ParEval");
#endif
	return 1;
}

ConstMatProp::ConstMatProp(double mp, const Const& d, int v)
	: p(mp), dof(d), dv(v)
{
}

void
ConstMatProp::Eval(double* v, LocalDomain& d,
	Element &e, const GPInfo & c) const
{
	int	i;

	foreach(i,c)
		v[i-1] = p;
}

int
ConstMatProp::ParEval(double *v, int bn, LocalDomain& d, Element &e,
					const GPInfo& c) const
{
	int	i;

	if (bn!=dv)
	{
		foreach(i,c)
			v[i-1] = 0.0;
		return 0;
	}

	foreach(i,c)
		v[i-1] = 1.0;
	return	1;
}

DesignMatProp::DesignMatProp(double v, const Const& d, DesignSpec &s)
	: dof(d), spec(s)
{
	dv = spec.Add(v);
}

void
DesignMatProp::Eval(double* v, LocalDomain& d,
	Element &e, const GPInfo& c) const
{
	int	i;

#if	0
	// The SubDomain version can't support this, I'm not sure what to
	// do about it.

	assert(&d.GetSpec()==&spec);	// Make sure this domain uses the
									// Same design variables
#endif

	foreach(i,c)
		v[i-1] = spec.Get(dv);
}

int
DesignMatProp::ParEval(double *v, int bn, LocalDomain& d, Element &e,
					const GPInfo& c) const
{
	int	i;

#if	0
	// The SubDomain version can't support this, I'm not sure what to
	// do about it.

	assert(&d.GetSpec()==&spec);	// Make sure this domain uses the
									// Same design variables
#endif

	if (bn!=dv)
	{
		foreach(i,c)
			v[i-1] = 0.0;
		return 0;
	}

	foreach(i,c)
		v[i-1] = 1.0;
	return	1;
}


