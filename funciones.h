#include <stdio.h>
#include <string.h>

#define MAX_NOMBRE_BIBLIOTECA 100
#define MAX_LIBROS 10
#define MAX_TITULO 100
#define MAX_AUTOR 50
#define MAX_ESTADO 20

struct Libro {
    int id;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    int anio_publicacion;
    char estado[MAX_ESTADO];
};

struct ResultadoBusquedaLibro {
    struct Libro *libro;
    int posicion;
};

void limpiarBufferEntrada();
int leerCadenaValida(const char mensaje[], char destino[], int longitudMax);
int leerEntero(const char mensaje[]);
int leerEnteroPositivo(const char mensaje[]);
int leerEnteroEntreLimites(const char mensaje[], int min, int max);
int confirmarAccion(const char mensaje[]);

int buscarLibroPorId(struct Libro libros[], int numLibros, int id);
int buscarLibroPorTitulo(struct Libro libros[], int numLibros, const char titulo[]);
struct Libro *buscarLibroPorAutor(struct Libro libros[], int numLibros, const char autorBuscar[]);
struct Libro *buscarLibroPorIdPtr(struct Libro libros[], int numLibros, int id);
struct ResultadoBusquedaLibro buscarLibroCompleto(struct Libro libros[], int numLibros, int id);

void mostrarMenuPrincipal(const char nombreBiblioteca[], int librosRegistrados);
void registrarBiblioteca(char nombreBiblioteca[]);
void registrarLibros(struct Libro libros[], int *numLibros);
void mostrarLibros(struct Libro libros[], int numLibros);
void buscarLibro(struct Libro libros[], int numLibros);
void actualizarEstadoLibro(struct Libro libros[], int numLibros);
void eliminarLibro(struct Libro libros[], int *numLibros);
void editarNombreBiblioteca(char nombreBiblioteca[]);
void editarTituloLibro(struct Libro libros[], int numLibros);
void editarAutorLibro(struct Libro libros[], int numLibros);
void editarIdLibro(struct Libro libros[], int numLibros);
void editarAnioPublicacion(struct Libro libros[], int numLibros);
void prestarLibro(struct Libro libros[], int numLibros);
void devolverLibro(struct Libro libros[], int numLibros);
void editarNombresMenu(char nombreBiblioteca[], struct Libro libros[], int numLibros);
void editarComponentesMenu(struct Libro libros[], int numLibros);


