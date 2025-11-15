#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "usuarios.h"
#include "productos.h"

#define MAX_VENTAS_TEMP 100

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void obtener_fecha_actual(char *fecha) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(fecha, 15, "%Y-%m-%d", tm_info);
}

int main() {
    Usuario *usuarios = NULL;
    Producto *productos = NULL;
    int cant_usuarios = 0, cant_productos = 0;
    int intentos = 0;
    char usuario_input[MAX_USUARIO_LEN];
    char clave_input[MAX_CLAVE_LEN];
    
    printf("=== SISTEMA DE VENTAS - TIENDA ELECTRONICA ===\n\n");
    
    // Cargar usuarios
    if (!cargar_usuarios(&usuarios, &cant_usuarios)) {
        printf("Error al cargar usuarios. Cerrando sistema.\n");
        return 1;
    }
    
    // Autenticación
    while (intentos < 3) {
        printf("Usuario: ");
        scanf("%49s", usuario_input);
        printf("Clave: ");
        scanf("%49s", clave_input);
        
        if (validar_usuario(usuarios, cant_usuarios, usuario_input, clave_input)) {
            printf("\n¡Autenticacion exitosa! Bienvenido.\n\n");
            break;
        } else {
            intentos++;
            if (intentos < 3) {
                printf("Usuario o clave incorrectos. Intento %d de 3.\n\n", intentos);
            } else {
                printf("Usuario o clave incorrectos. Se han agotado los intentos.\n");
                liberar_usuarios(usuarios);
                return 1;
            }
        }
    }
    
    // Cargar productos
    if (!cargar_productos(&productos, &cant_productos)) {
        printf("Error al cargar productos. Cerrando sistema.\n");
        liberar_usuarios(usuarios);
        return 1;
    }
    
    // Proceso de ventas
    int num_factura = obtener_ultimo_numero_factura() + 1;
    char fecha[15];
    obtener_fecha_actual(fecha);
    
    printf("=== NUEVA VENTA ===\n");
    printf("Fecha: %s\n", fecha);
    printf("Factura No: %d\n\n", num_factura);
    
    Venta ventas_temp[MAX_VENTAS_TEMP];
    int cant_ventas = 0;
    float total_venta = 0.0;
    char continuar;
    
    do {
        char codigo[MAX_CODIGO_LEN];
        int cantidad;
        
        printf("Ingrese codigo del producto: ");
        scanf("%49s", codigo);
        
        Producto *prod = buscar_producto(productos, cant_productos, codigo);
        
        if (!prod) {
            printf("Producto no encontrado.\n\n");
            limpiar_buffer();
            printf("¿Desea continuar agregando productos? (s/n): ");
            scanf(" %c", &continuar);
            continue;
        }
        
        printf("Producto: %s\n", prod->nombre);
        printf("Disponible: %d unidades\n", prod->cantidad_disponible);
        printf("Precio: $%.2f\n", prod->precio_venta);
        
        printf("Cantidad a vender: ");
        scanf("%d", &cantidad);
        
        if (cantidad <= 0) {
            printf("Cantidad invalida.\n\n");
            limpiar_buffer();
            printf("¿Desea continuar agregando productos? (s/n): ");
            scanf(" %c", &continuar);
            continue;
        }
        
        if (cantidad > prod->cantidad_disponible) {
            printf("Error: No hay suficiente inventario. Solo hay %d unidades disponibles.\n\n", prod->cantidad_disponible);
            limpiar_buffer();
            printf("¿Desea continuar agregando productos? (s/n): ");
            scanf(" %c", &continuar);
            continue;
        }
        
        // Agregar a ventas temporales
        if (cant_ventas < MAX_VENTAS_TEMP) {
            ventas_temp[cant_ventas].num_factura = num_factura;
            strcpy(ventas_temp[cant_ventas].codigo, prod->codigo);
            strcpy(ventas_temp[cant_ventas].nombre, prod->nombre);
            ventas_temp[cant_ventas].cantidad_vendida = cantidad;
            ventas_temp[cant_ventas].costo = prod->costo;
            ventas_temp[cant_ventas].precio_venta = prod->precio_venta;
            strcpy(ventas_temp[cant_ventas].fecha_venta, fecha);
            
            float subtotal = cantidad * prod->precio_venta;
            total_venta += subtotal;
            
            printf("\nProducto agregado: %s x %d = $%.2f\n\n", 
                   prod->nombre, cantidad, subtotal);
            
            cant_ventas++;
        } else {
            printf("Error: Se ha alcanzado el limite de productos por factura.\n\n");
        }
        
        limpiar_buffer();
        printf("¿Desea continuar agregando productos? (s/n): ");
        scanf(" %c", &continuar);
        
    } while (continuar == 's' || continuar == 'S');
    
    // Mostrar resumen
    if (cant_ventas > 0) {
        printf("\n=== RESUMEN DE VENTA ===\n");
        printf("Factura No: %d\n", num_factura);
        printf("Fecha: %s\n\n", fecha);
        printf("%-15s %-30s %10s %12s %12s\n", "CODIGO", "PRODUCTO", "CANTIDAD", "PRECIO UNIT", "SUBTOTAL");
        printf("---------------------------------------------------------------------\n");
        
        for (int i = 0; i < cant_ventas; i++) {
            float subtotal = ventas_temp[i].cantidad_vendida * ventas_temp[i].precio_venta;
            printf("%-15s %-30s %10d $%11.2f $%11.2f\n", ventas_temp[i].codigo, ventas_temp[i].nombre, ventas_temp[i].cantidad_vendida, ventas_temp[i].precio_venta, subtotal);
        }
        printf("---------------------------------------------------------------------\n");
        printf("%58s $%11.2f\n", "TOTAL:", total_venta);
        
        limpiar_buffer();
        char confirmar;
        printf("\n¿Confirmar venta? (s/n): ");
        scanf(" %c", &confirmar);
        
        if (confirmar == 's' || confirmar == 'S') {
            // Actualizar inventario
            for (int i = 0; i < cant_ventas; i++) {
                Producto *prod = buscar_producto(productos, cant_productos, ventas_temp[i].codigo);
                if (prod) {
                    prod->cantidad_disponible -= ventas_temp[i].cantidad_vendida;
                }
            }
            
            // Guardar cambios
            if (actualizar_inventario(productos, cant_productos) &&
                registrar_ventas(ventas_temp, cant_ventas)) {
                printf("\n¡Venta registrada exitosamente!\n");
            } else {
                printf("\nError al registrar la venta.\n");
            }
        } else {
            printf("\nVenta cancelada.\n");
        }
    } else {
        printf("\nNo se registraron productos para la venta.\n");
    }
    
    // Liberar memoria
    liberar_usuarios(usuarios);
    liberar_productos(productos);
    
    return 0;
}