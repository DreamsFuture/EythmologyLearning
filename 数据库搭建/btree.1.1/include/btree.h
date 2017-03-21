/*
//	B+TreeR2
//	(c) 1990,1991,1995 Larry A. Walker
//	This source is proprietary information which cannot be distributed 
//	without written permission of Larry A. Walker
*/
#ifndef BTREEH
#define BTREEH

#include "bfh.h"
#include "treelib.h"

struct B_tree {
	
	/* Btree information */
        long    	magic;
        short     	search_levels;
        short     	duplicates;
        short     	type_of_key;
        short     	fix_lng_key_lng;
	short		lock_on;
        long    	rootnode;
        long    	number_of_keys;
        time_t  	time_of_last_open;
        time_t  	time_of_last_close;
        short     	exhausted;
	int		block_size;

	/* Open file information */
	int 		vector;		/* Hierarchy list position */
	int		blocks_to_flush;/* total hierarchy blocks */
	int 		ActiveHeight;	/* count of nodes in hierarchy */
	struct 	HierarchyBlock * hierarchy_list;
	struct 	KeyGroup * active_key;	/* the active key */
	struct  KeyGroup * key_copy;	/* copy of the returned key */
	struct 	Node * 	active_node;	/* the active node  */
	int 		key_position;	/* the active keys position */
        long 		left;		/* left address */
			/* variable length compare function */
	int		past_exact_key; /* signal that there was an exact key */
	int     (*compare_funct)(union Key *,struct KeyGroup *,int);
	int      rootsplit;
	BFH	*bfh;		/* open bfh discriptor */
};

typedef struct B_tree BTREE;

static int	LocateExact(BTREE *, union Key *); /* internal key locate  */

static int 	LocateInsert(BTREE *, union Key *,int);/* internal key locate  */

static int 	Insert(BTREE *, struct KeyGroup *, int);/* internal key insert */

int KeyGroupSize(BTREE *, struct KeyGroup *);/* what is the key group size */

struct KeyGroup *CopyKey(BTREE *);/* copy the active key into a user area */

	/* ++++++ Low Level Hierarchy operations */

static int OpenHierarchy(BTREE * );	/* --------- Create the Hierarchy */

int FlushHierarchy(BTREE * );   	/* Flush the entire Hierarchy */

int LoadHierarchyNode(BTREE *, long);/* Load a single Hierarchy block */

static struct Node * Parent(BTREE * );		/* return the parent */

void OpenKeyNode(BTREE *, struct Node *);

static int  InsertNodeKeyGroup(BTREE *, struct KeyGroup *);
				/* insert a key at the current position */

static int  InsertSpareKeyGroup(BTREE *, struct KeyGroup *,struct Node *);
				/* insert a key into the spare key group */

static int  DeleteNodeKeyGroup(BTREE * ); 	/* delete a key */

static int  RemoveNodeKeyGroup(BTREE * ); 	/* remove and replace a key */

static long LeftAddress(BTREE * );		/* return the left address */
		
int  IncrementPosition(BTREE * );
		/* increment the position to the next key */
		/* return Success or fail if no more keys */
                /* at the end of the node, we are actually looking left at the */
                /* most far right position */

void Rewind(BTREE * );			/* first key in node */

int  Compare(BTREE *, union Key *); /* routines to return comparison of key */
				    /* value to the KeyGroup key */

int Currentkey (BTREE *, struct KeyGroup * );

		/* ++++++ Intermediate level operations */

static int Combine(BTREE * );
		/* combine with left node, if fail combine with right */

static void CombineAll(BTREE *);
		/* call combine until root reached */

int UnlinkNode(BTREE *);
		/* release an empty node */

		/* ++++++ High level user interface operations */

BTREE * OpenBtree(char *, int , int , int, int);

int DestroyBtree(BTREE * );

int Close(BTREE * );

int  InstallCompare(BTREE *,  int (*) (union Key *, struct KeyGroup *,int) );

int  Locate(BTREE *, union Key *);

/* operator overloads ------------ */

int  InsertKey (BTREE *, struct KeyGroup *);

int  DeleteKey (BTREE *, union Key *);

struct KeyGroup *  Increment (BTREE * ); 	/* next, prefix only */

struct KeyGroup *  Decrement (BTREE * ); 	/* previous, prefix only */
	
/*-------------------------------- */

struct KeyGroup * First(BTREE * );

struct KeyGroup * Last(BTREE * );

struct KeyGroup * CurrentKey(BTREE * );

int Update(BTREE *, void * data);	/* update the current key's data */

	/* utilities --------------------- */

void DisplayNodeHeader(BTREE * );

void DisplayKey(BTREE *,struct KeyGroup *);

void DumpBtree (BTREE * );

void GetPrintNode (BTREE *, long l);

void PrintNode(BTREE *, struct Node *, int);

void DisplayHierarchy (BTREE *, int);

long NodePosn(BTREE * );

void PrintBtDescriptor( BTREE *);

void PrintBfhDescriptor ( BFH * );

/*	int	(*compare_funct)(union Key *,struct KeyGroup *,int);	 */

/*
	--- Some hardware is not forgiving about the alignment of keys in 
	    tree nodes.  Adjust these alignment definitions as appropriate.
/*
#define ALIGN2
*/
#define ALIGN4

/*
        --- It is possible for an archatecture to be satisified with a 32bit
            alignment and complain when a double lands on a non 64 bit
            boundry - usually indicating a 64 bit fpu.  If everything else
            works fine except float trees, define 64BITFP
            example HP9000
*/
#define _64BITFPU

/*021994*/
#define SPLITKEYMIN	2	/* the minimum keys in a node after split */

#define TREELOCKKEY	0x33034

#endif /*BTREE*/
