#ifndef	_DESIGN_H_
#define	_DESIGN_H_

class	DesignSpec
{
	public:
		virtual	void	Init() = 0;
		virtual	int		Num() = 0;
		virtual	double	Get(int) = 0;
		virtual	void	Set(int, double) = 0;
		virtual	int		Add(double) = 0;
};

#endif
