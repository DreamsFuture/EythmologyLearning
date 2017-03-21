#include "treelib.h"
#include "btree.h"
#include "debug.h"
#define DOPADDING
void mv_mem(char *source,char *destin,int len);

#define EXTENDER "variable_length_key_number"

char * extender = EXTENDER;

main(int argc,char **argv)
{
	struct Node node;
	struct KeyGroup k;
	long i,j;
	long term;
	char number[10], buffer[BUFSIZ], *ptr;
	char name[40];
	int create,type,dups,size;
	int verify;
	union   Key             bk;
	int start;
	int stat;
	int ii;
	BTREE * btree;
	int dblevel;
	
	
	printf("================================================\n");
	printf("      Program to generate keys for a BTree\n");
	printf("================================================\n");
	
	printf("Enter the name of the index.\n: ");
	if(argc > 1){
		strcpy(name,*(argv+1));
		printf("%s\n",name);
	}else{
		gets(name);
	}
	
	printf("1.  Create a new index.\n");
	printf("2.  Add to current index.\n");
	printf("\nEnter a number to select an item.\n: ");
	if(argc > 2){
		strcpy(buffer,*(argv+2));
		printf("%s\n",buffer);
	}else{
		gets(buffer);
	}
	create=(atoi(buffer));
	if( (create < 1) || (create > 2) ){
		fprintf(stderr,"Create Must be either 1 or 2\n");
		exit(1);
	}
	if(create == 2) create = 0;
 	else
		create = CREATE;
	
	printf("1.  Variable length key.\n");
	printf("2.  Fix length key.\n");
	printf("3.  Long key.\n");
	printf("4.  Double key.\n");
	printf("\nEnter a number to select an item.\n: ");
	if(argc > 3){
		strcpy(buffer,*(argv+3));
		printf("%s\n",buffer);
	}else{
		gets(buffer);
	}
	type=(atoi(buffer));
	switch(type){
		case 1:
			type = VAR_LNG_KEY;
			break;
		case 2:
			type = FIX_LNG_KEY;
			printf("\nEnter the size of the key.\n: ");
			gets(buffer);
			size = (atoi(buffer));
			break;
		case 3:
			type = LONG_KEY;
			break;
		case 4:
			type = DOUBLE_KEY;
			break;
		default:
			fprintf(stderr,"Key Type Must be either 1, 2, 3 or 4\n");
			exit(1);
			break;
	}
	
	printf("1.  Duplicates allowed.\n");
	printf("2.  No duplicates allowed.\n");
	printf("\nEnter a number to select an item.\n: ");
	if(argc > 4){
		strcpy(buffer,*(argv+4));
		printf("%s\n",buffer);
	}else{
		gets(buffer);
	}
	dups=(atoi(buffer));
	if( (dups < 1) || (dups > 2) ){
		fprintf(stderr,"Duplicates Must be either 1 or 2\n");
		exit(1);
	}
	if(dups == 2) 
		dups = 0;

	printf("1. No debugging\n");
	printf("2. Debug level \n");
	printf("3. Trace level \n");
	printf("\nEnter a number to select an item.\n: ");
	if(argc > 7){
                strcpy(buffer,*(argv+7));
                printf("%s\n",buffer);
        }else{
                gets(buffer);
        }
	dblevel = (atoi(buffer));
	if( dblevel < 1 || dblevel > 3){
		fprintf(stderr, "Debugging Must be either 1, 2, or 3\n");
		exit(1);
	}
	if(dblevel == 3)
		debug_open( TRACE, "gen.log");
	else
		if(dblevel == 2)
			debug_open( DEBUG, "gen.log");
	
	btree = OpenBtree(name, create, type, dups, size);
	if(btree == NULL){
		Debug("Could not open tree Btree( %s, %d, %d, %d )\n", 
			name, type, dups, size);
		exit(1);
	}
	
	ptr = (char *)&node;
	for(j=0;j< sizeof(node);j++){
		*ptr++ = 0;
		}
	
	printf("\nEnter the number of keys to generate.\n: ");
	if(argc > 5){
		strcpy(buffer,*(argv+5));
		printf("%s\n",buffer);
	}else{
		gets(buffer);
	}
	term =  atol(buffer);
	
	printf("\nEnter the beginning number.\n: ");
	if(argc > 6){
		strcpy(buffer,*(argv+6));
		printf("%s\n",buffer);
	}else{
		gets(buffer);
	}
	start =  atoi(buffer);
	i = start;
	j = term;
	
	fprintf(stderr,"\nVerify?\n: ");
	gets(buffer);
	if(( buffer[0] == 'Y' ) || (buffer[0] == 'y')){
		verify = 1;
	}
	sProf(__LINE__, 0);
	for(; j ; j--){
		if(j)
			if(!(j%1000))
				fprintf(stderr,".");
		k.KeyData.address = (long)i;
		switch(type){
			case 1:
				sprintf(buffer,"%s%ld",extender,i++);
				create_index_string( k.k.key,buffer);
				break;
			case 2:
#ifdef DOPADDING
				for(ii = 0; ii < btree->fix_lng_key_lng; ii++){
					k.k.key[ii] = ' ';
				}
				sprintf(buffer,"%ld",i++);
				mv_mem(buffer,k.k.key,strlen(buffer));
				k.k.key[btree->fix_lng_key_lng-1] = 0;

#else
				sprintf(k.k.key,"%ld",i++);
#endif
				break;
			case 3:
				k.k.long_key = i++;
				break;
			case 4:
				k.k.double_key = (double)i++;
				break;
		}
		if( (stat = InsertKey(btree,&k) ) == FAIL){
			fprintf(stderr,"couldnt insert %ld\n",(i-1));
			break;
		}
/*
if( (!reported) && (i>93) ){
	sprintf(k.k.key,"%ld",93);
	if(Locate(btree,&k.k) == FAIL) {
        	fprintf(stderr,"93 not in index when added %d\n", i);
		reported = 1;
       	}
}
*/
	}
	eProf(__LINE__, 0);

	sProf(__LINE__, 0);
	if(verify){
		i = start;
		j = term;
		fprintf(stderr,"\nverifying\n");
		for(; j ; j--){
			if(j)
				if(!(j%1000))
					fprintf(stderr,".");
			k.KeyData.address = (long)i;
			switch(type){
				case 1:
					sprintf(buffer,"%s%ld",extender,i++);
					create_index_string( bk.key,buffer);
					break;
				case 2:
#ifdef DOPADDING
				for(ii = 0; ii < btree->fix_lng_key_lng; ii++){
					bk.key[ii] = ' ';
				}
				sprintf(buffer,"%ld",i++);
				mv_mem(buffer,bk.key,strlen(buffer));
				bk.key[btree->fix_lng_key_lng-1] = 0;

#else
				sprintf(bk.key,"%ld",i++);
#endif
				break;
				case 3:
					bk.long_key = i++;
					break;
				case 4:
					bk.double_key = (double)i++;
					break;
			}
			if(Locate(btree, &bk) == FAIL) {
				if( (type == 1) || (type == 2))
       	         			fprintf(stderr,"\"%s\" not in index\n", buffer);
				else
       	         			fprintf(stderr,"%d not in index\n", i);
       	         	}
		}
	}

	eProf(__LINE__, 0);

	View("\n=========================\n");
	DisplayKey(btree,First(btree));
	DisplayNodeHeader(btree );
	DisplayKey(btree,Last(btree));
/*HERE*/
	Close(btree);
	exit(0);
}
