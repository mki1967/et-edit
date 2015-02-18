#include "include/et-edit.h"
 /* // POINTS */

void point_print()
{
  int i;
  printf("POINTS:\n");
  for(i=0; i<10; i++)
    printf("'%d' = (%f, %f, %f)\n", i, 
	   point[i][0],point[i][1],point[i][2]);
  printf("\n");
  for(i=10; i<POINT_MAX; i++)
    printf("'%c' = (%f, %f, %f)\n", i-10+'A', 
	   point[i][0],point[i][1],point[i][2]);

}


void point_store(int i, float v[3])
{
  if(i<0 || i>= POINT_MAX) return;
  vectorcpy(point[i], v);
  printf("POINT STORED: '%c' =(%f, %f, %f)\n", 
	 i<10 ? i+'0' :  i-10+'A',
	 point[i][0],
	 point[i][1],
	 point[i][2]);
}



 /* // REGULAR POLYGON */

void polygon_get_vertex(int i, float v[3])
{
  int d[3], x,y;

  sort_dimmensions(1.0, 0.0, 0.0, &transformation,d);
  x=d[0];
  sort_dimmensions(0.0, 1.0, 0.0, &transformation,d);
  y= (d[0]!=x)? d[0]: d[1];

  vectorcpy(v, cursor);
  v[x]+= 2*cursor_step*cos(i*2*M_PI/polygon_n);
  v[y]+= 2*cursor_step*sin(i*2*M_PI/polygon_n);

}



void polygon_n_increase(int d)
{
  polygon_n+= d;
  if(polygon_n>POLYGON_MAX) polygon_n=POLYGON_MAX;
  if(polygon_n<POLYGON_MIN) polygon_n=POLYGON_MIN;

  printf("NUMBER OF POLYGON VERTICES: %d\n",polygon_n); 

}

void polygon_add(Bool edges, Bool triangles)
{
  int first_free;
  int group_new;
  int i;

  first_free=vertex_compress(vertex,
			     vertex_used,
			     edge_top, edge,
			     triangle_top, triangle, triangle_normal,
			     group);


  if(first_free+polygon_n >= VERTEX_MAX)
    {
      printf("polygon_add: too many vertices !!!\n");
      return;
    }
  if(edges && edge_top+polygon_n >= EDGE_MAX)
      {
      printf("polygon_add: too many edges !!!\n");
      return;
      }
  if(triangles && triangle_top+polygon_n-2 >= TRIANGLE_MAX)
      {
      printf("polygon_add: too many triangles !!!\n");
      return;
      }



  group_new=group_max(group,vertex_used)+1;
  group_change_current(group_new, &group_current, group, vertex_used );

  for(i=0; i<polygon_n; i++)
    polygon_get_vertex(i,vertex[i+first_free]);



  if(edges)
    {
    for(i=0; i<polygon_n; i++)
      {
	edge[edge_top+i][0]=first_free+i;
	edge[edge_top+i][1]=first_free+(i+1)%polygon_n;
	edge_sort(edge[edge_top+i]);
	edge[edge_top+i][2]=current_color;
	vertex_used[first_free+i]++;
	vertex_used[first_free+(i+1)%polygon_n]++;
      }
    edge_top+=polygon_n;
    }


  if(triangles)
    {
    for(i=0; i<polygon_n-2; i++)
      {
	triangle[triangle_top+i][0]=first_free;
	triangle[triangle_top+i][1]=first_free+i+1;
	triangle[triangle_top+i][2]=first_free+i+2;
	triangle[triangle_top+i][3]=current_color;
	 /*  triangle_sort(triangle[triangle_top+i]); */
	triangle_set_normal(triangle_top+i);
	vertex_used[first_free]++;
	vertex_used[first_free+i+1]++;
	vertex_used[first_free+i+2]++;
      }
    triangle_top+=polygon_n-2; 
    }

  group_change_current(group_current, &group_current, group, vertex_used );


}



/* OTHER CONSTRUCTIVE METHOS */

void find_group_center(int g, float X[3])
{
  int i,n; 


  n=group_size(g, group, vertex_used);
  if(n==0)
    {
      printf("FIND GROUP CENTER: group contains no vertices !!\n");
      return;
    }

  float_zeroes(3, X);
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==g)
      vector_add(vertex[i], X,X);

  vector_scale(1.0/n, X);
}



void line_triangle_intersection( float A[3], float B[3],
			    float C[3], float D[3], float E[3],
			    float X[3])
{
  double t;
  line_triangle_solve(A,B, C,D,E, X, &t);
}



