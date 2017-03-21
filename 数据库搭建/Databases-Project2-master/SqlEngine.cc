<<<<<<< HEAD
/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  BTreeIndex index;
  RecordId   rid;  // record cursor for table scanning

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }
  
  bool useTree = false;
  bool validWhere = true;
  int keyEQCond = -2;
  vector<SelCond> keyNECond;
  int keyLECond = -2;
  int keyGECond = -2;
  vector<SelCond> valueCond;
  
  if((rc = index.open(table+".idx", 'r') == 0))
  {
        
    for(unsigned i = 0; i < cond.size(); i++)
    {
        if(cond[i].attr == 1 && cond[i].comp != SelCond::NE)
            useTree = true;
        if(cond[i].attr == 1)
        {
            switch (cond[i].comp) {
                case SelCond::EQ:
                    if(keyEQCond == -2)
                        keyEQCond = atoi(cond[i].value);
                    else if(keyEQCond != atoi(cond[i].value))
                    {
                        //can't have a key be equal to two differant numebers
                        validWhere = false;
                        break;
                    }
                    for(unsigned k = 0; k < keyNECond.size(); k++)
                    {
                        if(atoi(keyNECond[k].value) == keyEQCond)
                        {
                            //can't have a key that is both equal and not equal to same number
                            validWhere = false;
                            break;
                        }
                    }
                    if(keyLECond != -2 && keyLECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyGECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::NE:
                    keyNECond.push_back(cond[i]);
                    if(keyEQCond == atoi(cond[i].value))
                    {
                        //can't have a key that is both equal and not equal to same number
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::LT:
                    if(keyLECond == -2)
                        keyLECond = atoi(cond[i].value)-1;
                    else if(atoi(cond[i].value) <= keyLECond)
                        keyLECond = atoi(cond[i].value)-1;
                    if(keyEQCond != -2 && keyLECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::GT:
                    if(keyGECond == -2)
                        keyGECond = atoi(cond[i].value)+1;
                    else if(atoi(cond[i].value) >= keyGECond)
                        keyGECond = atoi(cond[i].value)+1;
                    if(keyEQCond != -2 && keyGECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyLECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::LE:
                    if(keyLECond == -2)
                        keyLECond = atoi(cond[i].value);
                    else if(atoi(cond[i].value) < keyLECond)
                        keyLECond = atoi(cond[i].value);
                    if(keyEQCond != -2 && keyLECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::GE:
                    if(keyGECond == -2)
                        keyGECond = atoi(cond[i].value);
                    else if(atoi(cond[i].value) > keyGECond)
                        keyGECond = atoi(cond[i].value);
                    if(keyEQCond != -2 && keyGECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                default:
                    break;
            }
        }
        else
            valueCond.push_back(cond[i]);
        if(!validWhere)
            break;
    }
        
  }
    
  rid.pid = rid.sid = 0;
  count = 0;
  if(useTree && validWhere)
  {
      IndexCursor cursor;
      if(keyEQCond != -2)
      {
          index.locate(keyEQCond, cursor);
          index.readForward(cursor, key, rid);
          // read the tuple
          if ((rc = rf.read(rid, key, value)) < 0) {
              fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
              rf.close();
              index.close();
              return rc;
          }
          for(unsigned i=0; i<valueCond.size(); i++)
          {
              diff = strcmp(value.c_str(), valueCond[i].value);
              // skip the tuple if any condition is not met
              switch (valueCond[i].comp) {
                  case SelCond::EQ:
                      if (diff != 0) goto end_EQ;
                      break;
                  case SelCond::NE:
                      if (diff == 0) goto end_EQ;
                      break;
                  case SelCond::GT:
                      if (diff <= 0) goto end_EQ;
                      break;
                  case SelCond::LT:
                      if (diff >= 0) goto end_EQ;
                      break;
                  case SelCond::GE:
                      if (diff < 0) goto end_EQ;
                      break;
                  case SelCond::LE:
                      if (diff > 0) goto end_EQ;
                      break;
              }
          }
          
          // the condition is met for the tuple.
          // increase matching tuple counter
          count++;
          
          // print the tuple
          switch (attr) {
              case 1:  // SELECT key
                  fprintf(stdout, "%d\n", key);
                  break;
              case 2:  // SELECT value
                  fprintf(stdout, "%s\n", value.c_str());
                  break;
              case 3:  // SELECT *
                  fprintf(stdout, "%d '%s'\n", key, value.c_str());
                  break;
          }
          end_EQ:
          ;
      }
      else //if(keyGECond != -2) Dont think I need this, want to start from leftmost leaf node in the tree, and calling locate on -2 will do this
      {
          index.locate(keyGECond, cursor);
          while(index.readForward(cursor,key,rid) != RC_END_OF_TREE)
          {
              // read the tuple
              if ((rc = rf.read(rid, key, value)) < 0) {
                  fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
                  rf.close();
                  index.close();
                  return rc;
              }
              //check LT Condition on key
              if(keyLECond != -2 && key > keyLECond)
              {
                  break;
              }
              //check NE Conditions on key
              for(unsigned i=0; i < keyNECond.size(); i++)
              {
                  if(atoi(keyNECond[i].value) == key)
                      goto next_GE;
              }
              
              //check value attribute if necessary
              for(unsigned i=0; i<valueCond.size(); i++)
              {
                  diff = strcmp(value.c_str(), valueCond[i].value);
                  // skip the tuple if any condition is not met
                  switch (valueCond[i].comp) {
                      case SelCond::EQ:
                          if (diff != 0) goto next_GE;
                          break;
                      case SelCond::NE:
                          if (diff == 0) goto next_GE;
                          break;
                      case SelCond::GT:
                          if (diff <= 0) goto next_GE;
                          break;
                      case SelCond::LT:
                          if (diff >= 0) goto next_GE;
                          break;
                      case SelCond::GE:
                          if (diff < 0) goto next_GE;
                          break;
                      case SelCond::LE:
                          if (diff > 0) goto next_GE;
                          break;
                  }
              }
              
              // the condition is met for the tuple.
              // increase matching tuple counter
              count++;
              
              // print the tuple
              switch (attr) {
                  case 1:  // SELECT key
                      fprintf(stdout, "%d\n", key);
                      break;
                  case 2:  // SELECT value
                      fprintf(stdout, "%s\n", value.c_str());
                      break;
                  case 3:  // SELECT *
                      fprintf(stdout, "%d '%s'\n", key, value.c_str());
                      break;
              }
              
            next_GE:
              ;
          }
      }
      
      // print matching tuple count if "select count(*)"
      if (attr == 4) {
          fprintf(stdout, "%d\n", count);
      }
      rc = 0;
      rf.close();
      index.close();
  }
  else if(validWhere)
  {
    // scan the table file from the beginning
    while (rid < rf.endRid()) {
    // read the tuple
    if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
    }

    // check the conditions on the tuple
    for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
            case 1:
                diff = key - atoi(cond[i].value);
                break;
            case 2:
                diff = strcmp(value.c_str(), cond[i].value);
                break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
            case SelCond::EQ:
                if (diff != 0) goto next_tuple;
                break;
            case SelCond::NE:
                if (diff == 0) goto next_tuple;
                break;
            case SelCond::GT:
                if (diff <= 0) goto next_tuple;
                break;
            case SelCond::LT:
                if (diff >= 0) goto next_tuple;
                break;
            case SelCond::GE:
                if (diff < 0) goto next_tuple;
                break;
            case SelCond::LE:
                if (diff > 0) goto next_tuple;
                break;
        }
    }

    // the condition is met for the tuple. 
    // increase matching tuple counter
    count++;

    // print the tuple 
    switch (attr) {
    case 1:  // SELECT key
      fprintf(stdout, "%d\n", key);
      break;
    case 2:  // SELECT value
      fprintf(stdout, "%s\n", value.c_str());
      break;
    case 3:  // SELECT *
      fprintf(stdout, "%d '%s'\n", key, value.c_str());
      break;
    }

    // move to the next tuple
    next_tuple:
    ++rid;
    }

      // print matching tuple count if "select count(*)"
      if (attr == 4) {
          fprintf(stdout, "%d\n", count);
      }
      rc = 0;

      // close the table file and return
    exit_select:
      rf.close();
    }
    return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  /* your code here */
  int retval = 0;

  /* Open loadfile */
  ifstream fs;
  fs.open(loadfile.c_str());
  if(fs.is_open()){};
    /* Need to figure out what to return if fopen fails */

  /* Initialize RecordFile instance and open in write mode */
  string record_file_name = table + ".tbl";
  RecordFile record_file;
  retval = record_file.open(record_file_name,'w');
  if (retval < 0)
      return retval;
    
  BTreeIndex BTree;
  /* Create an index for this table when applicable */
  if(index)
  {
      string BTree_file_name = table + ".idx";
      retval = BTree.open(BTree_file_name, 'w');
      if(retval < 0)
          return retval;
  }

  
  /* Parse loadfile line-by-line and enter records into record file, and index if applicable */
    string next_line = "";
    while(getline(fs,next_line))
    {
      int key; string value;
      parseLoadLine(next_line,key,value);

      RecordId record_file_pos = record_file.endRid();
      record_file.append(key,value,record_file_pos);

      if(index)
      {
          retval = BTree.insert(key, record_file_pos);
          if(retval < 0)
              return retval;
      }
    }

    fs.close();
    record_file.close();
    if(index)
    {
        BTree.close();
    }
    return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
=======
=======
>>>>>>> FETCH_HEAD
/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  BTreeIndex index;
  RecordId   rid;  // record cursor for table scanning

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }
  
  bool useTree = false;
  bool validWhere = true;
  int keyEQCond = -2;
  vector<SelCond> keyNECond;
  int keyLECond = -2;
  int keyGECond = -2;
  vector<SelCond> valueCond;
  
  if((rc = index.open(table+".idx", 'r') == 0))
  {
        
    for(unsigned i = 0; i < cond.size(); i++)
    {
        if(cond[i].attr == 1 && cond[i].comp != SelCond::NE)
            useTree = true;
        if(cond[i].attr == 1)
        {
            switch (cond[i].comp) {
                case SelCond::EQ:
                    if(keyEQCond == -2)
                        keyEQCond = atoi(cond[i].value);
                    else if(keyEQCond != atoi(cond[i].value))
                    {
                        //can't have a key be equal to two differant numebers
                        validWhere = false;
                        break;
                    }
                    for(unsigned k = 0; k < keyNECond.size(); k++)
                    {
                        if(atoi(keyNECond[k].value) == keyEQCond)
                        {
                            //can't have a key that is both equal and not equal to same number
                            validWhere = false;
                            break;
                        }
                    }
                    if(keyLECond != -2 && keyLECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyGECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::NE:
                    keyNECond.push_back(cond[i]);
                    if(keyEQCond == atoi(cond[i].value))
                    {
                        //can't have a key that is both equal and not equal to same number
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::LT:
                    if(keyLECond == -2)
                        keyLECond = atoi(cond[i].value)-1;
                    else if(atoi(cond[i].value) <= keyLECond)
                        keyLECond = atoi(cond[i].value)-1;
                    if(keyEQCond != -2 && keyLECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::GT:
                    if(keyGECond == -2)
                        keyGECond = atoi(cond[i].value)+1;
                    else if(atoi(cond[i].value) >= keyGECond)
                        keyGECond = atoi(cond[i].value)+1;
                    if(keyEQCond != -2 && keyGECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyLECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::LE:
                    if(keyLECond == -2)
                        keyLECond = atoi(cond[i].value);
                    else if(atoi(cond[i].value) < keyLECond)
                        keyLECond = atoi(cond[i].value);
                    if(keyEQCond != -2 && keyLECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::GE:
                    if(keyGECond == -2)
                        keyGECond = atoi(cond[i].value);
                    else if(atoi(cond[i].value) > keyGECond)
                        keyGECond = atoi(cond[i].value);
                    if(keyEQCond != -2 && keyGECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                default:
                    break;
            }
        }
        else
            valueCond.push_back(cond[i]);
        if(!validWhere)
            break;
    }
        
  }
    
  rid.pid = rid.sid = 0;
  count = 0;
  if(useTree && validWhere)
  {
      IndexCursor cursor;
      if(keyEQCond != -2)
      {
          index.locate(keyEQCond, cursor);
          index.readForward(cursor, key, rid);
          // read the tuple
          if ((rc = rf.read(rid, key, value)) < 0) {
              fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
              rf.close();
              index.close();
              return rc;
          }
          for(unsigned i=0; i<valueCond.size(); i++)
          {
              diff = strcmp(value.c_str(), valueCond[i].value);
              // skip the tuple if any condition is not met
              switch (valueCond[i].comp) {
                  case SelCond::EQ:
                      if (diff != 0) goto end_EQ;
                      break;
                  case SelCond::NE:
                      if (diff == 0) goto end_EQ;
                      break;
                  case SelCond::GT:
                      if (diff <= 0) goto end_EQ;
                      break;
                  case SelCond::LT:
                      if (diff >= 0) goto end_EQ;
                      break;
                  case SelCond::GE:
                      if (diff < 0) goto end_EQ;
                      break;
                  case SelCond::LE:
                      if (diff > 0) goto end_EQ;
                      break;
              }
          }
          
          // the condition is met for the tuple.
          // increase matching tuple counter
          count++;
          
          // print the tuple
          switch (attr) {
              case 1:  // SELECT key
                  fprintf(stdout, "%d\n", key);
                  break;
              case 2:  // SELECT value
                  fprintf(stdout, "%s\n", value.c_str());
                  break;
              case 3:  // SELECT *
                  fprintf(stdout, "%d '%s'\n", key, value.c_str());
                  break;
          }
          end_EQ:
          ;
      }
      else //if(keyGECond != -2) Dont think I need this, want to start from leftmost leaf node in the tree, and calling locate on -2 will do this
      {
          index.locate(keyGECond, cursor);
          while(index.readForward(cursor,key,rid) != RC_END_OF_TREE)
          {
              // read the tuple
              if ((rc = rf.read(rid, key, value)) < 0) {
                  fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
                  rf.close();
                  index.close();
                  return rc;
              }
              //check LT Condition on key
              if(keyLECond != -2 && key > keyLECond)
              {
                  break;
              }
              //check NE Conditions on key
              for(unsigned i=0; i < keyNECond.size(); i++)
              {
                  if(atoi(keyNECond[i].value) == key)
                      goto next_GE;
              }
              
              //check value attribute if necessary
              for(unsigned i=0; i<valueCond.size(); i++)
              {
                  diff = strcmp(value.c_str(), valueCond[i].value);
                  // skip the tuple if any condition is not met
                  switch (valueCond[i].comp) {
                      case SelCond::EQ:
                          if (diff != 0) goto next_GE;
                          break;
                      case SelCond::NE:
                          if (diff == 0) goto next_GE;
                          break;
                      case SelCond::GT:
                          if (diff <= 0) goto next_GE;
                          break;
                      case SelCond::LT:
                          if (diff >= 0) goto next_GE;
                          break;
                      case SelCond::GE:
                          if (diff < 0) goto next_GE;
                          break;
                      case SelCond::LE:
                          if (diff > 0) goto next_GE;
                          break;
                  }
              }
              
              // the condition is met for the tuple.
              // increase matching tuple counter
              count++;
              
              // print the tuple
              switch (attr) {
                  case 1:  // SELECT key
                      fprintf(stdout, "%d\n", key);
                      break;
                  case 2:  // SELECT value
                      fprintf(stdout, "%s\n", value.c_str());
                      break;
                  case 3:  // SELECT *
                      fprintf(stdout, "%d '%s'\n", key, value.c_str());
                      break;
              }
              
            next_GE:
              ;
          }
      }
      
      // print matching tuple count if "select count(*)"
      if (attr == 4) {
          fprintf(stdout, "%d\n", count);
      }
      rc = 0;
      rf.close();
      index.close();
      rc = 0;
  }
  else if(validWhere)
  {
    // scan the table file from the beginning
    while (rid < rf.endRid()) {
    // read the tuple
    if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
    }

    // check the conditions on the tuple
    for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
            case 1:
                diff = key - atoi(cond[i].value);
                break;
            case 2:
                diff = strcmp(value.c_str(), cond[i].value);
                break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
            case SelCond::EQ:
                if (diff != 0) goto next_tuple;
                break;
            case SelCond::NE:
                if (diff == 0) goto next_tuple;
                break;
            case SelCond::GT:
                if (diff <= 0) goto next_tuple;
                break;
            case SelCond::LT:
                if (diff >= 0) goto next_tuple;
                break;
            case SelCond::GE:
                if (diff < 0) goto next_tuple;
                break;
            case SelCond::LE:
                if (diff > 0) goto next_tuple;
                break;
        }
    }

    // the condition is met for the tuple. 
    // increase matching tuple counter
    count++;

    // print the tuple 
    switch (attr) {
    case 1:  // SELECT key
      fprintf(stdout, "%d\n", key);
      break;
    case 2:  // SELECT value
      fprintf(stdout, "%s\n", value.c_str());
      break;
    case 3:  // SELECT *
      fprintf(stdout, "%d '%s'\n", key, value.c_str());
      break;
    }

    // move to the next tuple
    next_tuple:
    ++rid;
    }

      // print matching tuple count if "select count(*)"
      if (attr == 4) {
          fprintf(stdout, "%d\n", count);
      }
      rc = 0;

      // close the table file and return
    exit_select:
      rf.close();
    }
    return rc;
  }

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  /* your code here */
  int retval = 0;

  /* Open loadfile */
  ifstream fs;
  fs.open(loadfile.c_str());
  if(fs.is_open()){};
    /* Need to figure out what to return if fopen fails */

  /* Initialize RecordFile instance and open in write mode */
  string record_file_name = table + ".tbl";
  RecordFile record_file;
  retval = record_file.open(record_file_name,'w');
  if (retval < 0)
      return retval;
    
  BTreeIndex BTree;
  /* Create an index for this table when applicable */
  if(index)
  {
      string BTree_file_name = table + ".idx";
      retval = BTree.open(BTree_file_name, 'w');
      if(retval < 0)
          return retval;
  }

  
  /* Parse loadfile line-by-line and enter records into record file, and index if applicable */
    string next_line = "";
    while(getline(fs,next_line))
    {
      int key; string value;
      parseLoadLine(next_line,key,value);

      RecordId record_file_pos = record_file.endRid();
      record_file.append(key,value,record_file_pos);

      if(index)
      {
          retval = BTree.insert(key, record_file_pos);
          if(retval < 0)
              return retval;
      }
    }

    fs.close();
    record_file.close();
    if(index)
    {
        BTree.close();
    }
    return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
=======
/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  BTreeIndex index;
  RecordId   rid;  // record cursor for table scanning

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }
  
  bool useTree = false;
  bool validWhere = true;
  int keyEQCond = -2;
  vector<SelCond> keyNECond;
  int keyLECond = -2;
  int keyGECond = -2;
  vector<SelCond> valueCond;
  
  if((rc = index.open(table+".idx", 'r') == 0))
  {
        
    for(unsigned i = 0; i < cond.size(); i++)
    {
        if(cond[i].attr == 1 && cond[i].comp != SelCond::NE)
            useTree = true;
        if(cond[i].attr == 1)
        {
            switch (cond[i].comp) {
                case SelCond::EQ:
                    if(keyEQCond == -2)
                        keyEQCond = atoi(cond[i].value);
                    else if(keyEQCond != atoi(cond[i].value))
                    {
                        //can't have a key be equal to two differant numebers
                        validWhere = false;
                        break;
                    }
                    for(unsigned k = 0; k < keyNECond.size(); k++)
                    {
                        if(atoi(keyNECond[k].value) == keyEQCond)
                        {
                            //can't have a key that is both equal and not equal to same number
                            validWhere = false;
                            break;
                        }
                    }
                    if(keyLECond != -2 && keyLECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyGECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::NE:
                    keyNECond.push_back(cond[i]);
                    if(keyEQCond == atoi(cond[i].value))
                    {
                        //can't have a key that is both equal and not equal to same number
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::LT:
                    if(keyLECond == -2)
                        keyLECond = atoi(cond[i].value)-1;
                    else if(atoi(cond[i].value) <= keyLECond)
                        keyLECond = atoi(cond[i].value)-1;
                    if(keyEQCond != -2 && keyLECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::GT:
                    if(keyGECond == -2)
                        keyGECond = atoi(cond[i].value)+1;
                    else if(atoi(cond[i].value) >= keyGECond)
                        keyGECond = atoi(cond[i].value)+1;
                    if(keyEQCond != -2 && keyGECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyLECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::LE:
                    if(keyLECond == -2)
                        keyLECond = atoi(cond[i].value);
                    else if(atoi(cond[i].value) < keyLECond)
                        keyLECond = atoi(cond[i].value);
                    if(keyEQCond != -2 && keyLECond < keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                case SelCond::GE:
                    if(keyGECond == -2)
                        keyGECond = atoi(cond[i].value);
                    else if(atoi(cond[i].value) > keyGECond)
                        keyGECond = atoi(cond[i].value);
                    if(keyEQCond != -2 && keyGECond > keyEQCond)
                    {
                        validWhere = false;
                        break;
                    }
                    if(keyGECond != -2 && keyLECond < keyGECond)
                    {
                        validWhere = false;
                        break;
                    }
                    break;
                default:
                    break;
            }
        }
        else
            valueCond.push_back(cond[i]);
        if(!validWhere)
            break;
    }
        
  }
    
  rid.pid = rid.sid = 0;
  count = 0;
  if(useTree && validWhere)
  {
      IndexCursor cursor;
      if(keyEQCond != -2)
      {
          index.locate(keyEQCond, cursor);
          index.readForward(cursor, key, rid);
          // read the tuple
          if ((rc = rf.read(rid, key, value)) < 0) {
              fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
              rf.close();
              index.close();
              return rc;
          }
          for(unsigned i=0; i<valueCond.size(); i++)
          {
              diff = strcmp(value.c_str(), valueCond[i].value);
              // skip the tuple if any condition is not met
              switch (valueCond[i].comp) {
                  case SelCond::EQ:
                      if (diff != 0) goto end_EQ;
                      break;
                  case SelCond::NE:
                      if (diff == 0) goto end_EQ;
                      break;
                  case SelCond::GT:
                      if (diff <= 0) goto end_EQ;
                      break;
                  case SelCond::LT:
                      if (diff >= 0) goto end_EQ;
                      break;
                  case SelCond::GE:
                      if (diff < 0) goto end_EQ;
                      break;
                  case SelCond::LE:
                      if (diff > 0) goto end_EQ;
                      break;
              }
          }
          
          // the condition is met for the tuple.
          // increase matching tuple counter
          count++;
          
          // print the tuple
          switch (attr) {
              case 1:  // SELECT key
                  fprintf(stdout, "%d\n", key);
                  break;
              case 2:  // SELECT value
                  fprintf(stdout, "%s\n", value.c_str());
                  break;
              case 3:  // SELECT *
                  fprintf(stdout, "%d '%s'\n", key, value.c_str());
                  break;
          }
          end_EQ:
          ;
      }
      else //if(keyGECond != -2) Dont think I need this, want to start from leftmost leaf node in the tree, and calling locate on -2 will do this
      {
          index.locate(keyGECond, cursor);
          while(index.readForward(cursor,key,rid) != RC_END_OF_TREE)
          {
              // read the tuple
              if ((rc = rf.read(rid, key, value)) < 0) {
                  fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
                  rf.close();
                  index.close();
                  return rc;
              }
              //check LT Condition on key
              if(keyLECond != -2 && key > keyLECond)
              {
                  break;
              }
              //check NE Conditions on key
              for(unsigned i=0; i < keyNECond.size(); i++)
              {
                  if(atoi(keyNECond[i].value) == key)
                      goto next_GE;
              }
              
              //check value attribute if necessary
              for(unsigned i=0; i<valueCond.size(); i++)
              {
                  diff = strcmp(value.c_str(), valueCond[i].value);
                  // skip the tuple if any condition is not met
                  switch (valueCond[i].comp) {
                      case SelCond::EQ:
                          if (diff != 0) goto next_GE;
                          break;
                      case SelCond::NE:
                          if (diff == 0) goto next_GE;
                          break;
                      case SelCond::GT:
                          if (diff <= 0) goto next_GE;
                          break;
                      case SelCond::LT:
                          if (diff >= 0) goto next_GE;
                          break;
                      case SelCond::GE:
                          if (diff < 0) goto next_GE;
                          break;
                      case SelCond::LE:
                          if (diff > 0) goto next_GE;
                          break;
                  }
              }
              
              // the condition is met for the tuple.
              // increase matching tuple counter
              count++;
              
              // print the tuple
              switch (attr) {
                  case 1:  // SELECT key
                      fprintf(stdout, "%d\n", key);
                      break;
                  case 2:  // SELECT value
                      fprintf(stdout, "%s\n", value.c_str());
                      break;
                  case 3:  // SELECT *
                      fprintf(stdout, "%d '%s'\n", key, value.c_str());
                      break;
              }
              
            next_GE:
              ;
          }
      }
      
      // print matching tuple count if "select count(*)"
      if (attr == 4) {
          fprintf(stdout, "%d\n", count);
      }
      rc = 0;
      rf.close();
      index.close();
      rc = 0;
  }
  else if(validWhere)
  {
    // scan the table file from the beginning
    while (rid < rf.endRid()) {
    // read the tuple
    if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
    }

    // check the conditions on the tuple
    for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
            case 1:
                diff = key - atoi(cond[i].value);
                break;
            case 2:
                diff = strcmp(value.c_str(), cond[i].value);
                break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
            case SelCond::EQ:
                if (diff != 0) goto next_tuple;
                break;
            case SelCond::NE:
                if (diff == 0) goto next_tuple;
                break;
            case SelCond::GT:
                if (diff <= 0) goto next_tuple;
                break;
            case SelCond::LT:
                if (diff >= 0) goto next_tuple;
                break;
            case SelCond::GE:
                if (diff < 0) goto next_tuple;
                break;
            case SelCond::LE:
                if (diff > 0) goto next_tuple;
                break;
        }
    }

    // the condition is met for the tuple. 
    // increase matching tuple counter
    count++;

    // print the tuple 
    switch (attr) {
    case 1:  // SELECT key
      fprintf(stdout, "%d\n", key);
      break;
    case 2:  // SELECT value
      fprintf(stdout, "%s\n", value.c_str());
      break;
    case 3:  // SELECT *
      fprintf(stdout, "%d '%s'\n", key, value.c_str());
      break;
    }

    // move to the next tuple
    next_tuple:
    ++rid;
    }

      // print matching tuple count if "select count(*)"
      if (attr == 4) {
          fprintf(stdout, "%d\n", count);
      }
      rc = 0;

      // close the table file and return
    exit_select:
      rf.close();
    }
    return rc;
  }

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  /* your code here */
  int retval = 0;

  /* Open loadfile */
  ifstream fs;
  fs.open(loadfile.c_str());
  if(fs.is_open()){};
    /* Need to figure out what to return if fopen fails */

  /* Initialize RecordFile instance and open in write mode */
  string record_file_name = table + ".tbl";
  RecordFile record_file;
  retval = record_file.open(record_file_name,'w');
  if (retval < 0)
      return retval;
    
  BTreeIndex BTree;
  /* Create an index for this table when applicable */
  if(index)
  {
      string BTree_file_name = table + ".idx";
      retval = BTree.open(BTree_file_name, 'w');
      if(retval < 0)
          return retval;
  }

  
  /* Parse loadfile line-by-line and enter records into record file, and index if applicable */
    string next_line = "";
    while(getline(fs,next_line))
    {
      int key; string value;
      parseLoadLine(next_line,key,value);

      RecordId record_file_pos = record_file.endRid();
      record_file.append(key,value,record_file_pos);

      if(index)
      {
          retval = BTree.insert(key, record_file_pos);
          if(retval < 0)
              return retval;
      }
    }

    fs.close();
    record_file.close();
    if(index)
    {
        BTree.close();
    }
    return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
>>>>>>> 1bfe994ba07d69c624c83659e395570ca58d713a
