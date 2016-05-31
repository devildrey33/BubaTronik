#include "StdAfx.h"
#include "VentanaPrincipalEx.h"
#include "ConfigBubaTronik.h"
#include "BubaTronikApp.h"
#include <DWLMouse.h>

VentanaPrincipalEx::VentanaPrincipalEx(void) {
	_MousePresionado = false;
	_RedimensionandoVentana = false;
}

VentanaPrincipalEx::~VentanaPrincipalEx(void) {
	for (size_t i = 0; i < _Ventanas.size(); i++) delete _Ventanas[i];
}


void VentanaPrincipalEx::Crear(void) {
	
	DWL::DWLString Titulo;
	Titulo.sprintf(TEXT("%s %s"), APP_NOMBRE, APP_STR_VERSION);
	CrearVentana(NULL, TEXT("BubaTronik_VentanaPrincipal"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, TEXT("BubaTronik"), Sistema.App.Config.X, Sistema.App.Config.Y, Sistema.App.Config.Ancho, Sistema.App.Config.Alto, NULL);
	CrearVentanaTranslucida(0);
	CrearVentanaTranslucida(1);
}


LRESULT VentanaPrincipalEx::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	Pintar(hDC);
    return 0;
}


LRESULT VentanaPrincipalEx::Evento_Cerrar(void) {
	PostQuitMessage(0);
	return 0;
}



LRESULT VentanaPrincipalEx::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	if (_MousePresionado == true) {
		if (_RedimensionandoVentana == true) { // Redimensionado ventana
			switch (_PuntoRedimension) {
				case 1 : 
					if (cX < _VentanaPresionada->Espacio.right - 20) _VentanaPresionada->Espacio.left = cX;
					if (cY > _VentanaPresionada->Espacio.top + 20) _VentanaPresionada->Espacio.bottom = cY;
					break;
				case 2 : 
					if (cY > _VentanaPresionada->Espacio.top + 20) _VentanaPresionada->Espacio.bottom = cY;
					break;
				case 3 :
					if (cX > _VentanaPresionada->Espacio.left + 20) _VentanaPresionada->Espacio.right = cX;
					if (cY > _VentanaPresionada->Espacio.top + 20) _VentanaPresionada->Espacio.bottom = cY;
					break;
				case 4 :
					if (cX < _VentanaPresionada->Espacio.right - 20) _VentanaPresionada->Espacio.left = cX;
					break;
				case 6 :
					if (cX > _VentanaPresionada->Espacio.left + 20) _VentanaPresionada->Espacio.right = cX;
					break;
				case 7 :
					if (cX < _VentanaPresionada->Espacio.right - 20) _VentanaPresionada->Espacio.left = cX;
					if (cY < _VentanaPresionada->Espacio.bottom - 20) _VentanaPresionada->Espacio.top = cY;
					break;
				case 8 :
					if (cY < _VentanaPresionada->Espacio.bottom - 20) _VentanaPresionada->Espacio.top = cY;
					break;
				case 9 :
					if (cX > _VentanaPresionada->Espacio.left + 20) _VentanaPresionada->Espacio.right = cX;
					if (cY < _VentanaPresionada->Espacio.bottom - 20) _VentanaPresionada->Espacio.top = cY;
					break;
			}
			Repintar();
		}
		else { // Movimiento ventana
			if (_VentanaPresionada != NULL) {
				OffsetRect(&_VentanaPresionada->Espacio, cX - _VentanaPresionada->PuntoPresion.x, cY - _VentanaPresionada->PuntoPresion.y);
				_VentanaPresionada->PuntoPresion.x = cX;
				_VentanaPresionada->PuntoPresion.y = cY;
				Repintar();
			}
		}
	}
	else {
		// Deteccion de bordes de las ventanas
		if (MirarBordeVentanas(cX, cY, _PuntoRedimension) == true) {
			switch (_PuntoRedimension) {
				case 1 : case 9 : DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNESO);	break;
				case 2 : case 8 : DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNS);	break;
				case 3 : case 7 : DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamNOSE);	break;
				case 4 : case 6 : DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_TamEO);	break;
			}
		}
		else {
			VentanaTranslucida *nVentana = VentanaEnPunto(cX, cY);
			if (nVentana != NULL) nVentana->Evento_Mouse_Movimiento(cX - nVentana->Espacio.left, cY - nVentana->Espacio.top, Param);

			DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_Flecha);
		}
	}
    return 0;
}


