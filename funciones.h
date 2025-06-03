#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NOMBRE_BIBLIOTECA 100
#define MAX_LIBROS 10
#define MAX_TITULO 100
#define MAX_AUTOR 50
#define MAX_ESTADO 20

typedef struct {
    int id;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    int anio_publicacion;
    char estado[MAX_ESTADO];
} Libro;

// Prototipos de funciones
void limpiarBufferEntrada();
int leerCadenaValida(const char mensaje[], char destino[], int longitudMax);
int leerEntero(const char mensaje[]);
int leerEnteroPositivo(const char mensaje[]);
int leerEnteroEntreLimites(const char mensaje[], int min, int max);
int confirmarAccion(const char mensaje[]);
int buscarLibroPorId(Libro libros[], int numLibros, int id);
int buscarLibroPorTitulo(Libro libros[], int numLibros, const char titulo[]);
void mostrarMenuPrincipal(const char nombreBiblioteca[], int librosRegistrados);
void registrarBiblioteca(char nombreBiblioteca[]);
void registrarLibros(Libro libros[], int *numLibros);
void mostrarLibros(Libro libros[], int numLibros);
void buscarLibro(Libro libros[], int numLibros);
void actualizarEstadoLibro(Libro libros[], int numLibros);
void eliminarLibro(Libro libros[], int *numLibros);
void editarNombreBiblioteca(char nombreBiblioteca[]);
void editarTituloLibro(Libro libros[], int numLibros);
void editarAutorLibro(Libro libros[], int numLibros);
void editarIdLibro(Libro libros[], int numLibros);
void editarAnioPublicacion(Libro libros[], int numLibros);
void prestarLibro(Libro libros[], int numLibros);
void devolverLibro(Libro libros[], int numLibros);
void editarNombresMenu(char nombreBiblioteca[], Libro libros[], int numLibros);
void editarComponentesMenu(Libro libros[], int numLibros);

#endif
