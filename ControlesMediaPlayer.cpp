#include "StdAfx.h"
#include "ControlesMediaPlayer.h"
#include "ConfigBubaTronik.h"
#include "DimensionesFijas.h"
#include "BubaTronikApp.h"
#include <DWLMouse.h>

//DWL_APP(BubaTronikApp);


ControlesMediaPlayer::ControlesMediaPlayer(void) {
}

ControlesMediaPlayer::~ControlesMediaPlayer(void) {
}



/*LRESULT CALLBACK ControlesMediaPlayer::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		ENLACE_Evento_PintarHDC();
		ENLACE_Evento_Mouse_Movimiento();
		ENLACE_Evento_Cerrar();
		ENLACE_BarraEx_Evento_Mouse_BotonSoltado();
		ENLACE_BotonEx_Evento_Mouse_BotonSoltado();
	}
	return DWL::Ventanas::DWLBaseVentana::GestorMensajes(uMsg, wParam, lParam);
}*/

void ControlesMediaPlayer::Crear(const bool Abajo) {
	_hWnd = CrearVentana(Sistema.App.PlayerEx.hWnd(), TEXT("Controles_MediaPlayer"), WS_POPUP, NULL, 0, 0, CONTROLESMP_ANCHO, CONTROLESMP_ALTO, NULL, WS_EX_TOPMOST | WS_EX_TOOLWINDOW, NULL, NULL);

	Botones.CrearBotonera(_hWnd, X_BOTONERA, 8);
	Botones.Boton_Stop_Desactivado(true, false);
	BotonShufleRepeat.CrearShufleRepeat(_hWnd, X_BOTON_SHUFLE_REPEAT, 8);
	TiempoStrActual.CrearCTA(_hWnd, X_OSCY, 8);
	Volumen.Crear(_hWnd, X_OSCY + 10, 22, ANCHO_VOLUMEN, ALTO_VOLUMEN, ID_CONTROL_VOLUMEN);
	SliderTiempo.CrearBarraEx(WS_CHILD | WS_VISIBLE, _hWnd, 10, 34, CONTROLESMP_ANCHO - 20, 12, ID_SLIDER_TIEMPO, DWL_BARRAEX_DESPLAZAMIENTO_HORIZONTAL, 0, 5000);
	SliderTiempo.Valor(0);
	SliderTiempo.Maximo(10000);

	DWL::GDI::DWLRegion Region;
	if (Abajo == true)  Region.CrearRegionRectaRedondeada(0, 0, CONTROLESMP_ANCHO, CONTROLESMP_ALTO + 10, 11, 11);
	else				Region.CrearRegionRectaRedondeada(0, -10, CONTROLESMP_ANCHO, CONTROLESMP_ALTO, 11, 11);
	Region.AsignarRegionVentana(_hWnd, FALSE);

//	Opacidad(255);
}

