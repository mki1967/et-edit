#include "./et-edit.h"

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




