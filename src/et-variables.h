/* DECLARATIONS OF THE GLOBAL VARIABLES DEFINED IN: src/et-variables.c */

 /* /////// X / GLX VARIABLES ///////////// */

extern Display *display;
extern XVisualInfo *xvisualinfo_array;
extern int xvisualinfo_nitems;
extern Window window;
extern Window terminal;

extern XSetWindowAttributes windowattributes;


extern GLXContext glxcontext;


 /* ////// PROGRAM INTERNAL VARIABLES */


extern float pixel_size;
extern float screen_distance;
extern float clip_min_limit;
extern float clip_min;
extern float clip_max;

extern int screen_width, screen_height;

extern char *program_name;


extern char file_name[];
extern char merge_file_name[];




 /* //// SCREEN VARIABLES */

 /*  float rwidth, rheight;  */


extern struct Screen screen;


 /* // FRUSTUM VARIABLES (computed by setfrustum() ) */

extern float frustum_left, frustum_right,frustum_bottom, frustum_top,
  frustum_near_val, frustum_far_val;


extern double scale;

 /* // STEREO VARIABLES */

extern Bool stereo_mode;
extern float stereo_eye_distance;
extern float stereo_left_color[];
extern GLboolean stereo_left_mask[];
extern float stereo_right_color[];
extern GLboolean stereo_right_mask[];
extern float stereo_background[];




 /* // TRANSFORMATION VARIABLES */


extern struct Transformation transformation;


 /* // CLIPPING VARIABLES */


extern struct Clipping_xyz clipping_xyz;

extern float clipping_cube_size;


 /* /// COLOR VARIABLES */


extern float color[][3];

extern int background;
extern int current_color;


 /* // LIGHT VARIABLES */

extern float basic_light[];
extern float light[];
extern Bool two_lights;


 /* // VERTEX VARIABLES */


extern int vertex_used[];
extern float vertex[][3];


 /* /// EDGE VARIABLES */

extern int edge[][3]; 

extern int edge_top;


 /* // TRIANGLE VARIABLES */

extern int triangle[][4]; 
 /*  int triangle_color[TRIANGLE_MAX]; // moved to last coordinate of triangle */
extern float triangle_normal[][3];
extern int triangle_top;


 /* // GROUP VARIABLES */

extern int group[];
extern int group_current;
extern int group_marker;
extern Bool group_restricted;


 /* // CURSOR VARIABLES */

extern float cursor[];

extern float cursor_vertex[][3];

extern float cursor_vertex_template[][3]; 


extern int cursor_edge[][3];
 /*  int cursor_edge_color[CURSOR_EDGE_TOP]= {0,0,0}; */

extern float cursor_step, cursor_min_step, cursor_max_step;


 /* // EDITOR VARIABLES */

extern float move_step, rotate_step;


extern enum Context context;


extern float  et0_marker[], t1_marker[];  /*  for storing endpoits of new edge or triangle */

 /* / CALLBACK */


extern enum Keymode keymode;


 /* / LOAD/SAVE VARIABLES */

extern char transformation_label[];
extern char light_label[];
extern char background_label[];
extern char cursor_label[];
extern char group_label[];



 /* // EXPORT FUNCTION VARIABLE */

extern int (*export_last_ptr)();

 /* / POVRAY VARIABLES */

extern char povray_name[];
extern float povray_light_distance;
extern float povray_ambient;
extern float povray_diffuse;
 /*  float povray_edge_ambient=1; */
 /*  float povray_edge_diffuse=0; */
extern float povray_light_red;
extern float povray_light_green;
extern float povray_light_blue;


extern float povray_edge_radius;
extern int povray_counter;



 /* // GRAPH VARIABLES */

extern int graph_first_nbr[];
extern int graph_nbr[];
extern int graph_next_nbr[];
extern int graph_next_free;

extern int graph_queue[];
extern int graph_queue_first, graph_queue_size;

extern int graph_marked[];


 /* // BACKUP AND UNDO VARIABLES */

extern Bool was_modified;

extern Bool backup_was_modified;


extern float backup_vertex[][3];
extern int   backup_vertex_used[];
extern int   backup_group[];
extern int   backup_group_current;
extern int   backup_group_marker;
extern Bool  backup_group_restricted;

extern int   backup_edge[][3];
extern int   backup_edge_top;

extern int   backup_triangle[][4];
extern int   backup_triangle_top;

extern struct Transformation backup_transformation;
extern float backup_cursor[];
extern float backup_cursor_step;

extern float backup_light[];
extern Bool  backup_two_lights;

extern int backup_background;
extern int backup_current_color;


extern enum Context backup_context;
extern float backup_et0_marker[], backup_t1_marker[];
extern float backup_move_step;




 /* // POINTS */

extern float point[][3]; /* contructive points */
extern Bool point_visible[]; /* visibility of contructive points */


 /* // REGULAR POLYGON */

extern int   polygon_n;


 /* // REDUCTION */

extern double reduction_epsilon;



/* FONT DATA */
extern const float font_scale;
extern const float font_width;
extern const float font_height;

/* #include "font_data.c" *//* include font data collected with make_font_data_c.bash */


/* TABLE OF SHAPES of digits 0...9 and then letters A...Z */
extern SHAPE_VE * digit_letter_shape[];
extern SHAPE_VE font_point;

