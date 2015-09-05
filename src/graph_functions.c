#include "./et-edit.h"

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



