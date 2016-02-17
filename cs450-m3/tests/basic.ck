# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
No file name passed; using "cs450-mplot.tga"
Building image file "cs450-mplot.tga"
Coordinates are (-2.00,-1.05) to (0.80,1.05)
No multithreading will be used
Total iterations: 7107658756
Wrote 8294401 bytes into file cs450-mplot.tga
EOF
pass;
