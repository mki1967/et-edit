#include "./et-edit.h"

/*
  MENU FOR MAKING NEW TEXT SHAPED GROUP
*/

#define MAX_TEXT_LENGTH 100


void draw_char_shape(float position[3],  SHAPE_VE * shape)
{
  float v1[3], v2[3];
  int e_idx;
  for( e_idx=0; e_idx < shape->edge_top; e_idx++ ) {
    int e0, e1, new_e;
    e0=(shape->edge)[e_idx][0];
    e1=(shape->edge)[e_idx][1];

    vectorcpy(v1, (shape->vertex)[e0]);
    vector_scale( font_scale, v1);
    vector_add(v1, position, v1);
    
    vectorcpy(v2, (shape->vertex)[e1]);
    vector_scale( font_scale, v2);
    vector_add(v2, position, v2);
    new_e= edge_vector_insert(v1, v2);
    edge_set_color(new_e, current_color );
  }
}

void draw_char(float position[3], char c)
{
  int symbol_idx;

  if('0'<= c && c<= '9') 
    symbol_idx= c-'0';
  else if('A'<= c && c<= 'A'+POINT_MAX-10)
    symbol_idx= c-'A'+10;
  else 
    symbol_idx=-1;

  if(0<= symbol_idx && symbol_idx<=POINT_MAX-1)
    { 
      /*
      printf("%c", font_idx_to_symbol(symbol_idx));
      */
      draw_char_shape(position,  digit_letter_shape[symbol_idx]);
    }
  else
    printf("\n INVALID INPUT SYMBOL: %c !!!\n", c);



}


void draw_text(float position[3], char* text)
{
  int i;
  for( i=0; text[i]!=0; i++) {
    draw_char(position, text[i]);
    position[0] += font_scale * font_width;
  }
}


void menu_text()
{  
  char * s; /* pointer to selector */
  int g_new;
  char text[MAX_TEXT_LENGTH];
  char formatString[20];
  formatString[0]=(char) 0;
  g_new=group_max(group, vertex_used)+1;
  group_change_current(g_new, &group_current, group, vertex_used); 

  printf("Input text of length at most %d to be placed on XY plane in cursor position.\n"
	 "Use upper case letters and digits only and use '_' instead of the space:\n",
	 MAX_TEXT_LENGTH );

  scanf("%99s", text);

  /*
  printf("\n You have input the text: %s", text);
  */

  {
    float position[3];
    vectorcpy(position, cursor);
    draw_text(position, text);
  }
}

