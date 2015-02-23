#include "./et-edit.h"
void menu_F5()
{
char * s; /* pointer to menu selector */


printf("\n REDUCTIONS (F5) MENU:\n");
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

