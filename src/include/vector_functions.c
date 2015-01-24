
void float_zeroes(int n, float v[])
{
  int i;
  for(i=0; i<n; i++) v[i]=0.0;
}


double matrix3_det(float m[3][3])
{
return    m[0][2]*( m[1][0]*m[2][1]-m[2][0]*m[1][1] )
         -m[1][2]*( m[0][0]*m[2][1]-m[0][1]*m[2][0] )
         +m[2][2]*( m[0][0]*m[1][1]-m[0][1]*m[1][0] );

}

void matrix3_vector_mult(float m[3][3], float v[3], float out[3])
{
  float r[3];
  r[0]=m[0][0]*v[0]+m[0][1]*v[1]+m[0][2]*v[2];
  r[1]=m[1][0]*v[0]+m[1][1]*v[1]+m[1][2]*v[2];
  r[2]=m[2][0]*v[0]+m[2][1]*v[1]+m[2][2]*v[2];
  vectorcpy(out, r);
}

void matrix_init(float m[16])
{
  float_zeroes(16, m);
  m[3]=m[7]=m[11]=m[15]=1;
}

void double_zeroes(int n, double v[])
{
  int i;
  for(i=0; i<n; i++) v[i]=0.0;
}


Bool vector_eq(float  a[], float b[])
{
  return a[0]==b[0] && a[1]==b[1] && a[2]==b[2];
}

void vector_add(float a[], float b[], float result[])
{
  result[0]=a[0]+b[0];
  result[1]=a[1]+b[1];
  result[2]=a[2]+b[2];
}

void vector_sub(float a[], float b[], float result[])
{
  result[0]=a[0]-b[0];
  result[1]=a[1]-b[1];
  result[2]=a[2]-b[2];
}

void vector_scale( float s, float v[])
{
  v[0]*=s;  v[1]*=s; v[2]*=s;
}

void vectorcpy(float dst[], float src[])
{
  dst[0]=src[0];
  dst[1]=src[1];
  dst[2]=src[2];
}


void vector_product(float a[], float b[], float r[])
{
  float result[3];
  result[0]=a[1]*b[2]-a[2]*b[1];
  result[1]=a[2]*b[0]-a[0]*b[2];
  result[2]=a[0]*b[1]-a[1]*b[0];
  vectorcpy(r, result);
}

double scalar_product(float a[3], float b[3])
{
  return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

double vector_length(float a[3])
{
  return sqrt(scalar_product(a,a));
}

double vector_distance(float a[3], float b[3])
{
  float c[3];
  vector_sub(a,b,c);
  return vector_length(c);
}

void vector_normalize(float v[])
{
  float sp=scalar_product(v,v);
  if(sp==0) return;  /*  zero length vector */
  sp=sqrt(sp); 
  v[0]/=sp;  v[1]/=sp;  v[2]/=sp;
}





 /* /// VECTOR I/O */

void vector_fprintf(FILE *stream, float v[3])
{
  fprintf(stream, "%f\n", v[0]);
  fprintf(stream, "%f\n", v[1]);
  fprintf(stream, "%f\n\n", v[2]);
} 

void vector_fscanf(FILE *stream, float v[3])
{
  fscanf(stream, "%f", &v[0]);
  fscanf(stream, "%f", &v[1]);
  fscanf(stream, "%f", &v[2]);
} 



