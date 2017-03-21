#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif
#include	<BIMatrix.h>
#include	<Vector.h>
#include	<Conj.h>

#include	"Domain.h"
#include	"ResSystem.h"
#include	"Essential.h"

#define	USE_NEWTON_RAPHSON	1
#define	UNSYM		1
#define	NRDEBUG 0
#define	SHOW_ASSEMBLE 0

#if	UNSYM
#define	SOLVER		BiConj
#else
#define	SOLVER		Conj
#endif

#if	0
StdDomainSystem::StdDomainSystem(StdDomain& d) : domain(d)
{
}

int
StdDomainSystem::Num()
{
	return domain.NumDOFS();
}

double
StdDomainSystem::Get(int n)
{
	return domain.Sol()[n];
}

void
StdDomainSystem::Set(int n, double v)
{
	domain.Sol()[n] = v;
}

void
StdDomainSystem::Residual(Vector &r)
{
	BIMatrix	K;
	Vector		f, &u = domain.Sol();

	domain.Stiff(K);
	domain.Force(f);

	r = K*u-f;

#if	SHOW_ASSEMBLE
	printf("Before BCs\n");
	printf("Stiffness Matrix = \n");
	K.Print();	
	printf("Force Vector = \n");
	f.Print();	
	printf("Solution = \n");
	u.Print();	
	printf("Residual = \n");
	r.Print();	
#endif
}

void
StdDomainSystem::Tangent(BIMatrix& K)
{
	domain.TangentStiff(K);
}

void
StdDomainSystem::GetDUDB(Vector &s, int bn)
{
	Vector	load, rhs;
	BIMatrix	DRDU;
	SOLVER		cg(DRDU);

	cout << "--> Solving for sensitivity w.r.t b" << bn << endl;
	Tangent(DRDU);
	GetDRDB(load,bn);
	rhs = -load;

	/* Sensitivities of essential boundary conditions is zero */
	Reduce(DRDU,rhs,zero);

#ifdef	MDEBUG
	malloc_enter("StdDomainSystem::GetDUDB");
#endif

	s = cg.Solve(rhs, .00001);

#ifdef	MDEBUG
	malloc_leave("StdDomainSystem::GetDUDB");
#endif

	cout << "--> done" << endl;

}

void
StdDomainSystem::GetDRDB(Vector& drdb, int bn)
{
	BIMatrix	dKdb;
	Vector		dFdb;
	Vector	&u = domain.Sol();

	domain.PartialStiff(dKdb,bn);
	domain.PartialForce(dFdb,bn);

	drdb = dKdb*u-dFdb;
#if	0
	printf("dKdb%d = \n", bn);
	dKdb.Print();
	printf(" u = \n");
	u.Print();
	printf("dFdb%d = \n", bn);
	dFdb.Print();
	printf("drdb%d = \n", bn);
	drdb.Print();
#endif
}

void
StdDomainSystem::Update()
{
}

#if	0
void
StdDomainSystem::Reduce(BIMatrix &K, Vector &f, ReducType rt)
{
	int	i, nbcs;
	Vector	off;
	const FixedSet<Essential *>	&bcs_set = domain.BCSSet();
	Indexer<Essential	*>	bcs(bcs_set);

	switch(rt)
	{
		case zero:
			for(i=1;i<=bcs.Size();i++)
				off += K.Eliminate(bcs[i]->Eq(),0.0);
			f+=off;
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = 0.0;
			break;
		case res:
			for(i=1;i<=bcs.Size();i++)
				off += K.Eliminate(bcs[i]->Eq(),bcs[i]->Val());
			f+=off;
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = bcs[i]->Val();
			break;
		case mat:
			for(i=1;i<=bcs.Size();i++)
				off += K.Eliminate(bcs[i]->Eq(),bcs[i]->Val());
			f+=off;
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = bcs[i]->Val();
			break;
		case rhsonly:
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = bcs[i]->Val();
			break;
		case rhszero:
			for(i=1;i<=bcs.Size();i++)
				f[bcs[i]->Eq()] = 0.0;
			break;
		default:
			fprintf(stderr, "Error in SteadySystem::Reduce\n");
			break;
	}
}
#endif
#endif

void
NewtonRaphson::Solve(double tol, int maxiter)
{
	int			iters, i;
	BIMatrix	mat;
	SOLVER		cg(mat);
	Vector		R,du,load,U; //&U=sys->StateVector(),load;

#ifdef	MDEBUG
	malloc_enter("NewtonRaphson::Solve");
#endif

	for(i=1;i<=sys->Num();i++)
		U[i] = sys->Get(i);

	iters=0;			// Set iterations
	sys->Update();

	sys->LocalReduce(mat,U,System::rhsonly);
	for(i=1;i<=sys->Num();i++)
		sys->Set(i,U[i]);
	sys->Residual(R);	// Get initial residual

	mat.Zero();			// Zero the matrix
	load = -R;
	sys->Tangent(mat);	// Get DR/Du
	sys->LocalReduce(mat,load,System::zero);
	
	while(iters<maxiter && load.norm()>tol)
	{

#ifdef	MDEBUG
		malloc_enter("NewtonRaphson::Solve1");
#endif

		du = cg.Solve(load,tol);
		U+=du;
		for(i=1;i<=sys->Num();i++)
			sys->Set(i,U[i]);

#ifdef	MDEBUG
		malloc_leave("NewtonRaphson::Solve1");
#endif

#if	NRDEBUG
		printf("NR Residual = \n");
		R.Print();
		printf("Modified NR Residual = \n");
		load.Print();
		printf("du = \n");
		du.Print();
		printf("new solution = \n");
		U.Print();
#endif

		sys->Update();
		sys->Residual(R);	// Recalculate residual

#ifdef	MDEBUG
		malloc_enter("NewtonRaphson::Solve2");
#endif

		mat.Zero();			// Zero the matrix

#ifdef	MDEBUG
		malloc_leave("NewtonRaphson::Solve2");
#endif

		load = -R;
		sys->Tangent(mat);	// Get DR/Du
		sys->LocalReduce(mat,load,System::zero);
		iters++;			// Increment iterations

#if	NRDEBUG
		printf("NR Residual = \n");
		R.Print();
		printf("Modified NR Residual = \n");
		load.Print();
#endif

		printf("||R|| = %g\n", load.norm());
	}

#ifdef	MDEBUG
	malloc_leave("NewtonRaphson::Solve");
#endif
}

