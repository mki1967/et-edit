#include "./et-edit.h"

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