Bool line_triangle_solve( float A[3], float B[3],
			    float C[3], float D[3], float E[3],
			    float X[3], double *t)
{
   /*  sets X to the intersection of line AB with triangle CDE */
  float m[3][3];
  double d, d1;

  if(vector_eq(A,B))
    {
      printf("intersection of AB with CDE not found: A=B !!!\n");
      return False;
    }

  if(vector_eq(C,D) ||vector_eq(C,E) || vector_eq(D,E) )
    {
      printf("intersection of AB with CDE not found: CDE is not a triangle !!!\n");
      return False;
    }


  vector_sub(D,C, m[0]);
  vector_sub(E,C, m[1]);
  vector_sub(A,B, m[2]);

  d=matrix3_det(m);

  if(d==0)
    {
      printf("intersection of AB with CDE not found: AB and CDE are parallel !!!\n");
      return False;    
    }


  vectorcpy(X, m[2]);
  vector_sub(A,C, m[2]);
  d1=matrix3_det(m);
  *t=d1/d;
  vector_scale(-d1/d, X);
  vector_add(A,X, X);
  return True;

} 


Bool triangle_triangle_intersection( float A1[3],float A2[3], float A3[3],
				     float B1[3],float B2[3], float B3[3],
				     float E1[3],float E2[3]
				     )
{
  float M[3][3], NA[3], NB[3],
    B_1[3], B_2[3], B_3[3], X1[3], X2[3],
    A_1[3], A_2[3], A_3[3], Y1[3], Y2[3];
  double det1,det2, det3, t1, t2;
  triangle_normal_vector(A1, A2, A3, NA);
  if(scalar_product(NA,NA)==0) 
    {
      return False;
    }
  
  triangle_normal_vector(B1, B2, B3, NB);
  if(scalar_product(NB,NB)==0) 
    {
      return False;
    }



  vector_sub(A2,A1, M[0]);
  vector_sub(A3,A1, M[1]);

  vector_sub(B1,A1, M[2]);
  det1=matrix3_det(M);

  vector_sub(B2,A1, M[2]);
  det2=matrix3_det(M);

  vector_sub(B3,A1, M[2]);
  det3=matrix3_det(M);


  if( (det1<=0 && det2<=0 && det3<=0) ||
      (det1>=0 && det2>=0 && det3>=0)
      )
    {
       /*  printf("all vertices of B are on the same side of plane A1,A2,A3\n"); */
      return False;
    }

  if( (det1<0 && det2>=0 && det3>=0) ||
      (det1>0 && det2<=0 && det3<=0) 
      )
    {
      vectorcpy(B_1,B1);
      vectorcpy(B_2,B2);
      vectorcpy(B_3,B3);
    }
  else
  if( (det2<0 && det1>=0 && det3>=0) ||
      (det2>0 && det1<=0 && det3<=0) 
      )
    {
      vectorcpy(B_1,B2);
      vectorcpy(B_2,B1);
      vectorcpy(B_3,B3);
    }
  else
  if( (det3<0 && det2>=0 && det1>=0) ||
      (det3>0 && det2<=0 && det1<=0) 
      )
    {
      vectorcpy(B_1,B3);
      vectorcpy(B_2,B2);
      vectorcpy(B_3,B1);
    }

   /*  B_1 is on the other side of the plane A1, A2, A3 than B_2, B_3. */

  line_triangle_intersection(B_1,B_2, A1,A2,A3, X1);
  line_triangle_intersection(B_1,B_3, A1,A2,A3, X2);



  vector_sub(B2,B1, M[0]);
  vector_sub(B3,B1, M[1]);

  vector_sub(A1,B1, M[2]);
  det1=matrix3_det(M);

  vector_sub(A2,B1, M[2]);
  det2=matrix3_det(M);

  vector_sub(A3,B1, M[2]);
  det3=matrix3_det(M);


  if( (det1<=0 && det2<=0 && det3<=0) ||
      (det1>=0 && det2>=0 && det3>=0)
      )
    {
       /*  printf("all vertices of A are on the same side of plane B1,B2,B3\n"); */
      return False;
    }

  if( (det1<0 && det2>=0 && det3>=0) ||
      (det1>0 && det2<=0 && det3<=0)
      )
    {
      vectorcpy(A_1,A1);
      vectorcpy(A_2,A2);
      vectorcpy(A_3,A3);
    }
  else
  if( (det2<0 && det1>=0 && det3>=0) ||
      (det2>0 && det1<=0 && det3<=0)
      )
    {
      vectorcpy(A_1,A2);
      vectorcpy(A_2,A1);
      vectorcpy(A_3,A3);
    }
  else
  if( (det3<0 && det2>=0 && det1>=0) ||
      (det3>0 && det2<=0 && det1<=0)
      )
    {
      vectorcpy(A_1,A3);
      vectorcpy(A_2,A2);
      vectorcpy(A_3,A1);
    }
   /*  A_1 is on the other side of the plane B1, B2, B3 than A_2, A_3. */

  vector_add(A_1, NA, NA);
  line_triangle_solve(X1,X2, A_1,A_2,NA, Y1, &t1);
  line_triangle_solve(X1,X2, A_1,A_3,NA, Y2, &t2);

  if(t2<t1)
    {
      double_swap(&t1, &t2);
      vector_swap(Y1, Y2);
    }

  if( t2<=0  || t1>=1 )
    {
       /*  printf("t2==%lf<=0  || t1==%lf>=1\n", t2, t1); */
      return False;
    }


  if(t1>0)
    vectorcpy(E1, Y1);
  else
    vectorcpy(E1, X1);

  if(t2<1)
    vectorcpy(E2, Y2);
  else
    vectorcpy(E2, X2);


  return True;

   /* / DOKONCZ ... */

}


