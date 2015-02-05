/*
void callback_key_F3(XKeyEvent* evptr)
{
  enum Keymode next_mode;
  char buffer[10];
  KeySym keysym;
  XKeyEvent ev;


  ev.state=evptr->state;
  evptr->state=0;
  XLookupString(evptr, buffer, 9, &keysym, NULL);
  evptr->state=ev.state;


  next_mode= key_default;

  switch(XLookupKeysym(evptr,0))
    {
    case  XK_Shift_L:
    case  XK_Shift_R:
    case XK_Control_L:
    case XK_Control_R:
    case XK_Alt_L:
    case XK_Alt_R:
      next_mode=key_F3;
      break;

    default:
      switch((evptr->state)&(ControlMask|Mod1Mask|ShiftMask))
	{
	case 0:
	  if('0'<= buffer[0] && buffer[0]<= '9')
	    point_store(buffer[0]-'0', cursor);
	  else
	  if('a'<= buffer[0] && buffer[0]<= 'a'+POINT_MAX-10)
	    point_store(buffer[0]-'a'+10, cursor);

	  break;
	case ControlMask:
	  if('0'<= buffer[0] && buffer[0]<= '9')
	    cursor_set(point[buffer[0]-'0'][0],
		       point[buffer[0]-'0'][1], 
		       point[buffer[0]-'0'][2]
		       );
	  else
	  if('a'<= buffer[0] && buffer[0]<= 'a'+POINT_MAX-10)
	    cursor_set(point[buffer[0]-'a'+10][0],
		       point[buffer[0]-'a'+10][1], 
		       point[buffer[0]-'a'+10][2]
		       );
	  break;
	  
	}
      redraw();
      break;
    }



  keymode=next_mode;
  if(keymode==key_default) printf("default key mode\n");

}

*/

int get_point_symbol()
/* gets point symbol from stdin */
{
char c;
int symbol_idx=-1; /* -1 denotes invalid symbol */

printf("\nInput point symbol (either '0'...'9' or 'A'...'Z' or 'a'...'z'): ");
scanf(" %c%*[^\n]", &c);
if('0'<= c && c<= '9') 
  symbol_idx= c-'0';
else
if('a'<= c && c<= 'a'+POINT_MAX-10)
  symbol_idx= c-'a'+10;
if('A'<= c && c<= 'A'+POINT_MAX-10)
  symbol_idx= c-'A'+10;

if(0<= symbol_idx && symbol_idx<=9)
  printf("point symbol is: %c\n", symbol_idx+'0');
else
if(10<= symbol_idx && symbol_idx<=POINT_MAX-1)
  printf("point symbol is: %c\n", symbol_idx+'A'-10);
else
  printf("INVALID INPUT SYMBOL: %c !!!\n", c);
 
return symbol_idx;
}

void menu_F3()
{
int point_idx;

  printf("CONSTRUCTIVE POINTS (F3) MENU:\n");
  /* point_print(); */
  printf("\n");
  printf("<*> stroes cursor position as the point '*'\n");
  printf("<Ctrl>+<*> moves the cursor to the point '*'\n");

/* TEST */

point_idx=get_point_symbol();
printf("TEST point_idx=%d\n", point_idx);

}



