#include "stdafx.h"
#include "ArbolOpcionesBasicas.h"
#include "resource.h"
#include <DWLGraficos.h>
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);

ArbolOpcionesBasicas::ArbolOpcionesBasicas(void) {
}


ArbolOpcionesBasicas::~ArbolOpcionesBasicas(void) {
}

void ArbolOpcionesBasicas::RecargarIdioma(void) {
	EliminarTodosLosNodos();
	DWL::ControlesEx::DWLArbolEx_Nodo *Nodo = NULL;
	DWL::ControlesEx::DWLArbolEx_Nodo *Nodo2 = NULL;
	AgregarNodo(NULL, NULL, IDI_INICIO2, IDIOMA__INICIO, 0);					// 0
	AgregarNodo(NULL, NULL, IDI_GUARDAR, IDIOMA__ABRIR_ARCHIVO, 1);				// 1
	Nodo = AgregarNodo(NULL, NULL, IDI_BDV, IDIOMA__BASE_DE_DATOS, 2);		// 3
		AgregarNodo(NULL, Nodo, IDI_BD_BUSCAR, IDIOMA__BUSCAR_ARCHIVOS, 1);			// 9
		AgregarNodo(NULL, Nodo, IDI_BD_REVISAR, IDIOMA__REVISAR_MEDIOS, 2);				// 9
//		AgregarNodo(NULL, Nodo, IDI_GENERO, IDIOMA__REVISAR_GENEROS, 3);				//
//		AgregarNodo(NULL, Nodo, IDI_GRUPO, IDIOMA__REVISAR_GRUPOS, 4);				//
//		AgregarNodo(NULL, Nodo, IDI_DISCO, IDIOMA__REVISAR_DISCOS, 5);				//
	
	AgregarNodo(NULL, NULL, IDI_LISTA, IDIOMA__LISTA_DE_REPRODUCCION, 3);		// 8
	AgregarNodo(NULL, NULL, IDI_EQ, IDIOMA__ECUALIZADOR, 4);					// 4
	Nodo = AgregarNodo(NULL, NULL, IDI_OPCIONES, IDIOMA__OPCIONES, 5);			// 10
		AgregarNodo(NULL, Nodo, IDI_LISTA_LLENA, IDIOMA__ASPECTO);						// 10
		AgregarNodo(NULL, Nodo, IDI_BDV, IDIOMA__BASE_DE_DATOS);					// 10
		AgregarNodo(NULL, Nodo, IDI_LISTA_MODIFICARPISTA, IDIOMA__TECLAS_RAPIDAS);						// 10
		AgregarNodo(NULL, Nodo, IDI_LISTA, IDIOMA__LISTA);					// 10
		AgregarNodo(NULL, Nodo, IDI_OPCIONESV, IDIOMA__VIDEO);								// 10
	
	AgregarNodo(NULL, NULL, IDI_PANTALLA, IDIOMA__VER_VIDEO, 6);				// 7
	AgregarNodo(NULL, NULL, IDI_AYUDA, IDIOMA__SOBRE_BUBATRONIK, 7);						// 13
	#ifdef MOSTRAR_LOG
		AgregarNodo(NULL, NULL, IDI_AYUDA, TEXT("Mostrar log"), 8);						// 13
	#endif
	ActualizarTodo(true, true);
}



HWND ArbolOpcionesBasicas::Crear(HWND hWndParent, const int cID, const int cX, const int cY, const int cAncho, const int cAlto) {
	DWL::ControlesEx::DWLArbolEx_Nodo *Nodo = NULL;
	CrearArbolEx(WS_CHILD, hWndParent, cX, cY, cAncho, cAlto, cID, false);
	RecargarIdioma();
/*	AgregarNodo(NULL, NULL, IDI_INICIO2, IDIOMA__INICIO, 0);					// 0
	AgregarNodo(NULL, NULL, IDI_GUARDAR, IDIOMA__ABRIR_ARCHIVO, 1);				// 1
	Nodo = AgregarNodo(NULL, NULL, IDI_AUDIO, IDIOMA__BASE_DE_DATOS, 2);		// 3
		AgregarNodo(NULL, Nodo, IDI_BD_BUSCAR, IDIOMA__BUSCAR_ARCHIVOS, 1);			// 9
		AgregarNodo(NULL, Nodo, IDI_BD_REVISAR, IDIOMA__REVISAR, 2);				// 9
	
	AgregarNodo(NULL, NULL, IDI_LISTA, IDIOMA__LISTA_DE_REPRODUCCION, 3);		// 8
	AgregarNodo(NULL, NULL, IDI_EQ, IDIOMA__ECUALIZADOR, 4);					// 4
	Nodo = AgregarNodo(NULL, NULL, IDI_OPCIONES, IDIOMA__OPCIONES, 5);			// 10
		AgregarNodo(NULL, Nodo, IDI_LISTA_LLENA, IDIOMA__ASPECTO);						// 10
		AgregarNodo(NULL, Nodo, IDI_BDV, IDIOMA__BASE_DE_DATOS);					// 10
		AgregarNodo(NULL, Nodo, IDI_LISTA_MODIFICARPISTA, IDIOMA__TECLAS_RAPIDAS);						// 10
		AgregarNodo(NULL, Nodo, IDI_LISTA, IDIOMA__LISTA);					// 10
		AgregarNodo(NULL, Nodo, IDI_OPCIONESV, IDIOMA__VIDEO);								// 10
	
	AgregarNodo(NULL, NULL, IDI_PANTALLA, IDIOMA__VER_VIDEO, 6);				// 7
	AgregarNodo(NULL, NULL, IDI_AYUDA, IDIOMA__AYUDA, 7);						// 13*/
	
//	Nodo = AgregarNodo(NULL, NULL, IDI_BDA, TEXT("Audio"), 2);					// 2
//	Nodo = AgregarNodo(NULL, NULL, IDI_BDV, TEXT("Video"), 3);					// 5
//	 AgregarNodo(NULL, Nodo, IDI_VIDEO, IDIOMA__BASE_DE_DATOS);					// 6
//	 AgregarNodo(NULL, Nodo, IDI_DISCO, TEXT("Audio"));							// 11
//	 AgregarNodo(NULL, Nodo, IDI_OPCIONESV, TEXT("Video"));						// 12
//	Nodo = AgregarNodo(NULL, NULL, IDI_AYUDA, TEXT("Acerca de.."), 8);			// 14

	ArbolEx_Estilos.MultiSeleccion = false;
	ArbolEx_Estilos.SubSeleccion = false;
	ActualizarTodo(true, true);
	Visible(true);
	return _hWnd;
}

