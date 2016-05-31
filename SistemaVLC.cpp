#include "StdAfx.h"
#include "SistemaVLC.h"
#include <DWLDebug.h>
#include "BubaTronikApp.h"
#include "VentanaMensaje.h"
#include "resource.h"

SistemaVLC::SistemaVLC(void) {
	_Instancia			= NULL;
	_MediaPlayer		= NULL;
	_Log				= NULL;
	_RateModificado		= false;
	_Comprobacion       = 0;
}

SistemaVLC::~SistemaVLC(void) {
	TerminarInstancia();
}


void SistemaVLC::TerminarInstancia(void) {
	if (_Log != NULL) {
		libvlc_log_close(_Log);
		_Log = NULL;
	}
	if (_MediaPlayer != NULL) {
		libvlc_media_player_stop(_MediaPlayer);
		libvlc_media_player_release(_MediaPlayer);
		_MediaPlayer = NULL;
	}
	if (_Instancia != NULL) {
		libvlc_release(_Instancia);
		_Instancia = NULL;
	}
}


const bool SistemaVLC::IniciarInstanciaVLC(void) {
	
	DWL::DWLString PluginPath;
	PluginPath.sprintf(TEXT("--plugin-path=%s%s"), Sistema.App.AppPath(), PATHVLC_PLUGINS);

	ArgumentosVLC ArgsVLC;
	ArgsVLC.AgregarArgumento(Sistema.App.AppPath());	// Path de la aplicacion
	ArgsVLC.AgregarArgumento(PluginPath());				// Path del directorio de plugins
	ArgsVLC.AgregarArgumento("--no-video-title-show");	// No mostrar el titulo del video

//	ArgsVLC.AgregarArgumento("-H");	// No mostrar el titulo del video
//	ArgsVLC.AgregarArgumento("--hotkeys-mousewheel-mode=0");
//	ArgsVLC.AgregarArgumento("--vout-event=2");			// solo va en linux.. deberia desactivar que al hacer dobleclick se ponga en fullscreen
//	ArgsVLC.AgregarArgumento("global-key-play-pause=32");
//	ArgsVLC.AgregarArgumento("--extraintf=logger");
//	ArgsVLC.AgregarArgumento("--vvv");
//	ArgsVLC.AgregarArgumento("--visual-80-bands");
//	ArgsVLC.AgregarArgumento("--file-logging");
//	ArgsVLC.AgregarArgumento("--logfile=c:\\LogVLC.txt");
//	ArgsVLC.AgregarArgumento("--logmode=text");


	_Instancia = libvlc_new(ArgsVLC.TotalArgumentos(), ArgsVLC());

	_Log = libvlc_log_open(_Instancia); // PARTE DEL LOG PARA EL VLC
	libvlc_set_log_verbosity(_Instancia, 2);

	return true;
}


const bool SistemaVLC::AbrirMedio(const TCHAR *Path) {
	
	char	Destino[2048];
	size_t  TamnTexto = wcslen(Path);
	int		TamRes = WideCharToMultiByte(CP_UTF8, NULL, Path, TamnTexto, Destino, 2048, NULL, NULL);
	Destino[TamRes] = 0;	// finalizo el string porque en la version RELEASE de WideCharToMultiByte no se pone el terminador de cadenas, en la debug si....

	EliminarMedioActual();
	int Existe = GetShortPathName(Path, NULL, NULL);
	if (Existe == 0) {
		return false;
	}
	libvlc_media_t *Media = NULL;
	Media = libvlc_media_new_path(_Instancia, Destino);
	libvlc_media_parse(Media);
//	libvlc_media_add_option(Media, "sout=#description:dummy"); // para obtener datos extras del medio

	_MediaPlayer = libvlc_media_player_new_from_media(Media);

	Sistema.App.SliderTiempo.Maximo(10000);
	Sistema.App.SliderTiempo.Valor(0);

	Sistema.App.ControlesMP.SliderTiempo.Maximo(10000);
	Sistema.App.ControlesMP.SliderTiempo.Valor(0);

	libvlc_media_release(Media);
	libvlc_media_player_set_hwnd(_MediaPlayer, Sistema.App.MediaPlayer.hWnd());
	Sistema.App.VentanaLogs.AgregarMensaje(Log_LibVLC, Log_Info, __FUNCTION__, IDIOMA__LA_OPERACION_SE_HA_COMPLETADO_CORRECTAMENTE);

	return true;
}



