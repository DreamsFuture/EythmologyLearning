#ifndef	_OBJECTIVE_H_
#define	_OBJECTIVE_H_

class	Vector;
class	Domain;
class	SimObject;

//
//	NOTE: All mention of Domain will probably have to be eliminated
//

class	Objective
{
	public:
		//
		// Clear : Clears sensitivity information
		// TimeStep : Calculates objective at current time and returns
		//            next mesh time
		// GetObj : Returns G and dGdb at end of simulation
		//
		virtual	void	Clear(SimObject&) = 0;
		virtual	double	TimeStep(SimObject&, int senses) = 0;

		// This subroutine may be called multiple times
		virtual	double	GetObj(SimObject&) = 0;
		virtual	void	GetGrad(SimObject&,Vector&) = 0;

		// The following routines are provided for misc funkiness
		// They will probably never be used.  I have in mind occasions
		// where you need to adjointish stuff or second order
		// sensitivities
		virtual	void	SupplyTanObj(SimObject&,Vector&);
		virtual	void	SupplyParObj(SimObject&,Vector&);
		virtual ~Objective() { }
};

class	NoObjective : public Objective
{
	public:
		NoObjective() { }

		void	Clear(SimObject&);
		double	TimeStep(SimObject&, int senses);
		double	GetObj(SimObject&);
		void	GetGrad(SimObject&,Vector&);
		void	SupplyTanObj(SimObject&,Vector&);
		void	SupplyParObj(SimObject&,Vector&);
};

#endif
