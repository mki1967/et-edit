/**
 *
 *    et-edit  EDGE TRIANGLE THREE-DIMENSIONAL EDITOR
 * 
 *    Copyright (C) 2003  Marcin Kik
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *    The author can be reached at mki1967@gmail.com and https://github.com/mki1967 
 *       
 *
 **/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define  XK_MISCELLANY
#define  XK_LATIN1
#include <X11/keysymdef.h>

#include <GL/gl.h>
#include <GL/glx.h>





 /* /// PROGRAM VARIABLES AND FUNCTIONS //////////////////////// */

 /* /////// X / GLX VARIABLES ///////////// */

Display *display;
XVisualInfo *xvisualinfo_array;
int xvisualinfo_nitems;
Window window;
Window terminal;

XSetWindowAttributes windowattributes;


GLXContext glxcontext;


 /* ////// PROGRAM INTERNAL VARIABLES */


float pixel_size=0.028;
float screen_distance= 70.0;
float clip_min_limit=0.0001;
float clip_min= 1.0;
float clip_max= 200.0;

int screen_width=600, screen_height=600;

char *program_name="E.T. EDITOR";

#define MAX_FILE_NAME 200
char file_name[MAX_FILE_NAME+5]="noname.et";
char merge_file_name[MAX_FILE_NAME+5]="noname.et";




 /* //// SCREEN VARIABLES */

 /*  float rwidth, rheight;  */

struct Screen
{
  float distance;
  float clip_min, clip_max;
  int width, height;
};

struct Screen screen;


 /* // FRUSTUM VARIABLES (computed by setfrustum() ) */

float frustum_left, frustum_right,frustum_bottom, frustum_top,
  frustum_near_val, frustum_far_val;


double scale=1.0;

 /* // STEREO VARIABLES */

Bool stereo_mode=False;
float stereo_eye_distance= 7.0;
float stereo_left_color[3]={0.6,0,0};
GLboolean stereo_left_mask[3]={GL_TRUE, GL_FALSE, GL_FALSE};
float stereo_right_color[3]={0,0,1};
GLboolean stereo_right_mask[3]={GL_FALSE, GL_TRUE, GL_TRUE};
float stereo_background[3]={0,0,0};

 /* / PCL VARIABLES */

char pcl_file_name[]="stereo.pcl";

float *pcl_red_line1=NULL;
float *pcl_green_line1=NULL;
float *pcl_blue_line1=NULL;

float *pcl_red_line2=NULL;
float *pcl_green_line2=NULL;
float *pcl_blue_line2=NULL;

double pcl_darkness=0.7;



 /* // TRANSFORMATION VARIABLES */

struct Transformation 
{
  float mov[16];
  double M[16], R[16];
  
};


struct Transformation transformation;


 /* // CLIPPING VARIABLES */

struct Clipping_xyz
{
  float xmin;
  Bool xmin_flag;
  float xmax;
  Bool xmax_flag;

  float ymin;
  Bool ymin_flag;
  float ymax;
  Bool ymax_flag;

  float zmin;
  Bool zmin_flag;
  float zmax;
  Bool zmax_flag;
};

struct Clipping_xyz clipping_xyz;

float clipping_cube_size=25.0;


 /* /// COLOR VARIABLES */

#define COLOR_MAX 12

float color[COLOR_MAX][3] =
  {
    { 1.0, 1.0, 1.0},
    { 1.0, 0.0, 0.0},
    { 0.0, 1.0, 0.0},
    { 0.0, 0.0, 1.0},
    { 1.0, 1.0, 0.0},
    { 1.0, 0.0, 1.0},
    { 0.0, 1.0, 1.0},
    { 1.0, 0.5, 0.5},
    { 0.5, 1.0, 0.5},
    { 0.5, 0.5, 1.0},
    { 0.5, 0.5, 0.5},
    { 0.0, 0.0, 0.0},
  };

int background= COLOR_MAX-1;
int current_color=0;


 /* // LIGHT VARIABLES */

float basic_light[16]= {0.0, 0.0, 1.0, 0.0};
float light[16]= {0.0, 0.0, 1.0, 0.0};
Bool two_lights = True;


 /* // VERTEX VARIABLES */

#define VERTEX_MAX 10000

int vertex_used[VERTEX_MAX];
float vertex[VERTEX_MAX][3];


 /* /// EDGE VARIABLES */

#define EDGE_MAX 20000

