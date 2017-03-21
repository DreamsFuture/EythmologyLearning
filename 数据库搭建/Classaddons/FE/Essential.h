#ifndef	_ESSENTIAL_H_
#define	_ESSENTIAL_H_

class	DesignSpec;
class	Essential
{
	public:
		virtual	long	Eq() = 0;
		virtual	double	Val() = 0;
		virtual	double	ParVal(int bn) = 0;
};

class	ConstEssent : public Essential
{
	protected:
		long	eq;
		double	val;
	public:
		ConstEssent(long, double);
		long	Eq();
		double	Val();
		double	ParVal(int);
};

class	DesignEssent
{
	protected:
		long	eq;
		DesignSpec	&spec;
		int			dv;
	public:
		DesignEssent(long, DesignSpec&, int);
		long	Eq();
		double	Val();
		double	ParVal(int);
};

#endif