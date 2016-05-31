#include "StdAfx.h"
#include "ControlMediaPlayer.h"
#include "DimensionesFijas.h"
#include "Definiciones.h"
#include <DWLListaIconos.h>
#include "Resource.h"
#include "BubaTronikApp.h"
#include <DWLMouse.h>

//DWL_APP(BubaTronikApp);

/*
WNDPROC				ControlMediaPlayer::ProcVLC			= NULL;
HWND				ControlMediaPlayer::hWndVLC			= NULL;
WNDPROC				ControlMediaPlayer::ProcVLC_Video	= NULL;
HWND				ControlMediaPlayer::hWndVLC_Video	= NULL;
ControlMediaPlayer *ControlMediaPlayer::This			= NULL;
*/

bool				ControlMediaPlayer::WndEncontrada	= false;

ControlMediaPlayer::ControlMediaPlayer(void) {
	_PantallaCompleta = false;
//	This = this;
	Tecla_PlayPausaPresionada	= false;
	Tecla_SiguientePresionada	= false;
	Tecla_AnteriorPresionada	= false;
	Tecla_StopPresionada		= false;
//	VLCProc = NULL;
//	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8,(void**)&ObjetoDirectInput, NULL);
//	ObjetoDirectInput->CreateDevice(GUID_SysKeyboard, &dinkeyboard, NULL);
	DobleClick = 0;
	PlayerMaximizado = false;
}

ControlMediaPlayer::~ControlMediaPlayer(void) {
}

LRESULT ControlMediaPlayer::Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto) {
	HDC hDC = GetDC(_hWnd);
    PAINTSTRUCT PS;
	Evento_Pintar(hDC, PS);
	ReleaseDC(_hWnd, hDC);
    return 0;
}

void ControlMediaPlayer::Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const UINT cID) {
	CrearControl(hWndParent, TEXT("ControlMediaPlayer"), WS_CHILD, TEXT("ControlMediaPlayer"), cX, cY, cAncho, cAlto, cID);

	// Miro donde esta posicionada la barra de inicio para saber donde no tengo que mostrar esta ventana
/*	HWND BarraTareas = FindWindowEx(NULL, NULL, TEXT("Shell_TrayWnd"), NULL);
	RECT RBT;
	GetClientRect(BarraTareas, &RBT);
	if (RBT.right > RBT.bottom) {	// Horizontal
		GetWindowRect(BarraTareas, &RBT);
		if (RBT.top == 0) { // Barra de inicio arriba
			ControlesMP_Abajo = true;
		}
		else {	// Barra de inicio abajo
			ControlesMP_Abajo = false;
		}
	}
	else {							// Vertical
	}
*/
	ControlesMP_Abajo = true;
//	SetWindowPos(BarraTareas, HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE);
	Sistema.App.ControlesMP.Crear(ControlesMP_Abajo);
	SetTimer(_hWnd, ID_TECLADO_GLOBAL, 25, NULL);
	SetTimer(_hWnd, ID_ESCONDER_MOUSE, 500, NULL);
}

/*

// SOLO PARA TECLAS NO REPETITIVAS
LRESULT ControlMediaPlayer::Evento_Teclado_TeclaSoltada(const UINT Tecla, const UINT Repeticion, const UINT Param) {
	switch (Tecla) {	
//			SOLO_DEBUG(DWL::DWLDebug::ImprimirDebug(TEXT("Tecla LEFT\n")));
//			break;
		case VK_SPACE :
//			if (Sistema.App.Media.ComprobarEstado() == EnPausa) Sistema.App.Media.Play();
//			else										Sistema.App.Media.Pausa();
			break;
	}
    return 0;
}
*/

LRESULT ControlMediaPlayer::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	RECT R;
	GetClientRect(_hWnd, &R);
	FillRect(hDC, &R, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
	DWL::SO::DWLListaIconos::PintarIcono(hDC, (R.right -128) / 2, (R.bottom -128) / 2, 128, 128, IDI_VIDEO);
    return 0;
}