void ControlesMediaPlayer::Mostrar(HWND Controlador, const bool Abajo) {
	if (Controlador == NULL) return;
	if (_hWnd == NULL) Crear(Abajo); // Por si al usuario se le ocurre cerrar la ventana con alt+f4
	// Miro si el VLC esta en pantalla completa observando su tamaño y comparandolo con el tamaño del escritorio
	RECT RC;
	GetClientRect(Controlador, &RC);
	RECT Recta;
	GetWindowRect(Controlador, &Recta);
	int AnchoS = (RC.right - CONTROLESMP_ANCHO) / 2;
	if (Abajo == true)	Mover(Recta.left + AnchoS, Recta.bottom - CONTROLESMP_ALTO, CONTROLESMP_ANCHO, CONTROLESMP_ALTO);
	else				Mover(Recta.left + AnchoS, 0, CONTROLESMP_ANCHO, CONTROLESMP_ALTO);
//	SetParent(NULL, Controlador);
//	ShowWindow(_hWnd, SW_SHOWNOACTIVATE);
	HWND BarraTareas = FindWindowEx(NULL, NULL, TEXT("Shell_TrayWnd"), NULL);
	RECT RBT;
	GetWindowRect(BarraTareas, &RBT);
	if (RBT.left == Recta.left && RBT.right == Recta.right)	ShowWindowAsync(BarraTareas, SW_HIDE);
//	SetWindowPos(BarraTareas, HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE);
	SetWindowPos(_hWnd, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
	Visible(true);
	//	SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	//	SetFocus(Controlador);
}

void ControlesMediaPlayer::Ocultar(HWND Controlador) {
	if (Visible() == TRUE) {
		Visible(false);
		if (Controlador != NULL) SetWindowPos(Controlador, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		HWND BarraTareas = FindWindowEx(NULL, NULL, TEXT("Shell_TrayWnd"), NULL);
		ShowWindowAsync(BarraTareas, SW_SHOWDEFAULT);
		// si utilizo setfocus el stop gripa
		SetFocus(Controlador);
	}
}


LRESULT ControlesMediaPlayer::Evento_Cerrar(void) {
	HWND BarraTareas = FindWindowEx(NULL, NULL, TEXT("Shell_TrayWnd"), NULL);
	ShowWindowAsync(BarraTareas, SW_SHOWDEFAULT);
	PostMessage(Sistema.App.PlayerEx.hWnd(), WM_CLOSE, 0, 0);
	return 0;
}


LRESULT ControlesMediaPlayer::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	DWL::GDI::DWLhDC		DC(hDC, CONTROLESMP_ANCHO, CONTROLESMP_ALTO);
	DWL::GDI::DWLRegion		Region(_hWnd);
	DWL::GDI::DWLBrocha		Brocha(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro);
	RECT					Recta = { 0, 0, CONTROLESMP_ANCHO, CONTROLESMP_ALTO };
	DC.PintaGradient(DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, &Recta, false);

	DC.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDE, VENTANAEX_BORDE);
	Brocha.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
	DC.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDECLARO, VENTANAEX_BORDECLARO);

	DC.PintarMapaBits(hDC, 0, 0, CONTROLESMP_ANCHO, CONTROLESMP_ALTO, 0, 0);

	//RECT R;
	//GetClientRect(_hWnd, &R);
//	FillRect(hDC, &R, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
    return 0;
}


LRESULT ControlesMediaPlayer::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	DWL::SO::DWLMouse::Visible(TRUE);
    return 0;
}

LRESULT ControlesMediaPlayer::Evento_BarraEx_Mouse_BotonSoltado(const UINT cID, const UINT nBoton) {
	switch (cID) {
		case ID_SLIDER_TIEMPO :
			Sistema.App.Media.TiempoActual(SliderTiempo.Valor());
			break;
	}
    return 0;
}


LRESULT ControlesMediaPlayer::Evento_BotonEx_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
	if (Boton != 0) return 0;
	switch (IDBotonEx) {
		case ID_BOTON_CANCION_ATRAS		:		
			Ocultar(Sistema.App.MediaPlayer.hWnd());
			Sistema.App.Media.Anterior();						
			break;
		case ID_BOTON_CANCION_PLAY		:		Sistema.App.Media.Play();							break;
		case ID_BOTON_CANCION_PAUSA		:		Sistema.App.Media.Pausa();							break;
		case ID_BOTON_CANCION_STOP		:		
			Ocultar(Sistema.App.MediaPlayer.hWnd());
			Sistema.App.Media.Stop();							
			break;
		case ID_BOTON_CANCION_ADELANTE	:		
			Ocultar(Sistema.App.MediaPlayer.hWnd());
			Sistema.App.Media.Siguiente();						
			break;
		case ID_BOTON_SHUFLE			:		
			Sistema.App.ListaC.Shufle(!Sistema.App.Config.Shufle);		
			BotonShufleRepeat.ShufleMarcado(Sistema.App.Config.Shufle);
			break;	
		case ID_BOTON_REPEAT			:		Sistema.App.PlayerEx.MenuRepeat->Mostrar(Sistema.App.PlayerEx.hWnd());	break;
	}
    return 0;
}
