
void callbackKeyPress( XKeyEvent* evptr)
{
  switch(keymode)
    {
    case key_default:
      callback_key_default(evptr);
      break;
    case key_F1:
      callback_key_F1(evptr);
      break;
    case key_F2:
      callback_key_F2(evptr);
      break;
    case key_F3:
      callback_key_F3(evptr);
      break;
    case key_F4:
      callback_key_F4(evptr);
      break;
    case key_F5:
      callback_key_F5(evptr);
      break;
    case key_F6:
       /*  callback_key_F6(evptr); */
      break;
    case key_F7:
      callback_key_F7(evptr);
      break;
    }
}

void callback_key_F7(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F7;
      break;
    case XK_F7:
      stereo_mode= !stereo_mode;
      if(stereo_mode)
	{
	  printf("STEREO MODE\n");
	  clip_cube(&clipping_xyz);
	}
      else printf("COLOR MODE\n");
      redraw();
      break;
    case XK_1:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(-0.1,0,0, stereo_left_color);
	  break;
	case ShiftMask:
	  color_adjust(0.1,0,0, stereo_left_color);
	  break;
	}
      redraw();
      printf("LEFT COLOR (%f, %f, %f)\n", 
	     stereo_left_color[0],
	     stereo_left_color[1],
	     stereo_left_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_2:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,-0.1,0, stereo_left_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0.1,0, stereo_left_color);
	  break;
	}
      redraw();
      printf("LEFT COLOR (%f, %f, %f)\n", 
	     stereo_left_color[0],
	     stereo_left_color[1],
	     stereo_left_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_3:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,0,-0.1, stereo_left_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0,0.1, stereo_left_color);
	  break;
	}
      redraw();
      printf("LEFT COLOR (%f, %f, %f)\n", 
	     stereo_left_color[0],
	     stereo_left_color[1],
	     stereo_left_color[2]
	     );
      next_mode=key_F7;
      break;

    case XK_7:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(-0.1,0,0, stereo_right_color);
	  break;
	case ShiftMask:
	  color_adjust(0.1,0,0, stereo_right_color);
	  break;
	}
      redraw();
      printf("RIGHT COLOR (%f, %f, %f)\n", 
	     stereo_right_color[0],
	     stereo_right_color[1],
	     stereo_right_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_8:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,-0.1,0, stereo_right_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0.1,0, stereo_right_color);
	  break;
	}
      redraw();
      printf("RIGHT COLOR (%f, %f, %f)\n", 
	     stereo_right_color[0],
	     stereo_right_color[1],
	     stereo_right_color[2]
	     );
      next_mode=key_F7;
      break;
    case XK_9:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  color_adjust(0,0,-0.1, stereo_right_color);
	  break;
	case ShiftMask:
	  color_adjust(0,0,0.1, stereo_right_color);
	  break;
	}
      redraw();
      printf("RIGHT COLOR (%f, %f, %f)\n", 
	     stereo_right_color[0],
	     stereo_right_color[1],
	     stereo_right_color[2]
	     );
      next_mode=key_F7;
      break;

    case XK_c:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  clipping_cube_size-=0.5;
	  if(clipping_cube_size<1) clipping_cube_size=1;
	  break;
	case ShiftMask:
	  clipping_cube_size+=0.5;
	  break;
	case ControlMask:
	  clipping_cube_size/=2;
	  if(clipping_cube_size<1) clipping_cube_size=1;
	  break;
	case ControlMask|ShiftMask:
	  clipping_cube_size*=2;
	  break;
	}
      clip_cube(&clipping_xyz);
      redraw();
      next_mode=key_F7;
      break;



    case XK_x:
      {
	int i,t;
	for(i=0; i<3; i++)
	  {
	    t=stereo_left_mask[i];
	    stereo_left_mask[i]=stereo_right_mask[i];
	    stereo_right_mask[i]=t;
	  }
      }
      vector_swap(stereo_left_color, stereo_right_color);
      printf("left and right color swaped\n");
      redraw();
      break;

    case XK_e:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  stereo_eye_distance-=0.1;
	  break;
	case ShiftMask:
	  stereo_eye_distance+=0.1;	  
	  break;
	}
      redraw();
      printf("EYE DISTANCE: %f\n", 
	     stereo_eye_distance
	     );
      next_mode=key_F7;
      break;

    case XK_p:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case ControlMask:
	  printf("Printing PCL to file: %s\n", pcl_file_name);
	  pcl_print();
	  break;
	}
      break;
    }


  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F7_help()
{
  printf("F7 stereo mode control:\n");
  printf("\n");
  printf("<F7> switch stereo mode ON/OFF\n");
  printf("[<Shift>+] <1>/<2>/<3> Modify R/G/B values of the left color\n");
  printf("[<Shift>+] <7>/<8>/<9> Modify R/G/B values of the right color\n");
/*   printf("[<Shift>+] <4>/<5>/<6> Modify R/G/B values of the background\n"); */
  printf("<Shift>+<E>/<E> Modify distance between eyes\n");
  printf("<Shift>+<C>/<C> Modify clipping cube size\n");
  printf("<Ctrl>+(<Shift>+<C>/<C>) double/halve clipping cube size\n");
  printf("<X> exchange left and right colors\n");
}



