struct Screen
{
  float distance;
  float clip_min, clip_max;
  int width, height;
};

struct Clipping_xyz
{
  float xmin;
  Bool xmin_flag;
  float xmax;
  Bool xmax_flag;

  float ymin;
  Bool ymin_flag;
  float ymax;
  Bool ymax_flag;

  float zmin;
  Bool zmin_flag;
  float zmax;
  Bool zmax_flag;
};

enum  Context
  { 
    context_default, 
    context_et0,
    context_t1 
  };

enum Keymode
  {
    key_default,
    /*    key_F1, */
    key_F2


  };

struct Transformation 
{
  float mov[16];
  double M[16], R[16];
  
};

typedef struct{
               float (*vertex)[3];
               int edge_top;
               int (*edge)[3];
              } SHAPE_VE; /* shape consisting of vectors and edges */



