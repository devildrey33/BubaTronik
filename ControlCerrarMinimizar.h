#pragma once

#include <DWLControlEx.h>

enum EstadosCM {
	CM_Normal				= 0,
	CM_Minimizar_Resaltado	= 1,
	CM_Minimizar_Presionado	= 2,
	CM_Restaurar_Resaltado	= 3,
	CM_Restaurar_Presionado	= 4,
	CM_Cerrar_Resaltado		= 5,
	CM_Cerrar_Presionado	= 6
};

class ControlCerrarMinimizar : public DWL::ControlesEx::Base::DWLControlEx {
public:
					ControlCerrarMinimizar(void);
				   ~ControlCerrarMinimizar(void);
 void				Crear(HWND hWndParent, const int cX, const int cY, const UINT nID, const bool SoloCerrar = false);
 void				Repintar(HDC hDC = NULL);
 LRESULT			Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
 inline LRESULT		Evento_BorrarFondo(HDC hDC) { return 0; };
 LRESULT			Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT			Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT			Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT			Evento_Mouse_Saliendo(void);
 void				Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool Repintar = false);
protected:
// LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
 EstadosCM			Estado;
 bool				Presionado;
 bool			   _SoloCerrar;
};
