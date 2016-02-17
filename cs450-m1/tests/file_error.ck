# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Usage: mplot [options]
Options:
  -f F.tga      Specify the .tga file to use as output
                (default name is cs450-mplot.tga)
  -h            Print this message and exit
  -x x          Set the lower-left x-coordinate for the image
  -y y          Set the lower-left y-coordinate for the image
  -X x          Set the upper-right x-coordinate for the image
  -Y y          Set the upper-right y-coordinate for the image
  -p N          Run with Pthreads, using N threads in parallel
  -p all        Run with Pthreads, using the maximum available CPUs
  -p row        Run with Pthreads, using one thread per row
EOF
pass;
