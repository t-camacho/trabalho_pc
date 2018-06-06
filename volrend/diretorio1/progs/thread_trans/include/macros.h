#pragma once

#define MIN(X,Y)	    ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y)	    ((X) > (Y) ? (X) : (Y))
#define ABS(X)		    ((X) > 0 ? (X) : -(X))
#define SIGN(X)		    ((X) > 0 ? 1 : -1)
#define NINT(X)		    (long)((X)+0.5)
#define	ROUNDDOWN(X)	(long)(X)
#define	ROUNDUP(X)	    ((X) == (long)(X) ? (long)(X) : (long)(X)+1)
#define	STEPDOWN(X)	    ((X) == (long)(X) ? (long)(X)-1 : (long)(X))
#define	STEPUP(X)	    ((long)(X)+1)				
#define NLONG(X)	    (long)((X)+0.5)				
#define	HALF(X)		    ((X)>>1)				
#define	FRACT(X)	    ((X)-(long)(X))		