#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

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

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Utilizare: %s <folder inputs> <folder outputs>\n", argv[0]);
        return 1;
    }

    char *input_folder = argv[1];
    char *output_folder = argv[2];

    // Creează folderul de output dacă nu există
    struct stat st = {0};
    if (stat(output_folder, &st) == -1)
    {
        mkdir(output_folder);
    }

    DIR *dir = opendir(input_folder);
    if (!dir)
    {
        perror("Eroare la deschiderea folderului inputs");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Ignorăm "." și ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Construim calea completă pentru fișierul de input
        char input_path[512];
        snprintf(input_path, sizeof(input_path), "%s/%s", input_folder, entry->d_name);

        // Construim calea completă pentru fișierul de output
        char output_path[512];
        snprintf(output_path, sizeof(output_path), "%s/%s.out", output_folder, entry->d_name);

        FILE *input = fopen(input_path, "r");
        if (!input)
        {
            perror("Eroare la deschiderea fișierului de input");
            continue;
        }

        FILE *output = fopen(output_path, "w");
        if (!output)
        {
            perror("Eroare la deschiderea fișierului de output");
            fclose(input);
            continue;
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

        // simulare Game of Life
        for (int k = 0; k < gen; k++)
        {
            simulare(a, b, n, m);

            // scrie matricea curentă în fișierul de ieșire
            fprintf(output, "Generatia %d:\n", k + 1);
            showmatrix(output, n, a);

            // schimbă matricele pentru următoarea generație
            char **temp = a;
            a = b;
            b = temp;
        }

        freematrix(a, n);
        freematrix(b, n);
        fclose(input);
        fclose(output);
    }

    closedir(dir);
    return 0;
}