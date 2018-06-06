#include <cstring>
#include <unistd.h>
#include "incl.h"
#include "tiffio.h"
#include "sys/wait.h"

int ROTATE_STEPS;

GlobalMemory *Global;

long image_section[NI];
long voxel_section[NM];

long num_nodes = 2, frame;

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

    while((opt = getopt(argc, argv, "hi:n:")) > 0) {
        switch(opt) {
            case 'h':
                usage();
                break;
            case 'i':
                std::strcpy(input, optarg);
                break;
            case 'n':
                num_nodes = std::strtol(optarg, NULL, 10);
                break;
        }
    }

    if(!std::strcmp(input, "test")) {
        std::strcpy(filename, "head-scaleddown4");
        ROTATE_STEPS = 4;
    }else if(!std::strcmp(input, "simdev")) {
        std::strcpy(filename, "head-scaleddown4");
        ROTATE_STEPS = 4;
    }else if(!std::strcmp(input, "simsmall")) {
        std::strcpy(filename, "head-scaleddown4");
        ROTATE_STEPS = 20;
    }else if(!std::strcmp(input, "simmedium")) {
        std::strcpy(filename, "head-scaleddown2");
        ROTATE_STEPS = 50;
    }else if(!std::strcmp(input, "simlarge")) {
        std::strcpy(filename, "head-scaleddown2");
        ROTATE_STEPS = 100;
    }else if(!std::strcmp(input, "native")) {
        std::strcpy(filename, "head");
        ROTATE_STEPS = 1000;
    }else {
        fatal("invalid input");
    }

    Frame();

    return 0;
}

void Frame() {
    pthread_barrierattr_t  barrier_attr;
    int shm_id, shm_id_img, shm_id_shd;

    Init_Options();
    Init_Decomposition();

    shm_id = shmget(IPC_PRIVATE, sizeof(GlobalMemory), IPC_CREAT|IPC_EXCL|0600);
    Global = (GlobalMemory*) shmat(shm_id, 0, 0);

    (void) pthread_barrierattr_init(&barrier_attr);
    (void) pthread_barrierattr_setpshared(&barrier_attr, PTHREAD_PROCESS_SHARED);

    sem_init(&Global->IndexLock, 1, 1);
    sem_init(&Global->CountLock, 1, 1);
    pthread_barrier_init(&Global->SlaveBarrier, &barrier_attr, num_nodes);
    pthread_barrier_init(&Global->TimeBarrier, &barrier_attr, num_nodes);
    for(long i = 0; i < MAX_NUMPROC+1; i++) {
        sem_init(&Global->QLock[i], 1, 1);
    }

    Load_Map(input, filename);
    Compute_Normal();
    Compute_Opacity();
    Compute_Pre_View();

    shd_length = LOOKUP_SIZE;

    shm_id_shd = shmget(IPC_PRIVATE, sizeof(PIXEL)*shd_length, IPC_CREAT|IPC_EXCL|0600);
    shd_address = (PIXEL*)shmat(shm_id_shd, 0, 0);
    Allocate_Shading_Table(&shd_address, shd_length);

    /* allocate space for image */
    image_len[X] = frust_len;
    image_len[Y] = frust_len;
    image_length = image_len[X] * image_len[Y];

    shm_id_img = shmget(IPC_PRIVATE, sizeof(PIXEL)*image_length, IPC_CREAT|IPC_EXCL|0600);
    image_address = (PIXEL*)shmat(shm_id_img, 0, 0);
    Allocate_Image(&image_address, image_length);

    num_xblocks = ROUNDUP((float)image_len[X]/(float)block_xlen);
    num_yblocks = ROUNDUP((float)image_len[Y]/(float)block_ylen);
    num_blocks = num_xblocks * num_yblocks;
    Lallocate_Image(&image_block,block_xlen*block_ylen);

    Compute_Octree();
    // Deallocate_Map(&map_address);

    Global->Index = NODE0;
    Global->Counter = 0;

    std::cout << "\nRendering...\n\n";

    for(long i = 0; i < num_nodes; i++) {
        if(fork() == 0) {
            Render_Loop();
            exit(0);
        }
    }
    wait(NULL);
}

