/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */

#ifdef	__GNUC__
#pragma interface
#endif

#ifndef	_TIMER_H_
#define	_TIMER_H_
extern	"C" {
#include	<stdio.h>
#include	<sys/time.h>
#include	<sys/resource.h>
#include	<unistd.h>
};

class	Timer
{
	private:
		struct	timeval	start;
		struct	timeval	finish;

	public:
		Timer() { SetTimer(); }
		void SetTimer();
		inline	double	Elapsed()
		{
			double	ret=-1.0;

			if (gettimeofday(&finish, (struct timezone *) NULL)==-1)
				fprintf(stderr, "Error in Timer::Elapsed (gettimeofday)!\n");
			else
			{
				finish.tv_sec -= start.tv_sec;
				if (finish.tv_usec < start.tv_usec)
				{
					finish.tv_usec += 1000000;
					--finish.tv_sec;
				}
				finish.tv_usec -= start.tv_usec;
				ret = finish.tv_sec * 1000.0 + finish.tv_usec / 1000.0;
				ret/=1000.0;
			}
			return(ret);
		}
};

#endif
