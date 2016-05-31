#ifndef BUBATRONIK_NODOARBOLBASEDATOS
#define BUBATRONIK_NODOARBOLBASEDATOS

#include <DWLArbolEx.h>

class TTipo;
class TGenero;
class TGrupo;
class TDisco;
class TMedio;

// Nodos para la base de datos
class NodoArbolBaseDatos : public DWL::ControlesEx::DWLArbolEx_Nodo {
  public :
					NodoArbolBaseDatos(void) { Tipo = NULL; Genero = NULL; Grupo = NULL; Disco = NULL, Medio = NULL; /* RootM3U = NULL; ListaM3U = NULL; CDRom = NULL; */ };
			       ~NodoArbolBaseDatos(void);
	TTipo          *Tipo;
	TGenero        *Genero;
	TGrupo         *Grupo;
	TDisco         *Disco;
	TMedio         *Medio;
//	TodosLosM3US   *RootM3U;
//  TM3U           *ListaM3U;
//  UnidadCDRom    *CDRom;
};


#endif