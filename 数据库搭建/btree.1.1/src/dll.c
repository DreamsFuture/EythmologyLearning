#include <stdio.h>
/*********************************************************


	Larry A. Walker & Co. 
	Provided free of charge to all clients.
	This code is copyright, but may be used freely and may be modified 
	as necessary to suit the programming task, so long as it contains
	this notification.

	STANDARD DOUBLE LINK LIST ROUTINES

	Function:
		
			*	--- Return a pointer to a DLL descriptor
			* 	--- Return NULL on failure
		DLL	DLL_Create(DLL);
			* 
			*	--- Insert data pointer, add node to end if DLLADD
			*	--- or insert at present position if DLLINSERT
			*	--- If DLL opened, set pointer to first node
			*
		int	DLL_Insert( DLL, int, void * );
			*
			*	--- Return data pointer for first node
			*
		void *	DLL_First(DLL);
			*
			*	--- Return data pointer for next node
			*
		void * 	DLL_Next(DLL);
			*
			*	--- Return data pointer for previous node
			*
		void * 	DLL_Prev(DLL);
			*
			*	--- Return data pointer for current node
			*
		void * 	DLL_Current(DLL);
			*
			*	--- Delete current node
			*
		int	DLL_Delete(DLL);
			*
			*	--- Remember to free data areas malloc'd
			*	--- Return Count of nodes
			*
		void	DLL_Free(DLL);
			*	
			*	--- Free the memory store at the active node.
			*	--- Often managed by outside source or not dynamic memory
			*
		int	DLL_Count(DLL);

	Returns:
		int 0 = fail, 1 = success
		void * NULL = failed
		
*********************************************************/

	/* The number of link list nodes in each malloc group */
#define	DLLCACHE	100

#include "dll.h"

/*internal function definitions */
static int DLL_attach();

/*
*	--- The Double Link List Structure
*/


typedef struct DLL_Node * DLLNODE;

/*.
** Function Name:	DLL_Create
** Access from:		External
** Synopsis:		DLL DLL_Create(void);
** Arguments:		none
** Description:		Create the DLL descriptor and initialize it
** Return:		DLL (pointer to the DLL descriptor )
** History:
**  Created by Larry A. Walker
**   for
**  Modified:
*/

DLL DLL_Create()
{
	DLL dllptr;
	
	dllptr = (DLL)malloc(sizeof(struct DLLinfo));
	if(dllptr == NULL)
		return NULL;
        dllptr->DLL_head = NULL;
        dllptr->DLL_tail = NULL;
        dllptr->DLL_first_vacant = NULL;
        dllptr->DLL_active_ptr = NULL;
        dllptr->DLL_count = 0;
        dllptr->DLL_avail = 0;
        dllptr->magic = DLLMAGIC;
	return dllptr;
}

/*.
**  Function Name:	DLL_Insert
**  Access from:	External
**  Synopsis:		int DLL_Insert( int where, void * data )
**  Arguments:		data	pointer to Link List data area
**  Description:	If the link list was never previously created
**			create the link list.  Insert into the list
**			at the next available node the data pointer and 
**			adjust prior node and next node pointers.
**  Return:		data pointer, NULL for failure
**  History:
**    Created by Larry A. Walker
**      for	Larry A. Walker & Co.
**    Modified:
*/

int DLL_Insert ( activeDLL, where, data )
DLL activeDLL;
int where;
void * data;
{
	int i;
	struct DLL_Node * dllp;
	void *next;
	
	if(activeDLL->magic != DLLMAGIC)
		return DLLFAIL;

	if(data == NULL)
		return DLLFAIL;

	/*
	*	-- if out of nodes, get more
	*/
	if((!activeDLL->DLL_avail) || (activeDLL->DLL_first_vacant == NULL))
		if(!DLL_attach(activeDLL))
			return DLLFAIL;	

	dllp = activeDLL->DLL_first_vacant;
	activeDLL->DLL_first_vacant = (DLLNODE)activeDLL->DLL_first_vacant->next;
	activeDLL->DLL_avail--;
	activeDLL->DLL_count++;

	/*
	*	-- attach data
	*/

	dllp->data = data;
	if( (where == DLLADD) || ( activeDLL->DLL_active_ptr == NULL) ){
		/* add to end of list */
		dllp->prior = (void *)activeDLL->DLL_tail;
		dllp->next = NULL;
		/* if no current ptr set one */
		if(activeDLL->DLL_active_ptr == NULL)
			activeDLL->DLL_active_ptr = dllp;
		if(activeDLL->DLL_tail != NULL)
			activeDLL->DLL_tail->next = (void *)dllp;
		activeDLL->DLL_tail = (DLLNODE)dllp;
	}else{
		/* insert before current position */
		dllp->prior = activeDLL->DLL_active_ptr->prior;
		dllp->next = (void *)activeDLL->DLL_active_ptr;
		if(activeDLL->DLL_active_ptr->prior != NULL){
			((DLLNODE)(activeDLL->DLL_active_ptr->prior))->next = (void *)dllp;
		}else{
			/* need new first node */
			activeDLL->DLL_head = (DLLNODE)dllp;
		}
		activeDLL->DLL_active_ptr->prior = (void *)dllp;
	}
	if(activeDLL->DLL_count == 1){
		/* first one */
		if(activeDLL->DLL_head != NULL){
			return DLLFAIL;
		}
		activeDLL->DLL_head = (DLLNODE)dllp;
		activeDLL->DLL_head->prior = NULL;
		activeDLL->DLL_tail->next = NULL;
		activeDLL->DLL_active_ptr = (DLLNODE)dllp;
	}

	/*
	*	-- Expand vacancies as required
	*/

	if( !activeDLL->DLL_avail){
		if(!DLL_attach(activeDLL))
			return DLLFAIL;
	}

	return DLLSUCCESS;
	
}


