#include "StdAfx.h"
#include "SistemaMedia.h"
#include "resource.h"
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);
//#include "SistemaVLC.h"
//#include "SistemaFMOD.h"


SistemaMedia::SistemaMedia(void) {
	MedioActual = NULL;
	Actual = 0;
	_EnStop = false;
}


SistemaMedia::~SistemaMedia(void) {
}


const bool SistemaMedia::Play(void) {
	if (Sistema.App.ListaC.TotalItems() == 0) return false;

//	SetTimer(Sistema.App.PlayerEx.hWnd(), TIMER_ANALISIS, 200, NULL);
	Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, __FUNCTION__, TEXT("Reproduciendo medio."));

	if (Actual < 0) Actual = 0;
	if (MedioActual == NULL) AbrirPrimerMedioLista(true);
	Sistema.App.PlayerEx.Oscy.IniciarThread();
	bool Ret = false;
	if (Sistema.App.ListaC.ItemMedio(Actual)->Medio != NULL) { 
		#if defined UTILIZAR_FMOD
			switch (Sistema.App.ListaC.ItemMedio(Actual)->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = Sistema.App.FMod.Play();
					break;
				case Tipo_Medio_Video :
					Ret = Sistema.App.VLC.Play();
					SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, TRUE);
					break;
			}
		#else
			if (Sistema.App.ListaC.ItemMedio(Actual)->Medio->TipoMedio == Tipo_Medio_Video) SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,FALSE,NULL,TRUE);
			Ret = Sistema.App.VLC.Play();
		#endif
	}

	_EnStop = false;
	ReemplazarIcono(!Ret);
	if (Ret == true) {
		Sistema.App.PlayerEx.Botones.Boton_Play_Desactivado(true);
		Sistema.App.PlayerEx.Botones.Boton_Pausa_Desactivado(false);
		Sistema.App.PlayerEx.Botones.Boton_Stop_Desactivado(false, true);

		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, false);
		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, true);
		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, true);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_P);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_N);

		Sistema.App.ControlesMP.Botones.Boton_Play_Desactivado(true);
		Sistema.App.ControlesMP.Botones.Boton_Pausa_Desactivado(false);
		Sistema.App.ControlesMP.Botones.Boton_Stop_Desactivado(false, true);
	}
	Volumen(Sistema.App.Config.Volumen);
	return Ret;

}


const bool SistemaMedia::Pausa(void) {
	if (Sistema.App.ListaC.TotalItems() == 0) return false;
	Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, __FUNCTION__, TEXT("Pausando el medio."));
	if (Actual < 0) Actual = 0;
	bool Ret = false;
	if (Sistema.App.ListaC.ItemMedio(Actual)->Medio != NULL) { 
		#if defined UTILIZAR_FMOD
			switch (Sistema.App.ListaC.ItemMedio(Actual)->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = Sistema.App.FMod.Pausa();
					break;
				case Tipo_Medio_Video :
					Sistema.App.PlayerEx.Botones.Activado(true);
					Ret = Sistema.App.VLC.Pausa();
					break;
			}
		#else
			Sistema.App.PlayerEx.Botones.Activado(true);
			Ret = Sistema.App.VLC.Pausa();
		#endif
	}
	ReemplazarIcono(!Ret);
	if (Ret == true) {
		Sistema.App.PlayerEx.Botones.Boton_Play_Desactivado(false);
		Sistema.App.PlayerEx.Botones.Boton_Pausa_Desactivado(true);
		Sistema.App.PlayerEx.Botones.Boton_Stop_Desactivado(false, true);

		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, true);
		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, false);
		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, true);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_P);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_N);

		Sistema.App.ControlesMP.Botones.Boton_Play_Desactivado(false);
		Sistema.App.ControlesMP.Botones.Boton_Pausa_Desactivado(true);
		Sistema.App.ControlesMP.Botones.Boton_Stop_Desactivado(false, true);
	}
	return Ret;
}


