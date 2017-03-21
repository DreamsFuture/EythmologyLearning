#ifndef	_INTEGRATOR_H_
#define	_INTEGRATOR_H_

#include	<math.h>
#include	"GenSystem.h"

class	Assembler;
class	LocalDomain;

class	Integrator
{
	public:
		virtual Assembler *CreateAssembler(LocalDomain &ld) const = 0;
		virtual	GenSystem &Sys() = 0;

		// Integration part...given a domain and a meshing time, returns
		// the time step used (by reference) and whether integration is
		// complete.
		virtual	int PerformStep(Domain&, double&, double tlimit) = 0;
		virtual	int PerformStep(Domain&, double&);
};

class	SteadyIntegrator : public Integrator
{
	protected:
		GenSystem	&system;
	public:
		SteadyIntegrator(GenSystem& sys);
		Assembler *CreateAssembler(LocalDomain &ld) const;
		GenSystem &Sys();
		int PerformStep(Domain&, double&, double tlimit);
};

#endif