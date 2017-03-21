/*
----------------------------------------------------------------------

     #####   ######  #####   #    #   ####
     #    #  #       #    #  #    #  #    #
     #    #  #####   #####   #    #  #
     #    #  #       #    #  #    #  #  ###
     #    #  #       #    #  #    #  #    #
     #####   ######  #####    ####    ####

----------------------------------------------------------------------
**
**
** Module    :	 debug
** File      :	 debug.c
** Functions :	
**	        
**	        
** Written by:	Larry A. Walker
**	        
**	        
** History   :	
**	   	Created: Fri Oct 28 11:28:38 EDT 1994
*/

static char sccsid[]="%Z% Module debug ( %M% ) %I% %G% %U%";

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
* Function Name: 	debug
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
* Created  Fri Oct 28 11:28:57 EDT 1994 by Larry A. Walker
*
*
*  Modified:
*
******************************************************/

static FILE * debug_out = NULL;
static int debug_level = 0;

	/* DEBUG level version */
void debug_pd( char *format, ...)
{
#ifdef __FUNCTION__
#undef __FUNCTION__
#endif
#define __FUNCTION__ "debug"
	va_list ap;
	
	if(debug_out == NULL)
		return ;
	/* if nither level set , quit*/
	if(debug_level != TRACE)
		if(debug_level != DEBUG)
			return;
	va_start( ap, format);
	vfprintf( debug_out, format, ap );
	va_end(ap);
	fflush( debug_out );
} /*end  "debug_pd" */
	/* TRACE level version */
void debug_pt( char *format, ...)
{
#ifdef __FUNCTION__
#undef __FUNCTION__
#endif
#define __FUNCTION__ "debug"
	va_list ap;
	
	if(debug_out == NULL)
		return ;
	/* if if not trace level , quit*/
	if(debug_level != TRACE)
			return;
	va_start( ap, format);
	vfprintf( debug_out, format, ap );
	va_end(ap);
	fflush( debug_out );
} /*end  "debug_pt" */
void debug_pv( char *format, ...)
{
#ifdef __FUNCTION__
#undef __FUNCTION__
#endif
#define __FUNCTION__ "debug"
	va_list ap;
	
	va_start( ap, format);
	if(( debug_out != NULL) && (debug_level == TRACE))
		vfprintf( debug_out, format, ap );
	else
		vfprintf( stdout , format, ap );
	va_end(ap);
	if( debug_out != NULL)
		fflush( debug_out );
	else
		fflush( stdout );
} /*end  "debug_pt" */
/*.*****************************************************
* Function Name: 	FatalError
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
* Created  Mon Nov 21 15:23:03 EST 1994 by Larry A. Walker
*
*
*  Modified:
*
******************************************************/

void FatalError( const char *format, ...)
{
#ifdef __FUNCTION__
#undef __FUNCTION__
#endif
#define __FUNCTION__ "FatalError"
	va_list ap;
	
	if(debug_out == NULL){
		debug_out = stderr;
	}

	va_start( ap, format);
	vfprintf( debug_out, format, ap );
	va_end(ap);
	fflush( debug_out );
	exit(1);
}

/*.*****************************************************
* Function Name: 	debug_id
*
* Created  Tue Nov 8 09:04:56 EST 1994 by Larry A. Walker
*
*  Modified:
*
******************************************************/

void debug_id( int level, char * file, char * function, int line, int cr)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"debug_id"
	extern int errno;
/* Auto's */
	if(debug_out == NULL)
		return ;
	/* if nither level set , quit*/
	if(debug_level != TRACE)
		if(debug_level != DEBUG)
			return;
	/* if debug set, but request is for trace, quit */
	if(( debug_level == DEBUG) && (level == TRACE))
		return;

	if(errno != 0){
		fprintf( debug_out, "->%s:%d:%s:errno=%d:",file,line,function,
			errno);
	}else{
		fprintf( debug_out, "->%s:%d:%s:",file,line,function);
	}
	if(cr)
		fprintf( debug_out, "\n");
} /*end  "debug_id" */

/*.*****************************************************
* Function Name: 	debug_open
*
* Access from:
*
* Synopsis:
*
* Arguments:
*
* Description:
*
* Return:	SUCCESS or FAILED
*
* History:
*
* Created  Mon Nov 7 16:31:30 EST 1994 by Larry A. Walker
*
*  Modified:
*
******************************************************/

