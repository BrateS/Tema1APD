#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

typedef char bool;
#define true 1
#define false 0

typedef struct {
    unsigned char r,g,b;
}pixel;

typedef struct {
    bool color;
    unsigned int width, height;
    unsigned int maxval;
    void **array;
}image;

typedef struct{
    int id;
    image *in;
    image *out;
}t_info;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */