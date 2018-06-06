#include "incl.h"

float invjacobian[NM][NM];
float invinvjacobian[NM][NM];

long num_rays_traced;
long num_traced_rays_hit_volume;
long num_samples_trilirped;

void Ray_Trace() {
    for (long i = 0; i < NM; i++) {
        invjacobian[X][i] = uout_invvertex[0][0][1][i] - uout_invvertex[0][0][0][i];
        invjacobian[Y][i] = uout_invvertex[0][1][0][i] - uout_invvertex[0][0][0][i];
        invjacobian[Z][i] = uout_invvertex[1][0][0][i] - uout_invvertex[0][0][0][i];
    }

    for (long i = 0; i < NM; i++) {
        for (long j = 0; j < NM; j++) {
            if (ABS(invjacobian[i][j]) > SMALL)
                invinvjacobian[i][j] = 1.0 / invjacobian[i][j];
        }
    }

    num_rays_traced = 0;
    num_traced_rays_hit_volume = 0;
    num_samples_trilirped = 0;

    Pre_Shade();
    Ray_Trace_Non_Adaptively();
}

void Ray_Trace_Non_Adaptively() {
    float foutx, fouty;
    PIXEL *pixel_address;

    // for(long outy = 0; outy < block_ylen; outy++) {
    //     for(long outx = 0; outx < block_xlen; outx++) {
    //         foutx = (float) outx;
    //         fouty = (float) outy;
    //         pixel_address = IMAGE_ADDRESS(outy, outx);
    //         Trace_Ray(foutx, fouty, pixel_address);
    //     }
    // }

    for(long outy = 0; outy < block_ylen; outy++) {
        for(long outx = 0; outx < block_xlen; outx++) {
            foutx = (float) outx;
            fouty = (float) outy;
            pixel_address = IMAGE_ADDRESS(outy, outx);
            Trace_Ray(foutx, fouty, pixel_address);
        }
    }
}