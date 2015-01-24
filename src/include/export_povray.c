
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



