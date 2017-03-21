/*	This source is proprietary information which cannot be distributed  */
/*	without written permission of Larry A. Walker */

#ifndef DBFILESH
#define DBFILESH
			/* Combined Block File Handler Header and */
			/* B+Tree Header */
void fail_que(int);
void display_que();

			/* Define one of these three */

/* write to lower blocks and truncate on close */
#define COMPRESS

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include "bfh.h"

/*The memory debugging module */
/*#include "memdebug.h" */

#define	SUCCESS			1
#define FAIL			0
#define NEAR_MATCH		2
#define NODE_EOF		-1
#define CLEAN			0
#define DIRTY			1
#define EXACT			1
#define INSERT			2
#define UNLINK			1

/*	Type of key	*/
#define UNDEF_KEY		0
#define VAR_LNG_KEY		1
#define FIX_LNG_KEY		2
#define LONG_KEY		3
#define DOUBLE_KEY		4

#define SEARCH_LEVELS		1
#define DUPLICATES		2
#define TYPE_OF_KEY		3
#define KEY_LENGTH		4
#define ACTIVE_LEVEL		5
#define NUMBER_OF_KEYS		6
#define LAST_ACCESS 		7
#define ROOT_NODE		8
#define EXHAUSTED		9

#define MAXVARSTR		64

#define EQ			0
#define LT			-1
#define LE			-2
#define GT			1
#define GE			2
#define ANY			3

#define FirstKeyOffset		((2*sizeof(int)) + sizeof(long))

	/* The definition TreeBlockSize is physical size of the block */
	/* as it lays on the disk.  There is a one byte overhead used */
	/* by the bfh class.  The archecture of he machine may ultim- */
	/* ately decide the value used when creating an index.  Use */
	/* only to open a bfh file. To get the block size of an already */
	/* created bfh file use GetBFHSize() */
#define DiskBlockSize		4096
/*#define DiskBlockSize		2048 */
#define BlockOverhead		1
#define TreeBlockSize		( DiskBlockSize - BlockOverhead )
#define KeySpace		( DiskBlockSize - FirstKeyOffset)

#define MaxKeySize		255	
#define MaxHierarchyPointers 	4  /* same as the number of allowable levels */
#define AllowableLevels		MaxHierarchyPointers

int create_index_string(char *,char *);
int create_c_string(char *,char *);

/* The trunk is the tree information storage block  */

	/* if the total_node_keys = -1 then this is an empty node */
	/* if the next free block addr is -1, this is the last */
	/* free block address */
struct Node {
	int 	total_node_keys;
	int	next_free_position;
	union {
		long	left_node;
		long	next_free_block_addr;
	}nodeblock;
	char	key_space[KeySpace];
};

union Key{
	long	long_key;
	double	double_key;
	char	key[MaxKeySize]; /* null terminated string */
};

#define USRDATASIZE sizeof(long)

struct KeyGroup {
	long	right_node;
	union {
		long	address;
		char 	key_data [USRDATASIZE];
	}KeyData;
	union Key k ;
};

/* The position status is returned from the position in node function */

struct posn_status {
	struct KeyGroup * prev_key;
	int term_status;
	int compare_status;
};

/* This hierarchy differs from a cache that its purpose is not to store nodes */
/* for more efficient rapid access, but to hold the linear parent list for */
/* more efficient movement through the tree */

struct HierarchyBlock {
	long	block_id;
	int	state; 	/* CLEAN or DIRTY */
	struct 	Node hierarchy_node;
};

#endif /*DBFILESH*/
