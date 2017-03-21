/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                Larry A. Walker & Co. B+TreeR2
        Corp. 1991, 1995 Larry A. Walker (Larry A. Walker & Co.)
                        All rights reserved.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "treelib.h"
#include "btree.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Non required debugging and informational functions
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
// NAME:	DumpBtree
//
// DESCRIPTION:	Walk every block in the btree file and write all the 
//		header and key information.
//
// RETURN:	NONE
*/
void DumpBtree ( BTREE * btree){
	long l,i;

	Trace("DUMP(%x)\n",(unsigned)btree);

	FlushHierarchy(btree);/*If we play this game it clean*/
	for(i = 0L;;i++){
		l = BfhAddrBlock(btree->bfh,i);
		if(l == -1L)break;
		if(BfhFullBlock(btree->bfh,i)){
			GetPrintNode(btree,l);
		}
		else  {
			Debug("\n***** Block %ld Address %ld is Free Block\n\n",i,l);
		}
	}
}

/*
//
// NAME:	GetPrintNode
//
// DESCRIPTION:	Print the node information for a block
//
// RETURN:	NONE
*/
void GetPrintNode (BTREE * btree,long l )
{

	LoadHierarchyNode(btree,l);
	PrintNode(btree,btree->active_node,0);
	btree->vector = -1;
} 

/*
// NAME:	PrintNode
//
// DESCRIPTION:	Print the header information and all the keys in a node.
//		If pause is set, wait for a keystroke before continuing.
//
// RETURN:	NONE
//
*/
void PrintNode (BTREE * btree,struct Node * np , int pause) 
{
	int i;
	struct Node * hold_active_node; 
	struct KeyGroup * hold_active_key;
	int hold_key_position;
	long hold_left;

	Trace("TreeDebug (%x,%x,%d)\n", (unsigned)btree, (unsigned)np, pause);

	hold_active_node = btree->active_node;
	hold_active_key = btree->active_key;
	hold_key_position = btree->key_position;
	hold_left = btree->left;
	
	if(np == NULL) np = btree->active_node;
	OpenKeyNode(btree,np);
	View("----------------------------> Node Debug \n");
	DisplayNodeHeader(btree);
	Rewind(btree);
	for(i = 0; i < btree->active_node->total_node_keys; i++ ){
		if(pause){
			if((i%18) == 17){
				View("press <return> to continue\n");
				getchar();
			}
		}
		View("%d: ",i+1);
		DisplayKey(btree,btree->active_key);
		IncrementPosition(btree);
	}
	View("----------------------------> End Debug\n");
	btree->active_node = hold_active_node;
	btree->active_key = hold_active_key;
	btree->key_position = hold_key_position;
	btree->left = hold_left;
	if(pause){
		View("press <return> to continue\n");
		getchar();
	}
	return;
}

/*
// NAME:	DisplayNodeHeader
//
// DESCRIPTION:	Displays information concerning the active node.
//
// RETURN:	NONE
//
*/
void DisplayNodeHeader(BTREE * btree){
	View("    Trunk:\nsearch_levels=%d duplicates=%d type_of_key=%d fix_lng_key_lng=%d rootnode=%ld\n",btree->search_levels,btree->duplicates,btree->type_of_key,btree->fix_lng_key_lng,btree->rootnode);
	View("    Hierarchy:\nblock_id=%ld state=%d\n",(btree->hierarchy_list+btree->vector)->block_id,(btree->hierarchy_list+btree->vector)->state); 
	View("    Node:\ntotal_node_keys=%d next_free_position=%d left_node=%ld\n",btree->active_node->total_node_keys, btree->active_node->next_free_position, btree->active_node->nodeblock.left_node);
	View("    Keys (intercepted key position: %d)\n",btree->key_position);
}

