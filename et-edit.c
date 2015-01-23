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



#include "et-edit.h"









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




 /* // REGULAR POLYGON */

void polygon_get_vertex(int i, float v[3])
{
  int d[3], x,y;

  sort_dimmensions(1.0, 0.0, 0.0, &transformation,d);
  x=d[0];
  sort_dimmensions(0.0, 1.0, 0.0, &transformation,d);
  y= (d[0]!=x)? d[0]: d[1];

  vectorcpy(v, cursor);
  v[x]+= 2*cursor_step*cos(i*2*M_PI/polygon_n);
  v[y]+= 2*cursor_step*sin(i*2*M_PI/polygon_n);

}



void polygon_n_increase(int d)
{
  polygon_n+= d;
  if(polygon_n>POLYGON_MAX) polygon_n=POLYGON_MAX;
  if(polygon_n<POLYGON_MIN) polygon_n=POLYGON_MIN;

  printf("NUMBER OF POLYGON VERTICES: %d\n",polygon_n); 

}

void polygon_add(Bool edges, Bool triangles)
{
  int first_free;
  int group_new;
  int i;

  first_free=vertex_compress(vertex,
			     vertex_used,
			     edge_top, edge,
			     triangle_top, triangle, triangle_normal,
			     group);


  if(first_free+polygon_n >= VERTEX_MAX)
    {
      printf("polygon_add: too many vertices !!!\n");
      return;
    }
  if(edges && edge_top+polygon_n >= EDGE_MAX)
      {
      printf("polygon_add: too many edges !!!\n");
      return;
      }
  if(triangles && triangle_top+polygon_n-2 >= TRIANGLE_MAX)
      {
      printf("polygon_add: too many triangles !!!\n");
      return;
      }



  group_new=group_max(group,vertex_used)+1;
  group_change_current(group_new, &group_current, group, vertex_used );

  for(i=0; i<polygon_n; i++)
    polygon_get_vertex(i,vertex[i+first_free]);



  if(edges)
    {
    for(i=0; i<polygon_n; i++)
      {
	edge[edge_top+i][0]=first_free+i;
	edge[edge_top+i][1]=first_free+(i+1)%polygon_n;
	edge_sort(edge[edge_top+i]);
	edge[edge_top+i][2]=current_color;
	vertex_used[first_free+i]++;
	vertex_used[first_free+(i+1)%polygon_n]++;
      }
    edge_top+=polygon_n;
    }


  if(triangles)
    {
    for(i=0; i<polygon_n-2; i++)
      {
	triangle[triangle_top+i][0]=first_free;
	triangle[triangle_top+i][1]=first_free+i+1;
	triangle[triangle_top+i][2]=first_free+i+2;
	triangle[triangle_top+i][3]=current_color;
	 /*  triangle_sort(triangle[triangle_top+i]); */
	triangle_set_normal(triangle_top+i);
	vertex_used[first_free]++;
	vertex_used[first_free+i+1]++;
	vertex_used[first_free+i+2]++;
      }
    triangle_top+=polygon_n-2; 
    }

  group_change_current(group_current, &group_current, group, vertex_used );


}





 /* // POINTS */

void point_print()
{
  int i;
  printf("POINTS:\n");
  for(i=0; i<10; i++)
    printf("'%d' = (%f, %f, %f)\n", i, 
	   point[i][0],point[i][1],point[i][2]);
  printf("\n");
  for(i=10; i<POINT_MAX; i++)
    printf("'%c' = (%f, %f, %f)\n", i-10+'A', 
	   point[i][0],point[i][1],point[i][2]);

}


void point_store(int i, float v[3])
{
  if(i<0 || i>= POINT_MAX) return;
  vectorcpy(point[i], v);
  printf("POINT STORED: '%c' =(%f, %f, %f)\n", 
	 i<10 ? i+'0' :  i-10+'A',
	 point[i][0],
	 point[i][1],
	 point[i][2]);
}




 /* // BACKUP AND UNDO */


void backup()
{
  int i;
  backup_was_modified=was_modified;
  was_modified=True;
  
  for(i=0;i<VERTEX_MAX; i++)
    {
      vectorcpy( backup_vertex[i], vertex[i]);
      backup_vertex_used[i]=vertex_used[i];
      backup_group[i]=group[i];
    }
  backup_group_current=group_current;
  backup_group_marker=group_marker;
  backup_group_restricted=group_restricted;

  backup_edge_top=edge_top;
  for(i=0;i<edge_top; i++)
    {  
      edgecpy(backup_edge[i], edge[i]);
    }
  
  backup_triangle_top=triangle_top;

  for(i=0;i<triangle_top; i++)
    {  
      trianglecpy(backup_triangle[i],triangle[i]);
    }

  
  for(i=0; i<16; i++)
    {
      backup_transformation.mov[i]=transformation.mov[i];
      backup_transformation.M[i]=transformation.M[i];
      backup_transformation.R[i]=transformation.R[i];
    }

  vectorcpy(backup_cursor, cursor);
  backup_cursor_step=cursor_step;
  
  for(i=0; i<4; i++)
   {
     backup_light[i]=light[i];
   }
  
  backup_two_lights=two_lights;
  
  backup_background= background;
  backup_current_color= current_color;

 
  backup_context=context;
  vectorcpy(backup_et0_marker,et0_marker);
  vectorcpy(backup_t1_marker, t1_marker); 
  backup_move_step=move_step;
}

void int_swap(int* a, int* b)
{
  int tmp;
  tmp=*a; *a=*b; *b=tmp;
}


void double_swap(double* a, double* b)
{
  double tmp;
  tmp=*a; *a=*b; *b=tmp;
}

void float_swap(float* a, float* b)
{
  float tmp;
  tmp=*a; *a=*b; *b=tmp;
}

void vector_swap(float a[3], float b[3])
{
  float_swap(&a[0],  &b[0]);
  float_swap(&a[1],  &b[1]);
  float_swap(&a[2],  &b[2]);
}


void edge_swap(int a[3], int b[3])
{
  int_swap(&a[0],  &b[0]);
  int_swap(&a[1],  &b[1]);
  int_swap(&a[2],  &b[2]);
}

void triangle_swap(int a[4], int b[4])
{
  int_swap(&a[0],  &b[0]);
  int_swap(&a[1],  &b[1]);
  int_swap(&a[2],  &b[2]);
  int_swap(&a[3],  &b[3]);
}





void undo()
{
  int i;

  int_swap(&backup_was_modified, &was_modified);

  
   /* // ....  */

  for(i=0;i<VERTEX_MAX; i++)
    {
      vector_swap( backup_vertex[i], vertex[i]);
      int_swap(&backup_vertex_used[i], &vertex_used[i]);
      int_swap(&backup_group[i], &group[i]);
    }

  int_swap(&backup_group_current, &group_current);
  group_change_current(group_current, &group_current, group, vertex_used );
  int_swap(&backup_group_marker, &group_marker);
  int_swap(&backup_group_restricted, &group_restricted);
  printf("RESTRICTED TO GROUP:");
  if(group_restricted) printf(" ON\n");
  else printf(" OFF\n");
 

  int_swap(&backup_edge_top, &edge_top);
  for(i=0;i<edge_top || i<backup_edge_top ; i++)
    {  
      edge_swap(backup_edge[i], edge[i]);
    }

  int_swap(&backup_triangle_top, &triangle_top);

  for(i=0;i<triangle_top || i<backup_triangle_top; i++)
    {  
      triangle_swap(backup_triangle[i],triangle[i]);
    }


  for(i=0; i<16; i++)
    {
      float_swap(&backup_transformation.mov[i],&transformation.mov[i]);
      double_swap(&backup_transformation.M[i], &transformation.M[i]);
      double_swap(&backup_transformation.R[i], &transformation.R[i]);
    }

  vector_swap(backup_cursor, cursor);
  cursor_set(cursor[0], cursor[1], cursor[2]);

  float_swap( &backup_cursor_step, &cursor_step);
  cursor_set_step( cursor_step);

 for(i=0; i<4; i++)
   {
     float_swap(&backup_light[i], &light[i]);
   }

 int_swap(&backup_two_lights, &two_lights);
 
 int_swap(&backup_background, &background);
 int_swap(&backup_current_color, &current_color);

 {
   enum Context tmp;
   tmp=backup_context;
   backup_context= context;
   context=tmp;
 }
 vector_swap(backup_et0_marker,et0_marker);
 vector_swap(backup_t1_marker, t1_marker); 
 context_switch(context);
 
 float_swap(&backup_move_step, &move_step);
}






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
void float_zeroes(int n, float v[])
{
  int i;
  for(i=0; i<n; i++) v[i]=0.0;
}


double matrix3_det(float m[3][3])
{
return    m[0][2]*( m[1][0]*m[2][1]-m[2][0]*m[1][1] )
         -m[1][2]*( m[0][0]*m[2][1]-m[0][1]*m[2][0] )
         +m[2][2]*( m[0][0]*m[1][1]-m[0][1]*m[1][0] );

}

void matrix3_vector_mult(float m[3][3], float v[3], float out[3])
{
  float r[3];
  r[0]=m[0][0]*v[0]+m[0][1]*v[1]+m[0][2]*v[2];
  r[1]=m[1][0]*v[0]+m[1][1]*v[1]+m[1][2]*v[2];
  r[2]=m[2][0]*v[0]+m[2][1]*v[1]+m[2][2]*v[2];
  vectorcpy(out, r);
}

void matrix_init(float m[16])
{
  float_zeroes(16, m);
  m[3]=m[7]=m[11]=m[15]=1;
}

void double_zeroes(int n, double v[])
{
  int i;
  for(i=0; i<n; i++) v[i]=0.0;
}


Bool vector_eq(float  a[], float b[])
{
  return a[0]==b[0] && a[1]==b[1] && a[2]==b[2];
}

void vector_add(float a[], float b[], float result[])
{
  result[0]=a[0]+b[0];
  result[1]=a[1]+b[1];
  result[2]=a[2]+b[2];
}

void vector_sub(float a[], float b[], float result[])
{
  result[0]=a[0]-b[0];
  result[1]=a[1]-b[1];
  result[2]=a[2]-b[2];
}

void vector_scale( float s, float v[])
{
  v[0]*=s;  v[1]*=s; v[2]*=s;
}

void vectorcpy(float dst[], float src[])
{
  dst[0]=src[0];
  dst[1]=src[1];
  dst[2]=src[2];
}


void vector_product(float a[], float b[], float r[])
{
  float result[3];
  result[0]=a[1]*b[2]-a[2]*b[1];
  result[1]=a[2]*b[0]-a[0]*b[2];
  result[2]=a[0]*b[1]-a[1]*b[0];
  vectorcpy(r, result);
}

double scalar_product(float a[3], float b[3])
{
  return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

double vector_length(float a[3])
{
  return sqrt(scalar_product(a,a));
}

double vector_distance(float a[3], float b[3])
{
  float c[3];
  vector_sub(a,b,c);
  return vector_length(c);
}

void vector_normalize(float v[])
{
  float sp=scalar_product(v,v);
  if(sp==0) return;  /*  zero length vector */
  sp=sqrt(sp); 
  v[0]/=sp;  v[1]/=sp;  v[2]/=sp;
}





 /* /// VECTOR I/O */

void vector_fprintf(FILE *stream, float v[3])
{
  fprintf(stream, "%f\n", v[0]);
  fprintf(stream, "%f\n", v[1]);
  fprintf(stream, "%f\n\n", v[2]);
} 

void vector_fscanf(FILE *stream, float v[3])
{
  fscanf(stream, "%f", &v[0]);
  fscanf(stream, "%f", &v[1]);
  fscanf(stream, "%f", &v[2]);
} 




 /* / CONSTRUCTIVE METHODS */

void find_group_center(int g, float X[3])
{
  int i,n; 


  n=group_size(g, group, vertex_used);
  if(n==0)
    {
      printf("FIND GROUP CENTER: group contains no vertices !!\n");
      return;
    }

  float_zeroes(3, X);
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==g)
      vector_add(vertex[i], X,X);

  vector_scale(1.0/n, X);
}



void line_triangle_intersection( float A[3], float B[3],
			    float C[3], float D[3], float E[3],
			    float X[3])
{
  double t;
  line_triangle_solve(A,B, C,D,E, X, &t);
}



Bool line_triangle_solve( float A[3], float B[3],
			    float C[3], float D[3], float E[3],
			    float X[3], double *t)
{
   /*  sets X to the intersection of line AB with triangle CDE */
  float m[3][3];
  double d, d1;

  if(vector_eq(A,B))
    {
      printf("intersection of AB with CDE not found: A=B !!!\n");
      return False;
    }

  if(vector_eq(C,D) ||vector_eq(C,E) || vector_eq(D,E) )
    {
      printf("intersection of AB with CDE not found: CDE is not a triangle !!!\n");
      return False;
    }


  vector_sub(D,C, m[0]);
  vector_sub(E,C, m[1]);
  vector_sub(A,B, m[2]);

  d=matrix3_det(m);

  if(d==0)
    {
      printf("intersection of AB with CDE not found: AB and CDE are parallel !!!\n");
      return False;    
    }


  vectorcpy(X, m[2]);
  vector_sub(A,C, m[2]);
  d1=matrix3_det(m);
  *t=d1/d;
  vector_scale(-d1/d, X);
  vector_add(A,X, X);
  return True;

} 


Bool triangle_triangle_intersection( float A1[3],float A2[3], float A3[3],
				     float B1[3],float B2[3], float B3[3],
				     float E1[3],float E2[3]
				     )
{
  float M[3][3], NA[3], NB[3],
    B_1[3], B_2[3], B_3[3], X1[3], X2[3],
    A_1[3], A_2[3], A_3[3], Y1[3], Y2[3];
  double det1,det2, det3, t1, t2;
  triangle_normal_vector(A1, A2, A3, NA);
  if(scalar_product(NA,NA)==0) 
    {
      return False;
    }
  
  triangle_normal_vector(B1, B2, B3, NB);
  if(scalar_product(NB,NB)==0) 
    {
      return False;
    }



  vector_sub(A2,A1, M[0]);
  vector_sub(A3,A1, M[1]);

  vector_sub(B1,A1, M[2]);
  det1=matrix3_det(M);

  vector_sub(B2,A1, M[2]);
  det2=matrix3_det(M);

  vector_sub(B3,A1, M[2]);
  det3=matrix3_det(M);


  if( (det1<=0 && det2<=0 && det3<=0) ||
      (det1>=0 && det2>=0 && det3>=0)
      )
    {
       /*  printf("all vertices of B are on the same side of plane A1,A2,A3\n"); */
      return False;
    }

  if( (det1<0 && det2>=0 && det3>=0) ||
      (det1>0 && det2<=0 && det3<=0) 
      )
    {
      vectorcpy(B_1,B1);
      vectorcpy(B_2,B2);
      vectorcpy(B_3,B3);
    }
  else
  if( (det2<0 && det1>=0 && det3>=0) ||
      (det2>0 && det1<=0 && det3<=0) 
      )
    {
      vectorcpy(B_1,B2);
      vectorcpy(B_2,B1);
      vectorcpy(B_3,B3);
    }
  else
  if( (det3<0 && det2>=0 && det1>=0) ||
      (det3>0 && det2<=0 && det1<=0) 
      )
    {
      vectorcpy(B_1,B3);
      vectorcpy(B_2,B2);
      vectorcpy(B_3,B1);
    }

   /*  B_1 is on the other side of the plane A1, A2, A3 than B_2, B_3. */

  line_triangle_intersection(B_1,B_2, A1,A2,A3, X1);
  line_triangle_intersection(B_1,B_3, A1,A2,A3, X2);



  vector_sub(B2,B1, M[0]);
  vector_sub(B3,B1, M[1]);

  vector_sub(A1,B1, M[2]);
  det1=matrix3_det(M);

  vector_sub(A2,B1, M[2]);
  det2=matrix3_det(M);

  vector_sub(A3,B1, M[2]);
  det3=matrix3_det(M);


  if( (det1<=0 && det2<=0 && det3<=0) ||
      (det1>=0 && det2>=0 && det3>=0)
      )
    {
       /*  printf("all vertices of A are on the same side of plane B1,B2,B3\n"); */
      return False;
    }

  if( (det1<0 && det2>=0 && det3>=0) ||
      (det1>0 && det2<=0 && det3<=0)
      )
    {
      vectorcpy(A_1,A1);
      vectorcpy(A_2,A2);
      vectorcpy(A_3,A3);
    }
  else
  if( (det2<0 && det1>=0 && det3>=0) ||
      (det2>0 && det1<=0 && det3<=0)
      )
    {
      vectorcpy(A_1,A2);
      vectorcpy(A_2,A1);
      vectorcpy(A_3,A3);
    }
  else
  if( (det3<0 && det2>=0 && det1>=0) ||
      (det3>0 && det2<=0 && det1<=0)
      )
    {
      vectorcpy(A_1,A3);
      vectorcpy(A_2,A2);
      vectorcpy(A_3,A1);
    }
   /*  A_1 is on the other side of the plane B1, B2, B3 than A_2, A_3. */

  vector_add(A_1, NA, NA);
  line_triangle_solve(X1,X2, A_1,A_2,NA, Y1, &t1);
  line_triangle_solve(X1,X2, A_1,A_3,NA, Y2, &t2);

  if(t2<t1)
    {
      double_swap(&t1, &t2);
      vector_swap(Y1, Y2);
    }

  if( t2<=0  || t1>=1 )
    {
       /*  printf("t2==%lf<=0  || t1==%lf>=1\n", t2, t1); */
      return False;
    }


  if(t1>0)
    vectorcpy(E1, Y1);
  else
    vectorcpy(E1, X1);

  if(t2<1)
    vectorcpy(E2, Y2);
  else
    vectorcpy(E2, X2);


  return True;

   /* / DOKONCZ ... */

}


