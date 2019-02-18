#include "homework.h"

int num_threads;
int resize_factor;
int gaussKernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
};

unsigned char resizePixelBW(unsigned char **img, int i, int j) ;

pixel resizePixelColor(pixel **img, int i, int j);

/* Reads the input and allocates the memory
 * for the image array. Reads line by line.*/

void readInput(const char * fileName, image *img){
    FILE *fp = fopen(fileName, "rb");
    int P;
    fscanf(fp, "P%d\n", &P);

    if (P == 5)img->color = false;
    else if (P == 6)img->color = true;
    else return;

    fscanf(fp, "%d %d\n", &img->width, &img->height);
    fscanf(fp, "%d\n", &img->maxval);

    if (!img->color) {
        img->array = malloc(sizeof(char*) * img->height);
        int i;
        for(i = 0;i < img->height;i++) {
            img->array[i] = malloc(sizeof(char) * img->width);
            fread(img->array[i], sizeof(char), img->width, fp);
        }
    } else {
        img->array = malloc(sizeof(pixel*) * img->height);
        int i;
        for(i = 0;i < img->height;i++) {
            img->array[i] = malloc(sizeof(pixel) * img->width);
            fread(img->array[i], sizeof(pixel), img->width, fp);
        }
    }
    fclose(fp);
}

/*Writes back the image line by line */
void writeData(const char * fileName, image *img) {
    FILE *fp = fopen(fileName, "wb");
    int P;
    if (img->color == false)P=5;
    else P=6;
    fprintf(fp, "P%d\n", P); // NOLINT

    fprintf(fp, "%d %d\n", img->width, img->height); // NOLINT
    fprintf(fp, "%d\n", img->maxval); // NOLINT

    if (!img->color) {
        int i;
        for(i = 0;i < img->height;i++) {
            fwrite(img->array[i], 1, img->width, fp);
            free(img->array[i]);
        }
    } else {
        int i;
        for(i = 0;i < img->height;i++) {
            fwrite(img->array[i], 1, img->width * 3, fp);
            free(img->array[i]);
        }
    }
    free(img->array);
    fclose(fp);
}

/*Function that computes point by point
 * split by the first coordinate*/
void* threadResize(void *var)
{
    int i;
    t_info info = *(t_info*)var;
    int thread_id = info.id;
    image *in = info.in;
    image *out = info.out;


    /*
     * Calculate first and last indexes
     */
    int len = out->height/num_threads;
    int first = len*thread_id;
    int last;
    if(thread_id==num_threads-1)
        last = out->height;
    else
        last = len*(thread_id+1);

    if (!out->color) {
        unsigned char **img = (unsigned char**)(in->array);
        for(i = first;i < last;i++) {
            out->array[i] = malloc(sizeof(char) * out->width);
            unsigned char *outImgRow = (unsigned char*)(out->array[i]);
            int j = 0;
            for( j = 0 ;j < out->width;j++){
                outImgRow[j] = resizePixelBW(img, i ,j);
            }
        }
    } else {
        pixel **img = (pixel**)(in->array);
        for(i = first;i < last;i++) {
            out->array[i] = malloc(sizeof(pixel) * out->width);
            pixel *outImgRow = (pixel*)(out->array[i]);
            int j = 0;
            for( j = 0 ;j < out->width;j++){
                outImgRow[j] = resizePixelColor(img, i ,j);
            }
        }
    }

}


/*
 * Calls the thread functions
 * and initializes the memory for
 * the output image.
 */
void resize(image *in, image * out) {

    *out = *in;
    out->width = in->width/resize_factor;
    out->height = out->height/resize_factor;

    if (!out->color) {
        out->array = malloc(sizeof(char*) * out->height);
    } else {
        out->array = malloc(sizeof(pixel*) * out->height);
    }

    int i;
    pthread_t tid[num_threads];
    t_info info[num_threads];
    for(i = 0;i < num_threads; i++) {
        info[i].id = i;
        info[i].in = in;
        info[i].out = out;
    }

    for(i = 0; i < num_threads; i++) {
        pthread_create(&(tid[i]), NULL, threadResize, &(info[i]));
    }

    for(i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }
}

/*
 * Gets the coordinates of a point from the resized output
 * and the original image.
 * It uses the resize*resize 2d array to compute
 * the resulting pixel.
 */
pixel resizePixelColor(pixel **img, int i, int j) {
    typedef struct{
        unsigned int r, g, b;
    }pixelSum;
    pixelSum sum;

    sum.r = 0;
    sum.g = 0;
    sum.b = 0;

    int k, l;
    for(k = i*resize_factor; k < (i+1)*resize_factor; k++){
        for(l = j*resize_factor; l < (j+1)*resize_factor; l++){
            if(resize_factor == 3){
                sum.r += img[k][l].r*gaussKernel[l - j * resize_factor][k - i * resize_factor];
                sum.g += img[k][l].g*gaussKernel[l - j * resize_factor][k - i * resize_factor];
                sum.b += img[k][l].b*gaussKernel[l - j * resize_factor][k - i * resize_factor];
            }else {
                sum.r += img[k][l].r;
                sum.g += img[k][l].g;
                sum.b += img[k][l].b;
            }
        }
    }
    int divide = 16;
    if(resize_factor != 3) {
        divide = (resize_factor * resize_factor);
    }

    pixel result;
    result.r = (unsigned char) (sum.r / divide);
    result.g = (unsigned char) (sum.g / divide);
    result.b = (unsigned char) (sum.b / divide);
    return result;
}

/*
 * Same as the pixel one only this one
 * is for bw pixel so only one byte and
 * sum.
 */
unsigned char resizePixelBW(unsigned char **img, int i, int j) {
    unsigned int sum = 0;
    int k, l;
    for(k = i*resize_factor; k < (i+1)*resize_factor; k++){
        for(l = j*resize_factor; l < (j+1)*resize_factor; l++){
            if(resize_factor == 3)
                sum += img[k][l]*gaussKernel[l-j*resize_factor][k - i * resize_factor];
            else sum += img[k][l];
        }
    }
    int divide = 16;
    if(resize_factor != 3) {
        divide = (resize_factor * resize_factor);
    }

    int value = sum / divide;
    return (unsigned char)value;
}