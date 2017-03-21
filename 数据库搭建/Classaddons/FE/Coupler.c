#include	<assert.h>
#include	<Vector.h>
#include	<BIMatrix.h>
#include	"Coupler.h"

const FixedSet<GenSystem *>&
StdCoupler::Systems()
{
	return	systems;
}

UniCoupler::UniCoupler()
{
	systems[1] = NULL;
}

void
UniCoupler::SetSystem(GenSystem *sys)
{
	systems[1] = sys;
}

void
UniCoupler::Tangent(BIMatrix &b)
{
	assert(systems[1]!=NULL);
	systems[1]->Tangent(b);
}

void
UniCoupler::TanPreMult(const Vector &v, Vector &r)
{
	assert(systems[1]!=NULL);
	systems[1]->TanPreMult(v,r);
}

void
UniCoupler::TanPostMult(const Vector &v, Vector &r)
{
	assert(systems[1]!=NULL);
	systems[1]->TanPreMult(v,r);
}

int
UniCoupler::Num()
{
	assert(systems[1]!=NULL);
	return systems[1]->Num();
}

double
UniCoupler::Get(int n)
{
	assert(systems[1]!=NULL);
	return systems[1]->Get(n);
}

void
UniCoupler::Set(int n, double v)
{
	assert(systems[1]!=NULL);
	systems[1]->Set(n, v);
}

void
UniCoupler::Residual(Vector &v)
{
	assert(systems[1]!=NULL);
	systems[1]->Residual(v);
}

void
UniCoupler::LocalReduce(BIMatrix &b, Vector &v, ReducType t)
{
	assert(systems[1]!=NULL);
	systems[1]->LocalReduce(b,v,t);
}

void
UniCoupler::DistReduce(Vector &v, ReducType t)
{
	assert(systems[1]!=NULL);
	systems[1]->DistReduce(v,t);
}

MultiCoupler::MultiCoupler(int nd, int neq) : map(nd)
{
	int	i;

	for(i=1;i<=nd;i++)
		systems[i] = NULL;
	num = neq;
}

void
MultiCoupler::SetSystem(int n, GenSystem *sys)
{
	assert(n>=1 && n<=systems.Size());
	systems[n] = sys;
}

void
MultiCoupler::Tangent(BIMatrix &b)
{
	int	i, j, k;

	foreach(i,systems)
	{
		BIMatrix	tmp;
		systems[i]->Tangent(tmp);

		// Have to remap columns here
		for(j=1;j<=systems[i]->Num();j++)
		{
#ifdef	USE_SET
			AssocSet<int,double> &array = tmp[j];
#else
			AArray<int,double>	&array = tmp[j];
#endif

			for(k=1;k<=array.Size();k++)
			{
				int	tmp;
				tmp = array.GetKey(k);
				array.GetKey(k) = map.Eq(i,tmp);
			}
		}

		// Remapping rows
		for(j=1;j<=systems[i]->Num();j++)
			for(k=1;k<=tmp[j].Size();k++)
				b[map.Eq(i,j)][map.Eq(i,tmp[j].GetKey(k))]+=
					tmp[j][tmp[j].GetKey(k)];
	}
}

void
MultiCoupler::TanPreMult(const Vector &v, Vector &r)
{
	int	i, j;

	foreach(i,systems)
	{
		SimpleSet<int>	mapping;
		Vector	lv, lr;

		for(j=1;j<=systems[i]->Num();j++)
		{
			mapping[j] = map.Eq(i,j);
			lv[j] = v[mapping[j]];
		}

		systems[i]->TanPreMult(lv,lr);

		for(j=1;j<=systems[i]->Num();j++)
			r[mapping[j]] = lr[j];
	}
}

void
MultiCoupler::TanPostMult(const Vector &v, Vector &r)
{
	int	i, j;

	foreach(i,systems)
	{
		SimpleSet<int>	mapping;
		Vector	lv, lr;

		for(j=1;j<=systems[i]->Num();j++)
		{
			mapping[j] = map.Eq(i,j);
			lv[j] = v[mapping[j]];
		}

		systems[i]->TanPostMult(lv,lr);

		for(j=1;j<=systems[i]->Num();j++)
			r[mapping[j]] = lr[j];
	}
}

int
MultiCoupler::Num()
{
	return num;
}

double
MultiCoupler::Get(int n)
{
	SimpleSet<DomainRef>	ref;
	int	i;

	map.Inverse(n, ref);
	assert(ref.Size()>=1);

	// return first one
	return systems[ref[1].dom]->Get(ref[1].eq);
}

void
MultiCoupler::Set(int n, double v)
{
	SimpleSet<DomainRef>	ref;
	int	i;

	map.Inverse(n, ref);
	assert(ref.Size()>=1);

	foreach(i,ref)
		systems[ref[i].dom]->Set(ref[i].eq, v);
}

void
MultiCoupler::Residual(Vector &v)
{
	int	i, j;

	foreach(i,systems)
	{
		Vector	r;
		assert(systems[i]!=NULL);
		systems[i]->Residual(r);
		for(j=1;j<=systems[i]->Num();j++)
			v[map.Eq(i,j)] = r[j];
	}
}

void
MultiCoupler::LocalReduce(BIMatrix &b, Vector &v, ReducType t)
{
	systems[1]->LocalReduce(b,v,t);
}

void
MultiCoupler::DistReduce(Vector &v, ReducType t)
{
	systems[1]->DistReduce(v,t);
}

