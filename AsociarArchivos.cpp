#include "StdAfx.h"
#include "AsociarArchivos.h"
#include <DWLRegistro.h>
#include "ConfigBubaTronik.h"
#include "TipoArchivo.h"
#include "BubaTronikApp.h"

#define TOTAL_EXTENSIONES_VIDEO 26
#define TOTAL_EXTENSIONES_AUDIO 4

AsociarArchivos::AsociarArchivos(void) {
}

AsociarArchivos::~AsociarArchivos(void) {
}

#define AUDIO_DESCRIPCION	IDIOMA__ARCHIVO_DE_AUDIO
#define VIDEO_DESCRIPCION	IDIOMA__ARCHIVO_DE_VIDEO
#define ACCION_ADD			IDIOMA__ANADIR_A_LISTA
#define ACCION_REP			IDIOMA__REPRODUCIR
#define ARGUMENTOS_REP		TEXT("-r")
#define ARGUMENTOS_ADD		TEXT("")
#define NOMBRE_BACKUP		TEXT("BubaTronik.Backup")
#define KEY_PATH            TEXT("Software\\Classes\\")
#define CLAVE				HKEY_LOCAL_MACHINE

void AsociarArchivos::DesRegistrarApp(void) {
	DWL::DWLString Clave;
	DWL::DWLString Comp;
	TCHAR Backup[1024];

	// Necesito restaurar todos los tipos por defecto (por si se ha borrado alguno despues)
	Sistema.App.Tipos.TiposPorDefecto();

	// Informacion del Uninstall de windows
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("DisplayName"));
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("DisplayVersion"));
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("InstallLocation"));
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("Publisher"));
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("UninstallString"));
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("UninstallPath"));
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("URLInfoAbout"));
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("DisplayIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"), TEXT("BubaTronik"));

	// HKLM\Software\RegisteredApplications
	DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, TEXT("Software\\RegisteredApplications"), TEXT("BubaTronik"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\"), TEXT("InstallInfo"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\Capabilities\\"), TEXT("FileAssociations"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\"), TEXT("Capabilities"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\"), TEXT("BubaTronik"));

	// Borro las clases basicas de medios
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes"), TEXT("BubaTronik.Audio"));

	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes"), TEXT("BubaTronik.Video"));

	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes"), TEXT("BubaTronik.CDAudio"));

	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes"), TEXT("BubaTronik.Lista"));

	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT(""), TEXT("BubaTronik.Audio"));

	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT(""), TEXT("BubaTronik.Video"));

	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT(""), TEXT("BubaTronik.CDAudio"));

	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\Añadir a lista\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\"), TEXT("Añadir a lista"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\open\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\"), TEXT("open"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\Reproducir\\"), TEXT("command"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\"), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\"), TEXT("Shell"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\"), TEXT("DefaultIcon"));
	DWL::SO::DWLRegistro::EliminarClave(HKEY_CLASSES_ROOT, TEXT(""), TEXT("BubaTronik.Lista"));

	// Restauro copias de seguridad de las extensiones
	for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
		ZeroMemory(Backup, sizeof(TCHAR) * 1024);
		Clave.sprintf(TEXT("Software\\Classes\\.%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, Clave(), TEXT("BubaTronik.Backup"), Backup, 1024);
		if (Backup[0] != 0) {
			DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, Clave(), TEXT(""), Backup);
		}
		DWL::SO::DWLRegistro::EliminarValor(HKEY_LOCAL_MACHINE, Clave(), TEXT("BubaTronik.Backup"));
		ZeroMemory(Backup, sizeof(TCHAR) * 1024);
		Clave.sprintf(TEXT("\\.%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, Clave(), TEXT("BubaTronik.Backup"), Backup, 1024);
		if (Backup[0] != 0) {
			DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, Clave(), TEXT(""), Backup);
		}
		DWL::SO::DWLRegistro::EliminarValor(HKEY_CLASSES_ROOT, Clave(), TEXT("BubaTronik.Backup"));
		// Miro la selección de la asociación (win vista / 7)
		ZeroMemory(Backup, sizeof(TCHAR) * 1024);
		Clave.sprintf(TEXT("Software\\Microsoft\\Windows\\Current version\\Explorer\\FileExts\\.%s\\UserChoice"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, Clave(), TEXT("Progid"), Backup, 1024);
		Comp = Backup;
		if (Comp == TEXT("BubaTronik.Audio") || Comp == TEXT("BubaTronik.Video") || Comp == TEXT("BubaTronik.CDAudio") || Comp == TEXT("BubaTronik.Lista")) {
			Clave.sprintf(TEXT("Software\\Microsoft\\Windows\\Current version\\Explorer\\FileExts\\.%s"), Sistema.App.Tipos.Tipos[i]->Extension());
			DWL::SO::DWLRegistro::EliminarClave(HKEY_CURRENT_USER, Clave() ,TEXT("UserChoice"));

		}

	}

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
}



void AsociarArchivos::RegistrarApp(void) {
	DWL::DWLString TmpTxt, TmpTxt2;
	TCHAR Res[1024];
	TCHAR Backup[1024];
	// Información para la des-instalación
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("DisplayName"), TEXT("BubaTronik"));
	TmpTxt.sprintf(TEXT("%03f"), APP_NUM_VER);
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("DisplayVersion"), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("InstallLocation"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("Publisher"), TEXT("devildrey33"));
	TmpTxt.sprintf(TEXT("%sDesInstaladorBubaTronik.exe -EliminarBubaTronik"), Sistema.App.AppPath()); // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\BubaTronik
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("UninstallString"), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("UninstallPath"), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("URLInfoAbout"), TEXT("http://bubatronik.devildrey33.es"));
	TmpTxt.sprintf(TEXT("%sBubaTronik.exe,0"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\BubaTronik"), TEXT("DisplayIcon"), TmpTxt());

	// Parte que registra la aplicación para que salga en Panel de control -> Programas predeterminados.
	// HKLM\Software\RegisteredApplications
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\RegisteredApplications"), TEXT("BubaTronik"), TEXT("Software\\Clients\\Media\\BubaTronik\\Capabilities"));
	// HKLM\Software\Clients\Media\BubaTronik
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik"), TEXT(""), TEXT("BubaTronik media player"));
	// HKLM\Software\Clients\Media\BubaTronik\InstallInfo -> [SZ]HideIconsCommand
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" -EsconderIconos"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\InstallInfo"), TEXT("HideIconsCommand"), TmpTxt());
	// HKLM\Software\Clients\Media\BubaTronik\InstallInfo -> [SZ]ReinstallCommand
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" -InstalarRegistroWindows"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\InstallInfo"), TEXT("ReinstallCommand"), TmpTxt());
	// HKLM\Software\Clients\Media\BubaTronik\InstallInfo -> [SZ]ShowIconsCommand
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" -MostrarIconos"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\InstallInfo"), TEXT("ShowIconsCommand"), TmpTxt());
	// HKLM\Software\Clients\Media\BubaTronik\InstallInfo -> [DWORD]IconsVisible
	DWL::SO::DWLRegistro::AsignarValor_DWORD(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\InstallInfo"), TEXT("IconsVisible"), 1);

	// HKLM\Software\Clients\Media\BubaTronik\Capabilities -> ApplicationDescription
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\Capabilities"), TEXT("ApplicationDescription"), TEXT("Reproductor de medios basado en VLC."));
	// HKLM\Software\Clients\Media\BubaTronik\Capabilities -> ApplicationName
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\Capabilities"), TEXT("ApplicationName"), TEXT("BubaTronik media player"));
	// HKLM\Software\Clients\Media\BubaTronik\Capabilities\FileAssociations
	for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
		TmpTxt.sprintf(TEXT(".%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		switch (Sistema.App.Tipos.Tipos[i]->Tipo) {
			case Tipo_Medio_Audio	: TmpTxt2 = TEXT("BubaTronik.Audio");	break;
			case Tipo_Medio_Video	: TmpTxt2 = TEXT("BubaTronik.Video");	break;
			case Tipo_Medio_CDAudio : TmpTxt2 = TEXT("BubaTronik.CDAudio");	break;
			case Tipo_Medio_Lista	: TmpTxt2 = TEXT("BubaTronik.Lista");	break;
		}
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Clients\\Media\\BubaTronik\\Capabilities\\FileAssociations"), TmpTxt(), TmpTxt2());
	}

	// Parte que registra los tipos basicos de la aplicación : Audio, Video, CDAudio, Lista
	// Se hace en HKEY_LOCAL_MACHINE/Software/Clases y en HKEY_CLASSES_ROOT porque en windows 7 no son la misma clave....

	// Tipo_Medio_Audio
	// - Descripción
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio"), TEXT(""), TEXT("Archivo de Audio"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio"), TEXT(""), TEXT("Archivo de Audio"));
	// - Icono 53
	TmpTxt.sprintf(TEXT("%sBubaTronik.exe,53"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\DefaultIcon"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\DefaultIcon"), TEXT(""), TmpTxt());
	// - Shell
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell"), TEXT(""), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell"), TEXT(""), TEXT("Reproducir"));
	// - Shell -> Añadir a lista
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	// - Shell -> Open
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
	// - Shell -> Reproducir
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" -r \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());

	// Tipo_Medio_Video
	// - Descripción
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video"), TEXT(""), TEXT("Archivo de Video"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video"), TEXT(""), TEXT("Archivo de Video"));
	// - Icono 52
	TmpTxt.sprintf(TEXT("%sBubaTronik.exe,52"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\DefaultIcon"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\DefaultIcon"), TEXT(""), TmpTxt());
	// - Shell
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell"), TEXT(""), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell"), TEXT(""), TEXT("Reproducir"));
	// - Shell -> Añadir a lista
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	// - Shell -> Open
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
	// - Shell -> Reproducir
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" -r \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());
	
	// Tipo_Medio_Lista
	// - Descripción
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista"), TEXT(""), TEXT("Lista de medios"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista"), TEXT(""), TEXT("Lista de medios"));
	// - Icono 41
	TmpTxt.sprintf(TEXT("%sBubaTronik.exe,41"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\DefaultIcon"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\DefaultIcon"), TEXT(""), TmpTxt());
	// - Shell
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell"), TEXT(""), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell"), TEXT(""), TEXT("Reproducir"));
	// - Shell -> Añadir a lista
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	// - Shell -> Open
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
	// - Shell -> Reproducir
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" -r \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());

	// Tipo_Medio_CDAudio
	// - Descripción
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio"), TEXT(""), TEXT("Pista de Audio"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio"), TEXT(""), TEXT("Pista de Audio"));
	// - Icono 41
	TmpTxt.sprintf(TEXT("%sBubaTronik.exe,76"), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\DefaultIcon"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\DefaultIcon"), TEXT(""), TmpTxt());
	// - Shell
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell"), TEXT(""), TEXT("Reproducir"));
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell"), TEXT(""), TEXT("Reproducir"));
	// - Shell -> Añadir a lista
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\Añadir a lista\\command"), TEXT(""), TmpTxt());
	// - Shell -> Open
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
//	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Open\\command"), TEXT(""), TmpTxt());
	// - Shell -> Reproducir
	TmpTxt.sprintf(TEXT("\"%sBubaTronik.exe\" -r \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());
	DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\Reproducir\\command"), TEXT(""), TmpTxt());

	// Parte que enlaza las extensiones con los tipos de medio (Se guardara un backup para poder desinstalar)
	for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
		// Creo la copia de seguridad si no existe.
		ZeroMemory(Backup, sizeof(TCHAR) *1024);
		TmpTxt.sprintf(TEXT("Software\\Classes\\.%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TmpTxt(), TEXT(""), Backup, 1024);			// Obtengo el ultimo programa
		if (Backup[0] == 0) { // No hay backup, lo creamos
			ZeroMemory(Res, sizeof(TCHAR) *1024);
			DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TmpTxt(), TEXT(""), Res, 1024);			// Obtengo el ultimo programa
			DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TmpTxt(), TEXT("BubaTronik.Backup"), Res);
		}

		ZeroMemory(Backup, sizeof(TCHAR) *1024);
		TmpTxt.sprintf(TEXT(".%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TmpTxt(), TEXT(""), Backup, 1024);			// Obtengo el ultimo programa
		if (Backup[0] == 0) { // No hay backup, lo creamos
			DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TmpTxt(), TEXT(""), Res, 1024);			// Obtengo el ultimo programa
			DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TmpTxt(), TEXT("BubaTronik.Backup"), Res);
		}

		// Guardo la clase asociada
		switch (Sistema.App.Tipos.Tipos[i]->Tipo) {
			case Tipo_Medio_Audio	:	TmpTxt2 = TEXT("BubaTronik.Audio");		break;
			case Tipo_Medio_Video	:	TmpTxt2 = TEXT("BubaTronik.Video");		break;
			case Tipo_Medio_CDAudio :	TmpTxt2 = TEXT("BubaTronik.CDAudio");	break;
			case Tipo_Medio_Lista	:	TmpTxt2 = TEXT("BubaTronik.Lista");		break;
		}
		TmpTxt.sprintf(TEXT("Software\\Classes\\.%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, TmpTxt(), TEXT(""), TmpTxt2());

		TmpTxt.sprintf(TEXT(".%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, TmpTxt(), TEXT(""), TmpTxt2());

		// Activo la clase para windows vista / 7
		TmpTxt.sprintf(TEXT("Software\\Microsoft\\Windows\\Current version\\Explorer\\FileExts\\.%s\\UserChoice"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CURRENT_USER, TmpTxt(), TEXT("Progid"), TmpTxt2());
	}

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
}



const bool AsociarArchivos::ComprobarAsociaciones(void) {
	TCHAR Str[1024];
	DWL::DWLString Clave;
	DWL::DWLString Nombre;
	// Comprobamos si se ha re-asignado alguna extension
    for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
		ZeroMemory(Str, sizeof(TCHAR) * 1024);
		switch (Sistema.App.Tipos.Tipos[i]->Tipo) {
			case Tipo_Medio_Audio	:	Nombre = TEXT("BubaTronik.Audio");		break;
			case Tipo_Medio_Video	:	Nombre = TEXT("BubaTronik.Video");		break;
			case Tipo_Medio_CDAudio :	Nombre = TEXT("BubaTronik.CDAudio");	break;
			case Tipo_Medio_Lista	:	Nombre = TEXT("BubaTronik.Lista");		break;
		}
		Clave.sprintf(TEXT("Software\\Classes\\.%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, Clave(), TEXT(""), Str, 1024);
		if (Nombre != Str) 
			return false;
		Clave.sprintf(TEXT(".%s"), Sistema.App.Tipos.Tipos[i]->Extension());
		DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, Clave(), TEXT(""), Str, 1024);
		if (Nombre != Str) 
			return false;
	}

	// Comprobamos que las clases esten correctas (Audio, Video, CDAudio, Lista)
	Nombre.sprintf(TEXT("\"%sBubaTronik.exe\" \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\Añadir a lista\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;

	Nombre.sprintf(TEXT("\"%sBubaTronik.exe\" -r \"%%L\""), Sistema.App.AppPath());
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Audio\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Audio\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Video\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Video\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.CDAudio\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.CDAudio\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_LOCAL_MACHINE, TEXT("Software\\Classes\\BubaTronik.Lista\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;
	DWL::SO::DWLRegistro::ObtenerValor_String(HKEY_CLASSES_ROOT, TEXT("BubaTronik.Lista\\Shell\\Reproducir\\command"), TEXT(""), Str, 1024);
	if (Nombre != Str) 
		return false;

	return true;
}


// Si retorna false se añade por defecto
// Si retorna true se reproduce por defecto
const bool AsociarArchivos::ReproducirPorDefecto(void) {
	DWL::DWLString Str, Key;
	Key.sprintf(TEXT("%s%s.Video\\shell\\"), KEY_PATH, APP_NOMBRE);
	DWL::SO::DWLRegistro::ObtenerValor_String(CLAVE, Key(), TEXT(""), Str);				// HKCR\.Extension\NombreApp\shell\open\command(default)
	if (Str.Tam() > 0)	return true;
	else				return false;
}


void AsociarArchivos::AsignarReproducirPorDefecto(const bool Reproducir) {
	DWL::DWLString Key, Key2;
	DWL::DWLString KeyR, KeyR2;
	Key.sprintf(TEXT("%s%s.Video\\shell\\"), KEY_PATH, APP_NOMBRE);
	Key2.sprintf(TEXT("%s%s.Audio\\shell\\"), KEY_PATH, APP_NOMBRE);
	KeyR.sprintf(TEXT("%s.Video\\shell\\"), APP_NOMBRE);
	KeyR2.sprintf(TEXT("%s.Audio\\shell\\"), APP_NOMBRE);
	if (Reproducir == false) {
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, Key(), TEXT(""), TEXT(""));				// HKCR\.Extension\NombreApp\shell\open\command(default)
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, Key2(), TEXT(""), TEXT(""));				// HKCR\.Extension\NombreApp\shell\open\command(default)
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, KeyR(), TEXT(""), TEXT(""));				// HKCR\.Extension\NombreApp\shell\open\command(default)
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, KeyR2(), TEXT(""), TEXT(""));				// HKCR\.Extension\NombreApp\shell\open\command(default)
	}
	else {
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, Key(), TEXT(""), ACCION_REP);				// HKCR\.Extension\NombreApp\shell\open\command(default)
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_LOCAL_MACHINE, Key2(), TEXT(""), ACCION_REP);			// HKCR\.Extension\NombreApp\shell\open\command(default)
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, KeyR(), TEXT(""), ACCION_REP);				// HKCR\.Extension\NombreApp\shell\open\command(default)
		DWL::SO::DWLRegistro::AsignarValor_String(HKEY_CLASSES_ROOT, KeyR2(), TEXT(""), ACCION_REP);			// HKCR\.Extension\NombreApp\shell\open\command(default)
	}
}

