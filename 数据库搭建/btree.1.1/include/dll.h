/***************************************************************

	Header information for the LAW&C Double Link List

	Copr. 1991, 1992 Larry A. Walker & Co.

	All clients may freely use this code and dll.c 
	for any use desired, so long as it contains this
	statement
***************************************************************/

#define trace()	fprintf(stderr, "%s : %d \n",__FILE__, __LINE__);
/*		DLL defines 	*/


#define DLLFAIL		0
#define DLLPTRFAIL	NULL
#define DLLSUCCESS	1
#define DLLINSERT	1
#define DLLCURRENT	1
#define DLLADD		2
#define DLLEND		2

#define DLLMAGIC	1704



struct DLL_Node {
	void * prior;
	void * next;
	void * data;
};

struct DLLinfo {
	long			magic;
	/* 
		points to the first filled node
	*/
	struct DLL_Node * 	DLL_head;
	/* 
		points to the last filled node
	*/
	struct DLL_Node * 	DLL_tail;
	/* 
		points to the first vacant node
	*/
	struct DLL_Node * 	DLL_first_vacant;
	/* 
		points to the current node 
	*/
	struct DLL_Node *	DLL_active_ptr;
	/* 
		the total number of nodes filled 
	*/
	int		  	DLL_count;
	/* 
		the total number of empty nodes available 
	*/
	/* 
		also the open switch 
	*/
	int		  	DLL_avail;

};

typedef struct DLLinfo * DLL;

DLL  	DLL_Create();
int	DLL_Insert();
void *	DLL_First();
void * 	DLL_Next();
void * 	DLL_Prev();
void * 	DLL_Current();
int	DLL_Delete();
int	DLL_Count();
void	DLL_Free();

