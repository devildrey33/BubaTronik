#include "StdAfx.h"
#include "ControlTiempoActual.h"
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);

ControlTiempoActual::ControlTiempoActual(void) {
}


ControlTiempoActual::~ControlTiempoActual(void) {
}


HWND ControlTiempoActual::CrearCTA(HWND hWndParent, const int cX, const int cY) {
	CrearControl(hWndParent, TEXT("BubaTronik_ControlTiempoActual"), WS_CHILD | WS_VISIBLE, TEXT(""), cX, cY, ANCHO_TIEMPO_ACTUAL, ALTO_TIEMPO_ACTUAL, 100);
	return _hWnd;
}


LRESULT ControlTiempoActual::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	Repintar(hDC, true);
    return 0;
}


void ControlTiempoActual::Repintar(HDC hDC, const bool Forzar) {
//	HDC						hDCCompatible	= CreateCompatibleDC(NULL);
//	HBITMAP					BufferBMP		= CreateCompatibleBitmap(hDCCompatible, ANCHO_TIEMPO_ACTUAL, ALTO_TIEMPO_ACTUAL);
//	HBITMAP					Viejo			= static_cast<HBITMAP>(SelectObject(hDCCompatible, BufferBMP));
	DWL::GDI::DWLhDC		Buffer(hDC, ANCHO_TIEMPO_ACTUAL, ALTO_TIEMPO_ACTUAL);
	DWL::DWLString			Txt;
	RECT FondoTA = { 0, -24, ANCHO_TIEMPO_ACTUAL, VENTANAEX_ALTURA_PARTE_SUPERIOR -24 };
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) {
		FondoTA.top = -3;
		FondoTA.bottom = VENTANAEX_ALTURA_PARTE_SUPERIOR - 25;
	}
	Buffer.PintaGradient(DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior, &FondoTA, false);
	UINT64 TiempoTotal	= Sistema.App.Media.TiempoTotal();
	UINT64 TiempoActual	= Sistema.App.Media.TiempoActualMs();
	
	Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
	Txt = Sistema.App.Media.TiempoStrL(TiempoActual);
	Txt += TEXT(" / ");
	Txt += Sistema.App.Media.TiempoStrL(TiempoTotal);
	
	Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloSombra);
	Buffer.ModoBk(TRANSPARENT);
	RECT RC;
	GetClientRect(_hWnd, &RC);
	Buffer.PintarTextoCentrado(Txt(), &RC);
//	Buffer.PintarTexto(Txt(), 1, 1);
	Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Normal);
//	Buffer.PintarTexto(Txt(), 0, 0);
	RC.left --; RC.right --; RC.top --; RC.bottom --;
	Buffer.PintarTextoCentrado(Txt(), &RC);
	Buffer.PintarMapaBits(hDC, 0, 0, ANCHO_TIEMPO_ACTUAL, ALTO_TIEMPO_ACTUAL, 0, 0);
//	Buffer.Fuente((HFONT)NULL);
}


/*LRESULT CALLBACK ControlTiempoActual::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		ENLACE_Evento_PintarHDC();
		case WM_ERASEBKGND	:  return 1;	
	}
	return DWL::ControlesEx::DWLBaseControlEx::GestorMensajes(uMsg, wParam, lParam);
}
*/