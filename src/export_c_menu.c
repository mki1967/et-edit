#include "./et-edit.h"
void export_c_menu()
{
char * s; /* pointer to menu selector */

static char  name_prefix[501]="data";

do{
    printf("\n EXPORT TO C MENU:\n");
    printf("\n");
    printf("  name  change name prefix [curent name prefix is '%s']\n", name_prefix);
    printf("  ve    export vertex table and edge table\n");
    printf("  0    Return\n");

    s=get_menu_selector();

    if(strcmp(s,"name")==0){ 
      printf("Inputnew new name prefix: ");
      scanf("%500s", name_prefix);
    }else
    if(strcmp(s,"ve")==0){ 
      export_c_ve(name_prefix); 
    }
  }while(strcmp(s,"0")!=0);
}

