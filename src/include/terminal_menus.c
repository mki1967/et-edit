 /* // TERMINAL INTERACTION */

void get_file_name(char *file_name)
{
  char new_name[MAX_FILE_NAME+5];
  XRaiseWindow(display, terminal);
  XSetInputFocus(display, terminal, RevertToParent, CurrentTime);
  printf("Input file name (input '-' to leave the old name: '%s'):\n", file_name);
  scanf("%100s", new_name);
  if( strcmp("-",new_name)!=0 ) strcpy(file_name, new_name);
  printf("New file name is: '%s'\n", file_name);
  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
}


float get_float()
{
  float x;
  scanf("%f", &x);
  return x;
}


int are_you_sure()
{
  char s[4];
  printf("The changes will be lost !!!\n");
  printf("Are you sure? (yes/no)\n");
  scanf("%3s", s);


  if(strcmp(s,"yes")==0) return 1;
  else return 0;

}

void quit_menu()
{
  char s[4];

  XLowerWindow(display, window);
  XRaiseWindow(display, terminal);
  XSetInputFocus(display, terminal, RevertToParent, CurrentTime);

  printf("Are you sure to QUIT? (yes/no):\n");
  scanf("%3s", s);
  if(strcmp(s,"yes")==0) exit(0);

  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
  printf("\nReturn to graphical window\n");

}


void main_menu()
{
  char s[4];


  goto_terminal();

  do
    {
      printf("\nMAIN MENU:\n\n");
      printf("1  Cursor\n");
      printf("2  Screen\n");
      printf("3  Groups\n");
      printf("4  Export\n");
       /*  ... */
      printf("0  Return\n");
      printf("\nselect intem: ");
      scanf("%3s", s);

      if(strcmp(s,"1")==0) cursor_menu();
      else
      if(strcmp(s,"2")==0) screen_menu();
      else
      if(strcmp(s,"3")==0) group_menu();
      else
      if(strcmp(s,"4")==0) export_menu();

       /*  ... */
    }
  while(strcmp(s,"0")!=0);

  printf("\nReturn to graphical window\n");

goto_window();


}

