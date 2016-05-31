#include "StdAfx.h"
#include "VentanaMensaje_Actualizacion.h"
#include "BubaTronikApp.h"
#include <shellapi.h>


VentanaMensaje_Actualizacion::VentanaMensaje_Actualizacion(void) {
}

VentanaMensaje_Actualizacion::~VentanaMensaje_Actualizacion(void) {
}


// Función para mostrar el mensaje Aceptar
void VentanaMensaje_Actualizacion::Mostrar(void) {
    DWL::DWLString TextoFinal;
    TextoFinal.sprintf(IDIOMA__SE_HA_ENCONTRADO_UNA_NUEVA_ACTUALIZACION_DE_BUBATRONIK_DESEAS_DESCARGARLA, TextoNovedades());
    SIZE TamMensaje = CrearVentanaMensaje(TextoFinal(), IDIOMA__NUEVA_ACTUALIZACION, 101, 10);
    UINT TmpY = 0;
   	if (Sistema.App.Config.UtilizarBarraTituloWindows == true)  TmpY = 8;
    else                                                        TmpY = 25;
	UINT TmpInt = (((TamMensaje.cx + 20) - ((ANCHO_BOTON_VM * 2) + 10)) - 180) / 2;
    ProgresoDescarga.CrearBarraEx(WS_CHILD | WS_VISIBLE, _hWnd, 10, TamMensaje.cy + TmpY, TamMensaje.cx, 8, 0, DWL_BARRAEX_PROGRESO_HORIZONTAL);
	Boton_Aceptar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__ACEPTAR, TmpInt, TamMensaje.cy + TmpY + 10, ANCHO_BOTON_VM, 18, ID_BOTON_ACEPTAR);
	Boton_Cancelar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__CANCELAR, TmpInt + ANCHO_BOTON_VM + 10, TamMensaje.cy + TmpY + 10, ANCHO_BOTON_VM, 18, ID_BOTON_CANCELAR);
	Boton_Cancelar.AsignarFoco();
	Visible(true);
    Descargado = false;
                FlashWindow(_hWnd, TRUE); // invert the title bar 
                Sleep(500);              // wait a bit 
                FlashWindow(_hWnd, TRUE); // invert again 
}


void VentanaMensaje_Actualizacion::ProbarInstalacionActualizacion(void) {
	Mostrar();
	Descargado = true;
	Evento_BotonEx_Mouse_Click(0, 0, 0, ID_BOTON_ACEPTAR, 0);
}


// Cuando recibimos un click en un boton mandamos el mensaje a la ventana del reproductor
LRESULT VentanaMensaje_Actualizacion::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
    switch (IDBotonEx) {
        case ID_BOTON_ACEPTAR :
            if (Descargado == false) {
                DescargarActualizacion();
            }
            else {
                VentanaMensaje_ErrorCritico MsgAcp(IDIOMA__EL_REPRODUCTOR_SE_APAGARA_PARA_ACTUALIZARSE, IDIOMA__EMPEZANDO_ACTUALIZACION, 0);
	            DWL::DWLString Path;
				DWL::DWLString DirectorioEjecucion;
				DWL::DWLString DirectorioInstalacion;
				Sistema.Directorio.AppData(DirectorioEjecucion);
				DirectorioEjecucion += TEXT("\\BubaTronik\\");
				Path = DirectorioEjecucion;
				Path += TEXT("Instalar.exe");
				DirectorioInstalacion = TEXT('"');
				DirectorioInstalacion += Sistema.App.AppPath();
				DirectorioInstalacion += TEXT('"');

				ShellExecute(_hWnd, TEXT("open"), Path(), DirectorioInstalacion(), DirectorioEjecucion(), SW_SHOWMAXIMIZED);
				PostQuitMessage(0);
            }
            break;
        case ID_BOTON_CANCELAR :
            Sistema.App.BuscarActu.TerminarThreadDescargarActualizacion();
            Destruir();
            break;
    }
    return 0;
}

LRESULT VentanaMensaje_Actualizacion::Evento_Cerrar(void) {
    Sistema.App.BuscarActu.TerminarThreadDescargarActualizacion();
    Destruir();
    return 0;
}

void VentanaMensaje_Actualizacion::DescargarActualizacion(void) {
	RECT RC;
	GetClientRect(_hWnd, &RC);
	if (ProgresoDescarga.hWnd() != NULL)	ProgresoDescarga.Valor(0);
	else									ProgresoDescarga.CrearBarraEx(WS_CHILD | WS_VISIBLE, _hWnd, 155, RC.bottom - 14, RC.right - 165, 7, NULL, DWL_BARRAEX_PROGRESO_HORIZONTAL);
	Sistema.App.BuscarActu.IniciarThreadDescargarActualizacion(_hWnd);
    Boton_Aceptar.Activado(false);
	// Repinto el borde inferior para mostrar el texto "Descargando Actualización"
//	RC.top = RC.bottom - VENTANAEX_ALTURA_PARTE_INFERIOR;
//	RedrawWindow(_hWnd, &RC, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
}

void VentanaMensaje_Actualizacion::Novedades(const char *nNovedades) {
    TextoNovedades = nNovedades;
}


LRESULT CALLBACK VentanaMensaje_Actualizacion::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
		case MENSAJE_ACTUALIZACION_MAXIMOBARRA :
			ProgresoDescarga.Maximo(static_cast<long>(lParam));
			break;
		case MENSAJE_ACTUALIZACION_POSICIONBARRA :
			ProgresoDescarga.Valor(static_cast<long>(lParam));
			BarraTareas.BarraProgreso_Valor(ProgresoDescarga.Valor(), ProgresoDescarga.Maximo());
			break;
		case MENSAJE_ACTUALIZACION_FINDESCARGA :
			if (lParam == TRUE) {
//				Path.sprintf(TEXT("%sInstalar.exe"), App.AppPath());
                FlashWindow(_hWnd, TRUE); // invert the title bar 
                Sleep(500);              // wait a bit 
                FlashWindow(_hWnd, TRUE); // invert again 
                Descargado = true;
				
                Boton_Aceptar.Texto(IDIOMA__ACTUALIZAR);
                Boton_Aceptar.Activado(true);
                Boton_Cancelar.Activado(false);
				BarraTareas.BarraProgreso_Ocultar();
			}
			else {
                Descargado = false;								
				VentanaMensaje_Aceptar MsgAcp2(IDIOMA__ERROR_DESCARGANDO_LA_ACTUALIZACION, IDIOMA__ERROR_DE_DESCARGA, 0);
			}
			break;
    }
    return VentanaMensaje_Base::GestorMensajes(uMsg, wParam, lParam);
}

