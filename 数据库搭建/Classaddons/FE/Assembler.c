#include	<iostream.h>
#include	<BIMatrix.h>
#include	<Vector.h>
#include	"Assembler.h"
#include	"Domain.h"
#include	"Element.h"
#include	"Essential.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

Assembler::Assembler(LocalDomain &ld) : domain(ld)
{	
}

void
Assembler::Reform()
{
}

void
Assembler::TangentStiff(BIMatrix &b) const
{
}

void
Assembler::PartialStiff(BIMatrix &b, int bn) const
{
}

void
Assembler::PartialForce(Vector &f, int bn) const
{
}

SteadyAssembler::SteadyAssembler(LocalDomain &ld) : Assembler(ld)
{
	reform = 1;
}

void
SteadyAssembler::TanPreMult(const Vector& x , Vector& b)
{
	if (reform)
	{
		tan.Zero();
		TangentStiff(tan);
		reform = 0;
	}
	b = x*tan;
}

void
SteadyAssembler::TanPostMult(const Vector& x, Vector& b)
{
	if (reform)
	{
		tan.Zero();
		TangentStiff(tan);
		reform = 0;
	}
	b = tan*x;
}

void
SteadyAssembler::DistReduce(Vector& f, ReducType rt)
{
	if (reform)
	{
		tan.Zero();
		TangentStiff(tan);
		reform = 0;
	}
	LocalReduce(tan, f, rt);
}

void
SteadyAssembler::Tangent(BIMatrix &b)
{
	if (reform)	
	{
		tan.Zero();
		TangentStiff(tan);
		reform = 0;
	}
	b = tan;
}

void
SteadyAssembler::LocalReduce(BIMatrix &K, Vector& f, ReducType rt)
{
	int	i;
	Vector	off;
	const FixedSet<Essential *>	&bcs_set = domain.BCSSet();
	Indexer<Essential	*>	bcs(bcs_set);

	switch(rt)
	{
		case zero:
			for(i=1;i<=bcs.Size();i++)
				off += K.Eliminate(bcs[i]->Eq(),0.0);
			f+=off;
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = 0.0;
			break;
		case res:
			for(i=1;i<=bcs.Size();i++)
				off += K.Eliminate(bcs[i]->Eq(),bcs[i]->Val());
			f+=off;
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = bcs[i]->Val();
			break;
		case mat:
			for(i=1;i<=bcs.Size();i++)
				off += K.Eliminate(bcs[i]->Eq(),bcs[i]->Val());
			f+=off;
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = bcs[i]->Val();
			break;
		case rhsonly:
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = bcs[i]->Val();
			break;
		case rhszero:
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = 0.0;
			break;
		default:
			fprintf(stderr, "Error in SteadySystem::Reduce\n");
			break;
	}
}

int
SteadyAssembler::Num()
{
	return domain.NumEqs();
}

double
SteadyAssembler::Get(int n)
{
	return domain.GetSol(n);
}

void
SteadyAssembler::Set(int n, double v)
{
	domain.SetSol(n, v);
}

void
SteadyAssembler::Residual(Vector &r)
{
	BIMatrix	K;
	Vector		f, u;

	domain.GetSol(u);

	Stiff(K);
	Force(f);

	r = K*u-f;

#if	SHOW_ASSEMBLE
	printf("Before BCs\n");
	printf("Stiffness Matrix = \n");
	K.Print();	
	printf("Force Vector = \n");
	f.Print();	
	printf("Solution = \n");
	u.Print();	
	printf("Residual = \n");
	r.Print();	
#endif
}

void
SteadyAssembler::Update()
{
	Reform();
}

void
SteadyAssembler::Reform()
{
	reform = 1;
}

void
SteadyAssembler::Stiff(BIMatrix& b) const
{
	const	FixedSet<Element *>&	elems_set = domain.ElemSet();
	Indexer<Element *>	elems(elems_set);
	int	i, ne = elems.Size();

	for(i=1;i<=ne;i++)
		elems[i]->Stiff(b, domain);
}

void
SteadyAssembler::TangentStiff(BIMatrix& b) const
{
	const	FixedSet<Element *>&	elems_set = domain.ElemSet();
	Indexer<Element *>	elems(elems_set);
	int	i, ne = elems.Size();
	BIMatrix	m;

	for(i=1;i<=ne;i++)
	{
		m.Zero();
		elems[i]->TanStiff(m,domain);
		b+=m;
		m.Zero();
		elems[i]->TanForce(m,domain);
		b-=m;
		m.Zero();
		elems[i]->Stiff(m,domain);
		b+=m;
	}
}

void
SteadyAssembler::PartialStiff(BIMatrix& b, int bn) const
{
	const	FixedSet<Element *>&	elems_set = domain.ElemSet();
	Indexer<Element *>	elems(elems_set);
	int	i, ne = elems.Size();

	for(i=1;i<=ne;i++)
		elems[i]->ParStiff(b,domain,bn);
}

void
SteadyAssembler::PartialForce(Vector& f, int bn) const
{
	const	FixedSet<Element *>&	elems_set = domain.ElemSet();
	Indexer<Element *>	elems(elems_set);
	int	i, ne = elems.Size();
	SVector	sv;

	for(i=1;i<=ne;i++)
		elems[i]->ParForce(sv,domain,bn);

	f+=sv;
}

void
SteadyAssembler::Force(Vector& f) const
{
	const	FixedSet<Element *>&	elems_set = domain.ElemSet();
	Indexer<Element *>	elems(elems_set);
	int	i, ne = elems.Size();
	SVector	lv;

	for(i=1;i<=ne;i++)
	{
		lv.Empty();
		elems[i]->Force(lv,domain);
		f+=lv;
	}
}