void screen_menu()
{
  char s[4];
  do
    {
      printf("\nSCREEN MENU:\n\n");
      printf("1  Screen distance (zoom)[%f]\n", screen.distance);
      printf("2. minimal clipping plane [%f]\n", screen.clip_min);
      printf("3. maximal clipping plane [%f]\n", screen.clip_max);
       /*  ... */
      printf("0  Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"1")==0)
	{
          printf("Screen distance is: %f.\n", screen.distance);
	  printf("Input new screen distance:\n");
	  screen_set_distance(&screen, get_float());
	}
      else
      if(strcmp(s,"2")==0)
	{
	  printf("Screen: clip_min = %f, clip_max = %f\n", screen.clip_min, screen.clip_max);
	  printf("Input new clip_min (between %f and %f):\n", clip_min_limit, screen.clip_max);
	  screen_set_clipping(&screen, get_float(), screen.clip_max);
	}
      if(strcmp(s,"3")==0)
	{
	  printf("Screen: clip_min = %f, clip_max = %f\n", screen.clip_min, screen.clip_max);
	  printf("Input new clip_max (greater than %f):\n", screen.clip_min);
	  screen_set_clipping(&screen,  screen.clip_min, get_float());
	}

      redraw();
    }
  while(strcmp(s,"0")!=0);
}


void cursor_menu()
{
  char s[4];
  do
    {
      printf("\nCURSOR MENU:\n\n");
      printf("1  Cursor step [%f]\n", cursor_step);
      printf("x  Cursor X [%f]\n", cursor[0]);
      printf("y  Cursor Y [%f]\n", cursor[1]);
      printf("z  Cursor Z [%f]\n", cursor[2]);
      if(context==context_et0 && 
	 !vector_eq(et0_marker, cursor))
	{
	  float v[3];
	  vector_sub(cursor, et0_marker,v);
	  printf("e move cursor to new distance from et0 [%f]\n",
		 vector_length(v));
	}
       /*  ... */
      printf("0  Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"1")==0)
	{
	  printf("Input cursor step (between %f and %f): ",cursor_min_step, cursor_max_step);
	  cursor_set_step(get_float());
	}
      else
      if(strcmp(s,"x")==0)
	{

	  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
	  printf("Input new cursor X coordinate: ");
	  cursor_set(get_float(), cursor[1], cursor[2]);
	}
      if(strcmp(s,"y")==0)
	{

	  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
	  printf("Input new cursor Y coordinate: ");
	  cursor_set(cursor[0], get_float(), cursor[2]);
	}
      if(strcmp(s,"z")==0)
	{

	  printf("CURSOR: %f, %f, %f\n", cursor[0], cursor[1], cursor[2]);
	  printf("Input new cursor Z coordinate: ");
	  cursor_set(cursor[0], cursor[1], get_float());
	}
      if(strcmp(s,"e")==0)
	{
	  float v[3];
	  vector_sub(cursor,et0_marker,v);
	  printf("input new distance: ");
	  vector_scale(get_float()/vector_length(v), v);
	  vector_add(v,et0_marker,v);
	  cursor_set(v[0], v[1], v[2]);
	}

       /*  ... */
      redraw();
    }
  while(strcmp(s,"0")!=0);

}


void group_menu()
{
  char s[4];
  int g_max;
  do
    {
      g_max=group_max(group, vertex_used);
      printf("\nGROUPS MENU\n");
      printf("(maximal group number is: %d)\n\n", g_max);
      printf("cg change current group [%d]\n", group_current);
      if(context==context_et0) 
	printf("mv move current group\n");
      printf("s  Scale\n");
      printf("sx Scale in X dimmension\n");
      printf("sy Scale in Y dimmension\n");
      printf("sz Scale in Z dimmension\n");


      printf("0  Return\n");

      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"cg")==0)
	{int g;
	  printf("Input group number (between 0 and %d):\n", g_max+1);
          scanf("%d", &g);
          if(0<=g && g<= g_max+1) 
	    group_change_current(g, &group_current, group, vertex_used); 
	  else printf("bad group number !\n");
	}
      else 
      if(context==context_et0 && strcmp(s,"mv")==0)
	{
	  float mv[3];
          vector_sub(cursor, et0_marker, mv);
	  backup();
          group_move(group_current, mv, group, vertex, vertex_used);
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

      redraw();
    }
  while(strcmp(s,"0")!=0);

}




void graph_menu()
{
  char s[4];
  printf("\nGRAPH CONNECTED COMPONET MENU:\n\n");
  printf("s  Scale\n");
  printf("sx Scale in X dimmension\n");
  printf("sy Scale in Y dimmension\n");
  printf("sz Scale in Z dimmension\n");
  printf("pe paint edges\n");
  printf("pt paint triangles\n");
  printf("de delete edges\n");
  printf("dt delete triangles\n");
  printf("rx rotate around X\n");
  printf("ry rotate around Y\n");
  printf("rz rotate around Z\n");
   /*  ... */
  printf("0  Return\n");
  printf("\nselect item: ");
  scanf("%3s", s);
  
  if(strcmp(s,"s")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(s,s,s, cursor);
    }
  else
  if(strcmp(s,"sx")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(s, 1.0, 1.0, cursor);
    }
  else
  if(strcmp(s,"sy")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(1.0,s, 1.0, cursor);
    }
  else
  if(strcmp(s,"sz")==0)
    {float s;
      printf("Input scaling factor:\n");
      s=get_float();
      backup();
      graph_scale_marked(1.0, 1.0, s, cursor);
    }
  else
    if(strcmp(s,"pe")==0)
      {
	backup();
	graph_paint_marked_edges(current_color);
      }
  else
    if(strcmp(s,"pt")==0) 
      {
	backup();
	graph_paint_marked_triangles(current_color);
      }
  else
    if(strcmp(s,"de")==0)
      {
	backup();
	graph_delete_marked_edges();
      }
  else
    if(strcmp(s,"dt")==0) 
      {
	backup();
	graph_delete_marked_triangles();
      }
  if(strcmp(s,"rx")==0)
    {float a;
      printf("Input angle:\n");
      a=get_float();
      backup();
      graph_rotate_marked(a, 1,0,0, cursor );
    }
  if(strcmp(s,"ry")==0)
    {float a;
      printf("Input angle:\n");
      a=get_float();
      backup();
      graph_rotate_marked(a, 0,1,0, cursor );
    }
  if(strcmp(s,"rz")==0)
    {float a;
      printf("Input angle:\n");
      a=get_float();
      backup();
      graph_rotate_marked(a, 0,0,1, cursor );
    }
 
  

  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
  redraw();
}







void export_menu()
{
  char s[4];
  do
    {
      printf("\nEXPORT MENU:\n\n");
      printf("pov  Povray\n");
      printf("0  Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"pov")==0)
	{
	  povray_menu();
	}
    }
  while(strcmp(s,"0")!=0);
}

void povray_menu()
{
  char s[4];
  do
    {
      printf("\nPOVRAY EXPORT MENU:\n\n");
      printf("n   file name prefix [%s]\n",povray_name);
      printf("c   counter [%d]\n", povray_counter);
      printf("e   export to Povray INI and POV files\n");
      printf("er  edge radius [%f]\n", povray_edge_radius);
      printf("amb ambient [%f]\n", povray_ambient);
      printf("dif diffuse [%f]\n", povray_diffuse);
      printf("ld light distance [%f]\n", povray_light_distance);
      printf("0   Return\n");
      printf("\nselect item: ");
      scanf("%3s", s);

      if(strcmp(s,"n")==0)
	{
	  printf("Input new file name prefix:\n");
	  get_file_name(povray_name);
	}
      else
      if(strcmp(s,"c")==0)
	{
	  printf("Input new counter value:\n");
	  scanf("%d", &povray_counter);
	}
      else
      if(strcmp(s,"e")==0)
	{
	  povray_export();
	}
      else
      if(strcmp(s,"er")==0)
	{
	  printf("Edges are exported as cylinders\n");
	  printf("Input new radius of the cylinder\n"); 
	  printf("(0 if you do not want to export edges):\n");
	  povray_edge_radius=get_float();
	}
      else
      if(strcmp(s,"amb")==0)
	{
	  printf("Input ambient value (>=0 )\n"); 
	  povray_ambient=get_float();
	}
      else
      if(strcmp(s,"dif")==0)
	{
	  printf("Input diffuse value (>=0 )\n"); 
	  povray_diffuse=get_float();
	}
      else
      if(strcmp(s,"ld")==0)
	{
	  printf("Input light distance from the point (0,0,0) (>=0 )\n"); 
	  povray_light_distance=get_float();
	}
    }
  while(strcmp(s,"0")!=0);
}