/*.
**  Function Name:	DLL_attach
**  Access from:	Internal module only (DLL_Insert)
**  Synopsis:		int DLL_attach()
**  Arguments:		none
**  Description:	Called to attach another piece of main memory to the
**			link list cache of empty available nodes
**  Return:		DLLFAIL or DLLSUCCESS
**  History:
**    Created 10/16/91 by Larry A. Walker
**      for	Larry A. Walker & Co.
**    Modified:
*/
static int DLL_attach(activeDLL)
DLL activeDLL;
{
	struct DLL_Node * dllp;
	struct DLL_Node * bdllp;
	int i;

	activeDLL->DLL_first_vacant = (DLLNODE)malloc( DLLCACHE * sizeof(struct DLL_Node) );
	if(activeDLL->DLL_first_vacant == NULL)
		return DLLFAIL;
	activeDLL->DLL_avail += DLLCACHE;
	dllp = activeDLL->DLL_first_vacant;
	bdllp = NULL;
	for(i=0;i<DLLCACHE;i++){
		dllp->data  = NULL;
		dllp->prior = (void *)bdllp;
		bdllp = dllp;
		++dllp;
		bdllp->next  = (void *)dllp;
	}
	bdllp->next = NULL;
	return DLLSUCCESS;
}

/*.
**  Function Name:	DLL_First
**  Access from:	External
**  Synopsis:		void * DLL_First()
**  Arguments:		none
**  Description:	Set the active pointer to the first node.  
**			Return a pointer to the data content of the
**			first node.
**  Return:		data pointer, NULL for failure
**  History:
**    Created 10/16/91 by Larry A. Walker
**      for		Larry A. Walker & Co.
**    Modified:
*/

void * DLL_First(activeDLL)
DLL activeDLL;
{
	
	if(activeDLL->magic != DLLMAGIC)
		return DLLFAIL;
	if(activeDLL->DLL_count){
		if(activeDLL->DLL_head != NULL){
			activeDLL->DLL_active_ptr = activeDLL->DLL_head;
			return activeDLL->DLL_active_ptr->data;
		}
		return NULL;
	}
	return NULL;
}

/*.
**  Function Name:	DLL_Next
**  Access from:	External
**  Synopsis:		void * DLL_Next();
**  Arguments:		none
**  Description:	Set the active pointer to the next node.
**			Return a pointer to the data content of the
**			next node.
**  Return:		data pointer, NULL for failure
**  History:
**    Created 10/16/91 by Larry A. WalOer
**      for
**    Modified:
*/
void * DLL_Next(activeDLL)
DLL activeDLL;
{
	
	if(activeDLL->magic != DLLMAGIC)
		return DLLFAIL;
	if(activeDLL->DLL_count){
		if(activeDLL->DLL_active_ptr != NULL){
			if(activeDLL->DLL_active_ptr->next != NULL){
				activeDLL->DLL_active_ptr = (DLLNODE)activeDLL->DLL_active_ptr->next;
				return activeDLL->DLL_active_ptr->data;
			}
			return NULL;
		}
		return NULL;
	}
	return NULL;
}

/*.
** Function Name:	DLL_Prev
** Access from:		External
** Synopsis:		void * DLL_Prev()
** Arguments:		none
** Description:		Set the active pointer to the previous node.
**			Return a pointer to the data content of the 
**			previous node.
** Return:		data pointer, NULL for failure
** History:		
**  Created 10/16/91 by Larry A. Walker
**   for
**  Modified:
*/

void * DLL_Prev(activeDLL)
DLL activeDLL;
{
	
	if(activeDLL->magic != DLLMAGIC)
		return DLLFAIL;
	if( activeDLL->DLL_active_ptr->prior == NULL ) 
		return NULL;
	activeDLL->DLL_active_ptr = (DLLNODE)activeDLL->DLL_active_ptr->prior;
	return activeDLL->DLL_active_ptr->data;
}

