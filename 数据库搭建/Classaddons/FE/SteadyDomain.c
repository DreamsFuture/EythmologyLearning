#include	<string.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<BIMatrix.h>
#include	"StdLocalDomain.h"
#include	"SteadyDomain.h"
#include	"Element.h"
#include	"Design.h"
#include	"Objective.h"
#include	"Essential.h"
#include	"Const.h"
#include	"Node.h"
#include	"Integrator.h"
#include	"Assembler.h"

SteadyDomain::SteadyDomain(Objective *o, DesignSpec *s)
	: StdLocalDomain(steady, o, s), steady(coupler)
{
	sys = steady.CreateAssembler(*this);
	coupler.SetSystem(sys);
}

Assembler&
SteadyDomain::Sys()
{
	return	*sys;
}

int
SteadyDomain::NumSubs()
{
	return 1;
}

Domain&
SteadyDomain::Sub(int i)
{
	assert(i==1);
	return *this;
}

void
SteadyDomain::NodalReport(const Vector &v, char *quantity, ostream& o)
{
	if (v.Size() != last.Size())
		fprintf(stderr, "Size mismatch in Domain::NodalReport\n");
	int	i, j, nnd;
	short dof=0;
	long	eq=0;

	o << "Node\tDOF\t" << quantity << endl;

	for(i=1;i<=NumNodes();i++)
	{
		nnd = NumNodalDOFS(i);
		for(j=1;j<=nnd;j++)
		{
			dof = NodalDOF(i,j);
			eq = EqNum(i,dof);
			o << i << '\t' << dof << '\t' << v[eq] << endl;
		}
	}
}

void
SteadyDomain::Empty()
{
	int	i;

	foreach(i,nodes)
		delete nodes[i];
	foreach(i,elems)
		delete elems[i];
	foreach(i,bcs)
		delete bcs[i];
}

int
SteadyDomain::Solve(int senses)
{
	spec.Init();
	obj.Clear(*this);

	NewtonRaphson	nr(sys);
#ifdef	MDEBUG
	malloc_enter("SteadyDomain::Solve");
#endif
	nr.Solve(.001, 25);
#ifdef	MDEBUG
	malloc_leave("SteadyDomain::Solve");
#endif
	obj.TimeStep(*this, senses);

	return	1;
}

int
SteadyDomain::GetDUDB(Vector &du, int bn)		// Direct Differentiation
{
	cerr << "Warning, functionality not implemented" << endl;
//	sys->GetDUDB(du, bn);
	return 1;
}

SteadyDomain::~SteadyDomain()
{
}

