#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char * a_dpulicar){
    char *duplicado = malloc((strlen(a_duplicar) + 1) * sizeof(char));
    strcpy(duplicado, clave);
    return duplicado;
}

//Estructuras de arbol--------------------------------------------------------------------
struct abb {
    size_t cantidad;
    abb_nodo_t *raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
}

typedef struct abb_nodo_t abb_nodo_t;

struct abb_nodo_t {
    char *clave;
    void *valor;
    abb_nodo_t *izq;
    abb_nodo_t *der;
    abb_nodo_t *padre;
}

//Primitivas de nodos--------------------------------------------------------------------
abb_nodo_t *nodo_crear(char *clave; void *valor, abb_nodo_t *padre){
    abb_nodo_t *nodo = malloc(sizeof(abb_nodo_t));
    if(!nodo) return NULL;
    nodo->clave = strdup(clave);
    nodo->valor = valor;
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->padre = padre;
    return nodo;
}

//Funciones auxiliares-----------------------------------------------------------------------

bool abb_guardar_auxiliar(abb_t *arbol; abb_nodo_t *nodo, const char *clave; void *valor){
    int comparacion = (arbol->cmp(nodo->clave, clave));
    if(!comparacion){
        arbol->destruir_dato(nodo->valor);
        nodo->valor = valor;
        return true;
    }

    if((comparacion > 0 && !nodo->izq) || (comparacion < 0 && !nodo->der)){
        abb_nodo_t *nodo_nuevo = nodo_crear(clave, valor, nodo);
        if(comparacion > 0) nodo->izq = nodo_nuevo;
        else nodo->der = nodo_nuevo;
        return true;
    }

    else if(comparacion > 0) return abb_guardar_auxiliar(arbol, nodo->izq, clave, valor);
    else return abb_guardar_auxiliar(arbol, nodo->der, clave, valor);

}

bool abb_pertenece_auxiliar(const abb_t *arbol; const char *clave; abb_nodo_t *nodo){
    if(nodo == NULL) return false;
    if(!arbol->cmp(nodo->clave, clave)) return true;
    if(!nodo->izq && !nodo->der) return false;
    return(abb_pertenece_auxiliar(arbol, clave, nodo->izq) || abb_pertenece_auxiliar(arbol, clave, nodo->der));
}

void *abb_obtener_auxiliar(const abb_t *arbol; const char *clave; abb_nodo_t *nodo){
    if(nodo == NULL) return NULL;
    int comparacion = (arbol->cmp(nodo->clave, clave));
    if(!comparacion) return nodo->valor;
    else if(comparacion > 0) return abb_obtener_auxiliar(arbol, clave, nodo->izq);
    else return abb_obtener_auxiliar(arbol, clave, nodo->der);
}

void destruir_nodo(abb_t *arbol; abb_nodo_t *nodo){
    if(nodo){
        free(nodo->clave);
        if(arbol->destruir_dato) arbol->destruir_dato(nodo->valor);
    }
}

void destruir_nodos(abb_t *arbol, abb_nodo_t *nodo){
    if(!nodo) return;
    destruir_nodos(arbol, nodo->izq);
    destruir_nodos(arbol, nodo->der);
    destruir_nodo(arbol, nodo);
}

void *abb_borrar_auxiliar(abb_t *arbol, const char *clave, abb_nodo_t *nodo){  
}

//Primitivas arbol-----------------------------------------------------------------------
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->raiz = NULL;
    abb->cantidad = 0;
    abb->destruir_dato = destruir_dato;
    abb->cmp = cmp;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol->raiz) arbol->raiz = nodo_crear(clave, valor, NULL);
    else{
        if(!(abb_guardar_auxiliar(arbol, arbol->raiz, clave, dato))) return false;
    }
    arbol->cantidad++;
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    abb_borrar_auxiliar(arbol, clave, arbol->raiz);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    return abb_obtener(arbol, clave, arbol->raiz);
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    return (abb_guardar_auxiliar(arbol, clave; arbol->raiz));
}

size_t abb_cantidad(abb_t *arbol){
    if(arbol->raiz == NULL) return 0;
    return(arbol->cantidad);
}

void abb_destruir(abb_t *arbol){
    destruir_nodos(arbol, arbol->raiz);
    free(arbol);
}

//Iterador interno-----------------------------------------------------------------------
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

//Iterador externo-----------------------------------------------------------------------

typedef struct abb_iter abb_iter_t;

abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
bool abb_iter_in_avanzar(abb_iter_t *iter);
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
bool abb_iter_in_al_final(const abb_iter_t *iter);
void abb_iter_in_destruir(abb_iter_t* iter);



