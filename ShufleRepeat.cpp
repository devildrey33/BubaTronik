#include "StdAfx.h"
#include "ShufleRepeat.h"
#include "DimensionesFijas.h"
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);

#define ID_SHUFLE_REPEAT 999



/*LRESULT CALLBACK ShufleRepeat::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		ENLACE_Evento_PintarHDC();
		case WM_ERASEBKGND	:  return 1;	
		ENLACE_Evento_Mouse_Movimiento();
		ENLACE_Evento_Mouse_BotonPresionado();
		ENLACE_Evento_Mouse_BotonSoltado();
	}
	return DWL::ControlesEx::DWLBaseControlEx::GestorMensajes(uMsg, wParam, lParam);
}*/



ShufleRepeat::ShufleRepeat(void) {
	Estado_Shufle = ShufleRepeat_Estado_Normal;
	Estado_Repeat = ShufleRepeat_Estado_Normal;
	Shufle_Marcado = false;
	Repeat_Marcado = false;
}

ShufleRepeat::~ShufleRepeat(void) {
}


void ShufleRepeat::CrearShufleRepeat(HWND hWndParent, const int cX, const int cY) {
	Region.CrearRegionRectaRedondeada(0, 0, ANCHO_BOTON_SHUFLE_REPEAT * 2 , ALTO_BOTON_SHUFLE_REPEAT, 13, 13);
	CrearControl(hWndParent, TEXT("ShufleRepeat"), WS_CHILD | WS_VISIBLE, NULL, cX, cY, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT, ID_SHUFLE_REPEAT);
	Region.AsignarRegionVentana(_hWnd, FALSE);
	Shufle_Marcado = Sistema.App.Config.Shufle;
	if (Sistema.App.Config.Repeat > 0) Repeat_Marcado = true;
//	Visible(true);
}


LRESULT ShufleRepeat::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	DWL::GDI::DWLhDC	Buffer(hDC, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT);
	DWL::GDI::DWLRecta	RShufle(0, 4, ANCHO_BOTON_SHUFLE_REPEAT, ALTO_BOTON_SHUFLE_REPEAT);
	DWL::GDI::DWLRecta	RRepeat(ANCHO_BOTON_SHUFLE_REPEAT, 4, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT);
	Buffer.ModoBk(TRANSPARENT);
	switch (Estado_Shufle) {
		case ShufleRepeat_Estado_Normal :
			Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Normal);
			if (Shufle_Marcado == false) {
				DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_SHUFLE_REPEAT, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			}
			else {
				DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_SHUFLE_REPEAT, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
				RShufle.operator ++();
			}
			Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
			Buffer.PintarTextoCentrado(IDIOMA__SHUFLE, &RShufle);
			break;
		case ShufleRepeat_Estado_Resaltado :
			Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Resaltado);
			if (Shufle_Marcado == false) {
				DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_SHUFLE_REPEAT, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			}
			else {
				DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_SHUFLE_REPEAT, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, true);
				RShufle.operator ++();
			}
			Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13_SubRayada());
			Buffer.PintarTextoCentrado(IDIOMA__SHUFLE, &RShufle);
			break;
		case ShufleRepeat_Estado_Presionado :
			Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Presionado);
			DWL::GDI::PintarGradient(Buffer.hDC(), 0, 0, ANCHO_BOTON_SHUFLE_REPEAT, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
			RShufle.operator ++();
			Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
			Buffer.PintarTextoCentrado(IDIOMA__SHUFLE, &RShufle);
			break;
	}

	switch (Estado_Repeat) {
		case ShufleRepeat_Estado_Normal :
			Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Normal);
			if (Repeat_Marcado == false) {
				DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_SHUFLE_REPEAT, 0, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			}
			else {
				DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_SHUFLE_REPEAT, 0, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
				RRepeat.operator ++();
			}
			Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
			Buffer.PintarTextoCentrado(IDIOMA__REPEAT, &RRepeat);
			break;
		case ShufleRepeat_Estado_Resaltado :
			Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Resaltado);
			if (Repeat_Marcado == false) {
				DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_SHUFLE_REPEAT, 0, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, true);
			}
			else {
				DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_SHUFLE_REPEAT, 0, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado, true);
				RRepeat.operator ++();
			}
			Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13_SubRayada());
			Buffer.PintarTextoCentrado(IDIOMA__REPEAT, &RRepeat);
			break;
		case ShufleRepeat_Estado_Presionado :
			Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Presionado);
			DWL::GDI::PintarGradient(Buffer.hDC(), ANCHO_BOTON_SHUFLE_REPEAT, 0, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, true);
			RRepeat.operator ++();
			Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
			Buffer.PintarTextoCentrado(IDIOMA__REPEAT, &RRepeat);
			break;
	}

	DWL::GDI::DWLPluma Pluma(RGB(100, 100, 100), PS_SOLID, 1);
	Pluma.PintarLinea(&Buffer, ANCHO_BOTON_SHUFLE_REPEAT, 0, ANCHO_BOTON_SHUFLE_REPEAT, ALTO_BOTON_SHUFLE_REPEAT);

