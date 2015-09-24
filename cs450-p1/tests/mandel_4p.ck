# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running Mandelbrot
Image width: 3840
Image height: 2880
Lower-left: (-0.030000,0.635000)
Upper-right: (0.010000,0.665000)
Number of threads: 20
Output image name: mand4p.tga
Thread 0 starts at y value 0.635000
Thread 1 starts at y value 0.636500
Thread 2 starts at y value 0.638000
Thread 3 starts at y value 0.639500
Thread 4 starts at y value 0.641000
Thread 5 starts at y value 0.642500
Thread 6 starts at y value 0.644000
Thread 7 starts at y value 0.645500
Thread 8 starts at y value 0.647000
Thread 9 starts at y value 0.648500
Thread 10 starts at y value 0.650000
Thread 11 starts at y value 0.651500
Thread 12 starts at y value 0.653000
Thread 13 starts at y value 0.654500
Thread 14 starts at y value 0.656000
Thread 15 starts at y value 0.657500
Thread 16 starts at y value 0.659000
Thread 17 starts at y value 0.660500
Thread 18 starts at y value 0.662000
Thread 19 starts at y value 0.663500
Writing 33177618 bytes
EOF
pass;
