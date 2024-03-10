/* SERB Razvan-Gabriel -> 311CB */
#include "quadtree.h"

/* InitializeImage:
- functie ce initializeaza o matrice de pixeli
- intoarce matricea daca s-a reusit initializarea sau NULL daca nu s-a reusit */
TPixel** InitializeImage(int n, int m) {
    TPixel** img = (TPixel **)malloc(n * sizeof(TPixel*));
    if (!img)
        return NULL;
    int i = 0;
    for (i = 0; i < n; i++) {
        img[i] = (TPixel *)malloc(m * sizeof(TPixel));
        if (!(img[i])) {
            int j = 0;
            for (j = 0; j < i; j++)
                free(img[j]);
            free(img);
            return NULL;
        }
    }
    return img;
}

/* DecryptImage:
- functie ce citeste o matrice de pixeli dintr-un fisier de intrare binar */
void DecryptImage(TPixel **img, int n, int m, FILE *fin) {
    int i = 0;
    for (i = 0; i < n; i++) {
        int j = 0;
        for (j = 0; j < m; j++) {
            TPixel color;
            fread(&(color.red), sizeof(unsigned char), 1, fin);
            fread(&(color.green), sizeof(unsigned char), 1, fin);
            fread(&(color.blue), sizeof(unsigned char), 1, fin);
            img[i][j] = color;
        }
    }
}

/* CryptImage:
- functie ce afiseaza o matrice de pixeli intr-un fisier de iesire binar */
void CryptImage(TPixel **img, int n, int m, FILE *fout) {
    int i = 0;
    for (i = 0; i < n; i++) {
        int j = 0;
        for (j = 0; j < m; j++) {
            TPixel color = img[i][j];
            fwrite(&(color.red), sizeof(unsigned char), 1, fout);
            fwrite(&(color.green), sizeof(unsigned char), 1, fout);
            fwrite(&(color.blue), sizeof(unsigned char), 1, fout);
        }
    }
}

/* PrintImage:
- functie ce afiseaza o matrice de pixeli intr-un fisier de iesire text */
void PrintImage(TPixel **img, int n, int m, FILE *fout) {
    fprintf(fout, "Image Dimension: %dx%d\n", m, n);
    int i = 0;
    for (i = 0; i < n; i++) {
        int j = 0;
        for (j = 0; j < m; j++) {
            fprintf(fout, "{%u|", img[i][j].red);
            fprintf(fout, "%u|", img[i][j].green);
            fprintf(fout, "%u} ", img[i][j].blue);
        }
        fprintf(fout, "\n");
    }
}

/* DestroyImage:
- functie ce distruge o matrice de pixeli */
void DestroyImage(TPixel ***img, int *n, int *m) {
    if ((*img) == NULL)
        return;
    int i = 0;
    for (i = 0; i < (*n); i++)
        free((*img)[i]);
    free(*img);
    *img = NULL;
    *m = 0;
    *n = 0;
}

/* AllocateNode:
- functie ce aloca un nod de tip frunza pentru un arbore cuaternar */
TTree AllocateNode(unsigned char type, unsigned int dim, TPixel color) {
    TTree node = (TTree)malloc(sizeof(TNode));
    if (!node)
        return NULL;
    node->type = type;
    node->dim = dim;
    node->color = color;
    node->son1 = NULL;
    node->son2 = NULL;
    node->son3 = NULL;
    node->son4 = NULL;
    return node;
}

/* InitializeImageTree:
- functie ce initializeaza un arbore cuaternar
- intoarce arborele daca s-a reusit initializarea sau NULL daca nu s-a reusit */
TTree InitializeImageTree() {
    TPixel color; color.red = 0; color.green = 0; color.blue = 0;
    TTree tree = AllocateNode(0, 0, color);
    if (!tree)
        return NULL;
    return tree;
}

