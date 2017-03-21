#ifndef	_DOMAIN_H_
#define	_DOMAIN_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * File : $Id: Domain.h,v 3.6 1993/06/10 13:58:10 tiller Exp $
 */


#ifdef	__GNUC__
#pragma interface
#endif

#include	<iostream.h>
#include	<Vector.h>

#include	"SimObject.h"
#include	"Design.h"
#include	"VecSpec.h"
#include	"Objective.h"

class	Vector;
class	Element;
class	Essential;
class	Node;
class	Const;
class	BIMatrix;
class	GenSystem;
class	Integrator;

class Domain : public SimObject
{
	private:
		Domain();
		Domain(const Domain&);
		Domain& operator=(const Domain&);
	protected:
		Integrator	&integrator;
	public:
		Domain(Integrator &in);

		// Domain interface adds the following
		virtual	int		Solve(int senses=0) = 0;
		virtual	void	NodalReport(const Vector &, char *, ostream&) = 0;
		virtual	void	SetTime(double) = 0;

		// Related to sub-domains
		virtual	int		NumSubs() = 0;
		virtual	Domain	&Sub(int) = 0;
};

class	LocalDomain : public Domain
{
	private:
		LocalDomain();
		LocalDomain(const LocalDomain&);
		LocalDomain& operator=(const LocalDomain&);
	public:
		LocalDomain(Integrator &);
		// Methods to serve local process (use of pointers)
		virtual	void	AddNode(Node *) = 0;
		virtual	void	AddElem(Element *) = 0;
		virtual	void	AddBC(Essential *) = 0;
		virtual	void	AddDOF(Node *,short) = 0;
		virtual	const	FixedSet<Node *>& NodeSet() const = 0;
		virtual	const	FixedSet<Element *>& ElemSet() const = 0;
		virtual	const	FixedSet<Essential *>& BCSSet() const = 0;
};

#endif
