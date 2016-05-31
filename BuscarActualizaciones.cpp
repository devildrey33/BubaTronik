#include "StdAfx.h"
#include "BuscarActualizaciones.h"
#include "Definiciones.h"
#include "ConfigBubatronik.h"
#include <wininet.h>
#include <wincrypt.h>
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);


HANDLE BuscarActualizaciones::Mutex = NULL;
bool   BuscarActualizaciones::_CancelarDescarga = false;

BuscarActualizaciones::BuscarActualizaciones(void) {
	hThreadDescargar = NULL;
	hThread = NULL;
}

BuscarActualizaciones::~BuscarActualizaciones(void) {
}


void BuscarActualizaciones::IniciarThreadBuscarActualizacion(HWND hWndDestino, const bool OmitirDia) {
	
	if (hThread == NULL) {
		SYSTEMTIME T;
		GetLocalTime(&T);
        if (OmitirDia == false) {
		    if (Sistema.App.Config.BuscarActualizaciones_UltimaVez.wDay == T.wDay) 
			    return;
        }
		Sistema.App.Config.BuscarActualizaciones_UltimaVez = T;
		hThread = CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->ThreadBuscarActualizacion, (void *)hWndDestino, 0, NULL);
		if (hThread) SetThreadPriority(hThread, 0);
	}
}

void BuscarActualizaciones::IniciarThreadDescargarActualizacion(HWND hWndDestino) {
	if (hThreadDescargar == NULL) {
        _CancelarDescarga = false;
        Mutex = CreateMutex(NULL, FALSE, TEXT("MutexActualizacion"));
		hThreadDescargar = CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->ThreadDescargarActualizacion, (void *)hWndDestino, 0, NULL);
		if (hThreadDescargar) SetThreadPriority(hThreadDescargar, 0);
	}
}


