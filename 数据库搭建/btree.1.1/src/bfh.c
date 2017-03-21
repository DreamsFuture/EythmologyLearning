/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                Larry A. Walker & Co. B+Tree R2
        Corp. 1991,1995 Larry A. Walker (Larry A. Walker & Co.)
                        All rights reserved.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "bfh.h"
#include "debug.h"

extern void free(void*);

#define RELEASECODE	"CFW01.01"


/*
//
// NAME:	BfhOpen
//
//
// RETURN:	NULL on error, or a pointer to the open descriptor (BFH *)
//
// DESCRIPTION:	Build a Block File
//             	Open routine for the block file handler.  This routine requires
//		a character pointer to the file name, a pointer to a descriptive
//		structure that the user programs maintain concerning the 
//		contents of the file, the size of this structure, a key 
//		indicating if the file is to be created, and the block size of
//		the file, including a one byte overhead.
//
*/

BFH * BfhOpen(	char * file_name, /* pointer to block file name */
			void * vptr, /* pointer to user description data */
			int vptr_size, /* size of the user description */
			int create , /* true or false create file switch */
			int size /* single block size */)
{
	int open_fd;
	BFH * bfh;
	char empty_block[BFHBLOCK];
	char * ptr;

	if(file_name == NULL){
		Debug("Must have a file name\n");
		return NULL;
	}

	Trace("BfhOpen(%s, %x, %d, %d, %d )\n", file_name, (unsigned)vptr, 
		vptr_size, create, size);

	if(vptr_size != 0){
		if(vptr_size > BFHBLOCK){
			Debug( "Invalid user data size. Quitting.\n");
			return NULL;
		}
	}
	/*
		--- Create a handle -- descriptor
	*/
	bfh = (BFH *)calloc(1,sizeof(struct BfhInfo));
	if(bfh == NULL){
		Debug( "Memory allocation failed. Quitting.\n");
		return NULL;
	}
	
	/*
		--- Initialize it
	*/
	bfh->file_fd = -1;
	bfh->file_ptr = NULL;
	bfh->file_name[0] = 0;

	/* the size of the block requested must be atleast this large*/
	if(size < sizeof (struct free_block_id) ) {
		size = sizeof(struct free_block_id) ;
		Debug("Force adjust blocksize\n");
	}
	++size;	/* the size must be adjusted to accommodate the switch */

	memset(empty_block, 0, BFHBLOCK);

	/*
		-- Create an empty file as necessary
	*/
	if(create == CREATE){
		char Nil = 0;
		if((sizeof(BFH) > BFHBLOCK) || ( vptr_size > BFHBLOCK))
		{
			Debug("Header contents too big to fit into block.  Block = %d, bfh block = %d, user block = %d\n", BFHBLOCK, sizeof(BFH), vptr_size);
		}
		open_fd = open(file_name,
			O_RDWR|O_CREAT|O_EXCL,0666);
		if(open_fd != -1){
			strcpy(bfh->version, RELEASECODE);
			bfh->magic = 7321L;
			bfh->date_created = time(NULL);
			bfh->date_opened = time(NULL);
			bfh->date_updated = time(NULL);
			bfh->block_size = size; /* Remember, the 
				requested size plus one */
			bfh->first_free_block_addr = -1L;
			bfh->last_free_block_addr = -1L;
			bfh->add_block_addr = 2*BFHBLOCK;
			bfh->mask = 0;

			/* write the bfh info block */
			memmove(empty_block,bfh,sizeof(struct BfhInfo));
			if(write(open_fd,empty_block,BFHBLOCK) 
				!= BFHBLOCK){
				Debug("bfh write failure\n");
				free((void *)bfh);
				return NULL;
			}
			memset(empty_block, 0, BFHBLOCK);

			/* write the user program block */
			if( (vptr_size > 0) && (vptr != NULL)){
				memmove(empty_block,vptr,vptr_size);
			}
			if(write(open_fd,empty_block,BFHBLOCK) != BFHBLOCK){
				Debug("bfh_data write failure\n");
				free((void*)bfh);
				return NULL;
			}
			/* write a single character position for seeks to land on */
			if(write(open_fd,&Nil,1) != 1){
				Debug("bfh_data write failure\n");
				free((void*)bfh);
				return NULL;
			}
			close(open_fd);
		} else{
			Debug("Failed to create file \"%s\"\n", file_name);
			free((void*)bfh);
			return NULL;
		}
	}
	/* Save the file name and put a pointer in the descriptor to the addr */
	ptr = strrchr(file_name,FILESEP);
	if(ptr != NULL){
		strncpy(bfh->file_name, ptr, 99);
	}else{
		strncpy(bfh->file_name, file_name, 99);
	}
/*
	bfh->file_ptr = (char *)malloc( strlen(file_name) + 1);
	if(bfh->file_ptr == NULL){
		Debug( "Memory allocation failed. Quitting.\n");
		free((void*)bfh);
		return NULL;
	}
	strcpy(bfh->file_ptr, file_name);
*/
	open_fd = open(file_name, O_RDWR);
	if(open_fd != -1){
		/* Lock the file until the close */
		if(lockf(open_fd, F_LOCK, 0) == -1){
			Debug("Failed to lock file \"%s\"\n", file_name);
			free((void*)bfh);
			return NULL;
		}
		/* Read the bfh info block */
		if(read(open_fd,empty_block,BFHBLOCK) != BFHBLOCK){
			Debug("bfh_data read failure\n");
			free((void*)bfh);
			return NULL;
		}
		memmove(bfh,empty_block,sizeof(struct BfhInfo));
		if(bfh->magic != 7321L){
			Debug("Not a known B+Tree\n");
			return NULL;
		}
		/* 
			This code will only understand CFW index versions. 
			( C freeware )
		*/
		if(strncmp(bfh->version, "CFW", 3) != 0){
			Debug("Unknown release %s\n", bfh->version);
			return NULL;
		}
		if(bfh->magic != 7321L){
			Debug("Not a BFH file\n");
		}
		/* Read the user info block */	
		if(read(open_fd,empty_block,BFHBLOCK) != BFHBLOCK){
			Debug("user data read failure\n");
			free((void*)bfh);
			return NULL;
		}
		/* Give the user a copy of the user data */
		if( (vptr_size > 0) && (vptr != NULL)){
			memmove((char *)vptr,(char *)empty_block,vptr_size);
		}
	} else{
		Debug("Failed to create file \"%s\"\n", file_name);
		free((void*)bfh);
		return NULL;
	}
	bfh->empty_rec = (char *)malloc(bfh->block_size);
	if(bfh->empty_rec == NULL ) {
		return NULL;
	}
	memset(bfh->empty_rec, 0, bfh->block_size);

	/* Save a copy of the fd in the descriptor */
	bfh->file_fd = open_fd;

	
	/* Update the last open date */
	bfh->date_opened = time(NULL);
/*
	if(bfh->file_ptr != NULL){
		free(bfh->file_ptr);
		bfh->file_ptr = NULL;
	}
*/

	return bfh;
}

