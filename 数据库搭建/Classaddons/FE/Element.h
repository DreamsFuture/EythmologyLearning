#ifndef	_ELEMENT_H_
#define	_ELEMENT_H_

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
 * File : $Id: Element.h,v 3.5 1993/06/10 20:59:29 tiller Exp tiller $
 */

#include	<Set/SimpleSet.h>

class Node;
class Domain;
class LocalDomain;
class BIMatrix;
class SVector;

class Element
{
	protected:
		SimpleSet<Node *> nlist;
	public:
		Element() { }
		const FixedSet<Node *>&	nodes() const;
		virtual void Register(LocalDomain&) = 0;
		long EqNum(int node, short dof);
		virtual ~Element() { }

		virtual	void Stiff(BIMatrix&, LocalDomain&) = 0;
		virtual	void TanStiff(BIMatrix&, LocalDomain&) = 0;
		virtual	void ParStiff(BIMatrix&, LocalDomain&, int) = 0;

		virtual	void Force(SVector&, LocalDomain&) = 0;
		virtual	void TanForce(BIMatrix&, LocalDomain&) = 0;
		virtual	void ParForce(SVector&, LocalDomain&, int) = 0;
};

#endif
