#include	<stdlib.h>
#include	<assert.h>
#include	"Local.h"
#include	"Domain.h"

#ifdef	__GNUC__
#pragma implementation
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
 * $Id: Local.c,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */


LocVecData::LocVecData(long e, const Scalar &v)
{
	eq = e;
	val = v;
}

LocVecData::LocVecData()
{
	eq = -1;
	val = 0.0;
}

LocVecData::LocVecData(const LocVecData& lvd)
{
	eq = lvd.eq;
	val = lvd.val;
}

LocVec::LocVec(const LocVec& lv)
{
	int	i;

	foreach(i,lv.data);
		data+=lv.data[i];
}


int
LocVecData::operator<(const LocVecData &rhs)
{
	if (eq < rhs.eq)
		return 1;
	return 0;
}

int
LocVecData::operator>(const LocVecData &rhs)
{
	if (eq > rhs.eq)
		return 1;
	return 0;
}

LocVec&
LocVec::operator=(const LocVec& lv)
{
	int	i;

	data.Empty();
	foreach(i,lv.data)
		data+=lv.data[i];
	return *this;
}

Scalar
LocVec::operator[](long eq) const
{
	Scalar	ret;
	int	i;

	foreach(i,data)
		if (data[i].eq==eq)
			ret+=data[i].val;

	return	ret;
}

Scalar&
LocVec::Add(long eq)
{
	data+=LocVecData(eq, 0.0);
	return data.Last().val;
}

void
LocVec::Remap(const FixedSet<long>& map)
{
	int		i;
	long	j;

	foreach(i,data)
	{
		map.Get((int)data[i].eq, j);
		data[i].eq = j;
	}
}

void
LocVec::Empty()
{
	data.Empty();
}

void
LocVec::Deriv(LocVec& f, const Scalar &e) const
{
	int	i;

	foreach(i,data)
	{
		Scalar	expr(data[i].val.D(e));
		if (!expr.IsZero())
			f.Add(data[i].eq) = expr;
	}
}

void
LocVec::Sort()
{
	SetSorter<LocVecData>	ps(data);
	ps.QuickSort();
}

void
LocVec::Squeeze()
{
	Sort();
	int	i;

	i = data.LBound();
	while(i<data.UBound())
		if (data[i].eq==data[i+1].eq)
		{
			data[i].val+=data[i+1].val;
			data.Remove(i+1);
		}
		else
			i++;
}

void
LocVec::Print() const
{
	int	i;

	foreach(i,data)
		printf("eq = %d, val = %g\n", data[i].eq, (double)data[i].val);
}

void
LocVec::operator+=(const LocVec& v)
{
	int	i, vs = v.Length(), ds = data.UBound();

	data.SetSize(ds+vs);
	foreach(i,v.data)
		data[i+ds] = v.data[i];
}

LocVecData &
LocVec::operator()(int p)
{
	if (p<=Length() && p>=1)
		return data[p];
	fprintf(stderr, "Out of Range in LocVec::operator()\n");
	exit(1);
}

const LocVecData &
LocVec::operator()(int p) const
{
	if (p<=Length() && p>=1)
		return data[p];
	fprintf(stderr, "Out of Range in LocVec::operator()\n");
	exit(1);
}

LocMatData::LocMatData(long e1, long e2, const Scalar& v)
{
	eq1 = e1;
	eq2 = e2;
	val = v;
}

LocMatData::LocMatData()
{
	eq1 = eq2 = -1;
	val = 0.0;
}

LocMatData::LocMatData(const LocMatData& lmd)
{
	eq1 = lmd.eq1;
	eq2 = lmd.eq2;
	val = lmd.val;
}

LocMatData::SortBy	LocMatData::sort_by = LocMatData::RowIndex;

int
LocMatData::operator<(const LocMatData &rhs)
{
	switch(sort_by)
	{
		case	RowIndex:
			if (eq1 < rhs.eq1)
				return 1;
			if (eq1 > rhs.eq1)
				return 0;
			if (eq2 < rhs.eq2)
				return 1;
			return 0;
		case	ColIndex:
			if (eq2 < rhs.eq2)
				return 1;
			if (eq2 > rhs.eq2)
				return 0;
			if (eq1 < rhs.eq1)
				return 1;
			return 0;
		default:
			assert(1);
			return 0;
	}
	assert(1);
	return 0;
}