/*.
** Function Name:	DLL_Current
** Access from:		External
** Synopsis:		void * DLL_Current()
** Arguments:		none
** Description:		Return a pointer to the data content of the 
**			Current node.
** Return:		data pointer, NULL for failure
** History:		
**  Created 10/16/91 by Larry A. Walker
**   for
**  Modified:
*/

void * DLL_Current(activeDLL)
DLL activeDLL;
{
	
	if(activeDLL->magic != DLLMAGIC)
		return DLLFAIL;
	if( activeDLL->DLL_active_ptr == NULL ) 
		return NULL;
	return activeDLL->DLL_active_ptr->data;
}

/*.
** Function Name:	DLL_Count
** Access from:		External
** Synopsis:		int DLL_Count()
** Arguments:		none
** Description:		Access function to the link list node count.
** Return:		Number of nodes in the list.
** History:
**  Created 10/16/91 by Larry A. Walker
**   for
**  Modified:
*/

int DLL_Count(activeDLL)
DLL activeDLL;
{
	
	if(activeDLL->magic != DLLMAGIC)
		return DLLFAIL;
	return activeDLL->DLL_count;
}

/*.
** Function Name:	DLL_Delete
** Access from:		External
** Synopsis:		int DLL_Delete()
** Arguments:		none
** Description:		Delete the active node from the link list
**			The current active node becomes the prior node
**			unless there is no prior node, then it becomes 
**			then next node on the list.
** Return:		DLLSUCCESS or DLLFAIL
** History:
**  Created 10/16/91 by Larry A. Walker
**   for
**  Modified:
*/

int DLL_Delete(activeDLL)
DLL activeDLL;
{

	struct DLL_Node * hold, * hold_also;
	
	if(activeDLL->magic != DLLMAGIC)
		return DLLFAIL;

	/* up front failures */
	if( activeDLL->DLL_active_ptr == NULL ) 
		return DLLFAIL;
	if(activeDLL->DLL_count < 1)
		return DLLFAIL;

	/* --- change prev to point to next */
	if( activeDLL->DLL_active_ptr->prior != NULL)
		((DLLNODE)(activeDLL->DLL_active_ptr->prior))->next = activeDLL->DLL_active_ptr->next;
	else
		/* this is the head */
		activeDLL->DLL_head = (DLLNODE)activeDLL->DLL_active_ptr->next;

	/* --- change next to point to prior */
	if( activeDLL->DLL_active_ptr->next != NULL)
		( (DLLNODE)(activeDLL->DLL_active_ptr->next))->prior = activeDLL->DLL_active_ptr->prior;
	else
		/* this is the tail */
		activeDLL->DLL_tail = (DLLNODE)activeDLL->DLL_active_ptr->prior;

	/* --- save some pointers so they don't disappear so quick */
	hold = activeDLL->DLL_active_ptr;
	hold_also = activeDLL->DLL_first_vacant;

	/* --- add the current node to the free list */
	activeDLL->DLL_first_vacant = activeDLL->DLL_active_ptr;

	/* --- clear the current node and set the new current node*/
	activeDLL->DLL_active_ptr->data = NULL; 	/* hope the calling program has freed !*/
	if(hold->next != NULL){
		/* set current to prior */
		activeDLL->DLL_active_ptr = (DLLNODE)hold->next;
	} else {
		/* already null */
		/* set it to next */
		/* still could be null */
		activeDLL->DLL_active_ptr = (DLLNODE)hold->prior;
	}

	/* --- add the rest of the list to the new free node */
	hold_also->prior = (void *)hold; /* prev first vacant point back to new addition */
	activeDLL->DLL_first_vacant->next = (void *)hold_also; /* new addition point fwd to prev first vacant */
	activeDLL->DLL_first_vacant->prior = NULL; 
	activeDLL->DLL_avail ++; /* one more to use */
	activeDLL->DLL_count --; /* reduce count */;
	if(activeDLL->DLL_count < 0){
		fprintf(stderr,"DLL panic < 0 nodes\n");
	}

	return DLLSUCCESS;
}

/*.
** Function Name:	DLL_Free
** Access from:		External
** Synopsis:		void DLL_Free()
** Arguments:		DLL
** Description:		Free the storage at the current node
** Return:		none
** History:
**  Created 10/16/91 by Larry A. Walker
**   for
**  Modified:
*/

void DLL_Free(activeDLL)
DLL activeDLL;
{

	if(activeDLL->magic != DLLMAGIC)
		return ;
	if(activeDLL->DLL_active_ptr != NULL)
		free(activeDLL->DLL_active_ptr->data);
}
