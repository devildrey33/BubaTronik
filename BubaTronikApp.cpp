#include "StdAfx.h"
#include "BubaTronikApp.h"
#include <tlhelp32.h>	// Tool Help Functions
#include <Shellapi.h>	//
#include "ConfigBubaTronik.h"
#include <DWLPortaPapeles.h>
#include <DWLArchivosWindows.h>
#include "TipoArchivo.h"
#include "AsociarArchivos.h"


//#include "EnviarEmail.h"

BubaTronikApp::BubaTronikApp(void) {
	EditandoTeclasRapidas = false;
	Idioma				= NULL;
	Idioma_EstiloStr	= NULL;
	Idioma_Version		= NULL;
	Idioma_Nombre		= NULL;
	Idioma_Icono		= NULL;
	MutexPlayer			= NULL;
	PlayerInicial		= false;
}


BubaTronikApp::~BubaTronikApp(void) {
	DescargarIdioma();
}



const BOOL BubaTronikApp::Inicio(void) {

	#if defined _DEBUG
		#if defined SIMULAR_PATH_DEBUG
			bool bDebug	  = false; // arreglo para que el apppath sea el directorio raiz en ve del directorio debug
		#endif
	#endif

	// Obtengo el directorio actual de la aplicacion
	TCHAR PathApp[1024];
	DWORD Size = GetModuleFileName(NULL, PathApp, 1024);
	for (Size; Size > 0; Size --) {
		if (PathApp[Size] == TCHAR('\\')) {
			#if defined _DEBUG
				#if defined SIMULAR_PATH_DEBUG
					if (bDebug == false) {
						bDebug = true;
					}
					else {
						break;
					}
				#else
					break;
				#endif
			#else // #if defined _DEBUG
				break;
			#endif
		}
	}
	PathApp[Size + 1] = 0;
	AppPath = PathApp;
	// Aseguro que el directorio actual sea el del player
	SetCurrentDirectory(AppPath()); 
	

	// Creo el directorio del \program data por si no existe
	DWL::DWLString AppData;
	Sistema.Directorio.AppData(AppData);
	AppData += TEXT("\\BubaTronik\\");
	BOOL RCD = CreateDirectory(AppData(), NULL);
	// Cargo la configuración
	Config.Cargar(AppData());
	
	// Cargo el idioma
	bool ErrorPlayer = false;
	ErrorPlayer = !CargarIdioma(Config.Idioma_Path());
	if (ErrorPlayer == true) { // No se encuentra el idioma de la configuracion
		Config.Idioma_Path = TEXT("IdiomaCastellano.dll"); // Pasamos al idioma por defecto
		ErrorPlayer = !CargarIdioma(Config.Idioma_Path());        
		if (ErrorPlayer == false) {
            VentanaMensaje_ErrorCritico ErrIdioma1(TEXT("ERROR 0"), TEXT("Error"), 0);
		}
	}
	if (ErrorPlayer == true) { // No se encuentra ".\Idiomas\Castellano.dll" ABORTAR EJECUCION
        VentanaMensaje_ErrorCritico ErrIdioma2(TEXT("FATAL ERROR 1"), TEXT("Error"), 0);
		return FALSE;
	}

	// Inicio el filtro de errores
	if (FiltroErrores.Iniciar(TEXT("BubaTronik")) == false) { // Ha sucedido un error, y ademas no se ha podido guardar el dump.....+
		VentanaMensaje_ErrorCritico MsgErr(IDIOMA__BUBATRONIK_HA_ENCONTRADO_UN_ERROR_CRITICO_Y_ADEMAS_NO_HA_PODIDO_CREAR_UN_INFORME_DE_ERRORES, IDIOMA__ERROR, 0);
		return FALSE;
	}

//	#define SIMULACION_ARGS
	// Miro la linea de comandos
	#if defined SIMULACION_ARGS
/*		int			 TotalArgs	= 3;
		const TCHAR *Args[] = {	TEXT("c:\\BubaTronik.exe"), 
								TEXT("c:\\mp3\\iron maiden\\iron maiden\\01 prowler.mp3"), 
								TEXT("c:\\mp3\\avalanch\\eternal flame\\03 awake.mp3"), 
							  };*/
		int TotalArgs = 2;
		const TCHAR *Args[] = {	TEXT("D:\\Programacio\\Projectes C++\\BubaTronik\\BubaTronik.exe"), 
								TEXT("-InstalarRegistroWindows"), 
		};
	#else
		int		TotalArgs	= 0;
		TCHAR **Args		= CommandLineToArgvW(GetCommandLine(), &TotalArgs);
	#endif

	bool LineaComando = false;
	bool ReproducirPorDefecto = false;
	if (TotalArgs > 1) {
		if (TotalArgs == 2) {
			if (DWLStrCmp(Args[1], TEXT("-InstalarRegistroWindows")) == 0) { // SPAD reinstall command
				if (IsUserAnAdmin() == FALSE) { // si es admin
					ShellExecute(NULL, TEXT("RunAs"), Args[0], TEXT("-InstalarRegistroWindows"), AppPath(), SW_SHOWNORMAL);
					return 0;
				}
				else { // Asociar reproductor a los archivos
					Sistema.App.Asociar.RegistrarApp();
					ReproducirPorDefecto = Sistema.App.Asociar.ReproducirPorDefecto();
					return 0;
				}
			}
			else if (DWLStrCmp(Args[1], TEXT("-DesinstalarRegistroWindows")) == 0) { // Eliminar toda la mierda del registro
				if (IsUserAnAdmin() == FALSE) { // si es admin
					ShellExecute(NULL, TEXT("RunAs"), Args[0], TEXT("-DesinstalarRegistroWindows"), AppPath(), SW_SHOWNORMAL);
					return 0;
				}
				else { // Asociar reproductor a los archivos
					Sistema.App.Asociar.DesRegistrarApp();
					return 0;
				}
			}
			else if (DWLStrCmp(Args[1], TEXT("-EsconderIconos")) == 0) { // SPAD hide icons
				MessageBox(NULL, TEXT("Opción no soportada por este reproductor, si quieres des-habilitarlo la mejor opción es desinstalarlo."), TEXT("Error"), MB_OK);
				return 0;
			}
			else if (DWLStrCmp(Args[1], TEXT("-MostrarIconos")) == 0) { // SPAD show icons
				// no hago ni muestro nada pero la dejo por si a alguien le da por usarla en XP, que no se encienda el reproductor.
				return 0;
			}
			else if (DWLStrCmp(Args[1], TEXT("-EliminarArchivosDisco")) == 0) { // Eliminar archivos del disco
				if (IsUserAnAdmin() == FALSE) { // si es admin
					ShellExecute(NULL, TEXT("RunAs"), Args[0], TEXT("-EliminarArchivosDisco"), AppPath(), SW_SHOWNORMAL);
					return 0;
				}
				else {
					ArchivoBinarioPlayer Archivo;
					DWL::DWLString PathBuffer;
					Sistema.Directorio.AppData(PathBuffer);
					PathBuffer += TEXT("\\Eliminar.btr");
					Archivo.AbrirArchivoLectura(PathBuffer());
					UINT TotalBorrar = 0;
					Archivo.LeerUINT(TotalBorrar);
					PathBuffer = TotalBorrar;
					for (size_t nz = 0; nz < TotalBorrar; nz ++) {
						Archivo.LeerString(PathBuffer);
						DeleteFile(PathBuffer());
						// Obtenemos el directorio y probamos a eliminarlo despues de eliminar cada cancion
						// si el directorio tiene archivos dentro no se eliminara.
						size_t PosP1 = 0;
						for (size_t az = PathBuffer.Tam(); (az > 0 && PosP1 == 0); az --) {
							if (PathBuffer[az -1] == TEXT('\\')) PosP1 = az - 1;
						}
						if (PosP1 != 0)	RemoveDirectory(PathBuffer.SubStr(0, PosP1).Texto());
					}
					Sistema.Directorio.AppData(PathBuffer);
					PathBuffer += TEXT("\\Eliminar.btr");
					DeleteFile(PathBuffer());
					return 0;
				}
			}
			else if (DWLStrCmp(Args[1], TEXT("-PetarBubaTronik")) == 0) { // Provoco una violacion de acceso para testear el sistema de enviado de dumps
				*(int*)0 = 1;	// Provocamos una violación de acceso para testear el filtro de errores.
				return 0;
			}

		}
		LineaComando = true;
	}



	// Obtengo/Creo un mutex para evitar que 2 o mas reproductores se inicien a la vez.
	// De esta forma cuando se inicia el reproductor, este espera a que el anterior termine completamente de cargarse.
	MutexPlayer = OpenMutex(NULL, FALSE, TEXT("Mutex_BubaTronik"));
	if (MutexPlayer == NULL) {
		PlayerInicial = true;
		MutexPlayer = CreateMutex(NULL, FALSE, TEXT("Mutex_BubaTronik"));
	}
	WaitForSingleObject(MutexPlayer, INFINITE);
	

	

	DWLSRand(); // Inicio la semilla para los numeros aleatorios
	

	HWND InstanciaPrevia = FindWindowEx(NULL, NULL, APP_NOMBRE, NULL);

	#ifndef IGNORAR_INSTANCIA_PREVIA
		if (InstanciaPrevia != NULL) {
			DWL::DWLString Tmp;
			for (int a = 1; a < TotalArgs; a ++) {
				Tmp += TEXT('<');
				Tmp += Args[a];
				Tmp += TEXT('>');
			}
			DWL::SO::DWLPortaPapeles::AsignarTexto(Tmp());
			SendMessage(InstanciaPrevia, WM_PARAMETROS, NULL, static_cast<LPARAM>(Tmp.Tam()));
			ReleaseMutex(MutexPlayer);
			return FALSE;
		}
	#endif // #ifndef IGNORAR_INSTANCIA_PREVIA

	EscanearUnidades();					// <- Necesario para saber si las canciones estan realmente o no.


	// Cargo el skin
	Skin.CargarSkin(Config.Skin_Path());

	#if defined UTILIZAR_FMOD
		// Inicio el FMOD
		ErrorPlayer = !FMod.Iniciar();
		if (ErrorPlayer == true) {
			ReleaseMutex(MutexPlayer);
			if (PlayerInicial == true) CloseHandle(MutexPlayer);
			return FALSE;
		}
	#endif
	// Cargo las canciones de la BD
	BD.Iniciar(AppData());

	// Creo la ventana del reproductor
	PlayerEx.Crear();
	PlayerEx.CP.Mostrar_CargandoVLC();
	
	MSG Message;
	int ContadorMensajes = 25;
	while(TRUE == GetMessage(&Message, 0, 0, 0) && --ContadorMensajes != 0) {		// Minetras la Ventana retorne Mensajes
		TranslateMessage(&Message);													// Traduzco el Mensaje
		DispatchMessage(&Message);													// Paso el Mensaje a la Ventana
	}  	


	// Inicio el VLC
	DWL::DWLString PathTmp, PathTmp2;
	PathTmp.sprintf(TEXT("%s%s"), AppPath(), PATHVLC);
	VLC.IniciarInstanciaVLC();

	bool Reproducir = false;

	if (LineaComando == false) {
		BD.CargarListaInicio();
	}
	else {
		for (int i = 1; i < TotalArgs; i++) {
			if (Args[i] == TEXT("-r")) 	Reproducir = true;
			else						PlayerEx.AgregarArchivoLista(Args[i]);
		}
		ListaC.ActualizarTodo();
		if (Config.Shufle == true) ListaC.Shufle(true);
		if (TotalArgs > 1 || Reproducir == true) Media.AbrirPrimerMedioLista();
	}

	// Elimino memoria que ocupan los argumentos
	if (TotalArgs > 0) {
//		for (int r = 0; r < TotalArgs; r++) LocalFree(Args[r]);
		LocalFree(Args);
	}
	PlayerEx.CP.Mostrar_Inicio();

	DragAcceptFiles(PlayerEx.hWnd(), TRUE);


	// Busco actualizaciones del reproductor
	if (Config.BuscarActualizaciones == true)	BuscarActu.IniciarThreadBuscarActualizacion(PlayerEx.hWnd());

	
    //////////////////////////////////////////////////////////////////////////////////////
	// TESTS PARA LA ACTUALIZACION	     												//
	//////////////////////////////////////////////////////////////////////////////////////
    //  BuscarActu.IniciarThreadBuscarActualizacion(PlayerEx.hWnd(), true); // Busca si hay nuevas actus, descarga e instala
	//  PlayerEx.VentanaActualizacion.ProbarInstalacionActualizacion();
	//////////////////////////////////////////////////////////////////////////////////////

	
	// Comprobacion de las asociaciones de archivos
	switch (Config.AsociarArchivos) {
		case Respuesta_Aceptar :
		case Respuesta_Cancelar :
			if (Asociar.ComprobarAsociaciones() == false) {
                VentanaAsociar.MostrarMensaje(IDIOMA__ESTE_REPRODUCTOR_NO, IDIOMA__ASOCIAR_ARCHIVOS, ID_MENSAJE_ASOCIAR);
			}
			break;
		case Respuesta_Aceptar_NoMostrar :
			if (Asociar.ComprobarAsociaciones() == false) {
				Asociar.RegistrarApp();
			}
			break;
		case Respuesta_Cancelar_NoMostrar :
			break;
	}

	ReleaseMutex(MutexPlayer);

	#ifdef MOSTRAR_VENTANAPRINCIPALEX
		VentanaPrincipal.Crear();
	#endif

	return TRUE;
};

