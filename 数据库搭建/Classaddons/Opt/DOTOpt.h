/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */
/*
 * File : $Id: DOTOpt.h,v 1.1 1993/05/09 19:00:00 tiller Exp $
 * Changelog
 *	$Log: DOTOpt.h,v $
 * Revision 1.1  1993/05/09  19:00:00  tiller
 * Initial revision
 *
 * Revision 1.1  1993/05/09  19:00:00  tiller
 * Initial revision
 *
 * Revision 2.7  1993/05/04  22:51:02  tiller
 * Made some radical changes to make Domain abstract and to modularize
 * things a little more.  The System heirarchy is probably in a state of
 * flux.
 *
 * Revision 2.6  1993/04/20  18:47:21  tiller
 * Too many changes to list...mostly just testing shape sensitivity!
 *
 * Revision 2.6  1993/04/20  18:47:21  tiller
 * Too many changes to list...mostly just testing shape sensitivity!
 *
 * Revision 2.5  1993/03/30  20:00:51  tiller
 * I seem to have fixed some bugs in how boundary conditions are handled.
 * In addition, Everything seems to be working...next I have to check
 * for some phantom sensitivities...hopefully they will be gone.  There
 * does seem to be a problem with the sensitivities be permuted by the
 * System::Direct method.
 *
 * Revision 2.5  1993/03/30  20:00:51  tiller
 * I seem to have fixed some bugs in how boundary conditions are handled.
 * In addition, Everything seems to be working...next I have to check
 * for some phantom sensitivities...hopefully they will be gone.  There
 * does seem to be a problem with the sensitivities be permuted by the
 * System::Direct method.
 *
 * Revision 2.4  1993/03/23  23:15:45  tiller
 * Tangent terms appears to be correct.  They have been tested on a ~200
 * node problem and appear to work correctly.
 *
 * Revision 2.4  1993/03/23  23:15:45  tiller
 * Tangent terms appears to be correct.  They have been tested on a ~200
 * node problem and appear to work correctly.
 *
 * Revision 2.3  1993/03/15  14:30:23  tiller
 * The 'Finite' abstraction seems to be working.
 *
 * Revision 2.3  1993/03/15  14:30:23  tiller
 * The 'Finite' abstraction seems to be working.
 *
 * Revision 2.2  1993/03/07  17:54:00  tiller
 * Things seem to work for the Non-Linear Heat Transfer examples.
 * I'm about to change the interfaces for some optimization reasons.
 * For now, everything seems stable.
 *
 * Revision 2.2  1993/03/07  17:54:00  tiller
 * Things seem to work for the Non-Linear Heat Transfer examples.
 * I'm about to change the interfaces for some optimization reasons.
 * For now, everything seems stable.
 *
 * Revision 2.1  1993/02/04  00:19:42  tiller
 * Minor Changes
 *
 * Revision 2.0  1993/02/01  17:00:24  tiller
 * Computer Assignment 1 Finished.
 *
 * Support for:
 *      * Direct Differentiation
 *      * Adjoint
 *      * Finite Difference
 *      * Dot Support
 *
 */

#ifndef	_DOT_H_
#define	_DOT_H_

#ifdef	__GNUC__
#pragma interface
#endif

#include	<stdio.h>
#include	<math.h>
#include	"Opt.h"

extern "C"
{
void	dot_(int *, int *, int *, int *, int *,
			 double *, double *, double *, double *,
		     int *, double *, double *, int *, double *,
		     int *, int *, int *);
}

class	DOT : public Optimizer
{
	private:
		int	nrwk, nriwk;
		int	ncon;
		double	*wk, *rprm;
		double	obj;
		int	*iwk, *iprm;
		enum	Instructions { Done, GetObj, GetGrad };
		int		instruction;

	protected:
		void	GetGradients(GradVec);
		Optimizable&	opt;
		DesignVec	xl, xu;
		GradVec		grad;
		void	Init();

	public:
		enum	Methods { Feasible=1, SLP=2, SQP=3 };
		enum	Info { Nothing=0, Initial=1, Objectives=2, Constraints=3,
		    	       Gradients=4, Directions=5, Extra=6, Everything=7 };
		int	method, iprint, minmax;

		DOT(Optimizable& o, DesignVec x0, int meth=Feasible,
			int pr=Objectives, int NRWK=1000, int NRIWK=500);
		int	Iterate(int, int&);
		virtual ~DOT();
};

#endif
