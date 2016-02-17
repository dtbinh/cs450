# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running Mandelbrot
Image width: 1920
Image height: 1440
Lower-left: (-1.400000,-0.300000)
Upper-right: (-0.800000,0.150000)
Number of threads: 1
Output image name: mand5.tga
Writing 8294418 bytes
EOF
pass;
