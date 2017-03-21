#include	<math.h>
#include	<stdio.h>

#define	TOLERANCE	3.0e-11 /* value used in NRiC */

/* I originally used the Gauss-Legendre routine 'gauleg' from
   Numerical Recipes in C.  Since that is copyrighted, I decided
   to write my own.  In doing so I think I found a couple errors
   in the Gauss-Legendre recurrence relation in NRiC (in the
   theory section, not in the code itself). */

void
mygauleg(int n, double x1, double x2, double *x, double *w)
{
	double	xavg = (x2+x1)/2.0, dx = (x2-x1);
	double	zi, oldzi;
	int	n2 = (n+1)/2;
	int	i, j;
	double	p[3], dp;

	/* loop over lower half the points */
	for(i=1;i<=n2;i++)
	{
		/* initial guess at root */
		zi = cos(M_PI*(i-.25)/(n+.5));

		do
		{
			/* Calculate value of nth polynomial.  This will be used
			   to calculate the derivative of the polynomial later on.
			   the derivative in turn is used to improve the guess of the
			   root */
			p[1] = 0.0;
			p[2] = 1.0;
			for(j=1;j<=n;j++)
			{			
				p[0] = p[1];
				p[1] = p[2];
				p[2] = ((2*j-1)*zi*p[1]-(j-1.0)*p[0])/j;
			}

			/* Newton-Raphson refinement - Continue until the
			   iteration doesn't improve much */

			oldzi = zi;
			dp = n*(zi*p[2]-p[1])/(zi*zi-1.0);
			zi = oldzi - p[2]/dp;
		} while(fabs(zi-oldzi) > TOLERANCE);
		
		/* Refinement succeeded, now transform from (-1,1) to (x1,x2) */
		x[i-1] = xavg-(dx/2.0)*zi;
		w[i-1] = dx/((1.0-zi*zi)*dp*dp);
		/* Mirror image values */
		x[n-i] = xavg+(dx/2.0)*zi;
		w[n-i] = w[i-1];
	}
}