void group_tt_intersection(int g1, int g2)
{
  int i,i1,i2, n1, n2, g_new;
  int *t1, *t2;
  float E1[3],E2[3];

  if(g1==g2)
    {
      printf("Intersection with the same group ?!\n");
      return;
    }
  n1=group_internal_triangles(g1);
  n2=group_internal_triangles(g2);
  if(n1==0 || n2==0)
    {
      printf("At least one of the groups contains no internal triangles\n");
      return;
    }


  backup();

  t1=(int*) malloc(n1*sizeof(int));
  t2=(int*) malloc(n2*sizeof(int));

  if(t1==NULL || t2==NULL) 
    {
      printf("Out of memory !!!\n");
      free(t1);
      free(t2);
      return;
    }

  i1=i2=0;
  for(i=0; i<triangle_top; i++)
    if(group[triangle[i][0]]==g1 &&
       group[triangle[i][1]]==g1 &&
       group[triangle[i][2]]==g1 
       )
      {
	*(t1+i1)=i;
	i1++;
      }
    else
    if(group[triangle[i][0]]==g2 &&
       group[triangle[i][1]]==g2 &&
       group[triangle[i][2]]==g2 
       )
      {
	*(t2+i2)=i;
	i2++;
      }

  g_new=group_max(group, vertex_used)+1;
  group_change_current(g_new, &group_current, group, vertex_used );

  for(i1=0; i1<n1; i1++)
    for(i2=0; i2<n2; i2++)
	if(triangle_triangle_intersection(
					  vertex[triangle[*(t1+i1)][0]],
					  vertex[triangle[*(t1+i1)][1]],
					  vertex[triangle[*(t1+i1)][2]],
					  vertex[triangle[*(t2+i2)][0]],
					  vertex[triangle[*(t2+i2)][1]],
					  vertex[triangle[*(t2+i2)][2]],
					  E1,E2
					  )
	   )
	  edge_set_color( edge_vector_insert(E1, E2), current_color);

  free(t1);
  free(t2);
  group_change_current(g_new, &group_current, group, vertex_used );
  if(!group_restricted)
    {
      group_restricted_switch();
      printf("(press <1> to switch back to GROUP RESTRICTED: OFF)\n");
    }


}


void constr_scale(float A[3], float B[3], float C[3], float D[3],
	     float fixed_point[3], 
	     int g)
      /*  scales group by |AB|/|CD| */
{
  double lAB,lCD,s;
  float AB[3], CD[3];
  vector_sub(B, A, AB);
  vector_sub(D, C, CD);
  lAB=vector_length(AB);
  lCD=vector_length(CD);
  if(lCD==0) 
    {
      printf("scaling refused: |CD| = 0 !!!\n");
      return;
    }
  s=lAB/lCD;
  
  if(s== 1.0)
    {
      printf("scaling by 1 does not change anything !!!\n");
      return;
    }

  backup();
  group_scale(g, s, s, s, fixed_point, group, vertex, vertex_used);
}


void constr_scale_in_direction(float A[3], float B[3], float C[3], float D[3],
			  float E[3], float F[3], float fixed_point[3], 
			  int g)
      /*  scales group by |AB|/|CD| in direction EF */
{
  double lAB,lCD,s;
  float AB[3], CD[3], EF[3], v[3], tmp[3];
  double sp;
  int i;

  if(vector_eq(E,F))
    {
      printf("scaling in direction EF refused: EF = 0 !!!\n");
      return;
    }

  vector_sub(B, A, AB);
  vector_sub(D, C, CD);
  vector_sub(F, E, EF);
  vector_normalize(EF);

  lAB=vector_length(AB);
  lCD=vector_length(CD);
  if(lCD==0) 
    {
      printf("scaling refused: |CD| = 0 !!!\n");
      return;
    }
  s=lAB/lCD;

  if(s== 1.0)
    {
      printf("scaling by 1 does not change anything !!!\n");
      return;
    }

  backup();  
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==g)
      {
	vector_sub(vertex[i], fixed_point, v);
	sp=scalar_product(EF,v);
	vectorcpy(tmp, EF);
	vector_scale((s-1)*sp, tmp);
	vector_add(v,tmp, v);
	vector_add(v, fixed_point, vertex[i]);
      }

}



Bool find_three_point_transform(float P1[3], float P2[3], float P3[3], 
		     float Q1[3], float Q2[3], float Q3[3],
		     float mv[3], float R[3][3])
      /*  assumption: for each Pi!=Pj and Qi!=Qj */
{
  float u[3][3], w[3][3];
  int i,j,k;

  vector_sub(P2,P1, u[0]);
  vector_sub(P3,P1, u[1]);
  vector_product(u[0], u[1], u[1]);  /*  u[1] = (P2-P1)x(P3-P1) */
  vector_normalize(u[0]);
  vector_normalize(u[1]);
  vector_product(u[0], u[1], u[2]);  /*  u[2] |_ u[1] |_ u[2] */

  if(vector_length(u[2])==0) 
    {
      printf("three points transformation: the first three points are colinear !!!\n");
      return False;
    }

  vector_sub(Q2,Q1, w[0]);
  vector_sub(Q3,Q1, w[1]);
  vector_product(w[0], w[1], w[1]);  /*  w[1] = (Q2-Q1)x(Q3-Q1) */
  vector_normalize(w[0]);
  vector_normalize(w[1]);
  vector_product(w[0], w[1], w[2]);  /*  w[2] |_ w[1] |_ w[2] */
  if(vector_length(w[2])==0) 
    {
      printf("three points transformation: the last three points are colinear !!!\n");
      return False;
    }


  for(i=0; i<3; i++)
    for(j=0; j<3; j++)
      {
	R[i][j]=0;
	for(k=0; k<3; k++)
	  R[i][j]+=u[k][j]*w[k][i];
      }

  matrix3_vector_mult(R, P1, mv);
  vector_sub(Q1, mv, mv);  /*  mv = Q1 - R*P1 */
  return True;

}


void three_point_transform(float P1[3], float P2[3], float P3[3], 
		      float Q1[3], float Q2[3], float Q3[3],
		       int g)
      /*  assumption: for each Pi!=Pj and Qi!=Qj */
{
  float  mv[3], R[3][3];

  if(find_three_point_transform(P1, P2, P3, 
				Q1, Q2, Q3,
				mv, R))
    {
      backup();
      group_transform(g,mv,R);
    }
}




 /*  FOLDING */


Bool find_centered_folding(float A1[3],float A2[3],
			    float B1[3],float B2[3],
			    float K[3],
			    float V[3])
{
  double eps= 1e-14;
  int swap=0;  /*  0 - no swap, 1 - swaped xy, 2 - swaped yz  */
  double m,p,q, A2B2, A1B1, a, b, c, delta, x1, y1, z1, d1,d2;
  float matrix[3][3];

  A2B2=scalar_product(A2,B2);
  A1B1=scalar_product(A1,B1);



  m=A2[2]*A1[0]-A1[2]*A2[0];

  if(fabs(m)<eps)
    {
      float_swap(&A1[0], &A1[1]);
      float_swap(&A2[0], &A2[1]);
      float_swap(&B1[0], &B1[1]);
      float_swap(&B2[0], &B2[1]);
      float_swap(&K[0], &K[1]);
      m=A2[2]*A1[0]-A1[2]*A2[0];
      swap=1;
    }

  if(fabs(m)<eps)
    {
      float_swap(&A1[0], &A1[1]);
      float_swap(&A2[0], &A2[1]);
      float_swap(&B1[0], &B1[1]);
      float_swap(&B2[0], &B2[1]);
      float_swap(&K[0], &K[1]);
      float_swap(&A1[2], &A1[1]);
      float_swap(&A2[2], &A2[1]);
      float_swap(&B1[2], &B1[1]);
      float_swap(&B2[2], &B2[1]);
      float_swap(&K[2], &K[1]);
      m=A2[2]*A1[0]-A1[2]*A2[0];
      swap=2;
    }



  if(fabs(m)<eps)
    {
      printf("FOLDING REFUSED: the axes are colinear !!\n");
      printf("m=%e\n", m);
      return False;
    }

   /*  m!=0  */

  p=(A1[1]*A2[0]-A2[1]*A1[0])/m;
  q=(A2B2*A1[0]-A1B1*A2[0])/m;

  a=(1+p*p)*A2[0]*A2[0]+(A2[1]+p*A2[2])*(A2[1]+p*A2[2]);
  b=2*(p*q*A2[0]*A2[0]-(A2B2-q*A2[2])*(A2[1]+p*A2[2]));
  c=(q*q-1)*A2[0]*A2[0]+(A2B2-q*A2[2])*(A2B2-q*A2[2]);

  if(fabs(a)<eps)
    {
      a=(1+p*p)*A1[0]*A1[0]+(A1[1]+p*A1[2])*(A1[1]+p*A1[2]);
      b=2*(p*q*A1[0]*A1[0]-(A1B1-q*A1[2])*(A1[1]+p*A1[2]));
      c=(q*q-1)*A1[0]*A1[0]+(A1B1-q*A1[2])*(A1B1-q*A1[2]);
    }

  if(fabs(a)<eps)
    {
    printf("FOLDING REFUSED: the axes are colinear !!!\n");
    return False;
    }

   /*  a!=0 */

  delta= b*b-4*a*c;

  if(delta<0) 
    {
      printf("FOLDING FAILED: probably too large angle between rotated lines !!!\n");
      return False;
    }

  y1= (-b-sqrt(delta))/(2*a);
  z1= p*y1+q;
  x1= (fabs(A2[0]) > fabs(A1[0])) ?
    (A2B2-y1*A2[1]-z1*A2[2])/A2[0]:
    (A1B1-y1*A1[1]-z1*A1[2])/A1[0];
 

  vectorcpy(matrix[0], A1);
  vectorcpy(matrix[1], A2);
  vectorcpy(matrix[2], K);

  d1=matrix3_det(matrix);

  matrix[2][0]=x1;
  matrix[2][1]=y1;
  matrix[2][2]=z1;

  d2=matrix3_det(matrix);

  if(d1*d2 < 0)
    {
      y1= (-b+sqrt(delta))/(2*a);
      z1= p*y1+q;
      x1= (fabs(A2[0]) > fabs(A1[0])) ?
	(A2B2-y1*A2[1]-z1*A2[2])/A2[0]:
	(A1B1-y1*A1[1]-z1*A1[2])/A1[0];
    }


  V[0]=x1;
  V[1]=y1;
  V[2]=z1;

  switch(swap)
    {
    case 1:
      float_swap(&A1[0], &A1[1]);
      float_swap(&A2[0], &A2[1]);
      float_swap(&B1[0], &B1[1]);
      float_swap(&B2[0], &B2[1]);
      float_swap(&K[0], &K[1]);
      float_swap(&V[0], &V[1]);
      break;

    case 2:
      float_swap(&A1[2], &A1[1]);
      float_swap(&A2[2], &A2[1]);
      float_swap(&B1[2], &B1[1]);
      float_swap(&B2[2], &B2[1]);
      float_swap(&K[2], &K[1]);
      float_swap(&V[2], &V[1]);
      break;

    }

  return True;
   /*  DOKONCZYC ... */


}




Bool find_folding(float A[3],float B[3],float C[3],
		  float D[3],float E[3],float F[3], 
		  float V[3])
     /* set V so that AV is the line, where AD meets AE while
        AD and AE are rotarted around AB and AC respectively,
	and F and V are on the same side of the plane ABC.
      */
{
  float A1[3], A2[3], B1[3], B2[3], K[3];

  if(vector_eq(A,B))
    {
      printf("FOLDING REFUSED: A=B !!!\n");
      return False;
    }
  vector_sub(B,A, A1);
  vector_normalize(A1);

  if(vector_eq(A,C))
    {
      printf("FOLDING REFUSED: A=C !!!\n");
      return False;
    }
  vector_sub(C,A, A2);
  vector_normalize(A2);

  if(vector_eq(A,D))
    {
      printf("FOLDING REFUSED: A=D !!!\n");
      return False;
    }
  vector_sub(D,A, B1);
  vector_normalize(B1);

  if(vector_eq(A,E))
    {
      printf("FOLDING REFUSED: A=E !!!\n");
      return False;
    }
  vector_sub(E,A, B2);
  vector_normalize(B2);

  vector_sub(F,A, K);

  if(find_centered_folding(A1,A2, B1,B2, K,V))
    {
      vector_add(A,V, V);
      return True;
    }
  else return False;

}



 /* /// TRANSFORMATION ////////////////////////////////// */




void transformation_fprintf(FILE* s, struct Transformation* t)
{
  int i;
  fprintf(s,"\n");
  vector_fprintf(s, t->mov );
  for(i=0; i<16; i++)
    fprintf(s, "%e\n", t->M[i]);
  for(i=0; i<16; i++)
    fprintf(s, "%e\n", t->R[i]);
  fprintf(s,"\n");

}

void transformation_fscanf(FILE* s, struct Transformation* t)
{
  int i;
  vector_fscanf(s, t->mov );
  for(i=0; i<16; i++)
    fscanf(s, "%le", &(t->M[i]));
  for(i=0; i<16; i++)
    fscanf(s, "%le", &(t->R[i]));

}


void transformation_init(struct Transformation* t)
{
  t->mov[0]=0.0;
  t->mov[1]=0.0;
  t->mov[2]=-screen_distance;
  transformation_init_orientation(t);
}

void transformation_init_orientation(struct Transformation* t)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGetDoublev( GL_MODELVIEW_MATRIX, t->M );
  glGetDoublev( GL_MODELVIEW_MATRIX, t->R );

}


void rotate(double angle, double x, double y, double z, struct Transformation* t)
{
  glMatrixMode(GL_MODELVIEW);


  glLoadIdentity();
  glRotated(angle, x,y,z);
  glMultMatrixd(t->M);
  glGetDoublev( GL_MODELVIEW_MATRIX, t->M );

  glLoadMatrixd( t->R );
  glRotated(-angle, x,y,z);
  glGetDoublev( GL_MODELVIEW_MATRIX, t->R );
    

}


void transformation_scale(double s,struct Transformation* t, float cursor[16])
{
  float tmp1[16], tmp2[16];

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp1 );
  glPopMatrix();

  scale*=s;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp2 );
  glPopMatrix();

  vector_sub(tmp2, tmp1, tmp1);
  vector_sub(t->mov, tmp1, t->mov);

}

void transformation_rotate(double angle, double x, double y, double z, 
		      struct Transformation* t, float cursor[16])
{
  float dist, tmp[16];

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp );
  glPopMatrix();


  vector_add(tmp, t->mov, tmp);
  dist=sqrt(scalar_product(tmp,tmp));

  rotate(angle, x,y,z, t);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp );
  glPopMatrix();
  
  t->mov[0]= -tmp[0];
  t->mov[1]= -tmp[1];
  t->mov[2]= -tmp[2]-dist;


}


void transformation_move(float x, float y, float z, struct Transformation* t)
{
  t->mov[0]+=x;
  t->mov[1]+=y;
  t->mov[2]+=z;
}



void find_versor(float x, float y, float z, float e[])
{
  
  float max, v[16];
  int  imax;
  float_zeroes(16,v);
  v[0]=x; v[1]=y; v[2]=z; v[3]=1.0;
 
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd(transformation.R);
  glMultMatrixf(v);
  glGetFloatv( GL_MODELVIEW_MATRIX, v);
  glPopMatrix();

  imax=0; max=fabs(v[0]);
  if(fabs(v[1])>max) { imax=1; max=fabs(v[1]); }
  if(fabs(v[2])>max) { imax=2; max=fabs(v[2]); }


  float_zeroes(3, e);
  e[imax]= (v[imax]>0) ? 1.0 : -1.0 ;

}

void sort_dimmensions(double x, double y, double z, struct Transformation *t,
		 int d[3])
{
  float v[16];

  float_zeroes(16,v);
  v[0]=x; v[1]=y; v[2]=z; v[3]=1.0;
 
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd(t->R);
  glMultMatrixf(v);
  glGetFloatv( GL_MODELVIEW_MATRIX, v);
  glPopMatrix();

  d[0]=0; d[1]=1; d[2]=2;


  if(fabs(v[d[0]]) < fabs(v[d[1]])) int_swap(&d[0], &d[1]);
  if(fabs(v[d[1]]) < fabs(v[d[2]])) int_swap(&d[1], &d[2]);
  if(fabs(v[d[0]]) < fabs(v[d[1]])) int_swap(&d[0], &d[1]);

}




 /* //// CLIPPING PLANES ///// */


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





 /* /// LIGHT /////////////////////////////////// */


void set_light(float light[16])
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd(transformation.R);
  glMultMatrixf(basic_light);
  glGetFloatv( GL_MODELVIEW_MATRIX, light);
  glPopMatrix();
  printf("light=( %f, %f, %f, %f )\n", light[0], light[1], light[2], light[3]);
}

void switch_second_light()
{
  two_lights= !two_lights;
  if(two_lights) printf("Two opposite lights mode\n");
  else printf("One light mode\n");
}



 /* // VERTEX FUNCTIONS */


int vertex_number_of(int vertex_used[VERTEX_MAX])
{
  int i, s=0;
  for(i=0; i< VERTEX_MAX; i++)
    if(vertex_used[i]) s++;
  return s;
}

