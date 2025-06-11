#include "funciones.h"

struct Buffer {
    int c;
};

struct CadenaInput {
    char buffer[100];
    size_t len;
};

struct EnteroInput {
    char buffer[100];
    int valor;
    char extra;
};

struct Confirmacion {
    char opcion;
};

struct Busqueda { 
    int i;
};


void limpiarBufferEntrada() {
    struct Buffer buffer;
    while ((buffer.c = getchar()) != '\n'); 
}

int leerCadenaValida(const char mensaje[], char destino[], int longitudMax) {
    struct CadenaInput input;
    
    printf("%s", mensaje);
    if (fgets(destino, longitudMax, stdin) != NULL) {
        input.len = strlen(destino);
        if (input.len > 0 && destino[input.len-1] == '\n') {
            destino[input.len-1] = '\0';
        } else {
            limpiarBufferEntrada();
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
    struct EnteroInput input;
    
    while (1) {
        printf("%s", mensaje);
        if (fgets(input.buffer, sizeof(input.buffer), stdin) != NULL) {
            if (sscanf(input.buffer, "%d %c", &input.valor, &input.extra) == 1) {
                return input.valor;
            }
        }
        printf("Error: Entrada invalida. Ingrese un numero entero valido.\n");
    }
}

int leerEnteroPositivo(const char mensaje[]) {
    struct EnteroInput input; 
    do {
        input.valor = leerEntero(mensaje);
        if (input.valor <= 0) {
            printf("Error: El valor debe ser positivo. Intente de nuevo.\n");
        }
    } while (input.valor <= 0);
    return input.valor;
}

int leerEnteroEntreLimites(const char mensaje[], int min, int max) {
    struct EnteroInput input; 
    do {
        input.valor = leerEntero(mensaje);
        if (input.valor < min || input.valor > max) {
            printf("Error: El valor debe estar entre %d y %d. Intente de nuevo.\n", min, max);
        }
    } while (input.valor < min || input.valor > max);
    return input.valor;
}

int confirmarAccion(const char mensaje[]) {
    struct Confirmacion conf;
    do {
        printf("%s (s/n): ", mensaje);
        conf.opcion = getchar();    
        limpiarBufferEntrada();     

        if (conf.opcion >= 'A' && conf.opcion <= 'Z') {
            conf.opcion = conf.opcion + ('a' - 'A'); 
        }
        
        if (conf.opcion != 's' && conf.opcion != 'n') {
            printf("Error: Ingrese 's' para si o 'n' para no.\n");
        }
    } while (conf.opcion != 's' && conf.opcion != 'n');
    return (conf.opcion == 's');
}

int buscarLibroPorId(struct Libro libros[], int numLibros, int id) {
    for (int i = 0; i < numLibros; i++) {
        if (libros[i].id == id) return i;
    }
    return -1;
}

int buscarLibroPorTitulo(struct Libro libros[], int numLibros, const char titulo[]) {
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].titulo, titulo) == 0) return i;
    }
    return -1;
}

struct Libro *buscarLibroPorAutor(struct Libro libros[], int numLibros, const char autorBuscar[]) {
    for (int i = 0; i < numLibros; i++) {
        if (strcmp(libros[i].autor, autorBuscar) == 0) {
            return &libros[i]; 
        }
    }
    return NULL; 
}

struct Libro *buscarLibroPorIdPtr(struct Libro libros[], int numLibros, int id) {
    for (int i = 0; i < numLibros; i++) {
        if (libros[i].id == id) {
            return &libros[i]; 
        }
    }
    return NULL; 
}

struct ResultadoBusquedaLibro buscarLibroCompleto(struct Libro libros[], int numLibros, int id) {
    struct ResultadoBusquedaLibro resultado;
    resultado.libro = NULL;
    resultado.posicion = -1;
    
    for (int i = 0; i < numLibros; i++) {
        if (libros[i].id == id) {
            resultado.libro = &libros[i];
            resultado.posicion = i;
            break;
        }
    }
    
    return resultado;
}

