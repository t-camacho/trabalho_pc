#include "incl.h"


long block_xlen, block_ylen;
long density_epsilon;
long magnitude_epsilon;

float density_opacity[MAX_DENSITY+1];
float magnitude_opacity[MAX_MAGNITUDE+1];

PIXEL background;

float light[NM];

float ambient_color;
float diffuse_color;
float specular_color;
float specular_exponent;

float depth_hither;
float depth_yon;
float depth_exponent;

float opacity_epsilon;
float opacity_cutoff;

// long highest_sampling_boxlen;
long lowest_volume_boxlen;

long volume_color_difference;

long pyr_highest_level;
long pyr_lowest_level;

float angle[NM];

void Init_Options(void) {
    norm_address = NULL;
    opc_address = NULL;
    pyr_address[0] = NULL;

    background = NULL_PIXEL;

    Init_Opacity();
    Init_Lighting();

    angle[X] = 90.0;
    angle[Y] = -36.0;
    angle[Z] = 0.0;

    Init_Parallelization();

    opacity_epsilon = 0.0;
    opacity_cutoff = 0.95;
    // highest_sampling_boxlen = HBOXLEN;

    lowest_volume_boxlen = 1;

    volume_color_difference = 16;
    pyr_highest_level = 5;
    pyr_lowest_level = 2;
}

void Init_Opacity(void) {
    float increment;

    density_epsilon = 96;
    magnitude_epsilon = 1;

    /* density */
    density_opacity[MIN_DENSITY] = 0.0;
    density_opacity[95] = 0.0;
    density_opacity[135] = 1.0;
    density_opacity[MAX_DENSITY] = 1.0;

    for(long i = MIN_DENSITY; i < 95; i++) {
        density_opacity[i] = 0.0;
    }

    increment = 1.0/(135.0-95.0);
    
    for(long i = 95; i < 134; i++) {
        density_opacity[i+1] = density_opacity[i] + increment;
    }

    for (long i=135; i < MAX_DENSITY; i++) {
        density_opacity[i] = 1.0;
    }

    /* magnitude */
    magnitude_opacity[MIN_MAGNITUDE] = 0.0;
    magnitude_opacity[70] = 1.0;
    magnitude_opacity[MAX_MAGNITUDE] = 1.0;

    increment = 1.0/(70.0-(float)MIN_MAGNITUDE);
    
    for(long i = MIN_MAGNITUDE; i < 69; i++) {
        magnitude_opacity[i+1] = magnitude_opacity[i] + increment;
    }

    for(long i = 70; i < MAX_MAGNITUDE-1; i++) {
        magnitude_opacity[i] = 1.0;
    }
}

void Init_Lighting(void) {
    float inv_magnitude;

    /* light */
    light[X] = 0.5;
    light[Y] = 0.7;
    light[Z] = -1.0;

    inv_magnitude = 1.0/sqrt(light[X]*light[X] + light[Y]*light[Y] +
	                light[Z]*light[Z]);

    light[X] = light[X] * inv_magnitude;
    light[Y] = light[Y] * inv_magnitude;
    light[Z] = light[Z] * inv_magnitude;

    /* others */
    ambient_color = 30.0;
    diffuse_color = 100.0;
    specular_color = 130.0;
    specular_exponent = 10.0;

    depth_hither = 1.0;
    depth_yon = 0.4;
    depth_exponent = 1.0;
}

void Init_Parallelization(void) {
    block_xlen = BLOCK_LEN;
    block_ylen = BLOCK_LEN;
}