//	COLORREF ColorBorde = DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal;
	DWL::GDI::DWLBrocha Borde;
	if (Foco() == false) {
		if (Estado_Repeat != ShufleRepeat_Estado_Presionado && Estado_Shufle != ShufleRepeat_Estado_Presionado) {
			Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
		}
		else {
			Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_FocoRaton);
		}
	}
	else  {
		Borde.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_FocoRaton);
	}
	

	/*BYTE R = GetRValue(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
	if (R < 255 - 32)	R += 32;
	else				R = 255;
	BYTE G = GetGValue(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
	if (G < 255 - 32)	G += 32;
	else				G = 255;
	BYTE B = GetBValue(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
	if (B < 255 - 32)	B += 32;
	else				B = 255;*/
	DWL::GDI::DWLBrocha BordeLight(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro);
	Region.EnmarcarRegion(&Buffer, &BordeLight, 2, 2);

	Region.EnmarcarRegion(&Buffer, &Borde, 1, 1);


	Buffer.PintarMapaBits(hDC, 0, 0, ANCHO_BOTON_SHUFLE_REPEAT * 2, ALTO_BOTON_SHUFLE_REPEAT, 0, 0);
    return 0;
}


LRESULT ShufleRepeat::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	if (Estado_Shufle != ShufleRepeat_Estado_Presionado) Estado_Shufle = ShufleRepeat_Estado_Normal;
	if (Estado_Repeat != ShufleRepeat_Estado_Presionado) Estado_Repeat = ShufleRepeat_Estado_Normal;
	if (cX > 0 && cX < ANCHO_BOTON_SHUFLE_REPEAT) {
		if (Estado_Shufle != ShufleRepeat_Estado_Presionado) Estado_Shufle = ShufleRepeat_Estado_Resaltado;
	}
	else if (cX > ANCHO_BOTON_SHUFLE_REPEAT && cX < ANCHO_BOTON_SHUFLE_REPEAT * 2) {
		if (Estado_Repeat != ShufleRepeat_Estado_Presionado) Estado_Repeat = ShufleRepeat_Estado_Resaltado;
	}
	Repintar();
    return 0;
}


LRESULT ShufleRepeat::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	SetCapture(_hWnd);
	if (cX > 0 && cX < ANCHO_BOTON_SHUFLE_REPEAT) {
		Estado_Shufle = ShufleRepeat_Estado_Presionado;
	}
	else if (cX > ANCHO_BOTON_SHUFLE_REPEAT && cX < ANCHO_BOTON_SHUFLE_REPEAT * 2) {
		Estado_Repeat = ShufleRepeat_Estado_Presionado;
	}
	Repintar();
    return 0;
}


LRESULT ShufleRepeat::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	ReleaseCapture();
	if (cX > 0 && cX < ANCHO_BOTON_SHUFLE_REPEAT) {
		if (Estado_Shufle == ShufleRepeat_Estado_Presionado) {
			DWL::ControlesEx::DWLBotonEx_ParametrosClick Params(cX, cY, ID_BOTON_SHUFLE, Boton, Param);
			SendMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO, reinterpret_cast<WPARAM>(&Params), NULL);
		}
		Estado_Shufle = ShufleRepeat_Estado_Resaltado;
		Estado_Repeat = ShufleRepeat_Estado_Normal;
	}
	else if (cX > ANCHO_BOTON_SHUFLE_REPEAT && cX < ANCHO_BOTON_SHUFLE_REPEAT * 2) {
		if (Estado_Repeat == ShufleRepeat_Estado_Presionado) {
			DWL::ControlesEx::DWLBotonEx_ParametrosClick Params(cX, cY, ID_BOTON_REPEAT, Boton, Param);
			SendMessage(GetParent(_hWnd), DWL_BOTONEX_MOUSE_BOTONSOLTADO, reinterpret_cast<WPARAM>(&Params), NULL);
		}
		Estado_Shufle = ShufleRepeat_Estado_Normal;
		Estado_Repeat = ShufleRepeat_Estado_Resaltado;
	}
    return 0;
}


void ShufleRepeat::Repintar(void) {
	DWL::GDI::DWLhDC hDC(_hWnd);
    PAINTSTRUCT PS;
	Evento_Pintar(hDC.hDC(), PS);
}


LRESULT ShufleRepeat::Evento_Mouse_Saliendo(void) {
	Estado_Shufle = ShufleRepeat_Estado_Normal;
	Estado_Repeat = ShufleRepeat_Estado_Normal;
	Repintar();
    return 0;
}


void ShufleRepeat::ShufleMarcado(const bool Marcado) {
	Shufle_Marcado = Marcado;
	Repintar();
}

void ShufleRepeat::RepeatMarcado(const bool Marcado) {
	Repeat_Marcado = Marcado;
	Repintar();
}