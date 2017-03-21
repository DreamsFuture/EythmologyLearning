
#include "treelib.h" 
#include "btree.h" 
#include "debug.h"

#define EXTENDER "variable_length_key_number"

char * extender = EXTENDER;
 
main(int argc,char **argv) 
{ 
	union Key k; 
	long count,term; 
	char number[200], buffer[BUFSIZ]; 
	int stat; 
	int start;
	char name[64];
	int tok;
	int i,j;
	BTREE * btree;
	int dblevel;
 
        printf("================================================\n");
        printf("      Program to delete keys from a BTree\n");
        printf("================================================\n");

        printf("Enter the name of the index.\n: ");
        if(argc > 1){
                strcpy(name,*(argv+1));
                printf("%s\n",name);
        }else{
                gets(name);
        }
	btree = OpenBtree(name, 0, VAR_LNG_KEY, 0, 0);

        printf("1. No debugging\n");
        printf("2. Debug level \n");
        printf("3. Trace level \n");
        printf("\nEnter a number to select an item.\n: ");
	gets(buffer);
        dblevel = (atoi(buffer));
        if( dblevel < 1 || dblevel > 2){
                fprintf(stderr, "Debugging Must be either 1, 2, or 3\n");
                exit(1);
        }
        if(dblevel == 3)
                debug_open( TRACE, "ungen.log");
        else
                if(dblevel == 2)
                        debug_open( DEBUG, "ungen.log");


        printf("\nEnter the number of keys to delete.\n: ");
        gets(buffer);
        count =  atol(buffer);

        printf("\nEnter the beginning number.\n: ");
        gets(buffer);
        start =  atoi(buffer);

	term = start+count;
	
	tok = (int)btree->type_of_key;
 
	sProf(__LINE__, 0);
	for(i = start; i < term; i++){ 
		switch(tok){
			case VAR_LNG_KEY:
	        		sprintf(number,"%ld",i); 
				sprintf(buffer,"%s%s",extender,number); 
				create_index_string( k.key,buffer);
			break;
			case LONG_KEY:
				k.long_key = (long)i;
				sprintf(number, "%ld", (long)i);
			break;
			case DOUBLE_KEY:
				k.double_key = (double)i;
				sprintf(number, "%f", (float)i);
			break;
			case FIX_LNG_KEY:
				for(j = 0; j < 200; j++){
					number[j] = 0;
				}
				sprintf(k.key,"%ld",i);
			break;
		}

		if((stat = DeleteKey(btree,&k)) == FAIL) 
			fprintf(stderr,"couldn't delete %s\n",buffer); 
                if(!(i%1000))
                        fprintf(stderr,".");

	} 
	eProf(__LINE__, 0);
	fprintf(stderr,"=========================\n"); 
	DisplayKey(btree,First(btree)); 
	DisplayNodeHeader(btree); 
	DisplayKey(btree,Last(btree)); 
	Close(btree); 
	exit(0); 
} 
