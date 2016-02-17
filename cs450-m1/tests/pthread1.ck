# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Building image file "pthread1.tga"
Coordinates are (-2.00,-1.05) to (0.80,1.05)
Running with Pthreads
All threads handle 144 rows each
Total iterations: 7107658756
Wrote 8294401 bytes into file pthread1.tga
EOF
pass;