int edge[EDGE_MAX][3]; 
 /*  int edge_color[EDGE_MAX]; // moved to last coordinate of edge */
int edge_top=0;


 /* // TRIANGLE VARIABLES */

#define TRIANGLE_MAX 20000

int triangle[TRIANGLE_MAX][4]; 
 /*  int triangle_color[TRIANGLE_MAX]; // moved to last coordinate of triangle */
float triangle_normal[TRIANGLE_MAX][3];
int triangle_top=0;


 /* // GROUP VARIABLES */

int group[VERTEX_MAX];
int group_current=0;
int group_marker=-1;
Bool group_restricted=False;


 /* // CURSOR VARIABLES */

float cursor[16]={ 0.0, 0.0, 0.0, 1.0 };

#define  CURSOR_VERTEX_MAX 7


float cursor_vertex[CURSOR_VERTEX_MAX][3];

float cursor_vertex_template[CURSOR_VERTEX_MAX][3] = 
  {
    {-0.5,  0.0,  0.0},
    { 0.5,  0.0,  0.0},
    { 0.0, -0.5,  0.0},
    { 0.0,  0.5,  0.0},
    { 0.0,  0.0, -0.5},
    { 0.0,  0.0,  0.5},
    { 0.25,  0.5,  0.0},
  };

#define CURSOR_EDGE_TOP  4

int cursor_edge[CURSOR_EDGE_TOP][3] = { {0,1,0}, {2,3,0}, {4,5,0}, {3,6,0} };
 /*  int cursor_edge_color[CURSOR_EDGE_TOP]= {0,0,0}; */

float cursor_step=1.0, cursor_min_step=0.0001, cursor_max_step=10000;


 /* // EDITOR VARIABLES */

float move_step=1.0, rotate_step=5.0;

enum  Context
  { 
    context_default, 
    context_et0,
    context_t1 
  };

enum Context context=context_default;


float  et0_marker[3], t1_marker[3];  /*  for storing endpoits of new edge or triangle */

 /* / CALLBACK */

enum Keymode
  {
    key_default,
    key_F1,
    key_F2,
    key_F3,
    key_F4,
    key_F5,
    key_F6,
    key_F7

  };

enum Keymode keymode=key_default;


 /* / LOAD/SAVE VARIABLES */

char transformation_label[]="TRANSFORMATION";
char light_label[]="LIGHT";
char background_label[]="BACKGROUND";
char cursor_label[]="CURSOR";
char group_label[]="GROUPS";



 /* // EXPORT FUNCTION VARIABLE */

int (*export_last_ptr)();

 /* / POVRAY VARIABLES */

char povray_name[MAX_FILE_NAME+5]="povname";
float povray_light_distance=100000.0;
float povray_ambient=0.3;
float povray_diffuse=0.7;
 /*  float povray_edge_ambient=1; */
 /*  float povray_edge_diffuse=0; */
float povray_light_red=1.0;
float povray_light_green=1.0;
float povray_light_blue=1.0;


float povray_edge_radius=0.017;
int povray_counter=0;



 /* // GRAPH VARIABLES */

int graph_first_nbr[VERTEX_MAX];
int graph_nbr[2*EDGE_MAX+6*TRIANGLE_MAX];
int graph_next_nbr[2*EDGE_MAX+6*TRIANGLE_MAX];
int graph_next_free;

int graph_queue[VERTEX_MAX];
int graph_queue_first, graph_queue_size;

int graph_marked[VERTEX_MAX];


 /* // BACKUP AND UNDO VARIABLES */

Bool was_modified=False;

Bool backup_was_modified=False;


float backup_vertex[VERTEX_MAX][3];
int   backup_vertex_used[VERTEX_MAX];
int   backup_group[VERTEX_MAX];
int   backup_group_current;
int   backup_group_marker;
Bool  backup_group_restricted;

int   backup_edge[EDGE_MAX][3];
int   backup_edge_top;

int   backup_triangle[TRIANGLE_MAX][4];
int   backup_triangle_top;

struct Transformation backup_transformation;
float backup_cursor[3];
float backup_cursor_step;

float backup_light[16];
Bool  backup_two_lights;

int backup_background= COLOR_MAX-1;
int backup_current_color=0;


enum Context backup_context;
float backup_et0_marker[3], backup_t1_marker[3];
float backup_move_step;




 /* // POINTS */

#define POINT_MAX 25

