#include "StdAfx.h"
#include "Escena.h"
#include "ControlPresentacionEx.h"
#include "BubaTronikApp.h"
#include "DimensionesFijas.h"
#include <DWLListaIconos.h>

std::vector <ObjetoEscenaPalabraPulsable *> Escena::PalabrasPulsables;
std::vector <ObjetoEscenaIconoPulsable *>   Escena::IconosPulsables;

Escena::Escena(ControlPresentacionEx *CPEX) {
	CP = CPEX;
	DWL::GDI::DWLRecta R(CP->hWnd());
	HDC hDC = GetDC(CP->hWnd());
	Buffer.CrearCompatibleDCBMP(hDC, R.Right(), 2000);
	DWL::GDI::DWLBrocha Brocha(/*Sistema.App.Sk.Ventana_Fondo*/ RGB(255,255,255));
	Buffer.PintarRecta(&R, &Brocha);
	Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
	ReleaseDC(CP->hWnd(), hDC);
	UPP = NULL;
	UltimaIDResaltada = 0;
}


void Escena::Pintar(HDC hDC, const int cX, const int cY) {
	RECT R;
	GetClientRect(CP->hWnd(), &R);
	Buffer.PintarMapaBits(hDC, 0, 0, R.right, R.bottom, cX, cY);
//	Buffer.CrearCompatibleDCBMP(hDC, R.right, 2000);
}


const UINT Escena::PintarEscena(void) {
	UINT YActual = MARGEN_ESCENA;
	BorrarPulsables();
//	unsigned int z;

	// Calculamos los tamaños para las filas / columnas
/*	for (z = 0; z < Objetos.size(); z++) {
		if (Objetos[z]->Tipo() == ObjetoEscena_FrameH) Objetos[z]->Alto(Objetos[z]->FrameH()->CalcularH());
	}*/

	// Obtengo el tamaño requerido para la escena
	RECT RC;
	GetClientRect(CP->hWnd(), &RC);
	RC.right -= 18; // quito el scroll de la recta
	RC.bottom = (CalcularAltoV(RC.right) + (MARGEN_ESCENA * 2)); // obtengo la altura estimada para todos los objetos

	// Creo un dc compatible con el ControlPresentacionEx para guardar la escena y repintarla rapidamente.
	HDC hDC = GetDC(CP->hWnd());
	Buffer.CrearCompatibleDCBMP(hDC, RC.right, RC.bottom);
	ReleaseDC(CP->hWnd(), hDC);

	// Pinto el fondo de la escena
	HBRUSH Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
	FillRect(Buffer(), &RC, Brocha);
	DeleteObject(Brocha);

	return PintarV(Buffer(), 0, MARGEN_ESCENA, RC.right, RC.bottom);

/*	for (z = 0; z < Objetos.size(); z++) {
		Objetos[z]->Pintar(Buffer(), 0, YActual, RC.right, YActual - CP->BarraScrollEx_ValorV());
		YActual = YActual + SEPARACION_ESCENA + Objetos[z]->Alto();
	}
	return YActual;*/
}



void Escena::Evento_Mouse_Movimiento(const int cX, const int cY, WPARAM wParam, const UINT BarraScrollEx_ValorV) {
	POINT Punto = { cX, cY };
	int  IDResaltada = -1;
	UINT IDIconoResaltado = 0;
	size_t i;
	// Iconos pulsables
	for (i = 0; i < IconosPulsables.size(); i++) {
		if (PtInRect(&IconosPulsables[i]->Espacio, Punto) == TRUE) {
			if (IconosPulsables[i]->Estado != 2) {
				IconosPulsables[i]->Estado = 1;
				IDIconoResaltado = IconosPulsables[i]->IDComando;
			}
		}
		else {
			if (IconosPulsables[i]->Estado != 2) IconosPulsables[i]->Estado = 0;
		}
	}
	Pintar_IconosPulsables(BarraScrollEx_ValorV);


	// Palabras pulsables
	// Determino que palabra esta resaltada
	for (i = 0; i < PalabrasPulsables.size(); i++) {
		if (PtInRect(&PalabrasPulsables[i]->Espacio, Punto) == TRUE) {
			if (PalabrasPulsables[i]->Estado != 2) {
				PalabrasPulsables[i]->Estado = 1;
				IDResaltada = PalabrasPulsables[i]->Palabra->ID;
			}
		}
		else {
			if (PalabrasPulsables[i]->Estado != 2) PalabrasPulsables[i]->Estado = 0;
		}
	}
	if (UltimaIDResaltada != IDResaltada || UltimaIDIcono != IDIconoResaltado) {
		KillTimer(CP->hWnd(), DWL_TIMER_TOOLTIP);
		SetTimer(CP->hWnd(), DWL_TIMER_TOOLTIP, DWL_TIEMPO_TOOLTIP, NULL);
		if (UltimaIDResaltada != IDResaltada)	SendMessage(CP->hWnd(), DWL_TOOLTIPEX_OCULTAR, IDResaltada, 0);
		else									SendMessage(CP->hWnd(), DWL_TOOLTIPEX_OCULTAR, IDIconoResaltado, 0);
		UltimaIDResaltada = IDResaltada;
		UltimaIDIcono = IDIconoResaltado;
	}

	// Asigno el estado resaltado a otras posibles palabras con la misma ID
	if (IDResaltada != -1) {
		for (i = 0; i < PalabrasPulsables.size(); i++) {
			if (PalabrasPulsables[i]->Palabra->ID == IDResaltada) PalabrasPulsables[i]->Estado = 1;
		}
	}
	Pintar_PalabrasPulsables(BarraScrollEx_ValorV);
}


