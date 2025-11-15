#ifndef USUARIOS_H
#define USUARIOS_H
#define MAX_USUARIOS 50
#define MAX_USUARIO_LEN 50
#define MAX_CLAVE_LEN 50
#define MAX_NOMBRE_LEN 100

// Estructura para almacenar la información del usuario
typedef struct {
    char usuario[MAX_USUARIO_LEN];
    char clave[MAX_CLAVE_LEN];
    char nombre[MAX_NOMBRE_LEN];
} Usuario;

// Funciones para manejo de usuarios
int cargar_usuarios(Usuario **usuarios, int *cantidad);
int validar_usuario(Usuario *usuarios, int cantidad, const char *usuario, const char *clave);
void liberar_usuarios(Usuario *usuarios);

#endif