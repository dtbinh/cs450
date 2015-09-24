# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running merge sort
Array size: 691200 entries
Bubble sort threshold: 5000
Threading depth: 1
Unsorted text file: ../tests/960_x_720.txt
Sorted text file: 960_x_720_sorted.txt
[0,691199] merging
EOF
pass;
