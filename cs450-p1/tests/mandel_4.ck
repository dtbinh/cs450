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
Number of threads: 1
Output image name: mand4.tga
Writing 33177618 bytes
EOF
pass;
