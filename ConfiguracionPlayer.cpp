#include "StdAfx.h"
#include "ConfiguracionPlayer.h"
#include "BubaTronikApp.h"
#include "TipoArchivo.h"

//DWL_APP(BubaTronikApp);
//static BubaTronikApp &App = *reinterpret_cast<BubaTronikApp *>(DWL::DWLApp);

#define NOMBRE_FUENTE TEXT("TAHOMA")
//#include "ArchivoBinarioPlayer.h"

ConfiguracionPlayer::ConfiguracionPlayer(void) {
//	Sistema.App.AppPath();
}


ConfiguracionPlayer::~ConfiguracionPlayer(void) {
}

void ConfiguracionPlayer::ConfiguracionPorDefecto(void) {
	Volumen						= 100;
	FMod_Buffer					= 2000; 
	FMod_Driver					= FMOD_OUTPUTTYPE_AUTODETECT;
	FMod_TargetaSonido			= 0; 
//	Criterio_Busqueda			= Primero_Path;
//	Buscar_Path					= TEXT("C:\\");
	Abrir_Path					= TEXT("C:\\");
	Buscar_FiltroCaracteres		= true;
	Buscar_FiltroCoincidencias  = true;
	Buscar_OmitirTagsV1			= true;
	OrdenBD						= Orden_Grupo_Disco;
	TipoOscy					= Oscy_Un_Pixel;
	EQActivado					= false;
	Repeat						= Repeat_Desactivado;
	Shufle						= false;
	ExcluirNota1				= false;
	Inicio						= Inicio_Cualquier_Cosa;
	X							= 80;
	Y							= 80;
	Ancho						= 620;
	Alto						= 420;
	XLog						= 100;
	YLog						= 100;
	AnchoLog					= 740;
	AltoLog						= 400;
	AsociarArchivos				= Respuesta_Aceptar;
	Idioma_Path					= TEXT("IdiomaCastellano.dll");
	Combo_Inicio_Generar_Lista  = -1;

	Tecla_PlayPausa.AsignarTecla(VK_SPACE);
	Tecla_Stop.AsignarTecla(VK_END);
	Tecla_VolumenMas.AsignarTecla(107); // +
	Tecla_VolumenMenos.AsignarTecla(109); // -
	Tecla_Adelante.AsignarTecla(VK_RIGHT);
	Tecla_Atras.AsignarTecla(VK_LEFT);
	Tecla_Siguiente.AsignarTecla(VK_RIGHT, true);
	Tecla_Anterior.AsignarTecla(VK_LEFT, true);

	TecladoGlobal				= false;
	BuscarActualizaciones		= true;

	MostrarTagsEnToolTips		= true;
	OcultarMouseEnVideo			= 2000;
	MostrarSoloAudio			= false;
	TamFuente					= 13;
	SiempreDelante				= false;
	Skin_Path					= TEXT("BubaTronik_Azul");

	if (Sistema.VersionMayor() > 5)	UtilizarBarraTituloWindows  = true;  // Windows Vista o superior
	else							UtilizarBarraTituloWindows  = false; // Windows XP o inferior

	Sistema.App.FuentePeque.Crear(NOMBRE_FUENTE, 12, FW_NORMAL, false, false, false);
	Sistema.App.FuenteMediana.Crear(NOMBRE_FUENTE, 13, FW_NORMAL, false, false, false);
	Sistema.App.FuenteGrande.Crear(NOMBRE_FUENTE, 14, FW_NORMAL, false, false, false);
	Sistema.App.FuentePequeU.Crear(NOMBRE_FUENTE, 12, FW_NORMAL, true, false, false);
	Sistema.App.FuenteMedianaU.Crear(NOMBRE_FUENTE, 13, FW_NORMAL, true, false, false);
	Sistema.App.FuenteGrandeU.Crear(NOMBRE_FUENTE, 14, FW_NORMAL, true, false, false);

    Sistema.App.Tipos.TiposPorDefecto();
//    DirectoriosBuscarPorDefecto();
}

// Mover funcion a ControlPresentacionEx
void ConfiguracionPlayer::DirectoriosBuscarPorDefecto(void) {
    Buscar_Paths.resize(0);
}


