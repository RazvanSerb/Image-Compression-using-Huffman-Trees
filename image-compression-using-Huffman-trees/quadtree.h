/* SERB Razvan-Gabriel -> 311CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TPixel: Structura folosita pentru a retine culorile care formeaza un pixel */
typedef struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} TPixel;

/* *TTree: Structura folosita pentru a retine arborele cuaternar al imaginii */
typedef struct tree {
    unsigned char type;
    unsigned int dim;
    TPixel color;
    struct tree *son1;
    struct tree *son2;
    struct tree *son3;
    struct tree *son4;
} TNode, *TTree;

/* *TQueue: Lista dublu inlantuita circulara cu santinela folosita pentru coada */
typedef struct cellQueue {
    TTree info;
    struct cellQueue *prev;
    struct cellQueue *next;
} TCellQueue, *TQueue;

TPixel** InitializeImage(int n, int m);
void DecryptImage(TPixel **img, int n, int m, FILE *fin);
void CryptImage(TPixel **img, int n, int m, FILE *fout);
void PrintImage(TPixel **img, int n, int m, FILE *fout);
void DestroyImage(TPixel ***img, int *n, int *m);
TTree AllocateNode(unsigned char type, unsigned int dim, TPixel color);
TTree InitializeImageTree();
void CreateImageTree(TTree arb, TPixel **img, int x, int y, int size, int prag);
void CreateImage(TPixel **img, TTree arb, int x, int y, int size, FILE *fout);
void PrintImageTree(TTree arb, int nivel, FILE *fout);
int NumLevelsImageTree(TTree arb);
int NumLeafsImageTree(TTree arb);
unsigned int FirstLeafDimension(TTree arb);
void DestroyImageTree(TTree *arb);
TQueue InitializeQueue();
int InsertQueue(TQueue coada, TTree info);
int ExtractQueue(TQueue coada, TTree *info);
void PrintQueue(TQueue coada, FILE *fout);
void DestroyQueue(TQueue* coada);
void CompressImageTree(TTree arb, FILE *fout);
void LoadDimensionForDecompressedImageTree(TTree tree, int size);
void DecompressImageTree(TTree tree, int size, FILE *fin);
