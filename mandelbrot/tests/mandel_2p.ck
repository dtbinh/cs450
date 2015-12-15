# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running Mandelbrot
Image width: 960
Image height: 720
Lower-left: (-1.500000,-0.200000)
Upper-right: (-0.300000,0.700000)
Number of threads: 10
Output image name: mand2p.tga
Thread 0 starts at y value -0.200000
Thread 1 starts at y value -0.110000
Thread 2 starts at y value -0.020000
Thread 3 starts at y value 0.070000
Thread 4 starts at y value 0.160000
Thread 5 starts at y value 0.250000
Thread 6 starts at y value 0.340000
Thread 7 starts at y value 0.430000
Thread 8 starts at y value 0.520000
Thread 9 starts at y value 0.610000
Writing 2073618 bytes
EOF
pass;
