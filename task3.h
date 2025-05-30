// biblioteci standard
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


// biblioteci pt python checker
#include <sys/stat.h>
#include <sys/types.h>

//structuri
//structuri pt task 2
//tine minte coordonatele celulei
typedef struct celula
{
    int linie;
    int coloana;
    struct celula *next;
} cell;
//tine minte nr de generatii parcurse si felul in care arata matricea la fiecare generatie
typedef struct generation
{
    int nrgen;
    cell *iteratii;
    struct generation *next;
} generation;

//pt task3
//structura tip arbore ptr observarea evolutiei matricei in functie de regula aplicata(deci de pozitia in arbore)
typedef struct Node
{
    char **matrix;
    int n, m;
    int nrgen;
    cell *differences;  // lista celule modificate față de părintele nodului
    struct Node *left;  // regula nouă (simulareleft)
    struct Node *right; // regula clasică (simulare)
} Node;

//functii
//task 1 basic functions
//nr celulele in viata de langa elementul din matrice si se asigura ca nu iesim din grid
int isalive(char **a, int n, int m, int i, int j)
{
    int alive = 0;
    int linieindex, coloanaindex;
    int linievecina, coloanavecina;
    for (linieindex = -1; linieindex <= 1; linieindex++)
    {
        for (coloanaindex = -1; coloanaindex <= 1; coloanaindex++)
        {
            if (coloanaindex == 0 && linieindex == 0)
            {
                continue;
            }
            linievecina = i + linieindex;
            coloanavecina = j + coloanaindex;
            if (linievecina >= 0 && coloanavecina >= 0 && linievecina < n && coloanavecina < m)
            {
                if (a[linievecina][coloanavecina] == 'X')
                {
                    alive++;
                }
            }
        }
    }
    return alive;
}

//simuleaza regulile jocului parcurgand matricea si modificand starea elementelor in functie de var alive returnata anterior
void simulare(char **a, char **b, int n, int m)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {

            int vecini;
            vecini = isalive(a, n, m, i, j);
            if (a[i][j] == 'X')
            {
                if (vecini < 2 || vecini > 3)
                {
                    b[i][j] = '+';
                }
                else
                {
                    b[i][j] = 'X';
                }
            }
            else
            {
                if (vecini == 3)
                {
                    b[i][j] = 'X';
                }
                else
                {
                    b[i][j] = '+';
                }
            }
        }
    }
}

// aloca memorie matrice
char **newmatrix(int n, int m)
{
    char **a = (char **)malloc(n * sizeof(char *));
    int i;
    for (i = 0; i < n; i++)
    {
        a[i] = (char *)malloc((m + 1) * sizeof(char)); // '\0'
    }
    return a;
}

//preia matricea din file
void readmatrix(FILE *f, char **a, int n, int m)
{
    int i;
    for (i = 0; i < n; i++)
    {
        fgets(a[i], m + 2, f);
        a[i][strcspn(a[i], "\n")] = '\0';
    }
}

//printeaza matricea modificata in fisier(in out)
void showmatrix(FILE *f, int n, char **a)
{
    int i;
    for (i = 0; i < n; i++)
    {
        fprintf(f, "%s\n", a[i]);
    }
    fprintf(f, "\n");
}

//elib memorie pt a preveni leaks
void freematrix(char **a, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(a[i]);
    }
    free(a);
}


//functii task2
//stocheaza diferentele prin care trece un element/celula a matricei de-a lungul generatiilor
cell *diferences(char **a, char **b, int n, int m)
{
    cell *cap = NULL;
    cell *ultim = NULL;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (a[i][j] != b[i][j])
            {
                cell *nou=(cell*)malloc(sizeof(cell));
                nou->linie = i;
                nou->coloana = j;
                nou->next = NULL;
                if (!cap)
                    cap = nou;
                else
                    ultim->next = nou;
                ultim = nou;
            }
        }
    }
    return cap;
}

//avoid memory leak
void freecell(cell *cap)
{
    while (cap)
    {
        cell *temp = cap;
        cap = cap->next;
        free(temp);
    }
}


//functii task 3
//copiaza matricea ca sa nu apara pierderi
char **copyMatrix(char **src, int n, int m)
{
    char **dest = newmatrix(n, m);
    for (int i = 0; i < n; i++)
        strcpy(dest[i], src[i]);
    return dest;
}
//simulare left declarata inainte fiindca am nevoie de ea pt buildree
void simulareleft(char **a, char **b, int n, int m);
//functia care imi construieste arborele in functie de simulare
Node *buildtree(char **parent_matrix, char **current_matrix, int n, int m, int currentLevel, int maxLevel)
{
    Node *node =(Node*) malloc(sizeof(Node));
    node->nrgen = currentLevel;
    node->n = n;
    node->m = m;
    node->left = NULL;
    node->right = NULL;

    node->matrix = copyMatrix(current_matrix, n, m);

    
    if (parent_matrix == NULL)
    {
        node->differences = NULL; 
    }
    else
    {
        node->differences = diferences(parent_matrix, current_matrix, n, m);
    }

    if (currentLevel == maxLevel)
        return node;

    
    char **left_matrix = newmatrix(n, m);
    char **right_matrix = newmatrix(n, m);

    simulareleft(current_matrix, left_matrix, n, m);
    node->left = buildtree(current_matrix, left_matrix, n, m, currentLevel + 1, maxLevel);

    simulare(current_matrix, right_matrix, n, m);
    node->right = buildtree(current_matrix, right_matrix, n, m, currentLevel + 1, maxLevel);

    freematrix(left_matrix, n);
    freematrix(right_matrix, n);

    return node;
}

//afiseaza diferentele parcurgand arborele in preordine
void preorder_print_differences(FILE *f, Node *node)
{
    if (!node)
        return;

    // diferentele pt nod curent 
    cell *c = node->differences;
    while (c)
    {
        fprintf(f, "%d %d %d\n", node->nrgen, c->linie, c->coloana);
        c = c->next;
    }

    preorder_print_differences(f, node->left);
    preorder_print_differences(f, node->right);
}

// simulare pt task 3 partea din stanga a arborelui asemanator cu simulare simplu dar implementeaza alta regula
void simulareleft(char **a, char **b, int n, int m)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            int vecini = isalive(a, n, m, i, j);
            if (vecini == 2)
                b[i][j] = 'X';
            else
                b[i][j] = '+';
        }
    }
}

//elib memoria 
void freetree(Node *node)
{
    if (!node)
        return;

    freematrix(node->matrix, node->n);
    freecell(node->differences);
    freetree(node->left);
    freetree(node->right);
    free(node);
}