#include "stdafx.h"
#include "VentanaEx.h"
#include <DWLMouse.h>
#include "resource.h"
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);

#define COL_GRADIENT1	RGB(200,200,200)
#define COL_GRADIENT2	RGB(100,100,100)
#define COL_BORDE		RGB(70,70,70)

/*LRESULT CALLBACK VentanaEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_ERASEBKGND  :	return 1;																																				break;
		ENLACE_Evento_PintarHDC();
//		ENLACE_Evento_BorrarFondoHDC();
		ENLACE_Evento_Mouse_Movimiento();
		ENLACE_Evento_Mouse_BotonPresionado();
		ENLACE_Evento_Mouse_BotonSoltado();
//		ENLACE_Evento_CambioTam();
		ENLACE_Evento_Temporizador();
	}
	return DWL::Ventanas::DWLVentana::GestorMensajes(uMsg, wParam, lParam);
};*/


VentanaEx::VentanaEx(void) {
	Redimensionando = false;
	_Maximizado = false;
	_AltoParteSuperior = VENTANAEX_ALTURA_PARTE_SUPERIOR;
//	DescargandoActualizacion = false;
}

VentanaEx::~VentanaEx(void) {
}



HWND VentanaEx::CrearVentanaEx(const TCHAR *nNombre, const int cX, const int cY, const int cAncho, const int cAlto, const int nIcono, const TCHAR *Titulo, const bool nMinimizable, const bool nMaximizable, const UINT AlturaSuperior) {
	_AltoParteSuperior = AlturaSuperior;
	if (Sistema.App.Config.UtilizarBarraTituloWindows == false) { // Windos XP o inferior
		DWL::GDI::DWLRegion Region(0, 0, cAncho, cAlto - 2, 11, 11);
		DWL::GDI::DWLRegion Region2(4, 4, cAncho - 4, cAlto - 6, 11, 11);
		// Aqui tenemos el borde para detectar si el mouse esta en el.
		Region.CombinarRegion(&Region2, &Borde, RGN_XOR);
		CrearVentana(NULL, nNombre, WS_POPUP | WS_SYSMENU, Titulo, cX, cY, cAncho, cAlto, NULL, NULL, NULL, nIcono);
		Region.AsignarRegionVentana(_hWnd, FALSE);
	}
	else { // Windows Vista o superior
		CrearVentana(NULL, nNombre, WS_OVERLAPPEDWINDOW, Titulo, cX, cY, cAncho, cAlto, NULL, NULL, NULL, nIcono);
	}
	_Icono = nIcono;
	_Titulo = Titulo;
	if (nMinimizable == true) Minimizable(true); // Permite que la ventana pueda ser minimizada desde la barra de inicio
	if (nMaximizable == true) Maximizable(true);
	return _hWnd;
}


void VentanaEx::Maximizar(const bool Maxi) {
	RECT RC;
	if (Maxi == false) { // restaurar
		WINDOWPLACEMENT WP;
		WP.length = sizeof(WINDOWPLACEMENT); 
		GetWindowPlacement(_hWnd, &WP); 

		if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
			DWL::GDI::DWLRegion Region (0, 0, (WP.rcNormalPosition.right - WP.rcNormalPosition.left), (WP.rcNormalPosition.bottom - WP.rcNormalPosition.top) - 0, 11, 11);
			DWL::GDI::DWLRegion Region2(4, 4, (WP.rcNormalPosition.right - WP.rcNormalPosition.left) - 4, (WP.rcNormalPosition.bottom - WP.rcNormalPosition.top) - 4, 11, 11);
			Region.CombinarRegion(&Region2, &Borde, RGN_XOR);
			Region.AsignarRegionVentana(_hWnd, FALSE);
		}

		if (_Maximizado == true) {
			InvalidateRect(NULL, NULL, FALSE);
		}
		_Maximizado = Maxi;
		ShowWindow(_hWnd, SW_RESTORE);
		GetClientRect(_hWnd, &RC);
		Evento_CambioTam(NULL, RC.right, RC.bottom);
//		Evento_CambioTam(NULL, &WP.rcNormalPosition.right - &WP.rcNormalPosition.left, &WP.rcNormalPosition.bottom - &WP.rcNormalPosition.top); // AQUESTA NO FA EL REPINTAT CHUNGO
	}
	else { // maximizar
		SetWindowRgn(_hWnd, NULL, FALSE);
		InvalidateRect(_hWnd, NULL, TRUE);
		ShowWindow(_hWnd, SW_MAXIMIZE);
		/*if (App.Config.UtilizarBarraTituloWindows == true) {
			RECT RW;
			GetWindowRect(_hWnd, &RW);
			RC.left = RW.left + DWL::DWLSO::Medidas.Ancho_Borde();
			RC.top = RW.top + DWL::DWLSO::Medidas.Alto_Borde();
			RC.right = (RW.right - RW.left); // - (DWL::DWLSO::Medidas.Ancho_Borde() * 2); 
			RC.bottom = (RW.bottom - RW.top) - DWL::DWLSO::Medidas.Alto_Borde();
		}
		else {*/
			GetClientRect(_hWnd, &RC);
//		}
//		if (App.Config.UtilizarBarraTituloWindows == true) RC.bottom -= DWL::DWLSO::Medidas.Alto_BarraTitulo();
		_Maximizado = Maxi;
		Evento_CambioTam(NULL, RC.right, RC.bottom);
		
		//RedrawWindow(_hWnd, NULL, NULL, RDW_ERASE);
	}
	InvalidateRect(Sistema.App.PlayerEx.Botones.hWnd(), NULL, FALSE);
	InvalidateRect(Sistema.App.PlayerEx.Volumen.hWnd(), NULL, FALSE);
	InvalidateRect(Sistema.App.PlayerEx.BotonShufleRepeat.hWnd(), NULL, FALSE);
	InvalidateRect(Sistema.App.SliderTiempo.hWnd(), NULL, FALSE);
}