void group_tt_intersection(int g1, int g2)
{
  int i,i1,i2, n1, n2, g_new;
  int *t1, *t2;
  float E1[3],E2[3];

  if(g1==g2)
    {
      printf("Intersection with the same group ?!\n");
      return;
    }
  n1=group_internal_triangles(g1);
  n2=group_internal_triangles(g2);
  if(n1==0 || n2==0)
    {
      printf("At least one of the groups contains no internal triangles\n");
      return;
    }


  backup();

  t1=(int*) malloc(n1*sizeof(int));
  t2=(int*) malloc(n2*sizeof(int));

  if(t1==NULL || t2==NULL) 
    {
      printf("Out of memory !!!\n");
      free(t1);
      free(t2);
      return;
    }

  i1=i2=0;
  for(i=0; i<triangle_top; i++)
    if(group[triangle[i][0]]==g1 &&
       group[triangle[i][1]]==g1 &&
       group[triangle[i][2]]==g1 
       )
      {
	*(t1+i1)=i;
	i1++;
      }
    else
    if(group[triangle[i][0]]==g2 &&
       group[triangle[i][1]]==g2 &&
       group[triangle[i][2]]==g2 
       )
      {
	*(t2+i2)=i;
	i2++;
      }

  g_new=group_max(group, vertex_used)+1;
  group_change_current(g_new, &group_current, group, vertex_used );

  for(i1=0; i1<n1; i1++)
    for(i2=0; i2<n2; i2++)
	if(triangle_triangle_intersection(
					  vertex[triangle[*(t1+i1)][0]],
					  vertex[triangle[*(t1+i1)][1]],
					  vertex[triangle[*(t1+i1)][2]],
					  vertex[triangle[*(t2+i2)][0]],
					  vertex[triangle[*(t2+i2)][1]],
					  vertex[triangle[*(t2+i2)][2]],
					  E1,E2
					  )
	   )
	  edge_set_color( edge_vector_insert(E1, E2), current_color);

  free(t1);
  free(t2);
  group_change_current(g_new, &group_current, group, vertex_used );
  if(!group_restricted)
    {
      group_restricted_switch();
      printf("(press <1> to switch back to GROUP RESTRICTED: OFF)\n");
    }


}


void constr_scale(float A[3], float B[3], float C[3], float D[3],
	     float fixed_point[3], 
	     int g)
      /*  scales group by |AB|/|CD| */
{
  double lAB,lCD,s;
  float AB[3], CD[3];
  vector_sub(B, A, AB);
  vector_sub(D, C, CD);
  lAB=vector_length(AB);
  lCD=vector_length(CD);
  if(lCD==0) 
    {
      printf("scaling refused: |CD| = 0 !!!\n");
      return;
    }
  s=lAB/lCD;
  
  if(s== 1.0)
    {
      printf("scaling by 1 does not change anything !!!\n");
      return;
    }

  backup();
  group_scale(g, s, s, s, fixed_point, group, vertex, vertex_used);
}


