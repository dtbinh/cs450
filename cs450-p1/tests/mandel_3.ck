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
Number of threads: 1
Output image name: mand3.tga
Writing 3240018 bytes
EOF
pass;
