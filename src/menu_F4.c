#include "./et-edit.h"
void menu_F4()
{
char * s; /* pointer to menu selector */


printf("\n CONSTRUCTIVE METHODS (F5) MENU:\n");
printf("\n");
  printf(" c       move cursor to the current group center\n");
  printf(" i       move cursor to intersection of line AB and plane CDE\n");
  printf(" itmcg   intersection of triangles of marked and curent groups\n");
  printf(" n       find cursor_step*(vector normal to the plane ABC)\n");
  printf(" s       scale current group by  |AB|/|CD|, where cursor is the fixed point of scaling\n");
  printf(" sd      scale current group by  |AB|/|CD| in direction EF\n");
  printf(" t       three point transformation ABC to DEF\n");
  printf(" ff      find folding point: axes AB, AC, rotated lines AD, AE, direction F\n");
  printf(" fold    folding marked and current group around AB and AC in direction F so that AD meets AE\n");

s=get_menu_selector();

if(strcmp(s,"c")==0){ 
	float X[3];
	vectorcpy(X, cursor);
	find_group_center(group_current, X);
	cursor_set(X[0], X[1], X[2]);
}
else if(strcmp(s,"i")==0){ 
	    float X[3];
	    vectorcpy(X, cursor);
	    line_triangle_intersection(point[10], point[11],
				       point[12], point[13], point[14],
				       X);
	    cursor_set(X[0], X[1], X[2]);
}
else if(strcmp(s,"itmcg")==0){ 
	  group_tt_intersection(group_current, group_marker);
}
else if(strcmp(s,"n")==0){ 
	float X[3];
	triangle_normal_vector(point[10], point[11],point[12],X);
        vector_scale(cursor_step,X);
        vectorcpy(et0_marker, cursor);
        vector_add(cursor, X, X);
        context_switch(context_et0);
	cursor_set(X[0], X[1], X[2]);
}
else if(strcmp(s,"s")==0){ 
	  constr_scale(point[10], point[11],
		       point[12], point[13], cursor,
		       group_current);
}
else if(strcmp(s,"sd")==0){ 
	  constr_scale_in_direction(point[10], point[11],
				    point[12], point[13], 
				    point[14], point[15], 
				    cursor,
				    group_current);
}
else if(strcmp(s,"t")==0){ 
	  three_point_transform(point[10], point[11], point[12], 
				point[13], point[14], point[15],
				group_current);
}
else if(strcmp(s,"ff")==0){ 
	    float V[3];
	    if(find_folding(point[10], 
			    point[11], point[12], 
			    point[13], point[14], 
			    point[15],
			    V)
	       )
	      {
		cursor_set(V[0], V[1], V[2]);
	      }
}
else if(strcmp(s,"fold")==0){ 
	    float V[3], mv1[3], mv2[3], R1[3][3], R2[3][3];
	    if(find_folding(point[10], 
			    point[11], point[12], 
			    point[13], point[14], 
			    point[15],
			    V)
	       &&
	       find_three_point_transform(point[10], point[11], point[13],
					 point[10], point[11], V,
					 mv1,R1)
	       &&
	       find_three_point_transform(point[10], point[12], point[14],
					 point[10], point[12], V,
					 mv2,R2)
	       )
	      {

		backup();
		group_transform(group_current,mv2,R2);
		if(group_marker!=group_current)
		  group_transform(group_marker,mv1,R1);
 
	       
	      }
}

}


