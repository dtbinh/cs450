# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running in verbose mode
Running in numeric mode
Output will be in hexadecimal format
Opening file "small.data"
File contains 6 bytes of data
48 65 6C 6C 6F 0A
EOF
pass;
