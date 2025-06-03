#include "funciones.h"

void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c);
}

int leerCadenaValida(const char mensaje[], char destino[], int longitudMax) {
    printf("%s", mensaje);
    if (fgets(destino, longitudMax, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len-1] == '\n') {
            destino[len-1] = '\0';
        }
        
        if (strlen(destino) == 0) {
            printf("Error: La entrada no puede estar vacia. Intente de nuevo.\n");
            return 0;
        }
        return 1;
    }
    printf("Error al leer la entrada. Intente de nuevo.\n");
    limpiarBufferEntrada();
    return 0;
}

int leerEntero(const char mensaje[]) {
    char buffer[100];
    int valor;
    char extra;
    
    while (1) {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d %c", &valor, &extra) == 1) {
                return valor;
            }
        }
        printf("Error: Entrada invalida. Ingrese un numero entero valido.\n");
        limpiarBufferEntrada();
    }
}

int leerEnteroPositivo(const char mensaje[]) {
    int valor;
    do {
        valor = leerEntero(mensaje);
        if (valor <= 0) {
            printf("Error: El valor debe ser positivo. Intente de nuevo.\n");
        }
    } while (valor <= 0);
    return valor;
}

int leerEnteroEntreLimites(const char mensaje[], int min, int max) {
    int valor;
    do {
        valor = leerEntero(mensaje);
        if (valor < min || valor > max) {
            printf("Error: El valor debe estar entre %d y %d. Intente de nuevo.\n", min, max);
        }
    } while (valor < min || valor > max);
    return valor;
}

int confirmarAccion(const char mensaje[]) {
    char opcion;
    do {
        printf("%s (s/n): ", mensaje);
        opcion = tolower(getchar());
        limpiarBufferEntrada();
        
        if (opcion != 's' && opcion != 'n') {
            printf("Error: Ingrese 's' para si o 'n' para no.\n");
        }
    } while (opcion != 's' && opcion != 'n');
    return (opcion == 's');
}

int buscarLibroPorId(Libro libros[], int numLibros, int id) {
    for (int i = 0; i < numLibros; i++) {
        if (libros[i].id == id) return i;
    }
    return -1;
}

int buscarLibroPorTitulo(Libro libros[], int numLibros, const char titulo[]) {
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].titulo, titulo) == 0) return i;
    }
    return -1;
}

void mostrarMenuPrincipal(const char nombreBiblioteca[], int librosRegistrados) {
    printf("\n--- Menu Principal: Biblioteca %s ---\n", nombreBiblioteca);
    printf("Libros registrados: %d/%d\n\n", librosRegistrados, MAX_LIBROS);
    printf("1. Registrar/Editar nombre biblioteca\n");
    printf("2. Registrar libros\n");
    printf("3. Mostrar todos los libros\n");
    printf("4. Buscar libro\n");
    printf("5. Actualizar estado de libro\n");
    printf("6. Eliminar libro\n");
    printf("7. Editar informacion de libros\n");
    printf("8. Prestar libro\n");
    printf("9. Devolver libro\n");
    printf("0. Salir del programa\n");
}

void registrarBiblioteca(char nombreBiblioteca[]) {
    printf("\n--- Registrar Biblioteca ---\n");
    if (strlen(nombreBiblioteca) > 0) {
        printf("Nombre actual: %s\n", nombreBiblioteca);
        if (!confirmarAccion("¿Desea cambiar el nombre de la biblioteca?")) {
            return;
        }
    }
    while (!leerCadenaValida("Ingrese el nombre de la biblioteca: ", nombreBiblioteca, MAX_NOMBRE_BIBLIOTECA));
    printf("Nombre de la biblioteca registrado: %s\n", nombreBiblioteca);
}

