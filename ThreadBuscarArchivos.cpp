#include "StdAfx.h"
#include "ThreadBuscarArchivos.h"
//#include "TipoArchivo.h"
#include "BubaTronikApp.h"
#include "InformacionArchivoEx.h"

ThreadBuscarArchivos::ThreadBuscarArchivos(void) {
    _Mutex = NULL;
	_Cancelar = false;
}

ThreadBuscarArchivos::~ThreadBuscarArchivos(void) {
}

// Para hacer la busqueda nos vamos a copiar todos los datos necesarios para no tener que tocar el thread principal
const BOOL ThreadBuscarArchivos::IniciarBusqueda(DWL::ControlesEx::DWLListaEx *ListaDirectorios) {
    // Path de la aplicacion
    _PathApp = Sistema.App.AppPath();

	DWL::DWLString TmpStr;
    size_t i;
    _hWndCP = Sistema.App.PlayerEx.CP.hWnd();
/*    // Guardamos paths de la BD
    for (i = 0; i < Sistema.App.BD.Medios.size(); i++) {
        _PathsBD.push_back(Sistema.App.BD.Medios[i]->Path);
    }*/
    // Guardamos los tipos a usar
	_TipoArchivo.CopiarTipos(Sistema.App.Tipos);
    // Error no hay tipos de archivo
    if (_TipoArchivo.Tipos.size() == 0) return -1; 
    // Guardamos la lista de directorios a escanear
	for (i = 0; i < Sistema.App.Config.Buscar_Paths.size(); i++) {
        _PathsBuscar.push_back(Sistema.App.Config.Buscar_Paths[i]);
    }
    // Error no hay directorios
    if (_PathsBuscar.size() == 0) return -2; 
    _Cancelar = false;

    // Obtengo una lista de las unidades con sus datos privada para esta clase
    _Unidades.Escanear_Unidades_Locales();

    // Iniciamos el Thread
	_Thread = CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->_ThreadBusqueda, (void *)this, 0, NULL);
	if (_Thread) SetThreadPriority(_Thread, 0);
    return TRUE;
}
/*
const bool ThreadBuscarArchivos::Buscando(void) {
    if (_Mutex == NULL) return false;
    return _Buscando;
}*/


const bool ThreadBuscarArchivos::Cancelado(void) {
	if (_Mutex != NULL) return false; // Si existe un mutex es que la funcion esta aun buscando y no queremos tocar memoria compartida
	return _Cancelar;
}

void ThreadBuscarArchivos::Cancelar(void) {
    WaitForSingleObject(_Mutex, INFINITE);
    _Cancelar = true;
  	ReleaseMutex(_Mutex);
}

