#include "./et-edit.h"

void main_menu()
{
  char * s;


  goto_terminal();

  do
    {
      printf("\nMAIN MENU:\n\n");
      printf("1  Cursor\n");
      printf("2  Screen\n");
      /* printf("3  Groups\n"); */
      printf("4  Export\n");
       /*  ... */
      printf("0  Return\n");

      s=get_menu_selector();

      if(strcmp(s,"1")==0) cursor_menu();
      else
      if(strcmp(s,"2")==0) screen_menu();
      else
/*    
      if(strcmp(s,"3")==0) group_menu();
      else
*/
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



