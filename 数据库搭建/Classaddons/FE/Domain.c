/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * File : $Id: Domain.c,v 3.6 1993/06/10 13:58:10 tiller Exp $
 * Changelog
 *	$Log: Domain.c,v $
 * Revision 3.6  1993/06/10  13:58:10  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 3.6  1993/06/10  13:58:10  tiller
 * Everything seems to be working under gcc-2.3.3
 *
 * Revision 3.5  1993/05/25  21:31:08  tiller
 * This version has been run on two of the tests programs and
 * produces correct results.  It appears that most of the sub-domain
 * related issues have been resolved.
 *
 * Revision 3.4  1993/05/25  13:38:19  tiller
 * Still not complete.  This image has SubDomains, but I am
 * now going to attempt unifying things again into two abstract
 * classes...Domains and LocalDomains.
 *
 * Revision 3.3  1993/05/24  15:50:12  tiller
 * This is an alpha version of some modifications I made while adding
 * SubDomain and GenSystem support.
 *
 * Revision 3.2  1993/05/20  15:13:21  tiller
 * In anticipation of doing some parallel solution schemes, I'm creating
 * a hierarchy of Domain subclasses.
 *
 * Revision 3.1  1993/05/17  22:37:42  tiller
 * This is version 3.1 of my Finite Element class library.  It represents
 * work done in completion of ME 497 in Spring '93.
 *
 * Revision 2.15  1993/05/13  15:59:21  tiller
 * This is the last check-in I think before going to version 3.  I
 * anticipate cleaning up the code a little, but no more significant
 * changes before version 3.
 *
 * ToDo:
 * 	* Preconditioning
 * 	* Transient Problems
 * 	* Enthalpy Model
 *
 * Revision 2.15  1993/05/13  15:59:21  tiller
 * This is the last check-in I think before going to version 3.  I
 * anticipate cleaning up the code a little, but no more significant
 * changes before version 3.
 *
 * ToDo:
 * 	* Preconditioning
 * 	* Transient Problems
 * 	* Enthalpy Model
 *
 * Revision 2.14  1993/05/11  15:43:02  tiller
 * Added support for tracking memory leaks.  Everything looks good in
 * this release.
 *
 * Revision 2.13  1993/05/04  22:51:02  tiller
 * Made some radical changes to make Domain abstract and to modularize
 * things a little more.  The System heirarchy is probably in a state of
 * flux.
 *
 * Revision 2.12  1993/04/20  22:59:45  tiller
 * Shape Sensitivities seem to be working!!!!
 *
 * Revision 2.11  1993/03/31  20:04:27  tiller
 * Everything involved in sensitivities seems to be working now.
 *
 * Revision 2.11  1993/03/31  20:04:27  tiller
 * Everything involved in sensitivities seems to be working now.
 *
 * Revision 2.10  1993/03/25  14:12:05  tiller
 * Sensitivities seem to be working properly, at least for the
 * test problem.
 *
 * Revision 2.10  1993/03/25  14:12:05  tiller
 * Sensitivities seem to be working properly, at least for the
 * test problem.
 *
 * Revision 2.9  1993/03/19  16:53:37  tiller
 * Stiffness Terms for Finite Elements seem to be working, at least
 * for the Conduction and Convection Elements.  Material Properties
 * seem to be in order as well.  Next step is to test the sensitivity
 * (Par*) methods that are already in place.
 *
 * Revision 2.9  1993/03/19  16:53:37  tiller
 * Stiffness Terms for Finite Elements seem to be working, at least
 * for the Conduction and Convection Elements.  Material Properties
 * seem to be in order as well.  Next step is to test the sensitivity
 * (Par*) methods that are already in place.
 *
 * Revision 2.8  1993/03/10  14:54:09  tiller
 * Minor Changes
 *
 * Revision 2.8  1993/03/10  14:54:09  tiller
 * Minor Changes
 *
 * Revision 2.7  1993/03/07  17:54:00  tiller
 * Things seem to work for the Non-Linear Heat Transfer examples.
 * I'm about to change the interfaces for some optimization reasons.
 * For now, everything seems stable.
 *
 * Revision 2.6  1993/02/25  20:56:58  tiller
 * Fixed numerous bugs involving force terms.  Mostly, forgeting to add them.
 *
 * Revision 2.5  1993/02/15  15:14:05  tiller
 * Homework 2 works...too many fixes to name
 *
 * Revision 2.5  1993/02/15  15:14:05  tiller
 * Homework 2 works...too many fixes to name
 *
 * Revision 2.4  1993/02/13  18:36:08  tiller
 * Fixed a bug in 'Domain::AddDOF(Node *node, short type)'
 *
 * Revision 2.3  1993/02/13  18:03:50  tiller
 * Added support for a default domain in which the Nodes
 * are constructed into...the overall benefit of this is
 * that when elements are created they can include
 * the correct DOF's on the appropriate nodes.
 *
 * Revision 2.2  1993/02/10  20:50:45  tiller
 * Improved the poorly handled cases of forming the:
 * 	* Stiffness Matrix
 * 	* Tangent Matrix
 * 	* Partial Matrix
 *
 * Revision 2.1  1993/02/04  00:20:09  tiller
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
 * Revision 1.6  1993/01/31  22:58:02  tiller
 * Got finite differences working, DOT support is really close!
 *
 * Revision 1.5  1993/01/31  00:11:57  tiller
 * Version 1.5 (Can construct domain from input file, more encapsulation,
 *              lacks DOT support)
 *
 * Revision 1.4  1993/01/30  22:41:29  tiller
 * Version 1.4 (Direct and Adjoint both work)
 *
*/

#ifdef	__GNUC__
#pragma implementation
#endif

#include	<string.h>
#include	<stdlib.h>
#include	<BIMatrix.h>
#include	<assert.h>
#include	"Domain.h"
#include	"Element.h"
#include	"Design.h"
#include	"Objective.h"
#include	"Essential.h"
#include	"Const.h"
#include	"Node.h"
#include	"Integrator.h"

#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif

#define	ASSEMBLE_DEBUG	0

// Static Members
// long	Domain::NumDOFS = 0;


Domain::Domain(Integrator &in) : integrator(in)
{
}

LocalDomain::LocalDomain(Integrator &in) : Domain(in)
{
}