void constr_scale_in_direction(float A[3], float B[3], float C[3], float D[3],
			  float E[3], float F[3], float fixed_point[3], 
			  int g)
      /*  scales group by |AB|/|CD| in direction EF */
{
  double lAB,lCD,s;
  float AB[3], CD[3], EF[3], v[3], tmp[3];
  double sp;
  int i;

  if(vector_eq(E,F))
    {
      printf("scaling in direction EF refused: EF = 0 !!!\n");
      return;
    }

  vector_sub(B, A, AB);
  vector_sub(D, C, CD);
  vector_sub(F, E, EF);
  vector_normalize(EF);

  lAB=vector_length(AB);
  lCD=vector_length(CD);
  if(lCD==0) 
    {
      printf("scaling refused: |CD| = 0 !!!\n");
      return;
    }
  s=lAB/lCD;

  if(s== 1.0)
    {
      printf("scaling by 1 does not change anything !!!\n");
      return;
    }

  backup();  
  for(i=0; i<VERTEX_MAX; i++)
    if(vertex_used[i] && group[i]==g)
      {
	vector_sub(vertex[i], fixed_point, v);
	sp=scalar_product(EF,v);
	vectorcpy(tmp, EF);
	vector_scale((s-1)*sp, tmp);
	vector_add(v,tmp, v);
	vector_add(v, fixed_point, vertex[i]);
      }

}



Bool find_three_point_transform(float P1[3], float P2[3], float P3[3], 
		     float Q1[3], float Q2[3], float Q3[3],
		     float mv[3], float R[3][3])
      /*  assumption: for each Pi!=Pj and Qi!=Qj */
{
  float u[3][3], w[3][3];
  int i,j,k;

  vector_sub(P2,P1, u[0]);
  vector_sub(P3,P1, u[1]);
  vector_product(u[0], u[1], u[1]);  /*  u[1] = (P2-P1)x(P3-P1) */
  vector_normalize(u[0]);
  vector_normalize(u[1]);
  vector_product(u[0], u[1], u[2]);  /*  u[2] |_ u[1] |_ u[2] */

  if(vector_length(u[2])==0) 
    {
      printf("three points transformation: the first three points are colinear !!!\n");
      return False;
    }

  vector_sub(Q2,Q1, w[0]);
  vector_sub(Q3,Q1, w[1]);
  vector_product(w[0], w[1], w[1]);  /*  w[1] = (Q2-Q1)x(Q3-Q1) */
  vector_normalize(w[0]);
  vector_normalize(w[1]);
  vector_product(w[0], w[1], w[2]);  /*  w[2] |_ w[1] |_ w[2] */
  if(vector_length(w[2])==0) 
    {
      printf("three points transformation: the last three points are colinear !!!\n");
      return False;
    }


  for(i=0; i<3; i++)
    for(j=0; j<3; j++)
      {
	R[i][j]=0;
	for(k=0; k<3; k++)
	  R[i][j]+=u[k][j]*w[k][i];
      }

  matrix3_vector_mult(R, P1, mv);
  vector_sub(Q1, mv, mv);  /*  mv = Q1 - R*P1 */
  return True;

}


void three_point_transform(float P1[3], float P2[3], float P3[3], 
		      float Q1[3], float Q2[3], float Q3[3],
		       int g)
      /*  assumption: for each Pi!=Pj and Qi!=Qj */
{
  float  mv[3], R[3][3];

  if(find_three_point_transform(P1, P2, P3, 
				Q1, Q2, Q3,
				mv, R))
    {
      backup();
      group_transform(g,mv,R);
    }
}




 /*  FOLDING */


