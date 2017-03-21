/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 *
 * $Id: GC.h,v 1.7 1993/06/10 21:02:09 tiller Exp tiller $
 * ChangeLog:
 *	$Log: GC.h,v $
 * Revision 1.7  1993/06/10  21:02:09  tiller
 * Added support for USE_SET and tested it.  It appears to work.
 *
 * Revision 1.6  1993/02/25  22:44:05  tiller
 * Minor mods
 *
 * Revision 1.5  1993/02/22  19:25:56  tiller
 * Fixed problems associated with alignment and added a linked-list
 * free memory list.
 *
 * Revision 1.5  1993/02/22  19:25:56  tiller
 * Fixed problems associated with alignment and added a linked-list
 * free memory list.
 *
 * Revision 1.4  1993/02/21  23:22:53  tiller
 * Everything appears to be stable at this point
 *
 * Revision 1.4  1993/02/21  23:22:53  tiller
 * Everything appears to be stable at this point
 *
 * Revision 1.3  1993/02/19  18:51:23  tiller
 * Implemented AllocCollector
 *
 * Revision 1.2  1993/02/19  15:16:18  tiller
 * Various Improvements (I hope)
 *
 */

#ifdef	__GNUC__
#pragma interface
#endif
/*
 * This is a garbage collection class.  Any class derived from this
 * class will automatically be cleaned up (this assumes the user is
 * intelligent enough to make sure he/she references that member
 * appropriately.  This class is fairly robust! (IMHO)
 */

#ifndef	_GC_H_
#define _GC_H_

#include	<iostream.h>
#include	<stddef.h>
#include	<Set/SimpleSet.h>

//
// Class: GC
//
//    Provides garbage collection capabilities.  All this class really
// has is a reference count and some smart ref/unref methods
//

class	GC
{
	protected:
		int	ref_count;

	public:
		GC();
		void	ref();
		int	ReferenceCount() { return ref_count; }
		virtual	void	unref();
		virtual ~GC();
		friend class AllocCollector;
};

//
// Class: GCCollector
//
//    Pure virtual class that other Collectors can be derived from.
// Really serves no useful purpose.
//

class GCCollector
{
	public:
		virtual void Clean() = 0;
		virtual ~GCCollector();
};

//
// Class: Simple Collector
//
//    This collector keeps pointers to GC objects and will go through
// removing these objects (when directed to) when their reference
// counts get to -1
//


class SimpleCollector : public GCCollector
{
	protected:
		SimpleSet<GC *>	allocated;
	public:
		SimpleCollector();
		void Add(GC *);
		void Clean();
		void Clean(int);
		void Print();
		virtual ~SimpleCollector();
};

//
// Class: BlockAllocator
//
//    A BlockAllocator is designed to handle memory management in much
// the same way as malloc/realloc/free.  It tries to make use of already
// allocated memory.  It can be used in conjunction with overloaded operator
// new.
//

class	BlockAllocator
{
	protected:
		struct	Pool
		{
			public:
				Pool() { ptr=NULL; size=0; used=0; }
				Pool(void *p, size_t s, int u) { ptr=p; size=s; used=u; }
				void	*ptr;
				size_t	size;
				int		used;
		};
		size_t	block_size;
		SimpleSet<BlockAllocator::Pool>	pools;

	public:
		BlockAllocator(size_t);
		void *Malloc(size_t);
		void *Realloc(void *, size_t);
		void Free(void *);
		virtual	~BlockAllocator();
};

//
// Class: AllocCollector
//
//    An AllocCollector acts similar to a BlockAllocator, except that it
// assumes that any objects being allocated are derived from GC.  That
// way it can check for unused memory by examining the ref_count.  An
// AllocCollector is restricted to allocating objects of a fixed size.
//

class AllocCollector : public GCCollector
{
	protected:
		struct	Header { struct Header *next; void *padding; };
		size_t	block_size;
		size_t	obj_size;
		size_t	pool_size;
		int		num_blocks;
		Header *head;
		SimpleSet<char *>	pools;	// char* to prevent some conflicts

		void AddPool();
		inline	void Prepend(Header *h)
		{
			h->next=head;
			head = h;
		}
		inline	GC *PtrToObj(Header *h)
		{
			h++;
			return (GC *)h;
		}
		inline	Header *Block(void *p, int i)
		{
			return (Header *)(p+block_size*i);
		}
	public:
		AllocCollector(size_t, int);
		void *New();
		void Delete(void *);
		void Print(int stat=0);
		void Clean();
		size_t	ObjSize() { return obj_size; }
};

//
// Class: AllocGC
//
//    This class tries to simplify the interface to AllocCollector
//

struct	AllocCollectorInfo
{
	AllocCollector	*ac;
	int size, bsize;
};

class AllocGC : public GC
{
	protected:
		static	void	*New(size_t,AllocCollector &);
		static	void	Delete(void *,size_t,AllocCollector &);
		void Delete();

		// These used to be inside the class
		void unref() { GC::unref(); AllocGC::Delete(); }
};

// This hokus pokus is necessitated by the fact that C++ doesn't
// have virtual static types, so it is necessary to define a static
// and declare it for each subclass of AllocGC, this attempts to make
// that as painful as possible.

#define	AllocGCInterface	void *operator new(size_t); \
							void operator delete(void *, size_t); \
							static AllocCollector my_ac

#define	AllocGCImplement(n,bs) \
void * \
n##::operator new(size_t size) \
{ \
	return AllocGC::New(size, my_ac); \
} \
void \
n##::operator delete(void *ptr, size_t size) \
{ \
	AllocGC::Delete(ptr, size, my_ac); \
} \
AllocCollector	n##::my_ac(sizeof(n), bs)

#endif


