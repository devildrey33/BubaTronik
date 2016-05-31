#include "stdafx.h"
#include "VentanaTranslucidaLista.h"
#include "BubaTronikApp.h"

void VentanaTranslucidaLista::PintarContenido(HDC hDCDestino, RECT *RC) {
	Sistema.App.ListaC.ListaEx_Pintar(hDCDestino, RC, false, false);
}

LRESULT VentanaTranslucidaLista::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	RECT RCContenido = { Espacio.left + 3, Espacio.top + 15, Espacio.right - 6, Espacio.bottom - 6 };
	POINT Pt = { cX, cY };
	if (PtInRect(&RCContenido, Pt) == TRUE) {
		Sistema.App.ListaC.ListaEx_Mouse_Movimiento(Sistema.App.ListaC.hWnd(), &RCContenido, cX, cY, Param);
		return 1;
	}
	return 0;
}

LRESULT VentanaTranslucidaLista::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	RECT RCContenido = { Espacio.left + 3, Espacio.top + 15, Espacio.right - 6, Espacio.bottom - 6 };
	POINT Pt = { cX, cY };
	if (PtInRect(&RCContenido, Pt) == TRUE) {
		Sistema.App.ListaC.ListaEx_Mouse_BotonSoltado(Sistema.App.ListaC.hWnd(), &RCContenido, Boton, cX, cY, Param);
		return 1;
	}
	return 0;
}

LRESULT VentanaTranslucidaLista::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	RECT RCContenido = { Espacio.left + 3, Espacio.top + 15, Espacio.right - 6, Espacio.bottom - 6 };
	POINT Pt = { cX, cY };
	if (PtInRect(&RCContenido, Pt) == TRUE) {
		Sistema.App.ListaC.ListaEx_Mouse_BotonPresionado(Sistema.App.ListaC.hWnd(), &RCContenido, Boton, cX, cY, Param);
		return 1;
	}
	return 0;
}