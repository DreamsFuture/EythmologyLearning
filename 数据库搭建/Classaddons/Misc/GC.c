/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */
#ifdef	__GNUC__
#pragma implementation
#endif

#include	"GC.h"
#include	<assert.h>
#include	<new.h>

// Class: GC

GC::GC()
{
	ref_count = 0;
}

void
GC::ref()
{
	ref_count++;
}

// If, when the ref_count is decremented, the new ref_count is 0,
// it is decremented to -1.  This is because it is possible for
// a newly created GC object to have a ref_count of 0 and removing
// such an object is not a good idea.  This way, it is safe to
// remove an object with a ref_count of -1, not 0
void
GC::unref()
{
	switch(ref_count)
	{
		case 0:
			cerr << "Unreferencing an object with ref_count already at 0" << endl;
			break;
		case 1:
			ref_count = -1;
			break;
		default:
			ref_count--;
			break;
	}
}

// The destructor warns if the ref_count is too low
GC::~GC()
{
	if (ref_count>0)
		cerr << "GC Object destructed with a ref_count of " << ref_count << endl;
}

// Class: GCCollector

// The destructor is here because it is virtual and I seem to recall
// having trouble with pure virtual destructors...it shouldn't make
// any difference as far as performance is concerned.
GCCollector::~GCCollector() { }

// Class: SimpleCollector

SimpleCollector::SimpleCollector() { }

// This function should be called from the constructor of any object
// that is being collected into a SimpleCollector.
void
SimpleCollector::Add(GC *gc) { allocated+=gc; }

// Remove everyone with a reference count of -1 (see Note is GC::unref)
void
SimpleCollector::Clean()
{
	int i;

	foreach(i,allocated)
		if (allocated[i]->ReferenceCount()==-1)
		{
			Clean(i);
			i--;
		}
}

// Remove a specific GC object
void
SimpleCollector::Clean(int p)
{
	delete allocated[p];
	allocated.Remove(p);
}

// Print the array of allocated objects and there associated reference
// counts
void
SimpleCollector::Print()
{
	int	i;

	cout << "[ ";
	foreach(i,allocated)
	{
		cout << allocated[i]->ReferenceCount() << " "; 
		if (i%20==0)
			cout << endl;
	}
	cout << "]" << endl;
}

SimpleCollector::~SimpleCollector()
{
	Clean();
	if (allocated.Size()!=0)
		cerr << "Looks like some temporaries didn't get destroyed!" << endl;
}

// Class: BlockAllocator

// The constructor initializes 30 'Pools' of memory.  Nothing is allocated
// in these pools though.
BlockAllocator::BlockAllocator(size_t s) : block_size(s)
{
	pools.SetSize(30);
}

// Checks for unused pools then begrudgingly allocates memory
void *
BlockAllocator::Malloc(size_t s)
{
	int	i;
	void	*mem;

	foreach(i,pools)
		if (!pools[i].used && pools[i].size>=s)
			return(pools[i].ptr);
	foreach(i,pools)
		if (pools[i].ptr==NULL)
		{
			size_t	bs;
			for(bs=0;bs<s;bs+=block_size);
//			pools[i] = Pool(new char[bs], bs, 1);
			mem = malloc(bs);
			pools[i] = Pool(mem, bs, 1);
			return pools[i].ptr;
		}

	size_t	bs;
	for(bs=0;bs<s;bs+=block_size);
//	pools+=Pool(new char[bs], bs, 1);
	mem = malloc(bs);
	pools+=Pool(mem, bs, 1);
	return pools.Last().ptr;
}

// Checks to see if the current block is big enough, if not, calls
// Malloc and copies the contents
void *
BlockAllocator::Realloc(void *p, size_t s)
{
	int	i;
	size_t	old_size;
	void	*newp;

	foreach(i,pools)
		if (pools[i].ptr==p)
		{
			if (pools[i].size >= s)
				return p;
			else
			{
				old_size = pools[i].size;
				newp = Malloc(s);
				memcpy(p,newp,old_size);
				return newp;
			}
		}

	cerr << "Error in 'void *BlockAllocator::Realloc'" << endl;
	cerr << "  Called for block that hasn't been previously allocated." << endl;
	assert(0);
	// GCC whines because my header files don't declare assert volatile
	// The next line should never be reached.
	return NULL;
}

