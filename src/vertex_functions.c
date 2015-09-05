#include "./et-edit.h"

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


