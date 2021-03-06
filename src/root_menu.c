#include "./et-edit.h"
void root_menu()
{
char * s; /* pointer to menu selector */
goto_terminal();

printf("\nROOT MENU:\n\n");
printf(" m   Main Menu\n");
printf(" g   Group (F1) menu\n");
printf(" r   Reduction (F5) menu\n");
printf(" e   Export menu\n");
printf(" cp  Constructive points (F3) menu\n");
printf(" cm  Constructive methods (F4) menu\n");
printf(" sm  Stereo (F7) menu\n");
printf(" txt text input menu\n");


s=get_menu_selector();

if(strcmp(s,"m")==0) main_menu();
else
if(strcmp(s,"g")==0) menu_F1();
else
if(strcmp(s,"r")==0) menu_F5();
else
if(strcmp(s,"e")==0) export_menu();
else
if(strcmp(s,"cp")==0) menu_F3();
else
if(strcmp(s,"cm")==0) menu_F4();
else
if(strcmp(s,"sm")==0) menu_F7();
else
if(strcmp(s,"txt")==0) menu_text();

printf("\nContinue in graphical window\n");

goto_window();
}