void mostrarMenuPrincipal(const char nombreBiblioteca[], int librosRegistrados) {
    printf("\n--- Menu Principal: Biblioteca %s ---\n", strlen(nombreBiblioteca) > 0 ? nombreBiblioteca : "[Sin Nombre]");
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

void registrarLibros(struct Libro libros[], int *numLibros) {
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
        
        struct Libro nuevoLibro;

        int id;
        do {
            id = leerEnteroPositivo("Ingrese el ID del libro: ");
            if (buscarLibroPorIdPtr(libros, *numLibros, id) != NULL) {
                printf("Error: Ya existe un libro con este ID. Intente con otro.\n");
            }
        } while (buscarLibroPorIdPtr(libros, *numLibros, id) != NULL);
        nuevoLibro.id = id;

        while (!leerCadenaValida("Ingrese el titulo del libro: ", nuevoLibro.titulo, MAX_TITULO));

        while (!leerCadenaValida("Ingrese el autor del libro: ", nuevoLibro.autor, MAX_AUTOR));

        nuevoLibro.anio_publicacion = leerEnteroEntreLimites("Ingrese el año de publicacion (1000-2025): ", 1000, 2025);

        strcpy(nuevoLibro.estado, "Disponible"); 

        libros[*numLibros] = nuevoLibro;
        (*numLibros)++;
        
        printf("Libro registrado exitosamente!\n");
    }
}

