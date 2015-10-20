# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Building image file "mandel4.tga"
Coordinates are (-0.03,0.64) to (0.01,0.67)
No multithreading will be used
Total iterations: 5148530685
Wrote 8294401 bytes into file mandel4.tga
EOF
pass;
