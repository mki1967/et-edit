#include "./et-edit.h"

void draw_triangles(int top, int t[][4],  float v[][3], float t_normal[][3],
	       int group_current, Bool group_restricted, int group[])
{
  int i;

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  if(two_lights) glEnable(GL_LIGHT1); 
  else glDisable(GL_LIGHT1);

  glBegin(GL_TRIANGLES);

  for(i=0; i<top; i++)
    if(!group_restricted || 
       (group[t[i][0]]==group_current && 
	group[t[i][1]]==group_current &&
	group[t[i][2]]==group_current
	)
       )

      {
	if(!stereo_mode) glColor3fv( color[t[i][3]] );
	glNormal3fv( t_normal[i] );
	glVertex3fv( v[t[i][0]] );
	glVertex3fv( v[t[i][1]] );
	glVertex3fv( v[t[i][2]] );
      }
  
  glEnd();    
}

void draw_triangle(float a[], float b[], float c[], int col)
{
  float norm[3];
  triangle_normal_vector(a,b,c, norm);  

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  if(two_lights) glEnable(GL_LIGHT1);
  else glDisable(GL_LIGHT1);


  glBegin(GL_TRIANGLES);
      if(!stereo_mode) glColor3fv( color[col] );
      glNormal3fv( norm );
      glVertex3fv( a );
      glVertex3fv( b );
      glVertex3fv( c );
  glEnd(); 
}

void draw_edge(float a[], float b[], int c)
{
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
      if(!stereo_mode) glColor3fv( color[c] );
      glVertex3fv( a );
      glVertex3fv( b );
  glEnd(); 
}


void draw_edges(int top, int e[][3], float v[][3],  /*  int e_color[], */
	       int group_current, Bool group_restricted, int group[], 
               int color_idx /* if 0<= color_idx < COLOR_MAX  then use color[color_idx] */
               )
{
  int i;
  Bool valid_color_idx;
  valid_color_idx= ( 0<= color_idx &&  color_idx < COLOR_MAX );
  if(valid_color_idx && !stereo_mode) glColor3fv( color[color_idx] );

  glDisable(GL_LIGHTING);


  glBegin(GL_LINES);
      
  for(i=0; i<top; i++)
    if(!group_restricted || 
       (group[e[i][0]]==group_current && 
	group[e[i][1]]==group_current
	)
       )
      {
	if(!(stereo_mode || valid_color_idx) ) glColor3fv( color[e[i][2]] );
	glVertex3fv( v[e[i][0]] );
	glVertex3fv( v[e[i][1]] );
      }
  
  glEnd();    
}

void draw_cursor()
{
  glPushMatrix();
  glTranslatef(cursor[0], cursor[1], cursor[2]);
  draw_edges(CURSOR_EDGE_TOP, cursor_edge, cursor_vertex, 
	     group_current, False, group, current_color);
  glPopMatrix();
}


void draw_font_shape_ve(float font_scale, SHAPE_VE * shape, float x, float y, float z)
{
/* UNFINISHED !!! */
  glPushMatrix();
  glTranslatef(x, y, z);
  glMultMatrixd(transformation.R); /* reverse rotations */
  glScaled(font_scale,font_scale,font_scale);
  
  draw_edges(shape->edge_top, shape->edge, shape->vertex, 
	     group_current, False, group, current_color);
  glPopMatrix();
}


void redraw_mono()
{
  glEnable(GL_NORMALIZE);
  glTranslated(transformation.mov[0],transformation.mov[1],transformation.mov[2]);
  glMultMatrixd(transformation.M);
  glScaled(scale,scale,scale);
/*   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
/*   glClear(GL_DEPTH_BUFFER_BIT); */
  glDisable(GL_CLIP_PLANE0);
  glDisable(GL_CLIP_PLANE1);
  glDisable(GL_CLIP_PLANE2);
  glDisable(GL_CLIP_PLANE3);
  glDisable(GL_CLIP_PLANE4);
  glDisable(GL_CLIP_PLANE5);
   /*  ... */

  {
    float diffuse[4];

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv( GL_LIGHT0, GL_POSITION, light);

    vector_scale(-1,light);
    diffuse[0]=diffuse[1]=diffuse[2]=diffuse[3]=1.0;
    glLightfv( GL_LIGHT1, GL_POSITION, light);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, diffuse);
    vector_scale(-1,light);
  }

  /* TEST - begin */