// Marks a given block as free
void
BlockAllocator::Free(void *p)
{
	int	i;

	foreach(i,pools)
		if (pools[i].ptr == p)
		{
			pools[i].used = 0;
			return;
		}

	cerr << "Error in 'void BlockAllocator::Free'" << endl;
	cerr << "  Called for block that hasn't been previously allocated." << endl;
	assert(0);
}

// Remove memory allocated
BlockAllocator::~BlockAllocator()
{
	int	i;

//	foreach(i,pools)
//		delete [] pools[i].ptr;
	foreach(i,pools)
		free(pools[i].ptr);
}

// Class: AllocCollector

// The constructor takes the size of the objects to be allocated plus
// a blocking factor
AllocCollector::AllocCollector(size_t o, int b)
	: block_size(o+sizeof(Header)), pool_size(b*block_size),
	  obj_size(o), num_blocks(b)
{
	head = NULL;
}

// This adds a 'Pool' of memory and makes it so that all memory appears
// to have a GC object with ref_count=-2.  This is because objects with
// ref_counts of -1 have to have their destructors called, objects with
// ref_counts of -2 dont't
void
AllocCollector::AddPool()
{
	void	*ptr = (void *)valloc(pool_size);
	GC	*gc;
	Header	*h;
	int	i;

	pools.Append(ptr);
	for(i=0;i<num_blocks;i++)
	{
		h = Block(ptr,i);
		Prepend(h);
		gc = PtrToObj(h);
		gc->ref_count=-2;
	}
}

// This function is typically called by an operator new() function.
// It scans all the available 'Pools' for memory.
void *
AllocCollector::New()
{
	int	i;
	Header	*ret;
	void	*vret;

	if (head==NULL)
	{
		AddPool();
		assert(head!=NULL);
	}

	ret=head;
	head=head->next;	
	vret = PtrToObj(ret);
	return	vret;
}

// This function does next to nothing except to check for some run-time
// conditions
void
AllocCollector::Delete(void *ptr)
{
	Header	*h=(Header *)ptr;
	GC	*gc = (GC *)ptr;

	h--;
	Prepend(h);
	assert(gc->ref_count<0);
}

// Print status of all pools
void
AllocCollector::Print(int stat)
{
	Header	*h;
	int	count = 0;

	if (stat)
		cout << "Allocator " << this << " Status" << endl;
	cout << "Number of Pools " << pools.Size() << endl;
	cout << "   Total Size " << pools.Size()*pool_size << " Bytes" << endl;
	cout << "              " << pools.Size()*num_blocks << " Objects" << endl;

	for(h=head;h!=NULL;h=h->next)
		count++;

	cout << count << " Free Objects" << endl;		
}

// Can't really do anything.  You could check for pools that only have objects
// reference counts below 0, but the chances are so slim...it's not worth it
void
AllocCollector::Clean()
{
}

// Class: AllocGC

// A typedef that simplifies things
typedef	void	(*PEHF)();

// This permits an object to delete itself...basically, this calls
// the objects destructor and sets ref_count to -2
void
AllocGC::Delete()
{
	GC	*gc;
	if (ref_count<=0)
	{
		// We need to call the destructor, not delete, but this will
		// have to do!
#if	0
		gc = this;
		gc->GC::~GC();
#else
		gc = this;
		delete this;
#endif

		// I can do this assuming that delete didn't really do anything
		gc->ref_count = -2;
	}
}
// This really doesn't do anything.  (see Note: AllocCollector::Delete)
void
AllocGC::Delete(void *p, size_t s, AllocCollector& ac)
{
	if (s!=ac.ObjSize())
		::delete p;

	ac.Delete(p);
}

// This is approximately lifted from Scott Meyer's 'Effective C++'
void *
AllocGC::New(size_t s, AllocCollector &ac)
{
	void	*ptr;

	if (s!=ac.ObjSize())
		return ::new char[s];

	while(1)
	{
		ptr = ac.New();
		if (ptr!=NULL)
			return ptr;

		PEHF currentHandler = set_new_handler(0);
		set_new_handler(currentHandler);

		if (currentHandler)
			(*currentHandler)();
		else
			return 0;
	}
}


