/**************************************************************
 *
 *      ppmtrans.c
 *
 *      Xiaoyan Xie (xxie05)
 *      Diwei Chen (dchen22)
 *      Oct 09 2024
 *      
 *      CS 40 HW03 - locality
 *
 *      This file provides a program for reading and rotating PPM images 
 *      by specified angles using a two-dimensional array structure, with 
 *      options for different mapping methods and performance timing.
 *
 **************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"
#include <pnmrdr.h>


typedef A2Methods_UArray2 A2;
typedef A2Methods_T A;
typedef A2Methods_mapfun Am;

/******************* struct closure *********************
 * Stores the rotated image and methods for mapping over
 * arrays during image rotation.
 * Used to pass necessary data during the rotation process.
 *********************************************************/
struct closure {
        A2 rotated_img;
        A methods;
};

/********************* Function Declarations *********************
 ***************************************************************/
void ppm_process(A methods, A2 src_array, 
        Am *map, char *time_file, int degree);

void handle_rotate(A2 src_array, A2 rotated_img, A methods,
        Am *map, FILE *time_file, int degree);

void rotate90(int col, int row, A2 src_array, void *el, void *cl);
void rotate180(int col, int row, A2 src_array, void *el, void *cl);
void rotate270(int col, int row, A2 src_array, void *el, void *cl);

Pnm_ppm image;

/********************** Method Setter Macro ******************
 ***************************************************************/
#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (false)

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] "
                        "[-time time_file] "
                        "[filename]\n",
                        progname);
        exit(1);
}

/************************ main **************************
 * Entry point for the image rotation program. Processes
 * command-line arguments, reads a PPM image, applies the 
 * specified rotation, and writes the result.
 * 
 * Parameters:
 *      int argc: Number of command-line arguments.
 *      char *argv[]: Array of command-line arguments.
 * 
 * Returns:
 *      int: Exit status of the program (0 on success).
 * 
 * Expects:
 *      argv contains valid arguments.
 * 
 * Notes:
 *      Will print usage and exit if arguments are invalid.
 *********************************************************/
int main(int argc, char *argv[])
{
        char *time_file_name = NULL;
        int   rotation       = 0;
        int   i;

        /* default to UArray2 methods */
        A methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* default to best map */
        Am *map = methods->map_default;
        assert(map != NULL);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        if (!(i + 1 < argc)) {      /* no time file */
                                usage(argv[0]);
                        }
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                        usage(argv[0]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }

        FILE *fp;
        if (i < argc) {
                fp = fopen(argv[i], "r");
                if (fp == NULL) {
                        fprintf(stderr, "Fail to open file.\n");
                        exit(0);
                }
        } else {
                fp = stdin;
        }

        image = Pnm_ppmread(fp, methods);
        assert(image != NULL);
        ppm_process(methods, image->pixels, map, time_file_name, rotation);

        fclose(fp);
        Pnm_ppmfree(&image);
        return EXIT_SUCCESS;
}

/********************** ppm_process ***********************
 * Processes and rotates a PPM image by a given degree.
 * Driver function for rotations.
 * 
 * Parameters:
 *      A methods: 2D array handling methods.
 *      A2 src_array: Source image pixels.
 *      Am *map: Mapping function.
 *      char *time_file: Optional file for timing info.
 *      int degree: Rotation angle (0, 90, 180, 270).
 * 
 * Returns:
 *      None
 * 
 * Expects:
 *      src_array must not be NULL.
 *********************************************************/
void ppm_process(A methods, A2 src_array, Am *map, char *time_file, int degree)
{       
        FILE *fp = NULL;
        if (!(time_file == NULL)) {
                fp = fopen(time_file, "w");
                if (fp == NULL) {
                        fprintf(stderr, "Fail to open file.\n");
                        return;
                }
        }

        int new_width;
        int new_height;

        /*get new dimention*/
        if (degree == 90 || degree == 270) {
                new_width = methods->height(src_array);
                new_height = methods->width(src_array);
        } else if (degree == 180 || degree == 0) {
                new_width = methods->width(src_array);
                new_height = methods->height(src_array);
        } 
        else {/*if no degree is provided*/
                new_width = methods->width(src_array);
                new_height = methods->height(src_array);
        }

        A2 rotated_img = methods->new(new_width, new_height,
        methods->size(src_array));
        assert(rotated_img != NULL);
        
        if (degree == 90 || degree == 180 || degree == 270) {
                handle_rotate(src_array, rotated_img, methods, map, fp, degree); 
                
        }
        else {/*if there is no rotation, directly print*/
                CPUTime_T timer = CPUTime_New();
                CPUTime_Start(timer);
                double time_used = CPUTime_Stop(timer);
                if (time_file != NULL) {
                        fprintf(fp, "Rotation finished in %.0f nanoseconds\n",
                        time_used); 
                }
                CPUTime_Free(&timer);
        }

        Pnm_ppmwrite(stdout, image);
        if (fp != NULL) {
                fclose(fp);
        }
}       

