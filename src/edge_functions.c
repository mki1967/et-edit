#include "./et-edit.h"


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