// Hack para conseguir acceso a los mensajes de las ventanas del reproductor VLC
// NO USAR SetFocus en la ventana del VLC o gripara el stop y luego se perdera la señal
void ControlMediaPlayer::HackVLCWNDPROC(const bool Activar) {
	if (Activar == true) {
//		Sleep(1000);
		CrearTemporizador(ID_HACKVLC, 100);
//		EnumChildWindows(_hWnd, &ControlMediaPlayer::EnumChildProc, NULL); 
	}
	else {
/*		if (hWndVLC != NULL && ProcVLC != NULL) {
			SetWindowLongPtr(hWndVLC, GWLP_WNDPROC, PtrToLong(ProcVLC));
			SetWindowLongPtr(hWndVLC_Video, GWLP_WNDPROC, PtrToLong(ProcVLC_Video));
		}
		ProcVLC = NULL;
		hWndVLC = NULL;
		ProcVLC_Video = NULL;
		hWndVLC_Video = NULL;*/
		SetFocus(_hWnd);
	}
}


LRESULT ControlMediaPlayer::Evento_Temporizador(const UINT nID) {
	switch (nID){
		case ID_HACKVLC :
			WndEncontrada = false;
			EnumChildWindows(_hWnd, &ControlMediaPlayer::EnumeracionVLC, NULL);
			if (WndEncontrada == true) KillTimer(_hWnd, ID_HACKVLC);

			break;
		case ID_ESCONDER_MOUSE :
			int MRet;
			if (_PantallaCompleta == true && Sistema.App.Config.OcultarMouseEnVideo > 0 && Sistema.App.ControlesMP.Visible() == FALSE) {
				HWND FocoActual;
				FocoActual = GetFocus();
				if (FocoActual == _hWnd || FocoActual == Sistema.App.PlayerEx.hWnd() || FocoActual == Sistema.App.ControlesMP.hWnd()) {
					if ((UltimoMovimiento + Sistema.App.Config.OcultarMouseEnVideo) < GetTickCount() && Sistema.App.Media.ComprobarEstado() == EnPlay) {
						MRet = DWL::SO::DWLMouse::Visible(FALSE);
					}
				}
			}
			break;

		case ID_TECLADO_GLOBAL :
			Teclado_Global();
			break;
	}
    return 0;
}

BOOL CALLBACK ControlMediaPlayer::EnumeracionVLC(HWND hWndWnd, LPARAM lParam) {
//	if (IsWindow(hWndWnd) == TRUE) 
//		hWndVLC_Video = hWnd;
	EnableWindow(hWndWnd, FALSE); // Desactivo los clicks en las ventanas del VLC para evitar el pantalla completa
	
	if (hWndWnd != NULL) WndEncontrada = true;
	return TRUE;
}


//#define TECLA_PRESIONADA(TeclaVirtual) (Teclado[TeclaVirtual] & 0x80)

