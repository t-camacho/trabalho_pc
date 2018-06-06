#include "incl.h"
#include <cstring>

short norm_len[NM];

int norm_length;

NORMAL *norm_address;

float nmag_epsilon;

void Compute_Normal() {
    for (long i = 0; i < NM; i++) {
        norm_len[i] = map_len[i] - 2*INSET;
    }
    norm_length = norm_len[X] * norm_len[Y] * norm_len[Z];
    Allocate_Normal(&norm_address, norm_length);

    std::cout << "Computing normal...\n";

    Normal_Compute();
}

void Allocate_Normal(NORMAL **address, long length) {
    std::cout << "Allocating normal map of " << length*sizeof(NORMAL) << " bytes\n";

    *address = (NORMAL *) malloc(length*sizeof(NORMAL));

    if(*address == NULL)
        fatal("No space available for map");

    for(long i = 0; i < length; i++) {
        *(*address+i) = 0;
    }
}

void Normal_Compute() {
    long inx, iny, inz;
    NORMAL *local_norm_address;
    double inv_mag_shift , magnitude, norm_lshift, grd_x, grd_y, grd_z;
    long zstart, zstop;
    long xnorm, ynorm, znorm, norm0;
    long xstart, xstop, ystart, ystop;

    norm0 = (long)(NORM_LSHIFT);
    norm_lshift = -NORM_LSHIFT;
    nmag_epsilon = magnitude_epsilon;
    
    zstart = 0;
    zstop = norm_len[Z];
    ystart = 0;
    ystop = norm_len[Y];
    xstart = 0;
    xstop = norm_len[X];

    for (long outz = zstart; outz < zstop; outz++) {
        for (long outy = ystart; outy < ystop; outy++) {
            for (long outx = xstart; outx < xstop; outx++) {
                inx = INSET + outx;
                iny = INSET + outy;
                inz = INSET + outz;

                grd_x = (double)((long)MAP(inz,iny,inx+1) - (long)MAP(inz,iny,inx-1));
	            grd_y = (double)((long)MAP(inz,iny+1,inx) - (long)MAP(inz,iny-1,inx));
	            grd_z = (double)((long)MAP(inz+1,iny,inx) - (long)MAP(inz-1,iny,inx));

                magnitude = grd_x*grd_x+grd_y*grd_y+grd_z*grd_z;
                local_norm_address = NORM_ADDRESS(outz,outy,outx,X);

                if (magnitude > SMALL) {
	                inv_mag_shift = norm_lshift/sqrt(magnitude);
	                if (grd_x*inv_mag_shift > 0.0) xnorm = 1;
	                else xnorm = 0;
	                ynorm = (long)(grd_y*inv_mag_shift);
	                znorm = (long)(grd_z*inv_mag_shift);
	                *local_norm_address = TADDR(znorm,ynorm,xnorm);
	            } else {
	                *local_norm_address = TADDR(norm0,2,1);
	            }
                
            }
        }
    }
}