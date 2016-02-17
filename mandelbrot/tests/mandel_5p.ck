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
Number of threads: 20
Output image name: mand5p.tga
Thread 0 starts at y value -0.300000
Thread 1 starts at y value -0.277500
Thread 2 starts at y value -0.255000
Thread 3 starts at y value -0.232500
Thread 4 starts at y value -0.210000
Thread 5 starts at y value -0.187500
Thread 6 starts at y value -0.165000
Thread 7 starts at y value -0.142500
Thread 8 starts at y value -0.120000
Thread 9 starts at y value -0.097500
Thread 10 starts at y value -0.075000
Thread 11 starts at y value -0.052500
Thread 12 starts at y value -0.030000
Thread 13 starts at y value -0.007500
Thread 14 starts at y value 0.015000
Thread 15 starts at y value 0.037500
Thread 16 starts at y value 0.060000
Thread 17 starts at y value 0.082500
Thread 18 starts at y value 0.105000
Thread 19 starts at y value 0.127500
Writing 8294418 bytes
EOF
pass;