int vertex_insert(float v[])
{
  int i;

  i=vertex_find(v);
  if(i!=-1) return i;

  for(i=0; 
      i<VERTEX_MAX && 
	vertex_used[i]>0;
      i++) ;

  if(i>=VERTEX_MAX)
    {
      printf("too many vertices !!!\n");
      return -1;
    }

  vectorcpy(vertex[i], v);

   /*  vertex_used[i]++; */
  return i;
}


int vertex_find(float v[])
{
  int i;
  for(i=0; i<VERTEX_MAX && !(vector_eq(v,vertex[i]) && group[i]== group_current); i++);
  return (i<VERTEX_MAX)? i : -1 ;
}

int vertex_used_find(float v[])
{
  int i;
  for(i=0; i<VERTEX_MAX && (!vertex_used[i]>0 || !vector_eq(v,vertex[i])); i++);
  return (i<VERTEX_MAX)? i : -1 ;
}

int vertex_find_nearest(float v[])
{
  int i, found=-1;
  float tmp[3], dist, dist1;
  for(i=0; i<VERTEX_MAX; i++)
   if(vertex_used[i]>0)
     {
       vector_sub(v, vertex[i], tmp);
       dist1=scalar_product(tmp,tmp);
       if(found==-1 || dist1<dist) 
	 {
	   found=i;
           dist=dist1;
	 }
     }
  return found ;
}


int vertex_compress(float vertex[][3],
                    int vertex_used[],
		    int edge_top, int edge[][3],
		    int triangle_top, int triangle[][4], float triangle_normal[][3],
		    int group[])
{
  int p[VERTEX_MAX];
  int i, first_unused=0, last_used=VERTEX_MAX-1;

  for(i=0; i<VERTEX_MAX; i++) p[i]=i;
  
  while(first_unused<=last_used)
    {
      while(first_unused<= last_used && vertex_used[first_unused]>0) first_unused++;
      while(last_used>=first_unused && vertex_used[last_used]<=0) last_used--;
      if(first_unused<last_used)
	{
	  p[last_used]=first_unused;
          vectorcpy(vertex[first_unused], vertex[last_used]);
          vertex_used[first_unused]=vertex_used[last_used];
	  group[first_unused]=group[last_used];
          vertex_used[last_used]=0;
	}
    }

  for(i=0; i<edge_top; i++)
    {
      edge[i][0]=p[edge[i][0]];
      edge[i][1]=p[edge[i][1]];
      edge_sort(edge[i]);
    }

  for(i=0; i<triangle_top; i++)
    {
      triangle[i][0]=p[triangle[i][0]];
      triangle[i][1]=p[triangle[i][1]];
      triangle[i][2]=p[triangle[i][2]];
      triangle_sort(triangle[i]);
      triangle_normal_vector( vertex[triangle[i][0]], 
                              vertex[triangle[i][1]], 
			      vertex[triangle[i][2]],
			      triangle_normal[i]);
    }

  return last_used+1;  /*  size == first free */

}


void vertex_get_range(float vertex[][3], int vertex_used[], 
		 float vmin[3], float vmax[3])
{
  int i;
  for(i=0; i<VERTEX_MAX && !vertex_used[i]; i++);
  if(i>VERTEX_MAX)
    {
      float_zeroes(3,vmin);
      float_zeroes(3,vmax);
      return;
    }

  vectorcpy(vmin, vertex[i]);
  vectorcpy(vmax, vertex[i]);

  while(i<VERTEX_MAX)
    {
      if(vertex_used[i])
	{
	  if(vertex[i][0]<vmin[0]) vmin[0]=vertex[i][0];
	  else
	    if(vertex[i][0]>vmax[0]) vmax[0]=vertex[i][0];

	  if(vertex[i][1]<vmin[1]) vmin[1]=vertex[i][1];
	  else
	    if(vertex[i][1]>vmax[1]) vmax[1]=vertex[i][1];

	  if(vertex[i][2]<vmin[2]) vmin[2]=vertex[i][2];
	  else
	    if(vertex[i][2]>vmax[2]) vmax[2]=vertex[i][2];
	}
      i++;
    }
}


 /* // I/O */

void vertex_fprintf(FILE* s, int n, float v[][3])
{
  int i;
  fprintf(s, "\n%d\n", n);
  for(i=0; i<n; i++) 
    vector_fprintf(s, v[i]);
  fprintf(s, "\n");

}

int vertex_fscanf(FILE* s,  float v[][3], int v_used[])
{
  int i,n;
  for(i=0; i<VERTEX_MAX; i++) v_used[i]=0;
  fscanf(s, "%d", &n);
  for(i=0; i<n; i++) 
    vector_fscanf(s, v[i]);
  return n;
}



 /* // EDGE STACK */



void edge_fprintf(FILE* s, int edge_top, int edge[][3] )
{
  int i;
  fprintf(s, "\n%d\n\n", edge_top);
  for(i=0; i<edge_top; i++)
    {
      fprintf(s, "%d\n", edge[i][0]);
      fprintf(s, "%d\n", edge[i][1]);
      fprintf(s, "%d\n\n", edge[i][2]);
    }
}

void edge_fscanf(FILE* s, int* edge_top, int edge[][3], int vertex_used[])
{
  int i;
  fscanf(s, "%d", edge_top);
  for(i=0; i< *edge_top; i++)
    {
      fscanf(s, "%d", &edge[i][0]);
      vertex_used[edge[i][0]]++;
      fscanf(s, "%d", &edge[i][1]);
      vertex_used[edge[i][1]]++;
      fscanf(s, "%d", &edge[i][2]);  /*  color */
    }
}



Bool edge_eq(int a[], int b[])
{
  return a[0]==b[0] && a[1]==b[1];
}

int edge_compare(const void* a, const void* b)
      /*  for qsort(...) */
{
  if( *((int*) a) < *((int*) b) ||
      ( *((int*) a) == *((int*) b) &&
	*(1+(int*) a) < *(1+(int*) b)
	)
      ) return -1;
  else
    if( *((int*) a) == *((int*) b) &&
	*(1+(int*) a) == *(1+(int*) b)
	) return 0;
    else return 1;

}


void edgecpy(int dest[],int src[])
{
  dest[0]=src[0]; dest[1]=src[1];
  dest[2]=src[2];  /*  color */
} 

void edge_sort(int e[])
{
  int t;
  if(e[0]>e[1]) {t=e[0]; e[0]=e[1]; e[1]=t;}
}


int edge_insert(int e[])
{
  int i;

  if(e[0]==e[1]) return -1;  /*  degenerate !!! */
  edge_sort(e);

  for(i=0; i<edge_top && !edge_eq(e,edge[i]); i++);

  if(i< edge_top) return i;

  if(i>= EDGE_MAX)
    {
      printf("too many egdes !!!\n");
      return -1;
    }

   /*  CASE:  i == edge_top < EDGE_MAX */
  edge_top++;
  edgecpy(edge[i],e);
  vertex_used[e[0]]++;
  vertex_used[e[1]]++;
  return i;
}

void edge_delete(int i)
{
  if(i>=edge_top) return;
  vertex_used[edge[i][0]]--;
  vertex_used[edge[i][1]]--;
  edge_top--;
  if(i<edge_top)
    {
      edgecpy(edge[i],edge[edge_top]);
    }

}


int edge_vector_insert(float a[], float b[])
{
  int e[3];
  e[0]=vertex_insert(a);
  if(e[0]==-1) 
    {
      printf("vector_edge_insert: failed !!!\n");
      return -1;
    }
  vertex_used[e[0]]++;  /*  temporarily block the vertex */
  
  e[1]=vertex_insert(b);

  vertex_used[e[0]]--;  /*  release the vertex */

  if(e[1]==-1) 
    {
      printf("vector_edge_insert: failed !!!\n");
      return -1;
    }

  return edge_insert(e);
}


void edge_set_color(int i, int color)
{
  if(i<0 || i>=EDGE_MAX) return;
  edge[i][2]= (color<COLOR_MAX) ? color : 0 ;
}



int edge_vector_delete(float a[], float b[])
{
  int i;
  Bool found;

  found=False;

  i=0; 
  while(!found && (i<edge_top))
    {
      found= 
	(vector_eq(vertex[edge[i][0]],a) && vector_eq(vertex[edge[i][1]],b)) ||
	(vector_eq(vertex[edge[i][0]],b) && vector_eq(vertex[edge[i][1]],a));
	
      if(!found) i++;
    }



  if(found) { edge_delete(i); return i;}
  else return -1;

}



int edge_find(int e[3])
{
  int i=0;
  while ( i<edge_top && !edge_eq(e,edge[i]) ) i++;
  return (i<edge_top) ? i : -1;
}


Bool edge_inside_group(int e[3], int g)
{
  return group[e[0]]== g && group[e[1]] == g;
}



void edge_delete_incident(float v[3])
{
  int i=0, prev;

  prev=edge_top;
  while(i<edge_top)
    {
      if( (!group_restricted || edge_inside_group(edge[i],group_current) ) &&
	  (
	   vector_eq(vertex[edge[i][0]],v) || 
	   vector_eq(vertex[edge[i][1]],v) 
	   ) 
	  )
	edge_delete(i);

      else i++;   
    }
  printf("%d EDGES DELETED.\n", prev-edge_top);

}


void edge_paint_incident(float v[3], int color)
{
  int i, n=0;
  for(i=0;i<edge_top; i++)
    if( (!group_restricted || edge_inside_group(edge[i],group_current) ) &&
	(
	 vector_eq(vertex[edge[i][0]],v) || 
	 vector_eq(vertex[edge[i][1]],v) 
	 ) 
	)
      {
	edge[i][2]=color;
	n++;
      }
  printf("%d EDGES PAINTED WITH COLOR %d.\n", n, color);
}


 /* // TRIANGLE  */


void triangle_fprintf(FILE* s, int triangle_top, int triangle[][4])
{
  int i;
  fprintf(s, "\n%d\n\n", triangle_top);
  for(i=0; i<triangle_top; i++)
    {
      fprintf(s, "%d\n", triangle[i][0]);
      fprintf(s, "%d\n", triangle[i][1]);
      fprintf(s, "%d\n", triangle[i][2]);
      fprintf(s, "%d\n\n", triangle[i][3]);
    }
}

void triangle_fscanf(FILE* s, int* triangle_top, int triangle[][4], 
		float triangle_normal[][3], float vertex[][3], int vertex_used[])
{
  int i;
  fscanf(s, "%d", triangle_top);
  for(i=0; i< *triangle_top; i++)
    {
      fscanf(s, "%d", &triangle[i][0]);
      vertex_used[triangle[i][0]]++;
      fscanf(s, "%d", &triangle[i][1]);
      vertex_used[triangle[i][1]]++;
      fscanf(s, "%d", &triangle[i][2]);
      vertex_used[triangle[i][2]]++;
      fscanf(s, "%d", &triangle[i][3]);
    }

  for(i=0; i< *triangle_top; i++)
    triangle_normal_vector(vertex[triangle[i][0]],
			       vertex[triangle[i][1]],
			       vertex[triangle[i][2]],
			       triangle_normal[i]);
}

Bool triangle_eq(int a[], int b[])
{
  return a[0]==b[0] && a[1]==b[1] && a[2]==b[2];
}

int triangle_compare(const void * a, const void *b)
{
  int ec;
  ec=edge_compare(a,b);
  if(ec!=0) return ec;
  else
    if(*(2+(int*)a)<*(2+(int*)b)) return -1;
       else 
       if(*(2+(int*)a )== *(2+(int*)b)) return 0;
	  else return 1;
}

void trianglecpy(int dest[],int src[])
{
  dest[0]=src[0]; dest[1]=src[1]; dest[2]=src[2];
  dest[3]=src[3];  /*  color */
} 

void triangle_sort(int e[])
{
  int t;
  if(e[0]>e[1]) {t=e[0]; e[0]=e[1]; e[1]=t;}
  if(e[1]>e[2]) {t=e[1]; e[1]=e[2]; e[2]=t;}
  if(e[0]>e[1]) {t=e[0]; e[0]=e[1]; e[1]=t;}
}


int triangle_insert(int e[])
{
  int i;

  triangle_sort(e);
  if(e[0]==e[1] || e[1]==e[2]) return -1;  /*  degenerate !!! */

  for(i=0; i<triangle_top && !triangle_eq(e,triangle[i]); i++);

  if(i< triangle_top) return i;

  if(i>= TRIANGLE_MAX)
    {
      printf("too many triangles !!!\n");
      return -1;
    }

   /*  CASE:  i == triangle_top < TRIANGLE_MAX */
  triangle_top++;
  trianglecpy(triangle[i],e);
  vertex_used[e[0]]++;
  vertex_used[e[1]]++;
  vertex_used[e[2]]++;
  triangle_set_normal(i);
  return i;
}


void triangle_delete(int i)
{
  if(i>=triangle_top) return;
  vertex_used[triangle[i][0]]--;
  vertex_used[triangle[i][1]]--;
  vertex_used[triangle[i][2]]--;

  triangle_top--;
  if(i<triangle_top)
    {
      trianglecpy(triangle[i],triangle[triangle_top]);
      triangle[i][3]=triangle[triangle_top][3];
      vectorcpy(triangle_normal[i],triangle_normal[triangle_top]);

    }

}



Bool triangle_inside_group(int e[4], int g)
{
  return group[e[0]]== g && group[e[1]] == g && group[e[2]] == g  ;
}



void triangle_delete_incident(float v[3])
{
  int i=0, prev;

  prev=triangle_top;
  while(i<triangle_top)
    {
      if( (!group_restricted || triangle_inside_group(triangle[i],group_current) ) &&
	  (
	   vector_eq(vertex[triangle[i][0]],v) || 
	   vector_eq(vertex[triangle[i][1]],v) || 
	   vector_eq(vertex[triangle[i][2]],v) 
	   ) 
	  )
	triangle_delete(i);

      else i++;   
    }
  printf("%d TRIANGLES DELETED.\n", prev-triangle_top);

}


void triangle_paint_incident(float v[3], int color)
{
  int i, n=0;
  for(i=0;i<triangle_top; i++)
    if( (!group_restricted || triangle_inside_group(triangle[i],group_current) ) &&
	(
	 vector_eq(vertex[triangle[i][0]],v) || 
	 vector_eq(vertex[triangle[i][1]],v) || 
	 vector_eq(vertex[triangle[i][2]],v) 
	 ) 
	)
      {
	triangle[i][3]=color;
	n++;
      }
  printf("%d TRIANGLES PAINTED WITH COLOR %d.\n", n, color);
}







int triangle_vector_insert(float a[], float b[], float c[])
{
  int e[4];

  e[0]=vertex_insert(a);
  if(e[0]==-1) 
    {
      printf("vector_triangle_insert: failed !!!\n");
      return -1;
    }

  vertex_used[e[0]]++;  /*  temporarily block the vertex */

  e[1]=vertex_insert(b);

  if(e[1]==-1) 
    {
      vertex_used[e[0]]--;  /*  release the vertex */
      printf("vector_triangle_insert: failed !!!\n");
      return -1;
    }

  vertex_used[e[1]]++;  /*  temporarily block the vertex */

  e[2]=vertex_insert(c);

  vertex_used[e[0]]--;  /*  release the vertex */
  vertex_used[e[1]]--;  /*  release the vertex */

  if(e[2]==-1) 
    {
      printf("vector_triangle_insert: failed !!!\n");
      return -1;
    }

  return triangle_insert(e);
}

void triangle_set_color(int i, int color)
{
  if(i<0 || i>triangle_top) return;
  triangle[i][3]= (color<COLOR_MAX) ? color : 0 ;
}

void triangle_set_normal(int i)
{
  float v[3], w[3];
  
  vector_sub( vertex[triangle[i][1]], vertex[triangle[i][0]], v);
  vector_sub( vertex[triangle[i][2]], vertex[triangle[i][0]], w);
  vector_product(v,w, triangle_normal[i]); 
  vector_normalize(triangle_normal[i]);
}

void triangle_normal_vector(float a[], float b[], float c[], float norm[])
{
  float v[3], w[3];
  vector_sub(b,a, v);
  vector_sub(c,a, w);
  vector_product(v,w, norm);
  vector_normalize(norm);
}


void triangle_recompute_normals()
{
  int i;
  for(i=0; i<triangle_top; i++)
    triangle_set_normal(i);
}



int triangle_vector_delete(float a[], float b[], float c[])
{
  int i;
  Bool found;

  found=False;

  i=0; 
  while(!found && (i<triangle_top))
    {
      found= 
	(
	 vector_eq(vertex[triangle[i][2]],c) &&
	 (
	  (vector_eq(vertex[triangle[i][0]],a) && vector_eq(vertex[triangle[i][1]],b)) ||
	  (vector_eq(vertex[triangle[i][0]],b) && vector_eq(vertex[triangle[i][1]],a))
	  )
	 )
	||
	(
	 vector_eq(vertex[triangle[i][2]],b) &&
	 (
	  (vector_eq(vertex[triangle[i][0]],a) && vector_eq(vertex[triangle[i][1]],c)) ||
	  (vector_eq(vertex[triangle[i][0]],c) && vector_eq(vertex[triangle[i][1]],a))
	  )
	 )
	||
	(
	 vector_eq(vertex[triangle[i][2]],a) &&
	 (
	  (vector_eq(vertex[triangle[i][0]],c) && vector_eq(vertex[triangle[i][1]],b)) ||
	  (vector_eq(vertex[triangle[i][0]],b) && vector_eq(vertex[triangle[i][1]],c))
	  )
	 )
	;

      if(!found) i++;
    }



  if(found) { triangle_delete(i); return i;}
  else return -1;

}




