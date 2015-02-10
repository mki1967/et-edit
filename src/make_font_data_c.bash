#!/bin/bash
# execute this script to create font_data.c -- the file that contans collected fonts
# exported in subdirectories of res/et-fonts/


echo '/* #include "include/et-types.h" */ /* uncomment after code reorganization */' > font_data.c
cat res/et-fonts/alphanum/font_* >> font_data.c
cat res/et-fonts/points/font_*  >> font_data.c
