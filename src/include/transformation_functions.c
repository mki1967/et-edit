
void transformation_fprintf(FILE* s, struct Transformation* t)
{
  int i;
  fprintf(s,"\n");
  vector_fprintf(s, t->mov );
  for(i=0; i<16; i++)
    fprintf(s, "%e\n", t->M[i]);
  for(i=0; i<16; i++)
    fprintf(s, "%e\n", t->R[i]);
  fprintf(s,"\n");

}

void transformation_fscanf(FILE* s, struct Transformation* t)
{
  int i;
  vector_fscanf(s, t->mov );
  for(i=0; i<16; i++)
    fscanf(s, "%le", &(t->M[i]));
  for(i=0; i<16; i++)
    fscanf(s, "%le", &(t->R[i]));

}


void transformation_init(struct Transformation* t)
{
  t->mov[0]=0.0;
  t->mov[1]=0.0;
  t->mov[2]=-screen_distance;
  transformation_init_orientation(t);
}

void transformation_init_orientation(struct Transformation* t)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGetDoublev( GL_MODELVIEW_MATRIX, t->M );
  glGetDoublev( GL_MODELVIEW_MATRIX, t->R );

}


void rotate(double angle, double x, double y, double z, struct Transformation* t)
{
  glMatrixMode(GL_MODELVIEW);


  glLoadIdentity();
  glRotated(angle, x,y,z);
  glMultMatrixd(t->M);
  glGetDoublev( GL_MODELVIEW_MATRIX, t->M );

  glLoadMatrixd( t->R );
  glRotated(-angle, x,y,z);
  glGetDoublev( GL_MODELVIEW_MATRIX, t->R );
    

}


void transformation_scale(double s,struct Transformation* t, float cursor[16])
{
  float tmp1[16], tmp2[16];

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp1 );
  glPopMatrix();

  scale*=s;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp2 );
  glPopMatrix();

  vector_sub(tmp2, tmp1, tmp1);
  vector_sub(t->mov, tmp1, t->mov);

}

void transformation_rotate(double angle, double x, double y, double z, 
		      struct Transformation* t, float cursor[16])
{
  float dist, tmp[16];

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp );
  glPopMatrix();


  vector_add(tmp, t->mov, tmp);
  dist=sqrt(scalar_product(tmp,tmp));

  rotate(angle, x,y,z, t);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd( t->M );
  glScaled(scale,scale,scale);  /* / */
  glMultMatrixf(cursor);
  glGetFloatv( GL_MODELVIEW_MATRIX, tmp );
  glPopMatrix();
  
  t->mov[0]= -tmp[0];
  t->mov[1]= -tmp[1];
  t->mov[2]= -tmp[2]-dist;


}


void transformation_move(float x, float y, float z, struct Transformation* t)
{
  t->mov[0]+=x;
  t->mov[1]+=y;
  t->mov[2]+=z;
}



void find_versor(float x, float y, float z, float e[])
{
  
  float max, v[16];
  int  imax;
  float_zeroes(16,v);
  v[0]=x; v[1]=y; v[2]=z; v[3]=1.0;
 
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd(transformation.R);
  glMultMatrixf(v);
  glGetFloatv( GL_MODELVIEW_MATRIX, v);
  glPopMatrix();

  imax=0; max=fabs(v[0]);
  if(fabs(v[1])>max) { imax=1; max=fabs(v[1]); }
  if(fabs(v[2])>max) { imax=2; max=fabs(v[2]); }


  float_zeroes(3, e);
  e[imax]= (v[imax]>0) ? 1.0 : -1.0 ;

}

void sort_dimmensions(double x, double y, double z, struct Transformation *t,
		 int d[3])
{
  float v[16];

  float_zeroes(16,v);
  v[0]=x; v[1]=y; v[2]=z; v[3]=1.0;
 
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd(t->R);
  glMultMatrixf(v);
  glGetFloatv( GL_MODELVIEW_MATRIX, v);
  glPopMatrix();

  d[0]=0; d[1]=1; d[2]=2;


  if(fabs(v[d[0]]) < fabs(v[d[1]])) int_swap(&d[0], &d[1]);
  if(fabs(v[d[1]]) < fabs(v[d[2]])) int_swap(&d[1], &d[2]);
  if(fabs(v[d[0]]) < fabs(v[d[1]])) int_swap(&d[0], &d[1]);

}


