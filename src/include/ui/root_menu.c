void root_menu()
{
char * s; /* pointer to menu selector */
goto_terminal();

printf("\nROOT MENU:\n\n");
printf("m  Main Menu\n");
printf("g  Group (F1) menu\n");

s=get_menu_selector();

if(strcmp(s,"m")==0) main_menu();
else
if(strcmp(s,"g")==0) menu_F1();

printf("\nReturn to graphical window\n");

goto_window();
}

