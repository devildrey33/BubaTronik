#include "StdAfx.h"
#include "Botonera.h"
#include "Resource.h"
#include "BubaTronikApp.h"
#include "DimensionesFijas.h"


Botonera::Botonera(void) {
	Estado_Atras	= Botonera_Estado_Normal;
	Estado_Play		= Botonera_Estado_Normal;
	Estado_Pausa	= Botonera_Estado_Normal;
	Estado_Stop		= Botonera_Estado_Normal;
	Estado_Adelante = Botonera_Estado_Normal;
	_Activado = true;
}

Botonera::~Botonera(void) {
}

/*LRESULT CALLBACK Botonera::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		ENLACE_Evento_Mouse_Movimiento();
		ENLACE_Evento_Mouse_BotonPresionado();
		ENLACE_Evento_Mouse_BotonSoltado();
//		ENLACE_Evento_Mouse_Saliendo();
		ENLACE_Evento_PintarHDC();
		case WM_ERASEBKGND	:  return 1;	
	}
	return DWL::ControlesEx::DWLBaseControlEx::GestorMensajes(uMsg, wParam, lParam);
}*/


HWND Botonera::CrearBotonera(HWND hWndParent, const int cX, const int cY) {
	// 24 [pixeles] * 5 [botones] = 120 pixeles de ancho
	Region.CrearRegionRectaRedondeada(0, 0, (ANCHO_BOTON_BOTONERA * 5), ALTO_BOTONERA, 13, 13);
	CrearControl(hWndParent, TEXT("BotoneraPlayer"), WS_CHILD, TEXT(""), cX, cY, (ANCHO_BOTON_BOTONERA * 5), ALTO_BOTONERA, 100);
	Region.AsignarRegionVentana(_hWnd);
	Visible(true);
	return _hWnd;
}	


LRESULT Botonera::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
//	_MirarMouseLeave();
	if (_Activado == false) return 0;

	if (Estado_Atras	!= Botonera_Estado_Desactivado  &&  Estado_Atras	!= Botonera_Estado_Presionado)	Estado_Atras	= Botonera_Estado_Normal;
	if (Estado_Play		!= Botonera_Estado_Desactivado  &&  Estado_Play		!= Botonera_Estado_Presionado)	Estado_Play		= Botonera_Estado_Normal;
	if (Estado_Pausa	!= Botonera_Estado_Desactivado  &&  Estado_Pausa	!= Botonera_Estado_Presionado)	Estado_Pausa	= Botonera_Estado_Normal;
	if (Estado_Stop		!= Botonera_Estado_Desactivado  &&  Estado_Stop		!= Botonera_Estado_Presionado)	Estado_Stop		= Botonera_Estado_Normal;
	if (Estado_Adelante != Botonera_Estado_Desactivado  &&  Estado_Adelante != Botonera_Estado_Presionado)	Estado_Adelante	= Botonera_Estado_Normal;

	if (cX > 0 && cX < ANCHO_BOTON_BOTONERA) {
		if (Estado_Atras != Botonera_Estado_Desactivado && Estado_Atras	!= Botonera_Estado_Presionado)			Estado_Atras	= Botonera_Estado_Resaltado;
	}
	else if (cX > (ANCHO_BOTON_BOTONERA - 1) && cX < (ANCHO_BOTON_BOTONERA * 2)) {
		if (Estado_Play != Botonera_Estado_Desactivado && Estado_Play != Botonera_Estado_Presionado)			Estado_Play		= Botonera_Estado_Resaltado;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 2) -1) && cX < (ANCHO_BOTON_BOTONERA * 3)) {
		if (Estado_Pausa != Botonera_Estado_Desactivado && Estado_Pausa	!= Botonera_Estado_Presionado)			Estado_Pausa	= Botonera_Estado_Resaltado;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 3) -1) && cX < (ANCHO_BOTON_BOTONERA * 4)) {
		if (Estado_Stop != Botonera_Estado_Desactivado && Estado_Stop != Botonera_Estado_Presionado)			Estado_Stop		= Botonera_Estado_Resaltado;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 4) -1) && cX < (ANCHO_BOTON_BOTONERA * 5)) {
		if (Estado_Adelante != Botonera_Estado_Desactivado && Estado_Adelante != Botonera_Estado_Presionado)	Estado_Adelante	= Botonera_Estado_Resaltado;
	}
	Repintar();
    return 0;
}


