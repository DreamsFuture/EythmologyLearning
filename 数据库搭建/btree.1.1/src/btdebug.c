#include <stdio.h>
#include "treelib.h"
#include "btree.h"
#include "debug.h"

long atol();
void pad(char *, int);

void GetTreeInfo(BTREE *tree);

main(int argc,char **argv)
{
	char 	indxbuff[40];
	char 	buff1[65];
	char 	buff2[65];
	int 	dups,reply,quit,index_exists,tfd,tok,lng,stat,print_key;
	struct 	KeyGroup 	k,*kp;
	union 	Key 		bk;
	long	update_val;
	char 	file[15];
	char 	command[95];
	struct 	Current_Key;
	char 	buffer[BUFSIZ];
	long 	current_node;
        BTREE * btree;
        int dblevel;


	fprintf(stderr,"==========================================================\n");
	fprintf(stderr,"        Test program to exercise the B+Tree\n");
	fprintf(stderr,"Copr. 1991 Larry A. Walker & Co. Sterling, Va 22170\n");
	fprintf(stderr,"==========================================================\n");

	/**/
	/* Get the name of the index*/
	/**/
	if(argc == 2){
		strcpy(indxbuff,*(argv+1));
	}else{
		fprintf(stderr,"\n\nWhat is the name of the index\n: ");
		gets(indxbuff);
	}
	index_exists = ( (tfd=open(indxbuff,O_RDONLY)) == -1 )?0:1;
	if(index_exists){
		close(tfd);
		dups = 0;
		lng = 0;
		tok = UNDEF_KEY;
	}else{
		/**/
		/* Duplicates required needed for opening new index*/
		/**/
		fprintf(stderr,"1.  Duplicates allowed.\n");
		fprintf(stderr,"2.  No duplicates allowed.\n");
		fprintf(stderr,"\nEnter a number to select an item.\n: ");
		gets(buff1);
		dups=atoi(buff1);
		if( (dups < 1) || (dups > 2) ){
			fprintf(stderr,"Must be either 1 or 2\n");
			exit(1);
		}
		dups=(dups == 2)?0:1;
		/**/
		/* Type of key required for opening new index*/
		/**/
		fprintf(stderr,"Please enter the type of key to use.\n");
		fprintf(stderr,"%d.  Variable length keys\n",VAR_LNG_KEY);
		fprintf(stderr,"%d.  Fixed length keys\n",FIX_LNG_KEY);
		fprintf(stderr,"%d.  Long keys\n",LONG_KEY);
		fprintf(stderr,"%d.  Double keys\n",DOUBLE_KEY);
		fprintf(stderr,"\nEnter a number to select an item.\n: ");
		gets(buff1);
		tok=atoi(buff1);
		switch(tok){
			case VAR_LNG_KEY:
				lng = -1;
				break;
			case FIX_LNG_KEY:
				fprintf(stderr,"Please enter the key length.\n: ");
				gets(buff1);
				lng = atoi(buff1);
				break;
			case LONG_KEY:
				lng = sizeof(long);
				break;
			case DOUBLE_KEY:
				lng = sizeof(double);
				break;
			default:
				fprintf(stderr,"Invalid response.\n");
				exit(1);
				break;
		}
	}
	/* index_name, create, type of key, */
	/* duplicates, fixed length key size*/
	
	btree = OpenBtree( indxbuff, (index_exists)?0:CREATE, tok, dups, lng);
	if(btree == NULL){
		fprintf(stderr, "Can't open index %s\n", indxbuff);
		exit(1);
	}
	/* If you already know the index exists, you can open it*/
	/* with default parameters as "Btree tree(indxbuff)"*/

	/* test the install routine*/
	
	if(tok == UNDEF_KEY){
		tok = (int)btree->type_of_key;
		GetTreeInfo(btree);
	}
        printf("1. No debugging\n");
        printf("2. Debug level \n");
        printf("3. Trace level \n");
        printf("\nEnter a number to select an item.\n: ");
        gets(buff2);
        dblevel = (atoi(buff2));
        if( dblevel < 1 || dblevel > 3){
                fprintf(stderr, "Debugging Must be either 1, 2, or 3\n");
                exit(1);
        }
        if(dblevel == 3)
                debug_open( TRACE, "btdebug.log");
        else
                if(dblevel == 2)
                        debug_open( DEBUG, "btdebug.log");

	quit=0;
	while(!quit) {
		print_key = 0;
	
		fprintf(stderr,"\n===========================================\n");
		fprintf(stderr,"|Pick an operation to perform on the index|\n");
		fprintf(stderr,"|f.  Find a key   i.  Insert a key        |\n");
		fprintf(stderr,"|d.  Delete a key n.  Next key            |\n");
		fprintf(stderr,"|p.  Prior key    t.  First key           |\n");
		fprintf(stderr,"|l.  Last key     c.  Current key         |\n");
		fprintf(stderr,"|x.  Exit/Quit    u.  Update              |\n");
		fprintf(stderr,"|m.  Dump         e.  Examine             |\n");
		fprintf(stderr,"|g.  Get Tree Info                        |\n");
		fprintf(stderr, "===========================================\n");
		fprintf(stderr, ": ");
		gets(buff2);
		reply = buff2[0];
		print_key = 0;
		switch(reply) {
			case 'g':
			case 'G':
				GetTreeInfo(btree);
				break;
			case 'f':
			case 'F':
				fprintf(stderr,"Check index for what key?\n: ");
				gets(buff2);
				fprintf(stderr,"finding ->%s<-\n",buff2);
				switch(tok){
					case VAR_LNG_KEY:
						create_index_string(bk.key,buff2);
						break;
					case FIX_LNG_KEY:
						if(strlen(buff2) < lng )
							pad(buff2,lng);
						memcpy(bk.key,buff2,lng);
						break;
					case LONG_KEY:
						bk.long_key = atol(buff2);
						break;
					case DOUBLE_KEY:
						bk.double_key = atof(buff2);
						break;
				}
				if(Locate(btree,&bk) == FAIL) {
					fprintf(stderr,"not in index\n");
					break;
				} else {
					if( (kp = CurrentKey(btree)) == NULL) {
						fprintf(stderr,"No current key\n");
						break;
					}
				}
				print_key = 1;
				break;
	
			case 'd':
			case 'D':
				fprintf(stderr,"What key to delete\n: ");
				gets(buff2);
				switch(tok){
					case VAR_LNG_KEY:
						create_index_string(bk.key,buff2);
						break;
					case FIX_LNG_KEY:
						if(strlen(buff2) < lng )
							pad(buff2,lng);
						memcpy(bk.key,buff2,lng);
						break;
					case LONG_KEY:
						bk.long_key = atol(buff2);
						break;
					case DOUBLE_KEY:
						bk.double_key = atof(buff2);
						break;
				}
				if( (stat = DeleteKey(btree,&bk)) == FAIL) {
					fprintf(stderr,"Key not deleted\n");
				}
				else fprintf(stderr,"Successful delete\n");
				break;
		
			case 'i':
			case 'I':
				fprintf(stderr,"What key to add\n: ");
				gets(buff1);
				switch(tok){
					case VAR_LNG_KEY:
						create_index_string(k.k.key,buff1);
						break;
					case FIX_LNG_KEY:
						if(strlen(buff1) < lng )
							pad(buff1,lng);
						memcpy(k.k.key,buff1,lng);
						break;
					case LONG_KEY:
						k.k.long_key = atol(buff1);
						break;
					case DOUBLE_KEY:
						k.k.double_key = atof(buff1);
						break;
				}
				fprintf(stderr,"What data\n: ");
				gets(buff2);
				k.KeyData.address = atol(buff2);
				if( (stat = InsertKey(btree,&k) ) == FAIL){
					fprintf(stderr,"Key add failed\n");
				}
				break;
		
			case 'n':
			case 'N':
	
				kp = Increment(btree);
				if(kp == NULL){
					fprintf(stderr,"next key not found\n");
					break;
				}
				print_key = 1;
				break;
		
			case 'p':
			case 'P':
	
				kp = Decrement(btree);
				if(kp == NULL){
					fprintf(stderr,"previous key not found\n");
					break;
				}
				print_key = 1;
				break;
	
			case 't':
			case 'T':
	
				kp=First(btree);
				if(kp == NULL){
					fprintf(stderr,"first key not found\n");
					break;
				}
				print_key = 1;
				break;
		
			case 'l':
			case 'L':
	
				kp=Last(btree);
				if(kp == NULL){
					fprintf(stderr,"last key not found\n");
					break;
				}
				print_key = 1;
				break;
		
			case 'c':
			case 'C':
	
				if((kp=CurrentKey(btree)) == NULL) {
					fprintf(stderr,"No current key\n");
					break;
				}
				print_key = 1;
				break;
	
			case 'x':
			case 'X':
			case 'q':
			case 'Q':
				quit=1;
				break;
			case 'U':
			case 'u':
				fprintf(stderr,"What data\n: ");
				gets(buff2);
				update_val = atol(buff2);
				if(!Update(btree,&update_val))
					fprintf(stderr,"Failed\n");
				print_key = 1;
				break;
			case 'M':
			case 'm':
				fprintf(stderr, "Output to a file (dump.out)?\n");
				gets(buff2);
				if( buff2[0] == 'y' || buff2[0] == 'Y'){
					int local_fd;
					local_fd = 
					  open("dump.out", O_WRONLY|O_CREAT|O_TRUNC, 0666);
					if(local_fd == -1){
						fprintf(stderr, "Error opening\
 file dump.out\n");
						Close(btree);
						exit(0);
					}
					close(1);
					close(2);
					dup(local_fd);
					dup(local_fd);
				}
				DumpBtree(btree);
				Close(btree);
				exit(0);
				break;
			case 'E':
			case 'e':
/*
				get_info(btree, (struct bfh_data *)&b);
*/

				current_node = btree->rootnode;
				do{
					printf("NODE: %ld\n", current_node);
					GetPrintNode(btree,current_node);
					printf("\nEnter new node ID\n: ");
					gets(buffer);
					if( (buffer[0] > '9')  ||  (buffer[0] < '1') )
						break;
					current_node = atol(buffer);
				}while(1);
				break;
			}
		if(print_key){
			switch(tok){
				case VAR_LNG_KEY:
					create_c_string(buff1,kp->k.key);
					fprintf(stderr,"found ->%s<-\ndata -> %ld\n",buff1, kp->KeyData.address);
					break;
				case FIX_LNG_KEY:
					/* this doesnt take care of */
					/* a fix length key being*/
					/* a struct of info*/
					memcpy(buff1,kp->k.key,btree->fix_lng_key_lng);
					buff1[btree->fix_lng_key_lng] = 0;
							
					fprintf(stderr,"found ->%s<-\ndata -> %ld\n",buff1, kp->KeyData.address);
					break;
				case LONG_KEY:
					fprintf(stderr,"found ->%ld<-\ndata -> %ld\n",kp->k.long_key, kp->KeyData.address);
					break;
				case DOUBLE_KEY:
					fprintf(stderr,"found ->%f<-\ndata -> %ld\n",kp->k.double_key, kp->KeyData.address);
					break;
				}
		}
	}
	Close(btree);
	exit(0);
}