int triangle_find(int e[4])
{
  int i=0;
  while ( i<triangle_top && !triangle_eq(e,triangle[i]) ) i++;
  return (i<triangle_top) ? i : -1;
}




 /* ///////// GROUPS */


void group_mark(int gr)
{
  group_marker=gr;
  printf("MARKED GROUP: %d\n", group_marker);
}

int group_find(float v[3])
{
  int i,found=-1;
  for(i=0; found == -1 && i<VERTEX_MAX; i++)
    if(vertex_used[i] && vector_eq(v, vertex[i])) found=group[i];
  return found;
}


void group_regroup(int old, int new)
{
  int i;
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==old) group[i]=new;
  printf("veritces from group %d moved to group %d\n", old, new);
}


void group_remove_empty()
{
  int i, n1, n2, s, max, r;
  max=group_max(group, vertex_used);
  r=n1=n2=0;
  while(n1<=max)
    {
      s=0;
      for(i=0; i<VERTEX_MAX; i++)
	if(vertex_used[i] && group[i]==n1)
	  {
	    group[i]=n2;
	    s++;
	  }
      if(s>0)
	{
	  if(n1== group_current) 
	    group_change_current(n2, &group_current, group, vertex_used );
	  if(n1== group_marker) group_marker=n2; 
	  n2++;
	}
      else
	{
	if(n1== group_marker) group_marker=-1; 
	if(n1== group_current) group_current=-1; 
 	r++;
	}
      n1++;
    }
  if(group_current==-1) 
    group_change_current(n2, &group_current, group, vertex_used );
  printf("removed %d groups\n", r);
}

void group_restricted_switch()
{
  group_restricted=!group_restricted;
  printf("RESTRICTED TO GROUP:");
  if(group_restricted) printf(" ON\n");
  else printf(" OFF\n");
}

int group_max(int group[], int vertex_used[])
{
  int i, max=-1;
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]>max) max=group[i];
  return max;
}


int group_size(int g, int group[], int vertex_used[])
{
  int i, size=0;
  for(i=0; i<VERTEX_MAX; i++)
    if(group[i]==g && vertex_used[i]) size++;
  return size;
}


int group_incident_edges(int g)
{
  int i,s;
  s=0;
  for(i=0; i<edge_top; i++)
    if(group[edge[i][0]] == g ||
       group[edge[i][1]] == g
       ) s++;
  return s;
}

int group_internal_edges(int g)
{
  int i,s;
  s=0;
  for(i=0; i<edge_top; i++)
    if(group[edge[i][0]] == g &&
       group[edge[i][1]] == g
       ) s++;
  return s;
}


int group_incident_triangles(int g)
{
  int i,s;
  s=0;
  for(i=0; i<triangle_top; i++)
    if(group[triangle[i][0]] == g ||
       group[triangle[i][1]] == g ||
       group[triangle[i][2]] == g 
       ) s++;
  return s;
}

int group_internal_triangles(int g)
{
  int i,s;
  s=0;
  for(i=0; i<triangle_top; i++)
    if(group[triangle[i][0]] == g &&
       group[triangle[i][1]] == g &&
       group[triangle[i][2]] == g
       ) s++;
  return s;
}



void group_statistics(int g)
{
  printf("Group %d statistics:\n", g);
  printf(" vertices: %d\n", group_size(g,group,vertex_used));
  printf(" incident edges: %d\n", group_incident_edges(g));
  printf(" internal edges: %d\n", group_internal_edges(g));
  printf(" incident triangles: %d\n", group_incident_triangles(g));
  printf(" internal triangles: %d\n", group_internal_triangles(g));
  printf("\n");
}


void group_change_current(int g, int *group_current, int group[], int vertex_used[])
{
  int i;
  *group_current=g;
  for(i=0; i<VERTEX_MAX; i++) 
    if(!vertex_used[i]) group[i]=g;
  printf("CURRENT GROUP is: %d (with: %d vertices)\n", 
	 *group_current, group_size(*group_current, group, vertex_used)
	 );
}

void group_fprintf(FILE* s, int n, int group[])
{
  int i;
  fprintf(s,"\n%d\n\n",n);
  for(i=0; i<n; i++)
    fprintf(s,"%d\n", group[i]);

}

void group_fscanf(FILE* s, int group[])
{
  int i, n;
  fscanf(s,"%d\n",&n);
  for(i=0; i<n; i++)
    {
    fscanf(s,"%d", &group[i]);
    }
}




void group_rot90(int g, float x, float y, float z)
{
  int i;
  float v[3], tmp;
  float e[3];
  find_versor(x,y,z, e);
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==g)
      {
	if(e[0]==1 || e[0]==-1  )
	  {
	    vector_sub(vertex[i], cursor, v);
	    tmp=v[2];
	    v[2]=v[1];
	    v[1]=-tmp;
	    vector_add(v,cursor, vertex[i]);
	  }
	else
	  {
	    if(e[1]==1 || e[1]==-1 )
	      {
		vector_sub(vertex[i], cursor, v);
		tmp=v[2];
		v[2]=-v[0];
		v[0]=tmp;
		vector_add(v,cursor, vertex[i]);
	      }
	  else
	    if(e[2]==1 || e[2]==-1 )
	      {
		vector_sub(vertex[i], cursor, v);
		tmp=v[1];
		v[1]=-v[0];
		v[0]=tmp;
		vector_add(v,cursor, vertex[i]);
	      }
	  }
      }
}


void group_move(int g, float mv[3], 
	   int group[], float vertex[][3], int vertex_used[])
{
  int i;
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==g)
      vector_add(vertex[i],mv, vertex[i]);

}


void group_cursor_move(float x, float y, float z)
{
  float e[3];
  find_versor(x,y,z, e);
  vector_scale(cursor_step, e);
  group_move(group_current, e, group, vertex, vertex_used);
}





void group_scale(int g, double sx, double sy, double sz, float cursor[3],
	   int group[], float vertex[][3], int vertex_used[])
{
   /*  cursor is a fixed point  */

  int i;
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==g)
      {
	vertex[i][0]=sx*(vertex[i][0]-cursor[0])+cursor[0];
	vertex[i][1]=sy*(vertex[i][1]-cursor[1])+cursor[1];
	vertex[i][2]=sz*(vertex[i][2]-cursor[2])+cursor[2];
      }

}



void group_transform(int g, float mv[3], float R[3][3])
{
  int i;
      for(i=0; i<VERTEX_MAX; i++)
	if(vertex_used[i] && group[i]==g) 
	  {
	    matrix3_vector_mult(R, vertex[i], vertex[i]);
	    vector_add(vertex[i],mv, vertex[i]);
	  }

}


void group_extract_clipped()
{
  int i,n;
  n = group_max(group, vertex_used)+1;
  for(i=0; i<VERTEX_MAX; i++)
    if( vertex_used[i] && 
	(! group_restricted || group[i]== group_current) &&
	(! clipping_xyz.xmin_flag || vertex[i][0] >= clipping_xyz.xmin) &&
	(! clipping_xyz.xmax_flag || vertex[i][0] <= clipping_xyz.xmax) &&
	(! clipping_xyz.ymin_flag || vertex[i][1] >= clipping_xyz.ymin) &&
	(! clipping_xyz.ymax_flag || vertex[i][1] <= clipping_xyz.ymax) &&
	(! clipping_xyz.zmin_flag || vertex[i][2] >= clipping_xyz.zmin) &&
	(! clipping_xyz.zmax_flag || vertex[i][2] <= clipping_xyz.zmax) 
	)
      group[i]= n;
  group_change_current(n, &group_current, group, vertex_used);
  if(!group_restricted)
    {
      group_restricted_switch();
      printf("(press <1> to switch back to GROUP RESTRICTED: OFF)\n");
    }

}


void group_copy(Bool glue_edges, Bool glue_triangles)
{
  int i,j, size, e_top, t_top, e[2], t[3];
  int first_free, g1,g2;
  int dest[VERTEX_MAX];

  first_free=vertex_compress(vertex,
			     vertex_used,
			     edge_top, edge,
			     triangle_top, triangle, triangle_normal,
			     group);

  g1=group_current;
  size=group_size(g1, group, vertex_used);
  g2= group_max(group, vertex_used)+1;

  if(first_free+size>=VERTEX_MAX) 
    {
      printf("GRAPH COPY: vertex table overflow !!!\n");
      return;
    }



  j=0;
  for(i=0; i<first_free; i++)
    if(group[i]==g1) 
      {
	group[first_free+j]=g2;
	vectorcpy(vertex[first_free+j],vertex[i]);
	dest[i]=first_free+j;
	j++;
	if(glue_edges)
	  {
	    e[0]=i;
	    e[1]=dest[i];
	    edge_set_color(edge_insert(e), current_color);
	  }
      } 
  


  e_top= edge_top;
  for(i=0; i<e_top && edge_top<EDGE_MAX; i++)
    if(
       group[edge[i][0]]==g1 &&
       group[edge[i][1]]==g1
       )
      {
	e[0]=dest[edge[i][0]];
	e[1]=dest[edge[i][1]];
	edge_set_color(edge_insert(e), edge[i][2]);
	if(glue_triangles)
	  {
	    t[0]=edge[i][0];
	    t[1]=edge[i][1];
	    t[2]=dest[edge[i][0]];
	    triangle_set_color(triangle_insert(t), edge[i][2]);
	    t[0]=edge[i][1];
	    t[1]=dest[edge[i][0]];
	    t[2]=dest[edge[i][1]];
	    triangle_set_color(triangle_insert(t), edge[i][2]);

	  }
      }

  t_top= triangle_top;
  for(i=0; i<t_top && triangle_top<TRIANGLE_MAX; i++)
    if(
       group[triangle[i][0]]==g1 &&
       group[triangle[i][1]]==g1 &&
       group[triangle[i][2]]==g1 
       )
      {
	t[0]=dest[triangle[i][0]];
	t[1]=dest[triangle[i][1]];
	t[2]=dest[triangle[i][2]];
	triangle_set_color(triangle_insert(t), triangle[i][3]);
      }

  group_change_current(g2, &group_current, group, vertex_used); 
  printf("Use <Alt>+<Arrow kyes>/<Home>/<End> to move new object.\n");

}



void group_paint_edges(int g, int color)
{
   /*  PAINT EDGES INCLUDED INSIDE GROUP g */
  int i;
  for(i=0; i<edge_top; i++)
    if(
       group[edge[i][0]]== g &&
       group[edge[i][1]]== g 
       ) 
      edge[i][2]=color;
}


void group_paint_triangles(int g, int color)
{
   /*  PAINT TRIANGLES INCLUDED INSIDE GROUP g */
  int i;
  for(i=0; i<triangle_top; i++)
    if(
       group[triangle[i][0]]== g &&
       group[triangle[i][1]]== g &&
       group[triangle[i][2]]== g 
       ) 
      triangle[i][3]=color;
}





void group_delete_edges(int g)
{
   /*  REMOVE ALL EDGES INCIDENT TO A GROUP g */

  int i=0;
  while(i<edge_top)
    {
      if( 
	 group[ edge[i][0] ] == g ||
	 group[ edge[i][1] ] == g 
	 ) 
	edge_delete(i);
      else i++;
    }
}

void group_delete_triangles(int g)
{
   /*  REMOVE ALL EDGES INCIDENT TO A GROUP g */

  int i=0;
  while(i<triangle_top)
    {
      if( 
	 group[ triangle[i][0] ] == g ||
	 group[ triangle[i][1] ] == g ||
	 group[ triangle[i][2] ] == g 
	 ) 
	triangle_delete(i);
      else i++;
    }
}






 /* ////// CURSOR /////// */

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




 /* / REDUTCIONS */



void reduce_group_vertices(int g)
{
  int *rtab;
  int g_size;
  Bool changed=False;

  g_size=group_size(g,group, vertex_used);

  if(g_size==0)
    {
      printf("current group contains 0 vertices !!!\n"); 
      return;
    }

  rtab= (int*) malloc(2*g_size*sizeof(int));
  {
    int i,j;
    j=0;
    for(i=0; i<VERTEX_MAX && j<g_size; i++)
      if(vertex_used[i] && group[i]==g)
	{
	  *(rtab+2*j)=i;
	  *(rtab+2*j+1)=j;
	  j++;
	}

    for(i=1; i<g_size; i++)
      for(j=0; j<i && *(rtab+2*i+1)== i; j++)
	if(vector_distance(vertex[*(rtab+2*i)],vertex[*(rtab+2*j)])<=
	   reduction_epsilon)
	  {
	    changed=True;
	    while(*(rtab+2*j+1)!=j) j=*(rtab+2*j+1);
            *(rtab+2*i+1)=j;
	  }
  }


  if(!changed)
    {
      printf("No vertex reductions can be done.\n");
      free(rtab);
      return;
    }



  group_statistics(g);
  backup();

  printf("REDUCING ...\n");

  {
    int i, old_top, e[3], t[4];
    graph_marked_init();
    for(i=0; i<g_size; i++)
      graph_marked[*(rtab+2*i)]= *(rtab+2*(*(rtab+2*i+1)));
     /*  if graph_marked[i]!=-1 thten group[i]== g and i should be replaced */
     /*  by graph_marked[i] */

     /*  REDUCE EDGES */
    old_top=edge_top;
    i=0;
    while(i<edge_top)
      if(graph_marked[edge[i][0]]!= -1||
	 graph_marked[edge[i][1]]!= -1
	 )
	{
	  edgecpy(e, edge[i]);
	  edge_delete(i);
          e[0]=graph_marked[e[0]];
          e[1]=graph_marked[e[1]];
	  edge_sort(e);
	  edgecpy(edge[edge_top],e);
	}
      else i++;
    qsort(edge+edge_top, old_top-edge_top, sizeof(int[3]), edge_compare);

    i=edge_top;
    while(i<old_top)
      {
	if(i==0 || !edge_eq(edge[i-1],edge[i]))
	  {
	    edgecpy(edge[edge_top],edge[i]);
            vertex_used[edge[edge_top][0]]++;
            vertex_used[edge[edge_top][1]]++;
	    edge_top++;
	  }
	i++;
      }


     /*  REDUCE TRIANGLES */
    old_top=triangle_top;
    i=0;
    while(i<triangle_top)
      if(graph_marked[triangle[i][0]]!= -1||
	 graph_marked[triangle[i][1]]!= -1||
	 graph_marked[triangle[i][2]]!= -1
	 )
	{
	  trianglecpy(t, triangle[i]);
	  triangle_delete(i);
          t[0]=graph_marked[t[0]];
          t[1]=graph_marked[t[1]];
          t[2]=graph_marked[t[2]];
	  triangle_sort(t);
	  trianglecpy(triangle[triangle_top],t);
	}
      else i++;
    qsort(triangle+triangle_top, old_top-triangle_top, sizeof(int[4]), 
	  triangle_compare);

    i=triangle_top;
    while(i<old_top)
      {
	if(i==0 || !triangle_eq(triangle[i-1],triangle[i]))
	  {
	    trianglecpy(triangle[triangle_top],triangle[i]);
            vertex_used[triangle[triangle_top][0]]++;
            vertex_used[triangle[triangle_top][1]]++;
            vertex_used[triangle[triangle_top][2]]++;
	    triangle_top++;
	  }
	i++;
      }

     /*  DOKONCZ ... dla triangles */
  }
  free(rtab);

  group_statistics(g);
}





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
	 int group[])
{
  int n_vertices;

  FILE *stream;
  stream=fopen(fname,"w");
  if(stream==NULL) 
  {
    printf("save: can not open '%s'\n", fname);
    return -1; 
  }


   /*  VERTICES */
  n_vertices=vertex_compress(vertex,
			     vertex_used,
			     edge_top, edge,
			     triangle_top, triangle, triangle_normal,
			     group);

  vertex_fprintf(stream, n_vertices, vertex);
  fprintf(stream, "\n");

   /*  EDGES */
  edge_fprintf(stream, edge_top, edge);
  fprintf(stream, "\n");

   /*  TRIANGLES */
  triangle_fprintf(stream, triangle_top, triangle);
  fprintf(stream, "\n");

   /*  TRANSFORMATION */
  fprintf(stream, "%s\n", transformation_label);
  transformation_fprintf(stream, transformation);

   /*  LIGHT */
  fprintf(stream, "%s\n", light_label);
  vector_fprintf(stream, light);

   /*  BACKGROUND */
  fprintf(stream, "%s\n", background_label);
  fprintf(stream, "%d\n", background);

   /*  CURSOR */
  fprintf(stream, "%s\n", cursor_label);
  vector_fprintf(stream, cursor);
  fprintf(stream, "%f\n", cursor_step);

   /*  GROUPS */
  fprintf(stream, "%s\n", group_label);
  group_fprintf(stream, n_vertices, group);

  fclose(stream);
  printf("saved to: '%s'\n", fname);
  return n_vertices;

 


} 
	 
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
	 int group[])
{

  FILE *stream;
  char label[200];
  int n_vertices;

  stream=fopen(fname,"r");
  if(stream==NULL) 
  {
    printf("load: can not open '%s'\n", fname);
    return -1; 
  }


   /*  VERTICES */
  n_vertices=vertex_fscanf(stream, vertex, vertex_used);

   /*  EDGES */
  edge_fscanf(stream, edge_top, edge,  
	      vertex_used);

   /*  TRIANGLES */
  triangle_fscanf(stream, triangle_top, triangle, 
		  triangle_normal, vertex, vertex_used);

   /*  TRANSFORMATION */
  fscanf(stream, "%s", label);
  if(strcmp(label, transformation_label)==0)
    {
      transformation_fscanf(stream, transformation);
    }

   /*  LIGHT */
  fscanf(stream, "%s", label);
  if(strcmp(label, light_label)==0)
    {
      vector_fscanf(stream, light);
    }

   /*  BACKGROUND */
  fscanf(stream, "%s", label);
  if(strcmp(label, background_label)==0)
    {
      fscanf(stream, "%d\n", background);
    }

   /*  CURSOR */
  fscanf(stream, "%s", label);
  if(strcmp(label, cursor_label)==0)
    {
      vector_fscanf(stream, cursor);
      fscanf(stream, "%f\n", cursor_step);
      cursor_set_step(*cursor_step);
    }

   /*  GROUP */
  fscanf(stream, "%s", label);
  if(strcmp(label, group_label)==0)
    {
      group_fscanf(stream, group);
    }
  else
    {
      int i;
      for(i=0;i<n_vertices; i++) group[i]=0;
    }


  fclose(stream);
  printf("loaded from: '%s'\n", fname);
  return n_vertices;
} 
	 


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
	  int group[])
{
  float m_vertex[VERTEX_MAX][3]; 
  int m_vertex_used[VERTEX_MAX];
  int m_edge_top; 
  int m_edge[EDGE_MAX][3];
  int m_triangle_top;
  int m_triangle[TRIANGLE_MAX][4]; 
  float m_triangle_normal[TRIANGLE_MAX][3];
  struct Transformation m_transformation;
  float m_light[3];
  int m_background;
  struct Screen m_screen;
  float m_cursor[3];
  float m_cursor_step;
  int m_current_color;
  int first_free, m_vertices, i;
  int m_group[VERTEX_MAX];

  first_free= vertex_compress(vertex,
			      vertex_used,
			      *edge_top, edge,
			      *triangle_top, triangle, triangle_normal,
			      group);
  m_vertices= load( fname, 
		    m_vertex, m_vertex_used,
		    &m_edge_top, m_edge,      
		    &m_triangle_top, m_triangle, m_triangle_normal,
		    & m_transformation,
		    m_light,
		    &m_background,
		    &m_screen,
		    m_cursor,
		    &m_cursor_step,
		    &m_current_color,
		    m_group);
  if(first_free+m_vertices>VERTEX_MAX) 
    {
      printf("merge: vertex table overflow !!!\n");
      return -1;
    }
  if(*edge_top+m_edge_top>EDGE_MAX) 
    {
      printf("merge: edge table overflow !!!\n");
      return -1;
    }

  if(*triangle_top+m_triangle_top>TRIANGLE_MAX) 
    {
      printf("merge: triangle table overflow !!!\n");
      return -1;
    }

   /*  merge groups */
  {
    int d;
    d=group_max(group, vertex_used)+1;
    for(i=0; i<m_vertices; i++)
      group[first_free+i]=d+m_group[i];
    printf("merged groups start from: %d\n", d);
  }

   /*  merge vertices */
  for(i=0; i<m_vertices; i++)
    {
      vectorcpy(vertex[first_free+i], m_vertex[i]);
      vertex_used[first_free+i]=m_vertex_used[i];
    }


   /*  merge edges */
  for(i=0; i<m_edge_top; i++)
    {
      edge[(*edge_top)+i][0]= m_edge[i][0]+first_free;
      edge[(*edge_top)+i][1]= m_edge[i][1]+first_free;
      edge[(*edge_top)+i][2]= m_edge[i][2];  /*  color */
    }
  *edge_top += m_edge_top;

   /*  merge triangles */
  for(i=0; i<m_triangle_top; i++)
    {
      triangle[(*triangle_top)+i][0]= m_triangle[i][0]+first_free;
      triangle[(*triangle_top)+i][1]= m_triangle[i][1]+first_free;
      triangle[(*triangle_top)+i][2]= m_triangle[i][2]+first_free;
      triangle[(*triangle_top)+i][3]= m_triangle[i][3];
      vectorcpy(triangle_normal[(*triangle_top)+i],m_triangle_normal[i]);
    }
  *triangle_top += m_triangle_top;

  return 0;
}



 /* // PCL raster printing */

