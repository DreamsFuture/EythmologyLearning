#ifndef	_CONST_H_
#define	_CONST_H_

class	Const
{
	protected:
		static	int	count;
		int	value;
	public:
		Const() { value = count; count++; }
		inline	operator int() const { return value; }
		inline	int	Value() const { return value; }
};

#endif
