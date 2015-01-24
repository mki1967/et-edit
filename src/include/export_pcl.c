
void pcl_init(FILE* f)
{
fprintf(f,"%c*t300R",27);  /*  RESOLUTION 300 DPI */
fprintf(f,"%c*b0M",27);  /*  UNENCODED */
fprintf(f,"%c*r-3U",27);  /*  CMY Color */
fprintf(f,"%c*r0A",27);  /*  START RASTER */
}


void pcl_finish(FILE* f)
{
fprintf(f,"%c*rC",27);  /*  END RASTER */
fprintf(f,"\f");  /*  END OF PAGE  */
}


void pcl_line1_bytes(FILE* f,int length, double red, double green, double blue)
{
  int shade_levels=10000;
  int i;
  double max_shade;
  unsigned char mask=128, out_byte=0; 

  max_shade= 
    red*(stereo_left_color[0]+stereo_right_color[0])+
    green*(stereo_left_color[1]+stereo_right_color[1])+
    blue*(stereo_left_color[2]+stereo_right_color[2]);

  for(i=0; i<length;i++)
    {
      if(((double)(labs(random())%shade_levels))/shade_levels<
	 pcl_darkness*
	 (
	  red*pcl_red_line1[i]+
          green*pcl_green_line1[i]+
          blue*pcl_green_line1[i]
	  )/max_shade)
	out_byte|=mask;
      mask/=2;

      if(i<length-1)
	if(
	   ((double)(labs(random())%shade_levels))/shade_levels<
	   pcl_darkness*
	   0.5*(
		red*(pcl_red_line1[i]+pcl_red_line1[i+1])+
		green*(pcl_green_line1[i]+pcl_green_line1[i+1])+
		blue*(pcl_blue_line1[i]+pcl_blue_line1[i+1])
		)/max_shade
	   )
	  out_byte|=mask;
      mask/=2;

      if(mask==0)
	{
	  fprintf(f,"%c", out_byte);
          mask=128;
          out_byte=0;
	}
    }

  if(mask!=128) fprintf(f,"%c", out_byte);  /*  LAST BYTE NON-FULL */

}

void pcl_line12_bytes(FILE* f,int length, double red, double green, double blue)
{
  int shade_levels=10000;
  int i;
  double max_shade;
  unsigned char mask=128, out_byte=0; 

  max_shade= 
    red*(stereo_left_color[0]+stereo_right_color[0])+
    green*(stereo_left_color[1]+stereo_right_color[1])+
    blue*(stereo_left_color[2]+stereo_right_color[2]);

  for(i=0; i<length;i++)
    {
      if(((double)(labs(random())%shade_levels))/shade_levels<
	 pcl_darkness*
	 0.5*(
	      red*(pcl_red_line1[i]+pcl_red_line2[i])+
	      green*(pcl_green_line1[i]+pcl_green_line2[i])+
	      blue*(pcl_green_line1[i]+pcl_green_line2[i])
	      )
	 /max_shade)
	out_byte|=mask;
      mask/=2;

      if(i<length-1)
	if(
	   ((double)(labs(random())%shade_levels))/shade_levels<
	   pcl_darkness*
	   0.25*(
		red*(pcl_red_line1[i]+pcl_red_line1[i+1]+
		     pcl_red_line2[i]+pcl_red_line2[i+1])+
		green*(pcl_green_line1[i]+pcl_green_line1[i+1]+
		       pcl_green_line2[i]+pcl_green_line2[i+1])+
		blue*(pcl_blue_line1[i]+pcl_blue_line1[i+1]+
		      pcl_blue_line2[i]+pcl_blue_line2[i+1])
		)/max_shade
	   )
	  out_byte|=mask;
      mask/=2;

      if(mask==0)
	{
	  fprintf(f,"%c", out_byte);
          mask=128;
          out_byte=0;
	}
      
    }

  if(mask!=128) fprintf(f,"%c", out_byte);  /*  LAST BYTE NON-FULL */

}


