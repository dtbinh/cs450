# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Building image file "mandel2.tga"
Coordinates are (-1.50,-0.20) to (-0.30,0.70)
No multithreading will be used
Total iterations: 13051570015
Wrote 8294401 bytes into file mandel2.tga
EOF
pass;
