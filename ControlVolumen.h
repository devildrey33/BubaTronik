#pragma once

#include <DWLBarraEx.h>

class ControlVolumen : public DWL::ControlesEx::DWLBarraEx {
public:
														ControlVolumen(void) { };
													   ~ControlVolumen(void) { };
 HWND													Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const UINT cID);
 LRESULT												Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT												Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT												Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 void													Valor(const long nValor);
//private:
// LRESULT CALLBACK										GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
// static DWL::ControlesEx::ColoresEx::DWLBarraEx_Colores	ColoresVolumen;
};
