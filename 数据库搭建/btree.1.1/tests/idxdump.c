#include "treelib.h"
#include "btree.h"
#include "debug.h"

main(int argc,char **argv)
{
	struct Current_Key;
	int count;
	char number[10];
	char buffer[BUFSIZ];
	BTREE * btree;


	if(argc != 2){
		fprintf(stderr,"Usage: idxdump <file name>\n");
		exit(1);
	}
	btree = OpenBtree(*(argv+1), 0, VAR_LNG_KEY, 0, 0);

	DumpBtree(btree);
	exit(0);
}
