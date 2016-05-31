#ifndef BUBATRONIK_ITEMLISTAMEDIOS
#define BUBATRONIK_ITEMLISTAMEDIOS

#include <DWLListaEx.h>

class TMedio;

// Items para la lista de canciones
class ItemListaMedios : public DWL::ControlesEx::DWLListaEx_Item {
  public :
				ItemListaMedios(void) { Existe = false; Medio = NULL; };
				ItemListaMedios(DWL::ControlesEx::DWLListaEx_Nucleo *This) { DWL::ControlesEx::DWLListaEx_Item::DWLListaEx_Item(This); Existe = false; Medio = NULL; };
		       ~ItemListaMedios(void) { };
	TMedio	   *Medio;
	bool		Existe;
	inline void	Siguiente(ItemListaMedios *Nuevo) {	_Siguiente = Nuevo; };
	inline void	Anterior(ItemListaMedios *Nuevo)  {	_Anterior  = Nuevo;	};
};

#endif