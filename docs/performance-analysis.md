/**************************************************************
 *
 *      README
 *
 *      Xiaoyan Xie (xxie05)
 *      Diwei Chen (dchen22)
 *      Oct 09 2024
 *      
 *      CS 40 HW03 - locality
 *
 **************************************************************/

ACKNOWLEDGEMENTS:
----------------
We received help on implementation and debugging from CS40 TA's; 
specifically, we want to acknowledge Michael and Naomi for their help on 
our design.

We also used the provided timing_test.c for timer part. We used a2test.c 
file as part of the test process for a2plain and uarray2b.


ARCHITECTURE:
-------------------------------
Part A, Part B have been implemented correctly to the best of our knowledge. 
In Part C we have implemented rotate 0, 90, 180, 270, and time options. 
Flip and transpose are unimplemented.

Part A: 
Here we use the UArray2 in uarray2.c from hw2 solution to create UArray2b. 
Since the solution uses an array of arrays to reoresent a 2D array, we use 
each array in UArray2 to store a block of data.
We found it necessary to use a if-else to assign blocksize in 
UArray2b_new_64K_block since the size of a single cell may be more than 64KB.

Part B:
We work on a2plain.c using a2block.c as a reference. 
We found it necessary to set block-related content to NULL in 
uarray2_methods_plain_struct, because they should not be called in row or 
column major transformation.

Part C:

The ppmtrans program handles image transformations and performance timing.
The program begins by parsing command line arguments to determine the rotation
angle and traversal method. 
1. Main Components: 
A. Main Function 
This is the entry point of the program. It parses command-line arguments to 
determine the rotation angle (0, 90, 180, or 270 degrees) and whether to 
apply row-major, column-major, or block-major traversal.
- It pens the PPM file and reads the image, then alls ppm_process to handle 
the image transformation and output.

B. ppm_process Function:
This is the driver function for rotation. It calculates the dimensions of 
the output image based on the rotation angle.
- Calls handle_rotate for actual image rotation and transformation.
- If no rotation is requested (0-degree), it bypasses the rotation logic and 
outputs the original image directly.

2. Rotation handlers
A. handle_rotate Function:
This is the core function responsible for applying the requested rotation to 
the image.
It uses the map function from the methods structure (determined by traversal
type: row-major, column-major, or block-major) to apply the rotation through
appropriate rotation functions (e.g., rotate90, rotate180, rotate270).
- It updates the dimensions of the output image only for rotations where width
and height are swapped (90 and 270 degrees).

B. Rotation Functions (rotate90, rotate180, rotate270):
These functions are responsible for pixel-by-pixel rotation of the image.
They use the pixel mapping functions (like new_methods->at) to set the 
transformed image coordinates based on the input image.

3. Traversal Methods:
A. A2Methods Structure:
- Ppmtrans uses an abstract A2Methods interface, which provides different 
traversal functions
 (row-major, column-major, block-major) to traverse and manipulate the image.

B. Mapping Functions:
These traversal methods use a map function, which iterates over the image data
in the specified traversal order and applies the appropriate transformation 
function (rotate90, rotate180, rotate270).

4. Performance Modules:
- The program tracks the time taken for image transformations by using
a custom CPU timer (CPUTime_T).
- Timing information is written to the file specified by the -time option.

5. Data Structures:
A. A2 (2D Array) and A2Methods_T:
- The A2 structure represents a 2D array used for storing image pixel data.
- The methods in A2Methods_T provide access to the array elements (pixels)
and allow for row-major, column-major, or block-major traversal.

B. closure Structure:
The closure structure is used to pass the destination image (rotated_img)
and method pointers during rotation. It allows the rotation functions to 
apply transformations to the correct image.

************************** Part E Experiment ***************************

We used halligan server via ssh to run the test.

Computer Name and model: halligan server
vendor_id       : GenuineIntel
cpu family      : 6
model           : 85
model name      : Intel(R) Xeon(R) Silver 4214Y CPU @ 2.20GHz
clock rate:     : 2194.844 MHz (2.20GHz)
cache size      : 16896 KB

We used two test images, flowers.ppm (18,100 pixels) and 
mobo.ppm (49,939,200 pixels). Below are test results for each image.

flowers.ppm (18,100 pixels)

	        Rotation Time                   Rotation Time Per Pixel 
            (nanoseconds)                   (nanoseconds)
90_block	41,134,896	                    2,263
90_column	33,863,821	                    1,863
90_row	    28,961,728                      1,593
180_block	44,534,966	                    2,450
180_column  38,271,239	                    2,105
180_row	    33,779,148	                    1,858
270_block   41,364,114	                    2,275
270_column  35,542,565	                    1,955
270_row	    30,297,024	                    1,667

Speed Comparison:
    Mapping: row-major > column-major > block-major
    Rotation: 90° > 270° > 180°

mobo.ppm (49,939,200 pixels)

	        Rotation Time                   Rotation Time Per Pixel 
            (nanoseconds)                   (nanoseconds)
90_block	3,464,778,489	                69
90_column	5,246,095,827	                105
90_row	    4,757,110,535	                95
180_block	3,496,304,344	                70
180_column	7,914,975,759	                158
180_row	    2,365,311,136	                47
270_block	3,269,381,313	                65
270_column	5,247,868,691	                105
270_row	    4,756,265,329	                95

Speed Comparison:
    Mapping:  block-major > row-major > column-major
    Rotation: 180° > 90° = 270°

Discussion of Patterns:

 - Plain vs. Blocked Arrays:
For smaller images (flower.ppm), row-major is fastest, while block-major 
is slowest. For larger images (mobo.ppm), block-major is the fastest due 
to better cache efficiency.

 - Rotation Performance:

For small images, 90° is fastest, followed by 270°, with 180° being slowest. 
For large images, 180° becomes the fastest due to simpler, symmetric pixel 
mappings.
 - Time Per Pixel Across Image Sizes:
For the smaller image (flower.ppm), the time per pixel is higher 
(ranging from ~1,593 to 2,450 nanoseconds). The higher time per pixel is 
likely due to less efficient cache usage as the image is smaller.

For the larger image (mobo.ppm), the time per pixel drops significantly 
(ranging from 47 to 158 nanoseconds). The number of instructions per pixel 
seems more consistent across different mappings for the larger image because 
the image size is better suited to take advantage of the cache.
