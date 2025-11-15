#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "productos.h"

int cargar_productos(Producto **productos, int *cantidad) {
    FILE *archivo = fopen("producto.txt", "r");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo producto.txt\n");
        return 0;
    }
    
    *cantidad = 0;
    int capacidad = MAX_PRODUCTOS;
    *productos = (Producto *)malloc(capacidad * sizeof(Producto));
    
    if (!(*productos)) {
        printf("Error: No se pudo asignar memoria\n");
        fclose(archivo);
        return 0;
    }
    
    char linea[300];
    while (fgets(linea, sizeof(linea), archivo)) {
        if (*cantidad >= capacidad) {
            capacidad *= 2;
            Producto *temp = (Producto *)realloc(*productos, capacidad * sizeof(Producto));
            if (!temp) {
                printf("Error: No se pudo reasignar memoria\n");
                free(*productos);
                fclose(archivo);
                return 0;
            }
            *productos = temp;
        }
        
        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = 0;
        
        // Parsear: Codigo_producto,Nombre_producto,cantidad_disponible,costo,precio_venta
        char *token = strtok(linea, ",");
        if (token) {
            strncpy((*productos)[*cantidad].codigo, token, MAX_CODIGO_LEN - 1);
            (*productos)[*cantidad].codigo[MAX_CODIGO_LEN - 1] = '\0';
        }
        
        token = strtok(NULL, ",");
        if (token) {
            strncpy((*productos)[*cantidad].nombre, token, MAX_NOMBRE_PROD_LEN - 1);
            (*productos)[*cantidad].nombre[MAX_NOMBRE_PROD_LEN - 1] = '\0';
        }
        
        token = strtok(NULL, ",");
        if (token) (*productos)[*cantidad].cantidad_disponible = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) (*productos)[*cantidad].costo = atof(token);
        
        token = strtok(NULL, ",");
        if (token) (*productos)[*cantidad].precio_venta = atof(token);
        
        (*cantidad)++;
    }
    
    fclose(archivo);
    return 1;
}

Producto* buscar_producto(Producto *productos, int cantidad, const char *codigo) {
    for (int i = 0; i < cantidad; i++) {
        if (strcmp(productos[i].codigo, codigo) == 0) {
            return &productos[i];
        }
    }
    return NULL;
}

int actualizar_inventario(Producto *productos, int cantidad) {
    FILE *archivo = fopen("producto.txt", "w");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo producto.txt para escritura\n");
        return 0;
    }
    
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%s,%s,%d,%.2f,%.2f\n",
                productos[i].codigo,
                productos[i].nombre,
                productos[i].cantidad_disponible,
                productos[i].costo,
                productos[i].precio_venta);
    }
    
    fclose(archivo);
    return 1;
}

void liberar_productos(Producto *productos) {
    if (productos) {
        free(productos);
    }
}

int obtener_ultimo_numero_factura() {
    FILE *archivo = fopen("ventas.txt", "r");
    int ultimo_numero = 0;
    
    if (!archivo) {
        // Si no existe el archivo, retornar 0
        return 0;
    }
    
    char linea[300];
    while (fgets(linea, sizeof(linea), archivo)) {
        int num_factura;
        if (sscanf(linea, "%d,", &num_factura) == 1) {
            if (num_factura > ultimo_numero) {
                ultimo_numero = num_factura;
            }
        }
    }
    
    fclose(archivo);
    return ultimo_numero;
}

int registrar_ventas(Venta *ventas, int cantidad_ventas) {
    FILE *archivo = fopen("ventas.txt", "a");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo ventas.txt para escritura\n");
        return 0;
    }
    
    for (int i = 0; i < cantidad_ventas; i++) {
        fprintf(archivo, "%d,%s,%s,%d,%.2f,%.2f,%s\n",
                ventas[i].num_factura,
                ventas[i].codigo,
                ventas[i].nombre,
                ventas[i].cantidad_vendida,
                ventas[i].costo,
                ventas[i].precio_venta,
                ventas[i].fecha_venta);
    }
    
    fclose(archivo);
    return 1;
}