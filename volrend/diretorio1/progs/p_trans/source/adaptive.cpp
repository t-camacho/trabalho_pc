#include "incl.h"

float invjacobian[NM][NM];
float invinvjacobian[NM][NM];

long num_rays_traced;
long num_traced_rays_hit_volume;
long num_samples_trilirped;

void Ray_Trace(long my_node) {
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

    pthread_barrier_wait(&Global->TimeBarrier);
    Pre_Shade(my_node);
    __transaction_atomic {
        Global->Counter--;
    }
    // while (Global->Counter);
    pthread_barrier_wait(&Global->TimeBarrier);
    // printf("--> %ld %ld\n", Global->Counter, my_node);        
    Ray_Trace_Non_Adaptively(my_node);
    pthread_barrier_wait(&Global->TimeBarrier);
}

void Ray_Trace_Non_Adaptively(long my_node) {
    long outx, outy, xindex, yindex;
    float foutx, fouty;
    PIXEL *pixel_address;

    // long num_xqueue, num_yqueue, num_queue, lnum_xblocks, lnum_yblocks, lnum_blocks;
    long num_xqueue, num_yqueue, lnum_xblocks, lnum_yblocks, lnum_blocks;
    long xstart, xstop, ystart, ystop, local_node, work;

    num_xqueue = ROUNDUP((float)image_len[X]/(float)image_section[X]);
    num_yqueue = ROUNDUP((float)image_len[Y]/(float)image_section[Y]);
    // num_queue = num_xqueue * num_yqueue;
    lnum_xblocks = ROUNDUP((float)num_xqueue/(float)block_xlen);
    lnum_yblocks = ROUNDUP((float)num_yqueue/(float)block_ylen);
    lnum_blocks = lnum_xblocks * lnum_yblocks;
    local_node = my_node;

    Global->Queue[local_node][0] = 0;

    while (Global->Queue[num_nodes][0] > 0) {
        xstart = (local_node % image_section[X]) * num_xqueue;
        xstop = MIN(xstart + num_xqueue, image_len[X]);
        ystart = (local_node / image_section[X]) * num_yqueue;
        ystop = MIN(ystart+num_yqueue, image_len[Y]);

        __transaction_atomic {
            work = Global->Queue[local_node][0]++;
        }
        
        while (work < lnum_blocks) {
            xindex = xstart + (work%lnum_xblocks)*block_xlen;
            yindex = ystart + (work/lnum_xblocks)*block_ylen;
            for (outy=yindex; outy<yindex+block_ylen && outy<ystop; outy++) {
                for (outx=xindex; outx<xindex+block_xlen && outx<xstop; outx++) {
                    foutx = (float)(outx);
                    fouty = (float)(outy);
                    pixel_address = IMAGE_ADDRESS(outy,outx);
                    Trace_Ray(foutx, fouty, pixel_address);
                }
            }
            __transaction_atomic {
                work = Global->Queue[local_node][0]++;
            }
        }
        if (my_node == local_node) {
            __transaction_atomic {
                Global->Queue[num_nodes][0]--;
            }
        }
        local_node = (local_node+1)%num_nodes;
    //     /*while (Global->Queue[local_node][0] >= lnum_blocks && Global->Queue[num_nodes][0] > 0) {
    //         local_node = (local_node+1)%num_nodes; 
    //     }*/
    }
}