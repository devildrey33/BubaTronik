#pragma once
#include <DWLListaEx.h>
#include <DWLString.h>
#include <DWLVentana.h>
#include "BaseDatos.h"
#include "Unidades_de_Disco.h"
#include "..\vlc-1.1.5\include\vlc\vlc.h"
#include "VentanaLog.h"
#include "TipoArchivo.h"
/*
class VLCAnalizarMedio {
  public:
                                            VLCAnalizarMedio(void) { Instancia = NULL; };
                                           ~VLCAnalizarMedio(void) { };
    const bool								IniciarInstanciaAnalisis(const TCHAR *PathApp);
    const bool                              Analizar(const TCHAR *Path, libvlc_time_t &TiempoTotal);
  private :
    libvlc_instance_t					   *Instancia;
};
*/

class ThreadBuscarArchivos {
  public:
                                 ThreadBuscarArchivos(void);
                                ~ThreadBuscarArchivos(void);
    const BOOL                   IniciarBusqueda(DWL::ControlesEx::DWLListaEx *ListaDirectorios);
//    const bool                   Buscando(void);
    void                         Cancelar(void);
    const bool                   Cancelado(void);
	inline HANDLE                Thread(void) { return _Thread; };
  private :
    static unsigned long        _ThreadBusqueda(void *pThis);
    const UINT			        _BusquedaRecursiva(const TCHAR *Path);
    const bool                  _Comprobacion(const TCHAR *Path);
    const bool                  _ExtensionValida(DWL::DWLString &PathFinal);
//	const bool                  _EsSoloAudio(DWL::DWLString &PathFinal);
    TipoArchivo                 _TipoArchivo;
//    std::vector<DWL::DWLString> _PathsBD;
    std::vector<DatosPathBuscar> _PathsBuscar;
    std::vector<DWL::DWLString> _PathsEncontrados;
  	HANDLE		                _Mutex;
	HANDLE	                    _Thread;
//    bool                        _Buscando;
    bool                        _Cancelar;
    HWND                        _hWndCP;
    Unidades_Disco              _Unidades;
    DWL::DWLString              _PathApp;
//	VentanaLog                 *_VentanaLog;
//    VLCAnalizarMedio            _VLC;
};

