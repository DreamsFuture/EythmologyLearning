#ifndef	_GP_INFO_H_
#define	_GP_INFO_H_

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: GPInfo.h,v 3.4 1993/06/10 20:59:29 tiller Exp tiller $
 * ChangeLog:
 *	$Log: GPInfo.h,v $
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
 * Revision 3.2  1993/05/20  15:13:21  tiller
 * In anticipation of doing some parallel solution schemes, I'm creating
 * a hierarchy of Domain subclasses.
 *
 * Revision 3.2  1993/05/20  15:13:21  tiller
 * In anticipation of doing some parallel solution schemes, I'm creating
 * a hierarchy of Domain subclasses.
 *
 * Revision 3.1  1993/05/17  22:37:42  tiller
 * This is version 3.1 of my Finite Element class library.  It represents
 * work done in completion of ME 497 in Spring '93.
 *
 * Revision 1.2  1993/05/17  22:03:37  tiller
 * *** empty log message ***
 *
 */

#include	<Set/SimpleSet.h>

#if	0
#include	"GaussContext.h"
#else
class	GaussContext;
#endif

class	Transformation;

class	GPInfo : public FixedSet<GaussContext *>
{
	private:
		GPinfo(const GPInfo&);
		GPInfo& operator=(const GPInfo&);
	protected:
		SimpleSet<GaussContext *>	gcs;
	public:
		GPInfo() { }
		int	Size() const { return gcs.Size(); }
		int Get(int p, GaussContext *&gc)
		{
			return gcs.Get(p,gc);
		}
		int Get(int p, GaussContext *&gc) const
		{
			return gcs.Get(p,gc);
		}
		inline GaussContext *operator[](int i) { return gcs[i]; }
		inline GaussContext *operator[](int i) const { return gcs[i]; }
		const FixedSet<GaussContext *>& set() { return gcs; }
		void Append(GaussContext *gcp);
		void Transform(Transformation&, GPInfo&) const;
		int ParTransform(Transformation&, GPInfo&, int) const;
		void Empty();
		virtual ~GPInfo();
};

#endif
