#include <cstring>
#include <unistd.h>

#include "incl.h"
#include "tiffio.h"

int ROTATE_STEPS = 4;

long image_section[NI];
long voxel_section[NM];

long frame;

PIXEL *shd_address;
PIXEL *image_address;
MPIXEL *mask_image_address;
PIXEL *image_block, *mask_image_block;

long shd_length;
long num_blocks, num_xblocks, num_yblocks;
long image_len[NI], mask_image_len[NI];
int  image_length;
long mask_image_length;

char filename[FILENAME_STRING_SIZE];
char input[INPUT_DIR_SIZE] = "test";

int main(int argc, char **argv) {
    int opt;

    while((opt = getopt(argc, argv, "hi:")) > 0) {
        switch(opt) {
            case 'h':
                usage();
                break;
            case 'i':
                std::strcpy(input, optarg);
                break;
        }
    }

    if(!std::strcmp(input, "test")) {
        ROTATE_STEPS = 4;
        std::strcpy(filename, "head-scaleddown4"); //4
    }else if(!std::strcmp(input, "simdev")) {
        std::strcpy(filename, "head-scaleddown4"); //4
        ROTATE_STEPS = 4;
    }else if(!std::strcmp(input, "simsmall")) {
        std::strcpy(filename, "head-scaleddown4"); //20
        ROTATE_STEPS = 20;        
    }else if(!std::strcmp(input, "simmedium")) {
        std::strcpy(filename, "head-scaleddown2"); //50
        ROTATE_STEPS = 50;        
    }else if(!std::strcmp(input, "simlarge")) {
        std::strcpy(filename, "head-scaleddown2"); //100
        ROTATE_STEPS = 100;        
    }else if(!std::strcmp(input, "native")) {
        std::strcpy(filename, "head");             //1000
        ROTATE_STEPS = 1000;        
    }else {
        fatal("invalid input");
    }

    Frame();

    return 0;
}

void Frame() {
    Init_Options();
    Init_Decomposition();
    Load_Map(input, filename);
    Compute_Normal();
    Compute_Opacity();
    Compute_Pre_View();

    shd_length = LOOKUP_SIZE;
    
    Allocate_Shading_Table(&shd_address, shd_length);

    /* allocate space for image */
    image_len[X] = frust_len;
    image_len[Y] = frust_len;
    image_length = image_len[X] * image_len[Y];
    
    Allocate_Image(&image_address, image_length);

    block_xlen = image_len[X];
    block_ylen = image_len[Y];
    num_blocks = 1;
    num_xblocks = 1;
    num_yblocks = 1;
    image_block = image_address;

    Compute_Octree();
    Deallocate_Map(&map_address);

    std::cout << "\nRendering...\n\n";

    Render_Loop();
}

void Render_Loop() {
    PIXEL *local_image_address;
    long image_partition;
    char outfile[FILENAME_STRING_SIZE];
    float inv_num_nodes = 1.0;
    
    image_partition = ROUNDUP(image_length*inv_num_nodes);

    #ifdef DIM
    int dim;
    for (dim = 0; dim < NM; dim++) {
    #endif

    for (long step = 0; step < ROTATE_STEPS; step++) {
        frame = step;
        
        /* initialize images here */
        local_image_address = image_address + image_partition * 0;
        for (long i = 0; i < image_length; i++) {
            *local_image_address++ = background;
        }
        #ifdef DIM
	    Select_View((float)STEP_SIZE, dim);
        #else
        Select_View((float)STEP_SIZE, Y);
        #endif
        Render();
        #ifdef DIM
	    sprintf(outfile, "output/%s_%ld.tiff",filename, 1000+dim*ROTATE_STEPS+step);
        #else
	    sprintf(outfile, "output/%s_%ld.tiff",filename, 1000+step);
        #endif
        WriteGrayscaleTIFF(outfile, image_len[X],image_len[Y],image_len[X], image_address);
    }
    #ifdef DIM
    }
    #endif
}

void Allocate_Shading_Table(PIXEL **address1, long length) {
    std::cout << "Allocating shade lookup table of " << length*sizeof(PIXEL) 
              << " bytes...\n";

    *address1 = (PIXEL *) malloc(length * sizeof(PIXEL));

    if (*address1 == NULL)
        fatal("No space available for table.");

    for (long i = 0; i < length; i++) {
        *(*address1+i) = 0;
    }
}

void Allocate_Image(PIXEL **address, long length) {
    std::cout << "Allocating image of " << length*sizeof(PIXEL) << " bytes\n";

    *address = (PIXEL *) malloc(length*sizeof(PIXEL));

    if (*address == NULL)
	    fatal("No space available for image.");

    for (long i = 0; i < length; i++) {
        *(*address+i) = 0;
    }
}

long WriteGrayscaleTIFF(char *filename, long width, long height, long scanbytes, unsigned char *data) {
    double factor;
    unsigned long cmap[256];
    
    TIFF *outimage;

    factor = (double)((1 << 16) - 1) / (double)((1 << 8) - 1);
    for (long c = 0; c < 256; c++)
        cmap[c] = (long)(c * factor);

    if ((outimage = TIFFOpen(filename, "w")) == NULL)
        fatal("tiff");

    TIFFSetField(outimage, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(outimage, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(outimage, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(outimage, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(outimage, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(outimage, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
    TIFFSetField(outimage, TIFFTAG_ORIENTATION, ORIENTATION_BOTLEFT);
    TIFFSetField(outimage, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE);
    TIFFSetField(outimage, TIFFTAG_COLORMAP, cmap, cmap, cmap);

    for (long y = 0; y < height; y++) {
        if (!TIFFWriteScanline(outimage, data, y, 0)) {
            TIFFClose(outimage);
            return(0);
        }
        data += scanbytes;
    }

    TIFFClose(outimage);

    // for(long i = 0; i < image_length; i++) {
    //     printf("%i ", data[i]);
    // }
    // puts("\n\n");

    return 1;
}

void Init_Decomposition() {
    image_section[X] = 1;
    image_section[Y] = 1;
    voxel_section[X] = 1;
    voxel_section[Y] = 1;
    voxel_section[Z] = 1;
}