void callback_key_F5(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F5;
      break;

    case XK_1:
      printf("Inputnew reduction epsilon (>= 0):");
      reduction_epsilon=get_float();
      if(reduction_epsilon<0) reduction_epsilon=0.0;
      printf("\n new value of reduction epsilon is: %e\n", reduction_epsilon);
      redraw();
      break;

    case XK_v:
      reduce_group_vertices(group_current);
      redraw();
      break;

/*     case XK_s: */
/*       qsort(edge,edge_top, sizeof(int[3]), edge_compare); */
/*       qsort(triangle,triangle_top, sizeof(int[4]), triangle_compare); */
/*       redraw(); */
/*       break; */

    }


  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F5_help()
{
  printf("F5 mode (REDUCTIONS):\n");
  printf("\n");
  printf("<1> input new reduction epsilon [%e]\n", reduction_epsilon);
  printf("<V> reduce vertices in current group\n");
}


void callback_key_F4(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F4;
      break;

    case XK_i:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  {
	    float X[3];
	    vectorcpy(X, cursor);
	    line_triangle_intersection(point[10], point[11],
				       point[12], point[13], point[14],
				       X);
	    cursor_set(X[0], X[1], X[2]);
	    
	  }
	  redraw();
	  break;
	case ShiftMask:
	  group_tt_intersection(group_current, group_marker);
	  redraw();
	  break;
	}
      break;
    case XK_c:
      {
	float X[3];
	vectorcpy(X, cursor);
	find_group_center(group_current, X);
	cursor_set(X[0], X[1], X[2]);
      }
      redraw();
      break;

    case XK_n:
      {
	float X[3];
	triangle_normal_vector(point[10], point[11],point[12],X);
        vector_scale(cursor_step,X);
        vectorcpy(et0_marker, cursor);
        vector_add(cursor, X, X);
        context_switch(context_et0);
	cursor_set(X[0], X[1], X[2]);

      }
      redraw();
      break;

    case XK_s:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  constr_scale(point[10], point[11],
		       point[12], point[13], cursor,
		       group_current);
	  break;
	case ControlMask:
	  constr_scale_in_direction(point[10], point[11],
				    point[12], point[13], 
				    point[14], point[15], 
				    cursor,
				    group_current);
	  break;
	}
      redraw();
      break;

    case XK_t:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  three_point_transform(point[10], point[11], point[12], 
				point[13], point[14], point[15],
				group_current);
	  break;
	}
      redraw();
      break;

    case XK_f:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  {
	    float V[3];
	    if(find_folding(point[10], 
			    point[11], point[12], 
			    point[13], point[14], 
			    point[15],
			    V)
	       )
	      {
		cursor_set(V[0], V[1], V[2]);
	      }
	  }
	  break;
	case ControlMask:
	  {
	    float V[3], mv1[3], mv2[3], R1[3][3], R2[3][3];
	    if(find_folding(point[10], 
			    point[11], point[12], 
			    point[13], point[14], 
			    point[15],
			    V)
	       &&
	       find_three_point_transform(point[10], point[11], point[13],
					 point[10], point[11], V,
					 mv1,R1)
	       &&
	       find_three_point_transform(point[10], point[12], point[14],
					 point[10], point[12], V,
					 mv2,R2)
	       )
	      {

		backup();
		group_transform(group_current,mv2,R2);
		if(group_marker!=group_current)
		  group_transform(group_marker,mv1,R1);
 
	         /*  DOKONCZ ... */
	      }
	  }
	  break;
	}
      redraw();
      break;
      
      
    }


  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F4_help()
{
  printf("F4 mode (CONSTRUCTIVE METHODS):\n");
  printf("\n");
  printf("<C> move cursor to the current group center\n");
  printf("<I> move cursor to intersection of line AB and plane CDE\n");
  printf("<Shift>+<I> intersection of triangles of marked and curent groups\n");
  printf("<N> find cursor_step*(normal to ABC)\n");
  printf("<S> scale current group by  |AB|/|CD| (where fixed_point=cursor)\n");
  printf("<Ctrl>+<S> scale current group by  |AB|/|CD| in direction EF\n");
  printf("<T> three point transformation ABC to DEF\n");
  printf("<F> find folding point: axes AB, AC, rotated lines AD, AE, direction F\n");
  printf("<Ctrl>+<F> folding marked and current group around AB and AC in direction F so that AD meets AE\n");


}




