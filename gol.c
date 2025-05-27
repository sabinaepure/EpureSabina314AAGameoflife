#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// task1
//  numara cate celule in viata se afla langa a[i][j]
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

// aloca matrice
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

void readmatrix(FILE *f, char **a, int n, int m)
{
    int i;
    for (i = 0; i < n; i++)
    {
        fgets(a[i], m + 2, f);
        a[i][strcspn(a[i], "\n")] = '\0';
    }
}

// printeaza in fisier
void showmatrix(FILE *f, int n, char **a)
{
    int i;
    for (i = 0; i < n; i++)
    {
        fprintf(f, "%s\n", a[i]);
    }
    fprintf(f, "\n");
}

// elib memo
void freematrix(char **a, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(a[i]);
    }
    free(a);
}

// task2
typedef struct celula
{
    int linie;
    int coloana;
    struct celula *next;
} cell;

typedef struct generation
{
    int nrgen;
    cell *iteratii;
    struct generation *next;
} generation;

void push(generation **top, int index, cell *diferencelist)
{
    generation *nou = malloc(sizeof(generation));
    nou->nrgen = index;
    nou->iteratii = diferencelist;
    nou->next = *top;
    *top = nou;
}

void showstack(FILE *f, generation *top)
{

    generation *invers = NULL;

    while (top)
    {
        generation *next = top->next;
        top->next = invers;
        invers = top;
        top = next;
    }

    while (invers)
    {
        cell *c = invers->iteratii;
        while (c)
        {
            fprintf(f, "%d %d %d\n", invers->nrgen, c->linie, c->coloana);
            c = c->next;
        }
        invers = invers->next;
    }
}

cell *sortlist(cell *cap)
{
    if (!cap || !cap->next)
        return cap;

    for (cell *i = cap; i != NULL; i = i->next)
    {
        for (cell *j = i->next; j != NULL; j = j->next)
        {
            if (j->linie < i->linie || (j->linie == i->linie && j->coloana < i->coloana))
            {
                int tmp_lin = i->linie, tmp_col = i->coloana;
                i->linie = j->linie;
                i->coloana = j->coloana;
                j->linie = tmp_lin;
                j->coloana = tmp_col;
            }
        }
    }
    return cap;
}

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
                cell *nou = malloc(sizeof(cell));
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
/*
int isEmpty(generation *top)
{
    return (top->next == NULL);
}
    */

int main()
{
    FILE *input = fopen("data1.in", "r");
    if (!input)
    {
        perror("eroare la deschidere input");
        return 1;
    }
    FILE *output = fopen("data1.out", "w");
    if (!output)
    {
        perror("eroare la deschidere output");
        return 1;
    }
    int testdata;
    fscanf(input, "%d", &testdata);
    int n, m;
    fscanf(input, "%d %d", &n, &m);
    int gen;
    fscanf(input, "%d", &gen);
    fgetc(input);

    char **a = newmatrix(n, m);
    char **b = newmatrix(n, m);

    readmatrix(input, a, n, m);

    // simulare pt task 1 doar matrice
    int k;
    /*
    for (k = 0; k < gen; k++)
    {
        simulare(a, b, n, m);
        showmatrix(output, n, b);
        char **temp = a;
        a = b;
        b = temp;
    }
        */

    // task 2
    generation *stack = NULL;
    // simulare task 2 cu stiva

    for (k = 0; k < gen; k++)
    {
        simulare(a, b, n, m);

        cell *diff = diferences(a, b, n, m);
        diff = sortlist(diff);
        push(&stack, k + 1, diff);

        char **temp = a;
        a = b;
        b = temp;
    }

    showstack(output, stack);

    freematrix(a, n);
    freematrix(b, n);
    fclose(input);
    fclose(output);
    return 0;
}