
void set_light(float light[16])
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixd(transformation.R);
  glMultMatrixf(basic_light);
  glGetFloatv( GL_MODELVIEW_MATRIX, light);
  glPopMatrix();
  printf("light=( %f, %f, %f, %f )\n", light[0], light[1], light[2], light[3]);
}

void switch_second_light()
{
  two_lights= !two_lights;
  if(two_lights) printf("Two opposite lights mode\n");
  else printf("One light mode\n");
}