void BubaTronikApp::Fin(void) {
	DWL::DWLString AppData;
	Sistema.Directorio.AppData(AppData);
	AppData += TEXT("\\BubaTronik\\");

	DWL::DWLString PathTmp;
	if (PlayerInicial == true) CloseHandle(MutexPlayer);

	// Descargo el VLC
	VLC.TerminarInstancia();

	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, TRUE); // activo el protector de pantalla

	// Guardo bd, configuración y limpio la memoria.
	Config.Guardar(AppData());
	Sistema.App.Tipos.BorrarTodosLosTipos();
	#if defined UTILIZAR_FMOD
		FMod.Terminar();
	#endif
	BD.Guardar(AppData());
	BD.Terminar();

};

void BubaTronikApp::Eventos_Mirar(void) {
	static MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


//
// Función para cerrar el sistema de varias formas (SOCerrarSistema [CS_Apagar, CS_Reiniciar, CS_CerrarUsuario])
// void CerrarSistema(const SOCerrarSistema Forma = CS_Apagar, const bool Forzar = false);
void BubaTronikApp::CerrarSistema(const SOCerrarSistema Forma, const bool Forzar) {
	HANDLE           hToken;
	TOKEN_PRIVILEGES tkp;
	UINT             Flags = NULL;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, false, &tkp, NULL, (PTOKEN_PRIVILEGES)NULL, 0);
	switch (Forma) {
		case CS_Apagar			:	Flags = EWX_POWEROFF;	break;
		case CS_ReIniciar		:	Flags = EWX_REBOOT;		break;
		case CS_CerrarUsuario	:	Flags = EWX_LOGOFF;		break;
	}
	if (Forzar == true) Flags = (Flags | EWX_FORCE);
	ExitWindowsEx(Flags, 0);						
};


