#pragma once

#include <DWLVentana.h>
#include <DWLBotonEx.h>
#include "Teclado.h"

class ControlTeclaRapida : public DWL::Ventanas::DWLVentana {
public :
										ControlTeclaRapida(void);
									   ~ControlTeclaRapida(void);
 void									Crear(HWND hWndParent, const TCHAR *nTexto, const int cX, const int cY, const int cAncho, const int cAlto, const UINT cID, Tecla *nTecla);
 LRESULT								Evento_Teclado_TeclaSoltada(const UINT Tecla, const UINT Repeticion, const UINT Param);
 LRESULT								Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
 LRESULT								Evento_BotonEx_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
 const TCHAR						   *TextoTecla(const int TeclaVirtual);
 void									Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool Repintar = false);
 void                                   Pintar(HDC hDCDest, RECT *Espacio);
protected :
 DWL::ControlesEx::DWLBotonEx			BotonEditar;
// LRESULT CALLBACK						GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
 bool									CaptandoTecla;
 Tecla								   *TeclaRapida;
 DWL::DWLString							TextoResultado;
 DWL::DWLString							TextoControl;
};