void callback_key_F3(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  char buffer[10];
  KeySym keysym;
  XKeyEvent ev;


  ev.state=evptr->state;
  evptr->state=0;
  XLookupString(evptr, buffer, 9, &keysym, NULL);
  evptr->state=ev.state;


  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F3;
      break;

    default:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  if('0'<= buffer[0] && buffer[0]<= '9')
	    point_store(buffer[0]-'0', cursor);
	  else
	  if('a'<= buffer[0] && buffer[0]<= 'a'+POINT_MAX-10)
	    point_store(buffer[0]-'a'+10, cursor);

	  break;
	case ControlMask:
	  if('0'<= buffer[0] && buffer[0]<= '9')
	    cursor_set(point[buffer[0]-'0'][0],
		       point[buffer[0]-'0'][1], 
		       point[buffer[0]-'0'][2]
		       );
	  else
	  if('a'<= buffer[0] && buffer[0]<= 'a'+POINT_MAX-10)
	    cursor_set(point[buffer[0]-'a'+10][0],
		       point[buffer[0]-'a'+10][1], 
		       point[buffer[0]-'a'+10][2]
		       );
	  break;
	  
	}
      redraw();
      break;
    }



  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F3_help()
{
  printf("F3 mode (STORED POINTS):\n");
  point_print();
  printf("\n");
  printf("<*> stroes cursor position as the point '*'\n");
  printf("<Ctrl>+<*> moves the cursor to the point '*'\n");
}




