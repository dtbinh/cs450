# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF', <<'EOF']);
Running merge sort
ERROR: Expected 1172800 entries but only got 172800
EOF
Running merge sort
EOF
pass;
