#include "include/et-edit.h"
void menu_F2()
{
char * s; /* pointer to menu selector */


printf("\n OBSERVER SETTINGS (F2) MENU:\n");
printf("\n");
printf("  eps   input new reduction epsilon [%e]\n", reduction_epsilon);
printf("  r     reduce vertices in current group\n");

s=get_menu_selector();

if(strcmp(s,"eps")==0){ 
      printf("Inputnew reduction epsilon (>= 0):");
      reduction_epsilon=get_float();
      if(reduction_epsilon<0) reduction_epsilon=0.0;
      printf("\n new value of reduction epsilon is: %e\n", reduction_epsilon);
      get_anything();
}else
if(strcmp(s,"r")==0){ 
      reduce_group_vertices(group_current);
      get_anything();
}

}




/*

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
           // scale/=2; 
	  transformation_scale(0.5, &transformation, cursor);
          if(scale < 1/512) scale=1/512; 
	  break;
	case ShiftMask:
	   //       scale*=2; 
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


*/
