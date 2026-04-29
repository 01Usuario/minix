#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

/*
 * print_tree: funcion recursiva principal.
 * path ruta del directorio actual que vamos a listar
 * depth nivel de profundidad actual (empieza en 0)
 */
void print_tree(const char *path, int depth)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char full_path[4096];
    int i;

    
    dir = opendir(path);
    if (dir == NULL) {
        perror(path);
        return;
    }

    
    while ((entry = readdir(dir)) != NULL) {

        /* Saltamos las entradas especiales "."  y ".." para evitar loops infinitos */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        
         /* concatenando la ruta del padre + "/" + nombre del elemento */
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        
        if (lstat(full_path, &st) == -1) {
            perror(full_path);
            continue;
        }

        /*2 espacios por cada nivel de profundidad */
        for (i = 0; i < depth; i++)
            printf("  ");

        /* Imprimimos el nombre del elemento */
        printf("%s\n", entry->d_name);

        
        if (S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)) {
            print_tree(full_path, depth + 1);
        }
    }

   
    closedir(dir);
}

int main(int argc, char *argv[])
{
    const char *root;

    /* Si no se pasa ningun argumento, usamos el directorio actual "."
     * Si pasa un argumento, usamos ese como ruta de inicio */
    if (argc < 2)
        root = ".";
    else
        root = argv[1];

    /* Imprimimos la ruta raiz y luego llamamos a la funcin recursiva */
    printf("%s\n", root);
    print_tree(root, 1);

    return 0;
}