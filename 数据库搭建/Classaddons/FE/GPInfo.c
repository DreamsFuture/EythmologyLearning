#include	<stdlib.h>
#include	<math.h>
#include	<iostream.h>
#include	<assert.h>

#include	"GPInfo.h"
#include	"Node.h"
#include	"Element.h"
#include	"Trans.h"
#include	"GaussContext.h"

/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

/*
 * $Id: GPInfo.c,v 3.3 1993/05/25 21:31:08 tiller Exp $
 */

#ifdef	USE_MCHUNK
#include	"MemChunk.h"
#endif
#ifdef	MDEBUG
#include	<dbmalloc.h>
#endif


void
GPInfo::Append(GaussContext *gcp)
{
	gcs+=gcp;
}

void
GPInfo::Transform(Transformation& t, GPInfo &c) const
{
#ifdef	MDEBUG
	malloc_enter("Context::Transform");
#endif
	int	i;
	GaussContext	*gcp;

	c.Empty();
	foreach(i,gcs)
	{
		gcp = new GaussContext(*gcs[i]);
#ifdef	GC_IS_GC
		gcp->ref();
#endif
		gcp->Transform(t);
		c.Append(gcp);
	}
#ifdef	MDEBUG
	malloc_leave("Context::Transform");
#endif

}

int
GPInfo::ParTransform(Transformation& t, GPInfo &c, int bn) const
{
#ifdef	MDEBUG
	malloc_enter("Context::ParTransform");
#endif
	int	i, ic;
	GaussContext *gcp;

	ic = 0;
	c.Empty();
	foreach(i,gcs)
	{
		gcp = new GaussContext(*gcs[i]);
#ifdef	GC_IS_GC
		gcp->ref();
#endif
		ic |= gcp->ParTransform(t, bn);
		c.Append(gcp);
	}

#ifdef	MDEBUG
	malloc_leave("Context::ParTransform");
#endif
	return ic;
}

void
GPInfo::Empty()
{
	int	i;

#ifdef	GC_IS_GC
	foreach(i,gcs)
		gcs[i]->unref();
#else
	foreach(i,gcs)
		delete gcs[i];	
#endif

	gcs.Empty();
}

GPInfo::~GPInfo()
{
	Empty();
}

