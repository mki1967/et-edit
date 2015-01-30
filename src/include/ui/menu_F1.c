/*
GROUP MENU (F1 MENU)
*/

void menu_F1()
{  
char * s; /* pointer to selector */
int g_max;
g_max=group_max(group, vertex_used);

printf("GROUP (F1) MENU:\n");
printf("(maximal group number is: %d)\n\n", g_max);

printf("  =     Print current group statistics\n");  
printf("  cg    Change current group [%d]\n", group_current);
printf("  m     Mark current group [now marked: %d]\n", group_marker);
printf("  -m    Set group marker to -1 (no group)\n");
printf("  m2c   Move vertices from marked group (%d) to current group (%d)\n", group_marker, group_current );
printf("  ex    Extract visible vertices to a new group\n");
printf("  fi    Change to a group incident to the cursor\n");
printf("  cp    Copy to new group\n");  
printf("  cpe   Copy and glue with edges\n");  
printf("  cpt   Copy and glue with triangles\n");  
printf("  cpet  Copy and glue with edges and triangles\n");  
printf("  z     Remove empty groups (renumbers the remaining groups)\n");
printf("  de    Delete edges incident to current group\n");
printf("  dt    Delete triangles incident to current group\n");
printf("  det   Delete edges and triangles incident to current group\n");
printf("  pe    Paint edges of current group with current color\n");
printf("  pt    Paint triangles of current group with current color\n");

printf("  s     Scale\n");
printf("  sx    Scale in X dimmension\n");
printf("  sy    Scale in Y dimmension\n");
printf("  sz    Scale in Z dimmension\n");
printf("  rx    rotate current group 90 degrees around X\n");
printf("  ry    rotate current group 90 degrees around Y\n");
printf("  rz    rotate current group 90 degrees around Z\n");
if(context==context_et0) 
  printf("  mv    move current group by vector (et0_marker, cursor)\n");

s=get_menu_selector();

printf("%s\n", s);

if(strcmp(s,"cg")==0)
	{int g;
	  printf("Input group number (between 0 and %d):\n", g_max+1);
          scanf("%d", &g);
          if(0<=g && g<= g_max+1) 
	    group_change_current(g, &group_current, group, vertex_used); 
	  else printf("bad group number !\n");
	}
else
if(strcmp(s,"s")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, s, s, s, cursor,
		    group, vertex, vertex_used);
	}
else
if(strcmp(s,"sx")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, s, 1.0, 1.0, cursor,
		    group, vertex, vertex_used);
	}
else
if(strcmp(s,"sy")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, 1.0, s, 1.0, cursor,
		    group, vertex, vertex_used);
	}
else
if(strcmp(s,"sz")==0)
	{float s;
	printf("Input scaling factor:\n");
	s=get_float();
	backup();
	group_scale(group_current, 1.0, 1.0, s, cursor,
		    group, vertex, vertex_used);
	}
else
if(strcmp(s,"ry")==0){
      backup();
      group_rot90(group_current, 0, 1, 0);
} else
if(strcmp(s,"rx")==0){
      backup();
      group_rot90(group_current, 1, 0, 0);
} else
if(strcmp(s,"rz")==0){
      backup();
      group_rot90(group_current, 0, 0, 1);
} else
if(strcmp(s,"=")==0){
      group_statistics(group_current); 
      get_anything();
} else
if(strcmp(s,"ex")==0){
      backup();
      group_extract_clipped();
} else
if(strcmp(s,"m")==0){
	  group_mark(group_current);
} else
if(strcmp(s,"-m")==0){
	  group_mark(-1);
} else
if(strcmp(s,"m2c")==0){
	  backup();
          group_regroup(group_marker, group_current);
} else
if(strcmp(s,"z")==0){
          group_remove_empty();
          get_anything();
} else
if(strcmp(s,"fi")==0){
            int found;
	    found=group_find(cursor);
	    if(found!=-1)
	      {
		group_change_current(found, &group_current, group, vertex_used);
	      }
	    else
	      printf("CURSOR IS NOT INCIDENT TO ANY GROUP\n");
            get_anything();
} else
if(context==context_et0 && strcmp(s,"mv")==0){
	  float mv[3];
          vector_sub(cursor, et0_marker, mv);
	  backup();
          group_move(group_current, mv, group, vertex, vertex_used);
} else
if(strcmp(s,"de")==0){
	  backup();
          group_delete_edges(group_current);
} else
if(strcmp(s,"dt")==0){
	  backup();
          group_delete_triangles(group_current);
} else
if(strcmp(s,"det")==0){
	  backup();
          group_delete_triangles(group_current);
          group_delete_edges(group_current);
} else
if(strcmp(s,"pe")==0){
	  backup();
          group_paint_edges(group_current, current_color);
} else
if(strcmp(s,"pt")==0){
	  backup();
          group_paint_triangles(group_current, current_color);
} else
if(strcmp(s,"cp")==0){
	  backup();
          group_copy(False, False);
} else
if(strcmp(s,"cpe")==0){
	  backup();
          group_copy(True, False);
} else
if(strcmp(s,"cpt")==0){
	  backup();
          group_copy(False, True);
} else
if(strcmp(s,"cpet")==0){
	  backup();
          group_copy(True, True);
}

}