const bool SistemaMedia::Stop(void) {
	if (Sistema.App.ListaC.TotalItems() == 0) return false;
	Sistema.App.PlayerEx.Oscy.Terminar();
	Sistema.App.PlayerEx.Oscy.PintarFondoVacio();
	_EnStop = true;
	if (Actual < 0) Actual = 0;
	bool Ret = false;
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, TRUE); // activo el protector de pantalla
	if (Sistema.App.ListaC.ItemMedio(Actual)->Medio != NULL) { 
		Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, __FUNCTION__, TEXT("Parando el medio."));
		#if defined UTILIZAR_FMOD
			switch (Sistema.App.ListaC.ItemMedio(Actual)->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = Sistema.App.FMod.Stop();
					break;
				case Tipo_Medio_Video :
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ATRAS, true);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ADELANTE, true);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, true);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, true);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, true);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_N);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_N);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_N);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_N);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_N);
					Sistema.App.PlayerEx.Botones.Activado(true);
					Sistema.App.ControlesMP.Botones.Activado(true);
					Ret = Sistema.App.VLC.Stop();
					break;
			}
		#else
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ATRAS, true);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ADELANTE, true);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, true);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, true);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, true);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_N);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_N);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_N);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_N);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_N);
			Sistema.App.PlayerEx.Botones.Activado(true);
			Sistema.App.ControlesMP.Botones.Activado(true);
			Ret = Sistema.App.VLC.Stop();
		#endif
	}
	int t = Sistema.App.ListaC.TotalItems();
	ReemplazarIcono(!Ret);
	if (Ret == true) {
		Sistema.App.PlayerEx.Botones.Boton_Play_Desactivado(false);
		Sistema.App.PlayerEx.Botones.Boton_Pausa_Desactivado(false);
		Sistema.App.PlayerEx.Botones.Boton_Stop_Desactivado(true, true);

		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, true);
		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, true);
		Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, false);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_P);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_N);
		Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_N);

		Sistema.App.ControlesMP.Botones.Boton_Play_Desactivado(false);
		Sistema.App.ControlesMP.Botones.Boton_Pausa_Desactivado(false);
		Sistema.App.ControlesMP.Botones.Boton_Stop_Desactivado(true, true);
	}

	DWL::DWLString Titulo;
	Titulo.sprintf(TEXT("%s %s"), APP_NOMBRE, APP_STR_VERSION);
	Sistema.App.PlayerEx.Titulo(Titulo());

	return Ret;
}


const bool SistemaMedia::Siguiente(void) {
//	if (Actual == -1) return false;
	if (Sistema.App.ListaC.TotalItems() == 0) return false;
	Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, __FUNCTION__, TEXT("Pasando al siguiente medio de la lista."));
	if (Actual < -1) Actual = -1;
	Actual ++;
	if (Actual >static_cast<int>(Sistema.App.ListaC.TotalItems() -1)) Actual = 0;
	bool Ret = AbrirMedio(Sistema.App.ListaC.ItemMedio(Actual));
	if (Ret == true) Play();
	return Ret;

}


const bool SistemaMedia::Anterior(void) {
	if (Sistema.App.ListaC.TotalItems() == 0) return false;
//	Stop();
	Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, __FUNCTION__, TEXT("Pasando al anterior medio de la lista."));
	Actual --;
	if (Actual <= -1) Actual = Sistema.App.ListaC.TotalItems() -1;
	bool Ret = AbrirMedio(Sistema.App.ListaC.ItemMedio(Actual));
	if (Ret == true) Play();
	return Ret;
}


//void SistemaMedia::Asignar_Subtitulos_MedioActual(const TCHAR *Path) {
//}

