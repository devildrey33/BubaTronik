#pragma once

#include <DWLArbolEx.h>

class ArbolOpcionesBasicas : public DWL::ControlesEx::DWLArbolEx {
public:
			ArbolOpcionesBasicas(void);
		   ~ArbolOpcionesBasicas(void);
 HWND       Crear(HWND hWndParent, const int cID, const int cX, const int cY, const int cAncho, const int cAlto);
 const int  CalcularAncho(HWND hWnd);
 void		RecargarIdioma(void);
// void       Evento_Pintar_Fondo(HDC hDC, RECT *Espacio);
};
