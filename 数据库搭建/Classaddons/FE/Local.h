#ifndef	_LOCAL_H_
#define	_LOCAL_H_

#ifdef	__GNUC__
#pragma interface
#endif

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */


/*
 * $Id: Local.h,v 3.4 1993/06/10 20:59:29 tiller Exp tiller $
 */


// The LocVec and LocMat stuff has taken on a life of its own.  I hope
// to eventually get rid of this stuff for optimization reasons, we'll
// see how it goes.  For now this stuff is begin taken out of Element.h


#include	"Node.h"
#if	0
#include	<Gen/Array.h>
#endif
#include	<Set/SimpleSet.h>
#include	<Scalar/Scalar.h>
#include	<BIMatrix.h>

struct	LocVecData
{
	long	eq;
	Scalar	val;

	LocVecData(long e, const Scalar& v);
	LocVecData(const LocVecData&);
	LocVecData();
	~LocVecData() { }

	int	operator<(const LocVecData &);
	int	operator>(const LocVecData &);
};

class LocVec
{
	protected:
		SimpleSet<LocVecData>	data;
	public:
		LocVec() { }
		LocVec(const LocVec& lv);
		LocVec& operator=(const LocVec& lv);
		virtual ~LocVec() { Empty(); }

		int	Length() const { return data.Size(); }
		LocVecData& operator()(int);
		const LocVecData& operator()(int) const;
		Scalar&	Add(long eq);
		void Deriv(LocVec&, const Scalar &) const;
		void Print() const;
		void operator+=(const LocVec&);
		Scalar operator[](long) const;

		void Sort();
		void Squeeze();
		void Empty();
		void Remap(const FixedSet<long>&);
		int	IsEmpty() { return data.Size(); }
		friend class LocMat;
};

struct	LocMatData
{
	long	eq1, eq2;
	Scalar	val;

	LocMatData(long e1, long e2, const Scalar& v);
	LocMatData(const LocMatData&);
	LocMatData();
	~LocMatData() { }

	// Sorting stuff
	enum	SortBy { RowIndex, ColIndex };
	static	SortBy	sort_by;
	int	operator<(const LocMatData &rhs);
	int	operator>(const LocMatData &rhs);
};

class LocMat
{
	protected:
		SimpleSet<LocMatData>	data;
	public:
		LocMat() { }
		LocMat(const LocMat& lv);
		LocMat& operator=(const LocMat& lv);
		virtual ~LocMat() { Empty(); }

		int	Length() const { return data.Size(); }
		LocMatData& operator()(int);
		const LocMatData & operator()(int) const;
		Scalar&	Add(long eq1, long eq2);
		void AddRow(long eq, const LocVec& v);
		void AddCol(long eq, const LocVec& v);
		void Transpose(LocMat& lm) const;
		void Deriv(LocMat&, const Scalar &) const;
		void operator+=(const LocMat &);

		void Sort();
		void Squeeze();
		void Empty();
		int	IsEmpty() { return data.Size(); }
		void Remap(const FixedSet<long>&);
		void Print() const;
		friend class LocVec;
};

void operator+=(BIMatrix&, const LocMat&);
void operator-=(BIMatrix&, const LocMat&);

void operator+=(Vector&, const LocVec&);

void operator*=(LocMat& m, const Scalar& s);
void operator*=(LocVec& f, const Scalar& s);

LocMat operator*(const LocMat& m, const Scalar& s);
LocMat operator*(const Scalar& s, const LocMat& m);

LocVec operator*(const LocVec& v, const Scalar& s);
LocVec operator*(const Scalar& s, const LocVec& v);

LocVec operator*(const LocMat& m, const ScVector& v);
LocVec operator*(const ScVector& v, const LocMat& v);
LocVec operator*(const LocMat& m, const LocVec& v);
LocVec operator*(const LocVec& v, const LocMat& v);
LocMat operator*(const LocMat& a, const LocMat& b);

Scalar operator*(const LocVec& a, const LocVec& b);
LocMat LocOuterProd(const LocVec& a, const LocVec& b);

#endif