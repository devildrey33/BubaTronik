#include "StdAfx.h"
#include "ControlCerrarMinimizar.h"
#include "DimensionesFijas.h"
#include <DWLListaIconos.h>
#include "resource.h"
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);


/*LRESULT CALLBACK ControlCerrarMinimizar::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		ENLACE_Evento_PintarHDC();
		ENLACE_Evento_BorrarFondoHDC();
//		ENLACE_Evento_Mouse_Saliendo();
		ENLACE_Evento_Mouse_Movimiento();
		ENLACE_Evento_Mouse_BotonPresionado();
		ENLACE_Evento_Mouse_BotonSoltado();
	}
	return DWL::ControlesEx::DWLBaseControlEx::GestorMensajes(uMsg, wParam, lParam);
}*/


ControlCerrarMinimizar::ControlCerrarMinimizar(void) {
	Estado = CM_Normal;
	Presionado = false;
}


ControlCerrarMinimizar::~ControlCerrarMinimizar(void) {
}


void ControlCerrarMinimizar::Crear(HWND hWndParent, const int cX, const int cY, const UINT nID, const bool SoloCerrar) {
	HRGN Region;
	_SoloCerrar = SoloCerrar;
	if (_SoloCerrar == false) {
		CrearControl(hWndParent, TEXT("ControlCerrarMinimizar"), WS_CHILD, TEXT(""), cX, cY, CM_ANCHO, CM_ALTO, nID);
		Region = CreateRoundRectRgn(0, -10, CM_ANCHO + 20, CM_ALTO, 11, 11);
	}
	else {
		CrearControl(hWndParent, TEXT("ControlCerrarMinimizar"), WS_CHILD, TEXT(""), cX, cY, CM_ANCHO_SC, CM_ALTO, nID);
		Region = CreateRoundRectRgn(0, -10, CM_ANCHO_SC + 20, CM_ALTO, 11, 11);
	}
	AsignarRegion(Region, false);
}

void ControlCerrarMinimizar::Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool Repintar) {
	HRGN Region;
	if (_SoloCerrar == false)	Region = CreateRoundRectRgn(0, -10, CM_ANCHO + 20, CM_ALTO, 11, 11);
	else						Region = CreateRoundRectRgn(0, -10, CM_ANCHO_SC + 20, CM_ALTO, 11, 11);
	AsignarRegion(Region, false);
	MoveWindow(_hWnd, cX, cY, cAncho, cAlto, Repintar);
//	DWL::ControlesEx::BaseControlEx::Mover(cX, cY, cAncho, cAlto, Repintar);
}


LRESULT ControlCerrarMinimizar::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	Repintar(hDC);
    return 0;
}


void ControlCerrarMinimizar::Repintar(HDC hDC) {
	HDC		hDCDest = hDC;
	RECT	R;
	GetClientRect(_hWnd, &R);
	if (hDCDest == NULL) hDCDest = GetDC(_hWnd);
	// Preparo el buffer para el pintado
	HDC		Buffer		= CreateCompatibleDC(hDCDest);
	HBITMAP Bmp			= CreateCompatibleBitmap(hDCDest, R.right, R.bottom);
	HBITMAP Viejo		= static_cast<HBITMAP>(SelectObject(Buffer, Bmp));
	HBRUSH	Borde		= CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
	HBRUSH	BordeClaro  = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro);;
//	if (Foco() == false)	Borde = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
//	else					Borde = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_FocoRaton);
	// Pinto el fondo
	HRGN Region = CreateRectRgn(0, 0, 0, 0);
	// Pinto la linea del fondo claro
	GetWindowRgn(_hWnd, Region);
	FillRect(Buffer, &R, Borde);
	FrameRgn(Buffer, Region, BordeClaro, VENTANAEX_BORDECLARO, VENTANAEX_BORDECLARO);
	
	if (Sistema.App.PlayerEx.Maximizado() == false) {
		// Repinto la linea del fondo oscuro superior izquierdo
		RECT R2 = { 0, 0, VENTANAEX_BORDECLARO, VENTANAEX_BORDECLARO };
		FillRect(Buffer, &R2, Borde);
		// Repinto la linea del fondo oscuro inferior derecho
		RECT R3 = { R.right - (VENTANAEX_BORDECLARO + 1), R.bottom - (VENTANAEX_BORDECLARO + 1), R.right, R.bottom };
		FillRect(Buffer, &R3, Borde);
	}

	DeleteObject(Region);
	DeleteObject(BordeClaro);
	DeleteObject(Borde);
	
	if (_SoloCerrar == false) {
		switch (Estado) {
			case CM_Normal :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 0, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_MINIMIZAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE)+ CM_ANCHO_ICO, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_RESTAURAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE * 2) + (CM_ANCHO_ICO * 2), CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
			case CM_Minimizar_Resaltado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 0, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_MINIMIZAR_R);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE)+ CM_ANCHO_ICO, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_RESTAURAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE * 2) + (CM_ANCHO_ICO * 2), CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
			case CM_Minimizar_Presionado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 1, CM_BORDE + 2, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_MINIMIZAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE)+ CM_ANCHO_ICO, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_RESTAURAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE * 2) + (CM_ANCHO_ICO * 2), CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
			case CM_Restaurar_Resaltado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 0, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_MINIMIZAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE)+ CM_ANCHO_ICO, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_RESTAURAR_R);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE * 2) + (CM_ANCHO_ICO * 2), CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
			case CM_Restaurar_Presionado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 0, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_MINIMIZAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE+ CM_ANCHO_ICO) + 1, CM_BORDE + 2, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_RESTAURAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE * 2) + (CM_ANCHO_ICO * 2), CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
			case CM_Cerrar_Resaltado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 0, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_MINIMIZAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE)+ CM_ANCHO_ICO, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_RESTAURAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE * 2) + (CM_ANCHO_ICO * 2), CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_R);
				break;
			case CM_Cerrar_Presionado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 0, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_MINIMIZAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, (CM_BORDE)+ CM_ANCHO_ICO, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_RESTAURAR_N);
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, ((CM_BORDE * 2) + (CM_ANCHO_ICO * 2)) + 1, CM_BORDE + 2, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
		}
	}
	else {
		switch (Estado) {
			case CM_Normal :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 4, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
			case CM_Cerrar_Resaltado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 4, CM_BORDE + 1, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_R);
				break;
			case CM_Cerrar_Presionado :
				DWL::SO::DWLListaIconos::PintarIcono(Buffer, 5, CM_BORDE + 2, CM_ANCHO_ICO, CM_ANCHO_ICO, IDI_CERRAR_N);
				break;
		}
	}

	// Pinto el buffer en el dc destino
	BitBlt(hDCDest, 0, 0, R.right, R.bottom, Buffer, 0, 0, SRCCOPY);
	// Elimino memoria del buffer
	SelectObject(Buffer, Viejo);
	DeleteObject(Bmp);
	DeleteDC(Buffer);
	if (hDC == NULL) ReleaseDC(_hWnd, hDCDest);
}


