void export_c_ve(char * name_prefix)
{
/* UNFINISHED !!! */
  int n_vertices;
  char * fname; /* make it real table */
  FILE *stream;


  fname=name_prefix; /* update ! */  

  stream=fopen(fname,"w");
  if(stream==NULL) 
  {
    printf("save: can not open '%s'\n", fname);
    return ; /* -1; */
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

  fclose(stream);
  printf("NOT FINISHED: only saved to: '%s'\n", fname);

}