const bool SistemaMedia::AbrirMedio(ItemListaMedios *nMedio) {
	WaitForSingleObject(Sistema.App.PlayerEx.Oscy.Mutex, INFINITE);
	EliminarMedioActual();

	Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, __FUNCTION__, nMedio->Medio->Nombre());
	

	bool Ret = false;
	MedioActual = nMedio;
	//TCHAR PathPrueba[MAX_PATH];
	//GetShortPathName(Sistema.App.ListaC.ItemMedio(Actual)->Medio->Path(), PathPrueba, MAX_PATH);
	if (Sistema.App.ListaC.ItemMedio(Actual)->Medio != NULL) { 
		MedioActual->Medio = Sistema.App.ListaC.ItemMedio(Actual)->Medio;
		#if defined UTILIZAR_FMOD
			switch (Sistema.App.ListaC.ItemMedio(Actual)->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = Sistema.App.FMod.AbrirAudio(Sistema.App.ListaC.ItemMedio(Actual)->Medio);
					break;
				case Tipo_Medio_Video :
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ATRAS, false);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ADELANTE, false);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, false);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, false);
					Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, false);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_P);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_P);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_P);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_P);
					Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_P);
					Sistema.App.PlayerEx.Botones.Activado(false);
					Sistema.App.ControlesMP.Botones.Activado(false);
					Ret = Sistema.App.VLC.AbrirMedio(Sistema.App.ListaC.ItemMedio(Actual)->Medio->Path());
					if (Ret == false) ReemplazarIconoVideo(true);
					break;
			}
		#else
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PLAY, IDI_PLAY_P);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_PAUSA, IDI_PAUSA_P);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_STOP, IDI_STOP_P);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ATRAS, IDI_ATRAS_P);
			Sistema.App.PlayerEx.BarraTareas.Boton_Icono(ID_BOTON_CANCION_ADELANTE, IDI_ADELANTE_P);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ATRAS, false);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_ADELANTE, false);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PLAY, false);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_PAUSA, false);
			Sistema.App.PlayerEx.BarraTareas.Boton_Activado(ID_BOTON_CANCION_STOP, false);
			Sistema.App.PlayerEx.Botones.Activado(false);
			Sistema.App.ControlesMP.Botones.Activado(false);
			Ret = Sistema.App.VLC.AbrirMedio(Sistema.App.ListaC.ItemMedio(Actual)->Medio->Path());
			if (Ret == false) ReemplazarIconoVideo(true);
		#endif
		
		if (Ret == true) 	Sistema.App.BD.AgregarMedioReciente(nMedio->Medio);

	}

	ReleaseMutex(Sistema.App.PlayerEx.Oscy.Mutex);
	MedioActual->Existe = Ret;
	return Ret;
}


const bool SistemaMedia::AbrirPrimerMedioLista(const bool NoPlay) {
	Actual = 0;
//	Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, __FUNCTION__, TEXT("Pasando al anterior medio de la lista."));
	Stop();
	if (Sistema.App.ListaC.TotalItems() == 0) return false;
	bool Ret = AbrirMedio(Sistema.App.ListaC.ItemMedio(Actual));
	if (Ret == true && NoPlay == false) Play();
	return Ret;
}


void SistemaMedia::EliminarMedioActual(void) {
	Stop();
	if (MedioActual != NULL) {
		if (MedioActual->Medio != NULL)	{
			#if defined UTILIZAR_FMOD
				switch (MedioActual->Medio->TipoMedio) {
					case Tipo_Medio_Audio :
					case Tipo_Medio_CDAudio :
						Sistema.App.FMod.EliminarCancionActual();
						break;
					case Tipo_Medio_Video :
						Sistema.App.VLC.EliminarMedioActual();
						break;
				}
			#else
				Sistema.App.VLC.EliminarMedioActual();
			#endif
			// Parte para descargas con extension crdownload
			TipoArchivo_Datos *Datos = Sistema.App.Tipos.BuscarTipoArchivo(MedioActual->Medio->Path());
			if (Datos != NULL) {
				if (Datos->ID == EXT_CRDOWNLOAD) {
					size_t i = 0;
					for (i = MedioActual->Medio->Path.Tam(); i > 0; i--) {
						if (MedioActual->Medio->Path[i] == TEXT('.')) {
							break;
						}
					}
					DWL::DWLString NuevoPath = MedioActual->Medio->Path.SubStr(0, i).Texto();
					// Probamos de renombrar el archivo sin la extension crdownload
					BOOL Ret = MoveFile(MedioActual->Medio->Path(), NuevoPath());
					if (Ret == FALSE) { // error
						static VentanaMensaje_Aceptar Msg;
						// NO SE AÑADE ESTE MENSAJE AL IDIOMA PORQUE ES EXPERIMENTAL
						Msg.MostrarMensaje(TEXT("Error intentado quitar la extensión crdownload"), IDIOMA__ERROR);
					}
					else { // reasignamos el nombre en la BD
						MedioActual->Medio->Path = NuevoPath;
					}
				}
			}
		}
	}
	MedioActual = NULL;
}