LRESULT VentanaPrincipalEx::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	_VentanaPresionada = VentanaEnPunto(cX, cY);
	if (_VentanaPresionada != NULL) {
		VentanaActiva(_VentanaPresionada);
		if (MirarBordeVentanas(cX, cY, _PuntoRedimension) == true) {
			_MousePresionado = true;
			_VentanaPresionada->Presionada = true;
			_VentanaPresionada->PuntoPresion.x = cX;
			_VentanaPresionada->PuntoPresion.y = cY;
			_RedimensionandoVentana = true;
			return 0;
		}
		if (_VentanaPresionada->Evento_Mouse_BotonPresionado(Boton, cX - _VentanaPresionada->Espacio.left, cY - _VentanaPresionada->Espacio.top, Param) == 0) {
			_MousePresionado = true;
			_VentanaPresionada->Presionada = true;
			_VentanaPresionada->PuntoPresion.x = cX;
			_VentanaPresionada->PuntoPresion.y = cY;
		}
	}
    return 0;
}

LRESULT VentanaPrincipalEx::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	if (_VentanaPresionada != NULL) _VentanaPresionada->Evento_Mouse_BotonSoltado(Boton, cX - _VentanaPresionada->Espacio.left, cY - _VentanaPresionada->Espacio.top, Param);
	for (size_t i = 0; i < _Ventanas.size(); i++) {
		_Ventanas[i]->Presionada = false;
	}
	_VentanaPresionada = NULL;
	_MousePresionado = false;
	_RedimensionandoVentana = false;
    return 0;
}


void VentanaPrincipalEx::CrearVentanaTranslucida(const int TipoVentana) {
	VentanaTranslucida      *NuevaVentana = NULL;
	VentanaTranslucidaLista *NuevaVentanaL = NULL;
	switch (TipoVentana) {
		case TIPO_VENTANA_INICIO :
			NuevaVentana = new VentanaTranslucida();
			NuevaVentana->Crear(TipoVentana, 0, _Ventanas.size());
			if (NuevaVentana != NULL) {
				for (size_t i = 0; i < _Ventanas.size(); i++) _Ventanas[i]->OrdenZ ++;
				_Ventanas.insert(_Ventanas.begin(), NuevaVentana); // Insertamos la ventana al principio del vector
			}
			break;
		case TIPO_VENTANA_LISTA_MEDIOS :
			NuevaVentanaL = new VentanaTranslucidaLista();
			NuevaVentanaL->Crear(TipoVentana, 0, _Ventanas.size());
			if (NuevaVentanaL != NULL) {
				for (size_t i = 0; i < _Ventanas.size(); i++) _Ventanas[i]->OrdenZ ++;
				_Ventanas.insert(_Ventanas.begin(), NuevaVentanaL); // Insertamos la ventana al principio del vector
			}
			break;
	}

}


