#include "./et-edit.h"

void backup()
{
  int i;
  backup_was_modified=was_modified;
  was_modified=True;
  
  for(i=0;i<VERTEX_MAX; i++)
    {
      vectorcpy( backup_vertex[i], vertex[i]);
      backup_vertex_used[i]=vertex_used[i];
      backup_group[i]=group[i];
    }
  backup_group_current=group_current;
  backup_group_marker=group_marker;
  backup_group_restricted=group_restricted;

  backup_edge_top=edge_top;
  for(i=0;i<edge_top; i++)
    {  
      edgecpy(backup_edge[i], edge[i]);
    }
  
  backup_triangle_top=triangle_top;

  for(i=0;i<triangle_top; i++)
    {  
      trianglecpy(backup_triangle[i],triangle[i]);
    }

  
  for(i=0; i<16; i++)
    {
      backup_transformation.mov[i]=transformation.mov[i];
      backup_transformation.M[i]=transformation.M[i];
      backup_transformation.R[i]=transformation.R[i];
    }

  vectorcpy(backup_cursor, cursor);
  backup_cursor_step=cursor_step;
  
  for(i=0; i<4; i++)
   {
     backup_light[i]=light[i];
   }
  
  backup_two_lights=two_lights;
  
  backup_background= background;
  backup_current_color= current_color;

 
  backup_context=context;
  vectorcpy(backup_et0_marker,et0_marker);
  vectorcpy(backup_t1_marker, t1_marker); 
  backup_move_step=move_step;
}

void int_swap(int* a, int* b)
{
  int tmp;
  tmp=*a; *a=*b; *b=tmp;
}


void double_swap(double* a, double* b)
{
  double tmp;
  tmp=*a; *a=*b; *b=tmp;
}

void float_swap(float* a, float* b)
{
  float tmp;
  tmp=*a; *a=*b; *b=tmp;
}

void vector_swap(float a[3], float b[3])
{
  float_swap(&a[0],  &b[0]);
  float_swap(&a[1],  &b[1]);
  float_swap(&a[2],  &b[2]);
}


void edge_swap(int a[3], int b[3])
{
  int_swap(&a[0],  &b[0]);
  int_swap(&a[1],  &b[1]);
  int_swap(&a[2],  &b[2]);
}

void triangle_swap(int a[4], int b[4])
{
  int_swap(&a[0],  &b[0]);
  int_swap(&a[1],  &b[1]);
  int_swap(&a[2],  &b[2]);
  int_swap(&a[3],  &b[3]);
}





void undo()
{
  int i;

  int_swap(&backup_was_modified, &was_modified);

  
   /* // ....  */

  for(i=0;i<VERTEX_MAX; i++)
    {
      vector_swap( backup_vertex[i], vertex[i]);
      int_swap(&backup_vertex_used[i], &vertex_used[i]);
      int_swap(&backup_group[i], &group[i]);
    }

  int_swap(&backup_group_current, &group_current);
  group_change_current(group_current, &group_current, group, vertex_used );
  int_swap(&backup_group_marker, &group_marker);
  int_swap(&backup_group_restricted, &group_restricted);
  printf("RESTRICTED TO GROUP:");
  if(group_restricted) printf(" ON\n");
  else printf(" OFF\n");
 

  int_swap(&backup_edge_top, &edge_top);
  for(i=0;i<edge_top || i<backup_edge_top ; i++)
    {  
      edge_swap(backup_edge[i], edge[i]);
    }

  int_swap(&backup_triangle_top, &triangle_top);

  for(i=0;i<triangle_top || i<backup_triangle_top; i++)
    {  
      triangle_swap(backup_triangle[i],triangle[i]);
    }


  for(i=0; i<16; i++)
    {
      float_swap(&backup_transformation.mov[i],&transformation.mov[i]);
      double_swap(&backup_transformation.M[i], &transformation.M[i]);
      double_swap(&backup_transformation.R[i], &transformation.R[i]);
    }

  vector_swap(backup_cursor, cursor);
  cursor_set(cursor[0], cursor[1], cursor[2]);

  float_swap( &backup_cursor_step, &cursor_step);
  cursor_set_step( cursor_step);

 for(i=0; i<4; i++)
   {
     float_swap(&backup_light[i], &light[i]);
   }

 int_swap(&backup_two_lights, &two_lights);
 
 int_swap(&backup_background, &background);
 int_swap(&backup_current_color, &current_color);

 {
   enum Context tmp;
   tmp=backup_context;
   backup_context= context;
   context=tmp;
 }
 vector_swap(backup_et0_marker,et0_marker);
 vector_swap(backup_t1_marker, t1_marker); 
 context_switch(context);
 
 float_swap(&backup_move_step, &move_step);
}



