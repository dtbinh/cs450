# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF', <<'EOF']);
Running Mandelbrot
ERROR: Cannot run with no threads
EOF
Running Mandelbrot
EOF
pass;
