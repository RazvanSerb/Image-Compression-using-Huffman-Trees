#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *in = fopen("compressed.out", "rb");
    if (!in) exit(-1);
    FILE *out = fopen("quadtree.out", "wt");
    if (!out) exit(-1);
    unsigned int dim = 0;
    unsigned char type = 0, val = 0;
    fread(&dim, sizeof(unsigned int), 1, in);
    fprintf(out, "%d\n", dim);
    while (fread(&type, sizeof(unsigned char), 1, in)) {
        fprintf(out, "{%d", type);
        if (type == 1) {
            fread(&val, sizeof(unsigned char), 1, in);
            fprintf(out, ", (%d", val);
            fread(&val, sizeof(unsigned char), 1, in);
            fprintf(out, "|%d", val);
            fread(&val, sizeof(unsigned char), 1, in);
            fprintf(out, "|%d)", val);
        }
        fprintf(out, "}\n");
    }
    fclose(in);
    fclose(out);
    return 0;
}