LRESULT Botonera::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	SetCapture(_hWnd);
	if (_Activado == false) return 0;
	if (cX > 0 && cX < ANCHO_BOTON_BOTONERA) {
		Estado_Atras = Botonera_Estado_Presionado;
	}
	else if (cX > (ANCHO_BOTON_BOTONERA - 1) && cX < (ANCHO_BOTON_BOTONERA * 2)) {
		Estado_Play	= Botonera_Estado_Presionado;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 2) -1) && cX < (ANCHO_BOTON_BOTONERA * 3)) {
		Estado_Pausa = Botonera_Estado_Presionado;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 3) -1) && cX < (ANCHO_BOTON_BOTONERA * 4)) {
		Estado_Stop	= Botonera_Estado_Presionado;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 4) -1) && cX < (ANCHO_BOTON_BOTONERA * 5)) {
		Estado_Adelante	= Botonera_Estado_Presionado;
	}
	Repintar();
    return 0;
}


LRESULT Botonera::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	ReleaseCapture();

	if (_Activado == false) return 0;
	if (cX > 0 && cX < ANCHO_BOTON_BOTONERA) {
		if (Estado_Atras == Botonera_Estado_Presionado)	{
			//DWL::ControlesEx::DWLBotonEx_ParametrosClick Params(cX, cY, ID_BOTON_CANCION_ATRAS, Boton, wParam);
			Params.Asignar(cX, cY, ID_BOTON_CANCION_ATRAS, Boton, Param);
			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO, reinterpret_cast<WPARAM>(&Params), NULL);
//			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO_IZQUIERDO + Boton, ID_BOTON_CANCION_ATRAS, MAKELPARAM(cX ,cY));
		}
		Estado_Atras = Botonera_Estado_Resaltado;
		if (Estado_Play		!= Botonera_Estado_Desactivado) Estado_Play		= Botonera_Estado_Normal;
		if (Estado_Pausa	!= Botonera_Estado_Desactivado) Estado_Pausa	= Botonera_Estado_Normal;
		if (Estado_Stop		!= Botonera_Estado_Desactivado) Estado_Stop		= Botonera_Estado_Normal;
		if (Estado_Adelante != Botonera_Estado_Desactivado) Estado_Adelante = Botonera_Estado_Normal;
	}
	else if (cX > (ANCHO_BOTON_BOTONERA - 1) && cX < (ANCHO_BOTON_BOTONERA * 2)) {
		if (Estado_Play == Botonera_Estado_Presionado) {
			Params.Asignar(cX, cY, ID_BOTON_CANCION_PLAY, Boton, Param);
			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO, reinterpret_cast<WPARAM>(&Params), NULL);
//			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO_IZQUIERDO + Boton, ID_BOTON_CANCION_PLAY, MAKELPARAM(cX ,cY));
		}
		if (Estado_Atras	!= Botonera_Estado_Desactivado) Estado_Atras	= Botonera_Estado_Normal;
		Estado_Play	= Botonera_Estado_Resaltado;
		if (Estado_Pausa	!= Botonera_Estado_Desactivado) Estado_Pausa	= Botonera_Estado_Normal;
		if (Estado_Stop		!= Botonera_Estado_Desactivado) Estado_Stop		= Botonera_Estado_Normal;
		if (Estado_Adelante	!= Botonera_Estado_Desactivado) Estado_Adelante = Botonera_Estado_Normal;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 2) -1) && cX < (ANCHO_BOTON_BOTONERA * 3)) {
		if (Estado_Pausa == Botonera_Estado_Presionado)	{
			Params.Asignar(cX, cY, ID_BOTON_CANCION_PAUSA, Boton, Param);
			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO, reinterpret_cast<WPARAM>(&Params), NULL);
//			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO_IZQUIERDO + Boton, ID_BOTON_CANCION_PAUSA, MAKELPARAM(cX ,cY));
		}
		if (Estado_Atras	!= Botonera_Estado_Desactivado) Estado_Atras	= Botonera_Estado_Normal;
		if (Estado_Play		!= Botonera_Estado_Desactivado) Estado_Play		= Botonera_Estado_Normal;
		Estado_Pausa	= Botonera_Estado_Resaltado;
		if (Estado_Stop		!= Botonera_Estado_Desactivado) Estado_Stop		= Botonera_Estado_Normal;
		if (Estado_Adelante	!= Botonera_Estado_Desactivado) Estado_Adelante = Botonera_Estado_Normal;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 3) -1) && cX < (ANCHO_BOTON_BOTONERA * 4)) {
		if (Estado_Stop == Botonera_Estado_Presionado)	{
			Params.Asignar(cX, cY, ID_BOTON_CANCION_STOP, Boton, Param);
			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO, reinterpret_cast<WPARAM>(&Params), NULL);
//			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO_IZQUIERDO + Boton, ID_BOTON_CANCION_STOP, MAKELPARAM(cX ,cY));
		}
		if (Estado_Atras	!= Botonera_Estado_Desactivado) Estado_Atras	= Botonera_Estado_Normal;
		if (Estado_Play		!= Botonera_Estado_Desactivado) Estado_Play		= Botonera_Estado_Normal;
		if (Estado_Pausa	!= Botonera_Estado_Desactivado) Estado_Pausa	= Botonera_Estado_Normal;
		Estado_Stop		= Botonera_Estado_Resaltado;
		if (Estado_Adelante	!= Botonera_Estado_Desactivado) Estado_Adelante = Botonera_Estado_Normal;
	}
	else if (cX > ((ANCHO_BOTON_BOTONERA * 4) -1) && cX < (ANCHO_BOTON_BOTONERA * 5)) {
		if (Estado_Adelante == Botonera_Estado_Presionado)	{
			Params.Asignar(cX, cY, ID_BOTON_CANCION_ADELANTE, Boton, Param);
			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO, reinterpret_cast<WPARAM>(&Params), NULL);
//			PostMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO_IZQUIERDO + Boton, ID_BOTON_CANCION_ADELANTE, MAKELPARAM(cX ,cY));
		}
		if (Estado_Atras	!= Botonera_Estado_Desactivado) Estado_Atras	= Botonera_Estado_Normal;
		if (Estado_Play		!= Botonera_Estado_Desactivado) Estado_Play		= Botonera_Estado_Normal;
		if (Estado_Pausa	!= Botonera_Estado_Desactivado) Estado_Pausa	= Botonera_Estado_Normal;
		if (Estado_Stop		!= Botonera_Estado_Desactivado) Estado_Stop		= Botonera_Estado_Normal;
		Estado_Adelante = Botonera_Estado_Resaltado;		
	}
