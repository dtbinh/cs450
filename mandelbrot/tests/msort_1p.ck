# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running merge sort
Array size: 172800 entries
Bubble sort threshold: 5000
Threading depth: 4
Unsorted text file: ../tests/480_x_360.txt
Sorted text file: 480_x_360p_sorted.txt
[0,21599] merging
[0,43199] merging
[0,86399] merging
[0,172799] merging
EOF
pass;
