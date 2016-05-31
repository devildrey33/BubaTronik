#include "StdAfx.h"
#include "VentanaTranslucida.h"


void VentanaTranslucida::Crear(const int nTipoVentana, const int nOrdenZ, const int Pos) {
	Espacio.left	= 10 + (Pos * 10); 
	Espacio.top		= 10 + (Pos * 10);
	Espacio.right	= 200 + (Pos * 10);
	Espacio.bottom	= 200 + (Pos * 10);
	TipoVentana		= nTipoVentana;
	OrdenZ			= nOrdenZ;
}

void VentanaTranslucida::Pintar(HDC hDCDestino) {
	RECT	RC		= { 0, 0, Espacio.right - Espacio.left, Espacio.bottom - Espacio.top };
	HDC		BufferFondo		= CreateCompatibleDC(hDCDestino);
	HBITMAP BmpFondo		= CreateCompatibleBitmap(hDCDestino, RC.right, RC.bottom);
	HBITMAP ViejoFondo		= static_cast<HBITMAP>(SelectObject(BufferFondo, BmpFondo));

	HDC		BufferVentana	= CreateCompatibleDC(hDCDestino);
	HBITMAP BmpVentana		= CreateCompatibleBitmap(hDCDestino, RC.right, RC.bottom);
	HBITMAP ViejoVentana	= static_cast<HBITMAP>(SelectObject(BufferVentana, BmpVentana));

	HRGN	Region			= CreateRoundRectRgn(0, 0, RC.right, RC.bottom, 12, 12);
	
	
	// Pinto el fondo del hDCDestino en el buffer
	BitBlt(BufferFondo, 0, 0, RC.right, RC.bottom, hDCDestino, Espacio.left, Espacio.top, SRCCOPY);

	// Pinto el recuadro azul con su borde
	FillRect(BufferVentana, &RC, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
	HBRUSH Brocha = CreateSolidBrush(RGB(0, 0, 100));
	FillRgn(BufferVentana, Region, Brocha);
	DeleteObject(Brocha);
	if (OrdenZ == 0)	Brocha = CreateSolidBrush(RGB(0, 0, 250));
	else				Brocha = CreateSolidBrush(RGB(0, 0, 230));
	FrameRgn(BufferVentana, Region, Brocha, 2, 2);
	DeleteObject(Brocha);
	Brocha = CreateSolidBrush(RGB(0, 0, 200));
	FrameRgn(BufferVentana, Region, Brocha, 1, 1);
	DeleteObject(Brocha);
	
	RECT RCContenido = { RC.left + 3, RC.top + 15, RC.right - 6, RC.bottom - 6 };
	// PINTAR DETALLES DE LA VENTANA EN BUFFER VENTANA
	PintarContenido(BufferVentana, &RCContenido);
	//////////////////////////////////////////////////

	BLENDFUNCTION BF;
	BF.AlphaFormat = 0;
	BF.BlendOp = NULL;
	BF.BlendFlags = NULL;
	BF.SourceConstantAlpha = 200;
	AlphaBlend(BufferFondo, 0, 0, RC.right, RC.bottom, BufferVentana, 0, 0, RC.right, RC.bottom, BF);

	// Pinto la mascara para la ventana en el DC final
	FillRect(BufferVentana, &RC, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
	FillRgn(BufferVentana, Region, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
	BitBlt(hDCDestino, Espacio.left, Espacio.top, Espacio.right - Espacio.left, Espacio.bottom - Espacio.top, BufferVentana, 0, 0, SRCAND);
	BitBlt(hDCDestino, Espacio.left, Espacio.top, Espacio.right - Espacio.left, Espacio.bottom - Espacio.top, BufferFondo, 0, 0, SRCPAINT);

	
	DeleteObject(Region);
	SelectObject(BufferFondo, ViejoFondo);
	DeleteObject(BmpFondo);
	DeleteDC(BufferFondo);
	SelectObject(BufferVentana, ViejoVentana);
	DeleteObject(BmpVentana);
	DeleteDC(BufferVentana);
}