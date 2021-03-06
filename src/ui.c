#include "./et-edit.h"
/*** TERMINAL INTERACTION UI ***/

char * get_menu_selector()
{
static char selector[20]; /* actually a global variable that can be only modified by this function */
printf("\nInput item selection: ");
scanf(" %19s%*[^\n]", selector);
return selector;
}


void get_file_name(char *file_name)
{
  char new_name[MAX_FILE_NAME+5];
  XRaiseWindow(display, terminal);
  XSetInputFocus(display, terminal, RevertToParent, CurrentTime);
  printf("Input file name (input '-' to leave the old name: '%s'):\n", file_name);
  scanf("%100s", new_name);
  if( strcmp("-",new_name)!=0 ) strcpy(file_name, new_name);
  printf("New file name is: '%s'\n", file_name);
  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToParent, CurrentTime);
}


float get_float()
{
  float x;
  scanf("%f", &x);
  return x;
}


int are_you_sure()
{
  char * s;
  goto_terminal();
  printf("The changes will be lost !!!\n");
  printf("Are you sure? (yes/no)\n");
  s=get_menu_selector();
  goto_window();
  return (strcmp(s,"yes")==0) ;
}


void get_anything()
{
  get_enter();
  /* older version: 
    printf("Input any non-empty text to continue ...\n");
    scanf("%*s%*[^\n]");
  */
}

void get_enter()
{
  int c;
  printf("Press ENTER ...(once or twice ;-)\n");
  c=getchar();
  while(c!= '\n') c=getchar(); /* read the 'old' ENTER */
  c=getchar();
  while(c!= '\n') c=getchar();
}

/* INCLUDE SUB-MENUS */

/*
#include "menu_F1.c"
#include "menu_F3.c"
#include "menu_F4.c"
#include "menu_F5.c"
#include "menu_F7.c"
#include "povray_menu.c"
#include "export_c_menu.c"
#include "export_menu.c"
#include "terminal_menus.c"
#include "root_menu.c"

*/