/*
//
// NAME:	BfhClose
//
// DESCRIPTION:	Close the file and truncate the file of all free blocks which 
//		are at the end of the file
//
// RETURN:	SUCCESS or FAILED
//
*/

int BfhClose(	BFH * bfh, /* pointer to the descriptor */
		void * userdata, /* pointer to the updated user data */
		int userdatasize /* size of the user data */)
{
	long new_end;
	int truncate, endfound;
	struct free_block_id fbi;
	struct free_block_id newfbi;
	char cswitch;
	long present_position;

	Trace("BfhClose(%x, %x, %d)\n", (unsigned)bfh, (unsigned)userdata, 
		userdatasize);
	new_end = -1L;

	if(bfh == NULL){
		Debug("NULL descriptor\n");
		return FAILED;
	}
		/* write out the headers*/
	truncate = 0;
	endfound = 0;
	new_end = bfh->add_block_addr;
	if(new_end > (long)(2*BFHBLOCK)){ /* if we have added blocks */
		do{
			if(new_end <= (long)(2*BFHBLOCK)){
				/* there are no longer any blocks */
				break;
			}
			/* find the last block in the file */
			if(lseek(bfh->file_fd,new_end - bfh->block_size,0) == -1L) {
				Debug( "Failed to seek\n");
				return FAILED;
			}
			present_position = lseek(bfh->file_fd,0L,1);
			/* get the switch for the last block */
			if(read(bfh->file_fd,&cswitch,1) != 1 ) {
				Debug( "Failed to read swithc\n");
				return FAILED;
			}
			if(cswitch & BFHBLOCKINUSE){ /* a full block */
				endfound++;
				break;
			}
			/* otherwise the block can be truncacted */
			if(read(bfh->file_fd,&fbi,sizeof(struct free_block_id)) != sizeof(struct free_block_id) ) {
				Debug( "Failed to read");
				return FAILED;
			}
			/* release a free block from the list*/
			new_end = present_position;
			/*free up this block from the list*/

			/*
				--- The previous free block in the list can't be pointing 
				    forward to the one we are releasing
			*/
                        if (fbi.previous_block != -1L){
				/* go to the previous block */
				if(lseek(bfh->file_fd,fbi.previous_block+1,0) == -1L) {
					Debug( "Failed to seek\n");
					return FAILED;
				}
				/* get its free block identification */
				if(read(bfh->file_fd,&newfbi,sizeof(struct free_block_id)) != sizeof(struct free_block_id) ) {
					Debug( "Failed to read\n");
					return FAILED;
				}
				/* update its info to bypass the block at the end of the file */
				newfbi.next_block = fbi.next_block;
				/* reposition */
				if(lseek(bfh->file_fd,fbi.previous_block+1,0) == -1L) {
					Debug( "Failed to seek\n");
					return FAILED;
				}
				/* write the updated information */
				if(write(bfh->file_fd,&newfbi,sizeof(struct free_block_id)) != sizeof(struct free_block_id) ) {
					Debug( "Failed to read\n");
					return FAILED;
				}
				truncate = 1;
                        }else {
				/* there was no previous block, which means this was the
				   first free block available, and now it is gone */
				bfh->first_free_block_addr = fbi.next_block;
			}
			/*
				--- The next free block in the list can't be pointing 
				    backward to the one we are releasing
			*/
                        if (fbi.next_block != -1L){
				if(lseek(bfh->file_fd,fbi.next_block+1,0) == -1L) {
					Debug( "Failed to seek\n");
					return FAILED;
				}
				/* get its free block identification */
				if(read(bfh->file_fd,&newfbi,sizeof(struct free_block_id)) != sizeof(struct free_block_id) ) {
					Debug( "Failed to read\n");
					return FAILED;
				}
				/* update its info to bypass the block at the end of the file */
				newfbi.previous_block = fbi.previous_block;
				/* reposition */
				if(lseek(bfh->file_fd,fbi.next_block+1,0) == -1L) {
					Debug( "Failed to seek\n");
					return FAILED;
				}
				if(write(bfh->file_fd,&newfbi,sizeof(struct free_block_id)) != sizeof(struct free_block_id) ) {
					Debug( "Failed to read\n");
					return FAILED;
				}
				truncate = 1;
                         }else{
				/* there was no next block, which means this was the
				   last free block available, and now it is gone */
					bfh->last_free_block_addr = fbi.previous_block;
   				}
		}while(endfound == 0);
	}
	if(truncate)
		if(new_end >= (2*BFHBLOCK) )
			bfh->add_block_addr = new_end;
	/* save the descriptor in the file */
	if(lseek(bfh->file_fd,0L,0) == -1L){
		Debug( "Failed to seek\n");
		return FAILED;
	}
	if(write(bfh->file_fd,bfh,sizeof(struct BfhInfo)) != sizeof(struct BfhInfo)){
		Debug( "close write failure\n");
		return FAILED;
	}
	/* save the user information into the file */
	if(userdata != NULL){
		if(lseek(bfh->file_fd,(long)BFHBLOCK,0) == -1L){
			Debug( "Failed to seek\n");
			return FAILED;
		}
		if(write(bfh->file_fd,userdata,userdatasize) != userdatasize){
			Debug( "close write failure\n");
			return FAILED;
		}
	}
	/* if the file was shortened, fix it in the file system */
	if(truncate){
		ftruncate(bfh->file_fd,new_end+1L);
	}
	if(lockf(bfh->file_fd, F_ULOCK, 0) == -1){
		Debug("Failed to unlock file \"%s\"\n", bfh->file_name);
		free((void*)bfh);
		return FAILED;
	}
	close(bfh->file_fd);
	bfh->file_fd = -1;
	if(bfh->empty_rec != NULL){
		free(bfh->empty_rec);
		bfh->empty_rec = NULL;
	}
	free((void*)bfh);
	return SUCCESS;
}