int debug_open( int level, char * file)
{
#ifdef __FUNCTION__
#undef __FUNCTION__
#endif
#define __FUNCTION__ "debug_open"
	FILE * fd;


	if( (level != TRACE) && (level != DEBUG) )
		return FAILED;
	fd = NULL;
	if( strncmp( file, "stdout", 6) == 0){
		fd = stdout;
	}else{
		if( strncmp( file, "stderr", 6) == 0){
			fd = stderr;
		}else{
			fd = fopen( file, "w+" );
		}
	}
	if(fd != NULL){
		/* see if one was already open */
		if( debug_out != NULL){
			if( debug_out != stdout ){
				if( debug_out != stderr ){
					fclose( debug_out );
				}
			}
		} 
		debug_out = fd;
		debug_level = level;
	}else{
		return FAILED;
	}
	Trace("PID: %d, Log %s open.\n", getpid(), file);
	return SUCCESS;
} /*end  "debug_open" */
/*.*****************************************************
* Function Name: 	debug_malloc
*
* Created  Tue Nov 8 09:48:05 EST 1994 by Larry A. Walker
*
*  Modified:
*
******************************************************/

char *debug_malloc(unsigned size)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"malloc"
/* Auto's */
char * ptr;

#ifdef TRACING
Trace("Enter->%s()\n","debug_malloc" );
#endif
#undef mallocdef
#ifdef malloc
#define mallocdef
#undef malloc
#define malloc malloc
#endif
	ptr = (char *)malloc(size);
#ifdef mallocdef
#undef malloc
#define malloc debug_malloc
#endif
	Trace("DBM malloc %u bytes address = 0x%x \n",size,(unsigned)ptr);
	return ptr;
	
} /*end  "debug_malloc" */

/*.*****************************************************
* Function Name: 	debug_calloc
*
* Created  Tue Nov 8 09:48:05 EST 1994 by Larry A. Walker
*
*  Modified:
*
******************************************************/

char *debug_calloc(unsigned nelem, unsigned size)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"calloc"
/* Auto's */
char * ptr;

#ifdef TRACING
Trace("Enter->%s()\n","debug_calloc" );
#endif
#undef callocdef
#ifdef calloc
#define callocdef
#undef calloc
#define calloc calloc
#endif
	ptr = (char *)calloc(nelem,size);
#ifdef callocdef
#undef calloc
#define calloc debug_calloc
#endif
	Trace("DBM calloc %u elements of %u size address = 0x%x \n",nelem,
		size,(unsigned)ptr);
	return ptr;
	
} /*end  "debug_calloc" */

/*.*****************************************************
* Function Name: 	debug_realloc
*
* Created  Tue Nov 8 09:48:05 EST 1994 by Larry A. Walker
*
*  Modified:
*
******************************************************/

char *debug_realloc(void *old_ptr, unsigned size)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"realloc"
/* Auto's */
char * ptr;

#ifdef TRACING
Trace("Enter->%s()\n","debug_realloc" );
#endif
#undef reallocdef
#ifdef realloc
#define reallocdef
#undef realloc
#define realloc realloc
#endif
	ptr = (char *)realloc(old_ptr,size);
#ifdef reallocdef
#undef realloc
#define realloc debug_realloc
#endif
	Trace("DBM realloc 0x%x old_ptr to %u size address = 0x%x \n",
		(unsigned)old_ptr, size,(unsigned)ptr);
	return ptr;
	
} /*end  "debug_realloc" */

/*.*****************************************************
* Function Name: 	debug_free
*
* Created  Tue Nov 8 09:48:05 EST 1994 by Larry A. Walker
*
*  Modified:
*
******************************************************/

void debug_free(void *old_ptr)
{
#ifdef	__FUNCTION__
#undef	__FUNCTION__
#endif
#define	__FUNCTION__	"free"
/* Auto's */

#ifdef TRACING
Trace("Enter->%s()\n","debug_free" );
#endif
#undef freedef
#ifdef free
#define freedef
#undef free
#define free free
#endif
	free(old_ptr);
#ifdef freedef
#undef free
#define free debug_free
#endif
	Trace("DBM free 0x%x old_ptr\n", (unsigned)old_ptr);
	return;
	
} /*end  "debug_free" */
/*================================================================
			profiling
================================================================*/