int
LocMatData::operator>(const LocMatData &rhs)
{
	switch(sort_by)
	{
		case	RowIndex:
			if (eq1 > rhs.eq1)
				return 1;
			if (eq1 < rhs.eq1)
				return 0;
			if (eq2 > rhs.eq2)
				return 1;
			return 0;
		case	ColIndex:
			if (eq2 > rhs.eq2)
				return 1;
			if (eq2 < rhs.eq2)
				return 0;
			if (eq1 > rhs.eq1)
				return 1;
			return 0;
		default:
			assert(1);
			return 0;
	}
	assert(1);
	return 0;
}

LocMat::LocMat(const LocMat& lm)
{
	int	i;

	foreach(i,data)
		data+=lm.data[i];
}

LocMat&
LocMat::operator=(const LocMat& lm)
{
	int	i;

	data.Empty();
	foreach(i,data)
		data+=lm.data[i];
	return *this;
}

Scalar&
LocMat::Add(long eq1, long eq2)
{
	data+=LocMatData(eq1,eq2,0.0);
	return data.Last().val;
}

void
LocMat::AddRow(long eq, const LocVec& v)
{
	int	i;

	foreach(i,v.data)
		Add(eq, v.data[i].eq) = v.data[i].val;
}

void
LocMat::AddCol(long eq, const LocVec& v)
{
	int	i;

	foreach(i,v.data)	
		Add(v.data[i].eq, eq) = v.data[i].val;
}

void
LocMat::Deriv(LocMat& m, const Scalar &e) const
{
	int	i;
	Scalar	expr;

	foreach(i,data)
	{
		expr = data[i].val.D(e);
		if (!expr.IsZero())
			m.Add(data[i].eq1,data[i].eq2) = expr;
	}
}

void
LocMat::Remap(const FixedSet<long>& map)
{
	int		row, col, i;

	foreach(i,data)
	{
		row = data[i].eq1;
		col = data[i].eq2;

		map.Get(row, data[i].eq1);
		map.Get(col, data[i].eq2);
	}
}

void
LocMat::Empty()
{
	data.Empty();
}

void
LocMat::Sort()
{
	SetSorter<LocMatData>	ps(data);
	ps.QuickSort();
}

void
LocMat::Squeeze()
{
	Sort();
	int	i;

	i = data.LBound();
	while(i<data.UBound())
		if (data[i].eq1==data[i+1].eq1 && data[i].eq2==data[i+1].eq2)
		{
			data[i].val+=data[i+1].val;
			data.Remove(i+1);
		}
		else i++;
}

void
LocMat::Print() const
{
	int	i;

	foreach(i,data)
		printf("eq1 = %d, eq2 = %d, val = %g\n",
			data[i].eq1, data[i].eq2, (double)data[i].val);
}

void
LocMat::Transpose(LocMat &lm) const
{
	lm.Empty();
	int	i;

	foreach(i,data)
		lm.Add(data[i].eq2, data[i].eq1) = data[i].val;
}

void
LocMat::operator+=(const LocMat& v)
{
	int	i, vs = v.data.Size(), dub = data.UBound();

	data.SetSize(dub+vs);
	foreach(i,v.data)
		data[i+dub] = v.data[i];
}

const LocMatData &
LocMat::operator()(int p) const
{
	if (p<=Length() && p>=1)
		return data[p];
	fprintf(stderr, "Out of Range in LocMat::operator() const\n");
	exit(1);
}

LocMatData &
LocMat::operator()(int p)
{
	if (p<=Length() && p>=1)
		return data[p];
	fprintf(stderr, "Out of Range in LocMat::operator()\n");
	exit(1);
}