/*
  draw_font_shape_ve(font_scale, &font_point,  0,0,0); 
  draw_font_shape_ve(font_scale,digit_letter_shape[10+'Z'-'A'] ,  0,0,0); 
  draw_font_shape_ve(font_scale, &font_point, cursor[0], cursor[1], cursor[2] ); 
  draw_font_shape_ve(font_scale, digit_letter_shape[0], cursor[0], cursor[1], cursor[2] ); 
*/
  /* TEST - end */

  { /* draw visible constructive points */
    int i;
    for(i=0; i< POINT_MAX; i++)
      if(point_visible[i]) {
         draw_font_shape_ve(font_scale, &font_point,  point[i][0],point[i][1],point[i][2] ); 
         draw_font_shape_ve(font_scale,digit_letter_shape[i] , point[i][0],point[i][1],point[i][2]); 
      }
  }  


  draw_cursor(); /* cursor */
  switch(context)
    {
    case context_et0:
      draw_edge(et0_marker, cursor, current_color); break;
    case context_t1:
      draw_triangle(et0_marker, t1_marker, cursor, current_color); break;
    case context_default:
      break;
    }

  {
     /*  clipping */
    double eqn[4];    

    if(clipping_xyz.xmin_flag)
      {
	double_zeroes(4, eqn);
        eqn[0]=1.0;
        eqn[3]=-clipping_xyz.xmin;
        glClipPlane(GL_CLIP_PLANE0, eqn );
        glEnable(GL_CLIP_PLANE0);
      }
    if(clipping_xyz.xmax_flag)
      {
	double_zeroes(4, eqn);
        eqn[0]=-1.0;
        eqn[3]=clipping_xyz.xmax;
        glClipPlane(GL_CLIP_PLANE1, eqn );
        glEnable(GL_CLIP_PLANE1);
      }

    if(clipping_xyz.ymin_flag)
      {
	double_zeroes(4, eqn);
        eqn[1]=1.0;
        eqn[3]=-clipping_xyz.ymin;
        glClipPlane(GL_CLIP_PLANE2, eqn );
        glEnable(GL_CLIP_PLANE2);
      }
    if(clipping_xyz.ymax_flag)
      {
	double_zeroes(4, eqn);
        eqn[1]=-1.0;
        eqn[3]=clipping_xyz.ymax;
        glClipPlane(GL_CLIP_PLANE3, eqn );
        glEnable(GL_CLIP_PLANE3);
      }

    if(clipping_xyz.zmin_flag)
      {
	double_zeroes(4, eqn);
        eqn[2]=1.0;
        eqn[3]=-clipping_xyz.zmin;
        glClipPlane(GL_CLIP_PLANE4, eqn );
        glEnable(GL_CLIP_PLANE4);
      }
    if(clipping_xyz.zmax_flag)
      {
	double_zeroes(4, eqn);
        eqn[2]=-1.0;
        eqn[3]=clipping_xyz.zmax;
        glClipPlane(GL_CLIP_PLANE5, eqn );
        glEnable(GL_CLIP_PLANE5);
      }

     /*  ... */

  }

  draw_edges(edge_top, edge, vertex, 
	         group_current, group_restricted, group, -1); /* -1 is not valid color index */
  draw_triangles(triangle_top, triangle, vertex, triangle_normal,
	         group_current, group_restricted, group);

  glDisable(GL_CLIP_PLANE0);
  glDisable(GL_CLIP_PLANE1);
  glDisable(GL_CLIP_PLANE2);
  glDisable(GL_CLIP_PLANE3);
  glDisable(GL_CLIP_PLANE4);
  glDisable(GL_CLIP_PLANE5);

   /*    glFlush(); */
}





void redraw()
{
  float rwidth /* , rheight */;
  rwidth = screen.width*pixel_size;
  /* rheight = screen.height*pixel_size; */


  triangle_recompute_normals();  /*  SORRY ! */
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  if(!stereo_mode)
    {
       /*  CASE FOR MONO */
      glClearColor(color[background][0], 
		   color[background][1], 
		   color[background][2], 0.0);
      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      redraw_mono();
      glXSwapBuffers(display,window);
    }
  else
    {
      glClearColor(stereo_background[0],
		   stereo_background[1],
		   stereo_background[2], 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glColor3fv(stereo_left_color);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(
		(-rwidth+stereo_eye_distance)*screen.clip_min/
		(2*screen.distance),
		(rwidth+stereo_eye_distance)*screen.clip_min/
		(2*screen.distance),
       		frustum_bottom,
		frustum_top,
		frustum_near_val,
		frustum_far_val
		);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslated(stereo_eye_distance/2,0,0);
      glColorMask(stereo_left_mask[0], stereo_left_mask[1], stereo_left_mask[2], 
		  GL_FALSE);
      redraw_mono();


      glColor3fv(stereo_right_color);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(
		(-rwidth-stereo_eye_distance)*screen.clip_min/
		(2*screen.distance),
		(rwidth-stereo_eye_distance)*screen.clip_min/
		(2*screen.distance),
       		frustum_bottom,
		frustum_top,
		frustum_near_val,
		frustum_far_val
		);
      glMatrixMode(GL_MODELVIEW);

      glLoadIdentity();
      glTranslated(-stereo_eye_distance/2,0,0);
      glClear(GL_DEPTH_BUFFER_BIT);
      glColorMask(stereo_right_mask[0], stereo_right_mask[1], stereo_right_mask[2], 
		  GL_FALSE);

      redraw_mono();

      glXSwapBuffers(display,window);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(
		frustum_left,
		frustum_right,
		frustum_bottom,
		frustum_top,
		frustum_near_val,
		frustum_far_val
		);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
      glMatrixMode(GL_MODELVIEW);
    }


}