Bool find_centered_folding(float A1[3],float A2[3],
			    float B1[3],float B2[3],
			    float K[3],
			    float V[3])
{
  double eps= 1e-14;
  int swap=0;  /*  0 - no swap, 1 - swaped xy, 2 - swaped yz  */
  double m,p,q, A2B2, A1B1, a, b, c, delta, x1, y1, z1, d1,d2;
  float matrix[3][3];

  A2B2=scalar_product(A2,B2);
  A1B1=scalar_product(A1,B1);



  m=A2[2]*A1[0]-A1[2]*A2[0];

  if(fabs(m)<eps)
    {
      float_swap(&A1[0], &A1[1]);
      float_swap(&A2[0], &A2[1]);
      float_swap(&B1[0], &B1[1]);
      float_swap(&B2[0], &B2[1]);
      float_swap(&K[0], &K[1]);
      m=A2[2]*A1[0]-A1[2]*A2[0];
      swap=1;
    }

  if(fabs(m)<eps)
    {
      float_swap(&A1[0], &A1[1]);
      float_swap(&A2[0], &A2[1]);
      float_swap(&B1[0], &B1[1]);
      float_swap(&B2[0], &B2[1]);
      float_swap(&K[0], &K[1]);
      float_swap(&A1[2], &A1[1]);
      float_swap(&A2[2], &A2[1]);
      float_swap(&B1[2], &B1[1]);
      float_swap(&B2[2], &B2[1]);
      float_swap(&K[2], &K[1]);
      m=A2[2]*A1[0]-A1[2]*A2[0];
      swap=2;
    }



  if(fabs(m)<eps)
    {
      printf("FOLDING REFUSED: the axes are colinear !!\n");
      printf("m=%e\n", m);
      return False;
    }

   /*  m!=0  */

  p=(A1[1]*A2[0]-A2[1]*A1[0])/m;
  q=(A2B2*A1[0]-A1B1*A2[0])/m;

  a=(1+p*p)*A2[0]*A2[0]+(A2[1]+p*A2[2])*(A2[1]+p*A2[2]);
  b=2*(p*q*A2[0]*A2[0]-(A2B2-q*A2[2])*(A2[1]+p*A2[2]));
  c=(q*q-1)*A2[0]*A2[0]+(A2B2-q*A2[2])*(A2B2-q*A2[2]);

  if(fabs(a)<eps)
    {
      a=(1+p*p)*A1[0]*A1[0]+(A1[1]+p*A1[2])*(A1[1]+p*A1[2]);
      b=2*(p*q*A1[0]*A1[0]-(A1B1-q*A1[2])*(A1[1]+p*A1[2]));
      c=(q*q-1)*A1[0]*A1[0]+(A1B1-q*A1[2])*(A1B1-q*A1[2]);
    }

  if(fabs(a)<eps)
    {
    printf("FOLDING REFUSED: the axes are colinear !!!\n");
    return False;
    }

   /*  a!=0 */

  delta= b*b-4*a*c;

  if(delta<0) 
    {
      printf("FOLDING FAILED: probably too large angle between rotated lines !!!\n");
      return False;
    }

  y1= (-b-sqrt(delta))/(2*a);
  z1= p*y1+q;
  x1= (fabs(A2[0]) > fabs(A1[0])) ?
    (A2B2-y1*A2[1]-z1*A2[2])/A2[0]:
    (A1B1-y1*A1[1]-z1*A1[2])/A1[0];
 

  vectorcpy(matrix[0], A1);
  vectorcpy(matrix[1], A2);
  vectorcpy(matrix[2], K);

  d1=matrix3_det(matrix);

  matrix[2][0]=x1;
  matrix[2][1]=y1;
  matrix[2][2]=z1;

  d2=matrix3_det(matrix);

  if(d1*d2 < 0)
    {
      y1= (-b+sqrt(delta))/(2*a);
      z1= p*y1+q;
      x1= (fabs(A2[0]) > fabs(A1[0])) ?
	(A2B2-y1*A2[1]-z1*A2[2])/A2[0]:
	(A1B1-y1*A1[1]-z1*A1[2])/A1[0];
    }


  V[0]=x1;
  V[1]=y1;
  V[2]=z1;

  switch(swap)
    {
    case 1:
      float_swap(&A1[0], &A1[1]);
      float_swap(&A2[0], &A2[1]);
      float_swap(&B1[0], &B1[1]);
      float_swap(&B2[0], &B2[1]);
      float_swap(&K[0], &K[1]);
      float_swap(&V[0], &V[1]);
      break;

    case 2:
      float_swap(&A1[2], &A1[1]);
      float_swap(&A2[2], &A2[1]);
      float_swap(&B1[2], &B1[1]);
      float_swap(&B2[2], &B2[1]);
      float_swap(&K[2], &K[1]);
      float_swap(&V[2], &V[1]);
      break;

    }

  return True;
   /*  DOKONCZYC ... */


}




Bool find_folding(float A[3],float B[3],float C[3],
		  float D[3],float E[3],float F[3], 
		  float V[3])
     /* set V so that AV is the line, where AD meets AE while
        AD and AE are rotarted around AB and AC respectively,
	and F and V are on the same side of the plane ABC.
      */
{
  float A1[3], A2[3], B1[3], B2[3], K[3];

  if(vector_eq(A,B))
    {
      printf("FOLDING REFUSED: A=B !!!\n");
      return False;
    }
  vector_sub(B,A, A1);
  vector_normalize(A1);

  if(vector_eq(A,C))
    {
      printf("FOLDING REFUSED: A=C !!!\n");
      return False;
    }
  vector_sub(C,A, A2);
  vector_normalize(A2);

  if(vector_eq(A,D))
    {
      printf("FOLDING REFUSED: A=D !!!\n");
      return False;
    }
  vector_sub(D,A, B1);
  vector_normalize(B1);

  if(vector_eq(A,E))
    {
      printf("FOLDING REFUSED: A=E !!!\n");
      return False;
    }
  vector_sub(E,A, B2);
  vector_normalize(B2);

  vector_sub(F,A, K);

  if(find_centered_folding(A1,A2, B1,B2, K,V))
    {
      vector_add(A,V, V);
      return True;
    }
  else return False;

}


