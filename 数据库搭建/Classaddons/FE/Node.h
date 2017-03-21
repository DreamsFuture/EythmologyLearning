/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */
/*
 * File : $Id: Node.h,v 3.4 1993/06/10 20:59:29 tiller Exp tiller $
 * Changelog
 *	$Log: Node.h,v $
 * Revision 3.4  1993/06/10  20:59:29  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 3.3  1993/05/25  21:31:08  tiller
 * This version has been run on two of the tests programs and
 * produces correct results.  It appears that most of the sub-domain
 * related issues have been resolved.
 *
 * Revision 3.3  1993/05/25  21:31:08  tiller
 * This version has been run on two of the tests programs and
 * produces correct results.  It appears that most of the sub-domain
 * related issues have been resolved.
 *
 * Revision 3.2  1993/05/24  15:50:12  tiller
 * This is an alpha version of some modifications I made while adding
 * SubDomain and GenSystem support.
 *
 * Revision 3.2  1993/05/24  15:50:12  tiller
 * This is an alpha version of some modifications I made while adding
 * SubDomain and GenSystem support.
 *
 * Revision 3.1  1993/05/17  22:37:42  tiller
 * This is version 3.1 of my Finite Element class library.  It represents
 * work done in completion of ME 497 in Spring '93.
 *
 * Revision 3.1  1993/05/17  22:37:42  tiller
 * This is version 3.1 of my Finite Element class library.  It represents
 * work done in completion of ME 497 in Spring '93.
 *
 * Revision 2.11  1993/05/17  22:03:37  tiller
 * *** empty log message ***
 *
 * Revision 2.10  1993/05/13  15:59:21  tiller
 * This is the last check-in I think before going to version 3.  I
 * anticipate cleaning up the code a little, but no more significant
 * changes before version 3.
 *
 * ToDo:
 * 	* Preconditioning
 * 	* Transient Problems
 * 	* Enthalpy Model
 *
 * Revision 2.9  1993/05/04  22:51:02  tiller
 * Made some radical changes to make Domain abstract and to modularize
 * things a little more.  The System heirarchy is probably in a state of
 * flux.
 *
 * Revision 2.9  1993/05/04  22:51:02  tiller
 * Made some radical changes to make Domain abstract and to modularize
 * things a little more.  The System heirarchy is probably in a state of
 * flux.
 *
 * Revision 2.8  1993/04/20  22:59:45  tiller
 * Shape Sensitivities seem to be working!!!!
 *
 * Revision 2.7  1993/04/20  18:47:21  tiller
 * Too many changes to list...mostly just testing shape sensitivity!
 *
 * Revision 2.6  1993/03/30  20:00:51  tiller
 * I seem to have fixed some bugs in how boundary conditions are handled.
 * In addition, Everything seems to be working...next I have to check
 * for some phantom sensitivities...hopefully they will be gone.  There
 * does seem to be a problem with the sensitivities be permuted by the
 * System::Direct method.
 *
 * Revision 2.6  1993/03/30  20:00:51  tiller
 * I seem to have fixed some bugs in how boundary conditions are handled.
 * In addition, Everything seems to be working...next I have to check
 * for some phantom sensitivities...hopefully they will be gone.  There
 * does seem to be a problem with the sensitivities be permuted by the
 * System::Direct method.
 *
 * Revision 2.5  1993/03/23  23:15:45  tiller
 * Tangent terms appears to be correct.  They have been tested on a ~200
 * node problem and appear to work correctly.
 *
 * Revision 2.5  1993/03/23  23:15:45  tiller
 * Tangent terms appears to be correct.  They have been tested on a ~200
 * node problem and appear to work correctly.
 *
 * Revision 2.4  1993/03/15  14:30:23  tiller
 * The 'Finite' abstraction seems to be working.
 *
 * Revision 2.4  1993/03/15  14:30:23  tiller
 * The 'Finite' abstraction seems to be working.
 *
 * Revision 2.3  1993/03/07  17:54:00  tiller
 * Things seem to work for the Non-Linear Heat Transfer examples.
 * I'm about to change the interfaces for some optimization reasons.
 * For now, everything seems stable.
 *
 * Revision 2.3  1993/03/07  17:54:00  tiller
 * Things seem to work for the Non-Linear Heat Transfer examples.
 * I'm about to change the interfaces for some optimization reasons.
 * For now, everything seems stable.
 *
 * Revision 2.2  1993/02/13  18:04:06  tiller
 * Added support for a default domain in which the Nodes
 * are constructed into...the overall benefit of this is
 * that when elements are created they can include
 * the correct DOF's on the appropriate nodes.
 *
 * Revision 2.1  1993/02/04  00:26:30  tiller
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
#ifndef	_NODE_H_
#define	_NODE_H_

#ifdef	__GNUC__
#pragma interface
#endif

#include	<space/coord3d.h>
#ifdef	USE_SET
#include	<Set/AssocSet.h>
#else
#include	<Gen/AArray.h>
#endif

class	Domain;
class	Node
{
	private:
		Node();
		Node(const Node&);
		Node& operator=(const Node&);
	protected:
#ifdef	USE_SET
		AssocSet<short, long> map;
#else
		AArray<short, long> map;
#endif
		Coord3D	pos;
		int		number;
	public:
//		static	Domain	*current;

		Node(double x);
		Node(double x, double y);
		Node(double x, double y, double z);
		Node(const Coord3D&);

//		void	AddDOF(short idx);
//		long	dof(short idx) { return (*this)[idx]; }
		inline	long	dof(short idx) { return map[idx]; }
		inline	Coord3D	&Pos() { return pos; }
		inline	void	SetNumber(int n) { number = n; }
		inline	int		Number() { return number; }
#ifdef	USE_SET
		inline	AssocSet<short,long>& Map() { return map; }
#else
		inline	AArray<short,long>&	Map() { return map; }
#endif

		virtual	Coord3D	Par(int bn) { Coord3D ret(0.0,0.0,0.0); return ret; }
};

class	TestNode : public Node
{
	protected:
		Coord3D	par;
	public:
		TestNode(double cx,double cy,double cz,double dx,double dy,double dz);
		Coord3D Par(int);
};

class	StdNode : public Node
{
	public:
		StdNode(double cx,double cy,double cz);
		Coord3D Par(int);
};

#endif

