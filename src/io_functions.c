#include "include/et-edit.h"

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