const bool ConfiguracionPlayer::AgregarPathBuscar(const TCHAR *nPath, const OrdenEstructuraDirectorios TipoOrden, const bool PathAntesQueTag) {
    DWORD FA = GetFileAttributes(nPath);
    // Si no existe la ruta o no es un directorio retornamos false
    if (FA == INVALID_FILE_ATTRIBUTES || (FA & FILE_ATTRIBUTE_DIRECTORY) == 0) return false;

    for (size_t i = 0; i < Buscar_Paths.size(); i++) {
		if (Buscar_Paths[i].Path.Comparar(nPath) == true) return false;
    }
//    DWL::DWLString PathBuscar(nPath);
	DatosPathBuscar PathBuscar(nPath, TipoOrden, PathAntesQueTag);
    Buscar_Paths.push_back(PathBuscar);
    return true;
}


const bool ConfiguracionPlayer::EliminarPathBuscar(const TCHAR *nPath) {
    for (size_t i = 0; i < Buscar_Paths.size(); i++) {
        if (Buscar_Paths[i].Path.Comparar(nPath) == true) {
            Buscar_Paths.erase(Buscar_Paths.begin() +i);
            return true;
        }
    }
    return false;
}

// Si lee algo magicamente buscar en "C:\Users\devildrey33\AppData\Local\VirtualStore\ProgramData"
void ConfiguracionPlayer::Cargar(const TCHAR *Path) {
    bool bError = false;

	DWL::DWLString PathConfig;
	PathConfig.sprintf(TEXT("%s%s"), Path, TEXT("Config.BTR"));

	ArchivoBinarioPlayer Archivo;
	Archivo.AbrirArchivoLectura(PathConfig());
//	DWL::DWLDebug::MostrarUltimoError();
	UINT Tam = Archivo.Tam();
	float VersionConfig = 0.0f;
	try {
		if (Archivo.EstaAbierto() == true) {
			Archivo.LeerFloat(VersionConfig);
			// Solo para la version 0.61 que corte las versiones antiguas
			// Acordarme en la 0.62+ de quitar esto
			if (VersionConfig < 0.61f) {
				ConfiguracionPorDefecto();
				CargarFuentes();
				return;
			}

			Archivo.LeerInt(Volumen);
			Archivo.LeerInt(FMod_Buffer);
			Archivo.Leer(&FMod_Driver, sizeof(FMOD_OUTPUTTYPE));
			Archivo.LeerInt(FMod_TargetaSonido);
            if (Archivo.LeerString(Abrir_Path) == FALSE) bError = true;
            if (Archivo.LeerString(Idioma_Path) == FALSE) bError = true;
			Archivo.LeerBool(Buscar_FiltroCaracteres);
			Archivo.LeerBool(Buscar_FiltroCoincidencias);
			Archivo.LeerBool(Buscar_OmitirTagsV1);
			Archivo.Leer(&OrdenBD, sizeof(ORDEN_BASE_DATOS));
			Archivo.Leer(&TipoOscy, sizeof(TIPOS_OSCY));
			Archivo.LeerBool(EQActivado);
			Archivo.Leer(&Repeat, sizeof(TIPOS_REPEAT));
			Archivo.LeerBool(Shufle);
			Archivo.LeerBool(ExcluirNota1);
			Archivo.Leer(&Inicio, sizeof(TIPOS_INICIO));
			Archivo.LeerInt(X);
			Archivo.LeerInt(Y);
			Archivo.LeerInt(Ancho);
			Archivo.LeerInt(Alto);
			Archivo.Leer(&AsociarArchivos, sizeof(RespuestaVentanaMensaje));
			Archivo.LeerTecla(Tecla_PlayPausa);
			Archivo.LeerTecla(Tecla_Stop);
			Archivo.LeerTecla(Tecla_VolumenMas);
			Archivo.LeerTecla(Tecla_VolumenMenos);
			Archivo.LeerTecla(Tecla_Adelante);
			Archivo.LeerTecla(Tecla_Atras);
			Archivo.LeerTecla(Tecla_Siguiente);
			Archivo.LeerTecla(Tecla_Anterior);
			Archivo.LeerBool(TecladoGlobal);
			Archivo.LeerBool(BuscarActualizaciones);
			Archivo.LeerSystemTime(BuscarActualizaciones_UltimaVez);
			// 0.51c
			Archivo.LeerBool(MostrarTagsEnToolTips);
			Archivo.LeerInt(OcultarMouseEnVideo);
			Archivo.LeerBool(MostrarSoloAudio);
			Archivo.LeerInt(TamFuente);
			Archivo.LeerBool(SiempreDelante);
            if (Archivo.LeerString(Skin_Path) == FALSE) bError = true;
			if (Skin_Path.Tam() == 0) { // Si sale vacia es que es un archivo de la version 0.5b
				Skin_Path = TEXT("BubaTronik_Azul");
				MostrarTagsEnToolTips		= false;
				OcultarMouseEnVideo			= 2000;
				MostrarSoloAudio			= false;
				TamFuente					= 13;
				SiempreDelante				= false;
			}
			// 0.56+
			Archivo.LeerBool(UtilizarBarraTituloWindows);
            // 0.60+ // Intento cargar los tipos, si no existen creo los tipos por defecto
			if (Sistema.App.Tipos.CargarTipos(Archivo) == false)  { 
				Sistema.App.Tipos.TiposPorDefecto();
				Combo_Inicio_Generar_Lista = -1; // Si es el primer arranque de la 0.60 dejo la combo sin selección
			}
			else { // Los tipos existen seguimos leiendo la parte de la version 0.60
				// Leo las rutas donde el reproductor buscara
				size_t TotalBuscarPath = 0;
				Archivo.LeerSizeT(TotalBuscarPath);
				DWL::DWLString TmpStr;
				OrdenEstructuraDirectorios TmpOrden = Indefinido;
				bool TmpPathAntesQueTag;
				for (size_t i = 0; i < TotalBuscarPath; i++) {
					Archivo.LeerString(TmpStr);
					Archivo.Leer(&TmpOrden, sizeof(OrdenEstructuraDirectorios));
					Archivo.LeerBool(TmpPathAntesQueTag);
					AgregarPathBuscar(TmpStr(), TmpOrden, TmpPathAntesQueTag);
				}

				Archivo.LeerChar(Combo_Inicio_Generar_Lista);
				// Tamaño y posición de la ventana de log
				Archivo.LeerInt(XLog);
				Archivo.LeerInt(YLog);
				Archivo.LeerInt(AnchoLog);
				Archivo.LeerInt(AltoLog);
			}

		}
		else {
            bError = true;
		}
	}
	catch (...) {
        bError = true;
	}

    if (bError == true) {
	    ConfiguracionPorDefecto();
    }
	CargarFuentes();
}