const int SistemaVLC::Asignar_Subtitulos_MedioActual(const TCHAR *Path) {
	char	Destino[2048];
	size_t  TamnTexto = wcslen(Path);
	int		TamRes = WideCharToMultiByte(CP_UTF8, NULL, Path, TamnTexto, Destino, 2048, NULL, NULL);
	Destino[TamRes] = 0;
	return libvlc_video_set_subtitle_file(_MediaPlayer, Destino);
}

const bool SistemaVLC::Play(void) {
	
	if (_MediaPlayer != NULL) {
		_Comprobacion = 1;
		if (Sistema.App.Media.MedioActual->Medio->TipoMedio == Tipo_Medio_Video) {
			if (Sistema.App.MediaPlayer.Visible() == FALSE) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.PlayerEx.CP.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(true);
				Sistema.App.PlayerEx.OpcionesBasicas.DesSeleccionarTodo();
				Sistema.App.PlayerEx.OpcionesBasicas.Nodo(OPCIONES_BASICAS_VER_VIDEO)->Seleccionado(true, true);
			}
			if (ComprobarEstado() != EnPausa)	Sistema.App.MediaPlayer.HackVLCWNDPROC(true); // Si no esta en pausa activo el timer del hack
/*
            libvlc_video_set_logo_int(MediaPlayer, 0, 0); // enalbe a 1
            MostrarError();	
            libvlc_video_set_logo_string(MediaPlayer, 1, "h:\\p.png"); // ruta del logo
            MostrarError();	
        //    libvlc_video_set_logo_int(MediaPlayer, 2, 100); // pos X a 100
        //    libvlc_video_set_logo_int(MediaPlayer, 3, 100); // pos y a 100
            libvlc_video_set_logo_int(MediaPlayer, 6, 100); // opacidad a 100
            MostrarError();	
            libvlc_video_set_logo_int(MediaPlayer, 7, 10); // posicion abajo derecha
            MostrarError();	
//            libvlc_video_set_logo_int(MediaPlayer, 0, 1); // enalbe a 1
            MostrarError();	*/

		}		
		int Ret2 = libvlc_media_player_play(_MediaPlayer);
//		Ret2 = libvlc_media_player_play(_MediaPlayer);
		Sistema.App.VentanaLogs.AgregarMensaje(Log_LibVLC, Log_Info, __FUNCTION__, IDIOMA__LA_OPERACION_SE_HA_COMPLETADO_CORRECTAMENTE);
		return true;
	}
	return false;
}


const bool SistemaVLC::Pausa(void) {
	
	if (_MediaPlayer != NULL) {
		if (ComprobarEstado() == EnPlay) libvlc_media_player_pause(_MediaPlayer);
		Sistema.App.VentanaLogs.AgregarMensaje(Log_LibVLC, Log_Info, __FUNCTION__, IDIOMA__LA_OPERACION_SE_HA_COMPLETADO_CORRECTAMENTE);
		return true;
	}
	return false;
}


const bool SistemaVLC::Stop(void) {
	
	if (_MediaPlayer != NULL) {
		Sistema.App.MediaPlayer.HackVLCWNDPROC(false);
		libvlc_media_player_stop(_MediaPlayer);
		Sistema.App.VentanaLogs.AgregarMensaje(Log_LibVLC, Log_Info, __FUNCTION__, IDIOMA__LA_OPERACION_SE_HA_COMPLETADO_CORRECTAMENTE);
		return true;
	}
	return false;
}


void SistemaVLC::EliminarMedioActual(void) {
	
	if (_MediaPlayer != NULL) {
		Stop();

		libvlc_media_player_release(_MediaPlayer);
		_MediaPlayer = NULL;
	}
}

