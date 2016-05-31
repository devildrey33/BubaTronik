#ifndef BUBATRONIK_TGRUPO
#define BUBATRONIK_TGRUPO

#include "TGenero.h"

class NodoArbolBaseDatos;

// clase que engloba un grupo
class TGrupo {
  public:
                          TGrupo(void) { Canciones = 0; Discos = 0; Longitud = 0; Tiempo = 0; Nodo = NULL; };
                         ~TGrupo(void) { };
	StrTp                *Nombre;
	StrTp                *StrTipo;
	StrTp                *StrGenero;
	TTipo                *Tipo;
	TGenero              *Genero;
	unsigned int          Canciones;
	unsigned int          Discos;
	DWORD	              Longitud;
	UINT64                Tiempo;
	NodoArbolBaseDatos   *Nodo;
};

#endif