/*
 NAME:	WriteBlock

 DESCRIPTION: 	Write the block at the char pointer to the indicated position
		in the file

 RETURN:	SUCCESS or FAIL
*/

int BfhWriteBlock(	BFH * bfh, /* pointer to bfh descriptor */
			char *ptr, /* pointer to outgoing data */
			long block_addr /* outgoing data block id */)
{ 
	char key_out;
	char inuse;

	Trace("BfhWriteBlock(%x, %x, %ld)\n", 
		(unsigned)bfh, (unsigned)ptr, block_addr);

	key_out = BFHBLOCKINUSE; 	/* Used bit set*/
	key_out = key_out | bfh->mask;  /* And any other switches being used */

	/* Is the request beyond the last block in the file? */
	if( block_addr > ((bfh->add_block_addr - bfh->block_size) + 1)){
		Debug( "Cant write beyond end of file\n");
		return FAILED;
	}
	/* Is the request below the first block in the file? */
	if(block_addr < ((2*BFHBLOCK) + 1) ){
		Debug( "Invalid block address\n");
		return FAILED;
	}
	/* Is the request on a user block boundry ? */
	if( ( (block_addr - (2*BFHBLOCK) -1) % bfh->block_size ) != 0){
		Debug( "Invalid block address\n");
		return FAILED;
	}
	/* Go to the requested block's switch byte */
	if(lseek(bfh->file_fd, block_addr - 1L,0) == -1L){
		Debug( "Cant seek\n");
		return FAILED;
	}
	if(read(bfh->file_fd, &inuse, 1) == -1){
		Debug( "Cant read key in\n");
		return FAILED;
	}
	inuse &= BFHBLOCKINUSE;
	/* User can only write a block they are already using */
	if(!inuse){
		Debug( "BfhWriteBlock attempt to overwrite free list entry\n");
		return FAILED;
	}
	/* Now we are in position over the user block */
	if(write(bfh->file_fd,ptr,bfh->block_size-1) == -1){
		Debug( "Cant write bdata\n");
		return FAILED;
	}
	bfh->date_updated = time(NULL);
	return SUCCESS;
}