//	Presionado = false;
//	Evento_Mouse_Movimiento(cX, cY, wParam);
    return 0;
}


void Botonera::Repintar(void) {
	DWL::GDI::DWLhDC hDC(_hWnd);
    PAINTSTRUCT PS;
	Evento_Pintar(hDC.hDC(), PS);
}


LRESULT Botonera::Evento_Mouse_Saliendo(void) {
	if (Estado_Atras	!= Botonera_Estado_Desactivado) Estado_Atras	= Botonera_Estado_Normal;
	if (Estado_Play		!= Botonera_Estado_Desactivado) Estado_Play		= Botonera_Estado_Normal;
	if (Estado_Pausa	!= Botonera_Estado_Desactivado) Estado_Pausa	= Botonera_Estado_Normal;
	if (Estado_Stop		!= Botonera_Estado_Desactivado) Estado_Stop		= Botonera_Estado_Normal;
	if (Estado_Adelante != Botonera_Estado_Desactivado) Estado_Adelante = Botonera_Estado_Normal;
//	DWL::ControlesEx::DWLBaseControlEx::Evento_Mouse_Saliendo();
	Repintar();
    return 0;
}




LRESULT Botonera::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	DWL::GDI::DWLhDC Buffer(hDC, ANCHO_BOTON_BOTONERA * 5, ALTO_BOTONERA);
//	_PintarGradient(Buffer.hDC(), 0, 0, 120, 24, RGB(192, 192, 192), RGB(128, 128, 128), true);
	switch (Estado_Atras) {
		case Botonera_Estado_Normal :
			DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_BOTONERA, ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(4, 3, 16, 16, IDI_ATRAS_N);
			else					Buffer.PintarIcono(4, 3, 16, 16, IDI_ATRAS_P);
			break;
		case Botonera_Estado_Resaltado :
			DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_BOTONERA, ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(4, 3, 16, 16, IDI_ATRAS_R);
			else					Buffer.PintarIcono(4, 3, 16, 16, IDI_ATRAS_P);
			break;
		case Botonera_Estado_Presionado :
		case Botonera_Estado_Desactivado :
			DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_BOTONERA, ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
			Buffer.PintarIcono(5, 4, 16, 16, IDI_ATRAS_P);
			break;
	}

	switch (Estado_Play) {
		case Botonera_Estado_Normal :
			DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_BOTONERA, 0, (ANCHO_BOTON_BOTONERA * 2), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(2 + ANCHO_BOTON_BOTONERA, 3, 16, 16, IDI_PLAY_N);
			else					Buffer.PintarIcono(2 + ANCHO_BOTON_BOTONERA, 3, 16, 16, IDI_PLAY_P);
			break;
		case Botonera_Estado_Resaltado :
			DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_BOTONERA, 0, (ANCHO_BOTON_BOTONERA * 2), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(2 + ANCHO_BOTON_BOTONERA, 3, 16, 16, IDI_PLAY_R);
			else					Buffer.PintarIcono(2 + ANCHO_BOTON_BOTONERA, 3, 16, 16, IDI_PLAY_P);
			break;
		case Botonera_Estado_Presionado :
		case Botonera_Estado_Desactivado :
			DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_BOTONERA, 0, (ANCHO_BOTON_BOTONERA * 2), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
			Buffer.PintarIcono(3 + ANCHO_BOTON_BOTONERA, 4, 16, 16, IDI_PLAY_P);
			break;
	}

	switch (Estado_Pausa) {
		case Botonera_Estado_Normal :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 2), 0, (ANCHO_BOTON_BOTONERA * 3), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 2), 3, 16, 16, IDI_PAUSA_N);
			else					Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 2), 3, 16, 16, IDI_PAUSA_P);
			break;
		case Botonera_Estado_Resaltado :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 2), 0, (ANCHO_BOTON_BOTONERA * 3), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 2), 3, 16, 16, IDI_PAUSA_R);
			else					Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 2), 3, 16, 16, IDI_PAUSA_P);
			break;
		case Botonera_Estado_Presionado :
		case Botonera_Estado_Desactivado :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 2), 0, (ANCHO_BOTON_BOTONERA * 3), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
			Buffer.PintarIcono(5 + (ANCHO_BOTON_BOTONERA * 2), 4, 16, 16, IDI_PAUSA_P);
			break;
	}

	switch (Estado_Stop) {
		case Botonera_Estado_Normal :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 3), 0, (ANCHO_BOTON_BOTONERA * 4), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 3), 3, 16, 16, IDI_STOP_N);
			else					Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 3), 3, 16, 16, IDI_STOP_P);
			break;
		case Botonera_Estado_Resaltado :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 3), 0, (ANCHO_BOTON_BOTONERA * 4), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 3), 3, 16, 16, IDI_STOP_R);
			else					Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 3), 3, 16, 16, IDI_STOP_P);
			break;
		case Botonera_Estado_Presionado :
		case Botonera_Estado_Desactivado :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 3), 0, (ANCHO_BOTON_BOTONERA * 4), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
			Buffer.PintarIcono(5 + (ANCHO_BOTON_BOTONERA * 3), 4, 16, 16, IDI_STOP_P);
			break;
	}

	switch (Estado_Adelante) {
		case Botonera_Estado_Normal :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 4), 0, (ANCHO_BOTON_BOTONERA * 5), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(3 + (ANCHO_BOTON_BOTONERA * 4), 3, 16, 16, IDI_ADELANTE_N);
			else					Buffer.PintarIcono(3 + (ANCHO_BOTON_BOTONERA * 4), 3, 16, 16, IDI_ADELANTE_P);
			break;
		case Botonera_Estado_Resaltado :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 4), 0, (ANCHO_BOTON_BOTONERA * 5), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			if (_Activado == true)	Buffer.PintarIcono(3 + (ANCHO_BOTON_BOTONERA * 4), 3, 16, 16, IDI_ADELANTE_R);
			break;
		case Botonera_Estado_Presionado :
		case Botonera_Estado_Desactivado :
			DWL::GDI::PintarGradient(Buffer.hDC(), (ANCHO_BOTON_BOTONERA * 4), 0, (ANCHO_BOTON_BOTONERA * 5), ALTO_BOTONERA, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
			Buffer.PintarIcono(4 + (ANCHO_BOTON_BOTONERA * 4), 4, 16, 16, IDI_ADELANTE_P);
			break;
	}

	DWL::GDI::DWLPluma Pluma(RGB(100, 100, 100), PS_SOLID, 1);
	for (int i = ANCHO_BOTON_BOTONERA; i != (ANCHO_BOTON_BOTONERA * 5); i+= ANCHO_BOTON_BOTONERA) Pluma.PintarLinea(&Buffer, i - 1, 0, i - 1, ANCHO_BOTON_BOTONERA);
