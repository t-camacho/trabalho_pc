#include "incl.h"

short opc_len[NM];

int opc_length;

OPACITY *opc_address;

void Compute_Opacity() {
    for (long i = 0; i < NM; i++) {
        opc_len[i] = map_len[i] - 2*INSET;
    }

    opc_length = opc_len[X] * opc_len[Y] * opc_len[Z];

    Allocate_Opacity(&opc_address, opc_length);

    std::cout << "Computing opacity map...\n";

    Opacity_Compute();
}

void Allocate_Opacity(OPACITY **address, long length) {
    std::cout << "Allocating opacity map of " << length*sizeof(OPACITY) << " bytes\n";

    *address = (OPACITY *) malloc(length*sizeof(OPACITY));

    if (*address == NULL)
        fatal("No space available for map.\n");

    for (long i = 0; i < length; i++) {
        *(*address+i) = 0;
    }
}

void Opacity_Compute() {
    long inx, iny, inz;
    long outx, outy, outz;
    long density;
    float magnitude;
    float opacity, grd_x, grd_y, grd_z;
    long zstart, zstop;
    long xstart, xstop, ystart, ystop;

    zstart = 0;
    zstop = opc_len[Z];
    ystart = 0;
    ystop = opc_len[Y];
    xstart = 0;
    xstop = opc_len[X];

    for (outz=zstart; outz<zstop; outz++) {
        for (outy=ystart; outy<ystop; outy++) {
            for (outx=xstart; outx<xstop; outx++) {
                inx = INSET + outx;
                iny = INSET + outy;
                inz = INSET + outz;

                density = MAP(inz,iny,inx);
                if (density > density_epsilon) {
                    grd_x = (float)((long)MAP(inz,iny,inx+1) - (long)MAP(inz,iny,inx-1));
                    grd_y = (float)((long)MAP(inz,iny+1,inx) - (long)MAP(inz,iny-1,inx));
                    grd_z = (float)((long)MAP(inz+1,iny,inx) - (long)MAP(inz-1,iny,inx));
                    magnitude = grd_x*grd_x+grd_y*grd_y+grd_z*grd_z;

                    if (magnitude > nmag_epsilon) {
                        magnitude = .5*sqrt(magnitude);
                        opacity = density_opacity[density] *
                        magnitude_opacity[(long)magnitude];

                    if (opacity > opacity_epsilon)
                        OPC(outz,outy,outx) = NINT(opacity*MAX_OPC);
                    }else {
	                    OPC(outz,outy,outx) = MIN_OPC;
                    }
                }else {
                    OPC(outz,outy,outx) = MIN_OPC;
                }
            }
        }
    }
}