const int ArbolOpcionesBasicas::CalcularAncho(HWND hWnd) {
	int TamFinal = 0;
	HDC Buffer = GetDC(hWnd);
	
	HFONT Viejo = static_cast<HFONT>(SelectObject(Buffer, DWL::SO::DWLEstilos::Fuentes.Tahoma13()));
	SIZE Tam;
	GetTextExtentPoint(Buffer, IDIOMA__INICIO, DWLStrLen(IDIOMA__INICIO), &Tam);
	TamFinal = Tam.cx + 40;
	GetTextExtentPoint(Buffer, IDIOMA__ABRIR_ARCHIVO, DWLStrLen(IDIOMA__ABRIR_ARCHIVO), &Tam);
	if (TamFinal < Tam.cx + 40) TamFinal = Tam.cx + 40;
	GetTextExtentPoint(Buffer, IDIOMA__BASE_DE_DATOS, DWLStrLen(IDIOMA__BASE_DE_DATOS), &Tam);
	if (TamFinal < Tam.cx + 40) TamFinal = Tam.cx + 40;
	GetTextExtentPoint(Buffer, IDIOMA__BUSCAR_ARCHIVOS, DWLStrLen(IDIOMA__BUSCAR_ARCHIVOS), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__REVISAR_MEDIOS, DWLStrLen(IDIOMA__REVISAR_MEDIOS), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
/*	GetTextExtentPoint(Buffer, IDIOMA__REVISAR_GENEROS, DWLStrLen(IDIOMA__REVISAR_GENEROS), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__REVISAR_GRUPOS, DWLStrLen(IDIOMA__REVISAR_GRUPOS), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__REVISAR_DISCOS, DWLStrLen(IDIOMA__REVISAR_DISCOS), &Tam);*/
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__LISTA_DE_REPRODUCCION, DWLStrLen(IDIOMA__LISTA_DE_REPRODUCCION), &Tam);
	if (TamFinal < Tam.cx + 40) TamFinal = Tam.cx + 40;
	GetTextExtentPoint(Buffer, IDIOMA__ECUALIZADOR, DWLStrLen(IDIOMA__ECUALIZADOR), &Tam);
	if (TamFinal < Tam.cx + 40) TamFinal = Tam.cx + 40;
	GetTextExtentPoint(Buffer, IDIOMA__OPCIONES, DWLStrLen(IDIOMA__OPCIONES), &Tam);
	if (TamFinal < Tam.cx + 40) TamFinal = Tam.cx + 40;
	GetTextExtentPoint(Buffer, IDIOMA__ASPECTO, DWLStrLen(IDIOMA__ASPECTO), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__BASE_DE_DATOS, DWLStrLen(IDIOMA__BASE_DE_DATOS), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__TECLAS_RAPIDAS, DWLStrLen(IDIOMA__TECLAS_RAPIDAS), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__LISTA, DWLStrLen(IDIOMA__LISTA), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__VIDEO, DWLStrLen(IDIOMA__VIDEO), &Tam);
	if (TamFinal < Tam.cx + 58) TamFinal = Tam.cx + 58;
	GetTextExtentPoint(Buffer, IDIOMA__VER_VIDEO, DWLStrLen(IDIOMA__VER_VIDEO), &Tam);
	if (TamFinal < Tam.cx + 40) TamFinal = Tam.cx + 40;
	GetTextExtentPoint(Buffer, IDIOMA__SOBRE_BUBATRONIK, DWLStrLen(IDIOMA__SOBRE_BUBATRONIK), &Tam);
	if (TamFinal < Tam.cx + 40) TamFinal = Tam.cx + 40;

	SelectObject(Buffer, Viejo);
	ReleaseDC(_hWnd, Buffer);
	return TamFinal;
}

/*
void ArbolOpcionesBasicas::Evento_Pintar_Fondo(HDC hDC, RECT *Espacio) {
	HBRUSH Brocha = CreateSolidBrush(_Colores->FondoNormal);
	FillRect(hDC, Espacio, Brocha);
	DWL::GDI::DWLIcono Fondo(IDI_MP3, 96, 96);
	Fondo.PintarIcono(hDC, (Espacio->right -96) / 2, (Espacio->bottom -96) / 2, 96, 96);
	DeleteObject(Brocha);
}
*/