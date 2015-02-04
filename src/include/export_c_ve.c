
void c_vertex_fprintf(FILE* s, int n, float v[][3], char * name_prefix)
{
  int i;
  fprintf(s, "\nfloat %s_vertex[%d][3]={\n", name_prefix, n);
  for(i=0; i<n; i++) 
    fprintf(s, "  { %f, %f, %f},\n", v[i][0], v[i][1], v[i][2]);
  fprintf(s, "};\n");
}

void c_edge_fprintf(FILE* s, int edge_top, int edge[][3], char * name_prefix)
{
  int i;
  fprintf(s, "\nint %s_edge_top=%d;\n", name_prefix, edge_top);
  fprintf(s, "\nint %s_edge[%d][3]={\n", name_prefix, edge_top);
  for(i=0; i<edge_top; i++)
    {
      fprintf(s, "  { %d, %d, %d},\n", edge[i][0], edge[i][1], edge[i][2]);
    }
  fprintf(s, "};\n");
}


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

  c_vertex_fprintf(stream, n_vertices, vertex, name_prefix);
  fprintf(stream, "\n");

   /*  EDGES */
  c_edge_fprintf(stream, edge_top, edge, name_prefix);
  fprintf(stream, "\n");

  /* DEFINFE SHAPE _VE */
 
  fprintf(stream, "SHAPE_VE %s = { %s_vertex, %d, %s_edge };\n", name_prefix, name_prefix, edge_top, name_prefix);

  fclose(stream);
  printf("C data exported to: '%s'\n", fname);

}