void pcl_init(FILE* f)
{
fprintf(f,"%c*t300R",27);  /*  RESOLUTION 300 DPI */
fprintf(f,"%c*b0M",27);  /*  UNENCODED */
fprintf(f,"%c*r-3U",27);  /*  CMY Color */
fprintf(f,"%c*r0A",27);  /*  START RASTER */
}


void pcl_finish(FILE* f)
{
fprintf(f,"%c*rC",27);  /*  END RASTER */
fprintf(f,"\f");  /*  END OF PAGE  */
}





void pcl_line1_bytes(FILE* f,int length, double red, double green, double blue)
{
  int shade_levels=10000;
  int i;
  double max_shade;
  unsigned char mask=128, out_byte=0; 

  max_shade= 
    red*(stereo_left_color[0]+stereo_right_color[0])+
    green*(stereo_left_color[1]+stereo_right_color[1])+
    blue*(stereo_left_color[2]+stereo_right_color[2]);

  for(i=0; i<length;i++)
    {
      if(((double)(labs(random())%shade_levels))/shade_levels<
	 pcl_darkness*
	 (
	  red*pcl_red_line1[i]+
          green*pcl_green_line1[i]+
          blue*pcl_green_line1[i]
	  )/max_shade)
	out_byte|=mask;
      mask/=2;

      if(i<length-1)
	if(
	   ((double)(labs(random())%shade_levels))/shade_levels<
	   pcl_darkness*
	   0.5*(
		red*(pcl_red_line1[i]+pcl_red_line1[i+1])+
		green*(pcl_green_line1[i]+pcl_green_line1[i+1])+
		blue*(pcl_blue_line1[i]+pcl_blue_line1[i+1])
		)/max_shade
	   )
	  out_byte|=mask;
      mask/=2;

      if(mask==0)
	{
	  fprintf(f,"%c", out_byte);
          mask=128;
          out_byte=0;
	}
    }

  if(mask!=128) fprintf(f,"%c", out_byte);  /*  LAST BYTE NON-FULL */

}


void pcl_line12_bytes(FILE* f,int length, double red, double green, double blue)
{
  int shade_levels=10000;
  int i;
  double max_shade;
  unsigned char mask=128, out_byte=0; 

  max_shade= 
    red*(stereo_left_color[0]+stereo_right_color[0])+
    green*(stereo_left_color[1]+stereo_right_color[1])+
    blue*(stereo_left_color[2]+stereo_right_color[2]);

  for(i=0; i<length;i++)
    {
      if(((double)(labs(random())%shade_levels))/shade_levels<
	 pcl_darkness*
	 0.5*(
	      red*(pcl_red_line1[i]+pcl_red_line2[i])+
	      green*(pcl_green_line1[i]+pcl_green_line2[i])+
	      blue*(pcl_green_line1[i]+pcl_green_line2[i])
	      )
	 /max_shade)
	out_byte|=mask;
      mask/=2;

      if(i<length-1)
	if(
	   ((double)(labs(random())%shade_levels))/shade_levels<
	   pcl_darkness*
	   0.25*(
		red*(pcl_red_line1[i]+pcl_red_line1[i+1]+
		     pcl_red_line2[i]+pcl_red_line2[i+1])+
		green*(pcl_green_line1[i]+pcl_green_line1[i+1]+
		       pcl_green_line2[i]+pcl_green_line2[i+1])+
		blue*(pcl_blue_line1[i]+pcl_blue_line1[i+1]+
		      pcl_blue_line2[i]+pcl_blue_line2[i+1])
		)/max_shade
	   )
	  out_byte|=mask;
      mask/=2;

      if(mask==0)
	{
	  fprintf(f,"%c", out_byte);
          mask=128;
          out_byte=0;
	}
      
    }

  if(mask!=128) fprintf(f,"%c", out_byte);  /*  LAST BYTE NON-FULL */

}

void pcl_print_two_lines(FILE *f)
{
  int bytes_in_line;


  bytes_in_line=((screen.width+3)/4);  /*  TWO PCL POINTS PER SCREEN PIXEL */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line1_bytes(f,screen.width, 1,0,0);  /*  RED->CYAN */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line1_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->MAGENTA */

  fprintf(f,"%c*b%dW",27, bytes_in_line);
  pcl_line1_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->YELLOW */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line12_bytes(f,screen.width, 1,0,0);  /*  RED->CYAN */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line12_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->MAGENTA */

  fprintf(f,"%c*b%dW",27, bytes_in_line);
  pcl_line12_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->YELLOW */

}


void pcl_print_raster(FILE *f)
{
  int l;
  float *t;
  glReadBuffer(GL_FRONT);

  l=screen.height-1;
  glReadPixels(0, l, screen.width, 1,
	       GL_RED, GL_FLOAT, pcl_red_line2);
  glReadPixels(0, l, screen.width, 1,
	       GL_GREEN, GL_FLOAT, pcl_green_line2);
  glReadPixels(0, l, screen.width, 1,
	       GL_BLUE, GL_FLOAT, pcl_blue_line2);

  do
    {
      t=pcl_red_line1;
      pcl_red_line1=pcl_red_line2;
      pcl_red_line2=t;

      t=pcl_green_line1;
      pcl_green_line1=pcl_green_line2;
      pcl_green_line2=t;

      t=pcl_blue_line1;
      pcl_blue_line1=pcl_blue_line2;
      pcl_blue_line2=t;


      l--;
      glReadPixels(0,l, screen.width, 1,
		   GL_RED, GL_FLOAT, pcl_red_line2);
      glReadPixels(0,l, screen.width, 1,
		   GL_GREEN, GL_FLOAT, pcl_green_line2);
      glReadPixels(0,l, screen.width, 1,
		   GL_BLUE, GL_FLOAT, pcl_blue_line2);
      pcl_print_two_lines(f);
    }
  while(l>0);

  glReadBuffer(GL_BACK);

}



void pcl_print()
{
  FILE *f;

  if(!stereo_mode)
    {
      printf("PCL printing only in stereo mode.\n");
      return;
    }

  if(screen.width<4 || screen.height<4)
    {
      printf("The window is too small to be printed\n");
      return;
    }

  f=fopen(pcl_file_name,"w");
  if(f==NULL)
    {
      printf("Can not open file: %s\n",pcl_file_name);
      return;
    }

  pcl_red_line1= (float*) malloc(screen.width*sizeof(float)); 
  pcl_green_line1= (float*) malloc(screen.width*sizeof(float)); 
  pcl_blue_line1= (float*) malloc(screen.width*sizeof(float)); 

  pcl_red_line2= (float*) malloc(screen.width*sizeof(float)); 
  pcl_green_line2= (float*) malloc(screen.width*sizeof(float)); 
  pcl_blue_line2= (float*) malloc(screen.width*sizeof(float)); 

  if(pcl_red_line1==NULL ||
     pcl_green_line1==NULL||
     pcl_blue_line1==NULL||
     pcl_red_line2==NULL ||
     pcl_green_line2==NULL||
     pcl_blue_line2==NULL)
    {
      printf("Out of memory\n");
      fclose(f);
      free(pcl_red_line1);
      pcl_red_line1=NULL;
      free(pcl_green_line1);
      pcl_green_line1=NULL;
      free(pcl_blue_line1);
      pcl_blue_line1=NULL;
      free(pcl_red_line2);
      pcl_red_line2=NULL;
      free(pcl_green_line2);
      pcl_green_line2=NULL;
      free(pcl_blue_line2);
      pcl_blue_line2=NULL;
      return;
    }

  pcl_init(f);

   /*  EXPORT RASTER ... */
  pcl_print_raster(f);


  pcl_finish(f);


  fclose(f);
  free(pcl_red_line1);
  pcl_red_line1=NULL;
  free(pcl_green_line1);
  pcl_green_line1=NULL;
  free(pcl_blue_line1);
  pcl_blue_line1=NULL;
  free(pcl_red_line2);
  pcl_red_line2=NULL;
  free(pcl_green_line2);
  pcl_green_line2=NULL;
  free(pcl_blue_line2);
  pcl_blue_line2=NULL;
  printf("Printed to PCL file: %s\n", pcl_file_name);

}



 /* //// POVRAY */

