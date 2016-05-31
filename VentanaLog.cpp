#include "StdAfx.h"
#include "VentanaLog.h"
#include "BubaTronikApp.h"
#include "IDS_Definidas.h"
#include "BubaTronikApp.h"

//#include "..\vlc-1.1.5\include\vlc\vlc.h"

//#define TIMER_VLC_LOG 1000

VentanaLog::VentanaLog(void) {
//	_Mutex = CreateMutex(NULL, FALSE, TEXT("Mutex_VentanaLog"));
}


VentanaLog::~VentanaLog(void) {
//	CloseHandle(_Mutex);
//	_Mutex = NULL;
}


void VentanaLog::Crear(void) {
	if (_hWnd != NULL) return;
	CrearVentanaEx(TEXT("BubaTronik_VentanaLog"), Sistema.App.Config.XLog, Sistema.App.Config.YLog, Sistema.App.Config.AnchoLog, Sistema.App.Config.AltoLog, NULL, TEXT("Mensajes del log"), true, true, VENTANALOG_ALTURA_PARTE_SUPERIOR);
	int VentanaExBorde = VENTANAEX_BORDE;
	int VentanaExAltoParteSuperior = VENTANALOG_ALTURA_PARTE_SUPERIOR;
	int An = Sistema.App.Config.AnchoLog;
	int Al = Sistema.App.Config.AltoLog;
	int InicioControles = 24;

	CM.Crear(_hWnd, An - CM_ANCHO, 0, ID_CM);
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		VentanaExBorde = 0;
		VentanaExAltoParteSuperior -= 22;
		InicioControles = 3;
		Al -= (Sistema.Medidas.Alto_BarraTitulo() + Sistema.Medidas.Alto_Borde());
		An -= (Sistema.Medidas.Ancho_Borde() * 2);
	}
	else {
		CM.Visible(true);
	}

	EditLog.CrearEdicionEx(WS_CHILD | WS_VISIBLE, _hWnd, TEXT(""), VentanaExBorde, VentanaExAltoParteSuperior, An - (VentanaExBorde *2), Al - ((VentanaExAltoParteSuperior + VENTANALOG_ALTURA_PARTE_INFERIOR) - 1), ID_EDITLOG, false, true);
	EditLog.EdicionEx_Estilos.TextoModificable = false;

/*	EditLog.EdicionEx_AgregarFuente(EditLog.hWnd(), Sistema.App.Tahoma15b());
	EditLog.EdicionEx_AgregarColorTexto(RGB(200,0,0));
	EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("Proba"));
	EditLog.EdicionEx_AgregarFuente(EditLog.hWnd(), Sistema.App.Tahoma13());
	EditLog.EdicionEx_AgregarColorTexto(RGB(0,0,0));
	EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("Proba2\n"));*/
//	CrearTemporizador(TIMER_VLC_LOG, 200);
	Visible(true);
}


LRESULT	VentanaLog::Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto) {

	int An = nAncho; //Rectangulo->right - Rectangulo->left;
	int Al = nAlto;  //Rectangulo->bottom - Rectangulo->top;

//	int InicioControles = 24;
	int VentanaExBorde = VENTANAEX_BORDE;
	int VentanaExAltoParteSuperior = VENTANALOG_ALTURA_PARTE_SUPERIOR;
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		VentanaExBorde = 0;
//		InicioControles = 3;
		VentanaExAltoParteSuperior -= 22;
//		Al -= (DWL::DWLSO::Medidas.Alto_BarraTitulo() + DWL::DWLSO::Medidas.Alto_Borde());
//		An -= (DWL::DWLSO::Medidas.Ancho_Borde() * 2);
	}
	else {
		CM.Mover(An - CM_ANCHO, 0, CM_ANCHO, CM_ALTO, false);
	}

	if (Maximizada() == false) {
		EditLog.Mover(VentanaExBorde, VentanaExAltoParteSuperior, An - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANALOG_ALTURA_PARTE_INFERIOR) - 1), true);
	}
	else {
		EditLog.Mover(0, VentanaExAltoParteSuperior, An, Al - ((VentanaExAltoParteSuperior + VENTANALOG_ALTURA_PARTE_INFERIOR) - 1), true);
	}

	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		InvalidateRect(_hWnd, NULL, FALSE);
	}
	return 0;
}

