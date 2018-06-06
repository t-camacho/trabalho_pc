#include <fcntl.h>
#include <unistd.h>

#include "incl.h"

#define	RWMODE	0

int Open_File(char filename[]) {
    int fd = 0;
    
    if ((fd = open(filename, RWMODE)) == -1) {
        fatal("Can't open file");
    }
    
    return fd;
}

void Read_Bytes(int fd, unsigned char array[], long length) {
    long n_read;
    long more_read;

    n_read = read(fd, array, MIN(length, 32766));
    if (n_read != -1 && n_read != 0) {
        while (n_read < length) {
            more_read = read(fd, &array[n_read], MIN(length-n_read, 32766));
            if (more_read == -1 || more_read == 0) break;
            n_read += more_read;
        }
    }

    if (n_read != length) {
        Close_File(fd);
        fatal("Read failed on file");
    }
}

void Read_Shorts(int fd, unsigned char array[], long length) {
  long n_read;
  long more_read;

  n_read = read(fd, array, MIN(length, 32766));

  if (n_read != -1 && n_read != 0) {
    while (n_read < length) {
        more_read = read(fd,&array[n_read], MIN(length-n_read, 32766));
        if (more_read == -1 || more_read == 0) break;
        n_read += more_read;
    }
  }

  if (n_read != length) {
      fatal("Read failed on file");
  }
}

void Read_Longs(int fd, unsigned char array[], long length) {
    long n_read;
    long more_read;

    n_read = read(fd, array, MIN(length, 32766));
    if (n_read != -1 && n_read != 0) {
        while (n_read < length) {
            more_read = read(fd, &array[n_read], MIN(length-n_read, 32766));
        if (more_read == -1 || more_read == 0) break;
            n_read += more_read;
        }
    }
    if (n_read != length) {
        fatal("Read failed on file");
    }
}

void Close_File(int fd) {
    if (close(fd) == -1) {
        fatal("Can't close file");
    }
}