unsigned long ThreadBuscarArchivos::_ThreadBusqueda(void *pThis) {
    size_t                i    = 0;
    ThreadBuscarArchivos *This = reinterpret_cast<ThreadBuscarArchivos *>(pThis);
	This->_Mutex = CreateMutex(NULL, FALSE, TEXT("Mutex_ThreadBuscarArchivos"));
//    This->_Buscando = true;
	DWL::DWLString *TmpStr = NULL;

    // Inicio la VLC para el analisis
	DWL::DWLString PluginPath;
	PluginPath.sprintf(TEXT("--plugin-path=%s%s"), This->_PathApp(), PATHVLC_PLUGINS);
	ArgumentosVLC ArgsVLC;
	ArgsVLC.AgregarArgumento(This->_PathApp());		// Path de la aplicación
	ArgsVLC.AgregarArgumento(PluginPath());		// Path del directorio de plugins
   	libvlc_instance_t *InstanciaVLC = libvlc_new(ArgsVLC.TotalArgumentos(), ArgsVLC());

	// Inicio el FMOD para el analisis
	FMOD::System *InstanciaFMOD;
	FMOD_RESULT FmodErrorCode = FMOD_OK;
	FmodErrorCode = FMOD::System_Create(&InstanciaFMOD);
	// - Obtengo el path de los pluguins
	DWL::DWLString CodecsPath = This->_PathApp(); CodecsPath += TEXT("Plugins\\FMOD");
	// - Paso el path de unicode a ascii
	char CodecsPathAscii[MAX_PATH + 1] = "";
	WideCharToMultiByte(CP_OEMCP, WC_NO_BEST_FIT_CHARS, CodecsPath(), static_cast<int>(CodecsPath.Tam()) + 1, CodecsPathAscii, MAX_PATH + 1, NULL, NULL);
	FmodErrorCode = InstanciaFMOD->setPluginPath(CodecsPathAscii);
	// Inicio el sonido con 16 canales
	FmodErrorCode = InstanciaFMOD->init(16, FMOD_INIT_NORMAL, NULL);


    // Busco en todas las rutas
	for (i = 0; i < This->_PathsBuscar.size(); i++) This->_BusquedaRecursiva(This->_PathsBuscar[i].Path());

    PostMessage(This->_hWndCP, MENSAJE_OBTENIENDO_DATOS_MEDIOS, static_cast<WPARAM>(This->_PathsEncontrados.size()), NULL);
    // Ya tengo todos los paths, ahora los escaneamos
    InformacionArchivoEx *Info = NULL;
//    libvlc_time_t         TiempoTotal;
//    bool                  EsVideo;
    for (i = 0; i < This->_PathsEncontrados.size(); i++) {        
		if (This->_Mutex) WaitForSingleObject(This->_Mutex, INFINITE);
//		This->_VentanaLog->AgregarMensaje(Log_BubaTronik, Log_Info, "Analizando :", This->_PathsEncontrados[i].Texto());
		if (SendMessage(This->_hWndCP, MENSAJE_MEDIO_NO_EXISTE, reinterpret_cast<WPARAM>(This->_PathsEncontrados[i].Texto()), 0) == 1) {
			SOLO_DEBUG(DWL::DWLDebug::ImprimirDebug(This->_PathsEncontrados[i].Texto()));
			SOLO_DEBUG(DWL::DWLDebug::ImprimirDebug(TEXT("\n")));
			Info = new InformacionArchivoEx;
			TmpStr = new DWL::DWLString(This->_PathsEncontrados[i].Texto());

			PostMessage(This->_hWndCP, MENSAJE_MEDIO_ANALIZANDO, reinterpret_cast<WPARAM>(TmpStr), NULL);

			TMedio *TM = Info->ObtenerInfo(This->_PathsEncontrados[i].Texto(), This->_TipoArchivo, This->_Unidades, This->_PathsBuscar, InstanciaVLC, InstanciaFMOD);
			if (TM != NULL) PostMessage(This->_hWndCP, MENSAJE_MEDIO_ANALIZADO, reinterpret_cast<WPARAM>(TM), NULL);
			else			PostMessage(This->_hWndCP, MENSAJE_MEDIO_INVALIDO, reinterpret_cast<WPARAM>(TM), NULL);
			delete Info;
		}

		if (This->_Cancelar == true) { 
			if (This->_Mutex) ReleaseMutex(This->_Mutex);
			break;
		}
		if (This->_Mutex) ReleaseMutex(This->_Mutex);
    }

//	ReleaseMutex(This->_Mutex);
	// Termino la instancia de la VLC
	libvlc_release(InstanciaVLC);

	// Termino la instancia del FMOD
	InstanciaFMOD->close();
	InstanciaFMOD->release();

	// Cierro el mutex, y informo a la ventana del reproductor que ya se ha terminado
	if (This->_Mutex) CloseHandle(This->_Mutex);
	This->_Mutex = NULL;
    PostMessage(This->_hWndCP, MENSAJE_BUSCAR_ARCHIVOS_TERMINADO, NULL, NULL);
//	MessageBox(NULL, TEXT("Terminado"), TEXT("Terminado"), MB_OK);
    return 0;
}


