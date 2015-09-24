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
Number of threads: 7
Output image name: mand1_odd.tga
Thread 0 starts at y value -1.050000
Thread 1 starts at y value -0.752500
Thread 2 starts at y value -0.455000
Thread 3 starts at y value -0.157500
Thread 4 starts at y value 0.140000
Thread 5 starts at y value 0.437500
Thread 6 starts at y value 0.735000
Writing 518418 bytes
EOF
pass;
