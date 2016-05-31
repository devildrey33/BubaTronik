#pragma once

#include <DWLVentana.h>
#include <DWLBarraEx.h>

class VentanaDump : public DWL::Ventanas::DWLVentana {
  public:
									VentanaDump(void);
	                               ~VentanaDump(void);
	const BOOL						Mostrar(const TCHAR *PathDump);
//	const BOOL						Mostrar(struct _EXCEPTION_POINTERS* ExceptionInfo);

    LRESULT							Evento_Cerrar(void);

    LRESULT 						Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);

    LRESULT							Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);

  protected :
	void						   _PintarTextoSombra(HDC hDC, const TCHAR *Texto, COLORREF ColTexto, COLORREF ColSombra, UINT cX, UINT cY);
	DWL::ControlesEx::DWLBarraEx   _Barra;
    DWL::ControlesEx::DWLBotonEx   _BotonTerminar;
    DWL::ControlesEx::DWLBotonEx   _BotonGuardar;
	DWL::DWLString				   _PathDump;
};

