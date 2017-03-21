#include	"Integrator.h"
#include	"Assembler.h"
#include	<math.h>

int
Integrator::PerformStep(Domain &d, double &dt)
{
	return PerformStep(d,dt,HUGE_VAL);
}

SteadyIntegrator::SteadyIntegrator(GenSystem& sys) : system(sys)
{
}

Assembler *
SteadyIntegrator::CreateAssembler(LocalDomain &ld) const
{
	return new SteadyAssembler(ld);
}

GenSystem &
SteadyIntegrator::Sys()
{
	return system;
}

int
SteadyIntegrator::PerformStep(Domain &d, double &dt, double tlimit)
{
	dt = HUGE_VAL;	// As if it made a difference
	return 1;
}

