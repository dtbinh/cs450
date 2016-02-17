# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF', <<'EOF']);
Running merge sort
ERROR: File 'not_here.txt' does not exist
EOF
Running merge sort
EOF
pass;
