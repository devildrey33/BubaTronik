#pragma once

#include <DWLControlEx.h>
#include <DWLGraficos.h>
#include <DWLBotonEx.h>

#define ID_BOTON_CANCION_ATRAS		33005
#define ID_BOTON_CANCION_PLAY		33006
#define ID_BOTON_CANCION_PAUSA		33007
#define ID_BOTON_CANCION_STOP		33008
#define ID_BOTON_CANCION_ADELANTE	33009


enum Estados_Boton {
	Botonera_Estado_Normal		=  0,
	Botonera_Estado_Resaltado	=  1,
	Botonera_Estado_Presionado	=  2,
	Botonera_Estado_Desactivado	=  3
};


class Botonera : public DWL::ControlesEx::Base::DWLControlEx {
public:
												Botonera(void);
											   ~Botonera(void);
 inline void									Activado(const bool nActivar) { _Activado = nActivar; Repintar(); };
 inline BOOL									Activado(void) { return static_cast<BOOL>(_Activado); };
 HWND											CrearBotonera(HWND hWndParent, const int cX, const int cY);
 LRESULT										Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT										Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT										Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT										Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
 LRESULT										Evento_Mouse_Saliendo(void);
 void											Repintar(void);
 void											Boton_Atras_Desactivado(const bool Desactivado, const bool nRepintar = false);
 void											Boton_Play_Desactivado(const bool Desactivado, const bool nRepintar = false);
 void											Boton_Pausa_Desactivado(const bool Desactivado, const bool nRepintar = false);
 void											Boton_Stop_Desactivado(const bool Desactivado, const bool nRepintar = false);
 void											Boton_Adelante_Desactivado(const bool Desactivado, const bool nRepintar = false);
private:
// LRESULT CALLBACK								GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
// bool				Presionado;
 DWL::GDI::DWLRegion							Region;
 Estados_Boton									Estado_Atras;
 Estados_Boton									Estado_Play;
 Estados_Boton									Estado_Pausa;
 Estados_Boton									Estado_Stop;
 Estados_Boton									Estado_Adelante;
 DWL::ControlesEx::DWLBotonEx_ParametrosClick	Params;
 bool                                          _Activado;
};
