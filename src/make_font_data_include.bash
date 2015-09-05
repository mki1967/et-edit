#!/bin/bash
# execute this script to create font_data.include -- the file that contains collected fonts
# exported in subdirectories of res/et-fonts/


echo '/* #include "include/et-types.h" */ /* uncomment after code reorganization */' > font_data.include
cat res/et-fonts/alphanum/font_* >> font_data.include
cat res/et-fonts/points/font_*  >> font_data.include
