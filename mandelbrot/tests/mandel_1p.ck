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
Number of threads: 5
Output image name: mand1p.tga
Thread 0 starts at y value -1.050000
Thread 1 starts at y value -0.630000
Thread 2 starts at y value -0.210000
Thread 3 starts at y value 0.210000
Thread 4 starts at y value 0.630000
Writing 518418 bytes
EOF
pass;