/*
 NAME:	ReadBlock

 DESCRIPTION: 	Read the block at the user specified address

 RETURN:	SUCCESS or FAILED or EMPTY
*/

int BfhReadBlock(	BFH * bfh, /* pointer to descriptor */
			char * ptr, /* address in which to place data */
			long block_addr /* block id */)
{  

	char cswitch;

	Trace("BfhReadBlock(%x, %x, %ld)\n",
		(unsigned)bfh, (unsigned)ptr, block_addr);

	/* Is the request beyond the last block in the file? */
	if( block_addr > ((bfh->add_block_addr - bfh->block_size) + 1)){
		Debug( "Cant read beyond end of file\n");
		return FAILED;
	}
	/* Is the request below the first block in the file? */
	if(block_addr < ((2*BFHBLOCK) + 1) ){
		Debug( "Invalid block address\n");
		return FAILED;
	}
	/* Is the request on a user block boundry ? */
	if( ( (block_addr - (2*BFHBLOCK) -1) % bfh->block_size ) != 0){
		Debug( "Invalid block address\n");
		return FAILED;
	}
	/* Go to the switch byte for the block */
	if(lseek(bfh->file_fd, block_addr - 1L,0) == -1L){
		Debug( "Cant seek\n");
		return FAILED;
	}
	if(read(bfh->file_fd,&cswitch,1) != 1) {
		Debug( "Cant read switch\n");
		return FAILED;
	}
	/* If in use, return day, else return EMPTY */
	if(cswitch & BFHBLOCKINUSE){
		if(read(bfh->file_fd,ptr,bfh->block_size-1) != (bfh->block_size-1)) {
			Debug( "Cant read bdata\n");
			return FAILED;
		}
	}else{
		return EMPTY;
	}
	return SUCCESS;
}

