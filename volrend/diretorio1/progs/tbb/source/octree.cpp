#include "incl.h"

#define WRITE_PYR(IBIT,ILEVEL,IZ,IY,IX)\
				(PYR_ADDRESS(ILEVEL,IZ,IY,IX),\
				 *pyr_address2|=(IBIT)<<pyr_offset2)

short pyr_levels;
short pyr_len[MAX_PYRLEVEL+1][NM];
short pyr_voxlen[MAX_PYRLEVEL+1][NM];	

int pyr_length[MAX_PYRLEVEL+1];
BYTE *pyr_address[MAX_PYRLEVEL+1];

long pyr_offset1, pyr_offset2;
BYTE *pyr_address2;

void Compute_Octree() {
    long max_len = 0;
    
    for (long i = 0; i < NM; i++) {
        max_len = MAX(max_len, opc_len[i]);
    }
    pyr_levels = 1;
    while ((1 << (pyr_levels-1)) < max_len) {
        pyr_levels++;        
    }

    std::cout << "Computing binary pyramid of " << pyr_levels << " levels...\n";

    for (long i = 0; i < NM; i++) {
        pyr_len[0][i] = opc_len[i];
        pyr_voxlen[0][i] = 1;
    }

    pyr_length[0] = (pyr_len[0][X]*pyr_len[0][Y]*pyr_len[0][Z]+7)/8;
    Allocate_Pyramid_Level(&pyr_address[0], pyr_length[0]);

    std::cout << "Computing octree base...\n";

    Compute_Base();

    std::cout << "Performing OR of eight neighbors in binary mask...\n";

    Or_Neighbors_In_Base();

    for (long level = 1; level < pyr_levels; level++) {
        for (long i = 0; i<NM; i++) {
            if (pyr_len[level-1][i] > 1) {
	            pyr_len[level][i] = (pyr_len[level-1][i]+1)>>1;
	            pyr_voxlen[level][i] = pyr_voxlen[level-1][i]<<1;
            } else {
	            pyr_len[level][i] = pyr_len[level-1][i];
	            pyr_voxlen[level][i] = pyr_voxlen[level-1][i];
            }
        }
        pyr_length[level] = (pyr_len[level][X]*
                            pyr_len[level][Y]*pyr_len[level][Z]+7)/8;
        Allocate_Pyramid_Level(&pyr_address[level], pyr_length[level]);
        Compute_Pyramid_Level(level);
    }
}

void Allocate_Pyramid_Level(BYTE **address, long length) {
    std::cout << "Allocating pyramid level of " << length*sizeof(BYTE) << " bytes...\n";

    *address = (BYTE *) malloc(length*sizeof(BYTE));

    if (*address == NULL)
        fatal("No space available for pyramid level.");

    for (long i = 0; i < length; i++) {
        *(*address+i) = 0;
    }
}

void Compute_Base() {
    long outx, outy, outz;
    long zstart, zstop;
    long xstart, xstop, ystart, ystop;

    zstart = 0;
    zstop = pyr_len[0][Z];
    ystart = 0;
    ystop = pyr_len[0][Y];
    xstart = 0;
    xstop = pyr_len[0][X];

    for(outz=zstart; outz<zstop; outz++) {
        for(outy=ystart; outy<ystop; outy++) {
            for(outx=xstart; outx<xstop; outx++) {
                if(OPC(outz, outy, outx) == 0)
	                WRITE_PYR(0, 0, outz, outy, outx);
	            else {
	                WRITE_PYR(1, 0, outz, outy, outx);
                }
            }
        }
    }
}

void Or_Neighbors_In_Base() {
    long outx,outy,outz;
    long outx_plus_one,outy_plus_one,outz_plus_one;
    BOOLEAN bit;
    long zstart,zstop;

    zstart = 0;
    zstop = pyr_len[0][Z];

    for (outz = zstart; outz < zstop; outz++) {
        outz_plus_one = MIN(outz+1,pyr_len[0][Z]-1);
        for (outy = 0; outy < pyr_len[0][Y]; outy++) {
            outy_plus_one = MIN(outy+1,pyr_len[0][Y]-1);
            for (outx = 0; outx < pyr_len[0][X]; outx++) {
	            outx_plus_one = MIN(outx+1,pyr_len[0][X]-1);

                bit = PYR(0,outz,outy,outx);
                bit |= PYR(0,outz,outy,outx_plus_one);
                bit |= PYR(0,outz,outy_plus_one,outx);
                bit |= PYR(0,outz,outy_plus_one,outx_plus_one);
                bit |= PYR(0,outz_plus_one,outy,outx);
                bit |= PYR(0,outz_plus_one,outy,outx_plus_one);
                bit |= PYR(0,outz_plus_one,outy_plus_one,outx);
                bit |= PYR(0,outz_plus_one,outy_plus_one,outx_plus_one);

                WRITE_PYR(bit,0,outz,outy,outx);
            }
        }
    }
}

void Compute_Pyramid_Level(long level) {
    long outx,outy,outz;
    long inx,iny,inz;
    long inx_plus_one,iny_plus_one,inz_plus_one;
    BOOLEAN bit;

    std::cout << "Computing pyramid level " << level << " from " << level-1 
                    << " level...\n";

    for (outz = 0; outz < pyr_len[level][Z]; outz++) {
        inz = outz << 1;
        inz_plus_one = MIN(inz+1,pyr_len[level-1][Z]-1);
        for (outy = 0; outy < pyr_len[level][Y]; outy++) {
            iny = outy << 1;
            iny_plus_one = MIN(iny+1,pyr_len[level-1][Y]-1);
            for (outx = 0; outx < pyr_len[level][X]; outx++) {
                inx = outx<<1;
	            inx_plus_one = MIN(inx+1,pyr_len[level-1][X]-1);

                bit = PYR(level-1,inz,iny,inx);
                bit |= PYR(level-1,inz,iny,inx_plus_one);
                bit |= PYR(level-1,inz,iny_plus_one,inx);
                bit |= PYR(level-1,inz,iny_plus_one,inx_plus_one);
                bit |= PYR(level-1,inz_plus_one,iny,inx);
                bit |= PYR(level-1,inz_plus_one,iny,inx_plus_one);
                bit |= PYR(level-1,inz_plus_one,iny_plus_one,inx);
                bit |= PYR(level-1,inz_plus_one,iny_plus_one,inx_plus_one);

                WRITE_PYR(bit, level, outz, outy, outx);
            }
        }
    }
}