// Para obtener eventos de teclado la unica solucion es hacer un GetKeyState global mirando que este activa la ventana del reproductor
// Al salir de pantalla completa GetActiveWindow retorna NULL, y eso hace que no se reciban eventos de teclado. 
// Asignar el foco otravez sera una mala opcion seguro.
void ControlMediaPlayer::Teclado_Global(void) {
	BYTE Teclado[256];
	bool Foco = true;
	if (Sistema.App.PlayerEx.EditandoNombre == true) return;
	if (GetKeyboardState(Teclado) == 0) return;
//	int ControlPresionado	= TECLA_PRESIONADA(VK_CONTROL);
//	int AltPresionado		= TECLA_PRESIONADA(VK_MENU);
	HWND Ventana = GetActiveWindow();

	if (Foco == true) {
//		if (Sistema.App.Media.MedioActual == NULL) return;
//		if (Sistema.App.Config.Tecla_PlayPausa.TeclaSoltada(Teclado) == true) {
//			if (Sistema.App.Media.ComprobarEstado() == EnPausa) Sistema.App.Media.Play();
//			else										Sistema.App.Media.Pausa();
//		}
		// Play Pausa
		if (Sistema.App.Config.Tecla_PlayPausa.TeclaPresionada(Teclado) == true) {
			Tecla_PlayPausaPresionada = true;
		}
		else {
			if (Tecla_PlayPausaPresionada == true) {
				Tecla_PlayPausaPresionada = false;
				if (Sistema.App.Media.ComprobarEstado() == EnPlay)	Sistema.App.Media.Pausa();
				else										Sistema.App.Media.Play();
			}
		}
		// Stop
		if (Sistema.App.Config.Tecla_Stop.TeclaPresionada(Teclado) == true) {
			Tecla_StopPresionada = true;
		}
		else {
			if (Tecla_StopPresionada == true) {
				Tecla_StopPresionada = false;
				Sistema.App.Media.Stop();
//				if (Sistema.App.Media.ComprobarEstado() == EnPausa) Sistema.App.Media.Play();
//				else										Sistema.App.Media.Pausa();
			}
		}
		// Volumen Mas
		if (Sistema.App.Config.Tecla_VolumenMas.TeclaPresionada(Teclado) == true) {
			Sistema.App.Media.Volumen(Sistema.App.Media.Volumen() + 5);
		}       
		// Volumen menos
		if (Sistema.App.Config.Tecla_VolumenMenos.TeclaPresionada(Teclado) == true) {
			Sistema.App.Media.Volumen(Sistema.App.Media.Volumen() - 5);
		}                                         
		/*if (Sistema.App.Config.Tecla_Atras.TeclaPresionada(Teclado) == true) {
			Sistema.App.Media.RateAtrasX4();
		}
		// Rate adelante
		if (Sistema.App.Config.Tecla_Adelante.TeclaPresionada(Teclado) == true) {
			Sistema.App.Media.RateAdelanteX4();
		}*/

		else { // con una vez basta para el LEFT y el RIGHT
			Sistema.App.Media.RateNormal();
		}
		// Siguiente Medio
		if (Sistema.App.Config.Tecla_Siguiente.TeclaPresionada(Teclado) == true) {
			Tecla_SiguientePresionada = true;
		}
		else {
			if (Tecla_SiguientePresionada == true) {
				Tecla_SiguientePresionada = false;
				Sistema.App.Media.Siguiente();
			}
		}
		// Anterior Medio
		if (Sistema.App.Config.Tecla_Anterior.TeclaPresionada(Teclado) == true) {
			Tecla_AnteriorPresionada = true;
		}
		else {
			if (Tecla_AnteriorPresionada == true) {
				Tecla_AnteriorPresionada = false;
				Sistema.App.Media.Anterior();
			}
		}
	}
}


LRESULT ControlMediaPlayer::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT wParam) {
	UltimoMovimiento = GetTickCount();
	if (EstaEnPantallaCompleta() == true) {
		DWL::SO::DWLMouse::Visible(TRUE);
		RECT  Recta;
		POINT MousePos;
		DWL::SO::DWLMouse::ObtenerPosicion(&MousePos);
		HRGN RMP = CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(Sistema.App.ControlesMP.hWnd(), RMP);
		
		GetWindowRect(_hWnd, &Recta);
		int x;
		if (Recta.left > 0) x = ((Recta.right - Recta.left) - CONTROLESMP_ANCHO) / 2;
		else				x = (((Recta.right + Recta.left) + CONTROLESMP_ANCHO) / 2) - CONTROLESMP_ANCHO;
		if (ControlesMP_Abajo == true) {
			int y = (Recta.bottom - Recta.top) - CONTROLESMP_ALTO;
			int r = OffsetRgn(RMP, x, y);
			if (PtInRegion(RMP, MousePos.x, MousePos.y) == TRUE) {
				Sistema.App.ControlesMP.Mostrar(_hWnd, ControlesMP_Abajo);
			}
			else {
				Sistema.App.ControlesMP.Ocultar(hWnd());
			}
		}
		else {
			OffsetRgn(RMP, x, 0);
			if (PtInRegion(RMP, MousePos.x, MousePos.y) == TRUE) {
				Sistema.App.ControlesMP.Mostrar(_hWnd, ControlesMP_Abajo);
			}
			else {
				Sistema.App.ControlesMP.Ocultar(hWnd());
			}
		}
		DeleteObject(RMP);
	}
	return 0;
}

LRESULT ControlMediaPlayer::Evento_Mouse_DobleClick(const UINT Boton, const int cX, const int cY, const UINT Param) {
	PantallaCompleta(!_PantallaCompleta);
    return 0;
}

LRESULT ControlMediaPlayer::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	// Oculto el menu, porque bajo la ventana del vlc me ignora todos los eventos de mouse externos
	Sistema.App.PlayerEx.MenuVideo->OcultarTodo();
	Sistema.App.PlayerEx.MenuRepeat->OcultarTodo();
