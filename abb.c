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
abb_nodo_t *buscar_menor_der(abb_t *arbol, abb_nodo_t *nodo){
    abb_nodo_t *menor_der = NULL;
    if(nodo->der) menor_der = nodo->der;
    while true{
        if(menor_der->izq) menor_der = menor_der->izq;
        else break
    }
    return menor_der;
}

void swap(abb_nodo_t *nodo1, abb_nodo_t *nodo2){
        void *dato_auxiliar = nodo1->dato;
        char *clave_auxiliar = nodo1->clave;
        nodo1->clave = nodo2->clave;
        nodo1->dato = nodo2->dato;
        nodo2->dato = dato_auxiliar;
        nodo2->clave = clave_auxiliar;
}

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
    int comparacion = (arbol->cmp(nodo->clave, clave));
    if(!comparacion) return true;
    else if((comparacion > 0 && !nodo->izq) || (comparacion < 0 && !nodo->der)) return false;
    else if(comparacion > 0) return abb_pertenece_auxiliar(arbol, clave, nodo->izq);
    else return abb_pertenece_auxiliar(arbol, clave, nodo->der;
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

void *abb_borrar_auxiliar(abb_t *arbol, abb_nodo_t *nodo){
    int comparacion = arbol->cmp(nodo->clave, nodo->padre->clave);
    if(!nodo->izq && !nodo->der){
        if(comparacion > 0) nodo->padre->der = NULL;
        else nodo->padre->izq = NULL;
    }

    else if((!nodo->izq) || (!nodo->der)){
        if(nodo->izq){
            if(comparacion > 0) nodo->padre->der = nodo->izq;
            else nodo->padre->izq = nodo->izq  
        }
        else{
            if(comparacion < 0) nodo->padre->der = nodo->der;
            else nodo->padre->izq = nodo->der;
        }
    }

    else{
        abb_nodo_t *menor_der = buscar_menor_der(arbol, nodo);
        swap(nodo, menor_der);
        void *dato_auxiliar = abb_borrar_auxiliar(arbol, menor_der);
        return dato_auxiliar;

    }

    void *dato_auxiliar = nodo->dato;
    destruir_nodo(nodo);
    return dato_auxiliar;

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
    nodo_t *a_borrar = abb_obtener(arbol, clave);
    abb_borrar_auxiliar(arbol, a_borrar);
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






