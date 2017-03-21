/*
	

 #####   ######  #####   #    #   ####
 #    #  #       #    #  #    #  #    #
 #    #  #####   #####   #    #  #
 #    #  #       #    #  #    #  #  ###
 #    #  #       #    #  #    #  #    #
 #####   ######  #####    ####    ####

*/

/*
	This file is header for the debug.o object which provides
	special functionality code and definitions for the debugging
	process.
	You will historcally find this file, or something very similar in 
	every system programnmed by Larry A. Walker.
*/
#ifndef LWDEBUG
#define LWDEBUG

#define USESTDARGVARARG

/*
*       --- Includes
*/
#include <stdio.h>
#ifdef USESTDARGVARARG
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <stdlib.h>
#include <stddef.h>
#include <malloc.h>
#include <poll.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>


#ifndef SUCCESS
#define SUCCESS 	1
#endif

#ifndef FAILED
#define FAILED 		0		
#endif

#define 	DEBUG	1
#define		TRACE	2
#define		VIEW	3

#define 	db()	fprintf(stderr,"%s:%d\n",__FILE__,__LINE__)

/* Debug printing */

	/* includes programmer's only code segments */
	/* debugging and tracing are intended to be in the final */
	/* delivered product and be switch selectable, however, to */
	/* compile without the code in the module, remove these defines */
#define DEVELOPMENT     

/* Define DEBUGGING and tracing to separate out Debug() and Trace() calls
   from being compiled */
#define DEBUGGING	
#define TRACING	
	/* define this for malloc debugging */
	/* if you are compiling with debugging, you may wish to have */
	/* the malloc redefines turned on.  This will report to your */
	/* debug file the memory allocations and releases */
#define INCLUDEDBMALLOC


	/* rarely used function which prints variable debugging information
	   - used to keep the file/function/line info from appearing in 
	   the output
	*/
void debug_pt( char *format, ...);
void debug_pd( char *format, ...);
void debug_pv( char *format, ...);
	/* low level support routine not used in code */
void debug_id( int level, char * file, char * function, int line, int cr);

	/* print variable parameter information and exit */
void FatalError( const char *format, ...);
	/* open the debug output file.  in addition to file names the
	   words "stderr" or "stdout" can be used */
int debug_open( int level, char * file);
	/* functions to let the applaction know what is in effect */
int DebugIsOpen();
int DebugLevel();

	/* these are the real guts of the process.  note that since 
	   these are multi statement macros always write
		if(test){
			Debug("Test %s is a success.\n", mytest);
		}
	   not
		if(test)
			Debug("Test %s is a success.\n", mytest);
	*/
#define __FUNCTION__ "unknown"

/*
*	--- Debug output is used where errors are encountered and 
*	--- causes the errors to be written to an error log or 
*	--- stderr/stdout as determined by the debug_open function
*	--- If debug_open has not been called Debug output is not 
*	--- written
*/
#ifdef DEBUGGING
#define Debug	\
		debug_id(DEBUG,__FILE__,__FUNCTION__,__LINE__,0);\
		debug_pd
#else
#define Debug
#endif

/*
*	--- Tracing output is used to print informational messages
*	--- to the log during program development.  Functions often
*	--- begin by printing their name.  In the final product, TRACING
*	--- should not be defined.
*/
#ifdef TRACING
#define Trace	\
		debug_id(TRACE,__FILE__,__FUNCTION__,__LINE__,1);\
		debug_pt
#else
#define Tracing
#endif
/*
*	--- View replaces printf.  If there is no log open view performs
*	--- exactly as printf.  If there is a log open, and if it is open
*	--- for tracing, the output is directed to that file rather than 
*	--- stdout, so that the chain of events can be maintained.  
*/
#define View	debug_pv

	/* Memory debugging */
	/* These functions make calls the the Debug shown above, showing
	   what is being malloc'd free'd etc.
	*/

char *debug_malloc(unsigned size);
void debug_free(void *ptr);
char *debug_realloc(void *ptr, unsigned size);
char *debug_calloc(unsigned nelem, unsigned elsize);


	/* All you need to is define INCLUDEDBMALLOC and you'll be 
	   remapped to the calls above */
#ifdef INCLUDEDBMALLOC
#define malloc debug_malloc
#define free debug_free
#define realloc debug_realloc
#define calloc debug_calloc
#endif

	/* This function formats the output of times() to hrs, min, sec, hz
	*/
char * hzTime( long );
	
	/* Set the start time for a section of code you want to time */
	/* pass first the line number as __LINE__, and secondly the  */
	/* value of the counter you want to set, 0-(TIMECOUNTERS-1)  */
void sProf( int, int );
#define TIMECOUNTERS    20

	/* Report time for a prof segment using Debug() */
	/* Pass it first __LINE__ and secondly the counter you want reported */
void eProf( int, int );

	/* Macro for sleeping in milliseconds, or hz which is 100 of a sec */
#define hzSleep(hz) poll(NULL,0,hz * 10)
#define msSleep(ms) poll(NULL,0,ms)

#endif /*LWDEBUG*/
