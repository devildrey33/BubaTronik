#include "StdAfx.h"
#include "VentanaDump.h"
#include <dbghelp.h>
#include <DWLString.h>
#include <DWLGraficos.h>
#include "DimensionesFijas.h"
#include "BubaTronikApp.h"
#include "VentanaMensaje.h"
#include "EnviarEmail.h"
#include "resource.h"

VentanaDump::VentanaDump(void) {
}


VentanaDump::~VentanaDump(void) {
}

#define ID_BARRA_DUMP		10001


const BOOL VentanaDump::Mostrar(const TCHAR *PathDump) { 
	
	
	
	_PathDump = PathDump;
	UINT YC = 6;
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		CrearVentana(0, TEXT("BubaTronik_VentanaDump"), WS_OVERLAPPED | WS_SYSMENU, IDIOMA__ERROR_GRAVE, DWL_CENTRADO, DWL_CENTRADO, 547, 150, 0);
		YC = 80;
	}
	else {
		CrearVentana(0, TEXT("BubaTronik_VentanaDump"), WS_POPUP, IDIOMA__ERROR_GRAVE, DWL_CENTRADO, DWL_CENTRADO, 543, 145, 0);
		HRGN Region = CreateRoundRectRgn(0, 0, 544, 146, 11, 11);
		SetWindowRgn(_hWnd, Region, false);
		YC = 100;
	}
	if (_hWnd == NULL) return FALSE;
	_Barra.CrearBarraEx(WS_CHILD | WS_VISIBLE, _hWnd, 10, YC, 522, 8, ID_BARRA_DUMP, DWL_BARRAEX_PROGRESO_HORIZONTAL, 0, 1000);
	_BotonGuardar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__ENVIAR, 196, YC + 13, 70, 20, 101);
	_BotonTerminar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__TERMINAR, 276, YC + 13, 70, 20, 100);

	Visible(true);
	_BotonGuardar.AsignarFoco();
	return TRUE;
};


LRESULT VentanaDump::Evento_Cerrar(void) {
	PostQuitMessage(0); // salimos sin tocar nada mas... (que se ocupe windows de borrar la memoria)
//	exit(0);
	return 0;
};


LRESULT VentanaDump::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	RECT RC;
	GetClientRect(_hWnd, &RC);
	DWL::GDI::DWLhDC Buffer(hDC, RC.right, RC.bottom);

	// Creo la fuente para mostrar el error
	HFONT FuenteTahoma = CreateFont(15, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, TEXT("Tahoma")); 
	HFONT FuenteTahomaT = CreateFont(17, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, TEXT("Tahoma")); 
	HFONT FuenteVieja = static_cast<HFONT>(SelectObject(Buffer(), FuenteTahomaT));


	// Pinto el fondo con un degradado
	TRIVERTEX     GCVertex[2];
	GRADIENT_RECT tGRect;
    COLORREF      Col1 = DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior;
    COLORREF      Col2 = DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior;
    
	GCVertex[0].Red		= DWL_RGB_OBTENER_R(Col1);
	GCVertex[0].Green	= DWL_RGB_OBTENER_G(Col1);
	GCVertex[0].Blue	= DWL_RGB_OBTENER_B(Col1);
	GCVertex[0].x = 0;
	GCVertex[0].y = 0;
	GCVertex[1].Red		= DWL_RGB_OBTENER_R(Col2);
	GCVertex[1].Green	= DWL_RGB_OBTENER_G(Col2);
	GCVertex[1].Blue	= DWL_RGB_OBTENER_B(Col2);
	GCVertex[1].x = RC.right;
	GCVertex[1].y = RC.bottom;
	tGRect.UpperLeft = 0;  tGRect.LowerRight = 1;
	GradientFill(Buffer(), GCVertex, 2, &tGRect, 1, DWLGRADIENT_FILL_RECT_V);
	UINT YT = 6;

	if (Sistema.App.Config.UtilizarBarraTituloWindows == false) { 
		Buffer.PintarIcono(5, 5, 16, 16, IDI_DAP);
		_PintarTextoSombra(Buffer(), IDIOMA__ERROR_GRAVE, RGB(255,255,255), RGB(30, 30, 30), 25, 5);
		YT = 26;
		// Pinto el borde
		DWL::GDI::DWLBrocha		Brocha(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro);
		DWL::GDI::DWLRegion		Region(_hWnd);
		Buffer.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDE, VENTANAEX_BORDE);
		Brocha.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
		Buffer.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDECLARO, VENTANAEX_BORDECLARO);
	}
	else {
		YT = 6;
	}
	Buffer.PintarIcono(5, YT + 5, 48, 48, IDI_ERRORFATAL);
	SelectObject(Buffer(), FuenteTahoma);
	_PintarTextoSombra(Buffer(), IDIOMA__HA_SUCEDIDO_UN_ERROR_GRAVE_EN_BUBATRONIK_Y_ESTE_DEBE_CERRARSE, RGB(255,255,255), RGB(30, 30, 30), 60, YT);
	_PintarTextoSombra(Buffer(), IDIOMA__SE_HA_CREADO_UN_INFORME_DE_ERRORES_DESEAS_ENVIARLO_A_WWW_DEVILDREY33_ES, RGB(255,255,255), RGB(30, 30, 30), 60, YT + 16);
	_PintarTextoSombra(Buffer(), IDIOMA__PULSA_EL_BOTON_ENVIAR_PARA_ENVIAR_EL_MENSAJE_Y_TERMINAR_O_PULSA_EL_BOTON, RGB(255,255,255), RGB(30, 30, 30), 60, YT + 32);
	_PintarTextoSombra(Buffer(), IDIOMA__TERMINAR_PARA_QUE_NO_SE_MANDE_EL_MENSAJE, RGB(255,255,255), RGB(30, 30, 30), 60, YT + 48);
	// Pinto el texto
