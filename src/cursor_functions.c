#include "include/et-edit.h"

void cursor_set_shape()
{
  int i;
  for(i=0; i<CURSOR_VERTEX_MAX; i++) 
    {
      vectorcpy(cursor_vertex[i], cursor_vertex_template[i]);
      vector_scale(cursor_step, cursor_vertex[i]);
    }
}


void cursor_move(float x, float y, float z)
{
  float e[3];
  find_versor(x,y,z, e);
  vector_scale(cursor_step, e);
  vector_add(e, cursor, cursor);
  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
}


void cursor_set(double x, double y, double z)
{
  cursor[0]=x;
  cursor[1]=y;
  cursor[2]=z;
  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
}


void cursor_find_vertex()
{
  int i;
  i=vertex_find_nearest(cursor);
  if(i!=-1) vectorcpy(cursor, vertex[i]);
  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
}


void cursor_set_step(double step)
{
  if(step<cursor_min_step) 
    {
      printf("CURSOR STEP: %f < %f : too small !\n", step, cursor_min_step);
    }
  else
  if(step>cursor_max_step) 
    {
      printf("CURSOR STEP: %f > %f : too large !\n", step, cursor_max_step);
    }
  else
    {
      cursor_step=step;
      cursor_set_shape();
    }
  printf("CURSOR STEP = %f\n", cursor_step);

}


void cursor_to_screen()
{
  float v[16];
  matrix_init(v);
  v[2]=-screen.distance+cursor_step;
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMultMatrixd(transformation.R);
  glTranslatef(-transformation.mov[0],-transformation.mov[1],-transformation.mov[2]);
  glMultMatrixf(v);
  glGetFloatv( GL_MODELVIEW_MATRIX, v);
  glPopMatrix();

  v[0]=floorf(v[0]/cursor_step)*cursor_step;
  v[1]=floorf(v[1]/cursor_step)*cursor_step;
  v[2]=floorf(v[2]/cursor_step)*cursor_step;

  cursor_set(v[0],v[1],v[2]);
}



