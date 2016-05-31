#ifndef BUBATRONIK_TTIPO
#define BUBATRONIK_TTIPO

#include "StrTp.h"

class NodoArbolBaseDatos;

// clase que engloba un tipo
class TTipo {
  public:
						TTipo(void) { Canciones = 0; Generos = 0; Grupos = 0; Discos = 0; Longitud = 0; Tiempo = 0; Nodo = NULL; };
                       ~TTipo(void) { };
	StrTp              *Nombre;
	UINT64              Tiempo;
	DWORD               Longitud;
	UINT				Canciones;
	UINT			    Generos;
	UINT		        Grupos;
	UINT				Discos;
	NodoArbolBaseDatos *Nodo;
};

#endif