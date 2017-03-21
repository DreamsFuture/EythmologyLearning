/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "Bruinbase.h"
#include "SqlEngine.h"

//Modified
#include <iostream>
#include "BTreeNode.h"
#include "PageFile.h"
#include "BTreeIndex.h"
 using namespace std;


int main()
{
  // run the SQL engine taking user commands from standard input (console).
  SqlEngine::run(stdin);


//BTLeafNode a,b;

/* TEST : BTNonLeafNode::Insert with swap; 

    // Insert at end
    a.setEndPid(6);
    a.insert(0,1,false);
    a.insert(1,2,false);
    a.insert(2,3,false);
    a.insert(3,4,false);

    int mid;

    a.insertAndSplit(4,5,b,mid,false);


    list_node* curr = b.list;
    while(curr != NULL)
    {
        cout << curr->key << endl;
        curr = curr->next;
    }

    cout << mid << endl;
    cout << a.end_pid << endl << b.end_pid << endl; */



 //TEST : BTLeafNode::InsertAndSplit 
 /*

    RecordId rid;
    a.insert(0,rid);
    a.insert(1,rid);
    a.insert(2,rid);
    a.insert(3,rid);

    list_node* curr = a.list;
    while(curr != NULL)
    {
        cout << curr->key << endl;
        curr = curr->next;
    }
    int mid;

    a.insertAndSplit(4,rid,b,mid);


    while(curr != NULL)
    {
        cout << curr->key << endl;
        curr = curr->next;
    }

    cout << mid << endl; */

 /*TEST : BTreeIndex::insert 

    BTreeIndex index;
    index.open("a",'w');
    //cout << index.rootPid << endl;
    //cout << index.treeHeight << endl;
    RecordId rid;
    index.insert(1,rid);
    index.insert(2,rid);
    index.insert(3,rid);
    index.insert(4,rid);
    index.insert(5,rid);
    index.insert(6,rid);
    index.insert(7,rid);

    BTLeafNode root;
    cout << index.rootPid << endl;
    root.read(3,index.pf);

    index.close();  

    list_node* curr = root.list;
    while(curr != NULL)
    {
        cout << curr->key << "....."  << curr->id.pid <<endl;
        curr = curr->next;
    }

    cout << root.end_pid << endl; */

/* TEST : BTreeIndex::locatePtr */

/* TEST : BTreeIndes::readForward */

}