LRESULT VentanaLog::Evento_Cerrar(void) {
	WINDOWPLACEMENT WP;
	WP.length = sizeof(WINDOWPLACEMENT); 
	// Guardo posición de la ventana
	GetWindowPlacement(_hWnd, &WP); 
	if (WP.rcNormalPosition.left != -32000) {
		Sistema.App.Config.XLog = WP.rcNormalPosition.left;
		Sistema.App.Config.YLog = WP.rcNormalPosition.top;
		Sistema.App.Config.AnchoLog = WP.rcNormalPosition.right - WP.rcNormalPosition.left;
		Sistema.App.Config.AltoLog = WP.rcNormalPosition.bottom - WP.rcNormalPosition.top;
	}
	DestroyWindow(_hWnd);
	_hWnd = NULL;
	EditLog.EdicionEx_BorrarTexto();
	return 0;
}


void VentanaLog::AgregarMensaje(const LogProcedencia Procedencia, const LogSeveridad Severidad, const char *Modulo, const TCHAR *Mensaje, const bool nRepintar) {
//	WaitForSingleObject(_Mutex, INFINITE);
	if (_hWnd == NULL) {
//		ReleaseMutex(_Mutex);
		return;
	}
	bool bError = false;
	EditLog.EdicionEx_AgregarColorTexto(RGB(0,0,200));
	switch (Procedencia) {
		case Log_BubaTronik :	EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("BubaTronik "));	break;
		case Log_FMOD :			EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("FMOD "));			break;
		case Log_LibVLC :		EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("LibLVC "));		break;
		case Log_Sistema :		EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("Sistema "));		break;
	}

	switch (Severidad) {
		case Log_Info : 
			EditLog.EdicionEx_AgregarColorTexto(RGB(0, 150, 0));
			EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("Información "));
			break;
		case Log_Error :
			EditLog.EdicionEx_AgregarColorTexto(RGB(200, 0, 0));
			EditLog.EdicionEx_AgregarFuente(EditLog.hWnd(), DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita());
			EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("ERROR! "));
			bError = true;
			break;
		case Log_Warning:
			EditLog.EdicionEx_AgregarColorTexto(RGB(200, 100, 0));
			EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("Advertencia "));
			break;
		case Log_Debug:
			EditLog.EdicionEx_AgregarColorTexto(RGB(128, 128, 128));
			EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), TEXT("Depuración "));
			break;
	}
	if (bError == false) EditLog.EdicionEx_AgregarFuente(EditLog.hWnd(), DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita());
	EditLog.EdicionEx_AgregarColorTexto(RGB(0, 0, 0));
	DWL::DWLString MsgFinal;
	MsgFinal = Modulo; // Paso a unicode el string del modulo.
	EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), MsgFinal());
	EditLog.EdicionEx_AgregarFuente(EditLog.hWnd(), DWL::SO::DWLEstilos::Fuentes.Tahoma13());
	MsgFinal.sprintf(TEXT(" %s\n"), Mensaje);
	EditLog.EdicionEx_AgregarTexto(EditLog.hWnd(), MsgFinal());
	if (nRepintar == true) EditLog.Repintar(true);
	
//	ReleaseMutex(_Mutex);
}
/*
void VentanaLog::AsignarLogAnalisisVLC(libvlc_log_t *LogAnalisis) {
	WaitForSingleObject(_Mutex, INFINITE);
	_LogAnalisis = LogAnalisis;
	ReleaseMutex(_Mutex);
}*/

/*
LRESULT VentanaLog::Evento_Temporizador(const UINT IDTemporizador) {
	if (TIMER_VLC_LOG == IDTemporizador) {
		UINT TotalMensajes = 0;
		DWL::DWLString StrMsg;
		libvlc_log_message_t   msg;
		libvlc_log_message_t  *pmsg;
		libvlc_log_iterator_t *IteratorLog = libvlc_log_get_iterator(Sistema.App.VLC.Log());
		while (libvlc_log_iterator_has_next(IteratorLog)) {
			pmsg = libvlc_log_iterator_next(IteratorLog, &msg);
			StrMsg  = msg.psz_message; // conversion a unicode
			AgregarMensaje(Log_LibVLC, static_cast<LogSeveridad>(msg.i_severity),  msg.psz_name, StrMsg(), false);
			TotalMensajes ++;
		}
		libvlc_log_iterator_free(IteratorLog);
		libvlc_log_clear(Sistema.App.VLC.Log());
		return 0;
	}
	return VentanaEx::Evento_Temporizador(IDTemporizador);
}*/