void mostrarLibros(struct Libro libros[], int numLibros) {
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

void buscarLibro(struct Libro libros[], int numLibros) {
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
        printf("3. Por autor\n"); 
        printf("0. Cancelar\n");
        opcion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 3); 
        
        switch (opcion) {
            case 1: {
                int id = leerEnteroPositivo("Ingrese el ID del libro a buscar: ");
                struct Libro *libroEncontrado = buscarLibroPorIdPtr(libros, numLibros, id); 
                
                if (libroEncontrado == NULL) {
                    printf("No se encontro un libro con el ID %d.\n", id);
                } else {
                    printf("\nInformacion del libro:\n");
                    printf("ID: %d\n", libroEncontrado->id);
                    printf("Titulo: %s\n", libroEncontrado->titulo);
                    printf("Autor: %s\n", libroEncontrado->autor);
                    printf("Año de publicacion: %d\n", libroEncontrado->anio_publicacion);
                    printf("Estado: %s\n", libroEncontrado->estado);
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
            case 3: { 
                char autor[MAX_AUTOR];
                while (!leerCadenaValida("Ingrese el autor del libro a buscar: ", autor, MAX_AUTOR));

                struct Libro *libroEncontradoPorAutor = buscarLibroPorAutor(libros, numLibros, autor);

                if (libroEncontradoPorAutor == NULL) {
                    printf("No se encontro un libro con el autor \"%s\".\n", autor);
                } else {
                    printf("\nInformacion del libro:\n");
                    printf("ID: %d\n", libroEncontradoPorAutor->id);
                    printf("Titulo: %s\n", libroEncontradoPorAutor->titulo);
                    printf("Autor: %s\n", libroEncontradoPorAutor->autor);
                    printf("Año de publicacion: %d\n", libroEncontradoPorAutor->anio_publicacion);
                    printf("Estado: %s\n", libroEncontradoPorAutor->estado);
                }
                return; 
            }
            case 0:
                printf("Busqueda cancelada.\n");
                return; 
        }
    } while (opcion != 0);
}

void actualizarEstadoLibro(struct Libro libros[], int numLibros) {
    printf("\n--- Actualizar Estado de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para actualizar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a actualizar: ");
    struct Libro *libroAActualizar = buscarLibroPorIdPtr(libros, numLibros, id); 
    
    if (libroAActualizar == NULL) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Titulo: %s\n", libroAActualizar->titulo);
    printf("Estado actual: %s\n", libroAActualizar->estado);
    
    printf("\nSeleccione el nuevo estado:\n");
    printf("1. Disponible\n");
    printf("2. Prestado\n");
    printf("0. Cancelar\n");
    
    int opcion = leerEnteroEntreLimites("Ingrese su opcion: ", 0, 2);
    
    switch (opcion) {
        case 1:
            strcpy(libroAActualizar->estado, "Disponible");
            printf("Estado actualizado a \"Disponible\".\n");
            break;
        case 2:
            strcpy(libroAActualizar->estado, "Prestado");
            printf("Estado actualizado a \"Prestado\".\n");
            break;
        case 0:
            printf("Actualizacion cancelada.\n");
            break;
    }
}

void eliminarLibro(struct Libro libros[], int *numLibros) {
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

void editarTituloLibro(struct Libro libros[], int numLibros) {
    printf("\n--- Editar Titulo de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    struct Libro *libroAEditar = buscarLibroPorIdPtr(libros, numLibros, id); 
    
    if (libroAEditar == NULL) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Titulo actual: %s\n", libroAEditar->titulo);
    
    char nuevoTitulo[MAX_TITULO];
    while (!leerCadenaValida("Ingrese el nuevo titulo: ", nuevoTitulo, MAX_TITULO));
    
    printf("Titulo cambiado de \"%s\" a \"%s\".\n", libroAEditar->titulo, nuevoTitulo);
    strcpy(libroAEditar->titulo, nuevoTitulo);
}

void editarAutorLibro(struct Libro libros[], int numLibros) {
    printf("\n--- Editar Autor de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    struct Libro *libroAEditar = buscarLibroPorIdPtr(libros, numLibros, id); 
    
    if (libroAEditar == NULL) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Autor actual: %s\n", libroAEditar->autor);
    
    char nuevoAutor[MAX_AUTOR];
    while (!leerCadenaValida("Ingrese el nuevo autor: ", nuevoAutor, MAX_AUTOR));
    
    printf("Autor cambiado de \"%s\" a \"%s\".\n", libroAEditar->autor, nuevoAutor);
    strcpy(libroAEditar->autor, nuevoAutor);
}

void editarIdLibro(struct Libro libros[], int numLibros) {
    printf("\n--- Editar ID de Libro ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    struct Libro *libroAEditar = buscarLibroPorIdPtr(libros, numLibros, id); 
    
    if (libroAEditar == NULL) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("ID actual: %d\n", libroAEditar->id);
    
    int nuevoId;
    do {
        nuevoId = leerEnteroPositivo("Ingrese el nuevo ID: ");
        struct Libro *libroConNuevoId = buscarLibroPorIdPtr(libros, numLibros, nuevoId);
        if (libroConNuevoId != NULL && libroConNuevoId != libroAEditar) {
            printf("Error: Ya existe un libro con este ID. Intente con otro.\n");
        }
    } while (buscarLibroPorIdPtr(libros, numLibros, nuevoId) != NULL && nuevoId != libroAEditar->id);
    
    printf("ID cambiado de %d a %d.\n", libroAEditar->id, nuevoId);
    libroAEditar->id = nuevoId;
}

void editarAnioPublicacion(struct Libro libros[], int numLibros) {
    printf("\n--- Editar Año de Publicacion ---\n");
    
    if (numLibros == 0) {
        printf("No hay libros registrados para editar.\n");
        return;
    }
    
    mostrarLibros(libros, numLibros);
    
    int id = leerEnteroPositivo("\nIngrese el ID del libro a editar: ");
    struct Libro *libroAEditar = buscarLibroPorIdPtr(libros, numLibros, id); 
    
    if (libroAEditar == NULL) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    printf("\nLibro seleccionado:\n");
    printf("Año de publicacion actual: %d\n", libroAEditar->anio_publicacion);
    
    int nuevoAnio = leerEnteroEntreLimites("Ingrese el nuevo año de publicacion (1000-2025): ", 1000, 2025);
    
    printf("Año de publicacion cambiado de %d a %d.\n", libroAEditar->anio_publicacion, nuevoAnio);
    libroAEditar->anio_publicacion = nuevoAnio;
}

void prestarLibro(struct Libro libros[], int numLibros) {
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
    struct Libro *libroAPrestar = buscarLibroPorIdPtr(libros, numLibros, id); 
    
    if (libroAPrestar == NULL) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    if (strcmp(libroAPrestar->estado, "Prestado") == 0) {
        printf("El libro \"%s\" ya esta prestado.\n", libroAPrestar->titulo);
        return;
    }
    
    printf("\nLibro seleccionado para prestar:\n");
    printf("Titulo: %s\n", libroAPrestar->titulo);
    printf("Autor: %s\n", libroAPrestar->autor);
    
    if (confirmarAccion("¿Confirmar prestamo del libro?")) {
        strcpy(libroAPrestar->estado, "Prestado");
        printf("Libro prestado exitosamente. Estado actualizado a \"Prestado\".\n");
    } else {
        printf("Prestamo cancelado.\n");
    }
}

void devolverLibro(struct Libro libros[], int numLibros) {
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
    struct Libro *libroADevolver = buscarLibroPorIdPtr(libros, numLibros, id); // Usamos la versión con puntero
    
    if (libroADevolver == NULL) {
        printf("No se encontro un libro con el ID %d.\n", id);
        return;
    }
    
    if (strcmp(libroADevolver->estado, "Disponible") == 0) {
        printf("El libro \"%s\" ya esta disponible, no se puede devolver.\n", libroADevolver->titulo);
        return;
    }
    
    printf("\nLibro seleccionado para devolver:\n");
    printf("Titulo: %s\n", libroADevolver->titulo);
    printf("Autor: %s\n", libroADevolver->autor);
    
    if (confirmarAccion("¿Confirmar devolucion del libro?")) {
        strcpy(libroADevolver->estado, "Disponible");
        printf("Libro devuelto exitosamente. Estado actualizado a \"Disponible\".\n");
    } else {
        printf("Devolucion cancelada.\n");
    }
}

void editarNombresMenu(char nombreBiblioteca[], struct Libro libros[], int numLibros) {
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

void editarComponentesMenu(struct Libro libros[], int numLibros) {
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