LRESULT VentanaEx::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	//BubaTronikApp &App = DWL::DWL_ObtenerApp<BubaTronikApp>();
	int An, Al;
	int VentanaExAltoParteSuperior = _AltoParteSuperior;
	int VentanaExBorde = 1;
		RECT R;
		GetClientRect(_hWnd, &R);
		An = R.right;
		Al = R.bottom;
	if (_Maximizado == false) {
/*		WINDOWPLACEMENT WP;
		WP.length = sizeof(WINDOWPLACEMENT); 
		GetWindowPlacement(_hWnd, &WP); 
		An = WP.rcNormalPosition.right - WP.rcNormalPosition.left;
		Al = WP.rcNormalPosition.bottom - WP.rcNormalPosition.top;*/
		if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { // Windows vista o superior
//			Al -= (DWL::DWLSO::Medidas.Alto_BarraTitulo() + DWL::DWLSO::Medidas.Alto_Borde());
//			An -= (DWL::DWLSO::Medidas.Ancho_Borde() * 2);
		}
	}
	else { // _Maximizado == true
		VentanaExBorde = 0;
	}

	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) {
		VentanaExAltoParteSuperior -= 22;
		VentanaExBorde = 0;
	}

	

	DWL::GDI::DWLhDC		DC(hDC, An, Al); // Creo DC Compatible
	DWL::GDI::DWLRegion		Region(_hWnd);
    DWL::GDI::DWLBrocha		Brocha(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro);
	DWL::GDI::DWLRecta		Recta(0, 0, An, VentanaExAltoParteSuperior -1);
	DWL::GDI::DWLIcono		Icono(IDI_MP3);

	// Pinto el fondo degradado en la parte superior
	DC.PintaGradient(DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior, &Recta, false);
	// Pinto el fondo degradado en la parte inferior
	Recta.Asignar(0, (Al - VENTANAEX_ALTURA_PARTE_INFERIOR) + 2, An, Al);
	DC.PintaGradient(DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior, &Recta, false);

	// Pinto el borde superior
	Recta.Asignar(VentanaExBorde, VentanaExAltoParteSuperior - 1, An - (VentanaExBorde * 2), VentanaExAltoParteSuperior);
	DC.PintarRecta(&Recta, &Brocha);
	// Pinto el borde inferior
	Recta.Asignar(VentanaExBorde, (Al - VENTANAEX_ALTURA_PARTE_INFERIOR) + 1, An - (VentanaExBorde * 2), (Al - VENTANAEX_ALTURA_PARTE_INFERIOR) + 2);
	DC.PintarRecta(&Recta, &Brocha);
	
	if (_Maximizado == false && Sistema.App.Config.UtilizarBarraTituloWindows == false) {
		DC.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDE, VENTANAEX_BORDE);
		Brocha.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
		DC.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDECLARO, VENTANAEX_BORDECLARO);
	}

	DC.ModoBk(TRANSPARENT);
	// Pinto el titulo y el icono