//	DWL::DWLString Descripcion = TEXT("Ha ocurrido un error grave en la aplicación, y esta debe cerrarse.\nSi lo deseas puedes crear un archivo con información detallada sobre el error.\nEste archivo no contiene ningún dato confidencial, y puede ser de gran ayuda para encontrar y\nreparar errores infrecuentes en las aplicaciones.\nNOTA : en windows vista y superiores es recomendable guardar el archivo en la ruta inicial.\nPresiona Guardar para guardar el archivo de log y salir, o Terminar para salir.");

	// Elimino objetos de la memoria
	SelectObject(Buffer(), FuenteVieja);
	DeleteObject(FuenteTahoma);
	DeleteObject(FuenteTahomaT);

	BitBlt(hDC, 0, 0, RC.right, RC.bottom, Buffer(), 0, 0, SRCCOPY); 
    return 0;
};


LRESULT VentanaDump::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
	static VentanaMensaje_ErrorCritico Msg;
	if (Boton == 0 && IDBotonEx == 100)	{
		Visible(false);
		PostQuitMessage(0); // salimos sin tocar nada mas... (que se ocupe windows de borrar la memoria)
	}

	if (Boton == 0 && IDBotonEx == 101) {
		EnviarEmail Mail;
		if (Mail.Enviar(_PathDump(), &_Barra) == true)	{
			_BotonGuardar.Activado(false);
			Msg.MostrarMensaje(IDIOMA__INFORME_DE_ERRORES_ENVIADO_CORRECTAMENTE, IDIOMA__INFORMACION, 0);
			DeleteFile(_PathDump());
		}
		else {
			Msg.MostrarMensaje(IDIOMA__ERROR_ENVIANDO_EL_INFORME_DE_ERRORES, IDIOMA__INFORMACION, 0);
		}
	}
    return 0;
};



// Función para pintar texto con sombra
void VentanaDump::_PintarTextoSombra(HDC hDC, const TCHAR *Texto, COLORREF ColTexto, COLORREF ColSombra, UINT cX, UINT cY) {
    UINT TamTexto = DWLStrLen(Texto);
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, ColSombra);
	TextOut(hDC, cX, cY, Texto, TamTexto);
	cX --; cY --;
	SetTextColor(hDC, ColTexto);
	TextOut(hDC, cX, cY, Texto, TamTexto);
}