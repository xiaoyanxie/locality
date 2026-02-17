/*
 *  a2plain.c
 *  by Xiaoyan Xie (xxie05) and Diwei Chen (dchen22), 10-08-2024
 *  HW 3, Locality
 *
 *  The implementation of the A2Methods_UArray2 data structure that holds 
 *  2D array. 
 *  Adapt the UArray2 structure from hw2 solution to support 
 *  column-major and row-major mapping
 *  
 */

#include <string.h>
#include <a2plain.h>
#include "uarray2.h"


typedef A2Methods_UArray2 A2; /* private abbreviation */

/********** new() ********
 *
 * Creates a A2 data structure
 *
 * Parameters:
 *      int width: the number of columns in the array
 *      int height: the number of rows in the array
 *      int size: the size of a single cell in bytes
 * 
 * Return: the newly-created A2 data structure
 *
 * Expects:
 *      width, height, size should be non-negative, 
 * Notes:
 *      UArray2_new will handle CREs
 ***********************************************/
static A2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/********** new_with_blocksize() ********
 *
 * Creates a A2 data structure, ignore blocksize since 
 * this is not a blocked array
 *
 * Parameters:
 *      int width: the number of columns in the array
 *      int height: the number of rows in the array
 *      int size: the size of a single cell in bytes
 *      int blocksize: unused parameter
 * 
 * Return: the newly-created A2 data structure
 *
 * Expects:
 *      width, height, size should be non-negative, 
 * Notes:
 *      UArray2_new will handle CREs
 ***********************************************/
static A2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}

/********** a2free() ********
 *
 * Frees the memory allocated to the A2 data structure 
 *
 * Parameters:
 *      A2 *array2p: the A2 data structure
 * 
 * Return: none
 *
 * Expects:
 *      array2p should not be none
 * Notes:
 *      UArray2_free will handle CREs
 ***********************************************/
static void a2free(A2 *array2p)
{
        UArray2_free((UArray2_T *) array2p);
}

/********** width() ********
 *
 * Returns the width (number of columns) of the A2
 *
 * Parameters:
 *      A2 array2: the A2 data structure
 * 
 * Return: the width of A2
 *
 * Expects:
 *      array2 should not be none
 * Notes:
 *      UArray2_width will handle CREs
 ***********************************************/
static int width(A2 array2)
{
        return UArray2_width(array2);
}

/********** height() ********
 *
 * Returns the height (number of rows) of the A2
 *
 * Parameters:
 *      A2 array2: the A2 data structure
 * 
 * Return: the height of A2
 *
 * Expects:
 *      array2 should not be none
 * Notes:
 *      UArray2_height will handle CREs
 ***********************************************/
static int height(A2 array2)
{
        return UArray2_height(array2);
}

/********** size() ********
 *
 * Returns the size of each A2 element
 *
 * Parameters:
 *      A2 array2: the A2 data structure
 * 
 * Return: the element size of A2
 *
 * Expects:
 *      array2 should not be none
 * Notes:
 *      UArray2_size will handle CREs
 ***********************************************/
static int size(A2 array2)
{
        return UArray2_size(array2);
}

/********** blocksize() ********
 *
 * Returns 1, since this is not a blocked array
 *
 * Parameters:
 *      A2 array2: the A2 data structure
 * 
 * Return: 1
 *
 * Expects:
 *      none
 * Notes:
 *      none
 ***********************************************/
static int blocksize(A2 array2)
{
        (void)array2;
        return 1;
}

/********** at() ********
 *
 * Retrieves data from the A2 at the specified location
 *
 * Parameters:
 *      A2 array2: the A2 data structure
 *      int i: the column index
 *      int j: the row index
 *
 * Return: the pointer to the data from A2 at the (i, j) location
 *
 * Expects:
 *      integer parameters should be positive
 * Notes:
 *      UArray2_at will handle CREs
 ****************************************************/
static A2Methods_Object *at(A2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}

/* mapping functions */
/* each mapping function visits every cell in array2, and for each
* cell it calls 'apply' with these arguments:
*    i, the column index of the cell
*    j, the row index of the cell
*    array2, the array passed to the mapping function
*    cell, a pointer to the cell
*    cl, the closure pointer passed to the mapping function
*
* These functions differ only in the *order* they visit cells:
*   - row_major visits each row before the next, in order of
*     increasing row index; within a row, column numbers increase
*   - col_major visits each column before the next, in order of
*     increasing column index; within a column, row numbers increase
*   - block_major visits each block before the next; order of
*     blocks and order of cells within a block is not specified
*   - map_default uses a default order that has good locality
*
* In any record, map_block_major may be NULL provided that
* map_row_major and map_col_major are not NULL, and vice versa.
*/

/********** map_row_major() ********
 *
 * Iterate through the A2 in row major order
 * By row major order, you iterate it by height/row, then by width/column.
 * 
 * Parameters:
 *      A2Methods_UArray2 uarray2: the A2 data strcutre
 *                    void *apply: the function that iterates through A2 to 
 *                                 access and/or change values
 *                       void *cl: the closure pointer for the A2 data 
 *                                 structure
 *
 * Return: none
 *
 * Expects:
 *      A2 should not be null
 *      map_function should be function pointer
 * Notes:
 *      UArray2_map_row_major will handle CREs
 ************************/
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply, void *cl)
{
        UArray2_map_row_major(uarray2, (UArray2_applyfun*)apply, cl);
}

/********** map_col_major() ********
 *
 * Iterate through the A2 in column major order
 * By row major order, you iterate it by width/column, then by height/row.
 * 
 * Parameters:
 *      A2Methods_UArray2 uarray2: the A2 data strcutre
 *                    void *apply: the function that iterates through A2 to 
 *                                 access and/or change values
 *                       void *cl: the closure pointer for the A2 data 
 *                                 structure
 *
 * Return: none
 *
 * Expects:
 *      A2 should not be null
 *      map_function should be function pointer
 * Notes:
 *      UArray2_map_col_major will handle CREs
 ************************/
static void map_col_major(A2Methods_UArray2 uarray2,
                        A2Methods_applyfun apply, void *cl)
{
        UArray2_map_col_major(uarray2, (UArray2_applyfun*)apply, cl);
}


struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}


static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/********** uarray2_methods_plain_struct ********
 *
 * A structure defining methods for manipulating a 2D array.
 *
 * Return: A2Methods_T
 *
 * Expects:
 *      All function pointers should be initialized properly and not be NULL
 * Notes:
 *      This structure provides an interface for operations on a 2D array, 
 *      allowing various methods to be applied uniformly.
 */
static struct A2Methods_T uarray2_methods_plain_struct = {
        new,/*new*/
        new_with_blocksize,/*new_with_blocksize*/
        a2free,/*free*/
        width,/*width*/
        height,/*height*/
        size,/*size*/
        blocksize,/*blocksize*/
        at,/*at*/
        map_row_major,
        map_col_major,
        NULL,/*map_block_major*/
        map_row_major,/*map_default*/
        small_map_row_major,
        small_map_col_major,
        NULL,/*small_map_block_major*/
        small_map_row_major,/*small_map_default*/
};
A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
