#ifndef PRODUCTOS_H
#define PRODUCTOS_H
#define MAX_PRODUCTOS 500
#define MAX_CODIGO_LEN 50
#define MAX_NOMBRE_PROD_LEN 100
/*
* Estructura para almacenar la información del producto
*/
typedef struct {
    char codigo[MAX_CODIGO_LEN];
    char nombre[MAX_NOMBRE_PROD_LEN];
    int cantidad_disponible;
    float costo;
    float precio_venta;
} Producto;

/*
* Estructura para almacenar la información de la venta
*/
typedef struct {
    int num_factura;
    char codigo[MAX_CODIGO_LEN];
    char nombre[MAX_NOMBRE_PROD_LEN];
    int cantidad_vendida;
    float costo;
    float precio_venta;
    char fecha_venta[15];
} Venta;

// Funciones para manejo de productos
int cargar_productos(Producto **productos, int *cantidad);
Producto* buscar_producto(Producto *productos, int cantidad, const char *codigo);
int actualizar_inventario(Producto *productos, int cantidad);
void liberar_productos(Producto *productos);

// Funciones para manejo de ventas
int obtener_ultimo_numero_factura();
int registrar_ventas(Venta *ventas, int cantidad_ventas);

#endif