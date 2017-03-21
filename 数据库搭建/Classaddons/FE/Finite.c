#include	<assert.h>
#include	"Finite.h"
#include	"Gauss.h"
#include	"Domain.h"
#include	"BasicTypes.h"
#include	"MatProp.h"
#include	"Cache.h"
#include	"GaussContext.h"
#include	"GPInfo.h"
#include	"Basis.h"
#ifdef	USE_MCHUNK
#include	"MemChunk.h"
#endif
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

void 
Finite::TanStiff(BIMatrix&, LocalDomain&)
{
	// For models that don't specify this routine, it is assumed that
	// there is no contribution, hence this routine does nothing.
}

void 
Finite::ParStiff(BIMatrix&, LocalDomain&, int)
{
	// For models that don't specify this routine, it is assumed that
	// there is no contribution, hence this routine does nothing.
}

void 
Finite::TanForce(BIMatrix&, LocalDomain&)
{
	// For models that don't specify this routine, it is assumed that
	// there is no contribution, hence this routine does nothing.
}

void 
Finite::ParForce(SVector&, LocalDomain&, int)
{
	// For models that don't specify this routine, it is assumed that
	// there is no contribution, hence this routine does nothing.
}


CondElem::CondElem(const BasisFunctions& b, Transformation &t, const Const& d,
	const GenGauss& g, const MatProp &m)
	: bf(b), tran(t), ggw(g), dof(d), mp1(m)
{
}

void
CondElem::Stiff(BIMatrix& mat, LocalDomain& d)
{
#ifdef	MDEBUG
	malloc_enter("CondElem::Stiff");
#endif

	GPInfo	cs;
	bf.Eval(ggw,cs);					// This should be done in the
										// constructor
#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*2);
	double	*u_at_gp = tmp.Vec(ggw.Size());
	double	*k_at_gp = tmp.Vec(ggw.Size());
#else
	double *u_at_gp = new double[ggw.Size()*2];
	double *k_at_gp = u_at_gp+ggw.Size();
#endif

	mp1.Eval(k_at_gp, d, *this, cs);

	MatCache	stiff(mat,dof,*this);
	Recover1(d, dof, *this, u_at_gp, cs);	
	Stiff(stiff, cs, k_at_gp);

#ifndef	USE_MCHUNK
	delete [] u_at_gp;
#endif

#ifdef	MDEBUG
	malloc_leave("CondElem::Stiff");
#endif
}

void
CondElem::ParStiff(MatCache& mc, const GPInfo& cs, double *K, int bn)
{
	int	i, j, k, g;
	const int nn = nlist.Size();
	const int nd = bf.NumDim();
	GaussContext	*gcp, *pgcp;
	GPInfo	lc, plc;

	cs.Transform(tran,lc);
	cs.ParTransform(tran,plc,bn);

	assert(lc.Size()==ggw.Size());		// context for each gauss point
	foreach(g,lc)
	{
		gcp = lc[g];
		pgcp = plc[g];
#if	0
		cout << "Context " << endl;
		cout << *gcp << endl;
		cout << "Par Context " << endl;
		cout << *pgcp << endl;
#endif

		for(i=1;i<=nn;i++)
			for(j=1;j<=nn;j++)
				for(k=1;k<=nd;k++)
				{
					mc(i,j)+=K[g-1]*pgcp->B(k,i)*gcp->B(k,j)*gcp->detJ();
					mc(i,j)+=K[g-1]*gcp->B(k,i)*pgcp->B(k,j)*gcp->detJ();
					mc(i,j)+=K[g-1]*gcp->B(k,i)*gcp->B(k,j)*pgcp->detJ();
				}
	}
}

void
CondElem::Stiff(MatCache& mc, const GPInfo& cs, double *K)
{
#ifdef	MDEBUG
	malloc_enter("CondElem::Stiff");
#endif
	int	i, j, k, g;
	const int nn = nlist.Size();
	const int nd = bf.NumDim();
	GaussContext	*gcp;
	GPInfo	lc;

	cs.Transform(tran,lc);

	assert(lc.Size()==ggw.Size());		// context for each gauss point
	foreach(g,lc)
	{
		gcp = lc[g];
		for(i=1;i<=nn;i++)
			for(j=1;j<=nn;j++)
				for(k=1;k<=nd;k++)
					mc(i,j)+=K[g-1]*gcp->B(k,i)*gcp->B(k,j)*gcp->detJ();
	}
#ifdef	MDEBUG
	malloc_leave("CondElem::Stiff");
#endif
}

