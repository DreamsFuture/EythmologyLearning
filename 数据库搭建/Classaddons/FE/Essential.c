#include	"Essential.h"
#include	"Design.h"

ConstEssent::ConstEssent(long e, double v)
{
	eq = e;
	val = v;
}

long
ConstEssent::Eq()
{
	return eq;
}

double
ConstEssent::Val()
{
	return val;
}

double
ConstEssent::ParVal(int bn)
{
	return 0.0;
}

DesignEssent::DesignEssent(long e, DesignSpec &s, int n)
	: eq(e), spec(s), dv(n)
{
}

long
DesignEssent::Eq()
{
	return eq;
}

double
DesignEssent::Val()
{
	return spec.Get(dv);
}

double
DesignEssent::ParVal(int n)
{
	if (n==dv)
		return 1.0;
	else
		return 0.0;
}
