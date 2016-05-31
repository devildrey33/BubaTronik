#include "StdAfx.h"
#include "ControlTeclaRapida.h"
#include "BubaTronikApp.h"


#define ANCHO_BOTON_EDITAR    60
#define ANCHO_TEXTO_CONTROL  150
#define ID_BOTON_EDITAR     1021

ControlTeclaRapida::ControlTeclaRapida(void) {
}

ControlTeclaRapida::~ControlTeclaRapida(void) {
}

LRESULT ControlTeclaRapida::Evento_BotonEx_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
	RECT Rect;
	GetClientRect(_hWnd, &Rect);
	Rect.bottom -= ANCHO_BOTON_EDITAR;
	Sistema.App.EditandoTeclasRapidas = true;
	CaptandoTecla = true;
	SetFocus(_hWnd);
	TextoResultado = IDIOMA__PRESIONA_LA_NUEVA_TECLA_PARA_ESTA_ACCION;

	HDC hDC = GetDC(_hWnd);
    PAINTSTRUCT PS;
	Evento_Pintar(hDC, PS);
	ReleaseDC(_hWnd, hDC);
    return 0;
}


void ControlTeclaRapida::Pintar(HDC hDCDest, RECT *Espacio) {
	RECT Rect;
	GetClientRect(_hWnd, &Rect);

//	RECT TC = { 0, 0, ANCHO_TEXTO_CONTROL, Rect.bottom };
	RECT TR = { ANCHO_TEXTO_CONTROL, 0, Rect.right - ANCHO_BOTON_EDITAR, Rect.bottom };
	DWL::GDI::DWLhDC Buffer(hDCDest, Rect.right, Rect.bottom);
	
	HBRUSH Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
	Buffer.PintarRecta(&Rect, Brocha);
	DeleteObject(Brocha);

//	Rect.right -= ANCHO_BOTON_EDITAR;


	Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Claro);
	Buffer.PintarRecta(&TR, Brocha);
	DeleteObject(Brocha);
	HPEN Pluma = CreatePen(PS_SOLID, 1, DWL::SO::DWLEstilos::Colores.ContenedorEx_Borde_Normal);
	HPEN VPluma = static_cast<HPEN>(SelectObject(Buffer(), Pluma));
	MoveToEx(Buffer(), TR.left + 1, 0, NULL);
	LineTo(Buffer(), TR.right + 1, 0);
	MoveToEx(Buffer(), TR.left, 1, NULL);
	LineTo(Buffer(), TR.left, TR.bottom - 1);
	MoveToEx(Buffer(), TR.left + 1, TR.bottom - 1, NULL);
	LineTo(Buffer(), TR.right + 1, TR.bottom -1);
	SelectObject(Buffer(), VPluma);
	DeleteObject(Pluma);

	Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
	Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Normal);
	Buffer.ModoBk(TRANSPARENT);
	TR.top += 3;
	Buffer.PintarTextoCentrado(TextoResultado(), &TR);

	Buffer.PintarTexto(TextoControl(), 5, 3);
//	DWL::GDI::DWLhDC BufferBoton(hDCDest, ANCHO_BOTON_EDITAR, Rect.bottom);
//	PAINTSTRUCT PS;
//    BotonEditar.Evento_Pintar(BufferBoton(), PS);
//	BufferBoton.PintarMapaBits(Buffer(), Rect.right, 0, ANCHO_BOTON_EDITAR, Rect.bottom, 0, 0);

	RECT RectaBoton = { Rect.right - ANCHO_BOTON_EDITAR, 0, Rect.right, Rect.bottom };
	BotonEditar.BotonEx_Pintar(Buffer(), &RectaBoton, false, true);

	Buffer.PintarMapaBits(hDCDest, Espacio->left, Espacio->top, Espacio->right, Espacio->bottom, 0, 0);

}

LRESULT ControlTeclaRapida::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	
	
	RECT Rect;
	GetClientRect(_hWnd, &Rect);

	Pintar(hDC, &Rect);
    return 0;
}


void ControlTeclaRapida::Crear(HWND hWndParent, const TCHAR *nTexto, const int cX, const int cY, const int cAncho, const int cAlto, const UINT cID, Tecla *nTecla) {
	CaptandoTecla = false;
	TeclaRapida = nTecla;
	CrearVentana(hWndParent, TEXT("BubaTronik_ControlTeclaRapida"), WS_CHILD, TEXT(""), cX, cY, cAncho, cAlto, (HMENU)cID);
	BotonEditar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__EDITAR, (cAncho - ANCHO_BOTON_EDITAR) + 2, 0, ANCHO_BOTON_EDITAR, cAlto, ID_BOTON_EDITAR);
	DWL::DWLString Tmp;
	if (nTecla->Control == true) 	Tmp += IDIOMA__CONTROL_MAS; // TEXT("Control + ");
	if (nTecla->Alt == true)		Tmp += IDIOMA__ALT_MAS;     // TEXT("Alt + ");
	if (nTecla->Shift == true)		Tmp += IDIOMA__SHIFT_MAS;   // TEXT("Shift + ");
	Tmp += TEXT("'");
	Tmp += TextoTecla(nTecla->TeclaVirtual);
	Tmp += TEXT("'");
	//Tmp += TEXT("Tecla '");
	TextoResultado = Tmp;
	TextoControl = nTexto;
	Visible(true);
}