/*
 NAME:	NewBlock

 DESCRIPTION: 	Get the first empty block, or initialize a new block
		and return its user address

 RETURN:	The user address ofthe file of a new block, or the value
		-1L on failure
*/



long BfhNewBlock(BFH * bfh /* poiner to bfh descriptor */)
{
	struct free_block_id free_list_id;
	long blocknumber;
	char cswitch = 0;
	int status;

	Trace("BfhNewBlock(%x)\n", (unsigned)bfh);
	if(bfh==NULL){
		Debug("NULL bfh\n");
		return -1L;
	}
	/* This Case:  No free block in list */
	if(bfh->first_free_block_addr == -1L){
		if(lseek(bfh->file_fd,bfh->add_block_addr,SEEK_SET) == -1L){
			Debug("Cant lseek\n");
			return -1L;
		}
		/* put the switch into place */
		cswitch = bfh->mask | BFHBLOCKINUSE;
		if(write(bfh->file_fd,&cswitch,1) != 1){
 			Debug( "Cant write\n");
			return -1L;
		}
		if((status=write(bfh->file_fd,bfh->empty_rec,bfh->block_size)) != bfh->block_size){
 			Debug( "wrote %d bytes from %d blank bytes\n", status,bfh->block_size);
			return -1L;
		}
		blocknumber = bfh->add_block_addr;
		bfh->add_block_addr += bfh->block_size;
		return ( blocknumber + 1L );
	}
	/* where is the next one?, save it */
	blocknumber = bfh->first_free_block_addr;

	/* This Case: Only one free block*/
	if(bfh->first_free_block_addr == bfh->last_free_block_addr){
		bfh->first_free_block_addr = -1L;
		bfh->last_free_block_addr = -1L;
		if(lseek(bfh->file_fd,blocknumber,0) == -1L){
			Debug("Cant lseek\n");
			return -1L;
		}
		/* put the switch into place */
		cswitch = bfh->mask | BFHBLOCKINUSE;
		if(write(bfh->file_fd,&cswitch,1) != 1){
 			Debug( "Cant write\n");
			return -1L;
		}
		return (blocknumber + 1L) ;
	}
        /* Otherwise there must be more than one*/

        /* This Case: Multiple free blocks*/
	/* Get the free list info */
	if(lseek(bfh->file_fd,bfh->first_free_block_addr + 1,0) == -1L) {
 		Debug( "Cant seek\n");
		return -1L;
	}
	if(read(bfh->file_fd,&free_list_id,sizeof(struct free_block_id)) 
		!= sizeof(struct free_block_id) ){
		Debug( "Cant read\n");
		return -1L;
	}
	/* We'll give the user the first free block on list */
	bfh->first_free_block_addr = free_list_id.next_block;
        /* Go to the next block and mark it as the first*/
	if(lseek(bfh->file_fd,bfh->first_free_block_addr+1,0) == -1L){
 		Debug( "Cant seek\n");
		return -1L;
        }
	if( read(bfh->file_fd,&free_list_id,sizeof(struct free_block_id)) 
		!= sizeof(struct free_block_id) ){
		Debug( "Cant read\n");
		return -1L;
	}
	/* Change it */
       	free_list_id.previous_block = -1L;
	/* Reposition */
	if(lseek(bfh->file_fd,bfh->first_free_block_addr+1,0) == -1L){
 		Debug( "Cant seek\n");
		return -1L;
        }
	/* write it back */
	if( write(bfh->file_fd,&free_list_id,sizeof(struct free_block_id)) 
		!= sizeof(struct free_block_id) ){
		Debug( "Cant write\n");
		return -1L;
	}
	/* set the switch */
	if(lseek(bfh->file_fd,blocknumber,0) == -1L){
		Debug("Cant lseek\n");
		return -1L;
	}
	/* put the switch into place */
	cswitch = bfh->mask | BFHBLOCKINUSE;
	if(write(bfh->file_fd,&cswitch,1) != 1){
 		Debug( "Cant write\n");
		return -1L;
	}
	return (blocknumber + 1L);
}/* end BfhNewBlock(BFH * bfh) */

