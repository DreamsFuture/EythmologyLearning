/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */
#ifndef	_RUSAGE_H_
#define	_RUSAGE_H_
extern	"C" {
#include	<stdio.h>
#include	<sys/time.h>
#include	<sys/resource.h>
#include	<unistd.h>
};

class	RUsage
{
	private:
		struct	rusage	last;
		struct	timeval	tlast;

	public:
		RUsage() { Set(); }
		enum Interval { Absolute, Increment };
		inline void Set()
		{
			if (getrusage(RUSAGE_SELF, &last)==-1)
				fprintf(stderr, "Error in getrusage call!\n");
		}
		inline double UserTime(Interval i=Absolute)
		{
			struct	rusage	cur;
			struct	timeval	tcur;
			long	dsec, dusec;
			double	ret=0.0;

			if (getrusage(RUSAGE_SELF, &cur)==-1)
				fprintf(stderr, "Error in getrusage call!\n");
			if (gettimeofday(&tcur, NULL)==-1)
				fprintf(stderr, "Error in gettimeofday call!\n");
			switch(i)
			{
				case	Absolute:
					ret =  (double)(tcur.tv_sec);
					ret += (double)(tcur.tv_usec)*.001;
					break;
				case	Increment:
					dsec = tcur.tv_sec-tlast.tv_sec;
					dusec = tcur.tv_usec-tlast.tv_usec;
					printf("dusec = %ld\n", dusec);
					ret = (double)(dusec)*.001;
					break;
			}
			return(ret);
		}
		inline double SystemTime(Interval i=Absolute)
		{
			struct	rusage	cur;
			struct	timeval	tcur;
			double	ret=0.0;

			if (getrusage(RUSAGE_SELF, &cur)==-1)
				fprintf(stderr, "Error in getrusage call!\n");
			if (gettimeofday(&tcur, NULL)==-1)
				fprintf(stderr, "Error in gettimeofday call!\n");
			printf("tcur.tv_sec = %g\n", (double)tcur.tv_sec);
			printf("tcur.tv_usec = %g\n", (double)tcur.tv_usec);
			switch(i)
			{
				case	Absolute:
					ret =  (double)tcur.tv_sec;
					ret += (double)tcur.tv_usec*.001;
					break;
				case	Increment:
					printf("tlast.tv_sec = %g\n", (double)tlast.tv_sec);
					printf("tlast.tv_usec = %g\n", (double)tlast.tv_usec);
					ret = (double)(tcur.tv_sec-tlast.tv_sec);
					ret += (double)(tcur.tv_usec-tlast.tv_usec)*.001;
					break;
			}
			return(ret);
		}
		inline double TotalTime(Interval i=Absolute)
		{
			return(UserTime(i)+SystemTime(i));
		}
};

#endif