const int BubaTronikApp::EscanearUnidades(void) {
	unsigned int Total = UnidadesDisco.Escanear_Unidades_Locales();
	return Total;
}

const int BubaTronikApp::EscanearProcesos(void) {
	HANDLE         hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL           bResult   = FALSE;
	PROCESSENTRY32 pe;
	
	pe.dwSize = sizeof(pe);
	bResult = Process32First(hSnapShot, &pe);
	int Contador = 0;
	while (bResult != NULL) {
		bResult = Process32Next(hSnapShot, &pe);
		Contador ++;
	}
	CloseHandle(hSnapShot);		// Destruir snapshot
	return Contador;
}

/*
const RespuestaVentanaMensaje BubaTronikApp::MostrarMensaje(const TCHAR *Texto, const TCHAR *Titulo, const TipoVentanaMensaje Tipo) {
	VentanaMensaje VM;
	return VM.MostrarMensaje(Texto, Titulo, Tipo);
}*/


const bool BubaTronikApp::CargarIdioma(const TCHAR *Path) {
	DWL::DWLString TxtError;
	DWL::DWLString PathCompleto;
	PathCompleto.sprintf(TEXT("%s%s%s"), AppPath(), TEXT("Idiomas\\"), Path);
	LibActualIdioma = LoadLibrary(PathCompleto());
	if (LibActualIdioma == NULL) {
		return false;
	}
	
	Idioma				= (TpTextoIdioma *)GetProcAddress((HMODULE)LibActualIdioma, "TextoIdioma");
	Idioma_EstiloStr	= (TpTextoIdioma *)GetProcAddress((HMODULE)LibActualIdioma, "TextoEstilo");
	Idioma_Nombre		= (TpNombre *)GetProcAddress((HMODULE)LibActualIdioma, "Nombre");
	Idioma_Version		= (TpVersion *)GetProcAddress((HMODULE)LibActualIdioma, "Version");
	Idioma_Icono		= (TpIcono *)GetProcAddress((HMODULE)LibActualIdioma, "Icono");
	if (Idioma == NULL || Idioma_Nombre == NULL || Idioma_Version == NULL || Idioma_Icono == NULL || Idioma_EstiloStr == NULL)  {
		TxtError.sprintf(TEXT("Error cargando el idioma.\nLa libreria no es valida para este reproductor\n%s"), Path);
		VentanaMensaje_ErrorCritico MsgErr(TxtError(), TEXT("Error"), 0);
		return false;
	}
	if (Idioma_Version() != APP_IDIOMA_VER) {
		TxtError.sprintf(IDIOMA__ERROR_CARGANDO_EL_IDIOMA, Path, Idioma_Version(), APP_IDIOMA_VER);
		VentanaMensaje_ErrorCritico MsgErr2(TxtError(), TEXT("Error"), 0);
		return false;
	}
	return true;
}

void BubaTronikApp::DescargarIdioma(void) {
	if (LibActualIdioma != NULL) {
		FreeLibrary(LibActualIdioma);
		Idioma				= NULL;
		Idioma_EstiloStr	= NULL;
		Idioma_Nombre		= NULL;
		Idioma_Version		= NULL;
		Idioma_Icono		= NULL;
	}
}