void callback_key_F2(XKeyEvent* evptr)
{
  enum Keymode next_mode;

  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F2;
      break;

    case XK_a:
      {
	float new_clip;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    new_clip=screen.clip_min/2;
	    break;
	  case ShiftMask:
	    new_clip=screen.clip_min*2;
	    break;
	  case ControlMask:
	    new_clip=screen.clip_min-1;
	    break;
	  case ControlMask|ShiftMask:
	    new_clip=screen.clip_min+1;
	    break;

	  }
	if( new_clip<screen.clip_max/3000 || new_clip>screen.clip_max)
	  {
	  new_clip=screen.clip_min;
	  printf("RESTRICTION: clip_max/3000<clip_min<clip_max\n");
	  }
	screen_set_clipping(&screen, new_clip, screen.clip_max);
	redraw();
	next_mode=key_F2;
      }
      break;

    case XK_b:
      {
	float new_clip;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    new_clip=screen.clip_max/2;
	    break;
	  case ShiftMask:
	    new_clip=screen.clip_max*2;
	    break;
	  case ControlMask:
	    new_clip=screen.clip_max-1;
	    break;
	  case ControlMask|ShiftMask:
	    new_clip=screen.clip_max+1;
	    break;

	  }
	if( screen.clip_min<new_clip/3000 || screen.clip_min>new_clip)
	  {
	  new_clip=screen.clip_max;
	  printf("RESTRICTION: clip_max/3000<clip_min<clip_max\n");
	  }
	screen_set_clipping(&screen, screen.clip_min, new_clip);
	redraw();
	next_mode=key_F2;
      }
      break;



    case XK_z:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  screen.distance=floorf(0.8*screen.distance);
	  if(screen.distance<=0) screen.distance=1;
	  break;
	case ShiftMask:
	  screen.distance=floorf(1.25*screen.distance+1);
	  break;
	case ControlMask:
	  screen.distance-=1;
	  if(screen.distance<1) screen.distance=1;
	  break;
	case ControlMask|ShiftMask:
	  screen.distance+=1;
	  break;
	}
      screen_set_distance(&screen, screen.distance);
      redraw();
      next_mode=key_F2;
      break;

    case XK_m:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
          move_step/=2;
          if(move_step < 0.0001) move_step*=2; 
	  break;
	case ShiftMask:
          move_step*=2;
	  break;
	}
      printf("move step is: %f\n", move_step); 
      next_mode=key_F2;
      break;

    case XK_s:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
           /*  scale/=2; */
	  transformation_scale(0.5, &transformation, cursor);
          if(scale < 1/512) scale=1/512; 
	  break;
	case ShiftMask:
	   /*           scale*=2; */
	  transformation_scale(2, &transformation, cursor);
	  if(scale>512) scale=512;
	  break;
	}
      printf("view scale is: %e\n", scale); 
      redraw();
      next_mode=key_F2;
      break;


    case XK_c:
      {
	float c=cursor_step;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    c/=2;
	    break;
	  case ShiftMask:
	    c*=2;
	    break;
	  }
      cursor_set_step(c); 
      }
      redraw();
      next_mode=key_F2;
      break;
    }



  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F2_help()
{
  printf("F2 mode:\n");
  printf(" <Shift>+<Z>/<Z>  increase/decrease screen distance (zoom) [%f]\n",
	 screen.distance);
  printf(" <Ctrl>+(<Shift>+<Z>/<Z>)  precise zoom  (screen distance) [%f]\n",
	 screen.distance);

  printf(" [<Ctrl>]+(<Shift>+<A>/<A>) [precise] clip_min [%f]\n",
	 screen.clip_min);
  printf(" [<Ctrl>]+(<Shift>+<B>/<B>) [precise] clip_max [%f]\n",
	 screen.clip_max);

  printf(" <Shift>+<M>/<M>  double/halve move step [%f]\n",
	 move_step);
  printf(" <Shift>+<C>/<C>  double/halve cursor step [%f]\n",
	 cursor_step);
  printf(" <Shift>+<S>/<S>  scale up/down the view [%f]\n",
	   scale);
}


void callback_key_F1(XKeyEvent* evptr)
{
  enum Keymode next_mode;

  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F1;
      break;

    case XK_Up:
      backup();
      group_rot90(group_current, 0, 1, 0);
      redraw();
      break;
    case XK_Right:
      backup();
      group_rot90(group_current, 1, 0, 0);
      redraw();
      break;
    case XK_Home:
      backup();
      group_rot90(group_current, 0, 0, 1);
      redraw();
      break;

    case  XK_equal:
      group_statistics(group_current); 
      break;


    case XK_space :
      group_menu();
      XRaiseWindow(display, window);
      XSetInputFocus(display, window, RevertToParent, CurrentTime);
      break;

    case XK_e : 
      backup();
      group_extract_clipped();
      redraw();
      break;

    case XK_m :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  group_mark(group_current);
	  redraw();
	  break;
	case Mod1Mask:
	  group_mark(-1);
	  redraw();
	  break;
	case ControlMask:
	  backup();
          group_regroup(group_marker, group_current);
	  redraw();
	  break;
	}
      break;

    case XK_z :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
          group_remove_empty();
	  redraw();
	  break;
	}
      break;

    case XK_f :
      {
	int found;
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    found=group_find(cursor);
	    if(found!=-1)
	      {
		group_change_current(found, &group_current, group, vertex_used);
		redraw();
	      }
	    else
	      printf("CURSOR IS NOT INCIDENT TO ANY GROUP\n");
	    break;
	  }
      }
      break;

    case XK_v :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  if(context==context_et0)
	    {
	      float mv[3];
	      vector_sub(cursor, et0_marker, mv);
	      backup();
	      group_move(group_current, mv, 
			 group, vertex, vertex_used);
	      
	    }
	  redraw();
	  break;
	}
      break;

    case XK_x :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask:
	  backup();
          group_delete_edges(group_current);
	  redraw();
	  break;
	case ShiftMask:
	  backup();
          group_delete_triangles(group_current);
	  redraw();
	  break;
	case Mod1Mask|ShiftMask:
	  backup();
          group_delete_triangles(group_current);
          group_delete_edges(group_current);
	  redraw();
	}
      break;

    case XK_p :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask:
	  backup();
          group_paint_edges(group_current, current_color);
	  redraw();
	  break;
	case ShiftMask:
	  backup();
          group_paint_triangles(group_current, current_color);
	  redraw();
	  break;
	}
      break;

    case XK_c :
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  backup();
          group_copy(False, False);
	  redraw();
	  break;
	case Mod1Mask:
	  backup();
          group_copy(True, False);
	  redraw();
	  break;
	case ShiftMask:
	  backup();
          group_copy(False, True);
	  redraw();
	  break;
	case Mod1Mask|ShiftMask:
	  backup();
          group_copy(True, True);
	  redraw();
	  break;
	}
      break;
    }
  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}