float point[POINT_MAX][3];


 /* // REGULAR POLYGON */

#define POLYGON_MAX 720
#define POLYGON_MIN 3

int   polygon_n=POLYGON_MIN;


 /* // REDUCTION */

double reduction_epsilon=0.0001;








 /* ////////////  FUNCTIONS */



 /* // XTERMINAL INTERACTION */

void get_file_name(char *file_name);
float get_float();


 /* // REGULAR POLYGON */

void polygon_get_vertex(int i, float v[3]);
void polygon_n_increase(int d);
void polygon_add(Bool edges, Bool triangles);


 /* // POINTS */

void point_print();
void point_store(int i, float v[3]);


 /* // BACKUP AND UNDO */

void backup();
void int_swap(int* a, int* b);
void double_swap(double* a, double* b);
void float_swap(float* a, float* b);
void vector_swap(float a[3], float b[3]);
void edge_swap(int a[3], int b[3]);
void triangle_swap(int a[4], int b[4]);
void undo();


 /* // SCREEN  */

void screen_init(struct Screen* scr);
void screen_set_distance(struct Screen* scr,float distance);
void screen_set_clipping(struct Screen* scr,float clip_min, float clip_max);


 /* / COLOR ADJUSTMENT */

void color_adjust(float r, float g, float b, float c[3]);


 /* / VECTOR OPERATIONS */

void float_zeroes(int n, float v[]);
double matrix3_det(float m[3][3]);
void matrix3_vector_mult(float m[3][3], float v[3], float out[3]);
void matrix_init(float m[16]);
void double_zeroes(int n, double v[]);
Bool vector_eq(float  a[], float b[]);
void vector_add(float a[], float b[], float result[]);
void vector_sub(float a[], float b[], float result[]);
void vector_scale( float s, float v[]);
void vectorcpy(float dst[], float src[]);
void vector_product(float a[], float b[], float r[]);
double scalar_product(float a[3], float b[3]);
double vector_length(float a[3]);
double vector_distance(float a[3], float b[3]);
void vector_normalize(float v[]);

 /* /// VECTOR I/O */

void vector_fprintf(FILE *stream, float v[3]);
void vector_fscanf(FILE *stream, float v[3]);



 /* / CONSTRUCTIVE METHODS */

void find_group_center(int g, float X[3]);
void line_triangle_intersection( float A[3], float B[3],
			    float C[3], float D[3], float E[3],
				 float X[3]);
Bool line_triangle_solve( float A[3], float B[3],
			    float C[3], float D[3], float E[3],
			  float X[3], double *t);
Bool triangle_triangle_intersection( float A1[3],float A2[3], float A3[3],
				     float B1[3],float B2[3], float B3[3],
				     float E1[3],float E2[3]
				     );
     void group_tt_intersection(int g1, int g2);
void constr_scale(float A[3], float B[3], float C[3], float D[3],
	     float fixed_point[3], 
		  int g);
void constr_scale_in_direction(float A[3], float B[3], float C[3], float D[3],
			  float E[3], float F[3], float fixed_point[3], 
			       int g);
Bool find_three_point_transform(float P1[3], float P2[3], float P3[3], 
		     float Q1[3], float Q2[3], float Q3[3],
				float mv[3], float R[3][3]);
void three_point_transform(float P1[3], float P2[3], float P3[3], 
		      float Q1[3], float Q2[3], float Q3[3],
			   int g);
 /*  FOLDING */
Bool find_centered_folding(float A1[3],float A2[3],
			    float B1[3],float B2[3],
			    float K[3],
			   float V[3]);
Bool find_folding(float A[3],float B[3],float C[3],
		  float D[3],float E[3],float F[3], 
		  float V[3]);



 /* /// TRANSFORMATION ////////////////////////////////// */

void transformation_fprintf(FILE* s, struct Transformation* t);
void transformation_fscanf(FILE* s, struct Transformation* t);
void transformation_init(struct Transformation* t);
void transformation_init_orientation(struct Transformation* t);
void rotate(double angle, double x, double y, double z, struct Transformation* t);

void transformation_scale(double s,struct Transformation* t, float cursor[16]);
void transformation_rotate(double angle, double x, double y, double z, 
			   struct Transformation* t, float cursor[16]);
void transformation_move(float x, float y, float z, struct Transformation* t);
void find_versor(float x, float y, float z, float e[]);
void sort_dimmensions(double x, double y, double z, struct Transformation *t,
		      int d[3]);

 /* //// CLIPPING PLANES ///// */

