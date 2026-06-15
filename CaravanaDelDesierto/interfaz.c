#include "interfaz.h"

char CrearMenuInicial(const char* menuTexto, const char* opciones)
{
    char opcion=1;
    while (strchr(opciones, opcion) == NULL) {
        printf("%s\n", menuTexto);
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion);
        while(opcion == '\n') {
            scanf(" %c", &opcion);
        }
        if(strchr(opciones, opcion) == NULL) {
            printf("Opción no válida. Por favor, Ingresa de nuevo.\n");
        }
    } 
    return opcion;
}