int povray_export()
{
  FILE *povf, *inif;
  char pov_name[MAX_FILE_NAME+5], ini_name[MAX_FILE_NAME+5];

  if(two_lights)
    {
      printf("switching to one light mode:\n");
      switch_second_light();
      redraw();
      printf("(press <Shift>+<L> to switch back to two lights)\n");

    }

  sprintf(ini_name, "%s%d.ini", povray_name, povray_counter);


  sprintf(pov_name, "%s%d.pov", povray_name, povray_counter);
 
   /* // prepare *.ini */


  inif=fopen(ini_name,"w");
  if(inif==NULL) 
  {
    printf("save: can not open '%s'\n", ini_name);
    return -1; 
  }

   /* / prepare ini file */
  fprintf(inif, "Input_File_Name=%s\n", pov_name);
  fprintf(inif, "+W%d +H%d\n", screen.width, screen.height);
  fprintf(inif, "+A\n");

  fclose(inif);
  printf("Povray options exported to: %s\n", ini_name);



   /* / prepare *.pov */

  povf=fopen(pov_name,"w");
  if(povf==NULL) 
  {
    printf("save: can not open '%s'\n", pov_name);
    return -1; 
  }

  {
     /* / export pov file */
    float matrix[16];
     /*  matrix[0..3] - observer */
     /*  matrix[4..7] - look at */
     /*  matrix[8..11] -sky */

    float_zeroes(16, matrix);
    vectorcpy(matrix, transformation.mov);
    vector_scale(-1, matrix);
    vectorcpy(matrix+4, matrix);
    matrix[6]-=screen.distance;
    matrix[9]=1;  /*  sky = <0,1,0> */

     /*  multpily by reverse of transformation.M */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixd(transformation.R);
    glMultMatrixf(matrix);
    glGetFloatv( GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();


     /*  COMMENTS */
    {
      float vmin[3], vmax[3];
      fprintf(povf, "// Exported from E.T. EDITOR.\n");
      vertex_get_range(vertex, vertex_used, vmin, vmax);
      fprintf(povf, "//\n");
      fprintf(povf, "// All edge and triangle vertices are in the range\n");
      fprintf(povf, "// [(Xmin,Ymin,Zmin), (Xmax,Ymax,Zmax)] =\n");
      fprintf(povf, "// [(%f,%f,%f),(%f,%f,%f)].\n",
	      vmin[0],vmin[1],vmin[2],
              vmax[0],vmax[1],vmax[2]);
      fprintf(povf, "//\n");
      fprintf(povf, "// To render the scene call:\n");
      fprintf(povf, "//\n");
      fprintf(povf, "//   povray %s\n", ini_name);
      fprintf(povf, "//\n\n");
    }

     /*  CAMERA */

    fprintf(povf, " camera {\n");
    fprintf(povf, "    up <0,%f,0>\n", screen.height*pixel_size);
    fprintf(povf, "    right <%f,0,0>\n", screen.width*pixel_size);
    fprintf(povf, "    direction<0,0,%f>\n", -screen.distance);
    fprintf(povf, "    location <%f,%f,%f>\n",
	    matrix[0],matrix[1],matrix[2]);
    fprintf(povf, "    sky  <%f,%f,%f>\n",
	    matrix[8],matrix[9],matrix[10]);
    fprintf(povf, "    look_at  <%f,%f,%f>\n",
	    matrix[4],matrix[5],matrix[6]);
    fprintf(povf, "  }\n\n");


     /*  LIGHT */
    fprintf(povf, "light_source {\n  <%f,%f,%f>\n  color rgb<%f,%f,%f>\n  }\n\n",
	    povray_light_distance*light[0],
	    povray_light_distance*light[1],
	    povray_light_distance*light[2],
            povray_light_red,
            povray_light_green,
            povray_light_blue);

     /*  BACKGROUND */
    fprintf(povf,"background { color rgb <%f,%f,%f>}\n\n",
	    color[background][0],
	    color[background][1],
	    color[background][2]
	    );

     /*  EDGES */
    if(edge_top>0 && povray_edge_radius>0)
      {
	int i;
	/* // EDGE COLORS */
	fprintf(povf, "\n// Colors of the edges\n");
	for(i=0; i<COLOR_MAX; i++)
	  {
	    fprintf(povf,"#declare E%d = texture {\n",i);
	    fprintf(povf,"    pigment { color rgb<%f,%f,%f>}\n",
		    1*color[i][0]+0, 
		    1*color[i][1]+0, 
		    1*color[i][2]+0); 
	    
	    fprintf(povf,"    finish { ambient 1 }\n");
	      
	    
	    fprintf(povf,"  }\n");
	  }

	fprintf(povf, "\n// Edges exported as the union of cylinders\n");
	fprintf(povf,"\nunion {\n");
	for(i=0; i<edge_top; i++)
	  if(!group_restricted || edge_inside_group(edge[i],group_current))
	    {
	      fprintf(povf," cylinder{<%f,%f,%f>, <%f,%f,%f>, %f texture{E%d}}\n",
		      vertex[edge[i][0]][0], 
		      vertex[edge[i][0]][1], 
		      vertex[edge[i][0]][2], 
		      vertex[edge[i][1]][0], 
		      vertex[edge[i][1]][1], 
		      vertex[edge[i][1]][2],
		      povray_edge_radius,
		      edge[i][2]); 
	    }
	povray_clipping(povf);
	fprintf(povf,"}\n");  /*  end of union */
      }


     /*  COLORS */
    {
      int i;
      fprintf(povf, "\n// Colors of the triangles\n");
      for(i=0; i<COLOR_MAX; i++){
	fprintf(povf,"#declare C%d = texture {\n",i);
	fprintf(povf,"    pigment { color rgb<%f,%f,%f>}\n",
		1*color[i][0]+0, 
		1*color[i][1]+0, 
		1*color[i][2]+0); 
	fprintf(povf,"    finish { ambient %f diffuse %f }\n",
		povray_ambient, povray_diffuse);
	fprintf(povf,"  }\n");
      }

    }


     /*  TRIANGLES */
    if(triangle_top>0)
    {
      int i;
      fprintf(povf, "\n// Triangles exported as the mesh of triangles\n\n");
      fprintf(povf,"mesh {\n");
      for(i=0; i<triangle_top; i++)
	if(!group_restricted || triangle_inside_group(triangle[i], group_current))
	  {
	    fprintf(povf,"triangle{<%f,%f,%f>,<%f,%f,%f>,<%f,%f,%f> texture{C%d}}\n",
		    vertex[triangle[i][0]][0],
		    vertex[triangle[i][0]][1],
		    vertex[triangle[i][0]][2],
		    vertex[triangle[i][1]][0],
		    vertex[triangle[i][1]][1],
		    vertex[triangle[i][1]][2],
		    vertex[triangle[i][2]][0],
		    vertex[triangle[i][2]][1],
		    vertex[triangle[i][2]][2],
		    triangle[i][3]);
	  }
      povray_clipping(povf);
      fprintf(povf," }\n");  /*  end of mesh */
    }
 
  }



  fclose(povf);
  printf("Povray data exported to: %s\n", pov_name);
  povray_counter++;
  return 0;
}


void povray_clipping(FILE* povf)
{
   /*  CLIPPING */

  if(clipping_xyz.xmin_flag)
    {
      fprintf(povf,"clipped_by { plane{ <-1,0,0>, %f }}\n", 
	      -1*clipping_xyz.xmin);
    }
  if(clipping_xyz.xmax_flag)
    {
      fprintf(povf,"clipped_by { plane{ <1,0,0>, %f }}\n", 
	      1*clipping_xyz.xmax);
    }

  if(clipping_xyz.ymin_flag)
    {
      fprintf(povf,"clipped_by { plane{ <0,-1,0>, %f }}\n", 
	      -1*clipping_xyz.ymin);
    }
  if(clipping_xyz.ymax_flag)
    {
      fprintf(povf,"clipped_by { plane{ <0,1,0>, %f }}\n", 
	      1*clipping_xyz.ymax);
    }

  if(clipping_xyz.zmin_flag)
    {
      fprintf(povf,"clipped_by { plane{ <0,0,-1>, %f }}\n", 
	      -1*clipping_xyz.zmin);
    }
  if(clipping_xyz.zmax_flag)
    {
      fprintf(povf,"clipped_by { plane{ <0,0,1>, %f }}\n", 
	      1*clipping_xyz.zmax);
    }


}





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


 /* //////////// KEY COMMANDS //////////// */



void callbackKeyPress( XKeyEvent* evptr)
{
  switch(keymode)
    {
    case key_default:
      callback_key_default(evptr);
      break;
    case key_F1:
      callback_key_F1(evptr);
      break;
    case key_F2:
      callback_key_F2(evptr);
      break;
    case key_F3:
      callback_key_F3(evptr);
      break;
    case key_F4:
      callback_key_F4(evptr);
      break;
    case key_F5:
      callback_key_F5(evptr);
      break;
    case key_F6:
       /*  callback_key_F6(evptr); */
      break;
    case key_F7:
      callback_key_F7(evptr);
      break;
    }
}

void callback_key_F7(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F7;
      break;
    case XK_F7:
      stereo_mode= !stereo_mode;
      if(stereo_mode)
	{
	  printf("STEREO MODE\n");
	  clip_cube(&clipping_xyz);
	}
      else printf("COLOR MODE\n");
      redraw();
      break;
    case XK_1:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(-0.1,0,0, stereo_left_color);
	  break;
	case ShiftMask:
	  color_adjust(0.1,0,0, stereo_left_color);
	  break;
	}
      redraw();
      printf("LEFT COLOR (%f, %f, %f)\n", 
	     stereo_left_color[0],
	     stereo_left_color[1],
	     stereo_left_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_2:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,-0.1,0, stereo_left_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0.1,0, stereo_left_color);
	  break;
	}
      redraw();
      printf("LEFT COLOR (%f, %f, %f)\n", 
	     stereo_left_color[0],
	     stereo_left_color[1],
	     stereo_left_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_3:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,0,-0.1, stereo_left_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0,0.1, stereo_left_color);
	  break;
	}
      redraw();
      printf("LEFT COLOR (%f, %f, %f)\n", 
	     stereo_left_color[0],
	     stereo_left_color[1],
	     stereo_left_color[2]
	     );
      next_mode=key_F7;
      break;

    case XK_7:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(-0.1,0,0, stereo_right_color);
	  break;
	case ShiftMask:
	  color_adjust(0.1,0,0, stereo_right_color);
	  break;
	}
      redraw();
      printf("RIGHT COLOR (%f, %f, %f)\n", 
	     stereo_right_color[0],
	     stereo_right_color[1],
	     stereo_right_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_8:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,-0.1,0, stereo_right_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0.1,0, stereo_right_color);
	  break;
	}
      redraw();
      printf("RIGHT COLOR (%f, %f, %f)\n", 
	     stereo_right_color[0],
	     stereo_right_color[1],
	     stereo_right_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_9:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,0,-0.1, stereo_right_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0,0.1, stereo_right_color);
	  break;
	}
      redraw();
      printf("RIGHT COLOR (%f, %f, %f)\n", 
	     stereo_right_color[0],
	     stereo_right_color[1],
	     stereo_right_color[2]
	     );
      next_mode=key_F7;
      break;

    case XK_c:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  clipping_cube_size-=0.5;
	  if(clipping_cube_size<1) clipping_cube_size=1;
	  break;
	case ShiftMask:
	  clipping_cube_size+=0.5;
	  break;
	case ControlMask:
	  clipping_cube_size/=2;
	  if(clipping_cube_size<1) clipping_cube_size=1;
	  break;
	case ControlMask|ShiftMask:
	  clipping_cube_size*=2;
	  break;
	}
      clip_cube(&clipping_xyz);
      redraw();
      next_mode=key_F7;
      break;



    case XK_x:
      {
	int i,t;
	for(i=0; i<3; i++)
	  {
	    t=stereo_left_mask[i];
	    stereo_left_mask[i]=stereo_right_mask[i];
	    stereo_right_mask[i]=t;
	  }
      }
      vector_swap(stereo_left_color, stereo_right_color);
      printf("left and right color swaped\n");
      redraw();
      break;

    case XK_e:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  stereo_eye_distance-=0.1;
	  break;
	case ShiftMask:
	  stereo_eye_distance+=0.1;	  
	  break;
	}
      redraw();
      printf("EYE DISTANCE: %f\n", 
	     stereo_eye_distance
	     );
      next_mode=key_F7;
      break;

    case XK_p:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case ControlMask:
	  printf("Printing PCL to file: %s\n", pcl_file_name);
	  pcl_print();
	  break;
	}
      break;
    }


  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F7_help()
{
  printf("F7 stereo mode control:\n");
  printf("\n");
  printf("<F7> switch stereo mode ON/OFF\n");
  printf("[<Shift>+] <1>/<2>/<3> Modify R/G/B values of the left color\n");
  printf("[<Shift>+] <7>/<8>/<9> Modify R/G/B values of the right color\n");
/*   printf("[<Shift>+] <4>/<5>/<6> Modify R/G/B values of the background\n"); */
  printf("<Shift>+<E>/<E> Modify distance between eyes\n");
  printf("<Shift>+<C>/<C> Modify clipping cube size\n");
  printf("<Ctrl>+(<Shift>+<C>/<C>) double/halve clipping cube size\n");
  printf("<X> exchange left and right colors\n");
}



void callback_key_F5(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F5;
      break;

    case XK_1:
      printf("Inputnew reduction epsilon (>= 0):");
      reduction_epsilon=get_float();
      if(reduction_epsilon<0) reduction_epsilon=0.0;
      printf("\n new value of reduction epsilon is: %e\n", reduction_epsilon);
      redraw();
      break;

    case XK_v:
      reduce_group_vertices(group_current);
      redraw();
      break;

/*     case XK_s: */
/*       qsort(edge,edge_top, sizeof(int[3]), edge_compare); */
/*       qsort(triangle,triangle_top, sizeof(int[4]), triangle_compare); */
/*       redraw(); */
/*       break; */

    }


  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F5_help()
{
  printf("F5 mode (REDUCTIONS):\n");
  printf("\n");
  printf("<1> input new reduction epsilon [%e]\n", reduction_epsilon);
  printf("<V> reduce vertices in current group\n");
}


void callback_key_F4(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F4;
      break;

    case XK_i:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  {
	    float X[3];
	    vectorcpy(X, cursor);
	    line_triangle_intersection(point[10], point[11],
				       point[12], point[13], point[14],
				       X);
	    cursor_set(X[0], X[1], X[2]);
	    
	  }
	  redraw();
	  break;
	case ShiftMask:
	  group_tt_intersection(group_current, group_marker);
	  redraw();
	  break;
	}
      break;
    case XK_c:
      {
	float X[3];
	vectorcpy(X, cursor);
	find_group_center(group_current, X);
	cursor_set(X[0], X[1], X[2]);
      }
      redraw();
      break;

    case XK_n:
      {
	float X[3];
	triangle_normal_vector(point[10], point[11],point[12],X);
        vector_scale(cursor_step,X);
        vectorcpy(et0_marker, cursor);
        vector_add(cursor, X, X);
        context_switch(context_et0);
	cursor_set(X[0], X[1], X[2]);

      }
      redraw();
      break;

    case XK_s:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  constr_scale(point[10], point[11],
		       point[12], point[13], cursor,
		       group_current);
	  break;
	case ControlMask:
	  constr_scale_in_direction(point[10], point[11],
				    point[12], point[13], 
				    point[14], point[15], 
				    cursor,
				    group_current);
	  break;
	}
      redraw();
      break;

    case XK_t:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  three_point_transform(point[10], point[11], point[12], 
				point[13], point[14], point[15],
				group_current);
	  break;
	}
      redraw();
      break;

    case XK_f:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  {
	    float V[3];
	    if(find_folding(point[10], 
			    point[11], point[12], 
			    point[13], point[14], 
			    point[15],
			    V)
	       )
	      {
		cursor_set(V[0], V[1], V[2]);
	      }
	  }
	  break;
	case ControlMask:
	  {
	    float V[3], mv1[3], mv2[3], R1[3][3], R2[3][3];
	    if(find_folding(point[10], 
			    point[11], point[12], 
			    point[13], point[14], 
			    point[15],
			    V)
	       &&
	       find_three_point_transform(point[10], point[11], point[13],
					 point[10], point[11], V,
					 mv1,R1)
	       &&
	       find_three_point_transform(point[10], point[12], point[14],
					 point[10], point[12], V,
					 mv2,R2)
	       )
	      {

		backup();
		group_transform(group_current,mv2,R2);
		if(group_marker!=group_current)
		  group_transform(group_marker,mv1,R1);
 
	         /*  DOKONCZ ... */
	      }
	  }
	  break;
	}
      redraw();
      break;
      
      
    }


  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F4_help()
{
  printf("F4 mode (CONSTRUCTIVE METHODS):\n");
  printf("\n");
  printf("<C> move cursor to the current group center\n");
  printf("<I> move cursor to intersection of line AB and plane CDE\n");
  printf("<Shift>+<I> intersection of triangles of marked and curent groups\n");
  printf("<N> find cursor_step*(normal to ABC)\n");
  printf("<S> scale current group by  |AB|/|CD| (where fixed_point=cursor)\n");
  printf("<Ctrl>+<S> scale current group by  |AB|/|CD| in direction EF\n");
  printf("<T> three point transformation ABC to DEF\n");
  printf("<F> find folding point: axes AB, AC, rotated lines AD, AE, direction F\n");
  printf("<Ctrl>+<F> folding marked and current group around AB and AC in direction F so that AD meets AE\n");


}




void callback_key_F3(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  char buffer[10];
  KeySym keysym;
  XKeyEvent ev;


  ev.state=evptr->state;
  evptr->state=0;
  XLookupString(evptr, buffer, 9, &keysym, NULL);
  evptr->state=ev.state;


  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F3;
      break;

    default:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  if('0'<= buffer[0] && buffer[0]<= '9')
	    point_store(buffer[0]-'0', cursor);
	  else
	  if('a'<= buffer[0] && buffer[0]<= 'a'+POINT_MAX-10)
	    point_store(buffer[0]-'a'+10, cursor);

	  break;
	case ControlMask:
	  if('0'<= buffer[0] && buffer[0]<= '9')
	    cursor_set(point[buffer[0]-'0'][0],
		       point[buffer[0]-'0'][1], 
		       point[buffer[0]-'0'][2]
		       );
	  else
	  if('a'<= buffer[0] && buffer[0]<= 'a'+POINT_MAX-10)
	    cursor_set(point[buffer[0]-'a'+10][0],
		       point[buffer[0]-'a'+10][1], 
		       point[buffer[0]-'a'+10][2]
		       );
	  break;
	  
	}
      redraw();
      break;
    }



  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F3_help()
{
  printf("F3 mode (STORED POINTS):\n");
  point_print();
  printf("\n");
  printf("<*> stroes cursor position as the point '*'\n");
  printf("<Ctrl>+<*> moves the cursor to the point '*'\n");
}




void callback_key_F2(XKeyEvent* evptr)
{
  enum Keymode next_mode;

  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F2;
      break;

    case XK_a:
      {
	float new_clip;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    new_clip=screen.clip_min/2;
	    break;
	  case ShiftMask:
	    new_clip=screen.clip_min*2;
	    break;
	  case ControlMask:
	    new_clip=screen.clip_min-1;
	    break;
	  case ControlMask|ShiftMask:
	    new_clip=screen.clip_min+1;
	    break;

	  }
	if( new_clip<screen.clip_max/3000 || new_clip>screen.clip_max)
	  {
	  new_clip=screen.clip_min;
	  printf("RESTRICTION: clip_max/3000<clip_min<clip_max\n");
	  }
	screen_set_clipping(&screen, new_clip, screen.clip_max);
	redraw();
	next_mode=key_F2;
      }
      break;

    case XK_b:
      {
	float new_clip;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    new_clip=screen.clip_max/2;
	    break;
	  case ShiftMask:
	    new_clip=screen.clip_max*2;
	    break;
	  case ControlMask:
	    new_clip=screen.clip_max-1;
	    break;
	  case ControlMask|ShiftMask:
	    new_clip=screen.clip_max+1;
	    break;

	  }
	if( screen.clip_min<new_clip/3000 || screen.clip_min>new_clip)
	  {
	  new_clip=screen.clip_max;
	  printf("RESTRICTION: clip_max/3000<clip_min<clip_max\n");
	  }
	screen_set_clipping(&screen, screen.clip_min, new_clip);
	redraw();
	next_mode=key_F2;
      }
      break;



    case XK_z:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  screen.distance=floorf(0.8*screen.distance);
	  if(screen.distance<=0) screen.distance=1;
	  break;
	case ShiftMask:
	  screen.distance=floorf(1.25*screen.distance+1);
	  break;
	case ControlMask:
	  screen.distance-=1;
	  if(screen.distance<1) screen.distance=1;
	  break;
	case ControlMask|ShiftMask:
	  screen.distance+=1;
	  break;
	}
      screen_set_distance(&screen, screen.distance);
      redraw();
      next_mode=key_F2;
      break;

    case XK_m:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
          move_step/=2;
          if(move_step < 0.0001) move_step*=2; 
	  break;
	case ShiftMask:
          move_step*=2;
	  break;
	}
      printf("move step is: %f\n", move_step); 
      next_mode=key_F2;
      break;

    case XK_s:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
           /*  scale/=2; */
	  transformation_scale(0.5, &transformation, cursor);
          if(scale < 1/512) scale=1/512; 
	  break;
	case ShiftMask:
	   /*           scale*=2; */
	  transformation_scale(2, &transformation, cursor);
	  if(scale>512) scale=512;
	  break;
	}
      printf("view scale is: %e\n", scale); 
      redraw();
      next_mode=key_F2;
      break;


    case XK_c:
      {
	float c=cursor_step;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    c/=2;
	    break;
	  case ShiftMask:
	    c*=2;
	    break;
	  }
      cursor_set_step(c); 
      }
      redraw();
      next_mode=key_F2;
      break;
    }



  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F2_help()
{
  printf("F2 mode:\n");
  printf(" <Shift>+<Z>/<Z>  increase/decrease screen distance (zoom) [%f]\n",
	 screen.distance);
  printf(" <Ctrl>+(<Shift>+<Z>/<Z>)  precise zoom  (screen distance) [%f]\n",
	 screen.distance);

  printf(" [<Ctrl>]+(<Shift>+<A>/<A>) [precise] clip_min [%f]\n",
	 screen.clip_min);
  printf(" [<Ctrl>]+(<Shift>+<B>/<B>) [precise] clip_max [%f]\n",
	 screen.clip_max);

  printf(" <Shift>+<M>/<M>  double/halve move step [%f]\n",
	 move_step);
  printf(" <Shift>+<C>/<C>  double/halve cursor step [%f]\n",
	 cursor_step);
  printf(" <Shift>+<S>/<S>  scale up/down the view [%f]\n",
	   scale);
}