void ConfiguracionPlayer::CargarFuentes(void) {
/*	Sistema.App.Tahoma13.Crear(NOMBRE_FUENTE, TamFuente, FW_NORMAL, false, false, false);
	Sistema.App.Tahoma13b.Crear(NOMBRE_FUENTE, TamFuente, FW_BOLD, false, false, false);
	Sistema.App.Tahoma13u.Crear(NOMBRE_FUENTE, TamFuente, FW_NORMAL, true, false, false);
	Sistema.App.Tahoma13bu.Crear(NOMBRE_FUENTE, TamFuente, FW_BOLD, true, false, false);
//	Sistema.App.Tahoma15b.CrearFuente(TEXT("Times new roman"), 21, true, false, false, false);
//	Sistema.App.Tahoma15.CrearFuente(NOMBRE_FUENTE, TamFuente + 2, false, false, false, false);
//	Sistema.App.Tahoma17b.CrearFuente(NOMBRE_FUENTE, TamFuente + 4, true, false, false, false);*/
	Sistema.App.Tahoma15b.Crear(NOMBRE_FUENTE, TamFuente + 2, FW_BOLD, false, false, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13.Crear(							NOMBRE_FUENTE, TamFuente, FW_NORMAL, false, false, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13_SubRayada.Crear(					NOMBRE_FUENTE, TamFuente, FW_NORMAL, false, false, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13_Cursiva.Crear(					NOMBRE_FUENTE, TamFuente, FW_NORMAL, true, false, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13_Cursiva_SubRayada.Crear(			NOMBRE_FUENTE, TamFuente, FW_NORMAL, true, true, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita.Crear(					NOMBRE_FUENTE, TamFuente, FW_BOLD, false, false, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita_Cursiva.Crear(			NOMBRE_FUENTE, TamFuente, FW_BOLD, true, false, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita_Cursiva_SubRayada.Crear(	NOMBRE_FUENTE, TamFuente, FW_BOLD, true, true, false);
	DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita_SubRayada.Crear(			NOMBRE_FUENTE, TamFuente, FW_BOLD, false, true, false);
	Sistema.App.ToolTip.Estilos.Fuentes.Normal.Crear(NOMBRE_FUENTE, TamFuente +2);
	Sistema.App.ToolTip.Estilos.Fuentes.Negrita.Crear(NOMBRE_FUENTE, TamFuente +2, FW_BOLD);
/*	
	DWL::SO::DWLEstilos::Fuentes.Tahoma13 = Sistema.App.Tahoma13;
	DWL::SO::DWLEstilos::Fuentes.Tahoma13 = Sistema.App.Tahoma13;
	DWL::SO::DWLEstilos::Fuentes.Tahoma13 = Sistema.App.Tahoma13;*/
//	DWL::SO::DWLEstilos::Fuentes.Crear(TEXT("Tahoma"), TamFuente);
}

void ConfiguracionPlayer::Guardar(const TCHAR *Path) {
	DWL::DWLString PathConfig = Path; PathConfig += TEXT("Config.BTR");
	
	ArchivoBinarioPlayer Archivo;
	Archivo.AbrirArchivoEscritura(PathConfig());
	float Ver = APP_NUM_VER;
	if (Archivo.EstaAbierto()) {
		// A partir de la 0.61 guardamos la versión al principio
		Archivo.GuardarFloat(Ver);
		Archivo.GuardarInt(Volumen);
		Archivo.GuardarInt(FMod_Buffer);
		Archivo.Guardar(&FMod_Driver, sizeof(FMOD_OUTPUTTYPE));
		Archivo.GuardarInt(FMod_TargetaSonido);
		Archivo.GuardarString(Abrir_Path);
		Archivo.GuardarString(Idioma_Path);
		Archivo.GuardarBool(Buscar_FiltroCaracteres);
		Archivo.GuardarBool(Buscar_FiltroCoincidencias);
		Archivo.GuardarBool(Buscar_OmitirTagsV1);
		Archivo.Guardar(&OrdenBD, sizeof(ORDEN_BASE_DATOS));
		Archivo.Guardar(&TipoOscy, sizeof(TIPOS_OSCY));
		Archivo.GuardarBool(EQActivado);
		if (Repeat == Repeat_Apagar_Equipo || Repeat == Repeat_Apagar_Reproductor) Repeat = Repeat_Desactivado;
		Archivo.Guardar(&Repeat, sizeof(TIPOS_REPEAT));
		Archivo.GuardarBool(Shufle);
		Archivo.GuardarBool(ExcluirNota1);
		Archivo.Guardar(&Inicio, sizeof(TIPOS_INICIO));
		Archivo.GuardarInt(X);
		Archivo.GuardarInt(Y);
		Archivo.GuardarInt(Ancho);
		Archivo.GuardarInt(Alto);
		Archivo.Guardar(&AsociarArchivos, sizeof(RespuestaVentanaMensaje));
		Archivo.GuardarTecla(Tecla_PlayPausa);
		Archivo.GuardarTecla(Tecla_Stop);
		Archivo.GuardarTecla(Tecla_VolumenMas);
		Archivo.GuardarTecla(Tecla_VolumenMenos);
		Archivo.GuardarTecla(Tecla_Adelante);
		Archivo.GuardarTecla(Tecla_Atras);
		Archivo.GuardarTecla(Tecla_Siguiente);
		Archivo.GuardarTecla(Tecla_Anterior);
		Archivo.GuardarBool(TecladoGlobal);
		Archivo.GuardarBool(BuscarActualizaciones);
		Archivo.GuardarSystemTime(BuscarActualizaciones_UltimaVez);

		Archivo.GuardarBool(MostrarTagsEnToolTips);
		Archivo.GuardarInt(OcultarMouseEnVideo);
		Archivo.GuardarBool(MostrarSoloAudio);
		Archivo.GuardarInt(TamFuente);
		Archivo.GuardarBool(SiempreDelante);
		Archivo.GuardarString(Skin_Path);
		// 0.57+
		Archivo.GuardarBool(UtilizarBarraTituloWindows);
        // 0.60+ 
		// Guardo los tipos de archivo
		Sistema.App.Tipos.GuardarTipos(Archivo);

        // Guardo los paths para el buscador
        size_t TotalBuscarPath = Buscar_Paths.size();
        Archivo.GuardarSizeT(TotalBuscarPath);
        for (TotalBuscarPath = 0; TotalBuscarPath < Buscar_Paths.size(); TotalBuscarPath++) {
			Archivo.GuardarString(Buscar_Paths[TotalBuscarPath].Path);
			Archivo.Guardar(&Buscar_Paths[TotalBuscarPath].TipoOrden, sizeof(OrdenEstructuraDirectorios));
			Archivo.GuardarBool(Buscar_Paths[TotalBuscarPath].PathAntesQueTag);
        }

		Archivo.GuardarChar(Combo_Inicio_Generar_Lista);
		// Tamaño y posición de la ventana de log
		Archivo.GuardarInt(XLog);
		Archivo.GuardarInt(YLog);
		Archivo.GuardarInt(AnchoLog);
		Archivo.GuardarInt(AltoLog);

	}
	Archivo.CerrarArchivo();
}