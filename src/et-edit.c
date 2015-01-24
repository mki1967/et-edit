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



#include "include/et-edit.h"



 /* ////////////  FUNCTIONS */



 /* // XTERMINAL INTERACTION */

void get_file_name(char *file_name)
{
  char new_name[MAX_FILE_NAME+5];
  XRaiseWindow(display, terminal);
  XSetInputFocus(display, terminal, RevertToParent, CurrentTime);
  printf("Input file name (input '-' to leave the old name: '%s'):\n", file_name);
  scanf("%100s", new_name);
  if( strcmp("-",new_name)!=0 ) strcpy(file_name, new_name);
  printf("New file name is: '%s'\n", file_name);
  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
}


float get_float()
{
  float x;
  scanf("%f", &x);
  return x;
}








 /* // BACKUP AND UNDO */

#include "backup_undo.c"



 /* // SCREEN  */

void screen_init(struct Screen* scr)
{
  scr->distance=screen_distance;
  scr->clip_min=clip_min;
  scr->clip_max=clip_max;
  scr->width=screen_width;
  scr->height=screen_height;
}


void screen_set_distance(struct Screen* scr,float distance)
{
  scr->distance=distance;
  setfrustum();
  printf("Screen distance is: %f\n", scr->distance);
}


void screen_set_clipping(struct Screen* scr,float clip_min, float clip_max)
{
  if(clip_min_limit<= clip_min && clip_min<= clip_max)
    {
      scr->clip_min=clip_min;
      scr->clip_max=clip_max;
      setfrustum();
    }
  else printf("Screen clipping: bad values !\n");
  printf("Screen: clip_min = %f, clip_max = %f\n", scr->clip_min, scr->clip_max);

}



 /* / COLOR ADJUSTMENT */

void color_adjust(float r, float g, float b, float c[3])
{
  c[0]+=r;
  if(c[0]<0) c[0]=0;
  if(c[0]>1) c[0]=1;

  c[1]+=g;
  if(c[1]<0) c[1]=0;
  if(c[1]>1) c[1]=1;

  c[2]+=b;
  if(c[2]<0) c[2]=0;
  if(c[2]>1) c[2]=1;

}


 /* / VECTOR OPERATIONS */

#include "include/vector_functions.c"

 /* / CONSTRUCTIVE METHODS */

#include "include/constructive_methods.c"


/* /// TRANSFORMATION ////////////////////////////////// */


#include "include/transformation_functions.c"


 /* //// CLIPPING PLANES ///// */

#include "include/clip_functions.c"


 /* /// LIGHT /////////////////////////////////// */

#include "include/light_functions.c"

 /* // VERTEX FUNCTIONS */

#include "include/vertex_functions.c"

 /* // EDGE STACK */

#include "include/edge_functions.c"

 /* // TRIANGLE  */

#include "include/triangle_functions.c"

 /* ///////// GROUPS */

#include "include/group_functions.c"


 /* ////// CURSOR /////// */

#include "include/cursor_functions.c"

/* / REDUTCIONS */


#include "include/reduce_functions.c"



 /* ///////////////// EDITOR  /////////////////////////// */


void context_switch(enum Context new_context)
{
  context=new_context;
  printf("CONTEXT: ");
  switch(context)
    {
    case context_default:
      printf("default\n"); break;
    case context_et0:
      printf("et0\n"); break;
    case context_t1:
      printf("t1\n"); break;
    }
}


int set_current_color(int c)
{
  int i;
  if(c<0 || c>= COLOR_MAX) return -1;
  current_color=c;
  for(i=0; i<CURSOR_EDGE_TOP; i++) cursor_edge[i][2]=c;
  return 0;
}







 /* /////////// SAVING / LOADING */


#include "include/io_functions.c"

 /* // PCL raster printing */

 
#include "include/export_pcl.c" 


 /* //// POVRAY */

#include "include/export_povray.c"


 /* ////////// X / GLX ////////////////////// */