void
CondElem::TanStiff(BIMatrix& mat, LocalDomain& d)
{
#ifdef	MDEBUG
	malloc_enter("CondElem::TanStiff");
#endif
	if (mp1.IsConst())
		return;

	int	g,i,j,k,l;
	const int nn = nlist.Size();
	const int nd = bf.NumDim();
	GPInfo	cs, lc;
	GaussContext	*gcp;
	MatCache	stiff(mat,dof,*this);
#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*2+nn);
	double	*u = tmp.Vec(ggw.Size());
	double	*dk = tmp.Vec(ggw.Size());
	double	*Tn = tmp.Vec(nn);
#else
	double	*u = new double[ggw.Size()*2+nn];
	double	*dk = u+ggw.Size();
	double	*Tn = dk + ggw.Size();
#endif

	bf.Eval(ggw,cs);					// This should be done in the
										// constructor

	Recover1(d, dof, *this, u, cs);
	mp1.TanEval(dk, dof, d, *this, cs);
	
	cs.Transform(tran,lc);
	assert(lc.Size()==ggw.Size());		// context for each gauss point
	for(i=1;i<=nn;i++)
		Tn[i-1] = d.GetSol(EqNum(i,Misc::Temp));
//		Tn[i-1] = d.last[EqNum(i,Misc::Temp)];

	foreach(g,lc)
	{
		gcp = lc[g];
		for(i=1;i<=nn;i++)
			for(k=1;k<=nn;k++)
				for(l=1;l<=nd;l++)
					for(j=1;j<=nn;j++)
						stiff(i,k)+=dk[g-1]*gcp->B(l,i)*gcp->B(l,j)*
									Tn[j-1]*gcp->N(k)*gcp->detJ();
	}
#ifndef	USE_MCHUNK
	delete [] u;
#endif

#ifdef	MDEBUG
	malloc_leave("CondElem::TanStiff");
#endif
}

void
CondElem::ParStiff(BIMatrix& mat, LocalDomain& d, int bn)
{
#ifdef	MDEBUG
	malloc_enter("CondElem::ParStiff");
#endif
	GPInfo	cs;
	bf.Eval(ggw,cs);					// This should be done in the
										// constructor

#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*3);
	double *u_at_gp = tmp.Vec(ggw.Size());
	double *dk_at_gp = tmp.Vec(ggw.Size());
	double *k_at_gp = tmp.Vec(ggw.Size());
#else
	double *u_at_gp = new double[ggw.Size()*3];
	double *dk_at_gp = u_at_gp+ggw.Size();
	double *k_at_gp = dk_at_gp+ggw.Size();
#endif

	mp1.ParEval(dk_at_gp, bn, d, *this, cs);
	mp1.Eval(k_at_gp, d, *this, cs);

	MatCache	stiff(mat,dof,*this);
	Recover1(d, dof, *this, u_at_gp, cs);	
	Stiff(stiff, cs, dk_at_gp);
	ParStiff(stiff, cs, k_at_gp, bn);
#ifndef	USE_MCHUNK
	delete [] u_at_gp;
#endif

#ifdef	MDEBUG
	malloc_leave("CondElem::ParStiff");
#endif
}

void
CondElem::Force(SVector&, LocalDomain&)
{
}

ConvElem::ConvElem(const BasisFunctions& b, Transformation &t, const Const& d,
	const GenGauss& g, const MatProp &h, const MatProp &T)
	: bf(b), tran(t), ggw(g), dof(d), htc(h), tinf(T)
{
}

void
ConvElem::Stiff(BIMatrix& mat, LocalDomain& d)
{
#ifdef	MDEBUG
	malloc_enter("ConvElem::Stiff");
#endif
	GPInfo	cs;
	bf.Eval(ggw,cs);					// This should be done in the
										// constructor
	MatCache	stiff(mat,dof,*this);
#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*2);
	double	*u_at_gp = tmp.Vec(ggw.Size());
	double	*h_at_gp = tmp.Vec(ggw.Size());
#else
	double	*u_at_gp = new double[ggw.Size()*2];
	double	*h_at_gp = u_at_gp+ggw.Size();
#endif

	Recover1(d, dof, *this, u_at_gp, cs);
	htc.Eval(h_at_gp, d, *this, cs);

	Stiff(stiff, cs, h_at_gp);
#ifndef	USE_MCHUNK
	delete [] u_at_gp;
#endif

#ifdef	MDEBUG
	malloc_leave("ConvElem::Stiff");
#endif
}

