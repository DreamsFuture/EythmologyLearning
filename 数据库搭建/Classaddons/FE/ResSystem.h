#ifndef	_RESSYSTEM_H_
#define	_RESSYSTEM_H_

#include	"System.h"

class	BIMatrix;
class	Vector;
class	StdDomain;

class	ResSystem : public virtual System
{
	public:
		virtual void Residual(Vector&) = 0;
};

class	LocalSystem : public virtual ResSystem
{
	public:
		virtual	void Tangent(BIMatrix&) = 0;
		virtual	void LocalReduce(BIMatrix&, Vector &, ReducType) = 0;
};

class	LocalSysSolver
{
	protected:
		LocalSystem	*sys;
	public:
		LocalSysSolver(LocalSystem *s) { sys = s; }
		virtual	void	Solve(double, int) = 0;
};

class	NewtonRaphson : public LocalSysSolver
{
	public:
		NewtonRaphson(LocalSystem *s) : LocalSysSolver(s) { }
		void	Solve(double, int);
};

#if	0
// I think this guy is going to go...
class	StdDomainSystem : public virtual System, public LocalSystem
{
	protected:
		StdDomain	&domain;

		void	GetDRDB(Vector&, int);
	public:
		StdDomainSystem(StdDomain&);
		int		Num();
		double	Get(int);
		void	Set(int, double);
		void	Residual(Vector&);
		void	Tangent(BIMatrix&);
		void	GetDUDB(Vector &, int);
//		void	GetDGDB(Vector &);
		void	Update();
		void	Reduce(BIMatrix&, const Vector&, ReducType);
};
#endif

#endif