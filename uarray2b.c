/**************************************************************
 *
 *      uarray2b.c
 *
 *      Xiaoyan Xie (xxie05)
 *      Diwei Chen (dchen22)
 *      Oct 09 2024
 *      
 *      CS 40 HW03 - locality
 *
 *      This file provides the implementation for a blocked 
 *      two-dimensional array data structure, allowing for 
 *      efficient manipulation and storage of 2D arrays. 
 *      It includes functions for creating, freeing, and accessing 
 *      elements of the array, as well as applying functions 
 *      to all elements in a specified mapping order (row-major
 *      or column-major).
 *
 **************************************************************/
#include <stdlib.h>
#include "assert.h"
#include "uarray.h"
#include "uarray2b.h"
#include <string.h>
#include "uarray2.h"
#include <math.h>


#define T UArray2b_T

/*********************** struct T ***********************
 * Defines the structure for a blocked 2D array.
 *********************************************************/
struct T {
        int width, height;
        int size, blocksize;
        UArray2_T blocks;
};

/********************** UArray2b_new **********************
 * Creates a new blocked 2D array.
 * 
 * Parameters:
 *      - int width: Width of the array in cells.
 *      - int height: Height of the array in cells.
 *      - int size: Size of each cell in bytes.
 *      - int blocksize: Block size, which is the square root of the 
 *                      number of cells per block.
 * 
 * Returns:
 *      - UArray2b_T: A new blocked 2D array.
 * 
 * Expects:
 *      - width, height, and size must be non-negative.
 *      - blocksize must be greater than 0.
 * 
 * Notes:
 *      - Will CRE if memory allocation fails.
 *********************************************************/
extern T UArray2b_new (int width, int height, int size, int blocksize)
{
        assert(width >= 0);
        assert(height >= 0); 
        assert(size >= 0); 
        assert(blocksize > 0);

        int num_blocks_width = (width + blocksize - 1) / blocksize;
        int num_blocks_height = (height + blocksize - 1) / blocksize;
        T uarray2_b = (T)malloc(sizeof(struct T));
        assert(uarray2_b != NULL);

        uarray2_b->width = width;
        uarray2_b->height = height;
        uarray2_b->size = size;
        uarray2_b->blocksize = blocksize;

        uarray2_b->blocks = UArray2_new(num_blocks_width, num_blocks_height, 
                                            sizeof(UArray_T));
        assert(uarray2_b->blocks != NULL);
        
        for (int i = 0; i < num_blocks_height; i++) {
                for (int j = 0; j < num_blocks_width; j++) {
                        UArray_T *block_p = UArray2_at(
                                uarray2_b->blocks, j, i);

                        *block_p = UArray_new(blocksize * blocksize, size);
                        assert(*block_p != NULL);
                }
        }
        
        return uarray2_b;
}

/****************** UArray2b_new_64K_block ****************
 * Creates a new blocked 2D array with the largest block size possible 
 * such that the block occupies at most 64KB.
 * 
 * Parameters:
 *      - int width: Width of the array in cells.
 *      - int height: Height of the array in cells.
 *      - int size: Size of each cell in bytes.
 * 
 * Returns:
 *      - UArray2b_T: A new blocked 2D array.
 * 
 * Expects:
 *      - width and height must be non-negative.
 *      - size must be greater than 0.
 * 
 * Notes:
 *      - Will CRE if memory allocation fails.
 *********************************************************/
extern T UArray2b_new_64K_block(int width, int height, int size)
{
        assert(width >= 0);
        assert(height >= 0);
        assert(size > 0);

        int blocksize;
        if (size > (1024 * 64)) {
                blocksize = 1;
        } 
        else {
                blocksize = (int)floor(sqrt(1024 * 64 / size));
        }
        
        return UArray2b_new(width, height, size, blocksize);
}

/******************** UArray2b_free ***********************
 * Frees all memory associated with a blocked 2D array.
 * 
 * Parameters:
 *      UArray2b_T *array2b: Pointer to the blocked 2D array.
 * 
 * Returns:
 *      None
 * 
 * Expects:
 *      array2b must not be NULL.
 * 
 * Notes:
 *      Will CRE if array2b is NULL.
 *********************************************************/
extern void UArray2b_free(T *array2b)
{
        assert(array2b != NULL);
        assert(*array2b != NULL);

        int num_blocks_width = UArray2_width((*array2b)->blocks);
        int num_blocks_height = UArray2_height((*array2b)->blocks);

        for (int i = 0; i < num_blocks_height; i++) {
                for (int j = 0; j < num_blocks_width; j++) {

                UArray_T *block_p = UArray2_at((*array2b)->blocks, j, i);
                assert(block_p != NULL);

                /*free single block*/
                UArray_free(block_p);
                }
        }

        T array = *array2b;
        UArray2_free(&array->blocks);
        free(*array2b);        
}

