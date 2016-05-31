#ifndef BUBATRONIK_TGENERO
#define BUBATRONIK_TGENERO

#include "TTipo.h"

class NodoArbolBaseDatos;


// clase que engloba un genero
class TGenero {
  public:
							TGenero(void) { Canciones = 0; Grupos = 0; Discos = 0; Longitud = 0; Tiempo = 0; Nodo = NULL; };
						   ~TGenero(void) { };
	StrTp                  *Nombre;
	StrTp                  *StrTipo;
	TTipo                  *Tipo;
	UINT64                  Tiempo;
	DWORD                   Longitud;
	UINT					Canciones;
	UINT				    Grupos;
	UINT		            Discos;
	NodoArbolBaseDatos     *Nodo;
};

#endif