void callback_key_F1(XKeyEvent* evptr)
{
  enum Keymode next_mode;

  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F1;
      break;

    case XK_Up:
      backup();
      group_rot90(group_current, 0, 1, 0);
      redraw();
      break;
    case XK_Right:
      backup();
      group_rot90(group_current, 1, 0, 0);
      redraw();
      break;
    case XK_Home:
      backup();
      group_rot90(group_current, 0, 0, 1);
      redraw();
      break;

    case  XK_equal:
      group_statistics(group_current); 
      break;


    case XK_space :
      group_menu();
      XRaiseWindow(display, window);
      XSetInputFocus(display, window, RevertToParent, CurrentTime);
      break;

    case XK_e : 
      backup();
      group_extract_clipped();
      redraw();
      break;

    case XK_m :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  group_mark(group_current);
	  redraw();
	  break;
	case Mod1Mask:
	  group_mark(-1);
	  redraw();
	  break;
	case ControlMask:
	  backup();
          group_regroup(group_marker, group_current);
	  redraw();
	  break;
	}
      break;

    case XK_z :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
          group_remove_empty();
	  redraw();
	  break;
	}
      break;

    case XK_f :
      {
	int found;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    found=group_find(cursor);
	    if(found!=-1)
	      {
		group_change_current(found, &group_current, group, vertex_used);
		redraw();
	      }
	    else
	      printf("CURSOR IS NOT INCIDENT TO ANY GROUP\n");
	    break;
	  }
      }
      break;

    case XK_v :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  if(context==context_et0)
	    {
	      float mv[3];
	      vector_sub(cursor, et0_marker, mv);
	      backup();
	      group_move(group_current, mv, 
			 group, vertex, vertex_used);
	      
	    }
	  redraw();
	  break;
	}
      break;

    case XK_x :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask:
	  backup();
          group_delete_edges(group_current);
	  redraw();
	  break;
	case ShiftMask:
	  backup();
          group_delete_triangles(group_current);
	  redraw();
	  break;
	case Mod1Mask|ShiftMask:
	  backup();
          group_delete_triangles(group_current);
          group_delete_edges(group_current);
	  redraw();
	}
      break;

    case XK_p :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask:
	  backup();
          group_paint_edges(group_current, current_color);
	  redraw();
	  break;
	case ShiftMask:
	  backup();
          group_paint_triangles(group_current, current_color);
	  redraw();
	  break;
	}
      break;

    case XK_c :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  backup();
          group_copy(False, False);
	  redraw();
	  break;
	case Mod1Mask:
	  backup();
          group_copy(True, False);
	  redraw();
	  break;
	case ShiftMask:
	  backup();
          group_copy(False, True);
	  redraw();
	  break;
	case Mod1Mask|ShiftMask:
	  backup();
          group_copy(True, True);
	  redraw();
	  break;
	}
      break;
    }
  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F1_help()
{
  printf("F1 mode:\n");
  printf("  <M> Mark current group [now marked: %d]\n", group_marker);
  printf("  <Ctrl>+<M> Move vertices from marked group (%d) to current group (%d)\n", 
	 group_marker, group_current
	 );
  printf("  <Alt>+<M> Set group marker to -1 (no group)\n");
  printf("  <E> extract visible vertices to a new group\n");
  printf("  <F> change to a group incident to the cursor\n");
  printf("  <C> copy to new group\n");  
  printf("  <Alt>+<C> copy and glue with edges\n");  
  printf("  <Shift>+<C> copy and glue with triangles\n");  
  printf("  <Alt>+<Shift>+<C> copy and glue with edges and triangles\n");  
  printf("  <Z> remove empty group numbers\n");
  printf("  <Alt>+<X> delete edges incident to current group\n");
  printf("  <Shift>+<X> delete triangles incident to current group\n");
  printf("  <Shift>+<Alt>+<X> delete edges and triangles incident to current group\n");
  printf("  <Alt>+<P> paint edges of current group with current color\n");
  printf("  <Shift>+<P> paint triangles of current group with current color\n");
  printf(" <Up>/<Right>/<Home> rotate current group 90 degrees around the axis\n");
  if(context==context_et0) 
    printf("  <V> move current group by vector (et0_marker, cursor)\n");
  printf(" <Space> group menu in the text terminal\n");  
  printf(" <=> current group statistics\n");  
}




void callback_key_default(XKeyEvent* evptr)
{
  long event_mask=KeyPressMask;
  while(XCheckWindowEvent(display,window,event_mask , (XEvent*) evptr));


  switch(XLookupKeysym(evptr,0))
    {
    case XK_F1:
      keymode=key_F1;
      key_F1_help();
      break;

    case XK_F2:
      keymode=key_F2;
      key_F2_help();
      break;

    case XK_F3:
      keymode=key_F3;
      key_F3_help();
      break;

    case XK_F4:
      keymode=key_F4;
      key_F4_help();
      break;

    case XK_F5:
      keymode=key_F5;
      key_F5_help();
      break;

    case XK_F7:
      keymode=key_F7;
      key_F7_help();
      break;

    case XK_Up : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(-rotate_step, 1,0,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,1,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,1,0); redraw(); break;
	case ShiftMask:
          transformation_move(0,move_step,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,1,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Down : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(rotate_step, 1,0,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,-1,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,-1,0); redraw(); break;
	case ShiftMask:
          transformation_move(0,-move_step,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,-1,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Left :  /*  printf("XK_Left  evptr->state=%d\n",evptr->state); */
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(-rotate_step, 0,1,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(-1,0,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(-1,0,0); redraw(); break;
	case ShiftMask:
          transformation_move(-move_step,0,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(-1,0,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Right : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(rotate_step, 0,1,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(1,0,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(1,0,0); redraw(); break;
	case ShiftMask:
          transformation_move(move_step,0,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(1,0,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;


    case XK_Home : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(-rotate_step, 0,0,1, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,0,-1); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,0,-1); redraw(); break;
	case ShiftMask:
          transformation_move(0,0,-move_step, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,0,-1, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_End : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(rotate_step, 0,0,1, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,0,1); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,0,1); redraw(); break;
	case ShiftMask:
          transformation_move(0,0,move_step, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,0,1, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Delete:
      break;

    case XK_BackSpace:
      switch(context)
	{
	case context_et0:
	  backup();
	  edge_vector_delete(et0_marker, cursor);
	  context_switch(context_default); redraw(); break;
	case context_t1:
	  backup();
	  triangle_vector_delete(et0_marker, t1_marker, cursor);
	  context_switch(context_default); redraw(); break;
	case context_default:
	  break;
	}
      break;

    case XK_space:
      break; 

    case XK_x: 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask|ControlMask:
	  backup();
	  edge_delete_incident(cursor);
	  redraw();
	  break;
	case ShiftMask|Mod1Mask|ControlMask:
	  backup();
	  triangle_delete_incident(cursor);
	  redraw();
	  break;
	}
      break;

    case XK_p: 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask|ControlMask:
	  backup();
	  edge_paint_incident(cursor, current_color);
	  redraw();
	  break;
	case ShiftMask|Mod1Mask|ControlMask:
	  backup();
	  triangle_paint_incident(cursor, current_color);
	  redraw();
	  break;
	}
      break;

    case XK_Tab:
      break; 

    case XK_Escape : 
      context_switch(context_default); 
      redraw();
      break;

    case XK_e:
      switch(context)
	{
	case context_default:
          vectorcpy(et0_marker, cursor); context_switch(context_et0);  break;
	case context_et0:
	  backup();
	  edge_set_color( edge_vector_insert(et0_marker, cursor), current_color);
          context_switch(context_default);  
	  break;
	case context_t1:
	  break;
	}
      redraw(); break;

    case XK_t:
      switch(context)
	{
	case context_default:
          vectorcpy(et0_marker, cursor); 
	  context_switch(context_et0);  
	  break;
	case context_et0:
	  if(!vector_eq(et0_marker, cursor)) 
	    {
	      vectorcpy(t1_marker, cursor); 
	      context_switch(context_t1);
	    }  
          break;
	case context_t1:
	  backup();
	  triangle_set_color(triangle_vector_insert(et0_marker,t1_marker,cursor),current_color);
          context_switch(context_default);  break;
	  
	}
      redraw(); 
      break;

    case XK_Return:
      switch(context)
	{
	case context_default:
          vectorcpy(et0_marker, cursor); context_switch(context_et0);  break;
	case context_et0:
	  backup();
	  edge_set_color( edge_vector_insert(et0_marker, cursor), current_color);
	  vectorcpy(et0_marker, cursor); 
          context_switch(context_et0);  
	  break;
	case context_t1:
	  backup();
	  triangle_set_color(triangle_vector_insert(et0_marker,t1_marker,cursor),current_color);
	  vectorcpy(et0_marker, cursor);
          context_switch(context_et0);  
	  break;
	  
	}
      redraw(); break;
     
    case XK_r:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_init_orientation(&transformation);
          vectorcpy(transformation.mov,cursor);
          vector_scale(-1*scale,transformation.mov);
          transformation.mov[2]-=screen.distance+1;
          redraw(); break;
	case ControlMask:
          cursor_set(0,0,0); redraw(); break;
	case ControlMask|ShiftMask:
	  clip_reset(&clipping_xyz); redraw(); break;
	}
      break;

    case XK_f:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  cursor_find_vertex(); redraw(); break;
	case ControlMask:
	  cursor_to_screen(); redraw(); break;
	}
      break;

    case XK_b:
      background= (background+1)% COLOR_MAX; redraw(); break;
    case XK_c:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  set_current_color( (current_color+1)%COLOR_MAX );
	  printf("CURRENT COLOR: %d\n", current_color); 
	  redraw(); 
	  break;
	case ShiftMask|ControlMask:
	  clip_cube(&clipping_xyz);
	  redraw(); 
	  break;
	}
      break;
    case XK_n:
      {
	int max;
	max=group_max(group, vertex_used);
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    group_change_current((group_current+1)%(max+2),
				 &group_current, group, vertex_used);
	    redraw();
	    break;
	  case ShiftMask:
	    group_change_current((group_current+max+1)%(max+2),
				 &group_current, group, vertex_used);
	    redraw(); 
	    break;
	  }
      }
      break;

    case XK_1:
      group_restricted_switch();
      redraw(); 
      break;


    case XK_Page_Up:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  polygon_n_increase(1);
	  break;
	case ShiftMask:
	  polygon_n_increase(10);
	  break;
	}
      break; 

    case XK_Page_Down:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  polygon_n_increase(-1);
	  break;
	case ShiftMask:
	  polygon_n_increase(-10);
	  break;
	}
      break; 


    case XK_o:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask:
	  backup();
	  polygon_add(True, False);
	  break;
	case ShiftMask:
	  backup();
	  polygon_add(False, True);
	  break;
	case Mod1Mask|ShiftMask:
	  backup();
	  polygon_add(True, True);
	  break;


	}
      redraw(); 
      break;




    case XK_l:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  set_light(light); redraw(); break;
	case ShiftMask:
	  switch_second_light(); redraw(); break;
	case ControlMask:
	  if(!was_modified || are_you_sure())
	    {	    
	      printf("LOADING:\n");
	      get_file_name(file_name);
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
	      backup();
	      was_modified=False;
	    }
	  redraw(); break;
	case ControlMask|ShiftMask:
	  backup();
	  printf("MERGING:\n");
	  get_file_name(merge_file_name);
	  merge( merge_file_name, 
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
	  redraw(); break;
	};
      break;
          

    case XK_s:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case ControlMask:
	  printf("SAVING:\n");
          get_file_name(file_name);
	  save( file_name, 
	        vertex, vertex_used,
	        edge_top, edge, 
	        triangle_top, triangle, triangle_normal,
	        & transformation,
	        light,
		background,
	        &screen,
	        cursor, 
		cursor_step,
		current_color,
		group);
	  was_modified=False;
	  backup_was_modified=True;
	  redraw(); 
	  break;
	case ControlMask|ShiftMask:
	  printf("EXPORTING:\n");
          (*export_last_ptr)();
	  redraw(); break;
	};
      break;

    case XK_g:
      {
	int v=-1;
	if(context== context_default)
	  v=vertex_used_find(cursor);
	else 
	  {
	    if(context==context_et0)
	      v=vertex_used_find(et0_marker);
	  }
	if(v==-1)
	  printf("Either bad context or the cursor/et0-marker is not at a vertex\n");
	else
	  {
	    graph_create(edge_top, edge, triangle_top, triangle);
	    graph_bfs(v);
	    graph_menu();
	  }
      }
      break;

    case XK_v:
      if(context!=context_et0)
	printf("context should be: et0\n");
      else
	{
	    int v;
	    v=vertex_used_find(et0_marker);
	    if((evptr->state!=ControlMask) && v==-1)
	      printf("The first endpoint of cursor line is not at a vertex\n");
	    else 
	      {
		float mv[3];
		int first_free;
		vector_sub(cursor, et0_marker, mv);
		first_free=vertex_compress(vertex,
					   vertex_used,
					   edge_top, edge,
					   triangle_top, triangle, triangle_normal,
					   group);
		printf("first_free= %d\n", first_free);
		graph_create(edge_top, edge, triangle_top, triangle);
		graph_bfs(v);

		switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
		{
		case 0:
		  {
		    backup();
		    graph_move_marked(mv);
		    context_switch(context_default);
		    redraw();
		  } 
		  break;
		case ShiftMask:
		  {
		    backup();
		    graph_copy_move_marked(mv, first_free);
		    context_switch(context_default);
		    redraw();
		  } 
		  break;
		case ControlMask:
		  {
		    backup();
		    group_move(group_current, mv, 
			       group, vertex, vertex_used);
		     /*  context_switch(context_default); */
		    redraw();
		  } 
		  break;
		}
	      }
	}
      break;



    case XK_h:
      help_keys(); break;

    case  XK_equal:
      stats(); break;

    case XK_m:
      main_menu(); break;
    case XK_u:
      undo(); redraw(); break;
    case XK_q:
      if(!was_modified || are_you_sure()) exit(0); break;
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



 /* // REDRAW FUNCTIONS */


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
	       int group_current, Bool group_restricted, int group[])
{
  int i;
  glDisable(GL_LIGHTING);


  glBegin(GL_LINES);
      
  for(i=0; i<top; i++)
    if(!group_restricted || 
       (group[e[i][0]]==group_current && 
	group[e[i][1]]==group_current
	)
       )
      {
	if(!stereo_mode) glColor3fv( color[e[i][2]] );
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
	     group_current, False, group);
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


  draw_cursor();
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
	         group_current, group_restricted, group);
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
/*     { */
/*       glClearColor(stereo_background[0], */
/* 		   stereo_background[1], */
/* 		   stereo_background[2], 0.0); */

/*       glColor3fv(stereo_left_color); */
/*       glMatrixMode(GL_PROJECTION); */
/*       glLoadIdentity(); */
/*       glFrustum( */
/* 		(-rwidth+stereo_eye_distance)*screen.clip_min/ */
/* 		(2*screen.distance), */
/* 		(rwidth+stereo_eye_distance)*screen.clip_min/ */
/* 		(2*screen.distance), */
/*        		frustum_bottom, */
/* 		frustum_top, */
/* 		frustum_near_val, */
/* 		frustum_far_val */
/* 		); */

/*       glMatrixMode(GL_MODELVIEW); */
/*       glLoadIdentity(); */
/*       glTranslated(stereo_eye_distance/2,0,0); */
/*       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
/*       redraw_mono(); */

/*       glXSwapBuffers(display,window); */

/*       glClearColor(stereo_background[0], */
/* 		   stereo_background[1], */
/* 		   stereo_background[2], 0.0); */

/*       glColor3fv(stereo_right_color); */
/*       glMatrixMode(GL_PROJECTION); */
/*       glLoadIdentity(); */
/*       glFrustum( */
/* 		(-rwidth-stereo_eye_distance)*screen.clip_min/ */
/* 		(2*screen.distance), */
/* 		(rwidth-stereo_eye_distance)*screen.clip_min/ */
/* 		(2*screen.distance), */
/*        		frustum_bottom, */
/* 		frustum_top, */
/* 		frustum_near_val, */
/* 		frustum_far_val */
/* 		); */
/*       glMatrixMode(GL_MODELVIEW); */

/*       glLoadIdentity(); */
/*       glTranslated(-stereo_eye_distance/2,0,0); */
/*       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
/*       redraw_mono(); */

/*       glMatrixMode(GL_PROJECTION); */
/*       glLoadIdentity(); */

/*   glOrtho(0,screen.width, */
/*  	  0, screen.height, */
/* 	  0,3); */

/*   glMatrixMode(GL_MODELVIEW); */
/*   glLoadIdentity(); */


/* /\*       glDrawBuffer(GL_FRONT); *\/ */
/*       glReadBuffer(GL_FRONT); */

/*       glEnable(GL_COLOR_LOGIC_OP); */
/*       glLogicOp(GL_OR); */
/*   rwidth = screen.width*pixel_size; */
/*   rheight = screen.height*pixel_size; */


/*       glRasterPos3i(0,0,-1); */
/*       glCopyPixels(0,0,screen.width, screen.height,GL_COLOR); */
/*       glFlush(); */
/*       glLogicOp(GL_COPY); */
/*       glDisable(GL_COLOR_LOGIC_OP); */
/* /\*       glDrawBuffer(GL_BACK); *\/ */
/*       glReadBuffer(GL_BACK); */
/*       glXSwapBuffers(display,window); */

/*       glMatrixMode(GL_PROJECTION); */
/*       glLoadIdentity(); */
/*       glFrustum( */
/* 		frustum_left, */
/* 		frustum_right, */
/* 		frustum_bottom, */
/* 		frustum_top, */
/* 		frustum_near_val, */
/* 		frustum_far_val */
/* 		); */
/*       glMatrixMode(GL_MODELVIEW); */
/*     } */
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




 /* // MENU */


int are_you_sure()
{
  char s[4];
  printf("The changes will be lost !!!\n");
  printf("Are you sure? (yes/no)\n");
  scanf("%3s", s);


  if(strcmp(s,"yes")==0) return 1;
  else return 0;

}

void quit_menu()
{
  char s[4];

  XLowerWindow(display, window);
  XRaiseWindow(display, terminal);
  XSetInputFocus(display, terminal, RevertToParent, CurrentTime);

  printf("Are you sure to QUIT? (yes/no):\n");
  scanf("%3s", s);
  if(strcmp(s,"yes")==0) exit(0);

  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
  printf("\nReturn to graphical window\n");

}


void main_menu()
{
  char s[4];

  XLowerWindow(display, window);
  /* XIconifyWindow(display, window, DefaultScreen(display)); */
  /* XUnmapWindow(display, window); */ 

  XRaiseWindow(display, terminal);
  XSetInputFocus(display, terminal, RevertToParent, CurrentTime);

  do
    {
      printf("\nMAIN MENU:\n\n");
      printf("1  Cursor\n");
      printf("2  Screen\n");
      printf("3  Groups\n");
      printf("4  Export\n");
       /*  ... */
      printf("0  Return\n");
      printf("\nselect intem: ");
      scanf("%3s", s);

      if(strcmp(s,"1")==0) cursor_menu();
      else
      if(strcmp(s,"2")==0) screen_menu();
      else
      if(strcmp(s,"3")==0) group_menu();
      else
      if(strcmp(s,"4")==0) export_menu();

       /*  ... */
    }
  while(strcmp(s,"0")!=0);

  printf("\nReturn to graphical window\n");

  XLowerWindow(display, terminal);
 /* XMapWindow(display, window); */
/* XIconifyWindow(display, window, DefaultScreen(display)); */ /* screen_number set to 0 */
/* XMapRaised(display, window); */
  XRaiseWindow(display, window);
 XSetInputFocus(display, window, RevertToParent, CurrentTime); 

}

void screen_menu()
{
  char s[4];
  do
    {
      printf("\nSCREEN MENU:\n\n");
      printf("1  Screen distance (zoom)[%f]\n", screen.distance);
      printf("2. minimal clipping plane [%f]\n", screen.clip_min);
      printf("3. maximal clipping plane [%f]\n", screen.clip_max);
       /*  ... */
      printf("0  Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"1")==0)
	{
          printf("Screen distance is: %f.\n", screen.distance);
	  printf("Input new screen distance:\n");
	  screen_set_distance(&screen, get_float());
	}
      else
      if(strcmp(s,"2")==0)
	{
	  printf("Screen: clip_min = %f, clip_max = %f\n", screen.clip_min, screen.clip_max);
	  printf("Input new clip_min (between %f and %f):\n", clip_min_limit, screen.clip_max);
	  screen_set_clipping(&screen, get_float(), screen.clip_max);
	}
      if(strcmp(s,"3")==0)
	{
	  printf("Screen: clip_min = %f, clip_max = %f\n", screen.clip_min, screen.clip_max);
	  printf("Input new clip_max (greater than %f):\n", screen.clip_min);
	  screen_set_clipping(&screen,  screen.clip_min, get_float());
	}

      redraw();
    }
  while(strcmp(s,"0")!=0);
}


void cursor_menu()
{
  char s[4];
  do
    {
      printf("\nCURSOR MENU:\n\n");
      printf("1  Cursor step [%f]\n", cursor_step);
      printf("x  Cursor X [%f]\n", cursor[0]);
      printf("y  Cursor Y [%f]\n", cursor[1]);
      printf("z  Cursor Z [%f]\n", cursor[2]);
      if(context==context_et0 && 
	 !vector_eq(et0_marker, cursor))
	{
	  float v[3];
	  vector_sub(cursor, et0_marker,v);
	  printf("e move cursor to new distance from et0 [%f]\n",
		 vector_length(v));
	}
       /*  ... */
      printf("0  Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"1")==0)
	{
	  printf("Input cursor step (between %f and %f): ",cursor_min_step, cursor_max_step);
	  cursor_set_step(get_float());
	}
      else
      if(strcmp(s,"x")==0)
	{

	  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
	  printf("Input new cursor X coordinate: ");
	  cursor_set(get_float(), cursor[1], cursor[2]);
	}
      if(strcmp(s,"y")==0)
	{

	  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
	  printf("Input new cursor Y coordinate: ");
	  cursor_set(cursor[0], get_float(), cursor[2]);
	}
      if(strcmp(s,"z")==0)
	{

	  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
	  printf("Input new cursor Z coordinate: ");
	  cursor_set(cursor[0], cursor[1], get_float());
	}
      if(strcmp(s,"e")==0)
	{
	  float v[3];
	  vector_sub(cursor,et0_marker,v);
	  printf("input new distance: ");
	  vector_scale(get_float()/vector_length(v), v);
	  vector_add(v,et0_marker,v);
	  cursor_set(v[0], v[1], v[2]);
	}

       /*  ... */
      redraw();
    }
  while(strcmp(s,"0")!=0);

}


void group_menu()
{
  char s[4];
  int g_max;
  do
    {
      g_max=group_max(group, vertex_used);
      printf("\nGROUPS MENU\n");
      printf("(maximal group number is: %d)\n\n", g_max);
      printf("cg change current group [%d]\n", group_current);
      if(context==context_et0) 
	printf("mv move current group\n");
      printf("s  Scale\n");
      printf("sx Scale in X dimmension\n");
      printf("sy Scale in Y dimmension\n");
      printf("sz Scale in Z dimmension\n");


      printf("0  Return\n");

      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"cg")==0)
	{int g;
	  printf("Input group number (between 0 and %d):\n", g_max+1);
          scanf("%d", &g);
          if(0<=g && g<= g_max+1) 
	    group_change_current(g, &group_current, group, vertex_used); 
	  else printf("bad group number !\n");
	}
      else 
      if(context==context_et0 && strcmp(s,"mv")==0)
	{
	  float mv[3];
          vector_sub(cursor, et0_marker, mv);
	  backup();
          group_move(group_current, mv, group, vertex, vertex_used);
	}
      else
      if(strcmp(s,"s")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, s, s, s, cursor,
		    group, vertex, vertex_used);
	}
      else
      if(strcmp(s,"sx")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, s, 1.0, 1.0, cursor,
		    group, vertex, vertex_used);
	}
      else
      if(strcmp(s,"sy")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, 1.0, s, 1.0, cursor,
		    group, vertex, vertex_used);
	}
      else
      if(strcmp(s,"sz")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, 1.0, 1.0, s, cursor,
		    group, vertex, vertex_used);
	}

      redraw();
    }
  while(strcmp(s,"0")!=0);

}




