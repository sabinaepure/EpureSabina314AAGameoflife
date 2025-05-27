#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

int isalive(char **a, int n, int m, int i, int j)
{
    int alive = 0;
    for (int linieindex = -1; linieindex <= 1; linieindex++)
    {
        for (int coloanaindex = -1; coloanaindex <= 1; coloanaindex++)
        {
            if (coloanaindex == 0 && linieindex == 0)
                continue;
            int linievecina = i + linieindex;
            int coloanavecina = j + coloanaindex;
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
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int vecini = isalive(a, n, m, i, j);
            if (a[i][j] == 'X')
            {
                b[i][j] = (vecini < 2 || vecini > 3) ? '+' : 'X';
            }
            else
            {
                b[i][j] = (vecini == 3) ? 'X' : '+';
            }
        }
    }
}

char **newmatrix(int n, int m)
{
    char **a = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
    {
        a[i] = (char *)malloc((m + 1) * sizeof(char)); // +1 pentru '\0'
    }
    return a;
}

void readmatrix(FILE *f, char **a, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        fgets(a[i], m + 2, f);
        a[i][strcspn(a[i], "\n")] = '\0';
    }
}

void showmatrix(FILE *f, int n, char **a)
{
    for (int i = 0; i < n; i++)
    {
        fprintf(f, "%s\n", a[i]);
    }
    fprintf(f, "\n");
}

void freematrix(char **a, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(a[i]);
    }
    free(a);
}

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
    cell *cap = NULL, *ultim = NULL;
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

//  reverse modificările asupra unei matrice
void undo_generation(char **mat, cell *diff)
{
    for (cell *c = diff; c != NULL; c = c->next)
    {
        int i = c->linie;
        int j = c->coloana;
        mat[i][j] = (mat[i][j] == 'X') ? '+' : 'X';
    }
}

//  reverse o stivă de generații
generation *reverse_stack(generation *top)
{
    generation *invers = NULL;
    while (top)
    {
        generation *next = top->next;
        top->next = invers;
        invers = top;
        top = next;
    }
    return invers;
}

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
    int n, m, gen;
    fscanf(input, "%d %d %d", &n, &m, &gen);
    fgetc(input);

    char **a = newmatrix(n, m);
    char **b = newmatrix(n, m);
    readmatrix(input, a, n, m);

    generation *stack = NULL;

    for (int k = 0; k < gen; k++)
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

    //  matricea
    generation *inv = reverse_stack(stack);
    for (generation *g = inv; g != NULL; g = g->next)
    {
        undo_generation(a, g->iteratii);
    }

    fprintf(output, "matricea initiala :\n");
    showmatrix(output, n, a);

    freematrix(a, n);
    freematrix(b, n);
    fclose(input);
    fclose(output);
    return 0;
}
