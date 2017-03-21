#ifndef	_VECSPEC_H_
#define	_VECSPEC_H_

#include	<Vector.h>

#include	"Design.h"

class	VecSpec : public DesignSpec
{
	protected:
		Vector	design;
	public:
		VecSpec() { }
		void	Init() { }
		int		Num();
		double	Get(int);
		void	Set(int, double);
		int		Add(double);
};

#endif