void
ConvElem::Stiff(MatCache& mc, const GPInfo& cs, double *H)
{
	int	g,i,j;
	const int nn = nlist.Size();
	GPInfo	lc;
	GaussContext	*gcp;
	cs.Transform(tran,lc);
	assert(lc.Size()==ggw.Size());		// context for each gauss point

	foreach(g,lc)
	{
		gcp = lc[g];
		for(i=1;i<=nn;i++)
			for(j=1;j<=nn;j++)
				mc(i,j)+=H[g-1]*gcp->N(i)*gcp->N(j)*gcp->detJ();
	}
}

void
ConvElem::ParStiff(MatCache& mc, const GPInfo& cs, double *H, int bn)
{
	int	g,i,j;
	const int nn = nlist.Size();
	GPInfo	lc, plc;
	GaussContext	*gcp, *pgcp;

	cs.Transform(tran,lc);
	cs.ParTransform(tran,plc,bn);

	assert(lc.Size()==ggw.Size());		// context for each gauss point

	foreach(g,lc)
	{
		gcp = lc[g];
		pgcp = plc[g];
		for(i=1;i<=nn;i++)
			for(j=1;j<=nn;j++)
				mc(i,j)+=H[g-1]*gcp->N(i)*gcp->N(j)*pgcp->detJ();
	}
}

void
ConvElem::TanStiff(BIMatrix& mat, LocalDomain& d)
{
#ifdef	MDEBUG
	malloc_enter("ConvElem::TanStiff");
#endif
	if (htc.IsConst())
		return;

	int	g,i,j;
	const int nn = nlist.Size();
	GPInfo	cs, lc;
	GaussContext	*gcp;
	MatCache	stiff(mat,dof,*this);
#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*3);
	double	*u = tmp.Vec(ggw.Size());
	double	*dh = tmp.Vec(ggw.Size());
	double	*Ti = tmp.Vec(ggw.Size());
#else
	double	*u = new double[ggw.Size()*3];
	double	*dh = u+ggw.Size();
	double	*Ti = dh + ggw.Size();
#endif

	bf.Eval(ggw,cs);					// This should be done in the
										// constructor

	Recover1(d, dof, *this, u, cs);
	htc.TanEval(dh, Misc::Temp, d, *this, cs);
	tinf.Eval(Ti, d, *this, cs);

	cs.Transform(tran,lc);
	assert(lc.Size()==ggw.Size());		// context for each gauss point

	foreach(g,lc)
	{
		gcp = lc[g];
		for(i=1;i<=nn;i++)
			for(j=1;j<=nn;j++)
				stiff(i,j)+=dh[g-1]*(u[g-1]-Ti[g-1])*
								gcp->N(i)*gcp->N(j)*gcp->detJ();
	}
#ifndef	USE_MCHUNK
	delete [] u;
#endif

#ifdef	MDEBUG
	malloc_leave("ConvElem::TanStiff");
#endif
}

void
ConvElem::ParStiff(BIMatrix& mat, LocalDomain& d, int bn)
{
#ifdef	MDEBUG
	malloc_enter("ConvElem::ParStiff");
#endif
	GPInfo	cs;
	bf.Eval(ggw,cs);					// This should be done in the
										// constructor
	MatCache	stiff(mat,dof,*this);
#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*3);
	double	*u_at_gp = tmp.Vec(ggw.Size());
	double	*h_at_gp = tmp.Vec(ggw.Size());
	double	*dh_at_gp = tmp.Vec(ggw.Size());
#else
	double	*u_at_gp = new double[ggw.Size()*3];
	double	*h_at_gp = u_at_gp+ggw.Size();
	double	*dh_at_gp = h_at_gp+ggw.Size();
#endif

	Recover1(d, dof, *this, u_at_gp, cs);

	htc.Eval(h_at_gp, d, *this, cs);
	htc.ParEval(dh_at_gp, bn, d, *this, cs);

	Stiff(stiff, cs, dh_at_gp);
	ParStiff(stiff, cs, h_at_gp, bn);

#ifndef	USE_MCHUNK
	delete [] u_at_gp;
#endif

#ifdef	MDEBUG
	malloc_leave("ConvElem::ParStiff");
#endif
}

