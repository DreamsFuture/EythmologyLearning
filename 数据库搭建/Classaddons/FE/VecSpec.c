#include	"VecSpec.h"
#include	<assert.h>

int
VecSpec::Num()
{
	return design.Size();
}

double
VecSpec::Get(int n)
{
	assert(n>0 && n<=design.Size());
	return design[n];
}

void
VecSpec::Set(int n, double v)
{
	assert(n>0 && n<=design.Size());
	design[n] = v;
}

int
VecSpec::Add(double v)
{
	design[design.Size()+1] = v;
	return design.Size();
}
