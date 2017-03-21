/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 * $Id: GCPtr.h,v 1.5 1993/06/10 21:02:09 tiller Exp tiller $
 * ChangeLog:
 *	$Log: GCPtr.h,v $
 * Revision 1.5  1993/06/10  21:02:09  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 1.4  1993/02/25  22:44:05  tiller
 * Minor mods
 *
 * Revision 1.3  1993/02/22  19:25:56  tiller
 * Fixed problems associated with alignment and added a linked-list
 * free memory list.
 *
 * Revision 1.3  1993/02/22  19:25:56  tiller
 * Fixed problems associated with alignment and added a linked-list
 * free memory list.
 *
 * Revision 1.2  1993/02/21  23:22:53  tiller
 * Everything appears to be stable at this point
 *
 * Revision 1.2  1993/02/21  23:22:53  tiller
 * Everything appears to be stable at this point
 *
 * Revision 1.1  1993/02/19  15:16:18  tiller
 * Initial revision
 *
 * Revision 1.1  1993/02/19  15:16:18  tiller
 * Initial revision
 *
 */

/*
 *	This is used as a pointer to GC...it is a convenience because
 *	it does the reference counting for you.
 */

#include	<Misc/GC.h>
#include	<Set/SimpleSet.h>

template<class T>
class	GCPtr
{
	private:
		T	*ptr;
	public:
		GCPtr()
		{
			ptr = NULL;
		}
		GCPtr(const GCPtr& g)
		{
			ptr = g.ptr;
			ptr->ref();
		}
		GCPtr(T *a) : ptr(a)
		{
			ptr = a;
			ptr->ref();
		}
		inline	T *operator->() { return(ptr); }
		inline	T& operator*() { return(*ptr); }
		inline	T *operator=(T *a)
		{ 
			if (ptr!=NULL)		// I think
				ptr->unref();	// this belongs here
			ptr = a;
			ptr->ref();
			return(ptr);
		}
		inline	GCPtr& operator=(const GCPtr &a)
		{ 
			if (ptr!=NULL)		// I think
				ptr->unref();	// this belongs here
			ptr = a.ptr;
			ptr->ref();
			return(*this);
		}
		~GCPtr()
		{
			ptr->unref();
		}
};