void key_F1_help()
{
  printf("F1 mode:\n");
  printf("  <M> Mark current group [now marked: %d]\n", group_marker);
  printf("  <Ctrl>+<M> Move vertices from marked group (%d) to current group (%d)\n", 
	 group_marker, group_current
	 );
  printf("  <Alt>+<M> Set group marker to -1 (no group)\n");
  printf("  <E> extract visible vertices to a new group\n");
  printf("  <F> change to a group incident to the cursor\n");
  printf("  <C> copy to new group\n");  
  printf("  <Alt>+<C> copy and glue with edges\n");  
  printf("  <Shift>+<C> copy and glue with triangles\n");  
  printf("  <Alt>+<Shift>+<C> copy and glue with edges and triangles\n");  
  printf("  <Z> remove empty group numbers\n");
  printf("  <Alt>+<X> delete edges incident to current group\n");
  printf("  <Shift>+<X> delete triangles incident to current group\n");
  printf("  <Shift>+<Alt>+<X> delete edges and triangles incident to current group\n");
  printf("  <Alt>+<P> paint edges of current group with current color\n");
  printf("  <Shift>+<P> paint triangles of current group with current color\n");
  printf(" <Up>/<Right>/<Home> rotate current group 90 degrees around the axis\n");
  if(context==context_et0) 
    printf("  <V> move current group by vector (et0_marker, cursor)\n");
  printf(" <Space> group menu in the text terminal\n");  
  printf(" <=> current group statistics\n");  
}




