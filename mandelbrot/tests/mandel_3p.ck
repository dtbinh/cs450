# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running Mandelbrot
Image width: 1200
Image height: 900
Lower-left: (-0.150000,0.850000)
Upper-right: (-0.070000,0.900000)
Number of threads: 10
Output image name: mand3p.tga
Thread 0 starts at y value 0.850000
Thread 1 starts at y value 0.855000
Thread 2 starts at y value 0.860000
Thread 3 starts at y value 0.865000
Thread 4 starts at y value 0.870000
Thread 5 starts at y value 0.875000
Thread 6 starts at y value 0.880000
Thread 7 starts at y value 0.885000
Thread 8 starts at y value 0.890000
Thread 9 starts at y value 0.895000
Writing 3240018 bytes
EOF
pass;
