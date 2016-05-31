#pragma once

#include "DWL.h"

#pragma comment(lib, "wininet.lib")	// Agrego la libreria wininet.lib al proyecto


class BuscarActualizaciones {
public:
						BuscarActualizaciones(void);
					   ~BuscarActualizaciones(void);
   void					IniciarThreadBuscarActualizacion(HWND hWndDestino, const bool OmitirDia = false);
   void					IniciarThreadDescargarActualizacion(HWND hWndDestino);
   void                 TerminarThreadDescargarActualizacion(void);
   void                 CancelarDescarga(void);
private:
   static unsigned long	ThreadBuscarActualizacion(void *pThis);
   static unsigned long	ThreadDescargarActualizacion(void *phWnd);
   HANDLE				hThread;
   HANDLE				hThreadDescargar;
   static bool         _CancelarDescarga;
   static HANDLE        Mutex;
};