void Escena::Evento_Mouse_BotonPresionado(const int Boton, const int cX, const int cY, WPARAM wParam, const UINT BarraScrollEx_ValorV) {
	POINT Punto = { cX, cY };
	int IDPresionada = -1;
	UINT IDIconoPresionado = 0;
	size_t i;
	// Iconos pulsables
	for (i = 0; i < IconosPulsables.size(); i++) {
		if (PtInRect(&IconosPulsables[i]->Espacio, Punto) == TRUE) {
			IconosPulsables[i]->Estado = 2;
			IDIconoPresionado = IconosPulsables[i]->IDComando;
			SetCapture(CP->hWnd());
		}
		else {
			IconosPulsables[i]->Estado = 0;
		}
	}
	Pintar_IconosPulsables(BarraScrollEx_ValorV);

	// Palabras pulsables
	// Determino la ID pulsada
	for (i = 0; i < PalabrasPulsables.size(); i++) {
		if (PtInRect(&PalabrasPulsables[i]->Espacio, Punto) == TRUE) {
			PalabrasPulsables[i]->Estado = 2;
			IDPresionada = PalabrasPulsables[i]->Palabra->ID;
			SetCapture(CP->hWnd());
		}
		else {
			PalabrasPulsables[i]->Estado = 0;
		}
	}
	if (IDPresionada != -1) {
		for (i = 0; i < PalabrasPulsables.size(); i++) {
			if (PalabrasPulsables[i]->Palabra->ID == IDPresionada) PalabrasPulsables[i]->Estado = 2;
		}
	}
	Pintar_PalabrasPulsables(BarraScrollEx_ValorV);
}


void Escena::Evento_Mouse_BotonSoltado(const int Boton, const int cX, const int cY, WPARAM wParam, const UINT BarraScrollEx_ValorV) {
	POINT Punto = { cX, cY };
	int IDSoltada = -1;
	UINT IDIconoSoltado = 0;
	size_t i;
				
	ReleaseCapture();

	// Iconos pulsables
	for (i = 0; i < IconosPulsables.size(); i++) {
		if (PtInRect(&IconosPulsables[i]->Espacio, Punto) == TRUE) {
			if (IconosPulsables[i]->Estado == 2) IDIconoSoltado = IconosPulsables[i]->IDComando;
			IconosPulsables[i]->Estado = 1;
			CP->Evento_ObjetoPulsado(Boton, IDIconoSoltado);
		}
		else {
			IconosPulsables[i]->Estado = 0;
		}
	}
	Pintar_IconosPulsables(BarraScrollEx_ValorV);


	// Palabras pulsables
	// Determino la ID soltada
	for (i = 0; i < PalabrasPulsables.size(); i++) {
		if (PtInRect(&PalabrasPulsables[i]->Espacio, Punto) == TRUE) {
			if (PalabrasPulsables[i]->Estado == 2) IDSoltada = PalabrasPulsables[i]->Palabra->ID;
			PalabrasPulsables[i]->Estado = 1;
		}
		else {
			PalabrasPulsables[i]->Estado = 0;
		}
	}
	if (IDSoltada != -1) {
		for (i = 0; i < PalabrasPulsables.size(); i++) {
			if (PalabrasPulsables[i]->Palabra->ID == IDSoltada) PalabrasPulsables[i]->Estado = 1;
		}
		Pintar_PalabrasPulsables(BarraScrollEx_ValorV);
		CP->Evento_ObjetoPulsado(Boton, IDSoltada);
	}
	else {
		Pintar_PalabrasPulsables(BarraScrollEx_ValorV);
	}
}

