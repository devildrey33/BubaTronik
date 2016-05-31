#ifndef BUBATRONIK_TDISCO
#define BUBATRONIK_TDISCO

#include "TGrupo.h"

class NodoArbolBaseDatos;


// clase que engloba un disco
class TDisco {
  public:
							TDisco(void) { Canciones = 0; Longitud = 0; Tiempo = 0; Nodo = NULL; };
                           ~TDisco(void) { };
	StrTp                  *Nombre;
	StrTp                  *StrTipo;
	StrTp                  *StrGenero;
	StrTp                  *StrGrupo;
	TTipo                  *Tipo; 
	TGenero                *Genero;
	TGrupo                 *Grupo;
	unsigned int			Canciones;
	DWORD					Longitud;
	UINT64					Tiempo;
	NodoArbolBaseDatos     *Nodo;
};

#endif