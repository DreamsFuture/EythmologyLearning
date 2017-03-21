#ifndef	_DOMAIN_OPT_H_
#define	_DOMAIN_OPT_H_

#include	<Opt/Opt.h>

class	Domain;

class	DomainOpt : public Optimizable
{
	protected:
		Domain	&domain;
	public:
		DomainOpt(Domain&);
		int	NumVars() const;
		double	Function(DesignVec);
		void	Gradient(DesignVec, GradVec);
};

#endif
