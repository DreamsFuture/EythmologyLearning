
/*

  ####   ######  #    #    ##    #####   #    #   ####   #####   ######
 #       #       ##  ##   #  #   #    #  #    #  #    #  #    #  #
  ####   #####   # ## #  #    #  #    #  ######  #    #  #    #  #####
      #  #       #    #  ######  #####   #    #  #    #  #####   #
 #    #  #       #    #  #    #  #       #    #  #    #  #   #   #
  ####   ######  #    #  #    #  #       #    #   ####   #    #  ######

*/

static char sccsid[]="%Z% Module semiphore ( %M% ) %I% %G% %U%";

/*
*	--- Includes
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include "debug.h"

/*
*	--- Definitions 
*/

/*
*	--- Externals
*/

/*
*	--- Definitions of Local Module Functions
*/

/*
*	--- Module Globals
*/

/*.*****************************************************
* Function Name: 	semid
*
* Access from:
*
* Synopsis:
*
* Arguments:
*
* Description:
*
* Return:
*
* History:
*
* Created  Mon Apr 10 09:16:45 EDT 1995 by Larry A. Walker
*
*   for ITN Project, PRC McLean Va
*
*  Modified:
*
******************************************************/

int semid( int key )
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"semid"
/* Auto's */
	int sid;
	extern int errno;
	unsigned short value;
Trace("Enter->%s()\n", "semid" );
	
	errno = 0;
	if((sid=semget((key_t)key, 1, 0))==-1){
		if(errno == ENOENT){
			/* it needs to be created */
			if((sid=semget((key_t)key, 1, 0666|IPC_CREAT))==-1){
				Debug("failed semget()\n");
				return -1;
			}else{
				value = 1;
				if( semctl( sid, 1, SETALL, &value) == -1){
					Debug("failed semctl()\n");
					return -1;
				}
			}
		}
	}else{
	}
	return(sid);
	
} /*end  "semid" */
/*.*****************************************************
* Function Name: 	semcall
*
* Access from:
*
* Synopsis:
*
* Arguments:
*
* Description:
*
* Return:
*
* History:
*
* Created  Mon Apr 10 10:39:13 EDT 1995 by Larry A. Walker
*
*   for ITN Project, PRC McLean Va
*
*  Modified:
*
******************************************************/

int semcall( int key, int op, int pause)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"semcall"
/* Auto's */
	int sid;
	struct sembuf sb;
	extern int errno;
Trace("Enter->%s()\n", "semcall" );

	if(( sid = semid(key)) == -1){
		return -1;
	}
	sb.sem_num = 0;
	sb.sem_op = op;
	sb.sem_flg = SEM_UNDO;
	if((op == -1) && (pause == 0))
		sb.sem_flg |= IPC_NOWAIT;
	errno = 0;
	if(semop(sid, &sb, 1) == -1){
		Debug("failed semop()\n");
		if(sb.sem_flg == IPC_NOWAIT){
			if(errno == EAGAIN){
				return 0;
			}
		}
		return -1;
	}
	/* If nowait and it would have been successful, return 0, else
	   return 1.  That is, if you set pause to 0 and call P() 
	   and your return is 0, that means nobody was in the 
	   area the semaphore had locked.  Useful for a
	   should I do it now, or should I do it later sort of thing */
	return 0;
} /*end  "semcall" */


/*.*****************************************************
* Function Name: 	P
*
* Access from:
*
* Synopsis:
*
* Arguments:	
*
* Description:	acquire a semaphore, if pause is set then 
*		do not return until the semiphore is acquired
*
* Return:
*
* History:
*
* Created  Mon Apr 10 10:53:32 EDT 1995 by Larry A. Walker
*
*   for ITN Project, PRC McLean Va
*
*  Modified:
*
******************************************************/

int P( int key , int pause)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"P"
/* Auto's */
Trace("Enter->%s()\n", "P" );
	return(semcall(key, -1, pause));
} /*end  "P" */
/*.*****************************************************
* Function Name: 	V
*
* Access from:
*
* Synopsis:
*
* Arguments:
*
* Description:	release a semaphore
*
* Return:
*
* History:
*
* Created  Mon Apr 10 10:54:35 EDT 1995 by Larry A. Walker
*
*   for ITN Project, PRC McLean Va
*
*  Modified:
*
******************************************************/

int V( int key)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"V"
/* Auto's */
Trace("Enter->%s()\n", "V" );
	return(semcall(key, 1, 0));
} /*end  "V" */
/*.*****************************************************
* Function Name: 	rmsem
*
* Access from:
*
* Synopsis:
*
* Arguments:
*
* Description:	remove the semiphore indicated by key
*
* Return:
*
* History:
*
* Created  Mon Apr 10 11:03:47 EDT 1995 by Larry A. Walker
*
*   for ITN Project, PRC McLean Va
*
*  Modified:
*
******************************************************/

int rmsem(int key)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"rmsem"
/* Auto's */
Trace("Enter->%s()\n", "rmsem" );
	return ( semctl( semid( key ), 0, IPC_RMID) );
} /*end  "rmsem" */