//	Sistema.App.ControlesMP.Ocultar(hWnd);
	if (Boton == 0) {
		if (GetTickCount() < DobleClick + 1000) {
			PantallaCompleta(!_PantallaCompleta);
			DobleClick = 0;
		}
		else {
			DobleClick = GetTickCount();
		}
	}
	if (Boton == 1)	{
		// mostrar menu video
		Sistema.App.PlayerEx.MenuVideo->Mostrar(Sistema.App.PlayerEx.hWnd());									
	}
    return 0;
}

/*const bool ControlMediaPlayer::EstaEnPantallaCompleta(void) {
	RECT 		RectaVLC;
	HMONITOR	Monitor;
	MONITORINFO MI;
	GetWindowRect(_hWnd, &RectaVLC);
	Monitor = MonitorFromRect(&RectaVLC, MONITOR_DEFAULTTONEAREST);
	MI.cbSize = sizeof(MI);
	GetMonitorInfo(Monitor, &MI);
	if (MI.rcMonitor.bottom == RectaVLC.bottom && MI.rcMonitor.top == RectaVLC.top && MI.rcMonitor.left == RectaVLC.left && MI.rcMonitor.right == RectaVLC.right) {
		return true;
	}
	return false;
}*/

void ControlMediaPlayer::PantallaCompleta(const bool VerCompleta) {
	if (VerCompleta == true) { 
		WINDOWPLACEMENT WP;
		WP.length = sizeof(WINDOWPLACEMENT); 
		GetWindowPlacement(_hWnd, &WP); 
		Sistema.App.Config.X = WP.rcNormalPosition.left;
		Sistema.App.Config.Y = WP.rcNormalPosition.top;
		Sistema.App.Config.Ancho = WP.rcNormalPosition.right - WP.rcNormalPosition.left;
		Sistema.App.Config.Alto = WP.rcNormalPosition.bottom - WP.rcNormalPosition.top;

		PlayerMaximizado = Sistema.App.PlayerEx.Maximizado();
		Sistema.App.PlayerEx.OpcionesBasicas.Visible(false);
		Sistema.App.PlayerEx.Volumen.Visible(false);
		Sistema.App.PlayerEx.TiempoStrActual.Visible(false);
		Sistema.App.PlayerEx.CM.Visible(false);
		Sistema.App.PlayerEx.Oscy.Visible(false);
		Sistema.App.PlayerEx.BotonShufleRepeat.Visible(false);
		Sistema.App.PlayerEx.Botones.Visible(false);
		Sistema.App.SliderTiempo.Visible(false);

		if (Sistema.App.Config.UtilizarBarraTituloWindows == true) {
			SetWindowLongPtr(Sistema.App.PlayerEx.hWnd(), GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
			GetWindowPlacement(_hWnd, &WP); 
		}
		if (Sistema.App.PlayerEx.Maximizado() == false) {
			Sistema.App.PlayerEx.Maximizar(true);
		}

		RECT R;
		GetClientRect(Sistema.App.PlayerEx.hWnd(), &R);
		SetWindowPos(Sistema.App.PlayerEx.hWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		
		MoveWindow(_hWnd, R.left, R.top, R.right, R.bottom, TRUE);
	}
	else {
//		if (PlayerMaximizado == false) {
		Sistema.App.PlayerEx.Maximizar(PlayerMaximizado);
//		}

		if (Sistema.App.Config.UtilizarBarraTituloWindows == true) {
			SetWindowLongPtr(Sistema.App.PlayerEx.hWnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		}
		Sistema.App.PlayerEx.OpcionesBasicas.Visible(true);
		Sistema.App.PlayerEx.Volumen.Visible(true);
		Sistema.App.PlayerEx.TiempoStrActual.Visible(true);
		if (Sistema.App.Config.UtilizarBarraTituloWindows == false) Sistema.App.PlayerEx.CM.Visible(true);
		Sistema.App.PlayerEx.Oscy.Visible(true);
		Sistema.App.PlayerEx.BotonShufleRepeat.Visible(true);
		Sistema.App.PlayerEx.Botones.Visible(true);
		Sistema.App.SliderTiempo.Visible(true);
		if (Sistema.App.Config.SiempreDelante == false) SetWindowPos(Sistema.App.PlayerEx.hWnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		Sistema.App.PlayerEx.Oscy.PintarFondoVacio();
//		SetWindowLong(_hWnd, GWL_EXSTYLE, WS_TOPMOST);

	}
	_PantallaCompleta = VerCompleta;
	if (_PantallaCompleta == false) DWL::SO::DWLMouse::Visible(TRUE);
}
