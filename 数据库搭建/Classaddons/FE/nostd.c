#include	"MatProp.h"
#include	<iostream.h>
#include	<stdlib.h>

extern "C"
{
	void matinfo(int *, double *, double *, double *, double *, int *, int *)
	{
		cerr << "Error, no 'matinfo' defined!" << endl;
		exit(1);
	}

	void matdinfo(int *, double *, int *, double *,
		double *, double *, int *, int *)
	{
		cerr << "Error, no 'matdinfo' defined!" << endl;
		exit(1);
	}

	void nodedinfo(int *, double *, int *)
	{
		cerr << "Error, no 'nodedinfo' defined!" << endl;
		exit(1);
	}
}

