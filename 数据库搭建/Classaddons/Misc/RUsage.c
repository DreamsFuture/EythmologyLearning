/*
 *	Copyright (C) 1992 Michael Tiller
 *
 *	Permission must be explicitly given by the author to use, copy,
 *  modify, distribute and/or sell any portion of this code.  This
 *  code is provided "as is" without any expressed or implied warranty.
 *
 */
#include	<sys/time.h>
#include	<sys/resource.h>

class	RUsage
{
	private:
		struct	rusage	last;

	public:
		RUsage() { Set(); }
		enum Interval { Absolute, Increment };
		inline void Set()
		{
			getrusage(RUSAGE_SELF, &last);
		}
		inline double UserTime(Interval i=Absolute)
		{
			struct	rusage	cur;
			double	ret=0.0;

			getrusage(RUSAGE_SELF, &cur);
			switch(i)
			{
				case	Absolute:
					ret =  (double)cur.ru_utime.tv_sec;
					ret += (double)cur.ru_utime.tv_usec*.001;
					break;
				case	Increment:
					ret =
						(double)(cur.ru_utime.tv_sec-last.ru_utime.tv_sec);
					ret +=
						(double)(cur.ru_utime.tv_usec-last.ru_utime.tv_usec)*.001;
					break;
			}
			return(ret);
		}
		inline double SystemTime(Interval i=Absolute)
		{
			struct	rusage	cur;
			double	ret=0.0;

			getrusage(RUSAGE_SELF, &cur);
			switch(i)
			{
				case	Absolute:
					ret =  (double)cur.ru_stime.tv_sec;
					ret += (double)cur.ru_stime.tv_usec*.001;
					break;
				case	Increment:
					ret =
						(double)(cur.ru_stime.tv_sec-last.ru_stime.tv_sec);
					ret +=
						(double)(cur.ru_stime.tv_usec-last.ru_stime.tv_usec)*.001;
					break;
			}
			return(ret);
		}
		inline double TotalTime(Interval i=Absolute)
		{
			return(UserTime(i)+SystemTime(i));
		}
};
