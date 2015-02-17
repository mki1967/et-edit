
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


char file_name[MAX_FILE_NAME+5]="noname.et";
char merge_file_name[MAX_FILE_NAME+5]="noname.et";




 /* //// SCREEN VARIABLES */

 /*  float rwidth, rheight;  */


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


struct Transformation transformation;


 /* // CLIPPING VARIABLES */


struct Clipping_xyz clipping_xyz;

float clipping_cube_size=25.0;


 /* /// COLOR VARIABLES */


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


int vertex_used[VERTEX_MAX];
float vertex[VERTEX_MAX][3];


 /* /// EDGE VARIABLES */

int edge[EDGE_MAX][3]; 
 /*  int edge_color[EDGE_MAX]; // moved to last coordinate of edge */
int edge_top=0;


 /* // TRIANGLE VARIABLES */

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


int cursor_edge[CURSOR_EDGE_TOP][3] = { {0,1,0}, {2,3,0}, {4,5,0}, {3,6,0} };
 /*  int cursor_edge_color[CURSOR_EDGE_TOP]= {0,0,0}; */

float cursor_step=1.0, cursor_min_step=0.0001, cursor_max_step=10000;


 /* // EDITOR VARIABLES */

float move_step=1.0, rotate_step=5.0;


enum Context context=context_default;


float  et0_marker[3], t1_marker[3];  /*  for storing endpoits of new edge or triangle */

 /* / CALLBACK */


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

float point[POINT_MAX][3]; /* contructive points */
Bool point_visible[POINT_MAX]; /* visibility of contructive points */


 /* // REGULAR POLYGON */

int   polygon_n=POLYGON_MIN;


 /* // REDUCTION */

double reduction_epsilon=0.0001;



/* FONT DATA */
const float font_scale=0.06;

#include "font_data.c" /* include font data collected with make_font_data_c.bash */


/* TABLE OF SHAPES of digits 0...9 and then letters A...Z */
SHAPE_VE * digit_letter_shape[] = {
                                   &font_0,
                                   &font_1,
                                   &font_2,
                                   &font_3,
                                   &font_4,
                                   &font_5,
                                   &font_6,
                                   &font_7,
                                   &font_8,
                                   &font_9,
                                   &font_A,
                                   &font_B,
                                   &font_C,
                                   &font_D,
                                   &font_E,
                                   &font_F,
                                   &font_G,
                                   &font_H,
                                   &font_I,
                                   &font_J,
                                   &font_K,
                                   &font_L,
                                   &font_M,
                                   &font_N,
                                   &font_O,
                                   &font_P,
                                   &font_Q,
                                   &font_R,
                                   &font_S,
                                   &font_T,
                                   &font_U,
                                   &font_V,
                                   &font_W,
                                   &font_X,
                                   &font_Y,
                                   &font_Z,
};