//	COLORREF A = DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal;


	DWL::GDI::DWLBrocha Borde;
	if (Foco() == false) {
		if (Estado_Atras != Botonera_Estado_Presionado && Estado_Play != Botonera_Estado_Presionado && Estado_Pausa != Botonera_Estado_Presionado && Estado_Stop != Botonera_Estado_Presionado && Estado_Adelante != Botonera_Estado_Presionado) {
			Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
		}
		else {
			if (_Activado == true)	Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_FocoRaton);
			else					Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
		}
	}
	else  {
		if (_Activado == true)	Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_FocoRaton);
		else					Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
	}


	DWL::GDI::DWLBrocha BordeLight(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro);
	Region.EnmarcarRegion(&Buffer, &BordeLight, 2, 2);

	Region.EnmarcarRegion(&Buffer, &Borde, 1, 1);


	Buffer.PintarMapaBits(hDC, 0, 0, ANCHO_BOTON_BOTONERA * 5, ALTO_BOTONERA, 0, 0);
    return 0;
}


void Botonera::Boton_Atras_Desactivado(const bool Desactivado, const bool nRepintar) {
	if (Desactivado == true)	Estado_Atras = Botonera_Estado_Desactivado;
	else						Estado_Atras = Botonera_Estado_Normal;
	if (nRepintar == true)		Repintar();
}


void Botonera::Boton_Play_Desactivado(const bool Desactivado, const bool nRepintar) {
	if (Desactivado == true)	Estado_Play = Botonera_Estado_Desactivado;
	else						Estado_Play = Botonera_Estado_Normal;
	if (nRepintar == true)		Repintar();
}


void Botonera::Boton_Pausa_Desactivado(const bool Desactivado, const bool nRepintar) {
	if (Desactivado == true)	Estado_Pausa = Botonera_Estado_Desactivado;
	else						Estado_Pausa = Botonera_Estado_Normal;
	if (nRepintar == true)		Repintar();
}


void Botonera::Boton_Stop_Desactivado(const bool Desactivado, const bool nRepintar) {
	if (Desactivado == true)	Estado_Stop = Botonera_Estado_Desactivado;
	else						Estado_Stop = Botonera_Estado_Normal;
	if (nRepintar == true)		Repintar();
}


void Botonera::Boton_Adelante_Desactivado(const bool Desactivado, const bool nRepintar) {
	if (Desactivado == true)	Estado_Adelante = Botonera_Estado_Desactivado;
	else						Estado_Adelante = Botonera_Estado_Normal;
	if (nRepintar == true)		Repintar();
}
