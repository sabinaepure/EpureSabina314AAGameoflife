#include"task3.h"

/*
// biblioteci standard
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

//pt easy reading se muta functiile in task3.h
//#include "task3.h"

// biblioteci pt python checker
#include <sys/stat.h>
#include <sys/types.h>
*/

// task1
// numara cate celule in viata se afla langa a[i][j]
int isalive(char **a, int n, int m, int i, int j);


// simularea cerintei de la task1
void simulare(char **a, char **b, int n, int m);

// aloca matrice
char **newmatrix(int n, int m);

void readmatrix(FILE *f, char **a, int n, int m);

void showmatrix(FILE *f, int n, char **a);

void freematrix(char **a, int n);

cell *diferences(char **a, char **b, int n, int m);

void freecell(cell *cap);


char **copyMatrix(char **src, int n, int m);


void simulareleft(char **a, char **b, int n, int m);

Node *buildtree(char **parent_matrix, char **current_matrix, int n, int m, int currentLevel, int maxLevel);


void preorder_print_differences(FILE *f, Node *node);


void freetree(Node *node);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("utilizare: %s <folder inputs> <folder outputs>\n", argv[0]);
        return 1;
    }

    char *input_folder = argv[1];
    char *output_folder = argv[2];

    // face output daca nu exista deja
    struct stat st = {0};
    if (stat(output_folder, &st) == -1)
    {
#ifdef _WIN32
        _mkdir(output_folder);
#else
        mkdir(output_folder, 0700);
#endif
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
        // ignor "." și ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // built input path
        char input_path[512];
        snprintf(input_path, sizeof(input_path), "%s/%s", input_folder, entry->d_name);

        // built output path
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
        fgetc(input); // consumă newline după gen

        char **a = newmatrix(n, m);
        readmatrix(input, a, n, m);

        
        Node *root = buildtree(NULL, a, n, m, 0, gen);

       
        preorder_print_differences(output, root);

        
        freematrix(a, n);
        freetree(root);

        fclose(input);
        fclose(output);
    }
    closedir(dir);

    return 0;
}
