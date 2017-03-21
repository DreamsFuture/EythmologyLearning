#include	"MemChunk.h"
#include	<assert.h>

BlockAllocator	MemChunk::allocator(50*sizeof(double));

MemChunk::MemChunk(int nelem, int sz)
{
	size = sz;
	length = nelem;
	data = allocator.Malloc(length*size);
	used = 0;
}

void
MemChunk::Resize(int nlen)
{
	if (used >= nlen)
	{
		cerr << "Error, Cannot shrink a memchunk!" << endl;
		assert(used >= nlen);
	}
	data = allocator.Realloc(data, nlen*size);
	length = nlen;
}

void *
MemChunk::Chunk(int len)
{
	void	*ret;
	assert(used+len <= length);
	ret = data+(used*size);
	used+=len;
	return ret;	
}

MemChunk::~MemChunk()
{
	allocator.Free(data);
}

DoubleChunk::DoubleChunk(int nelem) : MemChunk(nelem, sizeof(double))
{
}

double *
DoubleChunk::Vec(int len)
{
	return (double *)Chunk(len);
}