/* CreateImageTree:
- functie ce completeaza un arbore cuaternar in functie de o imagine */
void CreateImageTree(TTree tree, TPixel **img, int x, int y, int size, int threshold) {
    unsigned long long s_red = 0, s_green = 0, s_blue = 0;
    int i = 0;
    for (i = x; i < x+size; i++) {
        int j = 0;
        for (j = y; j < y+size; j++) {
            unsigned long long aux = 0;
            aux = img[i][j].red;
            s_red = s_red + aux;
            aux = img[i][j].green;
            s_green = s_green + aux;
            aux = img[i][j].blue;
            s_blue = s_blue + aux;
        }
    }
    TPixel average;
    average.red = s_red/(size * size);
    average.green = s_green/(size * size);
    average.blue = s_blue/(size * size);
    unsigned long long s_average = 0;
    i = 0;
    for (i = x; i < x+size; i++) {
        int j = 0;
        for (j = y; j < y+size; j++) {
            unsigned long long aux = 0;
            aux = img[i][j].red - average.red;
            s_average = s_average + (aux * aux);
            aux = img[i][j].green - average.green;
            s_average = s_average + (aux * aux);
            aux = img[i][j].blue - average.blue;
            s_average = s_average + (aux * aux);;
        }
    }
    unsigned long long mean = s_average/(3 * size * size);
    if (mean <= threshold) {
        // completez informatiile pentru nodul de type frunza
        tree->type = 1;
        tree->dim = size;
        tree->color = average;
    } else {
        // aloc fii pentru nodul de type intermediar
        TPixel color; color.red = 0; color.green = 0; color.blue = 0;
        tree->son1 = AllocateNode(0, 0, color);
        if (!(tree->son1)) exit(-1);
        tree->son2 = AllocateNode(0, 0, color);
        if (!(tree->son2)) exit(-1);
        tree->son3 = AllocateNode(0, 0, color);
        if (!(tree->son3)) exit(-1);
        tree->son4 = AllocateNode(0, 0, color);
        if (!(tree->son4)) exit(-1);
        // apelez recursiv functia pentru fiecare fiu al nodului de type intermediar
        CreateImageTree(tree->son1, img, x, y, size / 2, threshold);
        CreateImageTree(tree->son2, img, x, y + size / 2, size / 2, threshold);
        CreateImageTree(tree->son3, img, x + size / 2, y + size / 2, size / 2, threshold);
        CreateImageTree(tree->son4, img, x + size / 2, y, size / 2, threshold);
    }
}

/* CreateImage:
- functie ce completeaza o imagine in functie de un arbore cuaternar */
void CreateImage(TPixel **img, TTree tree, int x, int y, int size, FILE *fout) {
    if (tree->type) {
        // completez zona din matrice corespunzatoare nodului de type frunza
        int i = 0;
        for (i = x; i < x + size; i++) {
            int j = 0;
            for (j = y; j < y + size; j++)
                img[i][j] = tree->color;
        }
    } else {
        // apelez recursiv functia pentru fiecare fiu al nodului de type intermediar
        CreateImage(img, tree->son1, x, y, size / 2, fout);
        CreateImage(img, tree->son2, x, y + size / 2, size / 2, fout);
        CreateImage(img, tree->son3, x + size / 2, y + size / 2, size / 2, fout);
        CreateImage(img, tree->son4, x + size / 2, y, size / 2, fout);
    }
}

/* PrintImageTree:
- functie ce afiseaza continutul unui arbore cuaternar intr-un fisier de iesire text */
void PrintImageTree(TTree tree, int level, FILE *fout) {
    if (!tree)
        return;
    fprintf(fout, "(%d, ", tree->type);
    fprintf(fout, "%d, ", tree->dim);
    fprintf(fout, "{%d|", tree->color.red);
    fprintf(fout, "%d|", tree->color.green);
    fprintf(fout, "%d})\n", tree->color.blue);
    if ((tree->son1) && (tree->son2) && (tree->son3) && (tree->son4)) {
        int i = 0;
        for (i = 0; i < level; i++)
            fprintf(fout, "\t");
        fprintf(fout, "[\n");
        for (i = 0; i <= level; i++)
            fprintf(fout, "\t");
        PrintImageTree(tree->son1, level + 1, fout);
        for (i = 0; i <= level; i++)
            fprintf(fout, "\t");
        PrintImageTree(tree->son2, level + 1, fout);
        for (i = 0; i <= level; i++)
            fprintf(fout, "\t");
        PrintImageTree(tree->son3, level + 1, fout);
        for (i = 0; i <= level; i++)
            fprintf(fout, "\t");
        PrintImageTree(tree->son4, level + 1, fout);
        for (i = 0; i < level; i++)
            fprintf(fout, "\t");
        fprintf(fout, "]\n");
    }
}

/* NumLevelsImageTree:
- functie ce returneaza numarul de niveluri dintr-un arbore cuaternar */
int NumLevelsImageTree(TTree tree) {
    if (!tree)
        return 0;
    int s1 = 1 + NumLevelsImageTree(tree->son1);
    int s2 = 1 + NumLevelsImageTree(tree->son2);
    int s3 = 1 + NumLevelsImageTree(tree->son3);
    int s4 = 1 + NumLevelsImageTree(tree->son4);
    int max1 = (s1 + s2 + abs(s1 - s2)) / 2;
    int max2 = (s3 + s4 + abs(s3 - s4)) / 2;
    return ((max1 + max2 + abs(max1 - max2)) / 2);
}