void operator+=(BIMatrix &b, const LocMat &m)
{
	int	i, n = m.Length();

	for(i=1;i<=n;i++)
	{
		const LocMatData	*lmd = &m(i);
		int	eq1 = lmd->eq1,
			eq2 = lmd->eq2;
		double	val = (double)lmd->val;

		b[eq1][eq2] += val;
	}
}

void operator-=(BIMatrix &b, const LocMat &m)
{
	int	i, n = m.Length();

	for(i=1;i<=n;i++)
	{
		const LocMatData	*lmd = &m(i);
		int	eq1 = lmd->eq1,
			eq2 = lmd->eq2;
		double	val = (double)lmd->val;

		b[eq1][eq2] -= val;
	}
}

void operator+=(Vector &v, const LocVec &lv)
{
	int	i;

	for(i=1;i<=lv.Length();i++)
		v[lv(i).eq] += (double)lv(i).val;
}

void operator*=(LocMat &m, const Scalar& s)
{
	int	i;

	for(i=1;i<=m.Length();i++)
//		m(i).val=m(i).val*s;
		m(i).val*=s;
}

void operator*=(LocVec &f, const Scalar& s)
{
	int	i;

	for(i=1;i<=f.Length();i++)
//		f(i).val=f(i).val*s;
		f(i).val*=s;
}

LocMat operator*(const LocMat& m, const Scalar& s) return ret
{
	int	i;
	for(i=1;i<=m.Length();i++)
		ret.Add(m(i).eq1,m(i).eq2) = m(i).val*s;
}

LocMat operator*(const Scalar& s, const LocMat& m) return ret
{
	int	i;
	for(i=1;i<=m.Length();i++)
		ret.Add(m(i).eq1,m(i).eq2) = m(i).val*s;
}

LocVec operator*(const LocVec& v, const Scalar& s) return ret
{
	int	i;
	for(i=1;i<=v.Length();i++)
		ret.Add(v(i).eq) = v(i).val*s;
}

LocVec operator*(const Scalar& s, const LocVec& v) return ret
{
	int	i;
	for(i=1;i<=v.Length();i++)
		ret.Add(v(i).eq) = v(i).val*s;
}

LocVec operator*(const LocMat& m, const ScVector& v) return ret
{
	int	i;

	for(i=1;i<=m.Length();i++)
		ret.Add(m(i).eq1) = v[m(i).eq2]*m(i).val;
}

LocVec operator*(const ScVector& v, const LocMat& m) return ret
{
	int	i;

	for(i=1;i<=m.Length();i++)
		ret.Add(m(i).eq2) = v[m(i).eq1]*m(i).val;
}

LocVec operator*(const LocMat& m, const LocVec& v) return ret
{
	int	i, j;

	for(i=1;i<=m.Length();i++)
		for(j=1;j<=v.Length();j++)
			if (v(j).eq==m(i).eq2)
				ret.Add(m(i).eq1) = v(j).val*m(i).val;
}

LocVec operator*(const LocVec& v, const LocMat& m) return ret
{
	int	i, j;

	for(i=1;i<=m.Length();i++)
		for(j=1;j<=v.Length();j++)
			if (v(j).eq==m(i).eq1)
				ret.Add(m(i).eq2) = v(j).val*m(i).val;
}

LocMat operator*(const LocMat& a, const LocMat& b) return ret
{
	int	i, j;
	for(i=1;i<=a.Length();i++)
		for(j=1;j<=b.Length();j++)
			if (a(i).eq2==b(j).eq1)
				ret.Add(a(i).eq1,b(j).eq2) = a(i).val*b(j).val;
}

Scalar operator*(const LocVec& a, const LocVec& b) return ret
{
	int	i, j;

	for(i=1;i<=a.Length();i++)
		for(j=1;j<=b.Length();j++)
			if (a(i).eq==b(j).eq)
				ret += a(i).val*b(j).val;
}

LocMat LocOuterProd(const LocVec& a, const LocVec& b) return ret
{
	int	i, j;

	for(i=1;i<=a.Length();i++)
		for(j=1;j<=b.Length();j++)
			ret.Add(a(i).eq,b(j).eq) = a(i).val*b(j).val;
}