void VentanaPrincipalEx::Pintar(HDC hDCDestino) {
	HDC hDC;
	if (hDCDestino == NULL) hDC = GetDC(_hWnd);
	else					hDC = hDCDestino;
	RECT RC;
	GetClientRect(_hWnd, &RC);
	HDC		Buffer	= CreateCompatibleDC(hDC);
	HBITMAP Bmp		= CreateCompatibleBitmap(hDC, RC.right, RC.bottom);
	HBITMAP Viejo	= static_cast<HBITMAP>(SelectObject(Buffer, Bmp));

	FillRect(Buffer, &RC, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

	for (int i = _Ventanas.size() -1; i > -1 ; i--) {
		_Ventanas[i]->Pintar(Buffer);
	}

	BitBlt(hDC, 0, 0, RC.right, RC.bottom, Buffer, 0, 0, SRCCOPY);

	SelectObject(Buffer, Viejo);
	DeleteObject(Bmp);
	DeleteDC(Buffer);

}

// Funcion que retorna true si el puntero del mouse por si esta en un borde de alguna ventana
// En la variable Punto obtenemos la posicion del mouse que tiene en el borde, para interpretarlo basta con mirar el teclado numerico

const bool VentanaPrincipalEx::MirarBordeVentanas(const int cX, const int cY, int &Punto) {
	// Puntos cardinales (Norte - Sud) (Este - Oeste)
	// 0 es el primer punto (Norte - Este)
	// 1 es en medio
	// 2 es el segundo punto (Sud - Oeste);
	int NS = 0;
	int EO = 0;
	Punto = 0;
	VentanaTranslucida *Borde = VentanaEnPunto(cX, cY);
	if (Borde != NULL) {
		// El borde tiene 2 pixeles, pero aumentamos a 4 para tener mas sensibilidad con el mouse
		HRGN Borde1 = CreateRoundRectRgn(Borde->Espacio.left, Borde->Espacio.top, Borde->Espacio.right, Borde->Espacio.bottom, 12, 12);
		HRGN Borde2 = CreateRoundRectRgn(Borde->Espacio.left + 4, Borde->Espacio.top + 4, Borde->Espacio.right - 4, Borde->Espacio.bottom - 4, 12, 12);
		HRGN BordeFinal = CreateRectRgn(0, 0, 0, 0);
		CombineRgn(BordeFinal, Borde1, Borde2, RGN_XOR);
		DeleteObject(Borde1);
		DeleteObject(Borde2);
		if (PtInRegion(BordeFinal, cX, cY) == TRUE) {
			// Aqui aumentamos el borde hasta 6 pixels para aumentar la sensibilidad en las puntas
			// Miro en que posicion esta el cursor relativamente de la ventana 
			if (cX >= Borde->Espacio.left && cX <= Borde->Espacio.left + 6) {				// EO = 0;
				if (cY >= Borde->Espacio.top && cY <= Borde->Espacio.top + 6)				Punto = 7; //NS = 0;
				else if (cY <= Borde->Espacio.bottom && cY >= Borde->Espacio.bottom - 6)	Punto = 1; //NS = 2;
				else																		Punto = 4; //NS = 1;
			}
			else if (cX <= Borde->Espacio.right && cX >= Borde->Espacio.right - 6) {		// EO = 2;
				if (cY >= Borde->Espacio.top && cY <= Borde->Espacio.top + 6)				Punto = 9; //NS = 0;
				else if (cY <= Borde->Espacio.bottom && cY >= Borde->Espacio.bottom - 6)	Punto = 3; //NS = 2;
				else																		Punto = 6; //NS = 1;
			}
			else {																			// EO = 1;
				if (cY >= Borde->Espacio.top && cY <= Borde->Espacio.top + 6)				Punto = 8; //NS = 0;
				else if (cY <= Borde->Espacio.bottom && cY >= Borde->Espacio.bottom - 6)	Punto = 2; //NS = 2;
				else																		Punto = 5; //NS = 1;
			}
		}
		DeleteObject(BordeFinal);
	}
	if (Punto != 0 && Punto != 5)	return true;
	else							return false;
}

// Funcion que retorna la ventana que esta delante y dentro de las coordenadas especificadas.
VentanaTranslucida *VentanaPrincipalEx::VentanaEnPunto(const int cX, const int cY) {
	POINT Punto = { cX, cY };
	for (size_t i = 0; i < _Ventanas.size(); i++) {
		if (PtInRect(&_Ventanas[i]->Espacio, Punto) == TRUE) return _Ventanas[i];
	}
	return NULL;
}

void VentanaPrincipalEx::VentanaActiva(VentanaTranslucida *nVentana) {
	if (nVentana == NULL) return;
	VentanaTranslucida *Tmp = NULL;
	// actualizo los OrdenZ
	for (size_t i = nVentana->OrdenZ; i > 0 ; i--) {
		Tmp = _Ventanas[i -1];
		_Ventanas[i] = Tmp;
		_Ventanas[i]->OrdenZ ++;
	}
	_Ventanas[0] = nVentana;
	_Ventanas[0]->OrdenZ = 0;
	Repintar();
}

void VentanaPrincipalEx::Repintar(void) {
	HDC hDC = GetDC(_hWnd);
	Pintar(hDC);
	ReleaseDC(_hWnd, hDC);
}







