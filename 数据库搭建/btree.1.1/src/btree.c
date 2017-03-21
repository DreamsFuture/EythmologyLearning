/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                Larry A. Walker & Co. B+TreeR2
	Corp. 1991,1995 Larry A. Walker (Larry A. Walker & Co.)
	                All rights reserved.  
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "treelib.h"
#include "btree.h"
#include "bfh.h"
#include "debug.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	standard compare routines
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
// NAME: 	pascal_string_compare
//
// DESCRIPTION:	The format of these strings consists of a leading byte 
//		which designates
//		the length of the string, followed by the contents of the 
//		string.
//		All comparison value routines must return -1, 0 or 1 as
//		the result of the comparison.  Note:  The specifications for
//		strncmp() do not meet this requirement.
//		Here, if the length of one string is less
//		than the length of another, it is automatically a value
//		less than aother.
//
// RETURN:	-1 LT, 0 EQ, 1 GT
//
*/
static int pascal_string_compare(union Key * value, /* A raw key */
			  struct KeyGroup * active_key,/* An encapsolated key */
			  int lng /* A place holder here */)
{
	int status;

	Trace("pascal_string_compare(%x, %x, %d)", (unsigned)value, (unsigned)active_key, lng);
	status = strncmp((value->key)+1,(active_key->k.key)+1,
		(active_key->k.key[0] > value->key[0])?
			(int)active_key->k.key[0]:value->key[0] );
	if(status==0)
		return status;
	return ( (status < 1)? -1 : 1 );
}

/*
// NAME:	long_compare
//
// DESCRIPTION:	Compare two long values, returning the result
//
// RETURN:	-1 LT, 0 EQ, 1 GT
//
*/
static int long_compare( union Key * value, /* A raw key */
		  struct KeyGroup * active_key, /* An encapsolated key */
		  int lng /* A place holder here */)
{
	Trace("long_compare(%x, %x, %d)", (unsigned)value, (unsigned)active_key, lng);
	if(value->long_key < active_key->k.long_key)
		return -1;
	if(value->long_key > active_key->k.long_key)
		return 1;
	return 0;
}

/*
// NAME:	double_compare
//
// DESCRIPTION:	Compare two double values, returning the result.  
	    It is possible for an archatecture to be satisified with a 32bit
	    alignment and complain when a double lands on a non 64 bit 
	    boundry - usually indicating a 64 bit fpu.  If everything else 
	    works fine except float trees, define _64BITFPU
	    example HP9000
//
// RETURN:	-1 LT, 0 EQ, 1 GT
//
*/
static int double_compare( union Key * value, /* A raw key */
		  struct KeyGroup * active_key, /* An encapsolated key */
		  int lng /* A place holder here */)
{
#ifdef _64BITFPU
	double vd, ad;
#else
#define vd value->double_key
#define ad active_key->k.double_key
#endif
	Trace("double_compare(%x, %x, %d)\n", (unsigned)value, (unsigned)active_key, lng);

#ifdef _64BITFPU
	memcpy((void *)&vd, (void*)&(value->double_key), sizeof(double));
	memcpy((void *)&ad, (void*)&(active_key->k.double_key), sizeof(double));
#endif
	if(vd < ad)
		return -1;
	if(vd > ad)
		return 1;
	return 0;
}

