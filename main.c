#include "funciones.h" 

int main(int argc, char *argv[]) {
    char nombreBiblioteca[MAX_NOMBRE_BIBLIOTECA] = ""; 
    struct Libro libros[MAX_LIBROS]; 
    int numLibros = 0; 

    printf("¡Bienvenido al Sistema de Gestion de Biblioteca!\n");

    int opcion;
    do {
        mostrarMenuPrincipal(nombreBiblioteca, numLibros);
        opcion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 9);

        switch (opcion) {
            case 1:
                registrarBiblioteca(nombreBiblioteca);
                break;
            case 2:
                if (strlen(nombreBiblioteca) == 0) {
                    printf("Error: Primero debe registrar el nombre de la biblioteca.\n");
                } else {
                    registrarLibros(libros, &numLibros);
                }
                break;
            case 3:
                mostrarLibros(libros, numLibros);
                break;
            case 4:
                buscarLibro(libros, numLibros);
                break;
            case 5:
                actualizarEstadoLibro(libros, numLibros);
                break;
            case 6:
                eliminarLibro(libros, &numLibros);
                break;
            case 7:
                if (numLibros == 0) {
                    printf("No hay libros registrados para editar.\n");
                } else {
                    int opcionEdicion;
                    do {
                        printf("\n¿Que desea editar?\n");
                        printf("1. Nombres (biblioteca, titulo, autor)\n");
                        printf("2. Componentes (ID, año de publicacion)\n");
                        printf("0. Volver\n");
                        opcionEdicion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 2);
                        
                        switch (opcionEdicion) {
                            case 1:
                                editarNombresMenu(nombreBiblioteca, libros, numLibros);
                                break;
                            case 2:
                                editarComponentesMenu(libros, numLibros);
                                break;
                            case 0:
                                printf("Volviendo al menu anterior.\n");
                                break;
                        }
                    } while (opcionEdicion != 0);
                }
                break;
            case 8:
                prestarLibro(libros, numLibros);
                break;
            case 9:
                devolverLibro(libros, numLibros);
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                break;
        }

        if (opcion != 0) {
            printf("\nPresione Enter para continuar...");
            while (getchar() != '\n'); 
        }
    } while (opcion != 0); 
    
    return 0; 
}

