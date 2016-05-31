#pragma once

#include <DWLControlEx.h>

class ControlTiempoActual : public DWL::ControlesEx::Base::DWLControlEx {
public:
					ControlTiempoActual(void);
				   ~ControlTiempoActual(void);
 HWND				CrearCTA(HWND hWndParent, const int nX, const int nY);
 void				Repintar(HDC hDC, const bool Forzar = false);
 LRESULT			Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
// LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
