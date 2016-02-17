# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running merge sort
Array size: 691200 entries
Bubble sort threshold: 5000
Threading depth: 6
Unsorted text file: ../tests/960_x_720.txt
Sorted text file: 960_x_720p_sorted.txt
[0,21599] merging
[0,43199] merging
[0,86399] merging
[0,172799] merging
[0,345599] merging
[0,691199] merging
EOF
pass;
