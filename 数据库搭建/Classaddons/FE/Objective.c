#include	"SimObject.h"
#include	"Objective.h"
#include	"Design.h"
#include	<Vector.h>
#include	<math.h>
#include	<iostream.h>

void
Objective::SupplyTanObj(SimObject &,Vector &)
{
	cerr << "Error, Objective::SupplyTanObj called" << endl;
}

void
Objective::SupplyParObj(SimObject &, Vector &)
{
	cerr << "Error, Objective::SupplyParObj called" << endl;
}

void
NoObjective::Clear(SimObject&)
{
}

double
NoObjective::TimeStep(SimObject&, int senses)
{
	return	HUGE_VAL;
}

// This subroutine may be called multiple times!
double
NoObjective::GetObj(SimObject &so)
{
	return 0.0;
}

void
NoObjective::GetGrad(SimObject &so, Vector& dgdb)
{
	int	i;

	for(i=1;i<=so.GetSpec().Num();i++)
		dgdb[i] = 0.0;
}

void
NoObjective::SupplyTanObj(SimObject &d, Vector &v)
{
	int	i;

	for(i=1;i<=d.NumEqs();i++)
		v[i] = 0.0;
}

void
NoObjective::SupplyParObj(SimObject &d, Vector &v)
{
	int	i;

	for(i=1;i<=d.GetSpec().Num();i++)
		v[i] = 0.0;
}
