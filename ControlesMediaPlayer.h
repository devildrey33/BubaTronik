#pragma once

#include <DWLVentana.h>
#include <DWLBarraEx.h>
#include "Botonera.h"
#include "ShufleRepeat.h"
#include "ControlVolumen.h"
#include "ControlTiempoActual.h"

class ControlesMediaPlayer : public DWL::Ventanas::DWLVentana {
public:
								ControlesMediaPlayer(void);
							   ~ControlesMediaPlayer(void);
 void							Crear(const bool Abajo);
 void							Mostrar(HWND Controlador, const bool Abajo);
 void							Ocultar(HWND Controlador);
 LRESULT						Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
// LRESULT						Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT						Evento_BarraEx_Mouse_BotonSoltado(const UINT cID, const UINT nBoton);
 LRESULT						Evento_BotonEx_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
 LRESULT						Evento_Cerrar(void);
 DWL::ControlesEx::DWLBarraEx	SliderTiempo;
 Botonera						Botones;
 ControlVolumen					Volumen;
 ControlTiempoActual			TiempoStrActual;
 ShufleRepeat					BotonShufleRepeat;
protected:
// LRESULT CALLBACK				GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
