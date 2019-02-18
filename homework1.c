#include <math.h>
#include <pthread.h>
#include "homework1.h"

int num_threads;
int resolution;

void initialize(image *im) {
    /*
     * Initializez de image to the wanted dimensions.
     * Makes the whole image white(255).
     */

    im->maxval = 255;
    im->width = (unsigned int) resolution;
    im->height = (unsigned int) resolution;
    im->array = malloc(sizeof(char *) * resolution);

    int i;
    for (i = 0; i < resolution; i++) {
        int j = 0;
        im->array[i] = malloc(sizeof(char) * resolution);
        for (j = 0; j < resolution; j++) {
            im->array[i][j] = 255;
        }
    }
}

/*
 * Struct for thread information
 * passes the id of the thread,
 * the image and the centimeters per pixel.*/

typedef struct {
    int id;
    image *img;
    float cm;
} t_info;


/*
 * Thread function that computes the output
 * image for i from first to last,
 * splitting the array by the first coordinate.
 */
void *threadResize(void *var) {
    int i, j;
    t_info info = *(t_info *) var;
    int thread_id = info.id;
    image *im = info.img;
    float pixelCentimeter = info.cm;

    /*Compute first and last*/
    int len = resolution / num_threads;
    int first = len * thread_id;w
    int last;

    if (thread_id == num_threads - 1)
        last = resolution;
    else
        last = len * (thread_id + 1);

    for (i = first; i < last; i++) {
        for (j = 0; j < resolution; j++) {
            /*Calculate point coordinates*/
            float y = (resolution - i - 1) * pixelCentimeter + pixelCentimeter / 2;
            float x = j * pixelCentimeter + pixelCentimeter / 2;

            /*Calulate ecuation,
             * difference in abs and
             * then the distance by
             * the formula*/
            float ec = -x + 2 * y;
            float diff = abs((int) ec);
            float distance = diff / sqrtf(2 * 2 + 1 * 1);

            if (distance <= 3)
                im->array[i][j] = 0;
            else
                im->array[i][j] = 255;
        }
    }

}

/*
 * Function that calls the thread functions
 * and initializes their information.
 */
void render(image *im) {
    int i;
    float pixelCentimeter = 100 / (float) resolution;
    pthread_t tid[num_threads];
    t_info info[num_threads];
    for (i = 0; i < num_threads; i++) {
        info[i].id = i;
        info[i].img = im;
        info[i].cm = pixelCentimeter;
    }
    for (i = 0; i < num_threads; i++) {
        pthread_create(&(tid[i]), NULL, threadResize, &(info[i]));
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }

}

/*
 * Function that writes back the
 * image to output.
 */
void writeData(const char *fileName, image *img) {
    FILE *fp = fopen(fileName, "wb");
    int P = 5;
    fprintf(fp, "P%d\n", P); // NOLINT

    fprintf(fp, "%d %d\n", resolution, resolution); // NOLINT
    fprintf(fp, "%d\n", 255); // NOLINT

    int i;
    for (i = 0; i < resolution; i++) {
        fwrite(img->array[i], 1, (size_t) resolution, fp);
        free(img->array[i]);
    }

    free(img->array);
    fclose(fp);
}

