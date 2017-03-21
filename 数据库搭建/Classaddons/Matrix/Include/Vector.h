/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: Vector.h,v 1.5 1993/06/10 13:59:00 tiller Exp $
 * ChangeLog:
 *	$Log: Vector.h,v $
 * Revision 1.5  1993/06/10  13:59:00  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 1.5  1993/06/10  13:59:00  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 1.4  1993/05/11  15:42:32  tiller
 * Added support for tracking memory leaks.  Everything in this versions
 * looks good.
 *
 * Revision 1.4  1993/05/11  15:42:32  tiller
 * Added support for tracking memory leaks.  Everything in this versions
 * looks good.
 *
 * Revision 1.3  1993/03/07  17:36:43  tiller
 * Various Changes to get Non-Linear Optimization homework working.
 *
 * Revision 1.3  1993/03/07  17:36:43  tiller
 * Various Changes to get Non-Linear Optimization homework working.
 *
 * Revision 1.2  1993/02/20  18:59:28  tiller
 * Added RCS header info
 *
 */

#ifndef	_VECTOR_H_
#define	_VECTOR_H_
#ifdef	__GNUC__
#pragma interface
#endif

#include	<Misc/GC.h>
#include	<iostream.h>
#include	<math.h>

class   Vector
{
	protected:
		int size;
		double	eps;
		inline	void	AddTo(int i, double val)
		{
			if (size<i)
				SetSize(i);

			v[i-1] += val;
		}
		inline	void	SetTo(int i, double val)
		{
			if (size<i)
				SetSize(i);

			v[i-1] = val;
		}

	public:
		Vector();
		Vector(double *b, int s);
		Vector(const Vector& V2);
		void	SetSize(int);
		inline	int	Size() const { return(size); }
		void Free();
		~Vector();
		void	Load(double *b, int s);
		Vector&	operator=(const Vector& V2);
		inline  double&  operator[](int i)	// Read-Write
		{
			if (i>size)
				SetTo(i, 0.0);

			return(v[i-1]);
		}
		const double&  operator[](int i) const;	// Read-Only
		double	dot(const Vector &b) const;
		inline	double	norm() const
		{
			return(sqrt(dot(*this)));	
		}
		Vector operator+(const Vector &b);
		Vector operator-(const Vector &b);
		void operator+=(const Vector &b);
		void operator-=(const Vector &b);
		void	Print();
		double operator*(Vector&);
		double operator!();
		Vector operator-();
	private:
		double  *v;
};

Vector operator*(double a, Vector& b);
inline	Vector operator*(Vector &b, double a)
{
	return(a*b);
}
inline	Vector operator/(double a, Vector& b)
{
	return((1/a)*b);
}
inline	Vector operator/(Vector &b, double a)
{
	return((1/a)*b);
}

Vector	ReadVectorFromFile(istream &);
#endif
