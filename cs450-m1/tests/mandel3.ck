# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Building image file "mandel3.tga"
Coordinates are (-0.15,0.85) to (-0.07,0.91)
No multithreading will be used
Total iterations: 8074937510
Wrote 8294401 bytes into file mandel3.tga
EOF
pass;
