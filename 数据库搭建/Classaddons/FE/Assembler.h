#ifndef	_ASSEMBLER_H_
#define	_ASSEMBLER_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Assembler.h,v 1.3 1993/06/10 13:58:10 tiller Exp $
 */

#include	<BIMatrix.h>
#include	"GenSystem.h"

class	LocalDomain;

//
//	Assemblers are weird...you should never be able to copy one (accomplished
// by making the copy and default operations private) and you should only
// be able to create one by querying the appropriate Integrator (accomplished
// by using protected constructors and friendship declarations).
//

class	Assembler : public GenSystem
{
	private:
		Assembler();
		Assembler(const Assembler &);
		Assembler& operator=(const Assembler &);
	protected:
		LocalDomain&	domain;
		Assembler(LocalDomain& ld);

	public:
		// tells domain to reform tangent matrix on next request
		virtual	void 	Reform() = 0;
		virtual	void	Stiff(BIMatrix&) const = 0;
		virtual	void	TangentStiff(BIMatrix&) const;
		virtual	void	PartialStiff(BIMatrix&, int) const;
		virtual	void	PartialForce(Vector&, int) const;
		virtual	void	Force(Vector&) const = 0;

};

class	SteadyAssembler : public Assembler
{
	private:
		SteadyAssembler();
		SteadyAssembler(const SteadyAssembler &);
		SteadyAssembler& operator=(const SteadyAssembler &);
	protected:
		SteadyAssembler(LocalDomain& ld);
		BIMatrix	tan;
		int	reform;

	public:

		// To satisfy the DistSystem interface
		void TanPreMult(const Vector&, Vector&);
		void TanPostMult(const Vector&, Vector&);
		void DistReduce(Vector &, ReducType);

		// To satisfy the LocalSystem interface
		void Tangent(BIMatrix&);
		void LocalReduce(BIMatrix&, Vector &, ReducType);

		// To satisfy the System and ResSystem interfaces
		int		Num();
		double	Get(int);
		void	Set(int, double);
		void	Residual(Vector&);
		void	Update();

		// To satisfy Assembler interface
		void	Reform();
		void	Stiff(BIMatrix&) const;
		void	TangentStiff(BIMatrix&) const;
		void	PartialStiff(BIMatrix&, int) const;
		void	PartialForce(Vector&, int) const;
		void	Force(Vector&) const;

		friend class SteadyIntegrator;
};

#endif