void initglx()
{

display=XOpenDisplay( NULL );
if(display==NULL) 
  {
  printf("XOpenDisplay( NULL )== NULL\n");
  exit(-1);
  }


 {
   int glxminor, glxmajor;
   printf("glXQueryExtension: %d\n", glXQueryExtension(display, NULL, NULL));
   glXQueryVersion( display, &glxmajor, &glxminor);
   printf("Version : %d.%d\n", glxmajor, glxminor);
 }

{
XVisualInfo vinfo_template;
vinfo_template.screen=DefaultScreen(display);
xvisualinfo_array = XGetVisualInfo(display, 
                                   VisualScreenMask,
                                   &vinfo_template, &xvisualinfo_nitems);
if(xvisualinfo_array==NULL)
  {
  printf("No visual infos for default screen \n");
  exit(-1);
  }
}

 {
   int revert_to;
   XGetInputFocus(display, &terminal,&revert_to);
 }

 window= XCreateSimpleWindow( display, DefaultRootWindow(display) , 0,0, 
			      screen_width, screen_height,
                              0, 0, 0 );


 {
    

   unsigned long valuemask=CWEventMask | CWOverrideRedirect;
   windowattributes.event_mask=     KeyPressMask | ExposureMask ; /* / Enable events for the window */
   /* windowattributes.override_redirect = True; */ /* override redirection if window is raised */
   XChangeWindowAttributes(display, window, valuemask, &windowattributes);    
 }
 
 XStoreName(display, window, program_name);

 {
   XTextProperty text_prop;
   char **list;
   list= &program_name;
   XStringListToTextProperty(list, 1, &text_prop);
   XSetWMName(display, window, &text_prop);

 } 

 XMapWindow(display, window);
 XFlush(display);

/**/
    printf("terminal: %d\n", (int) terminal);  
   printf("window: %d\n", (int) window);  

 
 glxcontext= glXCreateContext(display, &xvisualinfo_array[0], NULL, True);

 printf("glXIsDirect: %d\n", glXIsDirect(display, glxcontext) );

 glXMakeCurrent(display, window, glxcontext);
 
}


 /* /////////////////////////////////////////////////////////////////////////////// */


void mainloop()
{
  XEvent event;

  while(1)
    {
      XWindowEvent(display, window, windowattributes.event_mask, &event);
      switch(event.type)
	{
        case KeyPress: callbackKeyPress((XKeyEvent*) &event ); break;
	case Expose: callbackExpose((XExposeEvent*) &event ); break;
	}

    }
}





 /* //// CALLBACKS */

#include "include/expose_callbacks.c"

/* //////////// KEY COMMANDS //////////// */


#include "include/key_callbacks.c"



void setfrustum()
{

  float rwidth, rheight;
  rwidth = screen.width*pixel_size/2;
  rheight = screen.height*pixel_size/2;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();



  frustum_left=-rwidth*screen.clip_min/screen.distance; 
  frustum_right=rwidth*screen.clip_min/screen.distance;
  frustum_bottom=-rheight*screen.clip_min/screen.distance; 
  frustum_top=rheight*screen.clip_min/screen.distance;
  frustum_near_val=screen.clip_min;
  frustum_far_val=screen.clip_max;
  glFrustum(
	    frustum_left,
	    frustum_right,
	    frustum_bottom,
	    frustum_top,
	    frustum_near_val,
	    frustum_far_val
	    );


/*   { */
/*     GLboolean x; */
/*     GLint y; */
/*     glGetBooleanv(GL_STEREO, &x ); */
/*     printf("glGetBooleanv(GL_STEREO, &x): x==%d\n",x); */
/*     glGetBooleanv(GL_DOUBLEBUFFER, &x ); */
/*     printf("glGetBooleanv(GL_DOUBLEBUFFER, &x): x==%d\n",x); */
/*     glGetIntegerv(GL_AUX_BUFFERS, &y); */
/*     printf("glGetIntegerv(GL_AUX_BUFFERS, &y): y==%d\n",y); */
/*   } */
}



 /* // REDRAW FUNCTIONS */

#include "include/redraw_functions.c"

 /* // MENU */

#include "include/terminal_menus.c"

 /* ///// GRAPH FUNCTIONS */

#include "include/graph_functions.c"

 /* // HELP */