/******************** UArray2b_width **********************
 * Gets the width of the blocked 2D array.
 * 
 * Parameters:
 *      UArray2b_T array2b: Blocked 2D array.
 * 
 * Returns:
 *      int: The width of the array.
 * 
 * Expects:
 *      array2b must not be NULL.
 * 
 * Notes:
 *      Will CRE if array2b is NULL.
 *********************************************************/
extern int UArray2b_width (T array2b)
{
        assert(array2b != NULL);
        return array2b->width;
}

/******************** UArray2b_height *********************
 * Gets the height of the blocked 2D array.
 * 
 * Parameters:
 *      UArray2b_T array2b: Blocked 2D array.
 * 
 * Returns:
 *      int: The height of the array.
 * 
 * Expects:
 *      array2b must not be NULL.
 * 
 * Notes:
 *      Will CRE if array2b is NULL.
 *********************************************************/
extern int UArray2b_height (T array2b)
{
        assert(array2b != NULL);
        return array2b->height;
}

/********************* UArray2b_size **********************
 * Gets the size of each cell in the blocked 2D array.
 * 
 * Parameters:
 *      UArray2b_T array2b: Blocked 2D array.
 * 
 * Returns:
 *      int: Size of each cell in bytes.
 * 
 * Expects:
 *      array2b must not be NULL.
 * 
 * Notes:
 *      Will CRE if array2b is NULL.
 *********************************************************/
extern int UArray2b_size (T array2b)
{
        assert(array2b != NULL);
        return array2b->size;
}

/******************* UArray2b_blocksize *******************
 * Gets the block size of the blocked 2D array.
 * 
 * Parameters:
 *      UArray2b_T array2b: Blocked 2D array.
 * 
 * Returns:
 *      int: Block size, which is the number of cells per block side.
 * 
 * Expects:
 *      array2b must not be NULL.
 * 
 * Notes:
 *      Will CRE if array2b is NULL.
 *********************************************************/
extern int UArray2b_blocksize(T array2b)
{
        assert(array2b != NULL);
        return array2b->blocksize;
}

/*********************** UArray2b_at **********************
 * Returns a pointer to the cell at the given column and row.
 * 
 * Parameters:
 *      UArray2b_T array2b: Blocked 2D array.
 *      int column: Column index of the cell.
 *      int row: Row index of the cell.
 * 
 * Returns:
 *      Pointer to the element at the specified position.
 * 
 * Expects:
 *      column and row must be within valid bounds.
 *      array2b must not be NULL.
 * 
 * Notes:
 *      Will CRE if the column or row is out of bounds.
 *********************************************************/
extern void *UArray2b_at(T array2b, int column, int row)
{
        assert(array2b != NULL);
        assert(column >= 0);
        assert(row < array2b->height);
        assert(row >= 0);
        assert(column < array2b->width);

        int blocksize = array2b->blocksize;
        int b_row = row / blocksize;
        int b_col = column / blocksize;

        UArray_T linear_array = *(UArray_T *)UArray2_at
                                (array2b->blocks, b_col, b_row);
        assert(linear_array != NULL);

        int local_row = row % blocksize;
        int local_col = column % blocksize;
        int b_index = blocksize * local_row + local_col;

        void *pixel_p = UArray_at(linear_array, b_index);
        return pixel_p;
}

/********************* UArray2b_map ***********************
 * Applies a function to each element of the blocked 2D array. 
 * Visits all cells in one block before moving to another block.
 * 
 * Parameters:
 *      UArray2b_T array2b: Blocked 2D array.
 *      void apply(int col, int row, UArray2b_T array2b, 
 *                 void *elem, void *cl): Function to apply to each 
 *                 element.
 *      void *cl: Closure passed to the apply function.
 * 
 * Returns:
 *      None
 * 
 * Expects:
 *      array2b must not be NULL.
 * 
 * Notes:
 *      Will CRE if array2b is NULL.
 *********************************************************/
extern void  UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                                     void *elem, void *cl), void *cl)
{
        assert(array2b != NULL);

        int width = UArray2b_width(array2b);
        int height = UArray2b_height(array2b);
        int blocksize = UArray2b_blocksize(array2b);

        int num_blocks_width = (width + blocksize - 1) / blocksize;
        int num_blocks_height = (height + blocksize - 1) / blocksize;

        for (int b_row = 0; b_row < num_blocks_height; b_row++) {
                for (int b_col = 0; b_col < num_blocks_width; b_col++) {

                        /*get the block */
                        UArray_T *block_p = UArray2_at(array2b->blocks, 
                                                        b_col, b_row);
                        UArray_T block = *block_p;
                        assert(block != NULL);

                        /*iterate over the block */
                        int b_length = UArray_length(block);
                        for (int index = 0; index < b_length; index++) {

                                int global_col = b_col * blocksize + 
                                                (index % blocksize);
                                int global_row = b_row * blocksize + 
                                                (index / blocksize);


                                if (global_col < array2b->width && 
                                        global_row < array2b->height) {
                                        void *elem = UArray_at(block, index);
                                        apply(global_col, global_row, 
                                                  array2b, elem, cl); 
                                }
                        }
                }
        }
}
        
