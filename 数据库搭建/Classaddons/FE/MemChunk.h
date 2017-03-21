#ifndef	_MEM_CHUNK_H_
#define	_MEM_CHUNK_H_

#include	<Misc/GC.h>

// 
// Class : MemChunk
//
//	This class is designed to provide very fast dynamic memory requests
// to subroutines and also to assure that bounds on the memory and freeing
// of the memory are enforced
//

class	MemChunk
{
	protected:
		static	BlockAllocator allocator;
		void *data;
		int	used, length, size;
	public:
		MemChunk(int nelem, int sz);
		void Resize(int nlen);
		void *Chunk(int len);
		~MemChunk();
};

class	DoubleChunk : public MemChunk
{
	public:
		DoubleChunk(int nelem);
		double *Vec(int len);
};
#endif