Estados_Medio SistemaMedia::ComprobarEstado(void) {
	if (_EnStop == true) return EnStop;
	if (MedioActual == NULL) return EnStop;
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					return Sistema.App.FMod.CancionActual()->ComprobarEstado();
				case Tipo_Medio_Video :
					return Sistema.App.VLC.ComprobarEstado();
			}
		#else
			return Sistema.App.VLC.ComprobarEstado();
		#endif
	}
	return SinCargar;
}


void SistemaMedia::ReemplazarIcono(const bool Error) {
	DWL::DWLString Titulo;
	if (MedioActual == NULL) {
		Titulo.sprintf(TEXT("%s %s"), APP_NOMBRE, APP_STR_VERSION);
		Sistema.App.PlayerEx.Titulo(Titulo());
		return;
	}
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					if (Sistema.App.FMod.CancionActual() != NULL)	Sistema.App.FMod.CancionActual()->ReemplazarIcono(Error);
					break;
				case Tipo_Medio_Video :
					ReemplazarIconoVideo(Error);
					break;
			}
		#else
			ReemplazarIconoVideo(Error);
		#endif

		switch (MedioActual->Medio->TipoMedio) {
			case Tipo_Medio_Audio :
				Titulo.sprintf(TEXT("%s %s [%d - %s]"), APP_NOMBRE, APP_STR_VERSION, static_cast<TMedioCDAudio *>(MedioActual->Medio)->Pista, MedioActual->Medio->Nombre());
				break;
			case Tipo_Medio_CDAudio :
				Titulo.sprintf(TEXT("%s %s [%d - %s]"), APP_NOMBRE, APP_STR_VERSION, static_cast<TMedioAudio *>(MedioActual->Medio)->Pista, MedioActual->Medio->Nombre());
				break;
			case Tipo_Medio_Video :
				Titulo.sprintf(TEXT("%s %s [%d - %s]"), APP_NOMBRE, APP_STR_VERSION, static_cast<TMedioVideo *>(MedioActual->Medio)->Pista, MedioActual->Medio->Nombre());
				break;
		}
		Sistema.App.PlayerEx.Titulo(Titulo());
	}
}

void SistemaMedia::SumarReproduccion(void) {
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Sistema.App.FMod.CancionActual()->SumarEscuchada();
					break;
				case Tipo_Medio_Video :
					MedioActual->Medio->Reproducido ++;
					break;
			}
		#else
			MedioActual->Medio->Reproducido ++;
		#endif
	}
}

const UINT64 SistemaMedia::TiempoTotal(void) {
	UINT64 Ret = 0;
	if (MedioActual == NULL) return 0;
	if (ComprobarEstado() == EnStop) return 0;
	if (MedioActual->Medio != NULL) {
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = static_cast<DWORD>(Sistema.App.FMod.TiempoTotal());
					break;
				case Tipo_Medio_Video :
					Ret = Sistema.App.VLC.TiempoTotal();
					break;
			}
		#else
			Ret = Sistema.App.VLC.TiempoTotal();
		#endif
	}
	return Ret;
}