void clip_reset(struct Clipping_xyz *clipping_xyz);
void clip_cube(struct Clipping_xyz *clipping_xyz);
void clip_xyz(float x, float y, float z, float cursor[], struct Clipping_xyz *clipping_xyz); 

 /* /// LIGHT /////////////////////////////////// */


void set_light(float light[16]);
void switch_second_light();


 /* // VERTEX FUNCTIONS */


int vertex_number_of(int vertex_used[VERTEX_MAX]);
int vertex_insert(float v[]);
int vertex_find(float v[]);
int vertex_compress(float vertex[][3],
                    int vertex_used[],
		    int edge_top, int edge[][3],
		    int triangle_top, int triangle[][4], float triangle_normal[][3],
		    int group[]);

void vertex_get_range(float vertex[][3], int vertex_used[], 
		      float vmin[3], float vmax[3]);


 /* // I/O */

void vertex_fprintf(FILE* s, int n, float v[][3]);
int vertex_fscanf(FILE* s,  float v[][3], int v_used[]);

 /* // EDGE STACK */

void edge_fprintf(FILE* s, int edge_top, int edge[][3] );
void edge_fscanf(FILE* s, int* edge_top, int edge[][3], int vertex_used[]);
Bool edge_eq(int a[], int b[]);
int edge_compare(const void* a, const void* b);
void edgecpy(int dest[],int src[]);
void edge_sort(int e[]);
int edge_insert(int e[]);
void edge_delete(int i);
int edge_vector_insert(float a[], float b[]);
void edge_set_color(int i, int color);
int edge_vector_delete(float a[], float b[]);
int edge_find(int e[3]);
Bool edge_inside_group(int e[3], int g);
void edge_delete_incident(float v[3]);
void edge_paint_incident(float v[3], int color);

 /* // TRIANGLE  */

void triangle_fprintf(FILE* s, int triangle_top, int triangle[][4]);
void triangle_fscanf(FILE* s, int* triangle_top, int triangle[][4], 
		     float triangle_normal[][3], float vertex[][3], int vertex_used[]);
Bool triangle_eq(int a[], int b[]);
int triangle_compare( const void * a, const void *b);
void trianglecpy(int dest[],int src[]);
void triangle_sort(int e[]);
int triangle_insert(int e[]);
void triangle_delete(int i);
Bool triangle_inside_group(int e[4], int g);
void triangle_delete_incident(float v[3]);
void triangle_paint_incident(float v[3], int color);
int triangle_vector_insert(float a[], float b[], float c[]);
void triangle_set_color(int i, int color);
void triangle_set_normal(int i);
void triangle_normal_vector(float a[], float b[], float c[], float norm[]);
void triangle_recompute_normals();
int triangle_vector_delete(float a[], float b[], float c[]);
int triangle_find(int e[4]);


 /* ///////// GROUPS */

void group_mark(int gr);
int group_find(float v[3]);
void group_regroup(int old, int new);
void group_remove_empty();
void group_restricted_switch();
int group_max(int group[], int vertex_used[]);
int group_size(int g, int group[], int vertex_used[]);
int group_incident_edges(int g);
int group_internal_edges(int g);
int group_incident_triangles(int g);
int group_internal_triangles(int g);
void group_statistics(int g);
void group_change_current(int g, int *group_current, int group[], int vertex_used[]);
void group_fprintf(FILE* s, int n, int group[]);
void group_fscanf(FILE* s, int group[]);
void group_rot90(int g, float x, float y, float z);
void group_move(int g, float mv[3], 
		int group[], float vertex[][3], int vertex_used[]);
void group_cursor_move(float x, float y, float z);
void group_scale(int g, double sx, double sy, double sz, float cursor[3],
		 int group[], float vertex[][3], int vertex_used[]);
void group_transform(int g, float mv[3], float R[3][3]);
void group_extract_clipped();
void group_copy(Bool glue_edges, Bool glue_triangles);
void group_paint_edges(int g, int color);
void group_paint_triangles(int g, int color);
void group_delete_edges(int g);
void group_delete_triangles(int g);

 /* ////// CURSOR /////// */

void cursor_set_shape();
void cursor_move(float x, float y, float z);
void cursor_set(double x, double y, double z);
void cursor_find_vertex();
void cursor_set_step(double step);
void cursor_to_screen();


 /* / REDUTCIONS */

