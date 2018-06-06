#include <cstring>
#include "incl.h"

#define	MAP_CUR_VERSION	1

short map_version;
short orig_min[NM], orig_max[NM], orig_len[NM];
short extr_min[NM], extr_max[NM], extr_len[NM];
short map_min[NM], map_max[NM], map_len[NM];
short map_warps;
int map_length;
DENSITY *map_address;

void Load_Map(char *input, char *filename) {
    char local_filename[FILENAME_STRING_SIZE];
    int fd;

    strcpy(local_filename, "input/input_");
    strcat(local_filename, input);
    strcat(local_filename, "/");
    strcat(local_filename, filename);
    strcat(local_filename, ".den");

    fd = Open_File(local_filename);

    Read_Shorts(fd, (unsigned char *)&map_version, (long)sizeof(map_version));

    if (map_version != MAP_CUR_VERSION)
        fatal("Can't load version file");

    Read_Shorts(fd, (unsigned char *)orig_min, (long)sizeof(orig_min));
    Read_Shorts(fd, (unsigned char *)orig_max, (long)sizeof(orig_max));
    Read_Shorts(fd, (unsigned char *)orig_len, (long)sizeof(orig_len));

    Read_Shorts(fd, (unsigned char *)extr_min, (long)sizeof(extr_min));
    Read_Shorts(fd, (unsigned char *)extr_max, (long)sizeof(extr_max));
    Read_Shorts(fd, (unsigned char *)extr_len, (long)sizeof(extr_len));

    Read_Shorts(fd, (unsigned char *)map_min, (long)sizeof(map_min));
    Read_Shorts(fd, (unsigned char *)map_max, (long)sizeof(map_max));
    Read_Shorts(fd, (unsigned char *)map_len, (long)sizeof(map_len));

    Read_Shorts(fd, (unsigned char *)&map_warps, (long)sizeof(map_warps));
    Read_Longs(fd, (unsigned char *)&map_length, (long)sizeof(map_length));

    Allocate_Map(&map_address, map_length);

    Read_Bytes(fd,(unsigned char *)map_address,(long)(map_length*sizeof(DENSITY)));
    
    Close_File(fd);

    std::cout << "Loading map from .den file...\n";
}

void Allocate_Map(DENSITY **address, long length) {
    printf("Allocating density map of %ld bytes...\n", length*sizeof(DENSITY));

    *address = (DENSITY *) malloc(length * sizeof(DENSITY));

    if (*address == NULL) {
        fatal("No space available for map.");
    }else {
        for (long i = 0; i < length; i++) {
            *(*address+i) = 0;
        }
  }
}