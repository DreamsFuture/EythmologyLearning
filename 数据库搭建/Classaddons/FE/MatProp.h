#ifndef	_MAT_PROP_
#define	_MAT_PROP_

class	Domain;
class	LocalDomain;
class	Element;
class	Const;
class	GPInfo;
class	DesignSpec;
class	GPInfo;

extern "C"
{
	void matinfo(int *, double *, double *, double *, double *, int *, int *);
	void matdinfo(int *, double *, int *, double *,
		double *, double *, int *, int *);
}

void Recover1(LocalDomain &, const Const&,
	Element &, double *, const GPInfo &c);

class	MatProp
{
	public:
		virtual	int	IsConst() const { return 0; }
		virtual	const Const& DOF() const = 0;
		virtual	void Eval(double *, LocalDomain&,
							Element&, const GPInfo&) const = 0;

		virtual int TanEval(double *, const Const&,
							LocalDomain&, Element&, const GPInfo&) const;
		virtual int ParEval(double *, int bn,
							LocalDomain&, Element&, const GPInfo&) const;
		virtual	~MatProp() { }
};

class	StdMatProp : public MatProp
{
	protected:
		int	id;
		int	I[3];
		const	Const&	dof;
	public:
		StdMatProp(int type, const Const& d, int i1=-1, int i2=-1, int i3=-1);
		const Const& DOF() const { return dof; }
		void Eval(double *, LocalDomain&, Element&, const GPInfo&) const;
		int TanEval(double *, const Const&,
							LocalDomain&, Element&, const GPInfo&) const;
		int ParEval(double *, int bn,
							LocalDomain&, Element&, const GPInfo&) const;
};

class	ConstMatProp : public MatProp
{
	protected:
		int	dv;
		double	p;
		const	Const&	dof;
	public:
		ConstMatProp(double mp, const Const& d, int v=-1);
		const Const& DOF() const { return dof; }
		int	IsConst() const { return 1; }
		void Eval(double *, LocalDomain&, Element&, const GPInfo&) const;
		int ParEval(double *, int bn,
							LocalDomain&, Element&, const GPInfo&) const;
};

class	DesignMatProp : public MatProp
{
	protected:
		int	dv;
		const	Const&	dof;
		DesignSpec	&spec;		
	public:
		DesignMatProp(double v, const Const& d, DesignSpec &s);
		const Const& DOF() const { return dof; }
		void Eval(double *, LocalDomain&, Element&, const GPInfo&) const;
		int ParEval(double *, int bn,
							LocalDomain&, Element&, const GPInfo&) const;
};

#endif
