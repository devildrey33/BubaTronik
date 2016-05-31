#pragma once

#include "ObjetoEscenaFrame.h"


#include "ControlTeclaRapida.h"


class Escena;


// Objeto general que ocupara todo el ancho disponible en la escena
class ObjetoEscena {
  public:
												ObjetoEscena(void);
												ObjetoEscena(TipoObjetoEscena TipoObjeto, LPVOID NuevoObjeto, const UINT nAncho, const UINT nAlto, AlineacionObjetoEscena nAlineacion = Alineacion_Expandible, HWND hWnd = NULL);
											   ~ObjetoEscena(void);

    const int									ID(void);
 
    const TipoObjetoEscena						Tipo(void)					{ return _Tipo; }
    const int									Separacion(void)			{ return reinterpret_cast<int>(_Objeto); };
    const ObjetoEscenaIcono					   *Icono(void)					{ return reinterpret_cast<ObjetoEscenaIcono *>(_Objeto); };
	ObjetoEscenaFrame						   *FrameV(void)				{ return reinterpret_cast<ObjetoEscenaFrame *>(_Objeto); }
	ObjetoEscenaFrame						   *FrameH(void)				{ return reinterpret_cast<ObjetoEscenaFrame *>(_Objeto); }
    DWL::ControlesEx::DWLListaEx			   *Lista(void)					{ return reinterpret_cast<DWL::ControlesEx::DWLListaEx *>(_Objeto); };
    DWL::ControlesEx::DWLArbolEx			   *Arbol(void)					{ return reinterpret_cast<DWL::ControlesEx::DWLArbolEx *>(_Objeto); };
    ObjetoEscenaTexto						   *Texto(void)					{ return reinterpret_cast<ObjetoEscenaTexto *>(_Objeto); };
    DWL::ControlesEx::DWLBotonEx	  		   *Boton(void)					{ return reinterpret_cast<DWL::ControlesEx::DWLBotonEx *>(_Objeto); };
    ControlMarca					  		   *Marca(void)					{ return reinterpret_cast<ControlMarca *>(_Objeto); };
    DWL::ControlesEx::DWLBarraEx			   *Barra(void)					{ return reinterpret_cast<DWL::ControlesEx::DWLBarraEx *>(_Objeto); };
    DWL::ControlesEx::DWLEdicionDesplegableEx  *ComboBox(void)				{ return reinterpret_cast<DWL::ControlesEx::DWLEdicionDesplegableEx *>(_Objeto); };

	// POR ELIMINAR //
	ControlTeclaRapida						   *TeclaRapida(void)			{ return reinterpret_cast<ControlTeclaRapida *>(_Objeto); };
	//////////////////

	inline const AlineacionObjetoEscena         Alineacion(void)			{ return _Alineacion; };
    inline const UINT                           Alto(void)					{ return _Alto; };
    inline void                                 Alto(const UINT nAlto)		{ _Alto = nAlto; };
    inline const UINT                           Ancho(void)					{ return _Ancho; };
    inline void                                 Ancho(const UINT nAncho)	{ _Ancho = nAncho; };
	inline HWND                                 hWnd(void)                  { return _hWnd; };
  protected:
    AlineacionObjetoEscena                     _Alineacion;
    LPVOID 									   _Objeto;
    TipoObjetoEscena						   _Tipo;
    UINT                                       _Alto;
    UINT                                       _Ancho;
    HWND                                       _hWnd;
    friend class								Escena;
};


