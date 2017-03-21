#include	<assert.h>
#include	<stdlib.h>
#include	"ScElement.h"
#include	"Domain.h"

void
ScElement::DStiff(LocMat& ret, Domain& d, const ScVector& last)
{
	LocMat	m, t;
	int	i;

	Stiff(m,d,last);
	for(i=1;i<=last.Size();i++)
	{
		t.Empty();
		m.Deriv(t, last[i]);
		ret.AddCol(i,t*last);
	}
}

void
ScElement::DStiff(LocMat& ret, Domain& d, const ScVector& last, int num)
{
	LocMat	m;
	int	i;
	ScVector	design;

	for(i=1;i<=d.GetSpec().Num();i++)
		design[i] = d.GetSpec().Get(i);

	Stiff(m,d,last);
	ret.Empty();
	m.Deriv(ret, design[num]);
}

void
ScElement::DForce(LocMat& ret, Domain& d, const ScVector& last)
{
	LocVec	f;
	LocVec	t;
	long	i;

	Force(f,d,last);
	ret.Empty();
	for(i=1;i<=last.Size();i++)
	{
		f.Deriv(t, last[i]);
		ret.AddRow(i, t);
		t.Empty();
	}
}

void
ScElement::DForce(LocVec& ret, Domain& d, const ScVector& last, int num)
{
	LocVec	f;
	int	i;
	ScVector	design;

	for(i=1;i<=d.GetSpec().Num();i++)
		design[i] = d.GetSpec().Get(i);

	Force(f,d,last);
	ret.Empty();
	f.Deriv(ret, design[num]);
}

void
ScElement::Stiff(BIMatrix& b, Domain &d)
{
	LocMat	m;
	ScVector	last;
	Vector		dlast;
	int	i;

	d.GetSol(dlast);
	for(i=1;i<=dlast.Size();i++)
		last[i] = dlast[i];
	Stiff(m,d,last);
	b+=m;
}

void
ScElement::TanStiff(BIMatrix &b, Domain &d)
{
	LocMat	m;
	ScVector	sv;
	Vector	dlast;
	int	i;

	d.GetSol(dlast);
	for(i=1;i<=dlast.Size();i++)
		sv[i]=dlast[i];		// This is really a hack and a performance hog!

	DStiff(m,d,sv);
	b+=m;
}

void
ScElement::ParStiff(BIMatrix &b, Domain &d, int bn)
{
	LocMat	m;
	ScVector	last;
	Vector	dlast;
	int	i;

	d.GetSol(dlast);
	for(i=1;i<=dlast.Size();i++)
		last[i] = dlast[i];

	DStiff(m,d,last,bn);
	b+=m;
}

void
ScElement::Force(SVector &sv, Domain& d)
{
	Vector	v;
	LocVec	lv;
	ScVector	last;
	Vector	dlast;
	int	i;

	d.GetSol(dlast);
	for(i=1;i<=dlast.Size();i++)
		last[i]=dlast[i];
	Force(lv,d,last);
	v+=lv;
	sv+=v;  // Is this ugly or what!
}

void
ScElement::TanForce(BIMatrix& b, Domain &d)
{
	LocMat	m;
	ScVector	sv;
	Vector	dlast;
	int	i;

	d.GetSol(dlast);
	for(i=1;i<=dlast.Size();i++)
		sv[i]=dlast[i];		// This is really a hack and a performance hog!

	DForce(m,d,sv);
	b+=m;
}

void
ScElement::ParForce(SVector &sv, Domain &d, int bn)
{
	LocVec	lv;
	Vector	v;
	ScVector	last;
	Vector	dlast;
	int	i;

	d.GetSol(dlast);
	for(i=1;i<=dlast.Size();i++)
		last[i] = dlast[i];

	DForce(lv, d, last, bn);
	v+=lv;
	sv+=v;	// Ho! is this ugly...man!
}