void registrarLibros(Libro libros[], int *numLibros) {
    printf("\n--- Registrar Libros ---\n");
    
    if (*numLibros >= MAX_LIBROS) {
        printf("No se pueden registrar mas libros. Limite alcanzado (%d).\n", MAX_LIBROS);
        return;
    }
    
    int disponibles = MAX_LIBROS - *numLibros;
    char mensaje[100];
    sprintf(mensaje, "Ingrese el numero de libros a registrar (1-%d, 0 para cancelar): ", disponibles);
    int cantidad = leerEnteroEntreLimites(mensaje, 0, disponibles);
    
    if (cantidad == 0) {
        printf("Operacion cancelada.\n");
        return;
    }
    
    for (int i = 0; i < cantidad; i++) {
        printf("\nRegistrando libro %d de %d:\n", i+1, cantidad);
        
        Libro nuevoLibro;
        
        // ID del libro
        int id;
        do {
            id = leerEnteroPositivo("Ingrese el ID del libro: ");
            if (buscarLibroPorId(libros, *numLibros, id) != -1) {
                printf("Error: Ya existe un libro con este ID. Intente con otro.\n");
            }
        } while (buscarLibroPorId(libros, *numLibros, id) != -1);
        nuevoLibro.id = id;
        
        // Título del libro
        while (!leerCadenaValida("Ingrese el titulo del libro: ", nuevoLibro.titulo, MAX_TITULO));
        
        // Autor del libro
        while (!leerCadenaValida("Ingrese el autor del libro: ", nuevoLibro.autor, MAX_AUTOR));
        
        // Año de publicación
        nuevoLibro.anio_publicacion = leerEnteroEntreLimites("Ingrese el año de publicacion (1000-2025): ", 1000, 2025);
        
        // Estado (siempre disponible al registrar)
        strcpy(nuevoLibro.estado, "Disponible");
        
        // Agregar el libro al arreglo
        libros[*numLibros] = nuevoLibro;
        (*numLibros)++;
        
        printf("Libro registrado exitosamente!\n");
    }
}

void mostrarLibros(Libro libros[], int numLibros) {
    printf("\n--- Lista de Libros ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados.\n");
        return;
    }
    
    printf("%-5s %-30s %-20s %-10s %-15s\n", "ID", "Titulo", "Autor", "Año", "Estado");
    printf("----- ------------------------------ -------------------- ---------- ---------------\n");
    
    for (int i = 0; i < numLibros; i++) {
        printf("%-5d %-30s %-20s %-10d %-15s\n", 
               libros[i].id, 
               libros[i].titulo, 
               libros[i].autor, 
               libros[i].anio_publicacion, 
               libros[i].estado);
    }
}

void buscarLibro(Libro libros[], int numLibros) {
    printf("\n--- Buscar Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para buscar.\n");
        return;
    }
    
    int opcion;
    do {
        printf("\nComo desea buscar el libro?\n");
        printf("1. Por ID\n");
        printf("2. Por titulo\n");
        printf("0. Cancelar\n");
        opcion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 2);
        
        switch (opcion) {
            case 1: {
                int id = leerEnteroPositivo("Ingrese el ID del libro a buscar: ");
                int indice = buscarLibroPorId(libros, numLibros, id);
                
                if (indice == -1) {
                    printf("No se encontro un libro con el ID %d.\n", id);
                } else {
                    printf("\nInformacion del libro:\n");
                    printf("ID: %d\n", libros[indice].id);
                    printf("Titulo: %s\n", libros[indice].titulo);
                    printf("Autor: %s\n", libros[indice].autor);
                    printf("Año de publicacion: %d\n", libros[indice].anio_publicacion);
                    printf("Estado: %s\n", libros[indice].estado);
                }
                return;
            }
            case 2: {
                char titulo[MAX_TITULO];
                while (!leerCadenaValida("Ingrese el titulo del libro a buscar: ", titulo, MAX_TITULO));
                
                int indice = buscarLibroPorTitulo(libros, numLibros, titulo);
                
                if (indice == -1) {
                    printf("No se encontro un libro con el titulo \"%s\".\n", titulo);
                } else {
                    printf("\nInformacion del libro:\n");
                    printf("ID: %d\n", libros[indice].id);
                    printf("Titulo: %s\n", libros[indice].titulo);
                    printf("Autor: %s\n", libros[indice].autor);
                    printf("Año de publicacion: %d\n", libros[indice].anio_publicacion);
                    printf("Estado: %s\n", libros[indice].estado);
                }
                return;
            }
            case 0:
                printf("Busqueda cancelada.\n");
                return;
        }
    } while (opcion != 0);
}

void actualizarEstadoLibro(Libro libros[], int numLibros) {
    printf("\n--- Actualizar Estado de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para actualizar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a actualizar: ");
    int indice = buscarLibroPorId(libros, numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Titulo: %s\n", libros[indice].titulo);
    printf("Estado actual: %s\n", libros[indice].estado);
    
    printf("\nSeleccione el nuevo estado:\n");
    printf("1. Disponible\n");
    printf("2. Prestado\n");
    printf("0. Cancelar\n");
    
    int opcion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 2);
    
    switch (opcion) {
        case 1:
            strcpy(libros[indice].estado, "Disponible");
            printf("Estado actualizado a \"Disponible\".\n");
            break;
        case 2:
            strcpy(libros[indice].estado, "Prestado");
            printf("Estado actualizado a \"Prestado\".\n");
            break;
        case 0:
            printf("Actualizacion cancelada.\n");
            break;
    }
}

