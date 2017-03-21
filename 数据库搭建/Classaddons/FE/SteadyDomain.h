#ifndef	_STEADY_DOMAIN_H_
#define	_STEADY_DOMAIN_H_
#include	"Domain.h"
#include	"Integrator.h"
#include	"Coupler.h"

// This class is really a hack (so is StdDomain).  Basically it provides
// a specific subclass that should really be provided by an application.

class	SteadyDomain : public StdLocalDomain
{
	protected:
		SteadyIntegrator	steady;
		UniCoupler	coupler;
		Assembler	*sys;

	public:
		SteadyDomain(Objective *o=NULL, DesignSpec *sp=NULL);
		Assembler	&Sys();

		// Related to SubDomains
		int		NumSubs();
		Domain	&Sub(int);

		void	NodalReport(const Vector &, char *, ostream&);
		void	Empty();

		// Solution scheme specific methods!
		int		GetDUDB(Vector&, int);
		int		Solve(int senses=0);

		~SteadyDomain();
};

#endif