#include "include/et-edit.h"

void export_menu()
{
  char * s;
  do
    {
      printf("\nEXPORT MENU:\n\n");
      printf("pov  Povray\n");
      printf("c    Export to C\n");
      printf("0    Return\n");
      
      s=get_menu_selector();

      if(strcmp(s,"pov")==0){
	  povray_menu();
      } else
      if(strcmp(s,"c")==0){
	  export_c_menu();
      }

    }
  while(strcmp(s,"0")!=0);
}