void eliminarLibro(Libro libros[], int *numLibros) {
    printf("\n--- Eliminar Libro ---\n");
    
    if (*numLibros == 0) {
        printf("No hay libros registrados para eliminar.\n");
        return;
    }
    
    mostrarLibros(libros, *numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a eliminar: ");
    int indice = buscarLibroPorId(libros, *numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado para eliminar:\n");
    printf("Titulo: %s\n", libros[indice].titulo);
    printf("Autor: %s\n", libros[indice].autor);
    
    if (confirmarAccion("¿Esta seguro que desea eliminar este libro?")) {
        for (int i = indice; i < *numLibros - 1; i++) {
            libros[i] = libros[i + 1];
        }
        
        (*numLibros)--;
        printf("Libro eliminado exitosamente.\n");
    } else {
        printf("Eliminacion cancelada.\n");
    }
}

void editarNombreBiblioteca(char nombreBiblioteca[]) {
    printf("\n--- Editar Nombre de la Biblioteca ---\n");
    char nombreAnterior[MAX_NOMBRE_BIBLIOTECA];
    strcpy(nombreAnterior, nombreBiblioteca);
    
    while (!leerCadenaValida("Ingrese el nuevo nombre de la biblioteca: ", nombreBiblioteca, MAX_NOMBRE_BIBLIOTECA));
    
    printf("Nombre de la biblioteca cambiado de \"%s\" a \"%s\".\n", nombreAnterior, nombreBiblioteca);
}

void editarTituloLibro(Libro libros[], int numLibros) {
    printf("\n--- Editar Titulo de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    int indice = buscarLibroPorId(libros, numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Titulo actual: %s\n", libros[indice].titulo);
    
    char nuevoTitulo[MAX_TITULO];
    while (!leerCadenaValida("Ingrese el nuevo titulo: ", nuevoTitulo, MAX_TITULO));
    
    printf("Titulo cambiado de \"%s\" a \"%s\".\n", libros[indice].titulo, nuevoTitulo);
    strcpy(libros[indice].titulo, nuevoTitulo);
}

void editarAutorLibro(Libro libros[], int numLibros) {
    printf("\n--- Editar Autor de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    int indice = buscarLibroPorId(libros, numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Autor actual: %s\n", libros[indice].autor);
    
    char nuevoAutor[MAX_AUTOR];
    while (!leerCadenaValida("Ingrese el nuevo autor: ", nuevoAutor, MAX_AUTOR));
    
    printf("Autor cambiado de \"%s\" a \"%s\".\n", libros[indice].autor, nuevoAutor);
    strcpy(libros[indice].autor, nuevoAutor);
}

void editarIdLibro(Libro libros[], int numLibros) {
    printf("\n--- Editar ID de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    int indice = buscarLibroPorId(libros, numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("ID actual: %d\n", libros[indice].id);
    
    int nuevoId;
    do {
        nuevoId = leerEnteroPositivo("Ingrese el nuevo ID: ");
        if (buscarLibroPorId(libros, numLibros, nuevoId) != -1 && nuevoId != libros[indice].id) {
            printf("Error: Ya existe un libro con este ID. Intente con otro.\n");
        }
    } while (buscarLibroPorId(libros, numLibros, nuevoId) != -1 && nuevoId != libros[indice].id);
    
    printf("ID cambiado de %d a %d.\n", libros[indice].id, nuevoId);
    libros[indice].id = nuevoId;
}

void editarAnioPublicacion(Libro libros[], int numLibros) {
    printf("\n--- Editar Año de Publicacion ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    int indice = buscarLibroPorId(libros, numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Año de publicacion actual: %d\n", libros[indice].anio_publicacion);
    
    int nuevoAnio = leerEnteroEntreLimites("Ingrese el nuevo año de publicacion (1000-2025): ", 1000, 2025);
    
    printf("Año de publicacion cambiado de %d a %d.\n", libros[indice].anio_publicacion, nuevoAnio);
    libros[indice].anio_publicacion = nuevoAnio;
}

void prestarLibro(Libro libros[], int numLibros) {
    printf("\n--- Prestar Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para prestar.\n");
        return;
    }
    
    printf("\nLibros disponibles:\n");
    printf("%-5s %-30s %-20s %-10s\n", "ID", "Titulo", "Autor", "Año");
    printf("----- ------------------------------ -------------------- ----------\n");
    
    int disponibles = 0;
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].estado, "Disponible") == 0) {
            printf("%-5d %-30s %-20s %-10d\n", 
                   libros[i].id, 
                   libros[i].titulo, 
                   libros[i].autor, 
                   libros[i].anio_publicacion);
            disponibles++;
        }
    }
    
    if (disponibles == 0) {
        printf("No hay libros disponibles para prestar.\n");
        return;
    }
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a prestar: ");
    int indice = buscarLibroPorId(libros, numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    if (strcmp(libros[indice].estado, "Prestado") == 0) {
        printf("El libro \"%s\" ya esta prestado.\n", libros[indice].titulo);
        return;
    }
    
    printf("\nLibro seleccionado para prestar:\n");
    printf("Titulo: %s\n", libros[indice].titulo);
    printf("Autor: %s\n", libros[indice].autor);
    
    if (confirmarAccion("¿Confirmar prestamo del libro?")) {
        strcpy(libros[indice].estado, "Prestado");
        printf("Libro prestado exitosamente. Estado actualizado a \"Prestado\".\n");
    } else {
        printf("Prestamo cancelado.\n");
    }
}

