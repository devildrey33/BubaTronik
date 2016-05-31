#include "StdAfx.h"
#include "ObjetoEscena.h"
#include "DimensionesFijas.h"
#include <DWLListaIconos.h>
#include "Escena.h"

ObjetoEscena::ObjetoEscena(void) : _Objeto(NULL), _Tipo(ObjetoEscena_NULL) { 
};


ObjetoEscena::ObjetoEscena(TipoObjetoEscena TipoObjeto, LPVOID NuevoObjeto, const UINT nAncho, const UINT nAlto, AlineacionObjetoEscena nAlineacion, HWND nhWnd) :  
	_Tipo(TipoObjeto), _Objeto(NuevoObjeto), _Alto(nAlto), _Ancho(nAncho), _Alineacion(nAlineacion), _hWnd(nhWnd) {
};


ObjetoEscena::~ObjetoEscena(void) { 
	if (_Objeto != NULL) {
		switch (_Tipo) {
			case ObjetoEscena_Separacion		:	_Objeto = 0; 																		break;
			case ObjetoEscena_Texto				:	delete reinterpret_cast<ObjetoEscenaTexto *>(_Objeto); 								    break;
			case ObjetoEscena_Boton				:	delete reinterpret_cast<DWL::ControlesEx::DWLBotonEx *>(_Objeto); 					break;
			case ObjetoEscena_Marca				:	delete reinterpret_cast<ControlMarca *>(_Objeto); 									break;
			case ObjetoEscena_FrameH			:	delete reinterpret_cast<ObjetoEscenaFrame *>(_Objeto); 								break;
			case ObjetoEscena_FrameV			:	delete reinterpret_cast<ObjetoEscenaFrame *>(_Objeto); 								break;
			case ObjetoEscena_Barra				:	delete reinterpret_cast<DWL::ControlesEx::DWLBarraEx *>(_Objeto); 					break;
			case ObjetoEscena_Lista				:	delete reinterpret_cast<DWL::ControlesEx::DWLListaEx *>(_Objeto); 					break;
			case ObjetoEscena_Arbol				:	delete reinterpret_cast<DWL::ControlesEx::DWLArbolEx *>(_Objeto); 					break;
			case ObjetoEscena_ComboBox			:	delete reinterpret_cast<DWL::ControlesEx::DWLEdicionDesplegableEx *>(_Objeto); 		break;
			case ObjetoEscena_Icono				:   delete reinterpret_cast<ObjetoEscenaIcono *>(_Objeto);													break;

			case ObjetoEscena_TeclaRapida		:	delete reinterpret_cast<ControlTeclaRapida *>(_Objeto); 							break;
		}
	}
};


const int ObjetoEscena::ID(void) {
	switch (_Tipo) {
		case ObjetoEscena_Boton				:	return reinterpret_cast<DWL::ControlesEx::DWLBotonEx *>(_Objeto)->ID();
		case ObjetoEscena_Marca				:	return reinterpret_cast<ControlMarca *>(_Objeto)->ID();
		case ObjetoEscena_Barra				:	return reinterpret_cast<DWL::ControlesEx::DWLBarraEx *>(_Objeto)->ID(); 				
		case ObjetoEscena_Lista				:	return reinterpret_cast<DWL::ControlesEx::DWLListaEx *>(_Objeto)->ID(); 				
		case ObjetoEscena_Arbol				:	return reinterpret_cast<DWL::ControlesEx::DWLArbolEx *>(_Objeto)->ID(); 				
		case ObjetoEscena_ComboBox			:	return reinterpret_cast<DWL::ControlesEx::DWLEdicionDesplegableEx *>(_Objeto)->ID();
		case ObjetoEscena_Icono				:   return reinterpret_cast<ObjetoEscenaIcono *>(_Objeto)->IDIcono;

		case ObjetoEscena_TeclaRapida		:	return reinterpret_cast<ControlTeclaRapida *>(_Objeto)->ID();
	}		
	return 0;
};
