#ifndef	_GENSYSTEM_H_
#define	_GENSYSTEM_H_

#include	"System.h"
#include	"ResSystem.h"

class	DistSystem : public virtual ResSystem
{
	public:
		virtual void TanPreMult(const Vector&, Vector &) = 0;
		virtual void TanPostMult(const Vector&, Vector &) = 0;
		virtual	void DistReduce(Vector &, ReducType) = 0;
};

// An example of this could (will) be Newton-Raphson with Conjugate Gradient
class	DistSysSolver
{
	protected:
		DistSystem	*sys;
	public:
		DistSysSolver(DistSystem *s) { sys = s; }
		virtual	void	Solve(double, int) = 0;
};

class	GenSystem
	: public virtual ResSystem, public DistSystem, public LocalSystem
{
};

#endif
