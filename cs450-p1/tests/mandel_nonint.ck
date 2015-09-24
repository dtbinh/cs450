# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF', <<'EOF']);
Running Mandelbrot
ERROR: Illegal value in input file
EOF
Running Mandelbrot
EOF
pass;