void graph_menu()
{
  char s[4];
  printf("\nGRAPH CONNECTED COMPONET MENU:\n\n");
  printf("s  Scale\n");
  printf("sx Scale in X dimmension\n");
  printf("sy Scale in Y dimmension\n");
  printf("sz Scale in Z dimmension\n");
  printf("pe paint edges\n");
  printf("pt paint triangles\n");
  printf("de delete edges\n");
  printf("dt delete triangles\n");
  printf("rx rotate around X\n");
  printf("ry rotate around Y\n");
  printf("rz rotate around Z\n");
   /*  ... */
  printf("0  Return\n");
  printf("\nselect item: ");
  scanf("%3s", s);
  
  if(strcmp(s,"s")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(s,s,s, cursor);
    }
  else
  if(strcmp(s,"sx")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(s, 1.0, 1.0, cursor);
    }
  else
  if(strcmp(s,"sy")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(1.0,s, 1.0, cursor);
    }
  else
  if(strcmp(s,"sz")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(1.0, 1.0, s, cursor);
    }
  else
    if(strcmp(s,"pe")==0)
      {
	backup();
	graph_paint_marked_edges(current_color);
      }
  else
    if(strcmp(s,"pt")==0) 
      {
	backup();
	graph_paint_marked_triangles(current_color);
      }
  else
    if(strcmp(s,"de")==0)
      {
	backup();
	graph_delete_marked_edges();
      }
  else
    if(strcmp(s,"dt")==0) 
      {
	backup();
	graph_delete_marked_triangles();
      }
  if(strcmp(s,"rx")==0)
    {float a;
      printf("Input angle:\n");
      a=get_float();
      backup();
      graph_rotate_marked(a, 1,0,0, cursor );
    }
  if(strcmp(s,"ry")==0)
    {float a;
      printf("Input angle:\n");
      a=get_float();
      backup();
      graph_rotate_marked(a, 0,1,0, cursor );
    }
  if(strcmp(s,"rz")==0)
    {float a;
      printf("Input angle:\n");
      a=get_float();
      backup();
      graph_rotate_marked(a, 0,0,1, cursor );
    }
 
  

  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
  redraw();
}







void export_menu()
{
  char s[4];
  do
    {
      printf("\nEXPORT MENU:\n\n");
      printf("pov  Povray\n");
      printf("0  Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"pov")==0)
	{
	  povray_menu();
	}
    }
  while(strcmp(s,"0")!=0);
}

void povray_menu()
{
  char s[4];
  do
    {
      printf("\nPOVRAY EXPORT MENU:\n\n");
      printf("n   file name prefix [%s]\n",povray_name);
      printf("c   counter [%d]\n", povray_counter);
      printf("e   export to Povray INI and POV files\n");
      printf("er  edge radius [%f]\n", povray_edge_radius);
      printf("amb ambient [%f]\n", povray_ambient);
      printf("dif diffuse [%f]\n", povray_diffuse);
      printf("ld light distance [%f]\n", povray_light_distance);
      printf("0   Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"n")==0)
	{
	  printf("Input new file name prefix:\n");
	  get_file_name(povray_name);
	}
      else
      if(strcmp(s,"c")==0)
	{
	  printf("Input new counter value:\n");
	  scanf("%d", &povray_counter);
	}
      else
      if(strcmp(s,"e")==0)
	{
	  povray_export();
	}
      else
      if(strcmp(s,"er")==0)
	{
	  printf("Edges are exported as cylinders\n");
	  printf("Input new radius of the cylinder\n"); 
	  printf("(0 if you do not want to export edges):\n");
	  povray_edge_radius=get_float();
	}
      else
      if(strcmp(s,"amb")==0)
	{
	  printf("Input ambient value (>=0 )\n"); 
	  povray_ambient=get_float();
	}
      else
      if(strcmp(s,"dif")==0)
	{
	  printf("Input diffuse value (>=0 )\n"); 
	  povray_diffuse=get_float();
	}
      else
      if(strcmp(s,"ld")==0)
	{
	  printf("Input light distance from the point (0,0,0) (>=0 )\n"); 
	  povray_light_distance=get_float();
	}
    }
  while(strcmp(s,"0")!=0);
}


 /* ///// GRAPH FUNCTIONS */


void graph_marked_init()
{
  int i;
  for(i=0; i<VERTEX_MAX; i++) graph_marked[i]=-1;
}


void graph_queue_init()
{
  graph_queue_first=0;
  graph_queue_size=0;
}


void graph_queue_insert(int v)
{
  if(graph_queue_size>= VERTEX_MAX) 
    {
      printf("GRAPH QUEUE overflow !!!\n");
      return;
    }

  graph_queue[(graph_queue_first+graph_queue_size) % VERTEX_MAX]=v;
  graph_queue_size++;
}


int graph_queue_get()
{
  int v;
  if(graph_queue_size<= 0) 
    {
      printf("GRAPH QUEUE empty !!!\n");
      return -1;
    }
  v=graph_queue[graph_queue_first];
  graph_queue_first = (graph_queue_first+1) % VERTEX_MAX;
  graph_queue_size--;
  return v;
}


void graph_push_nbr(int v, int nbr)
{
  int tmp_nxt;
  if(graph_next_free>=2*EDGE_MAX+6*TRIANGLE_MAX) 
    {
      printf("GRAPH overflow ?!\n");
      return;
    }

  tmp_nxt=graph_first_nbr[v];
  graph_first_nbr[v]=graph_next_free;
  graph_next_nbr[graph_next_free]=tmp_nxt;
  graph_nbr[graph_next_free]=nbr;
  graph_next_free++;
}


void graph_create(int edge_top, int edge[][3], int triangle_top, int triangle[][4])
{
  int i;
  for(i=0; i<VERTEX_MAX; i++)
    graph_first_nbr[i]=-1;

  for(i=0; i<edge_top; i++)
    {
      graph_push_nbr(edge[i][0], edge[i][1]);
      graph_push_nbr(edge[i][1], edge[i][0]);
    }

  for(i=0; i<triangle_top; i++)
    {
      graph_push_nbr(triangle[i][0], triangle[i][1]);
      graph_push_nbr(triangle[i][0], triangle[i][2]);
      graph_push_nbr(triangle[i][1], triangle[i][0]);
      graph_push_nbr(triangle[i][1], triangle[i][2]);
      graph_push_nbr(triangle[i][2], triangle[i][0]);
      graph_push_nbr(triangle[i][2], triangle[i][1]);
    }
}


int graph_bfs(int v)
{
  int mark=0;
  graph_marked_init();
  graph_queue_init();
  graph_marked[v]=mark;
  graph_queue_insert(v);
  mark++;
  while(graph_queue_size>0)
    {
      int nxt, x;
      x=graph_queue_get();
      nxt=graph_first_nbr[x];
      while(nxt!=-1)
	{
	  if(graph_marked[graph_nbr[nxt]]==-1)
	    {
	      graph_marked[graph_nbr[nxt]]=mark;
              graph_queue_insert(graph_nbr[nxt]);
	      mark++;
	    }
	  nxt=graph_next_nbr[nxt];
	}
    }
  return mark;  /*  number of marked vertices */
}

void graph_move_marked(float vector[3])
{
  int i;
  for(i=0; i<VERTEX_MAX; i++)
    if(graph_marked[i]!=-1) vector_add(vector, vertex[i], vertex[i]);
}



void graph_copy_move_marked(float vector[3], int first_free )
{
  int i,size, e_top, t_top, e[2], t[3];

  size=0;
  for(i=0; i<first_free && first_free+size<VERTEX_MAX; i++)
    if(graph_marked[i]!=-1)
      {
	vectorcpy(vertex[first_free+size],vertex[i]);
        graph_marked[i]=first_free+size;
        size++;
      }

  if(first_free+size>=VERTEX_MAX && i<first_free) 
    {
      printf("GRAPH COPY: vertex table overflow !!!\n");
      return;
    }

  e_top= edge_top;
  for(i=0; i<e_top && edge_top<EDGE_MAX; i++)
    if(graph_marked[edge[i][0]])
      {
	e[0]=graph_marked[edge[i][0]];
	e[1]=graph_marked[edge[i][1]];
	edge_set_color(edge_insert(e), edge[i][2]);
      }

  t_top= triangle_top;
  for(i=0; i<t_top && triangle_top<TRIANGLE_MAX; i++)
    if(graph_marked[triangle[i][0]])
      {
	t[0]=graph_marked[triangle[i][0]];
	t[1]=graph_marked[triangle[i][1]];
	t[2]=graph_marked[triangle[i][2]];
	triangle_set_color(triangle_insert(t), triangle[i][3]);
      }



  graph_move_marked(vector);
}


void graph_scale_marked(double sx, double sy, double sz, float cursor[3])
{
   /*  cursor is a fixed point  */

  int i;
  for(i=0; i<VERTEX_MAX; i++)
    if(graph_marked[i]!=-1)
      {
	vertex[i][0]=sx*(vertex[i][0]-cursor[0])+cursor[0];
	vertex[i][1]=sy*(vertex[i][1]-cursor[1])+cursor[1];
	vertex[i][2]=sz*(vertex[i][2]-cursor[2])+cursor[2];
      }

}


void graph_paint_marked_edges(int color)
{
  int i;
  for(i=0; i<edge_top; i++)
    if(graph_marked[edge[i][0]]!=-1) edge[i][2]=color;
}


void graph_paint_marked_triangles(int color)
{
  int i;
  for(i=0; i<triangle_top; i++)
    if(graph_marked[triangle[i][0]]!=-1) triangle[i][3]=color;
}


void graph_delete_marked_edges()
{
  int i=0;
  while(i<edge_top)
    {
      if( graph_marked[ edge[i][0] ] !=-1 ) edge_delete(i);
      else i++;
    }
}

void graph_delete_marked_triangles()
{
  int i=0;
  while(i<triangle_top)
    {
      if( graph_marked[ triangle[i][0] ] !=-1 ) triangle_delete(i);
      else i++;
    }
}



void graph_rotate_marked(double angle, double x, double y, double z, float cursor[3])
{
  float M[16];
  int i;

  float_zeroes(16,M);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRotatef(angle, x,y,z);

  for(i=0; i<VERTEX_MAX; i++)
    if(graph_marked[i]!=-1)
      {
        vector_sub(vertex[i], cursor, M);
        
	glPushMatrix();
      	glMultMatrixf(M);
	glGetFloatv( GL_MODELVIEW_MATRIX, M);
        vector_add(M, cursor, vertex[i]);
	glPopMatrix();
      }
  glPopMatrix();

  for(i=0; i<triangle_top; i++)
    if(graph_marked[triangle[i][0]])
      triangle_set_normal(i);
  
}




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
  printf("Copyright (C) 2003  Marcin Kik      kik@im.pwr.wroc.pl\n");
  printf("\nE.T. EDITOR comes with ABSOLUTELY NO WARRANTY.\n");

  printf("This is unstable test version, that can be freely redistributed.\n");
  printf("Send any comments or suggestions to: kik@im.pwr.wroc.pl\n");
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