void reduce_group_vertices(int g);



 /* ///////////////// EDITOR  /////////////////////////// */


void context_switch(enum Context new_context);
int set_current_color(int c);


 /* /////////// SAVING / LOADING */



int save(char* fname, 
	 float vertex[][3], int vertex_used[],
	 int edge_top, int edge[][3], 
	 int triangle_top, int triangle[][4], 
	 float triangle_normal[][3],
	 struct Transformation* transformation,
	 float light[3],
         int background,
	 struct Screen* screen,
	 float cursor[3],
	 float cursor_step,
	 int current_color,
	 int group[]);
int load(char* fname, 
	 float vertex[][3], int vertex_used[],
	 int* edge_top, int edge[][3], 
	 int* triangle_top, int triangle[][4], 
	 float triangle_normal[][3],
	 struct Transformation* transformation,
	 float light[3],
	 int* background,
	 struct Screen *screen,
	 float cursor[3],
	 float *cursor_step,
	 int* current_color,
	 int group[]);
int merge(char* fname, 
	  float vertex[][3], int vertex_used[],
	  int* edge_top, int edge[][3],
	  int* triangle_top, int triangle[][4], 
	  float triangle_normal[][3],
	  struct Transformation* transformation,
	  float light[3],
	  int* background,
	  struct Screen *screen,
	  float cursor[3],
	  float *cursor_step,
	  int* current_color,
	  int group[]);


 /* // PCL raster printing */

void pcl_init(FILE* f);
void pcl_finish(FILE* f);
void pcl_line1_bytes(FILE* f,int length, double red, double green, double blue);
void pcl_line12_bytes(FILE* f,int length, double red, double green, double blue);
void pcl_print_two_lines(FILE *f);
void pcl_print_raster(FILE *f);
void pcl_print();


 /* //// POVRAY */

int povray_export();
void povray_clipping(FILE* povf);


 /* ////////// X / GLX ////////////////////// */

void initglx();

 /* /////////////////////////////////////////////////////////////////////////////// */

void mainloop();

 /* //// CALLBACKS */

void callbackExpose( XExposeEvent* evptr);

 /* //////////// KEY COMMANDS //////////// */

void callbackKeyPress( XKeyEvent* evptr);
void callback_key_F7(XKeyEvent* evptr);
void key_F7_help();
void callback_key_F5(XKeyEvent* evptr);
void key_F5_help();
void callback_key_F4(XKeyEvent* evptr);
void key_F4_help();
void callback_key_F3(XKeyEvent* evptr);
void key_F3_help();
void callback_key_F2(XKeyEvent* evptr);
void key_F2_help();
void callback_key_F1(XKeyEvent* evptr);
void key_F1_help();
void callback_key_default(XKeyEvent* evptr);
void setfrustum();


 /* // REDRAW FUNCTIONS */

void draw_triangles(int top, int t[][4],  float v[][3], float t_normal[][3],
		    int group_current, Bool group_restricted, int group[]);
void draw_triangle(float a[], float b[], float c[], int col);
void draw_edge(float a[], float b[], int c);
void draw_edges(int top, int e[][3], float v[][3],  /*  int e_color[], */
		int group_current, Bool group_restricted, int group[]);
void draw_cursor();
void redraw_mono();
void redraw();


 /* // MENU */

int are_you_sure();
void quit_menu();
void main_menu();
void screen_menu();
void cursor_menu();
void group_menu();
void graph_menu();
void export_menu();
void povray_menu();


 /* ///// GRAPH FUNCTIONS */

void graph_marked_init();
void graph_queue_init();
void graph_queue_insert(int v);
int graph_queue_get();
void graph_push_nbr(int v, int nbr);
void graph_create(int edge_top, int edge[][3], int triangle_top, int triangle[][4]);
int graph_bfs(int v);
void graph_move_marked(float vector[3]);
void graph_copy_move_marked(float vector[3], int first_free );
void graph_scale_marked(double sx, double sy, double sz, float cursor[3]);
void graph_paint_marked_edges(int color);
void graph_paint_marked_triangles(int color);
void graph_delete_marked_edges();
void graph_delete_marked_triangles();
void graph_rotate_marked(double angle, double x, double y, double z, float cursor[3]);


 /* // HELP */

void help_keys();

 /* // STATISTICS */

void stats();

 /* //////// MAIN */

int main(int argc, char *argv[]);