void callback_key_default(XKeyEvent* evptr)
{
  long event_mask=KeyPressMask;
  while(XCheckWindowEvent(display,window,event_mask , (XEvent*) evptr));


  switch(XLookupKeysym(evptr,0))
    {
    case XK_F1:
      keymode=key_F1;
      key_F1_help();
      break;

    case XK_F2:
      keymode=key_F2;
      key_F2_help();
      break;

    case XK_F3:
      keymode=key_F3;
      key_F3_help();
      break;

    case XK_F4:
      keymode=key_F4;
      key_F4_help();
      break;

    case XK_F5:
      keymode=key_F5;
      key_F5_help();
      break;

    case XK_F7:
      keymode=key_F7;
      key_F7_help();
      break;

    case XK_Up : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(-rotate_step, 1,0,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,1,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,1,0); redraw(); break;
	case ShiftMask:
          transformation_move(0,move_step,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,1,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Down : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(rotate_step, 1,0,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,-1,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,-1,0); redraw(); break;
	case ShiftMask:
          transformation_move(0,-move_step,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,-1,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Left :  /*  printf("XK_Left  evptr->state=%d\n",evptr->state); */
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(-rotate_step, 0,1,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(-1,0,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(-1,0,0); redraw(); break;
	case ShiftMask:
          transformation_move(-move_step,0,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(-1,0,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Right : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(rotate_step, 0,1,0, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(1,0,0); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(1,0,0); redraw(); break;
	case ShiftMask:
          transformation_move(move_step,0,0, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(1,0,0, cursor, &clipping_xyz); redraw(); break;
	}
      break;


    case XK_Home : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(-rotate_step, 0,0,1, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,0,-1); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,0,-1); redraw(); break;
	case ShiftMask:
          transformation_move(0,0,-move_step, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,0,-1, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_End : 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_rotate(rotate_step, 0,0,1, &transformation, cursor); redraw(); break;
	case ControlMask:
          cursor_move(0,0,1); redraw(); break;
	case Mod1Mask:
	  backup();
          group_cursor_move(0,0,1); redraw(); break;
	case ShiftMask:
          transformation_move(0,0,move_step, &transformation); redraw(); break;
	case ShiftMask|ControlMask:
	  clip_xyz(0,0,1, cursor, &clipping_xyz); redraw(); break;
	}
      break;

    case XK_Delete:
      break;

    case XK_BackSpace:
      switch(context)
	{
	case context_et0:
	  backup();
	  edge_vector_delete(et0_marker, cursor);
	  context_switch(context_default); redraw(); break;
	case context_t1:
	  backup();
	  triangle_vector_delete(et0_marker, t1_marker, cursor);
	  context_switch(context_default); redraw(); break;
	case context_default:
	  break;
	}
      break;

    case XK_space:
      break; 

    case XK_x: 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask|ControlMask:
	  backup();
	  edge_delete_incident(cursor);
	  redraw();
	  break;
	case ShiftMask|Mod1Mask|ControlMask:
	  backup();
	  triangle_delete_incident(cursor);
	  redraw();
	  break;
	}
      break;

    case XK_p: 
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask|ControlMask:
	  backup();
	  edge_paint_incident(cursor, current_color);
	  redraw();
	  break;
	case ShiftMask|Mod1Mask|ControlMask:
	  backup();
	  triangle_paint_incident(cursor, current_color);
	  redraw();
	  break;
	}
      break;

    case XK_Tab:
      break; 

    case XK_Escape : 
      context_switch(context_default); 
      redraw();
      break;

    case XK_e:
      switch(context)
	{
	case context_default:
          vectorcpy(et0_marker, cursor); context_switch(context_et0);  break;
	case context_et0:
	  backup();
	  edge_set_color( edge_vector_insert(et0_marker, cursor), current_color);
          context_switch(context_default);  
	  break;
	case context_t1:
	  break;
	}
      redraw(); break;

    case XK_t:
      switch(context)
	{
	case context_default:
          vectorcpy(et0_marker, cursor); 
	  context_switch(context_et0);  
	  break;
	case context_et0:
	  if(!vector_eq(et0_marker, cursor)) 
	    {
	      vectorcpy(t1_marker, cursor); 
	      context_switch(context_t1);
	    }  
          break;
	case context_t1:
	  backup();
	  triangle_set_color(triangle_vector_insert(et0_marker,t1_marker,cursor),current_color);
          context_switch(context_default);  break;
	  
	}
      redraw(); 
      break;

    case XK_Return:
      switch(context)
	{
	case context_default:
          vectorcpy(et0_marker, cursor); context_switch(context_et0);  break;
	case context_et0:
	  backup();
	  edge_set_color( edge_vector_insert(et0_marker, cursor), current_color);
	  vectorcpy(et0_marker, cursor); 
          context_switch(context_et0);  
	  break;
	case context_t1:
	  backup();
	  triangle_set_color(triangle_vector_insert(et0_marker,t1_marker,cursor),current_color);
	  vectorcpy(et0_marker, cursor);
          context_switch(context_et0);  
	  break;
	  
	}
      redraw(); break;
     
    case XK_r:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  transformation_init_orientation(&transformation);
          vectorcpy(transformation.mov,cursor);
          vector_scale(-1*scale,transformation.mov);
          transformation.mov[2]-=screen.distance+1;
          redraw(); break;
	case ControlMask:
          cursor_set(0,0,0); redraw(); break;
	case ControlMask|ShiftMask:
	  clip_reset(&clipping_xyz); redraw(); break;
	}
      break;

    case XK_f:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  cursor_find_vertex(); redraw(); break;
	case ControlMask:
	  cursor_to_screen(); redraw(); break;
	}
      break;

    case XK_b:
      background= (background+1)% COLOR_MAX; redraw(); break;
    case XK_c:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  set_current_color( (current_color+1)%COLOR_MAX );
	  printf("CURRENT COLOR: %d\n", current_color); 
	  redraw(); 
	  break;
	case ShiftMask|ControlMask:
	  clip_cube(&clipping_xyz);
	  redraw(); 
	  break;
	}
      break;
    case XK_n:
      {
	int max;
	max=group_max(group, vertex_used);
	switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	  {
	  case 0:
	    group_change_current((group_current+1)%(max+2),
				 &group_current, group, vertex_used);
	    redraw();
	    break;
	  case ShiftMask:
	    group_change_current((group_current+max+1)%(max+2),
				 &group_current, group, vertex_used);
	    redraw(); 
	    break;
	  }
      }
      break;

    case XK_1:
      group_restricted_switch();
      redraw(); 
      break;


    case XK_Page_Up:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  polygon_n_increase(1);
	  break;
	case ShiftMask:
	  polygon_n_increase(10);
	  break;
	}
      break; 

    case XK_Page_Down:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  polygon_n_increase(-1);
	  break;
	case ShiftMask:
	  polygon_n_increase(-10);
	  break;
	}
      break; 


    case XK_o:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case Mod1Mask:
	  backup();
	  polygon_add(True, False);
	  break;
	case ShiftMask:
	  backup();
	  polygon_add(False, True);
	  break;
	case Mod1Mask|ShiftMask:
	  backup();
	  polygon_add(True, True);
	  break;


	}
      redraw(); 
      break;




    case XK_l:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  set_light(light); redraw(); break;
	case ShiftMask:
	  switch_second_light(); redraw(); break;
	case ControlMask:
	  if(!was_modified || are_you_sure())
	    {	    
	      printf("LOADING:\n");
	      get_file_name(file_name);
	      load( file_name, 
		    vertex, vertex_used,
		    &edge_top, edge, 
		    &triangle_top, triangle, triangle_normal,
		    & transformation,
		    light,
		    &background,
		    &screen,
		    cursor,
		    &cursor_step,
		    &current_color,
		    group);
	      backup();
	      was_modified=False;
	    }
	  redraw(); break;
	case ControlMask|ShiftMask:
	  backup();
	  printf("MERGING:\n");
	  get_file_name(merge_file_name);
	  merge( merge_file_name, 
		 vertex, vertex_used,
		 &edge_top, edge,
		 &triangle_top, triangle, triangle_normal,
		 & transformation,
		 light,
		 &background,
		 &screen,
		 cursor,
		 &cursor_step,
		 &current_color,
		 group);
	  redraw(); break;
	};
      break;
          

    case XK_s:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case ControlMask:
	  printf("SAVING:\n");
          get_file_name(file_name);
	  save( file_name, 
	        vertex, vertex_used,
	        edge_top, edge, 
	        triangle_top, triangle, triangle_normal,
	        & transformation,
	        light,
		background,
	        &screen,
	        cursor, 
		cursor_step,
		current_color,
		group);
	  was_modified=False;
	  backup_was_modified=True;
	  redraw(); 
	  break;
	case ControlMask|ShiftMask:
	  printf("EXPORTING:\n");
          (*export_last_ptr)();
	  redraw(); break;
	};
      break;

    case XK_g:
      {
	int v=-1;
	if(context== context_default)
	  v=vertex_used_find(cursor);
	else 
	  {
	    if(context==context_et0)
	      v=vertex_used_find(et0_marker);
	  }
	if(v==-1)
	  printf("Either bad context or the cursor/et0-marker is not at a vertex\n");
	else
	  {
	    graph_create(edge_top, edge, triangle_top, triangle);
	    graph_bfs(v);
	    graph_menu();
	  }
      }
      break;

    case XK_v:
      if(context!=context_et0)
	printf("context should be: et0\n");
      else
	{
	    int v;
	    v=vertex_used_find(et0_marker);
	    if((evptr->state!=ControlMask) && v==-1)
	      printf("The first endpoint of cursor line is not at a vertex\n");
	    else 
	      {
		float mv[3];
		int first_free;
		vector_sub(cursor, et0_marker, mv);
		first_free=vertex_compress(vertex,
					   vertex_used,
					   edge_top, edge,
					   triangle_top, triangle, triangle_normal,
					   group);
		printf("first_free= %d\n", first_free);
		graph_create(edge_top, edge, triangle_top, triangle);
		graph_bfs(v);

		switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
		{
		case 0:
		  {
		    backup();
		    graph_move_marked(mv);
		    context_switch(context_default);
		    redraw();
		  } 
		  break;
		case ShiftMask:
		  {
		    backup();
		    graph_copy_move_marked(mv, first_free);
		    context_switch(context_default);
		    redraw();
		  } 
		  break;
		case ControlMask:
		  {
		    backup();
		    group_move(group_current, mv, 
			       group, vertex, vertex_used);
		     /*  context_switch(context_default); */
		    redraw();
		  } 
		  break;
		}
	      }
	}
      break;



    case XK_h:
      help_keys(); break;

    case  XK_equal:
      stats(); break;

    case XK_m:
      main_menu(); break;
    case XK_u:
      undo(); redraw(); break;
    case XK_q:
      if(!was_modified || are_you_sure()) exit(0); break;
    }
}


