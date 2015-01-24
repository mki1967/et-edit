
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