//	if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
	/*	DC.PintarIcono(5, 5, 16, 16, IDI_DAP);
		DC.Fuente(Sistema.App.Tahoma15b());
		DC.ColorTexto(RGB(0,0,0));
		TCHAR TituloVentanaEx[256];
		GetWindowText(_hWnd, TituloVentanaEx, 256);
		DC.PintarTexto(TituloVentanaEx, 26, 6);
		DC.ColorTexto(RGB(255,255,255));
		DC.PintarTexto(TituloVentanaEx, 25, 5);*/
	PintarTitulo(false, &DC);
//	}

	Evento_PintarEx(DC());

	DC.PintarMapaBits(hDC, 0, 0, An, VentanaExAltoParteSuperior, 0, 0);
	DC.PintarMapaBits(hDC, 0, (Al - VENTANAEX_ALTURA_PARTE_INFERIOR) + 1, An, VENTANAEX_ALTURA_PARTE_INFERIOR, 0, (Al - VENTANAEX_ALTURA_PARTE_INFERIOR) + 1);
	if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
		DC.PintarMapaBits(hDC, 0, VentanaExAltoParteSuperior, 4, Al - VENTANAEX_ALTURA_PARTE_INFERIOR, 0, VentanaExAltoParteSuperior);
		DC.PintarMapaBits(hDC, An - 4, VentanaExAltoParteSuperior, 4, Al - VENTANAEX_ALTURA_PARTE_INFERIOR, An -4, VentanaExAltoParteSuperior);
	}
    return 0;
}


void VentanaEx::PintarTitulo(const bool SoloTitulo, DWL::GDI::DWLhDC *hDCDest) {
	if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
		TCHAR TituloVentanaEx[256];
		GetWindowText(_hWnd, TituloVentanaEx, 256);
		DWL::GDI::DWLIcono	Icono(IDI_MP3);

		if (SoloTitulo == true) { // hay que crear buffer
			int VentanaExAltoParteSuperior = _AltoParteSuperior;
			if (Sistema.App.Config.UtilizarBarraTituloWindows == true) VentanaExAltoParteSuperior -= 22;
			HDC hDC = GetDC(_hWnd);
			DWL::GDI::DWLRecta		RC(_hWnd);
			DWL::GDI::DWLhDC		DC(hDC, RC.Right(), VentanaExAltoParteSuperior +1); // Creo DC Compatible
			DWL::GDI::DWLRecta		Recta(0, 0, RC.Right(), VentanaExAltoParteSuperior -1);
			DC.PintaGradient(DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior, &Recta, false);
			DC.ModoBk(TRANSPARENT);
			DC.PintarIcono(5, 5, 16, 16, IDI_DAP);
			DC.Fuente(Sistema.App.Tahoma15b());
			DC.ColorTexto(RGB(0,0,0));
			DC.PintarTexto(TituloVentanaEx, 26, 6);
			DC.ColorTexto(RGB(255,255,255));
			DC.PintarTexto(TituloVentanaEx, 25, 5);
			DC.PintarMapaBits(hDC, 4, 4, RC.Right() - 70, 20, 4, 4);
			ReleaseDC(_hWnd, hDC);
		}
		else { // el buffer ya esta creado pintamos directamente
			hDCDest->ModoBk(TRANSPARENT);
			hDCDest->PintarIcono(5, 5, 16, 16, IDI_DAP);
			hDCDest->Fuente(Sistema.App.Tahoma15b());
			hDCDest->ColorTexto(RGB(0,0,0));
			hDCDest->PintarTexto(TituloVentanaEx, 26, 6);
			hDCDest->ColorTexto(RGB(255,255,255));
			hDCDest->PintarTexto(TituloVentanaEx, 25, 5);
		}
	}
}

LRESULT VentanaEx::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	if (Redimensionando == false && _Maximizado == false) {
		if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
			RECT RC;
			GetClientRect(_hWnd, &RC);
			int An = RC.right;
			int Al = RC.bottom;
			if (TRUE == Borde.PuntoEnRegion(cX, cY)) {
				if		(cX <= 10)					EO = 0;
				else if (cX > 10 && cX <= An - 10)	EO = 1;
				else								EO = 2;
				if		(cY <= 10)					NS = 0;
				else if (cY > 10 && cY <= Al - 10)	NS = 1;
				else								NS = 2;
			
				// Esquinas
				if (NS == 0 && EO == 0)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNOSE);
				if (NS == 2 && EO == 2)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNOSE);
				if (NS == 2 && EO == 0)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNESO);
				if (NS == 0 && EO == 2)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNESO);
				// Bordes
				if (NS == 0 && EO == 1)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNS);
				if (NS == 2 && EO == 1)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNS);
				if (NS == 1 && EO == 0)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamEO);
				if (NS == 1 && EO == 2)	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamEO);
			}
			else {
				DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_Flecha);
			}
		}
		if (Param == MK_LBUTTON) {
			SendMessage(_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);                                             //  Muevo el Formulario siguiendo la posicion del raton
		}
	}
    return 0;
}