void Escena::Pintar_IconosPulsables(const UINT BarraScrollEx_ValorV) {
	HDC hDC = GetDC(CP->hWnd());
	HDC     hDCDest; 
	HBITMAP BmpDest; 
	HBITMAP BmpViejo;
//	HFONT   FViejo;
	HBRUSH  Brocha;
	RECT    RC;
	UINT    Presionado = 0;
	for (size_t i = 0; i < IconosPulsables.size(); i++) {
		if (IconosPulsables[i]->Estado != IconosPulsables[i]->UEstado) {
			IconosPulsables[i]->UEstado = IconosPulsables[i]->Estado;
			RC.left = 0;
			RC.top = 0;
			RC.right = (IconosPulsables[i]->Espacio.right - IconosPulsables[i]->Espacio.left) + 2;
			RC.bottom = (IconosPulsables[i]->Espacio.bottom - IconosPulsables[i]->Espacio.top) + 2;
			hDCDest  = CreateCompatibleDC(hDC);
			BmpDest  = CreateCompatibleBitmap(hDC, RC.right, RC.bottom);
			BmpViejo = static_cast<HBITMAP>(SelectObject(hDCDest, BmpDest));
			Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
			FillRect(hDCDest, &RC, Brocha);
			DeleteObject(Brocha);

			switch (IconosPulsables[i]->Estado) {
				case 0 : // normal
					DWL::SO::DWLListaIconos::PintarIcono(hDCDest, 1, 1, RC.right -2, RC.bottom -2, IconosPulsables[i]->IDIcono);
					break;
				case 1 : // resaltado
					DWL::SO::DWLListaIconos::PintarIconoResaltado(hDCDest, 1, 1, RC.right -2, RC.bottom -2, IconosPulsables[i]->IDIcono, RGB(255,255,0));
//					Brocha = CreateSolidBrush(RGB(255,255, 0));
					break;
				case 2 : // presionado
					DWL::SO::DWLListaIconos::PintarIconoResaltado(hDCDest, 2, 2, RC.right -2, RC.bottom -2, IconosPulsables[i]->IDIcono, RGB(192,192,192));
//					Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Borde_Resaltado);
					Presionado = 1;
					break;
			}

//			DWL::SO::DWLListaIconos::PintarIcono(hDCDest, Presionado, Presionado, RC.right -1, RC.bottom -1, IconosPulsables[i]->IDIcono);

			BitBlt(hDC, IconosPulsables[i]->Espacio.left - 1, (IconosPulsables[i]->Espacio.top - 1) - BarraScrollEx_ValorV, RC.right, RC.bottom, hDCDest, 0, 0, SRCCOPY);

			SelectObject(hDCDest, BmpViejo);
			DeleteObject(BmpDest);
			DeleteDC(hDCDest);
		}
	}

	ReleaseDC(CP->hWnd(), hDC);

}

void Escena::Pintar_PalabrasPulsables(const UINT BarraScrollEx_ValorV) {
	HDC hDC = GetDC(CP->hWnd());
	HDC     hDCDest; 
	HBITMAP BmpDest; 
	HBITMAP BmpViejo;
	HFONT   FViejo;
	HBRUSH  Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
	RECT    RC;

	for (size_t i = 0; i < PalabrasPulsables.size(); i++) {
		if (PalabrasPulsables[i]->Estado != PalabrasPulsables[i]->UEstado) {
			PalabrasPulsables[i]->UEstado = PalabrasPulsables[i]->Estado;

			RC.left = 0;
			RC.top = 0;
			RC.right = (PalabrasPulsables[i]->Espacio.right - PalabrasPulsables[i]->Espacio.left) + 1;
			RC.bottom = (PalabrasPulsables[i]->Espacio.bottom - PalabrasPulsables[i]->Espacio.top) + 1;
			hDCDest  = CreateCompatibleDC(hDC);
			BmpDest  = CreateCompatibleBitmap(hDC, RC.right, RC.bottom);
			BmpViejo = static_cast<HBITMAP>(SelectObject(hDCDest, BmpDest));
			FViejo   = static_cast<HFONT>(SelectObject(hDCDest, DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita()));
			FillRect(hDCDest, &RC, Brocha);
			SetBkMode(hDCDest, TRANSPARENT);

			switch (PalabrasPulsables[i]->Estado) {
				case 0 : // normal
					SetTextColor(hDCDest, DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Seleccionado);
					TextOut(hDCDest, 0, 0, PalabrasPulsables[i]->Palabra->Palabra(), PalabrasPulsables[i]->Palabra->Palabra.Tam());
					break;
				case 1 : // resaltado
					SetTextColor(hDCDest, DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_SubSeleccionadoResaltado);
					TextOut(hDCDest, 0, 0, PalabrasPulsables[i]->Palabra->Palabra(), PalabrasPulsables[i]->Palabra->Palabra.Tam());
					break;
				case 2 : // presionado
					SetTextColor(hDCDest, DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Presionado);
					TextOut(hDCDest, 1, 1, PalabrasPulsables[i]->Palabra->Palabra(), PalabrasPulsables[i]->Palabra->Palabra.Tam());
					break;
			}

			BitBlt(hDC, PalabrasPulsables[i]->Espacio.left, PalabrasPulsables[i]->Espacio.top - BarraScrollEx_ValorV, RC.right, RC.bottom, hDCDest, 0, 0, SRCCOPY);

			SelectObject(hDCDest, FViejo);
			SelectObject(hDCDest, BmpViejo);
			DeleteObject(BmpDest);
			DeleteDC(hDCDest);
		}
	}
	DeleteObject(Brocha);
	ReleaseDC(CP->hWnd(), hDC);
}