const UINT64 SistemaMedia::TiempoActual(void) {
	UINT64 Ret = 0;
	if (MedioActual == NULL) return 0;
	if (MedioActual->Medio != NULL) {
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = static_cast<DWORD>(Sistema.App.FMod.TiempoActual());
					break;
				case Tipo_Medio_Video :
					Ret = static_cast<DWORD>(Sistema.App.VLC.TiempoActual() * 10000);
					// Tiempo 0 en el video, actualizamos el tiempo
					UINT64 TmpTiempo;
					TmpTiempo = Sistema.App.VLC.TiempoTotal();
					if (MedioActual->Medio->Tiempo != TmpTiempo) {
						MedioActual->Medio->Tiempo = TmpTiempo;
						MedioActual->Texto(2, TiempoStrL(MedioActual->Medio->Tiempo));
						Sistema.App.ListaC.ActualizarTodo(true);
					}
					break;
			}
		#else
			Ret = static_cast<DWORD>(Sistema.App.VLC.TiempoActual() * 10000);
		#endif
	}
	return Ret;
}


const UINT64 SistemaMedia::TiempoActualMs(void) {
	UINT64 Ret = 0;
	if (MedioActual == NULL) return 0;
	if (ComprobarEstado() == EnStop) return 0;
	if (MedioActual->Medio != NULL) {
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = static_cast<DWORD>(Sistema.App.FMod.TiempoActual());
					break;
				case Tipo_Medio_Video :
					Ret = static_cast<DWORD>(Sistema.App.VLC.TiempoActualMs());
					// Tiempo 0 en el video, actualizamos el tiempo
					UINT64 TmpTiempo;
					TmpTiempo = Sistema.App.VLC.TiempoTotal();
					if (MedioActual->Medio->Tiempo != TmpTiempo) {
						MedioActual->Medio->Tiempo = TmpTiempo;
						MedioActual->Texto(2, TiempoStrL(MedioActual->Medio->Tiempo));
						Sistema.App.ListaC.ActualizarTodo(true);
					}
					break;
			}
		#else
			Ret = static_cast<DWORD>(Sistema.App.VLC.TiempoActualMs());
		#endif
	}
	return Ret;
}

void SistemaMedia::TiempoActual(const DWORD nTiempo) {
	if (MedioActual == NULL) return;
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Sistema.App.FMod.TiempoActual(nTiempo);
					break;
				case Tipo_Medio_Video :
					Sistema.App.VLC.TiempoActual(static_cast<float>(nTiempo / 10000.0f));
					break;
			}
		#else
			Sistema.App.VLC.TiempoActual(static_cast<float>(nTiempo / 10000.0f));
		#endif
	}
}

void SistemaMedia::Volumen(const int nVolumen) {
	int Vol = nVolumen;
	if (Vol > 100)	Vol = 100;
	if (Vol < 0)	Vol = 0;
	Sistema.App.Config.Volumen = Vol;
	Sistema.App.PlayerEx.Volumen.Valor(Vol);
	Sistema.App.ControlesMP.Volumen.Valor(Vol);
	if (MedioActual == NULL) return;
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Sistema.App.FMod.Volumen(static_cast<float>(Vol) / 100.0f);
					break;
				case Tipo_Medio_Video :
					Sistema.App.VLC.Volumen(Vol * 2);
					break;
			}
		#else
			Sistema.App.VLC.Volumen(Vol * 2);
		#endif
	}
}

const int SistemaMedia::Volumen(void) {
	if (MedioActual == NULL) return Sistema.App.Config.Volumen;
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			switch (MedioActual->Medio->TipoMedio) {
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					return static_cast<int>(Sistema.App.FMod.Volumen() * 100.0f);
				case Tipo_Medio_Video :
					return static_cast<int>(Sistema.App.VLC.Volumen() / 2);
			}
		#else
			return static_cast<int>(Sistema.App.VLC.Volumen() / 2);
		#endif
	}
	return Sistema.App.Config.Volumen;
}


