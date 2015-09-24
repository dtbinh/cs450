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
Number of threads: 1
Output image name: mand2.tga
Writing 2073618 bytes
EOF
pass;
