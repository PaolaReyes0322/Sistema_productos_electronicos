#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"
/*
*  Función para cargar los usuarios desde el archivo vendedores.txt
*  Retorna 1 si se cargaron correctamente, 0 en caso de error
*/
int cargar_usuarios(Usuario **usuarios, int *cantidad) {
    FILE *archivo = fopen("vendedores.txt", "r");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo vendedores.txt\n");
        return 0;
    }
    
    *cantidad = 0;
    int capacidad = MAX_USUARIOS;
    *usuarios = (Usuario *)malloc(capacidad * sizeof(Usuario));
    
    if (!(*usuarios)) {
        printf("Error: No se pudo asignar memoria\n");
        fclose(archivo);
        return 0;
    }
    
    char linea[300];
    while (fgets(linea, sizeof(linea), archivo)) {
        if (*cantidad >= capacidad) {
            capacidad *= 2;
            Usuario *temp = (Usuario *)realloc(*usuarios, capacidad * sizeof(Usuario));
            if (!temp) {
                printf("Error: No se pudo reasignar memoria\n");
                free(*usuarios);
                fclose(archivo);
                return 0;
            }
            *usuarios = temp;
        }
        
        linea[strcspn(linea, "\n")] = 0;

        char *token = strtok(linea, ",");
        if (token) {
            strncpy((*usuarios)[*cantidad].usuario, token, MAX_USUARIO_LEN - 1);
            (*usuarios)[*cantidad].usuario[MAX_USUARIO_LEN - 1] = '\0';
        }
        
        token = strtok(NULL, ",");
        if (token) {
            strncpy((*usuarios)[*cantidad].clave, token, MAX_CLAVE_LEN - 1);
            (*usuarios)[*cantidad].clave[MAX_CLAVE_LEN - 1] = '\0';
        }
        
        token = strtok(NULL, ",");
        if (token) {
            strncpy((*usuarios)[*cantidad].nombre, token, MAX_NOMBRE_LEN - 1);
            (*usuarios)[*cantidad].nombre[MAX_NOMBRE_LEN - 1] = '\0';
        }
        
        (*cantidad)++;
    }
    
    fclose(archivo);
    return 1;
}
/*
    Función para validar un usuario y clave
    Retorna 1 si el usuario es válido, 0 en caso contrario
*/
int validar_usuario(Usuario *usuarios, int cantidad, const char *usuario, const char *clave) {
    for (int i = 0; i < cantidad; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0 && 
            strcmp(usuarios[i].clave, clave) == 0) {
            return 1;
        }
    }
    return 0;
}
/*
    Función para liberar la memoria asignada a los usuarios
*/
void liberar_usuarios(Usuario *usuarios) {
    if (usuarios) {
        free(usuarios);
    }
}