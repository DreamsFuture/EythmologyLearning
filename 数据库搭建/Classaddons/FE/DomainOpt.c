#include	"DomainOpt.h"
#include	"Domain.h"
#include	<Vector.h>


DomainOpt::DomainOpt(Domain &dom) : domain(dom)
{
}

int
DomainOpt::NumVars() const
{
	return domain.GetSpec().Num();
}

double
DomainOpt::Function(DesignVec x)
{
	int	i;

	for(i=1;i<=domain.GetSpec().Num();i++)
		domain.GetSpec().Set(i,x[i-1]);

	cout << "-->Solving System for Function" << endl;
	domain.Solve(0);
	cout << "-->done" << endl;
	return domain.GetObj().GetObj(domain);
}

void
DomainOpt::Gradient(DesignVec x, GradVec df)
{
	Vector	grad;
	int	i;

	for(i=1;i<=domain.GetSpec().Num();i++)
		domain.GetSpec().Set(i,x[i-1]);

	cout << "-->Solving System for Gradient" << endl;
	domain.Solve(1);
	cout << "-->done" << endl;
	domain.GetObj().GetGrad(domain,grad);

	for(i=1;i<=domain.GetSpec().Num();i++)
		df[i-1] = grad[i];
}
