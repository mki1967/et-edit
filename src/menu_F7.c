void menu_F7()
{
  char * s; /* pointer to menu selector */


  printf("\n STEREO (F7) MENU:\n");
  printf("\n");
  printf("  s   red-blue stereoscopic mode\n");
  printf("  m   color monoscopic mode\n");
  printf("  x   swap left and right colors in stereoscopic mode\n");
  printf("  e   set the distance beween the eyes (current value is: %f)\n",
	 stereo_eye_distance);
  printf("  c   set the size of the clipping cube for stereo mode (current value is: %f)\n",
	 clipping_cube_size);
  printf("\n  lr   set the red componnent of the left color (current value is: %f)\n",
	 stereo_left_color[0]);
  printf("  lg   set the green componnent of the left color (current value is: %f)\n",
	 stereo_left_color[1]);
  printf("  lb   set the blue  componnent of the left color (current value is: %f)\n",
	 stereo_left_color[2]);
  printf("\n  rr   set the red componnent of the right color (current value is: %f)\n",
	 stereo_right_color[0]);
  printf("  rg   set the green componnent of the right color (current value is: %f)\n",
	 stereo_right_color[1]);
  printf("  rb   set the blue  componnent of the right color (current value is: %f)\n",
	 stereo_right_color[2]);

  s=get_menu_selector();

  if(strcmp(s,"s")==0){
    stereo_mode= True;
    printf("STEREO MODE\n");
    clip_cube(&clipping_xyz);
  }else if(strcmp(s,"m")==0){ 
    stereo_mode= False;
    printf("COLOR MODE\n");
  }else if(strcmp(s,"x")==0){ 
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
  }else if(strcmp(s,"e")==0){ 
    printf("Input new eye distance (between 0 and 20):");
    stereo_eye_distance=get_float();
    if(stereo_eye_distance<0) stereo_eye_distance=0.0;
    if(stereo_eye_distance>20) stereo_eye_distance=20.0;
    printf("\n new value of eye distance is: %e\n", stereo_eye_distance );
    /* get_anything(); */
  }else if(strcmp(s,"c")==0){ 
    printf("Input new clipping_cube_size (between 1 and 200):");
    clipping_cube_size =get_float();
    if(clipping_cube_size<1) clipping_cube_size=1.0;
    if(clipping_cube_size>200) clipping_cube_size=200.0;
    printf("\n new value of clipping_cube_size is: %e\n", clipping_cube_size );
    clip_cube(&clipping_xyz);
  }else if(strcmp(s,"lr")==0){ 
    printf("Input new stereo_left_color[0] (between 0 and 1 ):");
    stereo_left_color[0] =get_float();
    if(stereo_left_color[0]<0) stereo_left_color[0]=0;
    if(stereo_left_color[0]>1) stereo_left_color[0]=1;
    printf("\n new value of stereo_left_color[0] is: %e\n", stereo_left_color[0]);
  }else if(strcmp(s,"lg")==0){ 
    printf("Input new stereo_left_color[1] (between 0 and 1 ):");
    stereo_left_color[1] =get_float();
    if(stereo_left_color[1]<0) stereo_left_color[1]=0;
    if(stereo_left_color[1]>1) stereo_left_color[1]=1;
    printf("\n new value of stereo_left_color[1] is: %e\n", stereo_left_color[1]);
  }else if(strcmp(s,"lb")==0){ 
    printf("Input new stereo_left_color[2] (between 0 and 1 ):");
    stereo_left_color[2] =get_float();
    if(stereo_left_color[2]<0) stereo_left_color[2]=0;
    if(stereo_left_color[2]>1) stereo_left_color[2]=1;
    printf("\n new value of stereo_left_color[2] is: %e\n", stereo_left_color[2]);
  }else if(strcmp(s,"rr")==0){ 
    printf("Input new stereo_right_color[0] (between 0 and 1 ):");
    stereo_right_color[0] =get_float();
    if(stereo_right_color[0]<0) stereo_right_color[0]=0;
    if(stereo_right_color[0]>1) stereo_right_color[0]=1;
    printf("\n new value of stereo_right_color[0] is: %e\n", stereo_right_color[0]);
  }else if(strcmp(s,"rg")==0){ 
    printf("Input new stereo_right_color[1] (between 0 and 1 ):");
    stereo_right_color[1] =get_float();
    if(stereo_right_color[1]<0) stereo_right_color[1]=0;
    if(stereo_right_color[1]>1) stereo_right_color[1]=1;
    printf("\n new value of stereo_right_color[1] is: %e\n", stereo_right_color[1]);
  }else if(strcmp(s,"rb")==0){ 
    printf("Input new stereo_right_color[2] (between 0 and 1 ):");
    stereo_right_color[2] =get_float();
    if(stereo_right_color[2]<0) stereo_right_color[2]=0;
    if(stereo_right_color[2]>1) stereo_right_color[2]=1;
    printf("\n new value of stereo_right_color[2] is: %e\n", stereo_right_color[2]);
  }

}

