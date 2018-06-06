#pragma once

#include "anl.h"

/* main */
extern int ROTATE_STEPS;
extern long frame;
extern PIXEL *image_address;
extern long image_len[NI]; 
extern PIXEL *shd_address;
extern GlobalMemory *Global;
extern long num_nodes;
extern long image_section[NI];

/* normal.cpp */
extern int norm_length;
extern NORMAL *norm_address;
extern float nmag_epsilon;
extern short map_len[NM];
extern short norm_len[NM];

/* opacity.cpp */
extern OPACITY *opc_address;
extern int opc_length;
extern short opc_len[NM];

/* octree.cpp */
extern BYTE *pyr_address[MAX_PYRLEVEL+1];
extern short pyr_levels;
extern short pyr_len[MAX_PYRLEVEL+1][NM];
extern short pyr_voxlen[MAX_PYRLEVEL+1][NM];	

/* option.cpp */
extern OPACITY *opc_address;
extern long density_epsilon;
extern long magnitude_epsilon;
extern PIXEL background;
extern float opacity_epsilon;
extern float density_opacity[MAX_DENSITY+1];	
extern float magnitude_opacity[MAX_MAGNITUDE+1];
extern float depth_hither;
extern float depth_yon;
extern float depth_exponent;
extern long block_xlen, block_ylen;
extern float light[NM];
extern float angle[NM];
extern long pyr_highest_level;
extern long pyr_lowest_level;
extern float opacity_cutoff;
extern float ambient_color;
extern float diffuse_color;
extern float specular_color;
extern float specular_exponent;

/* view.cpp */
extern long frust_len;
extern float transformation_matrix[4][4];
extern float out_invvertex[2][2][2][NM];
extern float uout_invvertex[2][2][2][NM];
extern long image_zlen;
extern float in_max[NM];
extern long opc_xlen, opc_xylen;
extern long norm_xlen, norm_xylen;
extern float depth_cueing[MAX_OUTLEN];

/* map.h */
extern DENSITY *map_address;

/* adaptive.cpp */
extern float invjacobian[NM][NM];
extern float invinvjacobian[NM][NM];

/* render.cpp */
extern float obslight[NM];	
extern float obshighlight[NM];

/* main */
void Frame(void);
void Init_Decomposition(void);
long WriteGrayscaleTIFF();
void Render_Loop();
long WriteGrayscaleTIFF(char *filename, long width, long height, long scanbytes, unsigned char *data);
void Lallocate_Image(PIXEL **address, long length);

/* option.cpp */
void Init_Options(void);
void Init_Opacity(void);
void Init_Lighting(void);
void Init_Parallelization(void);

/* normal.cpp */
void Load_Normal(char filename[]);
void Compute_Normal();
void Allocate_Normal(NORMAL **address, long length);
void Normal_Compute();

/* file.cpp */
int Open_File(char filename[]);
void Read_Shorts(int fd, unsigned char array[], long length);
void Read_Longs(int fd, unsigned char array[], long length);
void Read_Bytes(int fd, unsigned char array[], long length);
void Close_File(int fd);

/* map.cpp */
void Load_Map(char *input, char *filename);
void Allocate_Map(DENSITY **address, long length);
void Allocate_Shading_Table(PIXEL **address1, long length);
void Allocate_Image(PIXEL **address, long length);
void Deallocate_Map(DENSITY **address);

/* opacity.cpp */
void Compute_Opacity();
void Allocate_Opacity(OPACITY **address, long length);
void Opacity_Compute();

/* view.cpp */
void Compute_Pre_View();

void Transform_Point(float xold, float yold, float zold, 
                     float *xnew, float *ynew, float *znew);

void Select_View(float delta_angle, int axis);

void Load_Identity_Matrix(float matrix[4][4]);

void Inverse_Concatenate_Translation(float matrix[4][4],
                                     float xoffset, 
                                     float yoffset, 
                                     float zoffset);

void Load_Translation_Matrix(float matrix[4][4],
                             float xoffset,
                             float yoffset,
                             float zoffset);

void Inverse_Concatenate_Transform(float composite_matrix[][4],
                                   float transformation_matrix[][4]);

void Multiply_Matrices(float input_matrix1[][4],
                       float input_matrix2[][4],
                       float output_matrix[][4]);

void Copy_Matrix(float input_matrix[][4],
                 float output_matrix[][4]);

void Inverse_Concatenate_Scaling(float matrix[4][4],
                                 float xscale,
                                 float yscale,
                                 float zscale);

void Load_Scaling_Matrix(float matrix[4][4],
                         float xscale,
                         float yscale,
                         float zscale);

void Load_Rotation_Matrix(float matrix[4][4], float angle, long axis);
void Inverse_Concatenate_Rotation(float matrix[4][4], float angle, long axis);

void Load_Transformation_Matrix(float matrix[4][4]);

void Compute_Input_Dimensions();

void Compute_Input_Unit_Vector();

/* octree.cpp */
void Compute_Octree();
void Allocate_Pyramid_Level(BYTE **address, long length);
void Compute_Base();
void Or_Neighbors_In_Base();
void Compute_Pyramid_Level(long level);

/* render.cpp */
void Render(long my_node);
void Observer_Transform_Light_Vector();
void Compute_Observer_Transformed_Highlight_Vector();

/* adaptive.cpp */
void Ray_Trace(long my_node);
void Ray_Trace_Non_Adaptively(long my_node);

/* raytrace.cpp */
void Pre_Shade(long my_node);
void Trace_Ray(float foutx, float fouty, PIXEL *pixel_address);