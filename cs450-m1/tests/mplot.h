#ifndef __MANDELBROT_TEST_CASES__
#define __MANDELBROT_TEST_CASES__

#ifdef __INCLUDE_MANDEL_ARRAY__
double testcases[][4] = {
    { -2.00,  0.80, -1.05,  1.05 }, // mand_1.png
    { -1.50, -0.30, -0.20,  0.70 }, // mand_2.png
    { -0.15, -0.07,  0.85,  0.91 }, // mand_3.png
    { -0.03,  0.01,  0.635, 0.665 } // mand_4.png
};
#endif

#define MAX_TEST_CASE 3
#define WIDTH 1920
#define HEIGHT 1440

#endif
