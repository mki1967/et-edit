/*
 CONSTRUCTIVE POINTS (F3) MENU
*/

char font_idx_to_symbol(int symbol_idx)
/* we assume that symbol_idx is a valid index */
{
  if(0<= symbol_idx && symbol_idx<=9)
    return symbol_idx+'0';
  else
    return symbol_idx+'A'-10;
}

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

  if(0<= symbol_idx && symbol_idx<=POINT_MAX-1)
    printf("point symbol is: %c\n", font_idx_to_symbol(symbol_idx));
  else
    printf("INVALID INPUT SYMBOL: %c !!!\n", c);
 
  return symbol_idx;
}

void menu_F3()
{
  int point_idx;
  char * s;
  printf("CONSTRUCTIVE POINTS (F3) MENU:\n");
  /* point_print(); */
  printf("\n");
  printf(" c2p move cursor to constructive point ...\n");
  printf(" sp set constructive point at cursor position [%f,%f,%f] ...\n",
	 cursor[0], cursor[1], cursor[2]);
  printf(" hp hide constructive point ...\n");
  printf(" ha hide all constructive points\n");

  /* TEST */
  s=get_menu_selector();

  if(strcmp(s,"c2p")==0){
    point_idx=get_point_symbol();
    if(0<=point_idx && point_idx<POINT_MAX) {
      point_visible[point_idx]=True;
      cursor_set(
		 point[point_idx][0],
		 point[point_idx][1],
		 point[point_idx][2]
		 );
      printf("Cursor moved to [%f,%f,%f].\n",cursor[0], cursor[1], cursor[2]);
      printf("If you do not see the cursor in graphical window, then press the 'R' key.\n"); 
    }
    else {
      printf("NOT VALID POINT -- no action performed.\n");
    }
    get_anything();
  }else if(strcmp(s,"sp")==0){ /* ... */
    point_idx=get_point_symbol();
    if(0<=point_idx && point_idx<POINT_MAX) {
      point_visible[point_idx]=True;
      point_store(point_idx, cursor);
      printf("Point '%c' set at [%f,%f,%f].\n",
	     font_idx_to_symbol(point_idx),
	     cursor[0], cursor[1], cursor[2]
	     );
    }
    else {
      printf("NOT VALID POINT -- no action performed.\n");
    }
    get_anything();
  }else if(strcmp(s,"hp")==0){ /* ... */
    point_idx=get_point_symbol();
    if(0<=point_idx && point_idx<POINT_MAX) {
      point_visible[point_idx]=False;
      printf("Point '%c' is hidden.\n", font_idx_to_symbol(point_idx));
    }
    else {
      printf("NOT VALID POINT -- no action performed.\n");
    }
    /* get_anything(); */
  }else if(strcmp(s,"ha")==0){ /* ... */
    int i;
    for(i=0; i<POINT_MAX; i++)
      point_visible[i]=False;
  }

  /*
  point_idx=get_point_symbol();
  printf("TEST point_idx=%d\n", point_idx);
  */
}



