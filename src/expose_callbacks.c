#include "include/et-edit.h"
void callbackExpose( XExposeEvent* evptr)
{
long event_mask=ExposureMask;
while(XCheckWindowEvent(display,window,event_mask , (XEvent*) evptr));


  glXMakeCurrent(display, None, NULL);
  glXDestroyContext(display,glxcontext);
  glxcontext= glXCreateContext(display, &xvisualinfo_array[0], NULL, True);
  glXMakeCurrent(display, window, glxcontext);
  {
    int param[4];
    glGetIntegerv(GL_VIEWPORT, param);
    if(screen.width != param[2] || screen.height!= param[3])
      {
	screen.width= param[2];
	screen.height= param[3];
	printf("viewport: %d x %d\n", screen.width, screen.height);
      }
  }
  setfrustum();
  redraw();
}