void SistemaMedia::ReemplazarIconoVideo(const bool Error) {
	if (MedioActual == NULL)		return;
	if (MedioActual->Medio == NULL) return;
	// Para la BD
	Estados_Medio St = ComprobarEstado();
	ArbolBaseDatos *BDTree = &Sistema.App.ArbolBaseDatos;
	int TotalNodos = BDTree->TotalNodosLineales();
	for (int i = TotalNodos -1; i > 0 ; i--) {
		if (BDTree->NodoLinealMedio(i)->Medio == MedioActual->Medio) {
			if (Error == false) {
				switch (St) {
					case Terminada :
					case EnStop :
						BDTree->NodoLinealMedio(i)->Icono(Sistema.App.BD.MiraIconoVideo(MedioActual->Medio->Reproducido), false);  
						break;
					case EnPlay :
					case SinCargar :
						BDTree->NodoLinealMedio(i)->Icono(IDI_PLAY, false);  
						break;
					case EnPausa :
						BDTree->NodoLinealMedio(i)->Icono(IDI_PAUSA, false);  
						break;
				}
			}
			else { // La cancion no existe o esta mal.
				BDTree->NodoLinealMedio(i)->Icono(IDI_CERRAR_N, false);
			}
		}
	}
//	BDTree->FinAgregarNodos();
	BDTree->Repintar(true);
	// Para la Lista
	if (Error == false) {
		switch (St) {
			case Terminada :
			case EnStop :
				if (MedioActual->Medio->Item != NULL) MedioActual->Medio->Item->Icono(Sistema.App.BD.MiraIconoVideo(MedioActual->Medio->Reproducido), true);  
				break;
			case EnPlay :
			case SinCargar : // Se esta cargando, por lo tanto es play
				if (MedioActual->Medio->Item != NULL) MedioActual->Medio->Item->Icono(IDI_PLAY, true);  
				break;
			case EnPausa :
				if (MedioActual->Medio->Item != NULL) MedioActual->Medio->Item->Icono(IDI_PAUSA, true);  
				break;
		}
	}
	else {
		if (MedioActual->Medio->Item != NULL) MedioActual->Medio->Item->Icono(IDI_CERRAR_N, true); 
	}
}


const TCHAR *SistemaMedia::TiempoStrL(UINT64 TMS) {                                      //
	tm temp;
	temp.tm_hour = static_cast<int>(((TMS / 1000) / 60) / 60);
	temp.tm_min = static_cast<int>(((TMS / 1000) / 60) - (temp.tm_hour * 60));
	temp.tm_sec = static_cast<int>((TMS / 1000) - (temp.tm_min * 60) - ((temp.tm_hour * 60) * 60));
	swprintf(_TmpTxt, 256, TEXT("%.2d:%.2d:%.2d"), temp.tm_hour, temp.tm_min, temp.tm_sec);
	return _TmpTxt;	
}

const TCHAR *SistemaMedia::TiempoStr(UINT64 TMS) {                                      //
	tm temp;
	temp.tm_hour = static_cast<int>(((TMS / 1000) / 60) / 60);
	temp.tm_min = static_cast<int>(((TMS / 1000) / 60) - (temp.tm_hour * 60));
	temp.tm_sec = static_cast<int>((TMS / 1000) - (temp.tm_min * 60) - ((temp.tm_hour * 60) * 60));
	swprintf(_TmpTxt, 256, TEXT("%.2d:%.2d"), temp.tm_min, temp.tm_sec);
	  
	return _TmpTxt;	
}                                                                        //


void SistemaMedia::RateAdelanteX4(void) {
	if (MedioActual == NULL) return;
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			if (MedioActual->Medio->TipoMedio == Tipo_Medio_Video) Sistema.App.VLC.RateAdelanteX4();
		#else
			Sistema.App.VLC.RateAdelanteX4();
		#endif
	}
}

void SistemaMedia::RateAtrasX4(void) {
	if (MedioActual == NULL) return;
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			if (MedioActual->Medio->TipoMedio == Tipo_Medio_Video) Sistema.App.VLC.RateAtrasX4();
		#else
			Sistema.App.VLC.RateAtrasX4();
		#endif
	}
}

void SistemaMedia::RateNormal(void) {
	if (MedioActual == NULL) return;
	if (MedioActual->Medio != NULL)	{
		#if defined UTILIZAR_FMOD
			if (MedioActual->Medio->TipoMedio == Tipo_Medio_Video) Sistema.App.VLC.RateNormal();
		#else
			Sistema.App.VLC.RateNormal();
		#endif
	}
}