void Render_Loop() {
    PIXEL *local_image_address;
    char outfile[FILENAME_STRING_SIZE];
    long image_partition;
    float inv_num_nodes;
    long my_node;

    sem_wait(&Global->IndexLock);
    my_node = Global->Index++;
    sem_post(&Global->IndexLock);
    my_node = my_node % num_nodes;

    inv_num_nodes = 1.0/(float)num_nodes;
    image_partition = ROUNDUP(image_length*inv_num_nodes);

    #ifdef DIM
    int dim;
    for (dim = 0; dim < NM; dim++) {
    #endif

    for (long step = 0; step < ROTATE_STEPS; step++) {
        frame = step;

        /* initialize images here */
        local_image_address = image_address + image_partition * my_node;

        pthread_barrier_wait(&Global->SlaveBarrier);
        if (my_node == num_nodes-1) {
            for (long i = image_partition*my_node; i < image_length; i++)
                *local_image_address++ = 0;
        } else {
            for (long i = 0; i < image_partition; i++)
                *local_image_address++ = 0;
        }

        // if (my_node == ROOT) {
            #ifdef DIM
            Select_View((float)STEP_SIZE, dim);
            #else
            Select_View((float)STEP_SIZE, Y);
            #endif
        // }

        pthread_barrier_wait(&Global->SlaveBarrier);

        Global->Counter = num_nodes;
        Global->Queue[num_nodes][0] = num_nodes;
        Global->Queue[my_node][0] = 0;

        Render(my_node);

        if (my_node == ROOT) {
            #ifdef DIM
            sprintf(outfile, "output/%s_%ld.tiff",filename, 1000+dim*ROTATE_STEPS+step);
            #else
            sprintf(outfile, "output/%s_%ld.tiff",filename, 1000+step);
            #endif
            WriteGrayscaleTIFF(outfile, image_len[X],image_len[Y],image_len[X], image_address);
        }
    }
    #ifdef DIM
    }
    #endif
}

void Allocate_Shading_Table(PIXEL **address1, long length) {
    std::cout << "Allocating shade lookup table of " << length*sizeof(PIXEL)
              << " bytes...\n";

    // *address1 = (PIXEL *) malloc(length * sizeof(PIXEL));

    if (*address1 == NULL)
        fatal("No space available for table.");

    for (long i = 0; i < length; i++) {
        *(*address1+i) = 0;
    }
}

void Allocate_Image(PIXEL **address, long length) {
    std::cout << "Allocating image of " << length*sizeof(PIXEL) << " bytes\n";

    // *address = (PIXEL *) malloc(length*sizeof(PIXEL));

    if (*address == NULL)
	    fatal("No space available for image.");

    for (long i = 0; i < length; i++) {
        *(*address+i) = 0;
    }
}

void Lallocate_Image(PIXEL **address, long length) {
    std::cout << "Allocating image of " << length*sizeof(PIXEL) << " bytes...\n";
    *address = (PIXEL *) calloc(length,sizeof(PIXEL));
    if (*address == NULL)
        fatal("No space available for image.");
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

    return 1;
}

void Init_Decomposition() {
    long factors[MAX_NUMPROC];
    double processors, newfactor = 0;
    long sq, cu, maxcu = 140729450987608, count;

    count = 1;
    processors = (double) num_nodes;

    sq = (long)sqrt(processors);
    cu = (long)pow(processors,1.0/3.0);
    factors[0] = 1;

    for (long i = 2; i < sq; i++) {
        // printf("--> %ld\n", i);
      if (FRACT(processors/(double)i) == 0.0) {
        factors[count++] = i;
        if (i <= cu) {
          maxcu = i;
          newfactor = (double)(num_nodes/i);
        }
      }
    }
    count--;
    image_section[X] = factors[count];
    image_section[Y] = num_nodes/factors[count];


    sq = (long)sqrt(newfactor);
    count = 1;

    for (long i = 2; i < sq; i++) {
      if (FRACT(newfactor/(double)i) == 0.0)
        factors[count++] = i;
    }

    count--;

    voxel_section[X] = MIN(factors[count],maxcu);
    voxel_section[Y] = MAX(factors[count],maxcu);
    voxel_section[Z] = (long)newfactor/factors[count];
}
