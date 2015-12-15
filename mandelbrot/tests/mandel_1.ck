# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running Mandelbrot
Image width: 480
Image height: 360
Lower-left: (-2.000000,-1.050000)
Upper-right: (0.800000,1.050000)
Number of threads: 1
Output image name: mand1.tga
Writing 518418 bytes
EOF
pass;
