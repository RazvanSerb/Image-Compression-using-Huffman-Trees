/* SERB Razvan-Gabriel -> 311CB */
#include "quadtree.h"
#define MAXI 101

void TASK_C1(int threshold, FILE *fin, FILE *fout) {
    char type_file[MAXI];
    fgets(type_file, MAXI, fin);  // tipul fisierului de intrare
    int m = 0, n = 0, mvc = 0;
    fscanf(fin, "%d", &m);  // latimea imaginii
    fscanf(fin, "%d", &n);  // inaltimea imaginii
    fscanf(fin, "%d", &mvc);  // valoare maxima culoare pixel
    fseek(fin, 1, SEEK_CUR);
    TPixel **img = InitializeImage(n, m);
    if (!img) exit(-1);
    DecryptImage(img, n, m, fin);
    // PrintImage(img, n, m, fout);
    int size = (m + n - abs(m - n)) / 2;
    TTree tree = InitializeImageTree();
    if (!tree) exit(-1);
    CreateImageTree(tree, img, 0, 0, size, threshold);
    // PrintImageTree(tree, 0, fout);
    fprintf(fout, "%d\n", NumLevelsImageTree(tree));
    fprintf(fout, "%d\n", NumLeafsImageTree(tree));
    fprintf(fout, "%d\n", FirstLeafDimension(tree));
    DestroyImageTree(&tree);
    DestroyImage(&img, &n, &m);
}

void TASK_C2(int threshold, FILE *fin, FILE *fout) {
    char type_file[MAXI];
    fgets(type_file, MAXI, fin);  // tipul fisierului de intrare
    int m = 0, n = 0, mvc = 0;
    fscanf(fin, "%d", &m);  // latimea imaginii
    fscanf(fin, "%d", &n);  // inaltimea imaginii
    fscanf(fin, "%d", &mvc);  // valoare maxima culoare pixel
    fseek(fin, 1, SEEK_CUR);
    TPixel **img = InitializeImage(n, m);
    if (!img) exit(-1);
    DecryptImage(img, n, m, fin);
    // PrintImage(img, n, m, fout);
    int size = (m + n - abs(m - n)) / 2;
    fwrite(&size, sizeof(unsigned int), 1, fout);
    TTree tree = InitializeImageTree();
    if (!tree) exit(-1);
    CreateImageTree(tree, img, 0, 0, size, threshold);
    // PrintImageTree(tree, 0, fout);
    CompressImageTree(tree, fout);
    DestroyImageTree(&tree);
    DestroyImage(&img, &n, &m);
}

void TASK_D(FILE *fin, FILE *fout) {
    int size = 0;
    fread(&size, sizeof(unsigned int), 1, fin);
    TTree tree = InitializeImageTree();
    if (!tree) exit(-1);
    DecompressImageTree(tree, size, fin);
    // PrintImageTree(tree, 0, fout);
    char type_file[MAXI] = "P6";
    fprintf(fout, "%s\n", type_file);
    int m = size, n = size;
    fprintf(fout, "%d %d\n", m, n);
    const int mvc = 255;
    fprintf(fout, "%d\n", mvc);
    TPixel **img = InitializeImage(n, m);
    if (!img) exit(-1);
    CreateImage(img, tree, 0, 0, size, fout);
    // PrintImage(img, n, m, fout);
    CryptImage(img, n, m, fout);
    DestroyImageTree(&tree);
    DestroyImage(&img, &n, &m);
}

int main(int argc, char const *argv[]) {
    FILE *fin = NULL; FILE *fout = NULL;
    if (!strcmp(argv[1], "-c1")) {
        int threshold = atoi(argv[2]);
        fin = fopen(argv[3], "rb");
        if (!fin) exit(-1);
        fout = fopen(argv[4], "wt");
        if (!fout) exit(-1);
        TASK_C1(threshold, fin, fout);
        fclose(fin);
        fclose(fout);
    }
    if (!strcmp(argv[1], "-c2")) {
        int threshold = atoi(argv[2]);
        fin = fopen(argv[3], "rb");
        if (!fin) exit(-1);
        fout = fopen(argv[4], "wb");
        if (!fout) exit(-1);
        TASK_C2(threshold, fin, fout);
        fclose(fin);
        fclose(fout);
    }
    if (!strcmp(argv[1], "-d")) {
        fin = fopen(argv[2], "rb");
        if (!fin) exit(-1);
        fout = fopen(argv[3], "w");
        if (!fout) exit(-1);
        TASK_D(fin, fout);
        fclose(fin);
        fclose(fout);
    }
    return 0;
}
