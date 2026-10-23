#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile double result;

int main(int argc, char **argv)
{
    size_t bytes;

    if (argc != 2) {
        fprintf(stderr, "usage: %s small|large\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "small") == 0)
        bytes = 32 * 1024;
    else if (strcmp(argv[1], "large") == 0)
        bytes = 64 * 1024 * 1024;
    else {
        fprintf(stderr, "dataset must be small or large\n");
        return EXIT_FAILURE;
    }

    size_t elements = bytes / sizeof(double);
    size_t passes = (16ULL * 1024 * 1024 * 1024) / bytes;
    double *data = malloc(bytes);
    double total = 0.0;

    if (data == NULL)
        return EXIT_FAILURE;

    for (size_t i = 0; i < elements; ++i)
        data[i] = 1.0 + (double)(i % 1024) / 1000000.0;

    for (size_t pass = 0; pass < passes; ++pass) {
        double subtotal = 0.0;

        for (size_t i = 0; i < elements; ++i)
            subtotal += data[i];

        data[pass % elements] += 1e-12;
        total += subtotal;
    }

    result = total;
    free(data);
    return EXIT_SUCCESS;
}