/* NumLeafsImageTree:
- functie ce returneaza numarul de frunze dintr-un arbore cuaternar */
int NumLeafsImageTree(TTree tree) {
    if (!(tree->son1) && !(tree->son2) && !(tree->son3) && !(tree->son4))
        return 1;
    int s = 0;
    s = s + NumLeafsImageTree(tree->son1);
    s = s + NumLeafsImageTree(tree->son2);
    s = s + NumLeafsImageTree(tree->son3);
    s = s + NumLeafsImageTree(tree->son4);
    return s;
}

/* FirstLeafDimension:
- functie ce returneaza dimensiunea maxima dinr-o frunza */
unsigned int FirstLeafDimension(TTree tree) {
    if (!(tree->son1) && !(tree->son2) && !(tree->son3) && !(tree->son4))
        return tree->dim;
    unsigned int dim1 = FirstLeafDimension(tree->son1);
    unsigned int dim2 = FirstLeafDimension(tree->son2);
    unsigned int dim3 = FirstLeafDimension(tree->son3);
    unsigned int dim4 = FirstLeafDimension(tree->son4);
    unsigned int max1 = (dim1 + dim2 + abs((int)(dim1 - dim2))) / 2;
    unsigned int max2 = (dim3 + dim4 + abs((int)(dim3 - dim4))) / 2;
    return ((max1 + max2 + abs((int)(max1 - max2))) / 2);
}

/* DestroyImageTree:
- functie ce distruge un arbore cuaternar */
void DestroyImageTree(TTree *tree) {
    while (*tree) {
        if (!((*tree)->son1) && !((*tree)->son2) && !((*tree)->son3) && !((*tree)->son4)) {
            free(*tree);
            *tree = NULL;
            return;
        }
        DestroyImageTree(&((*tree)->son1));
        DestroyImageTree(&((*tree)->son2));
        DestroyImageTree(&((*tree)->son3));
        DestroyImageTree(&((*tree)->son4));
    }
}

/* InitializeQueue:
- functie ce initializeaza o coada
- intoarce coada daca s-a reusit initializarea sau NULL daca nu s-a reusit */
TQueue InitializeQueue() {
    TQueue queue = (TQueue) malloc(sizeof(TCellQueue));
    if (!queue)
        return NULL;
    queue->info = NULL;
    queue->prev = queue;
    queue->next = queue;
    return queue;
}

/* InsertQueue:
- functie ce insereaza un pointer la un nod in coada */
int InsertQueue(TQueue queue, TTree info) {
    TQueue aux = (TQueue) malloc(sizeof(TCellQueue));
    if (!(aux))
        return 0;
    aux->info = info;
    aux->prev = queue->prev;
    aux->next = queue;
    queue->prev = aux;
    aux->prev->next = aux;
    return 1;
}

/* ExtractQueue:
- functie ce extrage un pointer la un nod din coada */
int ExtractQueue(TQueue queue, TTree *info) {
    if (queue->next == queue)
        return 0;
    *info = queue->next->info;
    TQueue aux = queue->next;
    queue->next = queue->next->next;
    queue->next->prev = queue;
    free(aux);
    return 1;
}

/* PrintQueue:
- functie ce afiseaza continutul din coada */
void PrintQueue(TQueue queue, FILE *fout) {
    fprintf(fout, "QUEUE: ");
    TQueue sentinel = queue;
    for (queue = queue->next; queue != sentinel; queue = queue->next)
        fprintf(fout, "%d ", queue->info->type);
    fprintf(fout, "\n");
}

/* DestroyQueue:
- functie ce distruge o coada */
void DestroyQueue(TQueue* queue) {
    if (!(*queue))
        return;
    TQueue sentinel = (*queue);
    (*queue) = (*queue)->next;
    while ((*queue) != sentinel) {
        TQueue aux = *queue;
        (*queue) = (*queue)->next;
        free(aux);
    }
    free(sentinel);
    *queue = NULL;
}