/*
// NAME:	fix_lng_compare
//
// DESCRIPTION:	Compare two fixed length values, returning the result.
//
// RETURN:	-1 LT, 0 EQ, 1 GT
//
*/
static int fix_lng_compare( 	union Key * value, /* A raw key */
			struct KeyGroup * active_key, /* An encapsolated key*/
			int lng /* key size */ )
{
	int status;

	Trace("fix_lng_compare(%x, %x, %d)", (unsigned)value, (unsigned)active_key, lng);
	status = strncmp(value->key,active_key->k.key,lng);
	if(status==0)
		return status;
	return ( (status < 1)? -1 : 1 );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	move memory
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* 

//
// NAME:	move memory, allowing for overlapping memory areas
//
// DESCRIPTION:	Slower than any assembly language counterpart that 
//		would replace it, but this routine has proven most
//		trustworthy in any situation.
//
// RETURN:	NONE
//
*/
void mv_mem(	char *source,/* memory at */
		char *destin,/* memory to */
		int len /* for length */ )
{
	Trace("mv_mem(%x, %x, %d)\n", (unsigned)source, (unsigned)destin, len);
	if(len < 1)return;
	if(destin < source)
	{
		while(len--)
		{
			*destin=(*source); destin++; source++;
		}
		return;
	}
	else
	{
		source+=(len-1); destin+=(len-1);
		while(len--)
		{
			*destin=(*source); destin--; source--;
		}
		return;
	}
}

/* 
	--- Here is the alternative, if it works correctly, use it, the routine 
	    since the routine would be in assembly
*/

#define mv_mem(s,d,l) memmove(d,s,l)

/*
// NAME:	create_index_string
//
// DESCRIPTION:	This utility will take a null terminated C string and create
//		a pascal type string as required for indexing.  The first 
//		byte of the string provides the length of the string.  The
//		length of the string is therefor limited to 255 characters.
//		In actual indexing practice the it isnt reasonable to assume
//		that variable length keys should be allowed to be more than
//		about 64 characters.  
//		If you want your index to fail, change the commented code.
//		As it is, it will simply take the first MASVARSTR characters
//		from the string to create the index key string.
//
// RETURN:	SUCCESS or FAIL
//
*/

int create_index_string(char * to /*place the results here*/, 
			char * fm /*convert this string */)
{
	int i;

	Trace("create_index_string(%x,%x)\n", (unsigned)to, (unsigned)fm);

	i  = strlen(fm);

	if(i > MAXVARSTR){
		/*fprintf(stderr,"Warning: %s: Line %d: String too long",__FILE__, __LINE__);*/
		/*return FAIL;*/
		i=MAXVARSTR;
	}
	*to = (char)i;
	mv_mem(fm,to+1,i);
	return SUCCESS;
}

/*
// NAME:	create_c_string
//
// DESCRIPTION:	This utility will take an index type (pascal type) string 
//		and create a standard null terminated C string.
//
// RETURN:	Always returns SUCCESS
//
*/
int create_c_string(char * to /* put the new string here */, 
		    char * fm /* get the pascal string from here */) 
{
	int i;

	Trace(" create_c_string(%x,%x)\n", (unsigned)to, (unsigned)fm);

	i = (int)(*fm);

	strncpy(to,++fm,i);
	*(to+i) = 0;
	return SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Btree class routines
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
// NAME:	OpenKeyNode
//
// DESCRIPTION:	This function takes a pointer to a node and initializes
//		the class instance to use it as the current active node.
//		All active operations will be performed on this node.
//
// RETURN:	NONE
//
*/
void OpenKeyNode(BTREE * btree, /* tree descriptor pointer */
		 struct Node * n /* pointer to new active node */  )
{

	Trace("OpenKeyNode(%x,%x)\n", (unsigned)btree,
		(unsigned)n);
	btree->active_node = n;
	btree->left = btree->active_node->nodeblock.left_node;
	btree->key_position = 1;
	btree->active_key = (struct KeyGroup *)(((char *)n) + FirstKeyOffset); 
}

/*
// NAME:	Btree
//
// DESCRIPTION:	Btree initialization function.  Will create a new index if
//		second parameter is CREATE and index does not already exist.
//		The first parameter is the name of the index.
//		The third parameter is the type of key.
//		If the fourth parameter is set to 1, duplicate keys will be
//		allowed.  If 0 duplicates will not be allowed.
//		If the keys are not variable length, the fixed key length
//		size should be the fifth parameter.  For example, long keys
//		would be sizeof(long).
//
// RETURN:
//
*/
BTREE * OpenBtree(	char *index_name, /* disk file name */
			int create, /* true or false */
			int tok, /* VAR_LNG_KEY or FIX_LNG_KEY or LONG_KEY or DOUBLE_KEY */
			int dups, /* duplicate keys allowed, true or false */
			int fix_key_size /* ignored, or size of key if fixed length */)
{
	int i;
	BTREE * btree;

	Trace("OpenBtree( %s,%d,%d,%d,%d)\n", index_name, create, tok, 
		dups, fix_key_size);

	btree = (BTREE*)calloc( 1, sizeof(BTREE) );
	if(btree == NULL){
		Debug("Can't allocate space\n");
		return NULL;
	};
	
/*
*	--- Initialize the btree handle
*/
        btree->vector = -1;  /* -1 = empty*/
	btree->blocks_to_flush = 0;
	btree->ActiveHeight = 0;
	btree->hierarchy_list = NULL;
	btree->active_node = NULL;
	btree->active_key = NULL;
	btree->key_copy = NULL;
	btree->key_position = -1;
	btree->left = -1L;
	btree->past_exact_key = 0;
	btree->compare_funct = NULL;
	btree->rootsplit=0;
	btree->lock_on=0;

	if(create == CREATE ){
		btree->magic = 2216L;
		btree->search_levels = 0;
		btree->duplicates = dups;
		btree->type_of_key = tok;
		btree->fix_lng_key_lng = fix_key_size;
		btree->rootnode = 0L;
		btree->number_of_keys = 0L;
		btree->exhausted = 1;
		/*this will not be the same size as the bfh block_size*/
		btree->block_size = TreeBlockSize;
		/* override the lng parameter */
		switch(tok){
			case VAR_LNG_KEY:
				btree->fix_lng_key_lng = -1;
				break;
			case LONG_KEY:
				btree->fix_lng_key_lng = sizeof(long);
				break;
			case DOUBLE_KEY:
				btree->fix_lng_key_lng = sizeof(double);
				break;
			case FIX_LNG_KEY:
/*
*	--- Fix alignment problems on certain hardware (Sun)
*/
				if(fix_key_size)
#ifdef ALIGN2
				if(!((fix_key_size + 2 ) % 2) )
                                	btree->fix_lng_key_lng = fix_key_size;
				else
                                	btree->fix_lng_key_lng = fix_key_size +
                                	( 2 - ( (fix_key_size+2) % 2 ) );
#endif
#ifdef ALIGN4
				if(!((fix_key_size + 4 ) % 4) )
                                	btree->fix_lng_key_lng = fix_key_size;
				else
                                	btree->fix_lng_key_lng = fix_key_size +
                                	( 4 - ( (fix_key_size+4) % 4 ) );
#endif
				break;
			default:
				Debug("Invalid type of key\n");
				break;
		}
		if((btree->fix_lng_key_lng < 1) && (tok != VAR_LNG_KEY) ){
			Debug("Invalid key size\n");
			free ((void*)btree);
			return NULL;
		}
		btree->bfh = BfhOpen ( index_name, btree, sizeof(BTREE), CREATE, TreeBlockSize );
		if(btree == NULL){
			Debug("Failed to open %s\n", index_name);
			free ((void*)btree);
			return NULL;
		}
	}else{
		btree->bfh = BfhOpen ( index_name, btree, sizeof(BTREE), 0, 0);
		if(btree == NULL){
			Debug("Failed to open %s\n", index_name);
			free ((void*)btree);
			return NULL;
		}
		if(btree->magic != 2216L){
			Debug("%s Not a btree file\n",index_name);
			free ((void*)btree);
			return NULL;
		}
	}
	/* Next we need a hierarchy to store our nodes*/
	btree->ActiveHeight=(MaxHierarchyPointers >= 
		(btree->search_levels+2))?
		MaxHierarchyPointers:btree->search_levels+2;
	btree->hierarchy_list = (struct HierarchyBlock *)
		calloc(btree->ActiveHeight,sizeof(struct HierarchyBlock) );
	if(btree->hierarchy_list == NULL){
		Debug("Failed to allocate memory.\n");
		free( (void*)btree) ;
		return NULL;
	}
	for(i=0;i<btree->ActiveHeight;i++){
		(btree->hierarchy_list+i)->block_id = -1L;
		(btree->hierarchy_list+i)->state = CLEAN;
	}
	/* Install the comparison routine*/
	if(create != CREATE){
		if( (tok != btree->type_of_key) && (tok != 0) ){
			Debug("Invalid type of key\n");
		}
		tok = btree->type_of_key;
	}
	Trace("install tok # %d\n",tok);
	switch(tok){
		case VAR_LNG_KEY:
			btree->compare_funct = pascal_string_compare;
			break;
		case LONG_KEY:
			btree->compare_funct = long_compare;
			break;
		case DOUBLE_KEY:
			btree->compare_funct = double_compare;
			break;
		case FIX_LNG_KEY:
			btree->compare_funct = fix_lng_compare;
			break;
		default:
			Debug("Invalid type of key\n");
			break;
	}
	btree->time_of_last_open = time(NULL);
	return btree;
} /* Btree*/

/*
// NAME:	Close
//
// DESCRIPTION:	Shutdown a btree.
//
// RETURN:	Result of the block file handler closing function
//
*/
int Close(BTREE * btree /* pointer to tree descriptor */)
{
	int stat;

	Trace("Close(%x)\n", (unsigned)btree);
	FlushHierarchy(btree);
	free ((void *)btree->hierarchy_list);
	if(btree->key_copy != NULL)
		free((void *)btree->key_copy);
	btree->time_of_last_close = time(NULL);
	stat = -1;
	if(btree->bfh != NULL){
		stat = BfhClose( btree->bfh, btree, sizeof(BTREE) );
		if(!stat){
			Debug("Error in closing file\n");
		}
	}
	btree->bfh == NULL;
	return stat;
}

/*
// NAME:	InstallCompare
//
// DESCRIPTION:	Installs a new compare function for fixed length keys -
//		composite keys
//
// RETURN:	SUCCESS or FAIL
//
*/
int InstallCompare(BTREE * btree /* pointer to tree descriptor */, int (* fn)(union Key *, struct KeyGroup *, int) )
{
	Trace("InstallCompare(%x,%x)\n", (unsigned)btree, (unsigned)fn);

	if(btree->type_of_key == FIX_LNG_KEY){
		btree->compare_funct = fn;
		return SUCCESS;
	}
	return FAIL;
}

/*
// NAME:	Locate
//
// DESCRIPTION:	Provides public access to the private LocateExact function.
//
// RETURN:	result of the LocateExact function.
//
*/
int Locate(BTREE * btree /* pointer to tree descriptor */,
	   union Key * search_key /* A raw key */)
{

	Trace("Locate(%x,%x)\n", (unsigned)btree, (unsigned)search_key);

	if(btree->exhausted)
		return FAIL;
	/* access from outside, -1 level indicate search continues to leaf*/
	return LocateExact(btree, search_key);
}

/*
// NAME:	LocateInsert
//
// DESCRIPTION:	Search for an insert position at a specified level.
//		Called by the Insert function.
//
// RETURN:	SUCCESS, FAIL, NODE_EOF, or NEAR_MATCH
//
*/
static int LocateInsert(BTREE * btree /* pointer to tree descriptor */,
		 union Key * search_key /* An encapsolated key */,
		 int level /* specified level */)
{
	long next_node;
	int status,pos_status;

	Trace("LocateInsert(%x,%x,%d)\n",
		(unsigned)btree, (unsigned)search_key, level);

	next_node = btree->rootnode; /* root node */
	btree->vector = -1;
	btree->past_exact_key = 0;
	while(1){
		/* also opens the node*/
		if(LoadHierarchyNode(btree, next_node ) == FAIL){ 
			Debug("Cant load node\n");
			return FAIL;
		}
		if(btree->active_node->total_node_keys < 1){
			/* A tree deleted down to 0 keys*/
			if(next_node == btree->rootnode){
				return SUCCESS;
			}
			Debug("Empty node\n");
			return FAIL;
		}
		/* if the first key is already > the search key*/
		if(Compare(btree,search_key) == -1){
			if((level == btree->vector)||(btree->left == -1L)){
				return NEAR_MATCH;
			}
			next_node = btree->left;
			continue; 
		}
		/* walk forward until the key found or the search is > the*/
		/* found key*/
		while((pos_status = IncrementPosition(btree)) == SUCCESS){
			status = Compare(btree,search_key);
			if(status == -1 )
				break;
			if(status == 0 )
				btree->past_exact_key = 1;
			/* we must now not be past the position we would*/
			/* find the key, or at an insert position*/
		}
		/* if we couldnt increment*/
		if(pos_status == FAIL) 
			return FAIL;
		/* if we did increment*/
		if(pos_status == SUCCESS){
			/* status must be -1*/
			if((level == btree->vector)||(btree->left == -1L))
				return NEAR_MATCH;
			next_node = btree->left;
			continue;
		}
		if(pos_status == NODE_EOF){
			if((level == btree->vector) || (btree->left == -1L))
				return NODE_EOF;
			next_node = btree->left;
			continue;
		}
	}
} /* LocateInsert*/

/*
// NAME:	LocateExact
//
// DESCRIPTION: 	We are searching for an exact match at all levels.
//
// RETURN:	SUCCESS or FAIL
//
*/
static int LocateExact(BTREE * btree /* pointer to tree descriptor */,
	        union Key * search_key /* An encapsolated key */)
{
	long next_node;
	int status,pos_status;
	
	Trace("LocateExact(%x,%x)\n", (unsigned)btree, (unsigned)search_key);

	if(btree->exhausted)
		return FAIL;
	next_node = btree->rootnode; /* root node */
	btree->vector = -1;
	while(1){
		/* also opens the node*/
		if(LoadHierarchyNode(btree, next_node ) == FAIL){ 
			Debug("Cant load node\n");
			return FAIL;
		}
		if(btree->active_node->total_node_keys < 1){
			Debug("Empty node\n");
			return FAIL;
		}
		/* walk forward until the key found or the search is > the*/
		/* found key*/
		pos_status = SUCCESS;
		do {
			if((status = Compare(btree,search_key)) == 0 ){
				return SUCCESS;
			}
			if(status == -1 )
				break;
			/* we must now not be past the position we would*/
			/* find the key, or at an insert position*/
		} while((pos_status = IncrementPosition(btree)) == SUCCESS);
		/* if we couldnt increment*/
		if(pos_status == FAIL) 
			return FAIL;
		/* if we did increment*/
		if((pos_status == SUCCESS)||(pos_status == NODE_EOF)){
			/* status must be -1*/
			if(btree->left == -1L)
				return FAIL; /* couldnt find exact*/
			next_node = btree->left;
			continue;
		}
	}
} /* LocateExact*/

/*
// NAME:	InsertKey
//
// DESCRIPTION:	Insert a key into the tree, provide the key as a pointer
//		to a completed KeyGroup structure.
//
// RETURN:	SUCCESS or FAIL
//
*/
int InsertKey ( BTREE * btree /* pointer to tree descriptor */,
		struct KeyGroup * insert_group /* An encapsolated key */)
{
	/* outside access to the insert function*/
	int stat;

	Trace("InsertKey (%x,%x)\n", 
		(unsigned)btree, (unsigned)insert_group);

	btree->rootsplit = 0;
	insert_group->right_node = -1L; /* outside concerns cannot use a right*/
					/* node address*/
	stat = Insert(btree,insert_group, -1);  /* normal insert a leaf level*/
	if(stat == SUCCESS){
		btree->number_of_keys++;
		if(btree->exhausted)
			btree->exhausted = 0;
	}
		
	return stat;
} /* operator += */
	

/*
// NAME:	Insert
//
// DESCRIPTION:	Insert a new key, contained in pointed to key group, into
//		the tree at specified level
//
// RETURN:	SUCCESS or FAIL
//
*/
static int Insert(BTREE * btree /* pointer to tree descriptor */,
	   struct KeyGroup * insert_group /* An encapsolaed key */,
	   int level /* specified level for insert */)
{
	char *ptr;
	int i,status,half_posn;
	struct Node spare_node;
	struct KeyGroup nkey;
	long save_ptr;

	Trace("Insert(%x,%x,%d)\n", 
		(unsigned)btree, (unsigned)insert_group, level);
	if(btree->exhausted){ /* first key in tree*/
		ptr = (char *)&spare_node;
		for(i=0;i<sizeof(struct Node);i++)*ptr++=0;
		btree->active_node = &spare_node;
		btree->key_position = 1;
		btree->active_key = (struct KeyGroup *)(((char *)&spare_node) + FirstKeyOffset); 
		btree->active_node->total_node_keys = 0;
		btree->active_node->nodeblock.left_node = -1L;
		btree->left = btree->active_node->nodeblock.left_node;
		btree->active_node->next_free_position = FirstKeyOffset;
		status = InsertNodeKeyGroup(btree,insert_group);
		if (status == FAIL){
			Debug("Couldnt insert\n");
			return FAIL;
			}
		btree->rootnode = BfhNewBlock(btree->bfh);
		if(btree->rootnode == -1L){
			Debug("Couldnt get new block\n");
			return FAIL;
		}
	        if(BfhWriteBlock(btree->bfh,(char*)btree->active_node,btree->rootnode) == FAIL){
			Debug("Cant write block\n");
			return FAIL;
			}
		btree->search_levels = 1;
		LoadHierarchyNode(btree,btree->rootnode);
		return SUCCESS;
	}
        if( (level != -1 ) && (btree->rootsplit) ){
                /* If the root was split, all recursive inserts pending are*/
                /* off by one*/
                level++;
        }

	status = LocateInsert(btree, (union Key *)insert_group->k.key ,level);

	/* we could have eof success fail or near match */
	if(status == FAIL){
		Debug("Cant locate for insert\n");
		return FAIL;
	}
	if((status == SUCCESS)||(btree->past_exact_key) ){
		if(btree->duplicates == 0){
			Debug("No Duplicates Allowed\n");
			return FAIL;
		}
	}
	/* can also be NEAR_MATCH or NODE_EOF*/

	/* then must be near match or success with dups allowed or node eof*/
	status = InsertNodeKeyGroup(btree,insert_group);
	if(status == SUCCESS){
		return SUCCESS;
	}
	/* runtime_error(TRACE,__FILE__,__LINE__,"Node full");*/
	/* the only failure here is lack of space*/
	/* SPLIT the NODE*/
	/* if root node, make a new root node*/
	/* split out the left half to the spare node*/
	if(btree->vector < 0) {
		Debug("No node to split\n");
		return FAIL;
		}
	Rewind(btree);
	if(( btree->active_node->total_node_keys < 5 ) ||
		(half_posn = ( btree->active_node->total_node_keys / 2 )) 
		< SPLITKEYMIN){
		return FAIL;
		}
	ptr = (char *)&spare_node;
	for(i=0;i<sizeof(struct Node);i++)*ptr++=0;
/* HERE 0 or -1 ?? */
	spare_node.total_node_keys = 0;
	spare_node.nodeblock.left_node = -1L;
	spare_node.next_free_position = FirstKeyOffset;
	if(btree->vector == 0){
		for(i = 0; i < btree->active_node->total_node_keys/2; i++){
			if((status = InsertSpareKeyGroup(btree,btree->active_key,&spare_node))
				 == FAIL ){
				Debug("Failed to insert\n");
				return FAIL;
			}
			if( (status = DeleteNodeKeyGroup(btree)) == FAIL ){
				Debug("Key loss deleting during split\n");
			}
		}
		spare_node.nodeblock.left_node = btree->active_node->nodeblock.left_node;
		/* if root node move out the right half to another node*/
		/* leaving center key as root node*/
		btree->active_node->nodeblock.left_node = BfhNewBlock(btree->bfh);
		BfhWriteBlock(btree->bfh, (char *)&spare_node , btree->active_node->nodeblock.left_node );
		ptr = (char *)&spare_node;
		for(i=0;i<sizeof(struct Node);i++)*ptr++=0;
		spare_node.total_node_keys = 0;
		spare_node.next_free_position = FirstKeyOffset;
		if(btree->active_key->right_node != -1L){
			spare_node.nodeblock.left_node = btree->active_key->right_node;
		}
		else 
			spare_node.nodeblock.left_node = -1L;
		btree->active_key->right_node = BfhNewBlock(btree->bfh);
		/* bypass the center key as new single root key*/
		if(IncrementPosition(btree) != SUCCESS){
			Debug("failed to increment\n");
		}
		while(btree->key_position <= btree->active_node->total_node_keys){
			if((status = InsertSpareKeyGroup(btree,btree->active_key,&spare_node))
				 == FAIL ){
				Debug("Failed to Insert\n");
				return FAIL;
			}
			if( (status = DeleteNodeKeyGroup(btree)) == FAIL ){
				Debug("Key loss deleting during split\n");
			}
		}
		Rewind(btree);
		if ( BfhWriteBlock(btree->bfh, (char *)&spare_node , btree->active_key->right_node )
			== SUCCESS){
			FlushHierarchy(btree);
		        btree->search_levels ++;
		}
		/*021194 mark the global marker for root haveing been split */
		/*if(level != -1)	// if not insert in leaf*/
		/*	level++;	// we added a level*/
		btree->rootsplit = 1;
	} else	{
		int pos_status;

		for ( i = 0; i < half_posn; i++) {
			/* pick the middle key*/
			if(IncrementPosition(btree) != SUCCESS){
				Debug("failed to increment\n");
			}
		}
		if(btree->active_key->right_node != -1L){
			spare_node.nodeblock.left_node = btree->active_key->right_node;
		}
		else spare_node.nodeblock.left_node = -1L;
		save_ptr = BfhNewBlock(btree->bfh);
		btree->active_key->right_node = save_ptr;
		/* this key goes to the parent, and the rest of the*/
		/* node goes to the spare block*/
		/* struct KeyGroup * nkey = (struct KeyGroup *)calloc(1,sizeof(struct KeyGroup));*/
		mv_mem((char *)btree->active_key, (char *)&nkey, 
			KeyGroupSize(btree,btree->active_key));
		if( DeleteNodeKeyGroup(btree) == FAIL ){
			Debug("Key loss deleting during split\n");
		}
		while(btree->key_position <= btree->active_node->total_node_keys){
			if((status = InsertSpareKeyGroup(btree,btree->active_key,&spare_node))
				 == FAIL ){
				Debug("Failed to Insert\n");
				return FAIL;
			}
			if( DeleteNodeKeyGroup(btree) == FAIL ){
				Debug("Key loss deleting during split\n");
			}
		}
		BfhWriteBlock(btree->bfh, (char *)&spare_node , save_ptr );
		/* check the levels*/
/*if((btree->vector+1) >= btree->search_levels){*/
/*btree->search_levels = ( ( btree->vector + 1 would match search levels ) + 1 /* the new level );*/
/*		}*/

		/* we have a dangling nkey here*/
		if(Parent(btree) == NULL){
			Debug("Cant find parent\n");
		}
		if(btree->active_node->total_node_keys < 1){
			Debug("Empty node\n");
			return FAIL;
		}
		if(Compare( btree,(union Key *)nkey.k.key) != -1 ){
			while((pos_status = IncrementPosition(btree)) == SUCCESS){
				if((status = Compare(btree,(union Key *)nkey.k.key))
					 != 1 )
					break;
			}
			if(pos_status == FAIL){
				Debug("Couldnt increment\n");
				return FAIL;
			}
		}
		status = InsertNodeKeyGroup(btree,&nkey);
		if(status != SUCCESS){
			status = Insert(btree,&nkey,btree->vector); /* reload for this key*/
			}
		if(status == FAIL){
			Debug("Couldnt insert\n");
			return FAIL;
		}
	}
	/* we had to split it, so try it again*/
	FlushHierarchy(btree);
	return Insert(btree,insert_group, level);
} /* Insert*/

/*
// NAME:	DeleteKey
//
// DESCRIPTION:	Delete a key, pointed to by a union Key pointer.
//
// RETURN:	SUCCESS or FAIL
//
*/

int  DeleteKey (BTREE * btree /* pointer to tree descriptor */,
	        union Key * delete_key /* An encapsolated key */)
{

	Trace("DeleteKey (%x,%x)\n", (unsigned)btree, (unsigned)delete_key);

	if( btree->exhausted)
		return FAIL;
	/* we could have eof success fail or near match */
	if(LocateExact(btree,delete_key) != SUCCESS){
		Debug("Couldnt locate key\n");
		return FAIL;
	}

	/* check for leaf*/
	/* delete it and call combine*/
	if( btree->active_key->right_node == -1L){
		if(DeleteNodeKeyGroup(btree) == SUCCESS){
			btree->number_of_keys--;
			if(btree->number_of_keys == 0){
				btree->exhausted = 1;
				btree->search_levels = 0;
                		if(BfhFreeBlock(btree->bfh, btree->rootnode ) 						== FAIL){
		                        Debug("Cant free block\n");
               		         	return FAIL;
                		}
				btree->hierarchy_list->state = CLEAN;
			}else{
				if(btree->vector > 0)
					CombineAll(btree);
			}
			FlushHierarchy(btree);
			return SUCCESS;
		} else {
			Debug("Couldnt delete\n");
			return FAIL;
		}
	}

	/* ----  must be interior node*/
	if(RemoveNodeKeyGroup(btree) != SUCCESS){
		Debug("Coundnt remove the key\n");
		return FAIL;
	}
	btree->number_of_keys--;
	FlushHierarchy(btree);
	return SUCCESS;
} /* operator -=*/

/*
// NAME:    CombineAll
//
// DESCRIPTION:  Calls the Combine() routine from the current vector until the
//              root has been reached
//
// RETURN:     Pointer the the first key group in the tree, or NULL on
//              failure.
//
*/
static void CombineAll(BTREE * btree/* pointer to tree descriptor */)
{
        int status;
	int current;

	Trace("CombineAll(%x)\n", (unsigned)btree);

	current = btree->vector;

        while(1){
                status = Combine(btree);
                if(status == FAIL)
                        break;
		btree->vector = current;
                btree->vector --; current --;
                if( !btree->vector )
                        break;
                OpenKeyNode( btree,&(btree->hierarchy_list+btree->vector)->hierarchy_node);
        }

} /* end CombineAll()*/

/*
// NAME:	First
//
// DESCRIPTION:	Locate and return a pointer to the first key group in the
//		tree.
//
// RETURN:	Pointer the the first key group in the tree, or NULL on 
//		failure.
//
*/
struct KeyGroup * First(BTREE * btree /* pointer to tree descriptor */)
{
	long next_node;

	Trace("First(%x)\n", (unsigned)btree);

	if(btree->exhausted)
		return FAIL;
	next_node = btree->rootnode; /* root node */
	FlushHierarchy(btree);
	btree->vector = -1;
	btree->active_key = NULL;
	while(1){
		if(LoadHierarchyNode(btree, next_node ) == FAIL){ 
			Debug("Cant load node\n");
			return NULL;
		}
		if(btree->left == -1L){
			if(btree->active_node->total_node_keys == 0)
				return NULL;
			return (btree->active_key);
			}
		next_node = btree->left;
	}
} /* First*/

/*
// NAME:	Increment
//
// DESCRIPTION:	Increment the internal pointers to the next key in the 
//		tree.	
//
// RETURN:	Pointer to next key group structure in the tree.  
//		NULL on failure
//
*/
struct KeyGroup *  Increment(BTREE * btree /* pointer to tree descriptor */)
{
	long bid;

	Trace("Increment(%x)\n",(unsigned)btree);
	if (btree->exhausted )
		return FAIL;
	if (btree->active_key == NULL)
		return NULL;
	if ((btree->vector == 0) && ( btree->active_node->total_node_keys == 0 )){
		/* a root node can be fully exhausted*/
		btree->active_key = NULL;
		return NULL;
        }
	/* if there is anything here we have to go south*/
	if(btree->active_key->right_node != -1L){
		/* decend into that group*/
		if(LoadHierarchyNode(btree,btree->active_key->right_node)==SUCCESS){
			/* find the lowest key down that side*/
			while(btree->left != -1L){
				if(LoadHierarchyNode(btree,btree->left)!=SUCCESS){
					Debug("Cant load node\n");
					return NULL;
				}
			}
			return (btree->active_key);
		} else { 
			Debug("Cant load node\n");
			return NULL;
			}
	}

	/* otherwise lets try the next key*/
	if (btree->key_position < btree->active_node->total_node_keys){
		if(IncrementPosition(btree)==SUCCESS)
			return (btree->active_key);
		else { 
			Debug("Couldnt increment\n");
			return NULL;
			}
	}

	/* if already on the last key*/
	else {
		int stat;
		do	{
			/* keep the id of the present node*/
			bid = (btree->hierarchy_list+btree->vector)->block_id;
			if(Parent(btree)==NULL){
				Debug("Cant find parent\n");
				return NULL;
			}
			/* look for the key that has our child*/
			while(btree->left != bid){
				if((stat=IncrementPosition(btree))==FAIL){
					Debug("Cant increment\n");
					return NULL;
				}
			} 
			/* if we have a valid position return key*/
			if(btree->key_position < ( btree->active_node->total_node_keys + 1) ){
				return (btree->active_key);
				}
			/* if we are at eof we try again or quit if we have*/
			/* run out*/
		}while (btree->vector > 0);
		Debug(NULL);
		return NULL;
	}
} /* operator ++*/

/*
// NAME:	Last
//
// DESCRIPTION:	Find the last key in the tree.
//
// RETURN:	Pointer to last key group structure in the tree.  
//		NULL on failure
//
*/
struct KeyGroup *  Last(BTREE * btree /* pointer to tree descriptor */)
{
	long next_node;
	int i;
	next_node = btree->rootnode; /* root node */

	Trace("Last(%x)\n", (unsigned)btree);

	if(btree->exhausted)
		return FAIL;
	FlushHierarchy(btree);
	btree->vector = -1;
	while(1){
		if(LoadHierarchyNode(btree, next_node ) == FAIL){ 
			Debug("Couldnt load node\n");
			return NULL;
		}
		if(btree->active_node->total_node_keys == 0)
			return NULL;
		/* get the rightmost key*/
		for(i = 0; i < btree->active_node->total_node_keys-1;i++){
			if(IncrementPosition(btree)!=SUCCESS)
				return NULL;
		}
		if(btree->active_key->right_node == -1L){
			return (btree->active_key);
			}
		next_node = btree->active_key->right_node;
	}
} /* Last*/

/*
// NAME:	Decrement
//
// DESCRIPTION:	Decrement the internal pointers to the next key in the 
//		tree.	
//
// RETURN:	Pointer to previous key group structure in the tree.  
//		NULL on failure
*/
struct KeyGroup *  Decrement (BTREE * btree /* pointer to tree descriptor */)
{
	long bid;
	int i,hold_posn;


	Trace("Decrement(%x)\n", (unsigned)btree);
	if(btree->exhausted)
		return FAIL;
	/* if there is anything here we have to go south*/
        if(btree->active_key == NULL)
		return NULL;
	if(btree->left != -1L){
		/* decend into that group*/
		if(LoadHierarchyNode(btree,btree->left)==SUCCESS){
			/* find the highest key down that side*/
			while(1){
				for(i = 0; i < btree->active_node->total_node_keys-1;
						i++){
					if(IncrementPosition(btree)!=SUCCESS)
						return NULL;
				}
				if(btree->active_key->right_node == -1L)
					return (btree->active_key);
				if(LoadHierarchyNode(btree,btree->active_key->right_node)!=
						SUCCESS){
					Debug("Couldnt load node\n");
					return NULL;
				}
			}
		} else {
			return NULL;
		}
	}
	/* otherwise get prev key in same node*/
	hold_posn = btree->key_position;
	if(hold_posn == 1){
		if(btree->vector == 0)
			return NULL; /* no previous keys*/
		while(1){
			bid = (btree->hierarchy_list+btree->vector)->block_id;
			if(Parent(btree)==NULL){
				Debug("Couldnt locate parent\n");
				return NULL;
			}
			if(btree->left == bid){
				if(btree->vector == 0)
					return NULL;
				continue;
			}
			else break;
		}
		for(i=0;i<btree->active_node->total_node_keys;i++){
			if(btree->active_key->right_node == bid)
				return (btree->active_key);
			if(IncrementPosition(btree)!=SUCCESS){
				Debug("Couldnt increment\n");
				return NULL;
			}
		}
		return NULL;
	}
	Rewind(btree);
	for(i=1; i < (hold_posn - 1); i++){
		if(IncrementPosition(btree)!=SUCCESS){
			Debug("Couldnt increment\n");
			return NULL;
		}
	}
	return (btree->active_key);
}

/*
// NAME:	CurrentKey
//
// DESCRIPTION:	Identify the current key.
//
// RETURN:	Returns a pointer to the key group which is designated 
//		as the active key
*/
struct KeyGroup *  CurrentKey(BTREE * btree /* pointer to tree descriptor */)
{

	Trace("CurrentKey(%x)\n", (unsigned)btree);
	if(btree->exhausted)
		return FAIL;
	if(btree->active_key == NULL) 
		return NULL;
	/* exhausted root node*/
	if ((btree->vector == 0) && ( btree->active_node->total_node_keys == 0 )){
		/* a root node can be fully exhausted*/
		return NULL;
        }
	else return (btree->active_key);
} /* CurrentKey*/

/*
// NAME:	Update
//
// DESCRIPTION:	Update the data of the current key
//
// RETURN:	Overwrites the key data or the address stored in a key
*/
int  Update(BTREE * btree /* pointer to tree descriptor */, 
	    void * new_data /* new key data, consistent in type to data being replaced */)
{

	Trace("Update(%x, %x)\n", (unsigned)btree, (unsigned)new_data);

	if(btree->exhausted)
		return FAIL;
	if(btree->active_key == NULL) 
		return FAIL;
	mv_mem((char *)new_data, btree->active_key->KeyData.key_data,
			USRDATASIZE /* normally sizeof(long)*/ );
	(btree->hierarchy_list+btree->vector)->state = DIRTY;
	return SUCCESS;
} /* Update*/



/*
// NAME:	FlushHierarchy
//
// DESCRIPTION:	The hierarchy leading to a node is held in a vector of 
//		nodes beginning with the root node and ending with the
//		active node.  Nodes that have been modified are marked
//		in the hierarchy as "dirty".  When a new node is being
//		listed into the hierarchy, it may overlay a node already
//		listed in the node.  The underlying node is written back
//		to disk before the new node overwrites.  This function
//		walks the hierarchy, writes dirty nodes to disk and 
//		marking them clean.
//
// RETURN:	SUCCESS or FAIL
//
*/
int FlushHierarchy(BTREE * btree /* pointer to tree descriptor */)  
{
/* *** Although left as though it is completely empty, the loaded */
/* *** nodes are still there, and will be reused by load routines*/
/* *** later as required.*/
	int position, doexchange;
	long exchange_addr;
	long child;

	Trace("FlushHierarchy(%x)\n", (unsigned)btree);

	for(position = 0; position < btree->blocks_to_flush ; position++){
#ifdef COMPRESS
/* this routine will compress the tree into the lowest addressed blocks*/
		if( (position + 1) < btree->blocks_to_flush){
			child = (btree->hierarchy_list+(position+1))->block_id;
			/* look for the child*/
			if( ( btree->bfh->first_free_block_addr != -1L )&&
				( child > btree->bfh->first_free_block_addr)) {

		                OpenKeyNode(btree,&(btree->hierarchy_list+position)->hierarchy_node);
				doexchange = 0;
				if( btree->left == child ){
					exchange_addr = BfhNewBlock(btree->bfh);
					btree->active_node->nodeblock.left_node = exchange_addr;
					doexchange=1;
				}else{
					do{
					    if(btree->active_key->right_node == child){
						    exchange_addr = BfhNewBlock(btree->bfh);
					    	    btree->active_key->right_node = exchange_addr;
						    doexchange=1;
						    break;
					    }
					}while(IncrementPosition(btree)==SUCCESS);
				}
				if(doexchange){
					(btree->hierarchy_list+position)->state = DIRTY;
					BfhFreeBlock(btree->bfh,(btree->hierarchy_list+(position+1))->block_id);

					(btree->hierarchy_list+(position+1))->block_id = exchange_addr;
					(btree->hierarchy_list+(position+1))->state = DIRTY;
				}
			}
		}
#endif
		if((btree->hierarchy_list+position)->state == DIRTY){
                        if(BfhWriteBlock(btree->bfh, (char *)&((btree->hierarchy_list+position)->hierarchy_node),(btree->hierarchy_list+position)->block_id) == SUCCESS){
				(btree->hierarchy_list+position)->state = CLEAN;
			}
			else {
				Debug("Couldnt write block\n");
				return FAIL;
			}
		}
	}
	btree->vector = -1;
	btree->blocks_to_flush = 0;
	btree->active_node = NULL;
return SUCCESS;
} /* FlushHierarchy*/

/*
// NAME: 	LoadHierarchyNode
//
// DESCRIPTION:	Load a node into hierarchy at the current vector
//
// RETURN:	SUCCESS or FAIL
//
*/
int LoadHierarchyNode(BTREE * btree /* pointer to tree descriptor */,
		             long n /* node number */)
{

	Trace(" LoadHierarchyNode(%x,%ld)\n", (unsigned)btree, n);
	if (btree->vector < -1) {
		Debug("Invalid node vector\n");
		return FAILED;
		}
	++btree->vector;
	/* Want to create really huge b+trees?*/
/* law 2/4/94 */
	if( (btree->vector + 1) > btree->ActiveHeight ){
		btree->hierarchy_list = (struct HierarchyBlock *)
			realloc((void *)btree->hierarchy_list,
				( (btree->ActiveHeight+1)*sizeof(struct HierarchyBlock)) );
		if(btree->hierarchy_list == NULL)
			Debug("Insufficient available memory\n");
		/* new block in hierarchy gets set clean*/
		(btree->hierarchy_list+btree->ActiveHeight)->block_id = -1L;
		(btree->hierarchy_list+btree->ActiveHeight)->state = CLEAN;
		btree->ActiveHeight++;
	}
	if( ((btree->hierarchy_list+btree->vector)->state == DIRTY) && 
		((btree->hierarchy_list+btree->vector)->block_id != n) ){
			if(BfhWriteBlock(btree->bfh,(char*)&((btree->hierarchy_list+(btree->vector))->hierarchy_node), (btree->hierarchy_list+(btree->vector))->block_id) == FAIL){
			Debug("Couldnt write block\n");
			return FAIL;
			}
		}
	if((btree->hierarchy_list+btree->vector)->block_id == n){
		OpenKeyNode( btree,&(btree->hierarchy_list+btree->vector)->hierarchy_node);
		if(btree->blocks_to_flush < (btree->vector + 1))
			btree->blocks_to_flush = (btree->vector + 1);
		return SUCCESS;
		}
	if(BfhReadBlock(btree->bfh,(char *)&(btree->hierarchy_list+btree->vector)->hierarchy_node,n) != 
			SUCCESS){
		btree->active_node = NULL;
		Debug("Couldnt read block\n");
		return FAIL;
		}
	OpenKeyNode( btree,&(btree->hierarchy_list+btree->vector)->hierarchy_node);
	(btree->hierarchy_list+btree->vector)->block_id = n;
	(btree->hierarchy_list+btree->vector)->state = CLEAN;
	if(btree->blocks_to_flush < (btree->vector + 1))
		btree->blocks_to_flush = (btree->vector + 1);
	return SUCCESS;
} /* LoadHierarchyNode*/
	
/*
// NAME:	Parent
//
// DESCRIPTION:	Move up the hierarchy and make the parent node the new active
//		node.
//
// RETURN:	Pointer to the parent node
//
*/
static struct Node * Parent(BTREE * btree /* pointer to tree descriptor */)
{
	Trace("Parent(%x)\n", (unsigned)btree);
	if( btree->vector < 1 )
		return NULL;
	--btree->vector;
	OpenKeyNode( btree,&(btree->hierarchy_list+btree->vector)->hierarchy_node);
	return &((btree->hierarchy_list+btree->vector)->hierarchy_node);
}

/*
// NAME:	KeyGroupSize
//
// DESCRIPTION:	Provides a means to determine the size of a key group
//
// RETURN:	integer size of the key group
//
*/
int KeyGroupSize(BTREE * btree /* pointer to tree descriptor */,
	         struct KeyGroup * ks /* An encapsolated key */)
{
	int i;

	Trace("KeyGroupSize(%x,%x)\n", (unsigned)btree, (unsigned)ks);
	i = sizeof(long) + sizeof(ks->KeyData);
	switch(btree->type_of_key){
		case VAR_LNG_KEY:
			i+=(int)ks->k.key[0]+1;
/* law fix 1/29/94 */
#ifdef ALIGN2
                        	i += ( 2 - ( (i+2) % 2 ) );
#endif
#ifdef ALIGN4
                        	i += ( 4 - ( (i+4) % 4 ) );
#endif
			break;

		case FIX_LNG_KEY:
		case LONG_KEY:
		case DOUBLE_KEY:
			i+=btree->fix_lng_key_lng;
			break;
		default:
			Debug("Invalid KeyType\n");
	}
	return i;
}
	
/* HERE --- can't allow this to calloc memory junk */
/*
// NAME:	CopyKey
//
// DESCRIPTION:	Copies the active_key into a store area, and returns a pointer
//		to the area.  Calling programs cannot then corrupt the key.
//
// RETURN:	pointer to a key group
//
struct KeyGroup * CopyKey(BTREE * btree )
{
	Trace("CopyKey(%x)\n", (unsigned)btree);
	if(btree->key_copy == NULL){
		btree->key_copy = (struct KeyGroup *)calloc(1,sizeof(struct KeyGroup));
		if(btree->key_copy == NULL){
			Debug("Insufficient available memory\n");
		}
	}
	mv_mem((char *)btree->active_key,(char *)btree->key_copy,KeyGroupSize(btree,btree->active_key));
	return btree->key_copy;
}
*/

/*
// NAME:	InsertNodeKeyGroup
//
// DESCRIPTION:	Submit a key group for insert into a node.  If the node
//		is too full to accept the key, return FAIL, otherwise
//		insert at the current position in the node.
//
// RETURN:	SUCCESS or FAIL.  A full node returns FAIL, while anything
//		else inserts and returns SUCCESS. 
//
*/
static int  InsertNodeKeyGroup(BTREE * btree /* pointer to tree descriptor */,
	   		struct KeyGroup * kg /* An encapsolated key */)
{
	int i,active_posn;

	Trace("InsertNodeKeyGroup(%x,%x)\n",
		(unsigned)btree, (unsigned)kg);
	i = KeyGroupSize(btree,kg);
	if( (btree->block_size - btree->active_node->next_free_position ) < i ) {
		return FAIL;  
	}
	active_posn = (char *)btree->active_key - (char *)btree->active_node;
	mv_mem((char *)btree->active_node + active_posn, 
		(char *)btree->active_node + active_posn + i,
		btree->block_size - i - active_posn);
	mv_mem((char *)kg,(char *)btree->active_node+active_posn,i);
	btree->active_node->total_node_keys ++;
	btree->active_node->next_free_position += i;
	if(btree->vector != -1)
		(btree->hierarchy_list+btree->vector)->state = DIRTY;
	return SUCCESS;
}

/*
// NAME:	InsertSpareKeyGroup
//
// DESCRIPTION:	This function is used to build the spare node.  Called
//		repetively, this function inserts key groups into the 
//		spare node in the sequence provided, at the end of the
//		list.
//
// RETURN:	SUCCESS or FAIL
//
*/
static int  InsertSpareKeyGroup(BTREE * btree /* pointer to tree descriptor */,
			 struct KeyGroup * kg /* An encapsolated key */,
			 struct Node * spare_node /* Pointer to working spare */)
{
	/* insert into the spare node, end position only*/
	int i;

	Trace("InsertSpareKeyGroup(%x,%x,%x)\n", (unsigned)btree, 
		(unsigned)kg, (unsigned)spare_node);
	i = KeyGroupSize(btree,kg);
/*fprintf(stderr, "%d - ( %d + 1 ) <= %d\n",btree->block_size, spare_node->next_free_position, i);*/
	if( (btree->block_size - spare_node->next_free_position ) < i ) {
/*runtime_error(TRACE,__FILE__,__LINE__,"Node too full for insert");*/
		return FAIL;
	}  /* cant stuff any more here*/
	mv_mem((char *)kg,(char *)spare_node+spare_node->next_free_position,i);
	spare_node->total_node_keys ++;
	spare_node->next_free_position += i;
	return SUCCESS;
}


/*
// NAME:	DeleteNodeKeyGroup
//
// DESCRIPTION:	Deletes the current active key by moving down the contents
//		of upper end of the node past the current key to the 
//		current position.
//
// RETURN:	SUCCESS or FAIL
//
*/
static int  DeleteNodeKeyGroup(BTREE * btree /* pointer to tree descriptor */)
{
	int len,i;
	char * cp;

	Trace("DeleteNodeKeyGroup(%x)\n", (unsigned)btree);
	if(btree->key_position > btree->active_node->total_node_keys){
		/* cant delete at node eof*/
		Debug("Invalid delete position\n");
		return FAIL;
	}
	i = KeyGroupSize(btree /* pointer to tree descriptor */,btree->active_key);
	len = ( ((char *)btree->active_node + btree->block_size ) - ( (char *)btree->active_key + i ) );
	cp = (char *)btree->active_key;
	mv_mem(cp + i, cp, len ); 
	--btree->active_node->total_node_keys;
	btree->active_node->next_free_position -= i;
	(btree->hierarchy_list+btree->vector)->state = DIRTY;
	return SUCCESS;
}/*DeleteNodeKeyGroup()*/

/*
// NAME:	Combine
//
// DESCRIPTION: 	This function attempts to combine sibling nodes following a
//		key deletion.  Attempt is first made with the right node, 
//		and if that fails, with the left node.  If it results in the
//		parent root being emptied, a new root node is created.
//		Combine must be called immediately before a return 
//		and after all other processing on the hierarchy is completed.
//
// RETURN:	SUCCESS or FAIL
//
*/
	
static int Combine(BTREE * btree /* pointer to tree descriptor */)
{
	long combine_id;
	int combine_key_count;
	int combine_data_size;
	int combine_key_posn;

	long sibling_id;
	int sibling_key_count;
	int sibling_data_size;
	int sibling_key_posn;
	int sibling_key_size;

	long parent_id;
	int parent_key_count;
	int parent_data_size;
	int parent_key_posn;
	int parent_key_size;
	int parent_pass_through_space;

	int status, i;
	int on_right, parent_is_root;

	struct Node * spare_node;
/* HERE - put the node on the statk??*/
	struct KeyGroup * spare_key;

	struct KeyGroup parent_key;

	int root_exhausted;

	long hold_left;

	Trace("Combine(%x)\n", (unsigned)btree);

	on_right = 0; 
	combine_id = sibling_id = 0L; 
	combine_key_count = parent_key_size = sibling_key_count = 0; 
	root_exhausted = 0; 
	/* save our current id */
	combine_id = (btree->hierarchy_list+btree->vector)->block_id; 
	if(Parent(btree)==NULL) { 
		Debug("Couldnt locate parent\n"); 
		return FAIL; 
	}

	/* Mark the parent as being root node*/
	if(btree->vector == 0){
		parent_is_root = 1;
	}else{
		parent_is_root = 0;
	}

	/* find the right side*/
	status = SUCCESS;
	while(btree->left != combine_id){
		if(btree->active_key->right_node == combine_id)
			on_right = 1;
 		if( (status=IncrementPosition(btree)) != SUCCESS ){
			if(on_right){
				break;
			}
 			Debug("Failed to increment\n");
 			return FAIL;
 		}
		on_right = 0;
	} 
	if(on_right){
		/* This is the condition where we are */
		/* dealing with the far right node in a tree*/
		Rewind(btree);
		while(btree->active_key->right_node != combine_id){
 			if( (status=IncrementPosition(btree)) != SUCCESS ){
				Debug("Failed to increment\n");
				return FAIL;
			}
		}
		combine_id = btree->left;
		/* we have now swapped positions and are working with */
		/* a separate set of nodes*/
	}

	/* this would be the parent key*/
	mv_mem((char *)btree->active_key, (char *)&parent_key, 
		KeyGroupSize(btree,btree->active_key));
	parent_key_size = KeyGroupSize(btree,btree->active_key);
	sibling_id = btree->active_key->right_node;
	if( (btree->vector == 0) && (btree->active_node->total_node_keys == 1))
		root_exhausted = 1;

	/* --- we have our left and right sides*/

	/* get left side data*/
	if( LoadHierarchyNode( btree,combine_id ) == FAIL){ 
		Debug("Cant load node\n");
		return FAIL;
	}
	/* begin building the spare node*/
	spare_node = (struct Node *)malloc(sizeof(struct Node) );
/*fprintf(stderr, "%ld\n", (long)spare_node);*/
	if(spare_node == NULL){
		Debug("Insufficient available memory\n");
	}
	mv_mem( (char *)btree->active_node, (char *)spare_node, sizeof(struct Node) );

	/* collect information*/
	combine_key_count = btree->active_node->total_node_keys;
	combine_key_posn =  btree->active_node->next_free_position;
	combine_data_size = btree->active_node->next_free_position - FirstKeyOffset;

	if(Parent(btree)==NULL){
		Debug("Couldnt locate parent\n");
		return FAIL;
	}
	parent_key_count = btree->active_node->total_node_keys;
	parent_key_posn =  btree->active_node->next_free_position;
	parent_data_size = btree->active_node->next_free_position - FirstKeyOffset;
	parent_id = (btree->hierarchy_list+btree->vector)->block_id; 
	parent_pass_through_space = KeySpace - parent_data_size + 
		parent_key_size;

	/* get right side data*/
	if( LoadHierarchyNode( btree,sibling_id ) == FAIL){ 
		Debug("Cant load node\n");
		free((void*)spare_node);
		return FAIL;
	}
	sibling_key_count = btree->active_node->total_node_keys;
	sibling_key_posn =  btree->active_node->next_free_position;
	sibling_data_size = btree->active_node->next_free_position - FirstKeyOffset;
	sibling_key_size = KeyGroupSize(btree,btree->active_key);
	parent_key.right_node = btree->left;

	/* have we been wasting our time*/
	if(
		/* it is too big to combine*/
	((combine_data_size+parent_key_size+sibling_data_size) > KeySpace ) ||
		/* or, it is a variable length node and there wouldn't be */
		/* space in the node if combined, to fit in an extra key*/
	( (btree->type_of_key == VAR_LNG_KEY) && 
	  ((combine_data_size+parent_key_size+sibling_data_size) > 
		(( KeySpace / 5 ) * 4)))
	)
	
	{
		/* then lets shift left*/

		if (	

			/* if we have keys to shift*/
		   (    sibling_key_count > 2) &&

			/* and it would balance*/
		   (	sibling_key_count > combine_key_count ) &&

			/* and at least 1/5 th the available space is there*/
			/* to accept a shifted key*/
		   (    combine_data_size  < (( KeySpace / 5 ) * 4 )) &&

			/* and if variable length keys, the sibling*/
			/* key can fit into the parent*/
		   ( ! ( (btree->type_of_key == VAR_LNG_KEY ) &&
		    (  (sibling_key_size+1) > parent_pass_through_space ))) 
		)

		{
			/* steal - shift some of the sibling to the*/
			/* combine node*/
			/* - we have the parent key, and are at the */
			/* sibling.  We need the first sibling key in */
			/* the parent, and the parent key in the combine*/
			free((void *)spare_node);
			Rewind(btree);
			spare_key = (struct KeyGroup *)
				malloc(sizeof(struct KeyGroup));
			if(spare_key == NULL){
				Debug("Insufficient available memory\n");
			}
			mv_mem((char *)btree->active_key, (char *)spare_key, 
				KeyGroupSize(btree,btree->active_key));
			/* got the first sibling key, now remove it*/
                	if( DeleteNodeKeyGroup(btree) == FAIL ){
                       		Debug("Key not deleted \n");
				return FAIL;
                	}
			/* have both keys, now we need to shuffle the */
			/* pointers*/
			/* assuming active node is the sibling*/
			parent_key.right_node = btree->active_node->nodeblock.left_node;
			btree->active_node->nodeblock.left_node = spare_key->right_node;
			spare_key->right_node = sibling_id;
        		(btree->hierarchy_list+btree->vector)->state = DIRTY;

			/* subsitute in the parent the sibling key*/
			if(Parent(btree)==NULL){
				Debug("Couldnt locate parent\n");
				free((void *)spare_key);
				return FAIL;
			}
			while(btree->active_key->right_node != sibling_id){
 				if( (status=IncrementPosition(btree)) != SUCCESS ){
					Debug("Failed to increment\n");
					free((void *)spare_key);
					return FAIL;
				}
			}
       		        if( DeleteNodeKeyGroup(btree) == FAIL ){
	       	                Debug("Key loss deleting during split\n");
				free((void *)spare_key);
				return FAIL;
       		        }
/* HERE var lng poten fail */
                	if( InsertNodeKeyGroup(btree,spare_key) == FAIL){
                        	Debug("Couldnt insert replacement key\n");
                        	free((void *)spare_key);
                        	return FAIL;
                        }
			free((void *)spare_key);
			if( LoadHierarchyNode( btree,combine_id ) == FAIL){ 
				Debug("Cant load node\n");
				return FAIL;
			}
                	if(btree->active_node->total_node_keys > 0){
                        	for(;;){
                                	status = Compare(btree,(union Key *)
						parent_key.k.key);
                                	if(status != 1 )
                                        	break;
                                	if((status = IncrementPosition(btree)) != 
						SUCCESS){
                                        	break;
                                	}
                        	}
                	}
                	if(InsertNodeKeyGroup(btree,&parent_key) == FAIL){
                        	Debug("lost a key(s)\n");
                	}
			return SUCCESS;
		} 
		/* then lets shift right*/
		
		if (	
			/* if we have keys to shift*/
			( combine_key_count > 2) &&

			/* and it would balance*/
		   (	sibling_key_count < combine_key_count ) &&

			/* and at least 1/5 th the available space is there*/
		   (    sibling_data_size  < (( KeySpace / 5 ) * 4 ))
		   )
		{

			free((void *)spare_node);
			if(Parent(btree)==NULL){
				Debug("Couldnt locate parent\n");
				free((void *)spare_key);
				return FAIL;
			}
			if( LoadHierarchyNode( btree,combine_id ) == FAIL){ 
				Debug("Cant load node\n");
				return FAIL;
			}
			Rewind(btree);
			spare_key = (struct KeyGroup *)
				malloc(sizeof(struct KeyGroup));
			if(spare_key == NULL){
				Debug("Insufficient available memory\n");
			}
			for(i=1;i<combine_key_count;i++){
				if( IncrementPosition(btree) != SUCCESS){
					free((void *)spare_key);
					return FAIL;
				}
			}

			/* and if variable length keys, the sibling*/
			/* key can fit into the parent*/
			if( (btree->type_of_key == VAR_LNG_KEY) &&
		    	    (  (KeyGroupSize(btree,btree->active_key)+1) > 
					parent_pass_through_space )
			  ){
				return FAIL;
			}
			mv_mem((char *)btree->active_key, (char *)spare_key, 
				KeyGroupSize(btree,btree->active_key));
			/* got the last combine key, now remove it*/
                	if( DeleteNodeKeyGroup(btree) == FAIL ){
                       		Debug("failed to delete key\n");
				return FAIL;
                	}
			/* have both keys, now we need to shuffle the */
			/* pointers*/
			/* assuming active node is the sibling*/
			hold_left = spare_key->right_node;
			spare_key->right_node = sibling_id;

        		(btree->hierarchy_list+btree->vector)->state = DIRTY;

			/* subsitute in the parent the sibling key*/
			if(Parent(btree)==NULL){
				Debug("Couldnt locate parent\n");
				free((void *)spare_key);
				return FAIL;
			}
			while(btree->active_key->right_node != sibling_id){
 				if( (status=IncrementPosition(btree)) != SUCCESS ){
					Debug("Failed to increment\n");
					free((void *)spare_key);
					return FAIL;
				}
			}
       		        if( DeleteNodeKeyGroup(btree) == FAIL ){
	       	                Debug("failed to delete key\n");
				free((void *)spare_key);
				return FAIL;
       		        }
/* HERE var lng poten fail */
                	if( InsertNodeKeyGroup(btree,spare_key) == FAIL){
                        	Debug("Couldnt insert replacement key\n");
                        	free((void *)spare_key);
                        	return FAIL;
                        }
			free((void *)spare_key);
			if( LoadHierarchyNode( btree,sibling_id ) == FAIL){ 
				Debug("Cant load node\n");
				return FAIL;
			}
			parent_key.right_node = btree->active_node->nodeblock.left_node;
			btree->active_node->nodeblock.left_node = hold_left;
	
                	if(InsertNodeKeyGroup(btree,&parent_key) == FAIL){
                        	Debug("lost a key(s)\n");
                	}
/*Debug(btree->active_node, 0);*/
			return SUCCESS;
		}

		if 	((combine_key_count < 2) || (sibling_key_count < 2))
		{
/*
fprintf(stderr, "UNDERFLOW\n");
fprintf(stderr, "combine_id %ld combine_key_count %d combine_data_size %d \ncombine_key_posn %d sibling_id %ld sibling_key_count %d \nsibling_data_size %d sibling_key_posn %d parent_id %ld parent_key_count %d \nparent_data_size %d parent_key_posn %d\n",
combine_id, 
combine_key_count, combine_data_size, combine_key_posn, 
sibling_id, sibling_key_count, sibling_data_size, sibling_key_posn,
parent_id, parent_key_count, parent_data_size, parent_key_posn);
*/
			free((void *)spare_node);
			return FAIL;
		}
		free((void *)spare_node);
		/* It is success because it is sufficiently well balanced*/
		/* anyway*/
		return SUCCESS;
	}

	/* put it all together*/
        if( (status = InsertSpareKeyGroup(btree,&parent_key,spare_node)) == FAIL ){
                free( (void *)spare_node);
                Debug("Failed to Insert\n");
                return FAIL;
        }
        mv_mem( ((char *)btree->active_node)+FirstKeyOffset,
                ((char *)spare_node)+spare_node->next_free_position,
                sibling_data_size );
	spare_node->total_node_keys+=sibling_key_count;
	spare_node->next_free_position+=sibling_data_size;

	/* position to the results node*/
	if(Parent(btree)==NULL){
		Debug("Couldnt locate parent\n");
		free((void *)spare_node);
		return FAIL;
	}
	if( LoadHierarchyNode( btree,combine_id ) == FAIL){ 
		Debug("Cant load node\n");
		free((void *)spare_node);
		return FAIL;
	}

	/* replace the data with the combined node data*/
        mv_mem((char *)spare_node,
               (char *)btree->active_node,
               sizeof(struct Node));
        (btree->hierarchy_list+btree->vector)->state = DIRTY;
	BfhFreeBlock(btree->bfh,sibling_id);

	free((void *)spare_node);
        if(root_exhausted){
                /* then we have a new root node, the key we just*/
                /* combined*/
		long free_block_id;

                (btree->hierarchy_list+0)->block_id = -1L;
                (btree->hierarchy_list+0)->state = CLEAN;
                /* make it the root node*/
                free_block_id=(btree->hierarchy_list+btree->vector)->block_id;
                (btree->hierarchy_list+btree->vector)->block_id = btree->rootnode;
                if(BfhFreeBlock(btree->bfh, free_block_id ) == FAIL){
                        Debug("Cant free block\n");
                        return FAIL;
                }
                btree->search_levels --;
        }else{
		/* we still have a key in the root that needs to */
		/* be removed*/
		if(Parent(btree)==NULL){
			Debug("Couldnt locate parent\n");
			return FAIL;
		}
		while(btree->active_key->right_node != sibling_id){
 			if( (status=IncrementPosition(btree)) != SUCCESS ){
				Debug("Failed to increment\n");
				return FAIL;
			}
		}
                if( DeleteNodeKeyGroup(btree) == FAIL ){
                        Debug("Key loss deleting during split\n");
			return FAIL;
                }
	}

	FlushHierarchy(btree);
	return SUCCESS;
} /* end Combine()*/


/*
// NAME:	RemoveNodeKeyGroup
//
// DESCRIPTION:	Deletes the current active key by moving down the contents
//		of upper end of the node past the current key to the 
//		current position, and then inserting into the position
//		the next greater key in the tree (leaf position).
//
// RETURN:	SUCCESS or FAIL
//
*/
static int  RemoveNodeKeyGroup(BTREE * btree /* pointer to tree descriptor */)
{
	struct KeyGroup delete_key;
	struct KeyGroup replacement_key;
	int space_available;
	int saved_vector;
	long right_pointer;
	int shift_vector;

	shift_vector = 0;

	Trace("RemoveNodeKeyGroup(%x)\n", (unsigned)btree);
	/* keep important info*/
	mv_mem((char *)btree->active_key, (char *)&delete_key, 
		KeyGroupSize(btree,btree->active_key));
	saved_vector = btree->vector;
	right_pointer = btree->active_key->right_node;

	/* How much space do we have for the replacement key?*/
	space_available = KeyGroupSize(btree,btree->active_key);
	space_available += btree->block_size - btree->active_node->next_free_position;

	if(btree->active_key->right_node == -1L){
		Debug("remove operation for interior nodes\n");
		return FAIL;
	}

	/* Get the leaf key next in the node*/
	if(LoadHierarchyNode(btree,btree->active_key->right_node) == SUCCESS){
		while(btree->left != -1L){
			if(LoadHierarchyNode(btree,btree->left) != SUCCESS){
				Debug("Failed to load node\n");
				return FAIL;
			}
		}
	}else{
		Debug("Failed to load node\n");
		return FAIL;
	}
        if(btree->active_key->right_node != -1L){
/*Debug(btree->active_node, 0);*/
                Debug("Unbalanced node\n");
                return FAIL;
        }
/*	// will the key we are looking at fit??*/
/*	if( KeyGroupSize(btree,btree->active_key) > space_available ){*/
/*		//boink*/
/*if(DEVELOPMENT)fprintf(stderr,"key too big\n");*/
/*		return FAIL;*/
/*	}*/

	/* save it*/
	mv_mem((char *)btree->active_key, 
		(char *)&replacement_key, KeyGroupSize(btree,btree->active_key));

	/* delete it*/
	DeleteNodeKeyGroup(btree);

	/* It may be necessary to adjust the entire tree*/
	CombineAll(btree);   /* starts with a leaf shift which will */
			/* take care of empty leaves*/
	/* goback*/
	FlushHierarchy(btree);
        if(LocateExact(btree,(union Key *)&delete_key.k.key) != SUCCESS){
                Debug("Lost key, Couldnt locate key\n");
                return FAIL;
        }

	/* new key points to old key's right side*/
	replacement_key.right_node = btree->active_key->right_node;

	/* remove the original */
	if(DeleteNodeKeyGroup(btree) != SUCCESS){
		Debug("failed to delete key\n");
		return FAIL;
	}

/* insert the key from the leaf*/
/*HERE what if too big?*/
	if ( InsertNodeKeyGroup( btree,&replacement_key ) == FAIL){
		Debug("couldn't insert, better rebuild\n");
		return FAIL;
	}

	return SUCCESS;
}/*RemoveNodeKeyGroup*/


/*
// NAME:	LeftAddress
//
// DESCRIPTION:	Provides the public accessability to the left pointer
//		to the active key.
//
// RETURN:
//
*/
static long LeftAddress(BTREE * btree /* pointer to tree descriptor */)
{
	return btree->left;
}

/*
// NAME:	IncrementPosition
//
// DESCRIPTION:	Move up one position in the current active node.
//
// RETURN:	SUCCESS FAIL NODE_EOF
//
*/
int  IncrementPosition(BTREE * btree /* pointer to tree descriptor */)
{
	/* *** Increment past the last key is allowed in order to add a */
	/* *** key at the end of the key list.  */
	Trace("IncrementPosition(%x)\n", (unsigned)btree);
Trace("");
	if(( btree->key_position + 1 ) > ( btree->active_node->total_node_keys + 1 ) ){
		btree->active_key = NULL;
		return FAIL;
	}
	btree->left = btree->active_key->right_node;
	/* if this is the node eof the active key is invalid*/
	btree->active_key = (struct KeyGroup *)((char *)btree->active_key + KeyGroupSize(btree,btree->active_key));
	btree->key_position ++;
	if( btree->key_position == ( btree->active_node->total_node_keys + 1 ) )
		return NODE_EOF;
	return SUCCESS;
} /* IncrementPosition*/

/*
// NAME:	Rewind
//
// DESCRIPTION:	Rewind the active key to the first position
//
// RETURN:	NONE
//
*/
void  Rewind(BTREE * btree /* pointer to tree descriptor */)
{
	Trace("Rewind(%x)\n", (unsigned)btree);
	btree->left = btree->active_node->nodeblock.left_node;
	btree->key_position = 1;
	btree->active_key = (struct KeyGroup *)((char *)btree->active_node + FirstKeyOffset); 
}

/*
// NAME:	Compare
//
// DESCRIPTION:	Access the installed compare function, and compare the
//		passed key in a union Key to the active key.
//
// RETURN:
//
*/
int   Compare(BTREE * btree /* pointer to tree descriptor */,
	      union Key *value /* An encapsolated key */)
{
	return (*btree->compare_funct)(value,btree->active_key,btree->fix_lng_key_lng);
}

/*
//
// NAME:	Currentkey
//
// DESCRIPTION:	
//
// RETURN:
//
*/
int  Currentkey(BTREE * btree, /* pointer to tree descriptor */
                struct KeyGroup * ckp /* An encapsolated key */ )
{
	Trace("Currentkey(%x,%x)\n", (unsigned)btree, (unsigned)ckp);
	mv_mem((char *)btree->active_key,(char *)ckp,KeyGroupSize(btree,btree->active_key));
return SUCCESS;
}
