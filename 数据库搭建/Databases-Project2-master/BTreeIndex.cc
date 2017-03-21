/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"

using namespace std;

#include <iostream>
/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
    currPid = -1;
    currHeight = 0;
    treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
    RC ret = pf.open(indexname,mode);
    if( ret < 0)
        return ret;
    char buffer[1024];
    char* ptr = buffer;
    pf.read(pf.endPid()-1, buffer); 

    memcpy(&rootPid, reinterpret_cast<int*>(ptr), sizeof(int));  // Read in rootPid that will be stored in last pageId

    ptr += sizeof(int);
    memcpy(&treeHeight,reinterpret_cast<int*>(ptr), sizeof(int));  // Read in treeHeight that will be stored in last pageId

    currPid = rootPid;  // Set currPid equal to rootPid

    return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
    char buffer[1024];
    memset(buffer,0,1024*sizeof(char));
    char * ptr = buffer;

    memcpy(ptr,reinterpret_cast<char*>(&rootPid),sizeof(int));   // Save rootPid to last page

    ptr += sizeof(int);

    memcpy(ptr,reinterpret_cast<char*>(&treeHeight),sizeof(int));  // Save treeHeight to last page

    pf.write(pf.endPid(),buffer);
    pf.close();
    return 0;
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
    //WHEN CHANGE rootPid, CHANGE currPid to be equal

    if(treeHeight == 0) /* The tree is empty */
        {
            BTLeafNode root;
            int ret = root.insert(key, rid);

            if(ret < 0) return ret;
            treeHeight++;

            if(rootPid == -1){
            rootPid = pf.endPid();
            currPid = rootPid;
            }

            ret = root.write(rootPid,pf);
            if(ret < 0) return ret;
        }


    else if(treeHeight == 1) /* Three tree has only a root */
    {
            BTLeafNode root;
            root.read(rootPid,pf);

            int ret = root.insert(key,rid);
            if(ret == 0)
            {
                root.write(rootPid,pf);
                return 0;
            }


            if(ret == RC_NODE_FULL)
            {
                 BTNonLeafNode newRoot;
                 BTLeafNode sibling;
                 int sibKey;

                 root.insertAndSplit(key,rid,sibling,sibKey);

                 PageId sibPid = pf.endPid();
                 sibling.write(sibPid,pf);
                 root.setNextNodePtr(sibPid);

                 root.write(rootPid,pf);
                 sibling.write(sibPid,pf);

                 newRoot.insert(sibKey,rootPid,false);
                 newRoot.setEndPid(sibPid);

                 PageId newRootPid = pf.endPid();
                 newRoot.write(newRootPid,pf);

                 treeHeight++;
                 rootPid = newRootPid;
                 currPid = newRootPid;
            }
    }


    else { /* The tree has more than one level */ 

    BTLeafNode root;
    root.read(rootPid,pf);

    std::vector<PageId> path;
    findInsertLeaf(path,key);

    BTLeafNode insertLeaf;
    insertLeaf.read(*(path.end()-1),pf);
    int ret = insertLeaf.insert(key,rid);

    if(ret == 0){
        insertLeaf.write(*(path.end()-1),pf);
        return 0;
    } 

    if(ret == RC_NODE_FULL){

        int sibKey;
        PageId newPid;
        BTLeafNode sibling;
        insertLeaf.insertAndSplit(key,rid,sibling,sibKey);

        newPid = pf.endPid();
        sibling.write(newPid,pf);
        insertLeaf.setNextNodePtr(pf.endPid()-1);
        path.pop_back();

        BTNonLeafNode newBorn;
        
        PageId parentPid;
        while(path.size() != 0){
            BTNonLeafNode parent;
            
            parent.read(*(path.end()-1),pf);

            int retval = parent.insert(sibKey,newPid,true);

            if(retval == 0){
                parent.write(*(path.end()-1),pf);
                return 0;
            } 

            parent.insertAndSplit(sibKey,newPid,newBorn,sibKey,true);

            parent.write(*(path.end()-1),pf);
            newPid = pf.endPid();
            newBorn.write(newPid,pf);

            parentPid = *(path.end()-1);
            path.pop_back();


        }   

        BTNonLeafNode newRoot;
        newRoot.insert(sibKey,parentPid,false);
        newRoot.setEndPid(newPid);

        PageId newrootPid = pf.endPid();
        newRoot.write(newrootPid,pf);

        treeHeight++;
        rootPid = newrootPid;
        currPid = newrootPid;

    }



    }   

    return 0;
}

/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
    RC ret;
    if(cursor.pid < 0 || cursor.eid < 0)
    {
        //reset recursion variables
        currHeight = 0;
        currPid = rootPid;
        return RC_INVALID_CURSOR;
    }
    if(currHeight < treeHeight)
    {
        BTNonLeafNode currNodeNonLeaf;
        ret = currNodeNonLeaf.read(currPid, pf);
        if(ret < 0)
        {
            //reset recursion variables
            currHeight = 0;
            currPid = rootPid;
            return ret;
        }
        ret = currNodeNonLeaf.locateChildPtr(searchKey, currPid);
        if(ret < 0)
        {
            //reset recursion variables
            currHeight = 0;
            currPid = rootPid;
            return ret;
        }
        currHeight++;
        return BTreeIndex::locate(searchKey, cursor);
    }
    else //we are at the leaf node
    {
        BTLeafNode currNodeLeaf;
        ret = currNodeLeaf.read(currPid, pf);
        if(ret < 0)
        {
            //reset recursion variables
            currHeight = 0;
            currPid = rootPid;
            return ret;
        }
        int eid;
        ret = currNodeLeaf.locate(searchKey,eid);
        if(ret < 0)
        {
            //reset recursion variables
            currHeight = 0;
            currPid = rootPid;
            return ret;
        }
        else
        {
            cursor.pid = currPid;
            cursor.eid = eid;
            //reset recursion variables
            currHeight = 0;
            currPid = rootPid;
            return 0;
        }
    }
    
    
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
    BTLeafNode node;
    RC ret;
    ret = node.read(cursor.pid, pf);
    if(ret < 0) return ret;
    ret = node.readEntry(cursor.eid, key, rid);
    if(ret < 0) return ret;
    if(cursor.eid + 1 >= node.getKeyCount())
    {
        cursor.pid = node.getNextNodePtr();
        //return the return code for reaching the end of the tree if this was last node
        if(cursor.pid == -1)
        {
            return RC_END_OF_TREE;
        }
        cursor.eid = 0;
    }
    else
    {
        cursor.eid = cursor.eid + 1;
    }
    return 0;
}

RC BTreeIndex::findInsertLeaf(std::vector<PageId>& path, int searchKey)
{
    BTNonLeafNode curr;
    curr.read(rootPid,pf);
    path.push_back(rootPid);
    PageId next;
    curr.locateChildPtr(searchKey,next);
    int tempHeight = 1;

    while(tempHeight < treeHeight - 1)
    {
        curr.read(next,pf);
        path.push_back(next);
        curr.locateChildPtr(searchKey,next);
        tempHeight++;
    }

    path.push_back(next);
    return 0;

}