LRESULT ControlCerrarMinimizar::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
//	_MirarMouseLeave();
	
	EstadosCM St = Estado;
	if (_SoloCerrar == false) {
		if (cY > 0 && cY < CM_ALTO) {
			if (cX > 0 && cX < CM_ANCHO_ICO + CM_BORDE) {
				if (Presionado == false)	Estado = CM_Minimizar_Resaltado;
				else						Estado = CM_Minimizar_Presionado;
			}

			else if (cX > CM_ANCHO_ICO + CM_BORDE && cX < ((CM_ANCHO_ICO + CM_BORDE) * 2)) {
				if (Presionado == false)	Estado = CM_Restaurar_Resaltado;
				else						Estado = CM_Restaurar_Presionado;
			}

			else if (cX > ((CM_ANCHO_ICO + CM_BORDE) * 2) && cX < ((CM_ANCHO_ICO + CM_BORDE) * 3)) {
				if (Presionado == false)	Estado = CM_Cerrar_Resaltado;
				else						Estado = CM_Cerrar_Presionado;
			}

			else {
				Estado = CM_Normal;
			}
		}
		else {
			Estado = CM_Normal;
		}
	}
	else {
		if (cY > 0 && cY < CM_ALTO) {
			if (cX > 0 && cX < CM_ANCHO_ICO + CM_BORDE) {
				if (Presionado == false) {	
					Estado = CM_Cerrar_Resaltado;
				}
				else {
					Estado = CM_Cerrar_Presionado;
				}
			}
			else {
				Estado = CM_Normal;
			}
		}
		else {
			Estado = CM_Normal;
		}
	}
	if (St != Estado) Repintar();
    return 0;
}


LRESULT ControlCerrarMinimizar::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	Presionado = true;
	SetCapture(_hWnd);
	return Evento_Mouse_Movimiento(cX, cY, Param);
}


LRESULT ControlCerrarMinimizar::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	Presionado = false;
	ReleaseCapture();
	EstadosCM St = Estado;
	if (_SoloCerrar == false) {
		if (cY > 0 && cY < CM_ALTO) {
			// Minimizar
			if (cX > 0 && cX < CM_ANCHO_ICO + CM_BORDE) {
				PostMessage(GetParent(_hWnd), WM_SYSCOMMAND, SC_MINIMIZE, NULL);
				Estado = CM_Minimizar_Resaltado;
			}
			// Maximizar / restaurar
			else if (cX > CM_ANCHO_ICO + CM_BORDE && cX < ((CM_ANCHO_ICO + CM_BORDE) * 2)) {
				if (Sistema.App.PlayerEx.Maximizado() == true)	PostMessage(GetParent(_hWnd), WM_SYSCOMMAND, SC_RESTORE, NULL);
				else 									PostMessage(GetParent(_hWnd), WM_SYSCOMMAND, SC_MAXIMIZE, NULL);			
				Estado = CM_Restaurar_Resaltado;
			}
			// Cerrar
			else if (cX > ((CM_ANCHO_ICO + CM_BORDE) * 2) && cX < ((CM_ANCHO_ICO + CM_BORDE) * 3)) {
				PostMessage(GetParent(_hWnd), WM_SYSCOMMAND, SC_CLOSE, NULL);
				Estado = CM_Cerrar_Resaltado;
			}
			else {
				Estado = CM_Normal;
			}
		}
		else {
			Estado = CM_Normal;
		}
	}
	else {
		if (cY > 0 && cY < CM_ALTO) {
			if (cX > 0 && cX < CM_ANCHO_ICO + CM_BORDE) {
				PostMessage(GetParent(_hWnd), WM_SYSCOMMAND, SC_CLOSE, NULL);
				Estado = CM_Cerrar_Resaltado;
			}
			else {
				Estado = CM_Normal;
			}
		}
		else {
			Estado = CM_Normal;
		}
	}
	if (St != Estado) Repintar();
    return 0;
}


LRESULT ControlCerrarMinimizar::Evento_Mouse_Saliendo(void) {
	bool Rp = false;
	if (Estado != CM_Normal) Rp = true;
	Estado = CM_Normal;
	if (Rp == true) Repintar();
    return 0;
}