unsigned long BuscarActualizaciones::ThreadBuscarActualizacion(void *phWnd) {
//	try {
		HINTERNET		Sesion				= InternetOpen(TEXT("BubaTronik"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
		HINTERNET		Peticion			= InternetOpenUrl(Sesion, TEXT("http://www.devildrey33.es/BubaTronik/VERSION_BUBATRONIK.txt"), NULL, 0, INTERNET_FLAG_RELOAD, 0);
		char			Txt[32]				= "";
		DWORD			BytesLeidos			= 0;
		DWORD			TotalBytesLeidos	= 0;
		DWORD			MaxBuffer			= sizeof(Txt) -1;
		DWL::DWLString	Ret;
		// No se ha encontrado la version :/
		if (Peticion == NULL) {
    		InternetCloseHandle(Sesion);
			return 0;
		}

		BOOL Leido = InternetReadFile(Peticion, Txt, MaxBuffer, &BytesLeidos);
		if (BytesLeidos != 0) {
			Txt[BytesLeidos] = 0;
			Ret += Txt;
	//		ZeroMemory(Txt, sizeof(Txt) * sizeof(char));
		}
		HWND hWndPlayer = reinterpret_cast<HWND>(phWnd);

		if (Ret.Tam() == 0)	{
			InternetCloseHandle(Peticion);
			InternetCloseHandle(Sesion);
			return 0; 
		}
		if (Ret[0] == TEXT('<')) {
			InternetCloseHandle(Peticion);
			InternetCloseHandle(Sesion);
			return 0; // no se ha encontrado el documento
		}
		DWL::DWLString Version;
		Version.sprintf(TEXT("%.02f"), static_cast<float>(APP_NUM_VER));
		// La versión no es la misma, leemos las novedades
		if (Ret.SubStr(0, Version.Tam()) != Version)	{
    	
    		HINTERNET PeticionNovedades	= InternetOpenUrl(Sesion, TEXT("http://www.devildrey33.es/BubaTronik/NOVEDADES_BUBATRONIK.txt"), NULL, 0, INTERNET_FLAG_RELOAD, 0);
			DWORD	  TotalDatos        = 0;
    		DWORD	  Descargado		= 64;
			TCHAR	  TotalDatosStr[64];
			BOOL bRet = HttpQueryInfo(PeticionNovedades, HTTP_QUERY_CONTENT_LENGTH, (LPVOID)TotalDatosStr, &Descargado, (LPDWORD)0);
			if (bRet == TRUE) TotalDatos = _wtol(TotalDatosStr);
			char     *TmpBuffer = new char[TotalDatos + 1];

    		Leido = InternetReadFile(PeticionNovedades, TmpBuffer, TotalDatos, &BytesLeidos);
    		if (BytesLeidos != 0 && BytesLeidos <= TotalDatos) TmpBuffer[BytesLeidos] = 0;
			Sistema.App.PlayerEx.VentanaActualizacion.Novedades(TmpBuffer);
			delete [] TmpBuffer;

			PostMessage(hWndPlayer, MENSAJE_ACTUALIZACION_ENCONTRADA, 0, 0);
		}
		InternetCloseHandle(Peticion);
		InternetCloseHandle(Sesion);
//	}
//	catch (...) {
//	}
	return 0;
}


unsigned long BuscarActualizaciones::ThreadDescargarActualizacion(void *phWnd) {
	
	TCHAR			szHead[]			= TEXT("Accept: */*\r\n\r\n");
	HINTERNET		Sesion				= InternetOpen(TEXT("BubaTronik"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
	HINTERNET		Peticion			= InternetOpenUrl(Sesion, TEXT("http://www.devildrey33.es/BubaTronik/Instalar.exe"), szHead, 0, INTERNET_FLAG_RELOAD, 0);
//	DWORD			Longitud			= 0;
	DWORD			Descargado			= 64;
	DWORD           TotalDescargado		= 0;
	char		    Datos[4097];
	DWORD			TotalDatos			= 0;
	TCHAR			TotalDatosStr[64];
	BOOL Ret = HttpQueryInfo(Peticion, HTTP_QUERY_CONTENT_LENGTH, (LPVOID)TotalDatosStr, &Descargado, (LPDWORD)0);
	if (Ret == TRUE) TotalDatos = _wtol(TotalDatosStr);
	
	HWND hWndPlayer = reinterpret_cast<HWND>(phWnd);
	PostMessage(hWndPlayer, MENSAJE_ACTUALIZACION_MAXIMOBARRA, NULL, static_cast<LPARAM>(TotalDatos));
	Descargado = 0;
	DWL::DWLString PathFinal; // = Sistema.App.AppPath(); 
	Sistema.Directorio.AppData(PathFinal);
	PathFinal += TEXT("\\BubaTronik\\Instalar.exe");
	DWL::Archivos::DWLArchivoBinario Archivo(PathFinal(), true);
	
	while (TRUE) {
		PostMessage(hWndPlayer, MENSAJE_ACTUALIZACION_POSICIONBARRA, NULL, static_cast<LPARAM>(TotalDescargado));
		WaitForSingleObject(Mutex, INFINITE);
        if (!InternetReadFile(Peticion, (LPVOID)Datos, 4096, &Descargado) || _CancelarDescarga == true) {
            ReleaseMutex(Mutex);
			break;
		}
        ReleaseMutex(Mutex);
		Datos[Descargado] = '\0';
		TotalDescargado += Descargado;
		if (Descargado == 0)	break;
		else					Archivo.Guardar(Datos, Descargado);
	}
    
	
	InternetCloseHandle(Peticion);
	// Leemos el hash que tiene la web
	Peticion = InternetOpenUrl(Sesion, TEXT("http://www.devildrey33.es/BubaTronik/Instalar.exe.hash"), szHead, 0, INTERNET_FLAG_RELOAD, 0);
	char  TxtHash[33]	= "";
	DWORD BytesLeidos	= 0;
	BOOL  Leido			= InternetReadFile(Peticion, TxtHash, 32, &BytesLeidos);

	// Calculo el hash del archivo descargado
#define MD5LEN  16
	DWORD	sz = Archivo.Posicion(0, true);
	HCRYPTPROV hProv = 0,hHash = 0;
	BYTE	rgbHash[MD5LEN + 1] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    DWORD	cbHash = 0;
    char	finalhash[33] = "", dig[] = "0123456789abcdef";
    BYTE   *hash = new BYTE[sz];
	size_t  l = 0;
	Archivo.Posicion(0, false);
	Archivo.Leer(hash, sz);

 	CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);
	CryptHashData(hHash, hash, sz, 0);
	cbHash = MD5LEN;
	BOOL RET = CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0);
    for(DWORD i = 0; i < cbHash; i ++){
        finalhash[l] = dig[rgbHash[i] >> 4];
        l ++;
        finalhash[l] = dig[rgbHash[i] & 0xf];
        l ++;
    }
	     
    for(l = 32; l < strlen(finalhash); l++)	finalhash[l] = 0;
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
	delete [] hash;

	if (_strcmpi(finalhash, TxtHash) != 0)
		TotalDescargado ++; // Si no son iguales sumo 1 a los bytes descargados para retornar false

	InternetCloseHandle(Peticion);


	InternetCloseHandle(Sesion);


//    ReleaseMutex(Mutex);
    CloseHandle(Mutex);

	if (_CancelarDescarga == false) PostMessage(hWndPlayer, MENSAJE_ACTUALIZACION_FINDESCARGA, NULL, static_cast<LPARAM>(TotalDescargado == TotalDatos));
	return TRUE;
}

void BuscarActualizaciones::TerminarThreadDescargarActualizacion(void) {
	WaitForSingleObject(Mutex, INFINITE);
    _CancelarDescarga = true;
    ReleaseMutex(Mutex);
}

/*
// Get size of file with handle hOpenUrl
static DWORD getInetFileSize(HINTERNET hOpenUrl)
{
#define FILEDATA_SIZE 32
   DWORD lenbufsiz = FILEDATA_SIZE;
   TCHAR filedata[FILEDATA_SIZE];
   BOOL nRc;
   DWORD filesize = 0;

   nRc = HttpQueryInfo(hOpenUrl, HTTP_QUERY_CONTENT_LENGTH, (LPVOID)filedata, &lenbufsiz, (LPDWORD)0);
   if(nRc == TRUE) // Get size of file
      filesize = atol(filedata);
   return(filesize);
}
*/


