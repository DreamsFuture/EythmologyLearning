
/*
----------------------------------------------------------------------

  #####  ######   ####    #####  #####   ######  #    #
    #    #       #          #    #    #  #       #    #
    #    #####    ####      #    #####   #####   ######
    #    #            #     #    #    #  #       #    #
    #    #       #    #     #    #    #  #       #    #
    #    ######   ####      #    #####   #       #    #

----------------------------------------------------------------------
**
**	  Copyright (C) Larry A. Walker & Co
**
** Module    :	 testbfh
** File      :	 testbfh.c
** Functions :	
**	        
**	        
** Written by:	Larry A. Walker
**	        
**	        
** History   :	
**	   	Created: Thu Mar  2 12:47:24 EST 1995
*/

static char sccsid[]="%Z% Module testbfh ( %M% ) %I% %G% %U%";

/*
*	--- Includes
*/
#include "bfh.h"
#include "debug.h"

/*
*	--- Definitions 
*/
#define APPBLOCK	(512-1)

/*
*	--- Externals
*/

/*
*	--- Definitions of Local Module Functions
*/

/*
*	--- Module Globals
*/

/*.*****************************************************
* Function Name: 	main
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
* History:
*
* Created  Thu Mar 2 12:48:04 EST 1995 by Larry A. Walker
*
*
*  Modified:
*
******************************************************/

main()
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"main"
/* Auto's */
	char *userdata = "This is a test user data segment\n";
	BFH * bfh;
	int i, stat, counter;
	char dummyblock[APPBLOCK];
	long block;
	time_t * tlock;
	
#ifdef TRACING
Trace("Enter->%s()\n", "main" );
#endif
	debug_open(TRACE, "stderr");

	Trace("**Testing bfh\n");
	Trace("**Creating new block file\n");
	bfh = BfhOpen ( "testfile", userdata, strlen(userdata) + 1, CREATE, APPBLOCK);
	if(bfh == NULL){
		Trace("Failed to BfhOpen().  Aborting.\n");
		exit(1);
	}
	Trace("**Removing file\n");
	unlink("testfile");
	Trace("**Attempt to open file\n");
	bfh = BfhOpen ( "testfile", userdata, strlen(userdata) + 1, 0, 1024 - 1);
	if(bfh != NULL){
		Trace("BfhOpen() Should have failed.  Aborting.\n");
		exit(1);
	}
	Trace("**Create file\n");
	bfh = BfhOpen ( "testfile", userdata, strlen(userdata) + 1, CREATE, APPBLOCK);
	if(bfh == NULL){
		Trace("Failed to BfhOpen().  Aborting.\n");
		exit(1);
	}
	Trace("**Add 100 blocks\n");
	for(i=0;i<100;i++){
		memset(dummyblock,0,APPBLOCK);
		sprintf(dummyblock, "BFH BLOCK TEST, BLOCK # %d\n", i);
		block = BfhNewBlock(bfh);
		if(block == -1L){
			Trace("Failed to create a new block\n");
			exit(1);
		}
		stat = BfhWriteBlock(bfh, dummyblock, block);
		if(stat == FAILED){
			Trace("Failed to write a block\n");
			exit(1);
		}else{
			Trace("Block %d added\n", i);
		}
	}
	Trace("**Read Back the contents of 100 blocks\n");
	for(i=0;i<100;i++){
		block = BfhAddrBlock(bfh, i);
		if(block == -1L){
			Trace("Couldn't calculate a block number\n");
			exit(1);
		}
		memset(dummyblock,0,APPBLOCK);
		stat = BfhReadBlock(bfh, dummyblock, block);
		if(stat == FAILED){
			Trace("Failed to write a block\n");
			exit(1);
		}
		/* should be printable */
		Trace( dummyblock );
	}
	Trace("**Free a single block\n");
	block = BfhAddrBlock(bfh, 50);
	if(block == -1L){
		Trace("Couldn't calculate a block number\n");
		exit(1);
	}
	stat = BfhFreeBlock(bfh, block);
	if(stat == FAILED){
		Trace("Failed to free a block\n");
		exit(1);
	}
	
	Trace("**Attempt to access invalid blocks\n");
	memset(dummyblock,0,APPBLOCK);
	stat = BfhReadBlock(bfh,dummyblock, block);
	if(stat != EMPTY){
		Trace("Failed to recognize a free block\n");
		Trace("Block = %s\n", dummyblock);
		exit(1);
	}
	block = BfhAddrBlock(bfh,100);
	stat = BfhReadBlock(bfh,dummyblock, i);
	if(stat != FAILED){
		Trace("Failed to recognize invalid block\n");
		exit(1);
	}
	block = BfhAddrBlock(bfh,-1);
	stat = BfhReadBlock(bfh,dummyblock, i);
	if(stat != FAILED){
		Trace("Failed to recognize invalid block\n");
		exit(1);
	}
	Trace("**Free a bunch of blocks\n");
	for(i=1;i<100;i+=2){
		block = BfhAddrBlock(bfh, i);
		stat = BfhFreeBlock(bfh,block);	
		if(stat == FAILED){
			Trace("Failed to free a block\n");
			exit(1);
		}
	}
	Trace("**Check all the freed blocks\n");
	counter = 1;
	for(i=0;i<100;i++){
		stat = BfhFullBlock(bfh, i);
		if(stat ){
			if((i == 50) || (i == counter)){
				Trace("Failed to detect correct empty blocks %d\n", i);
				exit(1);
			}
		}
		counter+=2;
	}
	Trace("**Finally, the file should be truncated\n");
	system("ls -l testfile");
	for(i=50;i<100;i++){
		stat = BfhFullBlock(bfh,i);
		if(stat){
			stat = BfhFreeBlock(bfh, BfhAddrBlock(bfh, i) );
			if(stat == FAILED){
				Trace("Failed to free a block %d\n");
				exit(1);
			}
		}
	}
	stat = BfhClose( bfh, "This file is closed", strlen("This file is closed") + 1);
	system("ls -l testfile");
	Trace("**Reopen the file, print the contents, and exit\n");
	bfh = BfhOpen ( "testfile", userdata, strlen(userdata) + 1, 0, 0);
	if(bfh == NULL){
		Trace("Failed to BfhOpen().  Aborting.\n");
		exit(1);
	}
	PrintBfhDescriptor(bfh);
	Trace("SUCCESS\n");
} /*end  "main" */