/**/
/* pad an area with null values to a length*/
/**/
void pad( char *ptr ,int lng)
{
	int i;
	for(i = strlen(ptr); i < lng; i++ ){
		*(ptr+i) = (char)0;
	}
}



void GetTreeInfo(BTREE *btree){
	
	
	PrintBtDescriptor( btree );
	PrintBfhDescriptor( btree->bfh );

/*
	tok = (int)btree->type_of_key;
	dups = (int)btree->duplicates;
	lng = (int)ReportState(btree,KEY_LENGTH);
	fprintf(stderr,"Type of key = ");
	switch(tok){
		case VAR_LNG_KEY:
			fprintf(stderr,"VAR_LNG_KEY");
			break;
		case FIX_LNG_KEY:
			fprintf(stderr,"FIX_LNG_KEY");
			break;
		case LONG_KEY:
			fprintf(stderr,"LONG_KEY");
			break;
		case DOUBLE_KEY:
			fprintf(stderr,"DOUBLE_KEY");
			break;
	}
	fprintf(stderr," duplicates = %s ",(dups)?"YES":"NO");
	fprintf(stderr," key length = %d ",lng);
	tme = (time_t)btree->time_of_last_close;
	t_ptr = ctime(&tme) ;
	fprintf(stderr,"\n number of keys = %ld, last access %s", 
		btree->number_of_keys, t_ptr);
	fprintf(stderr," Tree Exhausted = %s\n",(btree->exhausted))?
		"TRUE":"FALSE");
*/
}
