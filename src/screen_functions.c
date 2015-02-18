#include "include/et-edit.h"
void screen_init(struct Screen* scr)
{
  scr->distance=screen_distance;
  scr->clip_min=clip_min;
  scr->clip_max=clip_max;
  scr->width=screen_width;
  scr->height=screen_height;
}


void screen_set_distance(struct Screen* scr,float distance)
{
  scr->distance=distance;
  setfrustum();
  printf("Screen distance is: %f\n", scr->distance);
}


void screen_set_clipping(struct Screen* scr,float clip_min, float clip_max)
{
  if(clip_min_limit<= clip_min && clip_min<= clip_max)
    {
      scr->clip_min=clip_min;
      scr->clip_max=clip_max;
      setfrustum();
    }
  else printf("Screen clipping: bad values !\n");
  printf("Screen: clip_min = %f, clip_max = %f\n", scr->clip_min, scr->clip_max);

}