/* CompressImageTree:
- functie ce afiseaza nodurile unui arbore cuaternar (pe niveluri) intr-un fisier de iesire binar */
void CompressImageTree(TTree tree, FILE *fout) {
    TQueue queue = InitializeQueue();
    if (!queue) exit(-1);
    if (!InsertQueue(queue, tree)) exit(-1);
    while (ExtractQueue(queue, &tree)) {
        fwrite(&(tree->type), sizeof(unsigned char), 1, fout);
        if (tree->type) {
            fwrite(&(tree->color.red), sizeof(unsigned char), 1, fout);
            fwrite(&(tree->color.green), sizeof(unsigned char), 1, fout);
            fwrite(&(tree->color.blue), sizeof(unsigned char), 1, fout);
        } else {
            if (!InsertQueue(queue, tree->son1)) exit(-1);
            if (!InsertQueue(queue, tree->son2)) exit(-1);
            if (!InsertQueue(queue, tree->son3)) exit(-1);
            if (!InsertQueue(queue, tree->son4)) exit(-1);
        }
        // PrintQueue(queue, stdout);
    }
    DestroyQueue(&queue);
}

void LoadDimensionForDecompressedImageTree(TTree tree, int size) {
    if (!tree) return;
    if (tree->type) {
        tree->dim = size;
    } else {
        LoadDimensionForDecompressedImageTree(tree->son1, size/2);
        LoadDimensionForDecompressedImageTree(tree->son2, size/2);
        LoadDimensionForDecompressedImageTree(tree->son3, size/2);
        LoadDimensionForDecompressedImageTree(tree->son4, size/2);
    }
}

/* DecompressImageTree:
- functie ce creeaza un arbore cuaternar dintr-un fisier de intrare binar */
void DecompressImageTree(TTree tree, int size, FILE *fin) {
    TTree root = tree;
    TQueue queue = InitializeQueue();
    if (!queue) exit(-1);
    fread(&(tree->type), sizeof(unsigned char), 1, fin);
    if (tree->type) {
        tree->dim = 0;
        fread(&(tree->color.red), sizeof(unsigned char), 1, fin);
        fread(&(tree->color.green), sizeof(unsigned char), 1, fin);
        fread(&(tree->color.blue), sizeof(unsigned char), 1, fin);
    } else {
        if (!InsertQueue(queue, tree)) exit(-1);
    }
    while (ExtractQueue(queue, &tree)) {
        TPixel color; color.red = 0; color.green = 0; color.blue = 0;
        tree->son1 = AllocateNode(0, 0, color);
        if (!(tree->son1)) exit(-1);
        fread(&(tree->son1->type), sizeof(unsigned char), 1, fin);
        if (tree->son1->type) {
            tree->son1->dim = 0;
            fread(&(tree->son1->color.red), sizeof(unsigned char), 1, fin);
            fread(&(tree->son1->color.green), sizeof(unsigned char), 1, fin);
            fread(&(tree->son1->color.blue), sizeof(unsigned char), 1, fin);
        } else {
            if (!InsertQueue(queue, tree->son1)) exit(-1);
        }
        tree->son2 = AllocateNode(0, 0, color);
        if (!(tree->son2)) exit(-1);
        fread(&(tree->son2->type), sizeof(unsigned char), 1, fin);
        if (tree->son2->type) {
            tree->son2->dim = 0;
            fread(&(tree->son2->color.red), sizeof(unsigned char), 1, fin);
            fread(&(tree->son2->color.green), sizeof(unsigned char), 1, fin);
            fread(&(tree->son2->color.blue), sizeof(unsigned char), 1, fin);
        } else {
            if (!InsertQueue(queue, tree->son2)) exit(-1);
        }
        tree->son3 = AllocateNode(0, 0, color);
        if (!(tree->son3)) exit(-1);
        fread(&(tree->son3->type), sizeof(unsigned char), 1, fin);
        if (tree->son3->type) {
            tree->son3->dim = 0;
            fread(&(tree->son3->color.red), sizeof(unsigned char), 1, fin);
            fread(&(tree->son3->color.green), sizeof(unsigned char), 1, fin);
            fread(&(tree->son3->color.blue), sizeof(unsigned char), 1, fin);
        } else {
            if (!InsertQueue(queue, tree->son3)) exit(-1);
        }
        tree->son4 = AllocateNode(0, 0, color);
        if (!(tree->son4)) exit(-1);
        fread(&(tree->son4->type), sizeof(unsigned char), 1, fin);
        if (tree->son4->type) {
            tree->son4->dim = 0;
            fread(&(tree->son4->color.red), sizeof(unsigned char), 1, fin);
            fread(&(tree->son4->color.green), sizeof(unsigned char), 1, fin);
            fread(&(tree->son4->color.blue), sizeof(unsigned char), 1, fin);
        } else {
            if (!InsertQueue(queue, tree->son4)) exit(-1);
        }
    }
    DestroyQueue(&queue);
    LoadDimensionForDecompressedImageTree(root, size);
}