void devolverLibro(Libro libros[], int numLibros) {
    printf("\n--- Devolver Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para devolver.\n");
        return;
    }
    
    printf("\nLibros prestados:\n");
    printf("%-5s %-30s %-20s %-10s\n", "ID", "Titulo", "Autor", "Año");
    printf("----- ------------------------------ -------------------- ----------\n");
    
    int prestados = 0;
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].estado, "Prestado") == 0) {
            printf("%-5d %-30s %-20s %-10d\n", 
                   libros[i].id, 
                   libros[i].titulo, 
                   libros[i].autor, 
                   libros[i].anio_publicacion);
            prestados++;
        }
    }
    
    if (prestados == 0) {
        printf("No hay libros prestados para devolver.\n");
        return;
    }
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a devolver: ");
    int indice = buscarLibroPorId(libros, numLibros, id);
    
    if (indice == -1) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    if (strcmp(libros[indice].estado, "Disponible") == 0) {
        printf("El libro \"%s\" ya esta disponible, no se puede devolver.\n", libros[indice].titulo);
        return;
    }
    
    printf("\nLibro seleccionado para devolver:\n");
    printf("Titulo: %s\n", libros[indice].titulo);
    printf("Autor: %s\n", libros[indice].autor);
    
    if (confirmarAccion("¿Confirmar devolucion del libro?")) {
        strcpy(libros[indice].estado, "Disponible");
        printf("Libro devuelto exitosamente. Estado actualizado a \"Disponible\".\n");
    } else {
        printf("Devolucion cancelada.\n");
    }
}

void editarNombresMenu(char nombreBiblioteca[], Libro libros[], int numLibros) {
    int opcion;
    do {
        printf("\n--- Menu de Edicion de Nombres ---\n");
        printf("1. Editar nombre de la biblioteca\n");
        printf("2. Editar titulo de libro\n");
        printf("3. Editar autor de libro\n");
        printf("0. Volver al menu anterior\n");
        opcion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 3);
        
        switch (opcion) {
            case 1:
                editarNombreBiblioteca(nombreBiblioteca);
                break;
            case 2:
                editarTituloLibro(libros, numLibros);
                break;
            case 3:
                editarAutorLibro(libros, numLibros);
                break;
            case 0:
                printf("Volviendo al menu anterior.\n");
                break;
        }
    } while (opcion != 0);
}

void editarComponentesMenu(Libro libros[], int numLibros) {
    int opcion;
    do {
        printf("\n--- Menu de Edicion de Componentes ---\n");
        printf("1. Editar ID de libro\n");
        printf("2. Editar año de publicacion\n");
        printf("0. Volver al menu anterior\n");
        opcion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 2);
        
        switch (opcion) {
            case 1:
                editarIdLibro(libros, numLibros);
                break;
            case 2:
                editarAnioPublicacion(libros, numLibros);
                break;
            case 0:
                printf("Volviendo al menu anterior.\n");
                break;
        }
    } while (opcion != 0);
}