/*
 NAME:	FreeBlock

 DESCRIPTION:	Add the block at the designated address to the free list

 RETURN:	SUCCESS or FAIL
*/

int BfhFreeBlock(BFH * bfh /*poiner to bfh descriptor */, 
		long user_address /* block ID to free */)
{
	struct free_block_id free_list_id;
	long old_last_block, old_first_block;
	char cswitch;
	long address;

	Trace("BfhFreeBlock(%x, %ld)\n", (unsigned)bfh, user_address);
	address = --user_address;

	/* Is the request beyond the last block in the file? */
	if( address > ((bfh->add_block_addr - bfh->block_size) + 1)){
		Debug( "Request beyond end of file\n");
		return FAILED;
	}
	/* Is the request below the first block in the file? */
	if(address < (2*BFHBLOCK) ){
		Debug( "Invalid block address\n");
		return FAILED;
	}
	/* Is the request on a user block boundry ? */
	if(
	    (((address - (2*BFHBLOCK)) + bfh->block_size)  % bfh->block_size ) 
			!= 0){
		Debug( "Invalid block address\n");
		return FAILED;
	}
	old_last_block = bfh->last_free_block_addr;
	/* if there is already a free block mark the old one*/
	/* go to the last free block*/
	cswitch = 0;
	if(bfh->last_free_block_addr != -1L) {
		/* if it is a high block, put it on the end of */
		/* the list*/
		if(address > bfh->first_free_block_addr){
			if(lseek(bfh->file_fd,bfh->last_free_block_addr+1,0) == -1L){
 				Debug("Couldnt seek\n");
				return FAILED;
			}
			if(read(bfh->file_fd,&free_list_id,sizeof(struct free_block_id)) != sizeof(struct free_block_id)) {
 				Debug("Read failure\n");
				return FAILED;
			}
			/* mark it with the next block id*/
			free_list_id.next_block = address ;
			/* write it back*/
			if(lseek(bfh->file_fd,bfh->last_free_block_addr+1,0) == -1L) {
				Debug("Couldnt seek\n");
				return FAILED;
			}
			if(write(bfh->file_fd,&free_list_id,sizeof(struct free_block_id)) != sizeof(struct free_block_id)) {
 				Debug( "Couldnt write\n");
				return FAILED;
			}
			/* mark the new last block*/
			free_list_id.previous_block = old_last_block;
			free_list_id.next_block = -1L;
			bfh->last_free_block_addr = address;
		}else{
			/* if it is a low block, put it on the front of */
			/* the list*/
			old_first_block = bfh->first_free_block_addr;
			if(lseek(bfh->file_fd,bfh->first_free_block_addr+1,0) == -1L){
       				Debug("Couldnt seek\n");
       				return FAILED;
			}
			if(read(bfh->file_fd,&free_list_id,
				sizeof(struct free_block_id)) != 
				sizeof(struct free_block_id)) 
			{
				Debug("Read failure\n");
				return FAILED;
			}
			/* mark it with the next block id*/
			free_list_id.previous_block = address ;
			/* write it back*/
			if(lseek(bfh->file_fd,bfh->first_free_block_addr+1,0) == -1L) {
				Debug("Couldnt seek\n");
				return FAILED;
			}
			if(write(bfh->file_fd,&free_list_id,
				sizeof(struct free_block_id)) != 
				sizeof(struct free_block_id)) 
			{
				Debug( "Couldnt write\n");
				return FAILED;
			}
			/* mark the new last block*/
			free_list_id.previous_block = -1;
			free_list_id.next_block = old_first_block;
			bfh->first_free_block_addr = address;
		}
	}else{	
		/*bfh->last_free_block_addr == -1L */
		/* the new one will be the only one*/
		if( bfh->last_free_block_addr == bfh->first_free_block_addr ){
			bfh->first_free_block_addr = address;
			bfh->last_free_block_addr = address;
			free_list_id.previous_block = -1L;
			free_list_id.next_block = -1L;
		}else{
			Debug("Cant be a first free block without a last free block\n");
			return FAILED;
		}
	} 
	/* mark the last block*/
	/* set up the free list structure*/

	/* mark the new last block*/
	if(lseek(bfh->file_fd,address,0) == -1L) {
		Debug("Couldnt seek\n");
		return FAILED;
	}
	if(write(bfh->file_fd,&cswitch,1) != 1) {
 		Debug( "Couldnt write\n");
		return FAILED;
	}
	if(write(bfh->file_fd,&free_list_id,sizeof(struct free_block_id)) != sizeof(struct free_block_id)) {
 		Debug( "Couldnt write\n");
		return FAILED;
	}
	return SUCCESS;
}
	