const UINT ThreadBuscarArchivos::_BusquedaRecursiva(const TCHAR *Path) {							
    DWL::DWLString      PathFinal = Path;
    WIN32_FIND_DATA		FindInfoPoint;
	HANDLE				hFind						= NULL;
	UINT        		TotalArchivosEscaneados	    = 0;
	int					TmpRet						= 0;
	TCHAR				TmpPath[MAX_PATH];
    DWL::DWLString     *TextoPostMessage            = NULL;

	GetShortPathName(Path, TmpPath, MAX_PATH); 
	if (TmpPath[0] == 0) return 0;

	if (PathFinal[PathFinal.Tam() -1] != TEXT('\\'))	 PathFinal += TEXT("\\*.*");
	else												 PathFinal += TEXT("*.*");
	hFind = FindFirstFile(PathFinal(), &FindInfoPoint);
	while (FindNextFile(hFind, &FindInfoPoint) != 0) {
        // Miro si se ha cancelado
        WaitForSingleObject(_Mutex, INFINITE);
        if (_Cancelar == true) {
           	ReleaseMutex(_Mutex);
            return static_cast<UINT>(TotalArchivosEscaneados);
        }
    	ReleaseMutex(_Mutex);
        
		PathFinal = Path;
		if (PathFinal[PathFinal.Tam() -1] != TEXT('\\')) PathFinal += TEXT('\\');

		if (_Comprobacion(FindInfoPoint.cFileName) == true) {
			PathFinal += FindInfoPoint.cFileName;
			// Si es un directorio y no empieza por ".", llamo a esta función recursiva de nuevo
			if (FindInfoPoint.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && FindInfoPoint.cFileName[0] != TEXT('.')) {
				TmpRet = _BusquedaRecursiva(PathFinal());
				if (TmpRet < 0) return TmpRet; // Retorno en caso de error
				TotalArchivosEscaneados += TmpRet;
			}
			// Si es un archivo, llamo a la función Encontrado y le paso los datos pertinentes
			else {
				TotalArchivosEscaneados ++;
                if (_ExtensionValida(PathFinal) == true) {
                    _PathsEncontrados.push_back(PathFinal);
//                    TextoPostMessage = new DWL::DWLString(PathFinal);
//                    PostMessage(_hWndCP, MENSAJE_MEDIO_ENCONTRADO, reinterpret_cast<WPARAM>(TextoPostMessage), NULL);
                }
			}
		}
	}
	FindClose(hFind);
	return static_cast<UINT>(TotalArchivosEscaneados);
};

const bool ThreadBuscarArchivos::_Comprobacion(const TCHAR *Path) {
	if (Path[0] == TEXT('.') && Path[1] == 0) return false;
	if (Path[0] == TEXT('.') && Path[1] == TEXT('.') && Path[2] == 0) return false;
	return true;
};
/*
const bool ThreadBuscarArchivos::_EsSoloAudio(DWL::DWLString &PathFinal) {
	DWL::DWLString Extension;
    size_t         i = 0;
    // Obtenemos la extensión
    for (i = PathFinal.Tam() -1; i > 0; i--) {
        if (PathFinal[i] == TEXT('.')) {
            Extension = PathFinal.SubStr(i + 1);
            break;
        }
    }
    // No tiene extension.
    if (Extension.Tam() == 0) return false;
	TipoArchivo_Datos *TPD = _TipoArchivo.BuscarTipoArchivo(Extension());
	if (TPD->Tipo == Audio && TPD->Buscar_Asociar == true)	return true;
	else													return false;
}*/


const bool ThreadBuscarArchivos::_ExtensionValida(DWL::DWLString &PathFinal) {
    DWL::DWLString Extension;
    size_t         i = 0;
    // Obtenemos la extensión
    for (i = PathFinal.Tam() -1; i > 0; i--) {
        if (PathFinal[i] == TEXT('.')) {
            Extension = PathFinal.SubStr(i + 1);
            break;
        }
    }
    // No tiene extension.
    if (Extension.Tam() == 0) return false;

    // Comparamos el Path con los tipos de archivo
    for (i = 0; i < _TipoArchivo.Tipos.size(); i++) {
        // La extension es valida
		if (Extension.Comparar(_TipoArchivo.Tipos[i]->Extension()) == true && _TipoArchivo.Tipos[i]->Buscar_Asociar == true) {
            // Escaneo las rutas de la BD por si coincide con el archivo
/*            for (size_t i2 = 0; i2 < _PathsBD.size(); i2++) {
                // Si el archivo existe en la BD retornamos false, para que no se añada al vector de _PathsEncontrados
                if (_PathsBD[i2].Comparar(PathFinal()) == true) return false;
            }*/
            // El archivo es valido
            return true;
        }
    }
  
    return false;
}





