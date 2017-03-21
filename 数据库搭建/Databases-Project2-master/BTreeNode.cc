#include "BTreeNode.h"

#include <iostream>
using namespace std;

/* Initialize fields to appropriate 0 values */
BTLeafNode::BTLeafNode()
{
	count = 0;
	list = NULL;
	end_pid = -1;
}

/* Free all the allocated key/rid airs */
BTLeafNode::~BTLeafNode()
{
	list_node* p1 = list;
	list_node* p2;
	if(list != NULL)
	p2 = list->next;

	while(p1 != NULL)
	{
		delete p1;
		p1 = p2;
		if(p1 == NULL)
			break;
		else p2 = p2->next;
	}
	
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{
    //check if the pid is valid
    if(pid < 0 || pid > pf.endPid()) return RC_INVALID_RID;

    memset(buffer,0,1024*sizeof(char));
    
    //read into the main memory buffer (1024 bytes)
    int ret = pf.read(pid,buffer);
    
    //unable to read, return same error code
    if(ret < 0) return ret;
    
    char* ptr = buffer;
    
    memcpy(&count, reinterpret_cast<int*>(ptr), sizeof(count));
    //printf(count);
    
    ptr += sizeof(count);
    
    memcpy(&end_pid, reinterpret_cast<int*>(ptr), sizeof(end_pid));
    ptr += sizeof(end_pid);

    
    list_node* curr = NULL;
    
    for(int i=0; i<count; i++)
    {
        if(i == 0)
        {
            //create root
            list = new list_node;
            memcpy(&list->id.rid, ptr, sizeof(RecordId));
            ptr += sizeof(RecordId);
            memcpy(&list->key, reinterpret_cast<int*>(ptr), sizeof(int));
            ptr += sizeof(int);
            list->next = NULL;
            curr = list;
        }
        else
        {
            curr->next = new list_node;
            curr = curr->next;
            memcpy(&curr->id.rid, ptr, sizeof(RecordId));
            ptr += sizeof(RecordId);
            memcpy(&curr->key, reinterpret_cast<int*>(ptr), sizeof(int));
            ptr += sizeof(int);
            curr->next = NULL;
        }
    }
    
    return 0;
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
    //check if the pid is valid

    if(pid < 0 || pid > pf.endPid()) return RC_INVALID_RID;

    memset(buffer, 0, 1024 * sizeof(char));
    
    char* ptr = buffer;

     memcpy(ptr, reinterpret_cast<char*>(&count), sizeof(count));

     ptr += sizeof(count);

     memcpy(ptr, reinterpret_cast<char*>(&end_pid), sizeof(end_pid));

     ptr += sizeof(end_pid);

     list_node* curr = list;
     list_node* prev;
     for(int i=0; i<count; i++)
    {
        if(i != 0){
           	prev->next = curr;
           }
            memcpy(ptr, reinterpret_cast<char*>(&curr->id.rid), sizeof(RecordId));
            ptr += sizeof(RecordId);
            memcpy(ptr, reinterpret_cast<char*>(&curr->key), sizeof(int));
            ptr += sizeof(int);
            prev = curr;
            if(curr == NULL)
           		break;
            curr = curr->next;
        
    }

    
    int ret = pf.write(pid,buffer);
    return ret;

}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ return count; }

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	
	if(count > N-2)
		return RC_NODE_FULL;

	list_node* temp = new list_node;
	temp->key = key;
	temp->id.rid = rid;
	temp->next = NULL;
	list_node* curr = list;

	if(list == NULL)
		{
			list = temp;
			list->next = NULL;
		}

	else {
		

		if(list->key > temp->key)
		{
			temp->next = curr;
			list = temp;
		}

	else {
		while(curr)
		{
			if(!curr->next)
				{
					curr->next = temp;
					break;
				}

			if(curr->next->key > temp->key)
			{
				temp->next = curr->next;
				curr->next = temp;
				break;
			}

			curr = curr->next;

		}

	}
		
	}

	count++;

	return 0;
 }

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	/* Check that sibling is empty */
	if(sibling.getKeyCount() > 0)
		return RC_NODE_FULL;


    /* Insert the given pair into this node */

	list_node* temp = new list_node;
	temp->key = key;
	temp->id.rid = rid;
	temp->next = NULL;
	list_node* curr = list;

	if(list == NULL)
		{
			list = temp;
		}

	else {
		

		if(list->key > temp->key)
		{
			temp->next = curr;
			list = temp;
		}

	else {
		while(curr)
		{
			if(!curr->next)
				{
					curr->next = temp;
					break;
				}

			if(curr->next->key > temp->key)
			{
				temp->next = curr->next;
				curr->next = temp;
				break;
			}

			curr = curr->next;

		}

	}
		
	}