void help_keys()
{
  printf("\n--------- LIST OF KEY COMMANDS -------------\n");
  printf("<Arrow keys>, <Home>, <End> - rotate around the cursor\n");
  printf("<Shift>+(<Arrow keys>, <Home>, <End>) - move the observer\n");
  printf("<Ctrl>+(<Arrow keys>, <Home>, <End>) - move the cursor\n");
  printf("<Ctrl>+<Shift>+(<Arrow keys>, <Home>, <End>) - clipping\n");
  printf("<Alt>+(<Arrow keys>, <Home>, <End>) - move current group\n");
  printf("<R> - reset observer\n");
  printf("<Ctrl>+<R> - reset cursor\n");
  printf("<Ctrl>+<Shift>+<C> - clipping cube\n");
  printf("<Ctrl>+<Shift>+<R> - reset clipping\n");
  printf("<L> - set the light perpendicular to the screen\n");
  printf("<Shift>+<L> - switch: one_light/two_lights mode\n");
  printf("<M> - menu in text terminal\n");
  printf("<H> - list of key commands\n");
  printf("<E> - enter edge vertex\n");
  printf("<T> - enter triangle vertex\n");
  printf("<Enter> - enter next link/triangle\n");
  printf("<Backspace> - delete edge/triangle\n");
  printf("<Esc> - cancel current operation\n");
  printf("<F> - find with the cursor the nearest vertex\n");
  printf("<Ctrl>+<F> - move the cursor toward the screen center\n");
  printf("<C> - change current color\n");
  printf("<B> - change background color\n");
  printf("<1> - switch restriction to current group ON/OFF \n");
  printf("<N> - change current group to next one (modulo max_group+2)\n");
  printf("<Shift>+<N> - change current group to previous one (modulo max_group+2)\n");
  printf("<Ctrl>+<S> - save\n");
  printf("<Ctrl>+<L> - load\n");
  printf("<Ctrl>+<Shift>+<L> - merge\n");
  printf("<Ctrl>+<Shift>+<S> - export\n");
  printf("<Ctrl>+<ALT>+<X> - delete edges incident to the vertex pointed by cursor\n");
  printf("<Ctrl>+<ALT>+<Shift>+<X> - delete triangles incident to the vertex pointed by cursor\n");
  printf("<Ctrl>+<ALT>+<P> - paint the edges incident to the vertex pointed by cursor\n");
  printf("<Ctrl>+<ALT>+<Shift>+<P> - paint the triangles incident to the vertex pointed by cursor\n");
  printf("<V> - move connected component (in context: et0)\n");
  printf("<Shift>+<V> - copy and move connected component (in context: et0)\n");  printf("<Ctrl>+<V> - move current group (in context: et0)\n");
  printf("<G> - menu for connected component in text terminal\n");
  printf("<Q> - quit\n");
  printf("<F1> - switch to F1 key mode (groups) \n");
  printf("<F2> - switch to F2 key mode (cursor, zoom, observer step) \n");
  printf("<F3> - switch to F3 mode (STORED POINTS)\n");
  printf("<F4> - switch to F4 mode (CONSTRUCTIVE METHODS)\n");
  printf("<F5> - reductions\n");
  printf("<F7> - stereo mode controls\n");
  printf("<U> -  undo (last step only) \n");
  printf("<=> -  statistics\n");
  printf("<Alt>+<O> - add regular polygon (edges)\n");
  printf("<Shift>+<O> - add regular polygon (triangles)\n");
  printf("<Alt>+<Shift>+<O> - add regular polygon (edges and triangles)\n");
  printf("<PageUp>/<PageDown> - increase/decrease number of polygon vertices [%d]\n",
polygon_n);
   /*  ... */

  printf("-----------------------------------------\n\n");


}



 /* // STATISTICS */

void stats()
{
  float vmin[3], vmax[3];
  printf("\n============================================\n");
  printf("STATISCTICS FOR: %s\n", file_name);
  printf("vertices: %d\n", vertex_number_of(vertex_used));
  printf("groups: %d\n", group_max(group, vertex_used)+1);
  printf("edges: %d\n", edge_top);
  printf("triangles: %d\n", triangle_top);
  vertex_get_range(vertex, vertex_used, vmin, vmax);
  printf("range:\n");
  printf(" (Xmin,Ymin,Zmin)=(%f, %f, %f)\n",
	 vmin[0],vmin[1],vmin[2]);
  printf(" (Xmax,Ymax,Zmax)=(%f, %f, %f)\n",
	 vmax[0],vmax[1],vmax[2]);
  printf("============================================\n");
 
}



 /* //////// MAIN */



int main(int argc, char *argv[])
{

  printf("==================================================\n");
  printf("E.T. EDITOR:  EDGE TRIANGLE THREE-DIMENSIONAL EDITOR\n");
  printf("version 0.21\n\n");
  printf("Copyright (C) 2003  Marcin Kik    mki1967@gmail.com \n");
  printf("\nE.T. EDITOR comes with ABSOLUTELY NO WARRANTY.\n");

  printf("This is unstable test version, that can be freely redistributed.\n");
  printf("Send any comments or suggestions to: mki1967@gmail.com \n");
  printf("Most recent versions are available at:  https://github.com/mki1967/et-edit \n"); 

  printf("This is free software, and you are welcome to redistribute it\n");
  printf(" under conditions of GNU GENERAL PUBLIC LICENSE\n\n");

  printf("\nThis terminal and the graphical window are both used for interactions with you.\n");
  printf("Thus the program must be a foreground process of this terminal.\n");
  printf("Most interactions are by pressing the keys, on the graphical window.\n");
  printf("(Press key <H> on graphical window to print the list of key commands)\n");
  printf("From time to time you may be requested to input a text in this terminal.\n");


  printf("==================================================\n");
  
  initglx();
  transformation_init(&transformation);
  screen_init(&screen);
  setfrustum();
  cursor_set_shape();
  export_last_ptr=povray_export;

  if(argc>=2) 
    {
      strncpy(file_name, argv[1], MAX_FILE_NAME-10);
      load( file_name, 
	    vertex, vertex_used,
	    &edge_top, edge, 
	    &triangle_top, triangle, triangle_normal,
	    & transformation,
	    light,
	    &background,
	    &screen,
	    cursor,
	    &cursor_step,
	    &current_color,
	    group);      
    }
  backup();
  was_modified=False;

  XRaiseWindow(display, window);
   /*  XSetInputFocus(display, window, RevertToParent, CurrentTime); */
  mainloop();
  return 0;
}




