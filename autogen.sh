#!/bin/sh
### autogen.sh #### (based on the hints from Stackoverflow ;-) ### 

# Use this script to bootstrap your build AFTER checking it out from
# source control. You should not have to use it for anything else.

echo "Regenerating autotools files"
autoreconf --install || exit 1
echo "Now run ./configure, make, and make install."