/* Find the middle pair in this node */

	list_node* mid = list;
	list_node* fast = list;

	while(true)
	{
		if(fast == NULL)
			break;
		if(fast->next == NULL)
			break;
		if(fast->next->next == NULL)
			break;
		fast = fast->next->next;
		mid = mid->next;
	}
	

siblingKey = mid->key;

 /* Add all pairs after mid to the sibling node */

	list_node* nxt = mid->next;
	while(nxt != NULL)
	{
		sibling.insert(nxt->key,nxt->id.rid);
		nxt = nxt->next;
	}



 /* Free all pairs after the middle one in the current node */

	list_node* curr1 = mid->next;
	list_node* curr2 = curr1->next;
	while(curr1 != NULL)
	{
		delete curr1;
		count--;
		curr1 = curr2;
		if(curr1 == NULL)
			break;
		else curr2 = curr2->next;

	}


	
	list_node* ptr = list;
	for(int i = 1; i < count; i++)
	{
		ptr = ptr->next;
	}

	mid->next = NULL;

	return 0;
}

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
	int num = 0;
	list_node* curr = list;
	while(curr->key < searchKey)
	{
		if(curr->next == NULL)
		{
			return RC_NO_SUCH_RECORD;
		}

		curr = curr->next;
		num++;
	}

	eid = num;

	return 0; 

}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
	if(eid < 0)
		return RC_NO_SUCH_RECORD;

	list_node* curr = list;
	int num = 0;
	while(curr != NULL && num < eid)
	{
		curr = curr->next;
		num++;
	}

	if(curr == NULL)
		return RC_NO_SUCH_RECORD;

	key = curr->key;
	rid = curr->id.rid;

	return 0;

}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{
    //end_pid will be -1 if this is the last node in the tree
    return end_pid; }

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	end_pid = pid;
	return 0;
}


/*
 * Initialize the fields to appropriate zero values *
 */
BTNonLeafNode::BTNonLeafNode()
{
	count = 0;
	list = NULL;
	end_pid = 0;

}

/* Free all the allocated key/rid airs */
BTNonLeafNode::~BTNonLeafNode()
{
	list_node* p1 = list;
	list_node* p2;
	if(list != NULL)
	p2 = list->next;

	while(p1 != NULL)
	{
		delete p1;
		p1 = p2;
		if(p1 == NULL)
			break;
		else p2 = p2->next;
	}
	
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
    //check if the pid is valid
    if(pid < 0 || pid > pf.endPid()) return RC_INVALID_RID;

    memset(buffer,0,1024*sizeof(char));
    
    //read into the main memory buffer (1024 bytes)
    RC ret = pf.read(pid,buffer);
    
    //unable to read, return same error code
    if(ret < 0) return ret;
    
    char* ptr = buffer;
    
    memcpy(&count, reinterpret_cast<int*>(ptr), sizeof(count));
    //printf(count);
    
    ptr += sizeof(count);
    
    memcpy(&end_pid, reinterpret_cast<int*>(ptr), sizeof(end_pid));
    ptr += sizeof(end_pid);
    
    list_node* curr = NULL;
    RecordId RID;
    
    for(int i=0; i<count; i++)
    {
        if(i == 0)
        {
            //create root
            list = new list_node;
            memcpy(&RID, ptr, sizeof(RecordId));
            list->id.pid = RID.pid;
            ptr += sizeof(RecordId);
            memcpy(&list->key, reinterpret_cast<int*>(ptr), sizeof(int));
            ptr += sizeof(int);
            list->next = NULL;
            curr = list;
        }
        else
        {
            curr->next = new list_node;
            curr = curr->next;
            memcpy(&RID, ptr, sizeof(RecordId));
            curr->id.pid = RID.pid;
            ptr += sizeof(RecordId);
            memcpy(&curr->key, reinterpret_cast<int*>(ptr), sizeof(int));
            ptr += sizeof(int);
            curr->next = NULL;
        }
    }
    
    return 0;
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ 
    //check if the pid is valid
    if(pid < 0 || pid > pf.endPid()) return RC_INVALID_RID;
    
    char* ptr = buffer;
    
    //initialize buffer to be all zeros before we write in the linked list data
    memset(ptr, 0, PageFile::PAGE_SIZE * sizeof(char));
    
    memcpy(ptr, &count, sizeof(int));
    ptr += sizeof(int);
    memcpy(ptr, &end_pid, sizeof(PageId));
    ptr += sizeof(PageId);
    
    list_node* curr = list;
    RecordId RID;
    
    while(curr != NULL)
    {
        RID.pid = curr->id.pid;
        RID.sid = 0; //this doesn't matter it wont be read for nonLeafNodes
        memcpy(ptr, &curr->id.rid, sizeof(RecordId));
        ptr += sizeof(RecordId);
        memcpy(ptr, &curr->key, sizeof(int));
        ptr += sizeof(int);
        curr = curr->next;
    }
    
    //if we are unable to write the buffer, return same write error code
    RC ret;
    if( (ret = pf.write(pid, buffer)) <0) return ret;
    
    return 0;
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ return count; }


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid, bool swap)
{ 
	/* Check if node is full */
	if(count > N-2)
		return RC_NODE_FULL;

	list_node* temp = new list_node;
	temp->key = key;
	temp->id.pid = pid;
	temp->next = NULL;
	list_node* curr = list;

	if(list == NULL)
		{
			list = temp;
			list->next = NULL;
		}

	else {
		

		if(list->key > temp->key)
		{
			temp->next = curr;
			list = temp;
			if(swap){
			int tempPid = list->id.pid;
			list->id.pid = list->next->id.pid;
			list->next->id.pid = tempPid;
		}
		}

	else {
		while(curr)
		{
			if(!curr->next)
				{
					curr->next = temp;
					if(swap){
					int tempPid = end_pid;
					end_pid = curr->next->id.pid;
					curr->next->id.pid = tempPid;
					}
					break;
				}

			if(curr->next->key > temp->key)
			{
				temp->next = curr->next;
				curr->next = temp;
				if(swap){
					int tempPid = curr->next->id.pid;
					curr->next->id.pid = curr->next->next->id.pid;
					curr->next->next->id.pid = tempPid;
				}
				break;
			}

			curr = curr->next;

		}

	}
		
	}

	count++;

	return 0;

}