void
ConvElem::Force(SVector& sv, LocalDomain& d)
{
#ifdef	MDEBUG
	malloc_enter("ConvElem::Force");
#endif
	int	g,i;
	const int nn = nlist.Size();
	GPInfo	cs, lc;
	GaussContext	*gcp;
	VecCache	force(sv,dof,*this);
#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*3);
	double	*u_at_gp = tmp.Vec(ggw.Size());
	double	*h_at_gp = tmp.Vec(ggw.Size());
	double	*T_at_gp = tmp.Vec(ggw.Size());
#else
	double	*u_at_gp = new double[ggw.Size()*3];
	double	*h_at_gp = u_at_gp + ggw.Size();
	double	*T_at_gp = h_at_gp + ggw.Size();
#endif

	bf.Eval(ggw,cs);					// This should be done in the
										// constructor

	Recover1(d, dof, *this, u_at_gp, cs);
	htc.Eval(h_at_gp, d, *this, cs);
	tinf.Eval(T_at_gp, d, *this, cs);
	
	cs.Transform(tran,lc);
	assert(lc.Size()==ggw.Size());		// context for each gauss point
	foreach(g,lc)
	{
		gcp = lc[g];
		for(i=1;i<=nn;i++)
			force(i)+=h_at_gp[g-1]*gcp->N(i)*T_at_gp[g-1]*gcp->detJ();
	}
#ifndef	USE_MCHUNK
	delete [] u_at_gp;
#endif

#ifdef	MDEBUG
	malloc_leave("ConvElem::Force");
#endif
}

void
ConvElem::ParForce(SVector& sv, LocalDomain& d, int bn)
{
#ifdef	MDEBUG
	malloc_enter("ConvElem::ParForce");
#endif
	int	g,i;
	const int nn = nlist.Size();
	GPInfo	cs, lc, plc;
	GaussContext	*gcp, *pgcp;
	VecCache	force(sv,dof,*this);
#ifdef	USE_MCHUNK
	DoubleChunk	tmp(ggw.Size()*5);
	double	*u_at_gp = tmp.Vec(ggw.Size());
	double	*h_at_gp = tmp.Vec(ggw.Size());
	double	*T_at_gp = tmp.Vec(ggw.Size());
	double	*dh_at_gp = tmp.Vec(ggw.Size());
	double	*dT_at_gp = tmp.Vec(ggw.Size());
#else
	double	*u_at_gp = new double[ggw.Size()*5];
	double	*h_at_gp = u_at_gp + ggw.Size();
	double	*T_at_gp = h_at_gp + ggw.Size();
	double	*dh_at_gp = T_at_gp + ggw.Size();
	double	*dT_at_gp = dh_at_gp + ggw.Size();
#endif

	bf.Eval(ggw,cs);					// This should be done in the
										// constructor

	Recover1(d, dof, *this, u_at_gp, cs);
	htc.Eval(h_at_gp, d, *this, cs);
	tinf.Eval(T_at_gp, d, *this, cs);
	htc.ParEval(dh_at_gp, bn, d, *this, cs);
	tinf.ParEval(dT_at_gp, bn, d, *this, cs);
	
	cs.Transform(tran,lc);
	cs.ParTransform(tran,plc,bn);

	assert(lc.Size()==ggw.Size());		// context for each gauss point
	foreach(g,lc)
	{
		gcp = lc[g];
		pgcp = plc[g];
		for(i=1;i<=nn;i++)
		{
			force(i)+= h_at_gp[g-1]*gcp->N(i)*dT_at_gp[g-1]*gcp->detJ();
			force(i)+= dh_at_gp[g-1]*gcp->N(i)*T_at_gp[g-1]*gcp->detJ();
			force(i)+= h_at_gp[g-1]*gcp->N(i)*T_at_gp[g-1]*pgcp->detJ();
//			force(i)+=(dh_at_gp[g-1]*gcp->N(i)*T_at_gp[g-1]+
//			           h_at_gp[g-1]*gcp->N(i)*dT_at_gp[g-1])*gcp->detJ();
		}
	}
#ifndef	USE_MCHUNK
	delete [] u_at_gp;
#endif

#ifdef	MDEBUG
	malloc_leave("ConvElem::ParForce");
#endif
}
