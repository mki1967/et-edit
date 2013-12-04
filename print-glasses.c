int dpi=300;
double cmpi=2.54;
double eye_min_dist=3; // cm
double width=4; //cm
double height=2.54;// cm
int lines, dots_in_line, bytes_in_line;
double  step /* 1 dot in cm */;

print_left_dots()
{
  int i;
  double position;
  unsigned char mask=128, out_byte=0;
  position=0;
  for(i=0; i<dots_in_line; i++)
    {
      if(position<width)
	out_byte|=mask;
      mask/=2;
      position+=step;
      if(mask==0)
	{
	printf("%c", out_byte);
	out_byte=0;
	mask=128;
	}
    } 

  if(mask!=128) printf("%c", out_byte);
}

print_right_dots()
{
  int i;
  double position;
  unsigned char mask=128, out_byte=0;
  position=0;
  for(i=0; i<dots_in_line; i++)
    {
      if(position> width+eye_min_dist && position<2*width+eye_min_dist)
	out_byte|=mask;
      mask/=2;
      position+=step;
      if(mask==0)
	{
	printf("%c", out_byte);
	out_byte=0;
	mask=128;
	}
    } 

  if(mask!=128) printf("%c", out_byte);
}


pcl_init()
{
printf("%c*t300R",27); // RESOLUTION 300 DPI
printf("%c*b0M",27); // UNENCODED
printf("%c*r-3U",27); // CMY Color
printf("%c*r0A",27); // START RASTER
}


pcl_finish()
{
printf("%c*rC",27); // END RASTER
printf("\f"); // END OF PAGE 
}

pcl_raster()
{
  int i;
  for(i=0; i<lines; i++)
    {
      printf("%c*b%dV",27, bytes_in_line);
      print_left_dots();
      printf("%c*b%dV",27, bytes_in_line);
      print_right_dots();
      printf("%c*b%dW",27, bytes_in_line);
      print_right_dots();

    }
}


main()
{

  step= cmpi*1.0/dpi;

  lines = (int) (dpi*height/cmpi);
  dots_in_line=(int) (2*width+eye_min_dist)*dpi/cmpi;
  bytes_in_line=(dots_in_line+7)/8;

/*   printf("%lf\n", step); */
/*   printf("%d\n",lines); */
/*   printf("%d\n",dots_in_line); */
/*   printf("%d\n", bytes_in_line); */

  pcl_init();
  pcl_raster();
  pcl_finish();

}