/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey, bool swap)
{ 

/* Check that sibling is empty */
	if(sibling.getKeyCount() > 0)
		return RC_NODE_FULL;


/* Insert the given pair into this node */

	list_node* temp = new list_node;
	temp->key = key;
	temp->id.pid = pid;
	temp->next = NULL;
	list_node* curr = list;

	if(list == NULL)
		{
			list = temp;
		}

	else {
		

		if(list->key > temp->key)
		{
			temp->next = curr;
			list = temp;
			if(swap){
			int tempPid = list->id.pid;
			list->id.pid = list->next->id.pid;
			list->next->id.pid = tempPid;
		}
		}

	else {
		while(curr)
		{
			if(!curr->next)
				{
					curr->next = temp;
					if(swap){
					int tempPid = end_pid;
					end_pid = curr->next->id.pid;
					curr->next->id.pid = tempPid;
					}
					break;
				}

			if(curr->next->key > temp->key)
			{
				temp->next = curr->next;
				curr->next = temp;
				break;
			}

			curr = curr->next;

		}

	}
		
	}

	count++;

/* Find the middle pair in this node */

	list_node* mid = list;
	list_node* fast = list;

	while(true)
	{
		if(fast == NULL)
			break;
		if(fast->next == NULL)
			break;
		if(fast->next->next == NULL)
			break;
		fast = fast->next->next;
		mid = mid->next;
	}
	

	midKey = mid->key;

 /* Add all pairs after mid to the sibling node */

	list_node* nxt = mid->next;
	while(nxt != NULL)
	{
		sibling.insert(nxt->key,nxt->id.pid,false);
		nxt = nxt->next;
	}


/* Set end pids for both siblings */

	sibling.setEndPid(end_pid);
	end_pid = mid->id.pid;
	

 /* Free all pairs after the middle one in the current node */

	list_node* curr1 = mid;
	list_node* curr2 = curr1->next;
	while(curr1 != NULL)
	{
		delete curr1;
		count--;
		curr1 = curr2;
		if(curr1 == NULL)
			break;
		else curr2 = curr2->next;

	}


	list_node* ptr = list;
	for(int i = 1; i < count; i++)
	{
		ptr = ptr->next;
	}

	ptr->next = NULL;


	return 0;

}



/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ 
	list_node* curr = list;
	while(curr->key < searchKey)
	{
		if(curr->next == NULL)
		{
			pid = end_pid;
			return 0;
		}

		curr = curr->next;
	}

	pid = curr->id.pid;

	return 0; 
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
	list = new list_node;
	list->key = key;
	list->id.pid = pid1;

	end_pid = pid2;
	count++;
	
	return 0; 

}

RC BTNonLeafNode::setEndPid(PageId pid)
{
	end_pid = pid;
    return 0;
}

