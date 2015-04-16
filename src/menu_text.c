#include "./et-edit.h"

/*
  MENU FOR MAKING NEW TEXT SHAPED GROUP
*/

#define MAX_TEXT_LENGTH 100


void menu_text()
{  
  char * s; /* pointer to selector */
  int g_max;
  char text[MAX_TEXT_LENGTH];
  char formatString[20];
  formatString[0]=(char) 0;
  g_max=group_max(group, vertex_used);
  printf("Input text of length at most %d to be placed on XY plane in cursor position.\n"
	 "Use upper case letters and digits only and use '_' instead of the space:\n",
	 MAX_TEXT_LENGTH );

  scanf("%s", text);

  printf("\n You have input the text: %s", text);

  /* dokonczyc ... */
}