/***********************************************************

	A collection of profiling and timing functions
**		
	1992 Larry A. Walker
***********************************************************/
/* for HPUX */
#ifndef _INCLUDE_POSIX_SOURCE
#define _INCLUDE_POSIX_SOURCE
#endif

#include <sys/types.h>
#include <sys/times.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/times.h>

static struct {
	int  CounterLine;
	long BaseTimeCounter;
}CounterHolder[TIMECOUNTERS];
static struct tms btmsbuf;

/*.
**	----------------------------------------
**		sProf
**	----------------------------------------
**
** Function  :	Set the start time for a prof segment
** Arguments :	none
** Return    :	Undefined
**
*/

void sProf(int line, int counter)
{
	if( counter < TIMECOUNTERS){
		CounterHolder[counter].CounterLine = line;
		CounterHolder[counter].BaseTimeCounter = times( &btmsbuf);
	}
}
/*.
**	----------------------------------------
**		eProf
**	----------------------------------------
**
** Function  :	Report time for a prof segment
** Arguments :	line number
** Return    :	Undefined
**
*/

void eProf(int line, int counter)
{
	char * ptr;
	long TimeCounterEnding;
	if( counter < TIMECOUNTERS){
		/* current time */
		TimeCounterEnding = times( &btmsbuf ); 
		/* time difference */
		TimeCounterEnding = 
			(TimeCounterEnding - CounterHolder[counter].BaseTimeCounter);
		ptr = hzTime(TimeCounterEnding);
		Debug("Time: Fm: %d To: %d %s\n",CounterHolder[counter].CounterLine,line, ptr);
	}
}

/*.
**	----------------------------------------
**		sTimer
**	----------------------------------------
**
** Function  :	Set the timing mechanism
** Arguments :	none
** Return    :	time from arbitrary point
**
*/
long sTimer( )
{
	return times( &btmsbuf );
}
/*.
**	----------------------------------------
**		cTimer
**	----------------------------------------
**
** Function  :	check the timeing mech
** Arguments :	base time returned by sTimer
**		number of HZ clicks to check for
** Return    :	bolean true or false that time has expired
**
*/
int cTimer ( long basetime, int HZcount )
{
	long TimeCounterEnding;

		/* current time */
	TimeCounterEnding = times( & btmsbuf ); 
		/* time difference */
	TimeCounterEnding = (TimeCounterEnding - basetime); 
	return (int) TimeCounterEnding > HZcount;
}

/*.
**	----------------------------------------
**		aTime
**	----------------------------------------
**
** Function  :	create a string of h m s from a time count of seconds
** Arguments :	time_t value
** Return    :	pointer to static string
**
*/
char _aTime[30];
char * aTime ( time_t intime )
{
	_aTime[0] = 0;
	sprintf(_aTime, "%03dh %02dm %02ds",
		(intime / 3600), 
		(intime % 3600 ) /60, 
		intime % 60);
	_aTime[12] = 0;
	return _aTime;
}

/*.
**	----------------------------------------
**		hzTime
**	----------------------------------------
**
** Function  :	create a string of h m s ms from a time count of milliseconds
** Arguments :	long count of hz
** Return    :	pointer to static string
**
*/
char * hzTime ( long intime )
{
#ifdef __FUNCTION__
#undef __FUNCTION__
#endif
#define __FUNCTION__ "hzTime"
	_aTime[0] = 0;
	sprintf(_aTime, "%03dh %02dm %02ds %02dhz",
		(intime / (3600*HZ) ), 
		(intime % (3600*HZ)  ) /(60*HZ), 
		(intime % (60*HZ)) / HZ, 
		intime % HZ);
	_aTime[18] = 0;
	return _aTime;
}

/*
	--- Sometimes we just need to know if debug is on
*/

int DebugIsOpen()
{
	if(debug_out == NULL)
		return 0;
	else
		return 1;
}

/*
	--- Sometimes we want to know what the debug level is
*/
int DebugLevel()
{
	return debug_level;
}