LRESULT VentanaEx::Evento_Temporizador(const UINT cID) {
	switch (cID) {
		case TIMER_MOVIMIENTO :
			WPARAM					Param = NULL;
			DWL::GDI::DWLRecta		UltimaPosicionVentanaEx;
			RECT					Dest;
			POINT					Rata;
			GetWindowRect(_hWnd, &Dest);
//			DWL::DWLDebug::ImprimirDebug(TEXT("Recta Origen (l %d, t %d, r %d, b %d)\n"), Dest.left, Dest.top, Dest.right, Dest.bottom); 
			UltimaPosicionVentanaEx = Dest;
			DWL::SO::DWLMouse::ObtenerPosicion(&Rata);
			// Esquinas
			if (NS == 0 && EO == 0)	{ // Izquierda superior
				Param = WMSZ_TOPLEFT;
				if ((Dest.right - Rata.x) >= MINIMO_PLAYER_ANCHO)	Dest.left = Rata.x;
				else												Dest.left = Dest.right - MINIMO_PLAYER_ANCHO;
				if ((Dest.bottom - Rata.y) >= MINIMO_PLAYER_ALTO)	Dest.top = Rata.y;
				else												Dest.top = Dest.bottom - MINIMO_PLAYER_ALTO;
			}
			if (NS == 2 && EO == 2)	{ // Derecha inferior
				Param = WMSZ_BOTTOMRIGHT;
				if (Rata.x < Dest.right && (Dest.right - Dest.left) <= MINIMO_PLAYER_ANCHO) Dest.right = Dest.left + MINIMO_PLAYER_ANCHO;
				else																		Dest.right = Rata.x;
				if (Rata.y < Dest.bottom && (Dest.bottom - Dest.top) <= MINIMO_PLAYER_ALTO) Dest.bottom = Dest.top + MINIMO_PLAYER_ALTO;
				else																		Dest.bottom = Rata.y;
			}
			if (NS == 2 && EO == 0) { // Izquierda inferior
				Param = WMSZ_TOPRIGHT;
				if ((Dest.right - Rata.x) >= MINIMO_PLAYER_ANCHO)	Dest.left = Rata.x;
				else												Dest.left = Dest.right - MINIMO_PLAYER_ANCHO;
				if (Rata.y < Dest.bottom && (Dest.bottom - Dest.top) <= MINIMO_PLAYER_ALTO) Dest.bottom = Dest.top + MINIMO_PLAYER_ALTO;
				else																		Dest.bottom = Rata.y;
			}
			if (NS == 0 && EO == 2)	{ // Derecha superior
				Param = WMSZ_BOTTOMLEFT;
				if (Rata.x < Dest.right && (Dest.right - Dest.left) <= MINIMO_PLAYER_ANCHO) Dest.right = Dest.left + MINIMO_PLAYER_ANCHO;
				else																		Dest.right = Rata.x;
				if ((Dest.bottom - Rata.y) >= MINIMO_PLAYER_ALTO)	Dest.top = Rata.y;
				else												Dest.top = Dest.bottom - MINIMO_PLAYER_ALTO;
			}

			// Bordes
			if (NS == 0 && EO == 1)	{ // superior
				Param = WMSZ_TOP;
				if ((Dest.bottom - Rata.y) >= MINIMO_PLAYER_ALTO)	Dest.top = Rata.y;
				else												Dest.top = Dest.bottom - MINIMO_PLAYER_ALTO;
			}
			if (NS == 2 && EO == 1) { // inferior
				Param = WMSZ_BOTTOM;
				if (Rata.y < Dest.bottom && (Dest.bottom - Dest.top) <= MINIMO_PLAYER_ALTO) Dest.bottom = Dest.top + MINIMO_PLAYER_ALTO;
				else																		Dest.bottom = Rata.y;
			}
			if (NS == 1 && EO == 0) { // izquierda
				Param = WMSZ_LEFT;
				if ((Dest.right - Rata.x) >= MINIMO_PLAYER_ANCHO)	Dest.left = Rata.x;
				else												Dest.left = Dest.right - MINIMO_PLAYER_ANCHO;
			}
			if (NS == 1 && EO == 2)	{ // derecha
				Param = WMSZ_RIGHT;
				if (Rata.x < Dest.right && (Dest.right - Dest.left) <= MINIMO_PLAYER_ANCHO) Dest.right = Dest.left + MINIMO_PLAYER_ANCHO;
				else																		Dest.right = Rata.x;
			}
//			DWL::DWLDebug::ImprimirDebug(TEXT("Recta Destino (l %d, t %d, r %d, b %d)\n"), Dest.left, Dest.top, Dest.right, Dest.bottom); 
			
			if ((Dest.right - Dest.left) <= MINIMO_PLAYER_ANCHO) Dest.right = Dest.left + MINIMO_PLAYER_ANCHO;
			if ((Dest.bottom - Dest.top) <= MINIMO_PLAYER_ALTO)  Dest.bottom = Dest.top + MINIMO_PLAYER_ALTO;

			if (UltimaPosicionVentanaEx != Dest) {
//				SendMessage(_hWnd, WM_SIZING, Param, (LPARAM)&Dest);
				if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
					DWL::GDI::DWLRegion Region(0, 0, Dest.right - Dest.left, (Dest.bottom - Dest.top) - 0, 11, 11);
					DWL::GDI::DWLRegion Region2(4, 4, Dest.right - Dest.left -4, Dest.bottom - Dest.top - 4, 11, 11);
					// Aqui tenemos el borde para detectar si el mouse esta en el.
					Region.CombinarRegion(&Region2, &Borde, RGN_XOR);
					SetWindowPos(_hWnd, HWND_TOP, Dest.left, Dest.top, Dest.right - Dest.left, (Dest.bottom - Dest.top), SWP_NOZORDER);
	//				DWL::GDI::DWLhDC hDC(_hWnd);
					Region.AsignarRegionVentana(_hWnd, TRUE);
				}
				else {
					SetWindowPos(_hWnd, HWND_TOP, Dest.left, Dest.top, Dest.right - Dest.left, (Dest.bottom - Dest.top), SWP_NOZORDER);
				}
				//Evento_PintarDWLhDC(&hDC);

				//RedrawWindow(_hWnd, NULL, NULL, RDW_NOCHILDREN);
				
				// Cacho de barra gris de abajo
				RECT R1 = { 0, ((Dest.bottom - Dest.top) - VENTANAEX_ALTURA_PARTE_INFERIOR) +1, Dest.right - Dest.left, (Dest.bottom - Dest.top)};
				InvalidateRect(_hWnd, &R1, FALSE);
				// Borde derecho
				RECT R2 = { (Dest.right - Dest.left) - 4, 0, Dest.right - Dest.left, Dest.bottom - Dest.top };
				InvalidateRect(_hWnd, &R2, FALSE);
				// Borde izquierdo
				RECT R3 = { 0, 0, 4, Dest.bottom - Dest.top };
				InvalidateRect(_hWnd, &R3, FALSE);
				// Cacho de barra gris de arriba
				RECT R4 = { 0, 0, Dest.right - Dest.left, VENTANAEX_ALTURA_PARTE_SUPERIOR};
				InvalidateRect(_hWnd, &R4, FALSE);
				//Evento_ValidarControles();
				
				
//				InvalidateRect(_hWnd, &Dest, FALSE);
				/*


				// Del oscy al slider
				RECT R6 = { 0, 84, Dest.right, 88 };
				InvalidateRect(_hWnd, &R6, FALSE);
				// Del slider al final
				RECT R5 = { 0, 96, Dest.right, 103 };
				InvalidateRect(_hWnd, &R5, FALSE);*/
			}
			
			break;
	}
    return 0;
}


LRESULT VentanaEx::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	if (TRUE == Borde.PuntoEnRegion(cX, cY)) {
		Redimensionando = true;
		SetCapture(_hWnd);
		CrearTemporizador(TIMER_MOVIMIENTO, 100);
	}
    return 0;
}


LRESULT VentanaEx::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	Redimensionando = false;
	ReleaseCapture();
	EliminarTemporizador(TIMER_MOVIMIENTO);
	EO = 1;
	NS = 1;
    return 0;
}