/*
 NAME:	AddrBlock

 DESCRIPTION:	Convert 0 based  block number to a user block address
		representing the data block point

 RETURN:	long address
*/

long BfhAddrBlock(BFH * bfh, /* pointer to bfh descriptor */ 
		  int i /* block sequential number */)
{ /* First file block is 0*/
	long l;

	Trace("BfhAddrBlock(%x, %d)\n", (unsigned)bfh, i);
	l = ( (2*BFHBLOCK) + ((long)i * bfh->block_size) + 1L);
	return ( (l < bfh->add_block_addr)?l:-1L );
}

/*
 NAME:	BfhFullBlock

 DESCRIPTION:	provided a block number, determine if the block is free

 RETURN:	1 for a used block, 0 for a free block, -1 for an error
*/

int BfhFullBlock(BFH * bfh, /* pointer to block file descriptor */
		 int blocknum /* block sequential number */) 
{ 
	long posn;
	char cswitch;

	Trace("BfhFullBlock(%x, %d)\n", (unsigned)bfh, blocknum);
	posn = BfhAddrBlock(bfh,blocknum);
	if(posn == -1L){
		Debug("Invalid position\n");
		return -1;
	}
	if(lseek(bfh->file_fd,posn-1,0) == -1L) {
		Debug( "Couldnt seek\n");
		return -1;
	}
	if(read(bfh->file_fd,&cswitch,1) != 1) {
		Debug( "Couldnt read\n");
		return -1;
	}
	return ( cswitch & BFHBLOCKINUSE);
}

/*.*****************************************************
* Function Name: 	PrintBfhDescriptor
*
* Access from:
*
* Synopsis:
*
* Arguments:
*
* Description:
*
* Return:
*
* History:	Created from B+Tree in C++, 1995
*
* Copr 1991,1992,1995  Tue Mar 21 15:57:56 EST 1995 Larry A. Walker
*
*  Modified:
*
******************************************************/

void PrintBfhDescriptor(BFH * bf /* pointer to block file descriptor */)
{
	if(bf == NULL){
		View("NULL Bfh\n");
		return;
	}
	View("\t\tBfh Info Bfh = %x\n", (unsigned)bf);
	View("version %s, file_fd %d, file_name %s\n", 
		(char *)(bf->version), bf->file_fd, (char *)(bf->file_name));
	View("magic %ld, block_size %d, mask %d\n", 
		bf->magic, bf->block_size, bf->mask);
	View("date_created %sdate_opened %sdate_updated %s\n", 
		ctime(&(bf->date_created)) , ctime(&(bf->date_opened)) , ctime(&(bf->date_updated)));
	View("first_free_block_addr %ld, last_free_block_addr %ld, \nadd_block_addr %ld\n", 
		bf->first_free_block_addr,bf->last_free_block_addr,bf->add_block_addr );
		
} /*end  "PrintBfhDescriptor" */
