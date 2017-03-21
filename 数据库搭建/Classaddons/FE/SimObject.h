#ifndef	_SIMOBJECT_H_
#define	_SIMOBJECT_H_

class	DesignSpec;
class	Objective;
class	Vector;

class	SimObject
{
	public:
		virtual	DesignSpec	&GetSpec() = 0;
		virtual	Objective	&GetObj() = 0;
		virtual	double		Time() const = 0;
		virtual	int			NumNodes() const = 0;
		virtual	int			NumEqs() const = 0;
		virtual	int			NumNodalDOFS(int) const = 0;
		virtual	int			NodalDOF(int,int) const = 0;
		virtual	void		NodalCoords(int, double &, double &, double &) = 0;
		virtual	long		EqNum(int,short) const = 0;

		virtual	double		GetSol(int) const = 0;
		virtual	void		SetSol(int, double) = 0;
		virtual	void		GetSol(Vector&) const;

		// This method in most cases will do a direct differentiation
		virtual	int			GetDUDB(Vector&, int bn);

		// This method in most cases will do an adjoint analysis
		virtual	int			GetDGDB(Vector&);

		virtual	~SimObject() { }
};

#endif
