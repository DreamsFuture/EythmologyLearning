#include "treelib.h"
#include "btree.h"
#include "debug.h"

main(int argc,char **argv)
{
	struct Current_Key;
	int count;
	char number[10];
	char buffer[BUFSIZ];
	struct KeyGroup * kp;
	BTREE * btree;
	
	if(argc != 2){
		fprintf(stderr,"Usage: list <file name>\n");
		exit(1);
	}
/*
	debug_open( TRACE, "stderr");
*/
	btree = OpenBtree(*(argv+1), 0, VAR_LNG_KEY, 0, 0);
	
	count = 0;
	printf("Begin list\n");
	if((kp=First(btree)) == NULL){
		printf("NO Keys\n");
		exit(1);
	}
	DisplayKey(btree,kp);
	count++;
	while( (kp = Increment(btree)) != NULL){
		View("Next Key --> ");
		DisplayKey(btree,kp);
		count++;
		}
	printf("End of list Count=%d\n",count);
	printf("=========================\n");
	exit(0);
}

