#ifndef	_SCELEMENT_H_
#define	_SCELEMENT_H_

#include	"Element.h"
#include	<Scalar/Scalar.h>
#include	"Local.h"

// ScElement
//
// A Scalar Element...solid and reliable, but slow as molasses.  This
// guy used to be the definition for Element but it was just too slow.
// I'm thinking of how to retire this guy and still hold on to some of
// the handiness of Scalars in general

class	ScElement : public Element
{
	public:
		virtual	void Stiff(BIMatrix&, Domain&);
		virtual	void TanStiff(BIMatrix&, Domain&);
		virtual	void ParStiff(BIMatrix&, Domain&, int);
		virtual	void Force(SVector&, Domain&);
		virtual	void TanForce(BIMatrix&, Domain&);
		virtual	void ParForce(SVector&, Domain&, int);

		virtual	void Stiff(LocMat&,Domain&,const ScVector&) = 0;
		virtual	void Force(LocVec&,Domain&,const ScVector&) = 0;

//		Derivatives...default, to Automatic Differentiation
		virtual	void DStiff(LocMat&, Domain&, const ScVector&);
		virtual	void DStiff(LocMat&, Domain&, const ScVector&, int);
		virtual	void DForce(LocMat&, Domain&, const ScVector&);
		virtual	void DForce(LocVec&, Domain&, const ScVector&, int);
};

#endif