void ControlTeclaRapida::Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool Repintar) {
	BotonEditar.Mover((cAncho - ANCHO_BOTON_EDITAR), 0, ANCHO_BOTON_EDITAR, cAlto, false);
	DWL::Ventanas::DWLVentana::Mover(cX, cY, cAncho, cAlto, Repintar);
}


LRESULT ControlTeclaRapida::Evento_Teclado_TeclaSoltada(const UINT Tecla, const UINT Repeticion, const UINT Param) {
	if (CaptandoTecla == true) {
        PAINTSTRUCT PS;
		CaptandoTecla = false;
		Sistema.App.EditandoTeclasRapidas = false;
		if (Tecla == 18) {
			TextoResultado = IDIOMA__COMBINACION_DE_TECLAS_INVALIDA;
			HDC hDC = GetDC(_hWnd);
			Evento_Pintar(hDC, PS);
			ReleaseDC(_hWnd, hDC);
			return 0;
		}
		DWL::DWLString Tmp;
		bool ControlPresionado	= false;
		bool AltPresionado		= false;
		bool ShiftPresionado	= false;
		BYTE EstadoTeclado[256];
		BOOL BKBST = GetKeyboardState(EstadoTeclado);
		if (TECLA_PRESIONADA(EstadoTeclado, VK_CONTROL))	ControlPresionado	= true;
		if (TECLA_PRESIONADA(EstadoTeclado, VK_MENU))		AltPresionado		= true;
		if (TECLA_PRESIONADA(EstadoTeclado, VK_SHIFT))		ShiftPresionado		= true;
		TeclaRapida->AsignarTecla(Tecla, ControlPresionado, AltPresionado, ShiftPresionado);
		if (ControlPresionado == true)	Tmp += IDIOMA__CONTROL_MAS; // TEXT("Control + ");
		if (AltPresionado == true)		Tmp += IDIOMA__ALT_MAS;		// TEXT("Alt + ");
		if (ShiftPresionado == true)	Tmp += IDIOMA__SHIFT_MAS;	// TEXT("Shift + ");
		Tmp += TEXT("'");
		Tmp += TextoTecla(Tecla);
		Tmp += TEXT("'");
		TextoResultado = Tmp;
		HDC hDC = GetDC(_hWnd);
		Evento_Pintar(hDC, PS);
		ReleaseDC(_hWnd, hDC);
	}
    return 0;
}


const TCHAR *ControlTeclaRapida::TextoTecla(const int TeclaVirtual) {
	static TCHAR T[2];
	T[0] = 0;
	T[1] = 0;
	switch (TeclaVirtual) {
		case VK_UP		: return IDIOMA__CURSOR_ADELANTE;
		case VK_DOWN	: return IDIOMA__CURSOR_ATRAS;
		case VK_LEFT	: return IDIOMA__CURSOR_IZQUIERDA;
		case VK_RIGHT	: return IDIOMA__CURSOR_DERECHA;
		case VK_SPACE	: return IDIOMA__ESPACIO;
		case VK_BACK	: return IDIOMA__RETROCESO;
		case VK_F1		: return TEXT("F1");
		case VK_F2		: return TEXT("F2");
		case VK_F3		: return TEXT("F3");
		case VK_F4		: return TEXT("F4");
		case VK_F5		: return TEXT("F5");
		case VK_F6		: return TEXT("F6");
		case VK_F7		: return TEXT("F7");
		case VK_F8		: return TEXT("F8");
		case VK_F9		: return TEXT("F9");
		case VK_F10		: return TEXT("F10");
		case VK_F11		: return TEXT("F11");
		case VK_F12		: return TEXT("F12");
		case VK_F13		: return TEXT("F13");
		case VK_F14		: return TEXT("F14");
		case VK_F15		: return TEXT("F15");
		case VK_F16		: return TEXT("F16");
		case VK_F17		: return TEXT("F17");
		case VK_F18		: return TEXT("F18");
		case VK_F19		: return TEXT("F19");
		case VK_F20		: return TEXT("F20");
		case VK_F21		: return TEXT("F21");
		case VK_F22		: return TEXT("F22");
		case VK_F23		: return TEXT("F23");
		case VK_F24		: return TEXT("F24");
		case VK_INSERT	: return IDIOMA__INSERT;
		case VK_DELETE	: return IDIOMA__SUPRIMIR;
		case VK_HOME	: return IDIOMA__INICIO;
		case VK_END		: return IDIOMA__FIN;
		case VK_ESCAPE	: return IDIOMA__ESC;
		case VK_SHIFT	: return IDIOMA__SHIFT;
		case VK_PRINT	: return IDIOMA__IMPRIMIR_PANTALLA;
/*		case VK_PLAY	: return TEXT("Play");
		case VK_PAUSE	: return TEXT("Play");
		case VK_STOP	: return TEXT("Play");
		case VK_PLAY	: return TEXT("Play");*/
		case 107		: return TEXT("+");
		case 109		: return TEXT("-");
		case 106		: return TEXT("*");
		case 111		: return TEXT("/");
		case 219		: return TEXT("'");
		case 221		: return TEXT("¡");
		default :
			T[0] = static_cast<TCHAR>(TeclaVirtual);
			return T;
	}
}