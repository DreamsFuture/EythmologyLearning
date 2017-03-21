#ifndef	_SLDOMAIN_H_
#define	_SLDOMAIN_H_

#include	"Domain.h"

class	StdLocalDomain : public LocalDomain
{
	protected:
		SimpleSet<Node *>		nodes;	// For LocalDomain interface
		SimpleSet<Element *>	elems;	// For LocalDomain interface
		SimpleSet<Essential *>	bcs;	// For LocalDomain interface
		int		numDOFS;

		// This bit was copied from StdDomain.  I could have used virtual
		// multiple inheritance to avoid copying, but it didn't seem worth
		// it.
		VecSpec	desvec;					// For 'simple spec' case
		static	NoObjective	no_obj;		// For 'no obj' case
		double	prev, time;				// For Domain interface
		Vector			last;			// For SimObject interface
		DesignSpec	&spec;				// For SimObject interface
		Objective	&obj;				// For SimObject interface

	public:
		StdLocalDomain(Integrator &, Objective *o=NULL, DesignSpec *sp=NULL);

		// The following implement the Domain interface
		int		Solve(int senses=0);
		void	NodalReport(const Vector &, char *, ostream&);
		void	SetTime(double);

		// The following complete the SimObject interface
		int		NumNodes() const;
		int		NumEqs() const;
		int		NumNodalDOFS(int) const;
		int		NodalDOF(int,int) const;
		long	EqNum(int,short) const;

		// The following implement the LocalDomain interface
		void	AddNode(Node *);
		void	AddElem(Element *);
		void	AddBC(Essential *);
		void	AddDOF(Node *,short);
		const	FixedSet<Node *>& NodeSet() const;
		const	FixedSet<Element *>& ElemSet() const;
		const	FixedSet<Essential *>& BCSSet() const;

		// The following implement the SimObject interface
		// This was also copied from StdDomain.
		DesignSpec	&GetSpec();
		Objective	&GetObj();
		double	Time() const;
		double	GetSol(int) const;
		void	SetSol(int, double);
		void	GetSol(Vector &) const;
		void	NodalCoords(int n, double &x, double &y, double &z);

		~StdLocalDomain();
};

#endif