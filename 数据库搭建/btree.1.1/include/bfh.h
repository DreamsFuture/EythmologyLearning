/*
//	(c) 1990,1991,1995 Larry A. Walker
//	This source is proprietary information which cannot be distributed 
//	without written permission of Larry A. Walker
*/

#ifndef BFHH
#define BFHH

/* *****    BFH Block File Handler  ***** */
/*
The block file handler is the basic component for all database and 
indexing operations 
*/

#include <time.h>
#include "debug.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#ifndef SUCCESS
#define SUCCESS 1
#endif
#ifndef FAILED
#define FAILED 0
#endif
#define EMPTY -1

#define BFHBLOCK	512

#define BFHBLOCKINUSE	0x01

#define CREATE	0000400

#define FILESEP	'/'

struct BfhInfo {
		/* Versions begin with a release number, and 
		   can be followed by more detailed numbers */
	char    version[9];
		/* This is the file descriptor when the file 
		   is open. */
	int 	file_fd;  
		/* first 99 char of the file name */
	char 	file_name[100] ;
		/* Unique identification */
	char *  file_ptr;
        long    magic;          
		/* Original date of the file was created */
        time_t  date_created;   
		/* Last time the file opened */
	time_t  date_opened;
		/* Adjusted when record written */
        time_t  date_updated;
		/* Physical block size for records*/
        int     block_size;   /* real block size which includes user
				 requested size + 1 byte overhead */
		/* First free block in free block list - or -1L*/
        long    first_free_block_addr;
		/* Last free block in free block list - or -1L*/
        long    last_free_block_addr;
		/* This is the address of the last byte in the file */
        long    add_block_addr;
		/* The mask is or'd with the use switch and has seven
		   bits available for future things like marking the
		   block as compressed, or encrypted etc.
		*/
	char	mask;
	char *  empty_rec; /* pointer to the empty record that is used to 
			      create new space */
};

typedef struct BfhInfo BFH;

/*
*	--- Describes the content of a free user data block, resulting
*	    in a double link list of free items in the file.
*/

struct free_block_id {
        /* the switch always resides outside the*/
        /* user block area and the free_block_id is in*/
        long    previous_block;
        long    next_block;
};

/*
*	---  Function prototypes
*/

BFH *BfhOpen( char * , void * , int , int, int );

int BfhClose( BFH *, void *, int);
	
int BfhWriteBlock(BFH *, char *, long);
				   
int BfhReadBlock(BFH *, char *, long);
				  
long BfhNewBlock(BFH *);	     
			
int BfhFreeBlock(BFH *, long);
			
long BfhAddrBlock(BFH *, int);
			
int BfhFullBlock(BFH *, int);

void PrintBfhDescriptor( BFH * );

			
#endif /*BFHH*/
