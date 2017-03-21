#include	"SimObject.h"
#include	<iostream.h>
#include	<Vector.h>

int
SimObject::GetDGDB(Vector &v)
{
	cerr << "Adjoint Method not implemented!" << endl;
	return 0;
}

int
SimObject::GetDUDB(Vector &v, int bn)
{
	cerr << "Direct Differentiation Method not implemented!" << endl;
	return 0;
}

void
SimObject::GetSol(Vector &v) const
{
	int	i;

	for(i=1;i<=NumNodes();i++)
		v[i] = GetSol(i);
}