void pcl_print_two_lines(FILE *f)
{
  int bytes_in_line;


  bytes_in_line=((screen.width+3)/4);  /*  TWO PCL POINTS PER SCREEN PIXEL */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line1_bytes(f,screen.width, 1,0,0);  /*  RED->CYAN */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line1_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->MAGENTA */

  fprintf(f,"%c*b%dW",27, bytes_in_line);
  pcl_line1_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->YELLOW */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line12_bytes(f,screen.width, 1,0,0);  /*  RED->CYAN */

  fprintf(f,"%c*b%dV",27, bytes_in_line);
  pcl_line12_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->MAGENTA */

  fprintf(f,"%c*b%dW",27, bytes_in_line);
  pcl_line12_bytes(f,screen.width, 0,1,1);  /*  GREEN+BLUE->YELLOW */

}


void pcl_print_raster(FILE *f)
{
  int l;
  float *t;
  glReadBuffer(GL_FRONT);

  l=screen.height-1;
  glReadPixels(0, l, screen.width, 1,
	       GL_RED, GL_FLOAT, pcl_red_line2);
  glReadPixels(0, l, screen.width, 1,
	       GL_GREEN, GL_FLOAT, pcl_green_line2);
  glReadPixels(0, l, screen.width, 1,
	       GL_BLUE, GL_FLOAT, pcl_blue_line2);

  do
    {
      t=pcl_red_line1;
      pcl_red_line1=pcl_red_line2;
      pcl_red_line2=t;

      t=pcl_green_line1;
      pcl_green_line1=pcl_green_line2;
      pcl_green_line2=t;

      t=pcl_blue_line1;
      pcl_blue_line1=pcl_blue_line2;
      pcl_blue_line2=t;


      l--;
      glReadPixels(0,l, screen.width, 1,
		   GL_RED, GL_FLOAT, pcl_red_line2);
      glReadPixels(0,l, screen.width, 1,
		   GL_GREEN, GL_FLOAT, pcl_green_line2);
      glReadPixels(0,l, screen.width, 1,
		   GL_BLUE, GL_FLOAT, pcl_blue_line2);
      pcl_print_two_lines(f);
    }
  while(l>0);

  glReadBuffer(GL_BACK);

}



void pcl_print()
{
  FILE *f;

  if(!stereo_mode)
    {
      printf("PCL printing only in stereo mode.\n");
      return;
    }

  if(screen.width<4 || screen.height<4)
    {
      printf("The window is too small to be printed\n");
      return;
    }

  f=fopen(pcl_file_name,"w");
  if(f==NULL)
    {
      printf("Can not open file: %s\n",pcl_file_name);
      return;
    }

  pcl_red_line1= (float*) malloc(screen.width*sizeof(float)); 
  pcl_green_line1= (float*) malloc(screen.width*sizeof(float)); 
  pcl_blue_line1= (float*) malloc(screen.width*sizeof(float)); 

  pcl_red_line2= (float*) malloc(screen.width*sizeof(float)); 
  pcl_green_line2= (float*) malloc(screen.width*sizeof(float)); 
  pcl_blue_line2= (float*) malloc(screen.width*sizeof(float)); 

  if(pcl_red_line1==NULL ||
     pcl_green_line1==NULL||
     pcl_blue_line1==NULL||
     pcl_red_line2==NULL ||
     pcl_green_line2==NULL||
     pcl_blue_line2==NULL)
    {
      printf("Out of memory\n");
      fclose(f);
      free(pcl_red_line1);
      pcl_red_line1=NULL;
      free(pcl_green_line1);
      pcl_green_line1=NULL;
      free(pcl_blue_line1);
      pcl_blue_line1=NULL;
      free(pcl_red_line2);
      pcl_red_line2=NULL;
      free(pcl_green_line2);
      pcl_green_line2=NULL;
      free(pcl_blue_line2);
      pcl_blue_line2=NULL;
      return;
    }

  pcl_init(f);

   /*  EXPORT RASTER ... */
  pcl_print_raster(f);


  pcl_finish(f);


  fclose(f);
  free(pcl_red_line1);
  pcl_red_line1=NULL;
  free(pcl_green_line1);
  pcl_green_line1=NULL;
  free(pcl_blue_line1);
  pcl_blue_line1=NULL;
  free(pcl_red_line2);
  pcl_red_line2=NULL;
  free(pcl_green_line2);
  pcl_green_line2=NULL;
  free(pcl_blue_line2);
  pcl_blue_line2=NULL;
  printf("Printed to PCL file: %s\n", pcl_file_name);

}


