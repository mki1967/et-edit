#include "include/et-edit.h"

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