/******************** handle_rotate ***********************
 * Rotates the image and updates its dimensions.
 * 
 * Parameters:
 *      A2 src_array: Source image pixels.
 *      A2 rotated_img: Destination for rotated image.
 *      A methods: 2D array handling methods.
 *      Am *map: Mapping function.
 *      FILE *time_file: Optional file for timing info.
 *      int degree: Rotation angle (90, 180, 270).
 * 
 * Returns:
 *      None
 * 
 * Expects:
 *      src_array and rotated_img must not be NULL.
 *********************************************************/
void handle_rotate(A2 src_array, A2 rotated_img, A methods, Am *map, FILE *time_file, int degree)
{       
        struct closure *new_cl = malloc(sizeof(struct closure)); 
        assert(new_cl != NULL);
        new_cl->methods = methods;
        new_cl->rotated_img = rotated_img;

        CPUTime_T timer = CPUTime_New();
        CPUTime_Start(timer);

        /*call map function and roate with apply functions */
        if (degree == 90) {
                map(src_array, rotate90, new_cl);
        } else if (degree == 180) {
                map(src_array, rotate180, new_cl);
        } else if (degree == 270) {
                map(src_array, rotate270, new_cl);
        } 
        
        /*update the image dimensions if rotation is 90 or 270 */
        if (degree == 90 || degree == 270) {
                image->height = methods->height(new_cl->rotated_img);
                image->width = methods->width(new_cl->rotated_img);
        }

        /*free old image pixels and assign new rotated image */
        methods->free(&image->pixels);
        image->pixels = new_cl->rotated_img;

        double time_used = CPUTime_Stop(timer);
        if (time_file != NULL) {
                fprintf(time_file, "Rotation finished in %.0f nanoseconds\n",
                time_used); 
        }
        
        CPUTime_Free(&timer);
        free(new_cl);
}

/********************** rotate90 *************************
 * Rotates the image by 90 degrees.
 * 
 * Parameters:
 *      int col: Column index.
 *      int row: Row index.
 *      A2 src_array: Source image pixels.
 *      void *el: element (Current pixel).
 *      void *cl: Closure for rotated image and methods.
 * 
 * Returns:
 *      None
 * 
 * Expects:
 *      src_array and closure must not be NULL.
 *********************************************************/
void rotate90(int col, int row, A2 src_array, void *el, void *cl)
{
        struct closure *new_cl = cl;

        A2 rotated_img = new_cl->rotated_img;
        A new_methods = new_cl->methods;

        int height = new_methods->height(src_array);
        *(struct Pnm_rgb *)new_methods->
        at(rotated_img, height - row - 1, col) = *(struct Pnm_rgb *)el;
}

/********************** rotate180 ************************
 * Rotates the image by 180 degrees.
 * 
 * Parameters:
 *      int col: Column index.
 *      int row: Row index.
 *      A2 src_array: Source image pixels.
 *      void *el: element (Current pixel).
 *      void *cl: Closure for rotated image and methods.
 * 
 * Returns:
 *      None
 * 
 * Expects:
 *      src_array and closure must not be NULL.
 *********************************************************/
void rotate180(int col, int row, A2 src_array, void *el, void *cl)
{
        struct closure *new_cl = cl;

        A2 rotated_img = new_cl->rotated_img;
        A new_methods = new_cl->methods;

        int height = new_methods->height(src_array);
        int width = new_methods->width(src_array);
        *(struct Pnm_rgb *)new_methods->at(rotated_img, width - col - 1, 
                height - row - 1) = *(struct Pnm_rgb *)el;
}

/********************** rotate270 ************************
 * Rotates the image by 270 degrees.
 * 
 * Parameters:
 *      int col: Column index.
 *      int row: Row index.
 *      A2 src_array: Source image pixels.
 *      void *el: element (Current pixel).
 *      void *cl: Closure for rotated image and methods.
 * 
 * Returns:
 *      None
 * 
 * Expects:
 *      src_array and closure must not be NULL.
 *********************************************************/
void rotate270(int col, int row, A2 src_array, void *el, void *cl)
{
        struct closure *new_cl = cl;

        A2 rotated_img = new_cl->rotated_img;
        A new_methods = new_cl->methods;

        int width = new_methods->width(src_array);
        *(struct Pnm_rgb *)new_methods->
        at(rotated_img, row, width - col - 1) = *(struct Pnm_rgb *)el;
}

