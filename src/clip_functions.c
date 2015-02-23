#include "./et-edit.h"

void clip_reset(struct Clipping_xyz *clipping_xyz)
{
  clipping_xyz->xmin_flag=False;
  clipping_xyz->xmax_flag=False;
  clipping_xyz->ymin_flag=False;
  clipping_xyz->ymax_flag=False;
  clipping_xyz->zmin_flag=False;
  clipping_xyz->zmax_flag=False;
  printf("CLIPPING RESET\n");
}


void clip_cube(struct Clipping_xyz *clipping_xyz)
{

  clipping_xyz->xmin=cursor[0]-clipping_cube_size/2;
  clipping_xyz->xmax=cursor[0]+clipping_cube_size/2;
  clipping_xyz->ymin=cursor[1]-clipping_cube_size/2;
  clipping_xyz->ymax=cursor[1]+clipping_cube_size/2;
  clipping_xyz->zmin=cursor[2]-clipping_cube_size/2;
  clipping_xyz->zmax=cursor[2]+clipping_cube_size/2;

  clipping_xyz->xmin_flag=True;
  clipping_xyz->xmax_flag=True;
  clipping_xyz->ymin_flag=True;
  clipping_xyz->ymax_flag=True;
  clipping_xyz->zmin_flag=True;
  clipping_xyz->zmax_flag=True;
  printf("CLIPPING CUBE SET (size=%f)\n", clipping_cube_size);
}


void clip_xyz(float x, float y, float z, float cursor[], struct Clipping_xyz *clipping_xyz) 
{
  float e[3];

  printf("CLIPPING: ");

  find_versor(x, y, z, e);

  if(e[0]==1.0) 
    {
      clipping_xyz->xmin = cursor[0];
      clipping_xyz->xmin_flag=!clipping_xyz->xmin_flag;
      printf("xmin_flag =%d, xmin=%f\n", 
	     clipping_xyz->xmin_flag, clipping_xyz->xmin);
      return;
    }
  if(e[0]==-1.0) 
    {
      clipping_xyz->xmax = cursor[0];
      clipping_xyz->xmax_flag=!clipping_xyz->xmax_flag;
      printf("xmax_flag =%d, xmax=%f\n", 
	     clipping_xyz->xmax_flag, clipping_xyz->xmax);
      return;
    }

  if(e[1]==1.0) 
    {
      clipping_xyz->ymin = cursor[1];
      clipping_xyz->ymin_flag=!clipping_xyz->ymin_flag;
      printf("ymin_flag =%d, ymin=%f\n", 
	     clipping_xyz->ymin_flag, clipping_xyz->ymin);
      return;
    }
  if(e[1]==-1.0) 
    {
      clipping_xyz->ymax = cursor[1];
      clipping_xyz->ymax_flag=!clipping_xyz->ymax_flag;
      printf("ymax_flag =%d, ymax=%f\n", 
	     clipping_xyz->ymax_flag, clipping_xyz->ymax);
      return;
    }

  if(e[2]==1.0) 
    {
      clipping_xyz->zmin = cursor[2];
      clipping_xyz->zmin_flag=!clipping_xyz->zmin_flag;
      printf("zmin_flag =%d, zmin=%f\n", 
	     clipping_xyz->zmin_flag, clipping_xyz->zmin);
      return;
    }
  if(e[2]==-1.0) 
    {
      clipping_xyz->zmax = cursor[2];
      clipping_xyz->zmax_flag=!clipping_xyz->zmax_flag;
      printf("zmax_flag =%d, zmax=%f\n", 
	     clipping_xyz->zmax_flag, clipping_xyz->zmax);
      return;
    }

}



