#pragma once

#include "VentanaEx.h"
#include "ControlCerrarMinimizar.h"
#include <DWLEdicionEx.h>


enum LogProcedencia {
	Log_BubaTronik = 0,
	Log_FMOD       = 1,
	Log_LibVLC     = 2,
	Log_Sistema    = 3
};

enum LogSeveridad {
	Log_Info    = 0,
	Log_Error   = 1,
	Log_Warning = 2,
	Log_Debug   = 3
};

struct libvlc_log_t;

class VentanaLog : public VentanaEx {
  public:	
									VentanaLog(void);
	                               ~VentanaLog(void);
	void							Crear(void);
	LRESULT							Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto);
	LRESULT                         Evento_Cerrar(void);
	void                            AgregarMensaje(const LogProcedencia Procedencia, const LogSeveridad Severidad, const char *Modulo, const TCHAR *Mensaje, const bool nRepintar = true);
//	LRESULT                         Evento_Temporizador(const UINT IDTemporizador);
//	void                            AsignarLogAnalisisVLC(libvlc_log_t *LogAnalisis);
	DWL::ControlesEx::DWLEdicionEx	EditLog;
	ControlCerrarMinimizar			CM;
  private :
//	HANDLE                         _Mutex;
//	libvlc_log_t				  *_LogAnalisis;
};