/*
// NAME:	DisplayKey
//
// DESCRIPTION:	Print the contents of a single key group
//
// RETURN:	NONE
//
*/
void DisplayKey(BTREE * btree,struct KeyGroup * kg){
	char buffer[256];
	int i;
#ifdef _64BITFPU
	double d;
#endif
	if(kg == NULL){
		View("NO KEYS\n");
		return;
	}
	switch(btree->type_of_key){
		case VAR_LNG_KEY:
			create_c_string(buffer,kg->k.key);	
			break;
		case LONG_KEY:
			sprintf(buffer,"%ld",kg->k.long_key);
			break;
		case DOUBLE_KEY:
#ifdef _64BITFPU
			memcpy((void*)&d,(void*)&(kg->k.double_key),
				sizeof(float));
			sprintf(buffer,"%f",d);
#else
			sprintf(buffer,"%f",kg->k.double_key);
#endif
			break;
		case FIX_LNG_KEY:
			for(i = 0; i < btree->fix_lng_key_lng; i++){
				if(i == 255) break;
				buffer[i] = kg->k.key[i];
			}
			buffer[i] = 0;
			break;
	}
	View("position=%d right node=%ld addr=%ld key=%s\n", btree->key_position, kg->right_node,kg->KeyData.address,buffer);
	/*View("<return> to continue\n");*/
	/*getchar();*/
}

/*
// NAME:	DisplayHierarchy
//
// DESCRIPTION:	Check the contents of the hierarchy.  Print a report
//		concerning the contents currently in the hierarchy
//
// RETURN:	NONE
//
*/
void DisplayHierarchy (BTREE * btree,int pause){
	int position;

	Trace("CHECK(%x,%d)\n", (unsigned)btree, pause);

	View("Block ID  State  Total Keys  Next Free  Vector: %d\n",btree->vector);
	View("--------  -----  ----------  ---------\n");

	for(position = 0; position < btree->blocks_to_flush ; position++){
		View("%8ld  %s  %10d  %9d\n",(btree->hierarchy_list+position)->block_id,( (btree->hierarchy_list+position)->state == DIRTY)?"DIRTY":"CLEAN",(btree->hierarchy_list+position)->hierarchy_node.total_node_keys,(btree->hierarchy_list+position)->hierarchy_node.next_free_position);
	}
	if(pause){
		View("press <return> to continue\n");
		getchar();
	}
}
/*
// NAME:	NodePosn
//
// DESCRIPTION:	Provides public access to the address of the block in the
//		hierarchy at the present vector.  Why? you ask.  Knowing
//		what level you are allows you to show a description of each
//		node as it pertains the its position in the tree.
//
// RETURN:	long value of block address in btree file.
		Return the active node position
//
//
*/ 
long NodePosn(BTREE * btree){
	return (btree->hierarchy_list+btree->vector)->block_id;
}

/*.*****************************************************
* Function Name: 	PrintBtDescriptor
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
* Copr 1991,1992,1995  Tue Mar 21 15:37:31 EST 1995 Larry A. Walker
*
*  Modified:
*
******************************************************/

void PrintBtDescriptor(BTREE * bt)
{
	if(bt == NULL){
		View("NULL Btree\n");
		return;
	}
	View("\t\tBtree Static info: Tree = %x\n", (unsigned)bt);
	View("magic %ld, search_levels %d, duplicates %s\n", 
		bt->magic, bt->search_levels, (bt->duplicates)?"YES":"NO");
	View("type_of_key %s, fix_lng_key_lng %d, rootnode %ld\n", 
		(bt->type_of_key==VAR_LNG_KEY)?"VARIABLE LENGTH KEY":
		(bt->type_of_key==FIX_LNG_KEY)?"FIXED LENGTH KEY":
		(bt->type_of_key==LONG_KEY)?"LONG KEY":
		(bt->type_of_key==DOUBLE_KEY)?"DOUBLE KEY":"INVALID", 
		bt->fix_lng_key_lng, bt->rootnode);

	View("number_of_keys %ld, exhausted %d, block_size %d lock_on %d\n", 
		bt->number_of_keys, bt->exhausted, bt->block_size, bt->lock_on);
	View("time_of_last_open %s, time_of_last_close %s", 
		ctime(&(bt->time_of_last_open)), ctime(&(bt->time_of_last_close)) );
	View("\t\tBtree Open Info:\n");
	View("vector %d, block_to_flush %d, ActiveHeight %d\n", 	
		bt->vector, bt->blocks_to_flush, bt->ActiveHeight);
	View("hierarchy_list %x, active_key %x, active_node %x,\n", 
		(unsigned)bt->hierarchy_list, (unsigned)bt->active_key, 
		(unsigned)bt->active_node );
	View("key_position %d, left %ld, past_exact_key %d\n",
		bt->key_position, bt->left, bt->past_exact_key);
	View("compare_funct %x, rootsplit %d, bfh %x\n", 
		(unsigned)bt->compare_funct, bt->rootsplit, (unsigned)bt->bfh);
	return;
	
} /*end  "PrintBtDescriptor" */
