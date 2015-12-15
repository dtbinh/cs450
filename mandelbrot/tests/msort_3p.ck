# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Running merge sort
Array size: 1080000 entries
Bubble sort threshold: 5000
Threading depth: 8
Unsorted text file: ../tests/1200_x_900.txt
Sorted text file: 1200_x_900p_sorted.txt
[0,33749] merging
[0,67499] merging
[0,134999] merging
[0,269999] merging
[0,539999] merging
[0,1079999] merging
EOF
pass;