const BOOL SistemaVLC::ComprobarError(void) {
	static VentanaMensaje_Aceptar MsgWnd;
	UINT TotalMensajes = 0;
	BOOL bError = FALSE;
	DWL::DWLString StrMsg;
	DWL::DWLString StrMsgchar;
	libvlc_log_message_t   msg;
	libvlc_log_message_t  *pmsg;
	libvlc_log_iterator_t *IteratorLog = libvlc_log_get_iterator(_Log);
	while (libvlc_log_iterator_has_next(IteratorLog)) {
		pmsg = libvlc_log_iterator_next(IteratorLog, &msg);
		if (msg.i_severity == 1) {
			StrMsgchar = msg.psz_message;
			Sistema.App.VentanaLogs.AgregarMensaje(Log_LibVLC, static_cast<LogSeveridad>(msg.i_severity),  msg.psz_name, StrMsgchar(), true);
			StrMsg += StrMsgchar; // conversion a unicode
			StrMsg += TEXT("\n");
			bError = TRUE;
		}
//		AgregarMensaje(Log_LibVLC, static_cast<LogSeveridad>(msg.i_severity),  msg.psz_name, StrMsg(), false);
		TotalMensajes ++;
	}
	libvlc_log_iterator_free(IteratorLog);
	libvlc_log_clear(_Log);
	if (StrMsg.Tam() > 0) {
		MsgWnd.MostrarMensaje(StrMsg(), IDIOMA__ERROR, 0);
//		Sistema.App.Media.Stop();
	}
//	Sistema.App.PlayerEx.Botones.Activado(true);
	Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ATRAS, true);
	Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ADELANTE, true);
	Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, false);
	Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, true);
	Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, true);
	Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_P);
	Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_N);
	Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_N);
	Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_N);
	Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_N);
	Sistema.App.PlayerEx.Botones.Activado(true);
	Sistema.App.ControlesMP.Botones.Activado(true);

	return bError;
}

Estados_Medio SistemaVLC::ComprobarEstado(void) {
	if (_MediaPlayer == NULL) return SinCargar;
	libvlc_state_t Estado = libvlc_media_player_get_state(_MediaPlayer);
	switch (Estado) {
		case libvlc_NothingSpecial :
		case libvlc_Opening :
		case libvlc_Buffering :
			return SinCargar;
		case libvlc_Playing :
			if (_Comprobacion == 1) {
				libvlc_media_player_pause(_MediaPlayer);
				libvlc_media_player_play(_MediaPlayer);
				_Comprobacion = 2;
				return SinCargar;
			}
			// Miramos el log para ver si hay errores
			else if (_Comprobacion < 10) {
				_Comprobacion ++;
				if (ComprobarError() != 0) 
					return EnError;
			}
			return EnPlay;
		case libvlc_Stopped :
			return EnStop;
		case libvlc_Paused :
			return EnPausa;
//	#if (LIBVLC_NUM_VERSION == 100)
		case libvlc_Ended :
			return Terminada;
//	#endif
		case libvlc_Error:
			return EnError;
	}
	return SinCargar;
}


const UINT64 SistemaVLC::TiempoTotal(void) {
	if (_MediaPlayer != NULL) {
		return static_cast<UINT64>(libvlc_media_player_get_length(_MediaPlayer));
	}
	return 0;
}

const UINT64 SistemaVLC::TiempoActualMs(void) {
	if (_MediaPlayer != NULL) {
		return static_cast<UINT64>(libvlc_media_player_get_time(_MediaPlayer));
	}
	return 0;
}


const float SistemaVLC::TiempoActual(void) {
	if (_MediaPlayer != NULL) {
		return libvlc_media_player_get_position(_MediaPlayer);
	}
	return 0.0f;
}

void SistemaVLC::TiempoActual(float nTiempo) {
	if (_MediaPlayer != NULL) {
		libvlc_media_player_set_position(_MediaPlayer, nTiempo);
	}
}

void SistemaVLC::Volumen(int nVolumen) {
	if (_MediaPlayer != NULL) {
		libvlc_audio_set_volume(_MediaPlayer, nVolumen);
	}
}

const int SistemaVLC::Volumen(void) {
	if (_MediaPlayer != NULL) {
		return libvlc_audio_get_volume(_MediaPlayer);
	}
	return 0;
}

void SistemaVLC::RateAdelanteX4(void) {
	if (_RateModificado == false) {
		libvlc_media_player_set_rate(_MediaPlayer, 8.0f);
		_RateModificado = true;
	}
}

void SistemaVLC::RateAtrasX4(void) {
	// Los rates negativos no funcionan
	//libvlc_media_player_set_rate(MediaPlayer, -4.0f);
}

void SistemaVLC::RateNormal(void) {
	if (_RateModificado == true) {
		_RateModificado = false;
		libvlc_media_player_set_rate(_MediaPlayer, 1.0f);
	}
}
