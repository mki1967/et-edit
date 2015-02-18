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


 /* // BACKUP AND UNDO */
#include "backup_undo.c"
/* // SCREEN  */
#include "include/screen_functions.c"
/* / VECTOR OPERATIONS */
#include "include/vector_functions.c"
/* / CONSTRUCTIVE METHODS */
#include "include/constructive_methods.c"
/* /// TRANSFORMATION  */
#include "include/transformation_functions.c"
/* //// CLIPPING PLANES ///// */
#include "include/clip_functions.c"
/* /// LIGHT  */
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
/* /////////// SAVING / LOADING */
#include "include/io_functions.c"
/* // PCL raster printing */
/* #include "include/export_pcl.c" */
/* //// POVRAY */
#include "include/export_povray.c"
/* EXPORT TO C DATA */
#include "include/export_c_ve.c"
/* // REDRAW FUNCTIONS */
#include "include/redraw_functions.c"
/* // MENU */
/* #include "include/terminal_menus.c" */
#include "include/ui/ui.c"
/* ///// GRAPH FUNCTIONS */
#include "include/graph_functions.c"
/* //// CALLBACKS */
#include "include/expose_callbacks.c"
/* //////////// KEY COMMANDS //////////// */
#include "include/key_callbacks.c"



 /* ///////////////// EDITOR  /////////////////////////// */



 /* ////////// X / GLX ////////////////////// */

void goto_terminal()
{
/*
XDestroyWindow(display,window);
glXDestroyContext(display, glxcontext);
*/
  XLowerWindow(display, window);
  /* XIconifyWindow(display, window, DefaultScreen(display)); */
  /* XUnmapWindow(display, window); */ 

  XRaiseWindow(display, terminal);
  XSetInputFocus(display, terminal, RevertToParent, CurrentTime);
  XFlush(display);
}

void goto_window()
{
  XLowerWindow(display, terminal);
 /* XMapWindow(display, window); */
/* XIconifyWindow(display, window, DefaultScreen(display)); */ /* screen_number set to 0 */
/* XMapRaised(display, window); */
  XRaiseWindow(display, window);
  redraw(); 
 XSetInputFocus(display, window, RevertToParent, CurrentTime); 
  XFlush(display);
}

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
   printf("terminal: %d\n", (int) terminal); /**/  
 }
{
 glxcontext= glXCreateContext(display, &xvisualinfo_array[0], NULL, True);

 printf("glXIsDirect: %d\n", glXIsDirect(display, glxcontext) );
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
 glXMakeCurrent(display, window, glxcontext);
/**/
 printf("window: %d\n", (int) window);  

 
/* goto_window(); */

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
  printf("Thus the program must be run as a FOREGROUND process of this terminal.\n");
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




