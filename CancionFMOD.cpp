#include "stdafx.h"
#include "CancionFMOD.h"
#include "resource.h"
//#include "InformacionArchivo.h"
#include "BubaTronikApp.h"
#include "TipoArchivo.h"

//DWL_APP(BubaTronikApp);

#if defined UTILIZAR_FMOD

CancionFMOD::CancionFMOD(void) {
	_System = NULL;
	_Channel = NULL;
	_Stream = NULL;
	_PistaCD = NULL;
	_Estado = SinCargar;
}

CancionFMOD::~CancionFMOD(void) {
	if (_Stream != NULL) {
		if (_PistaCD != NULL) {
			_PistaCD->release();
			_PistaCD = NULL;
		}
		_Stream->release();
		_Stream = NULL;
	}
}

const bool CancionFMOD::AbrirCancion(TMedio *nMedio, FMOD::System *System) {
	FMOD_RESULT		Error = FMOD_OK;
	bool			Ret = false;
	char			Letra[3]; 
	_PistaCD = NULL;
	DWL::DWLString TmpStr;
	TmpStr.sprintf(TEXT("Path : %s"), nMedio->Path());
	Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Info, __FUNCTION__, TmpStr());
	_System = System;
	_Medio = nMedio;
	if (nMedio->TipoMedio == Tipo_Medio_Audio) {
		Error = _System->createStream(reinterpret_cast<const char *>(nMedio->Path()), static_cast<FMOD_MODE>(FMOD_2D | FMOD_SOFTWARE | FMOD_ACCURATETIME | FMOD_LOOP_OFF | FMOD_UNICODE), 0, &_Stream);
	}
	else if (nMedio->TipoMedio == Tipo_Medio_CDAudio) { // CDROM
		char PathChar[MAX_PATH];
		WideCharToMultiByte(CP_ACP, WC_DEFAULTCHAR, _Medio->Path(), _Medio->Path.Tam(), PathChar, MAX_PATH, NULL, NULL);
		Letra[0] = static_cast<char>(_Medio->Path[0]);
		Letra[1] = ':';
		Letra[2] = '\0';
		Error = _System->createStream(reinterpret_cast<const char *>(Letra), static_cast<FMOD_MODE>(FMOD_2D | FMOD_SOFTWARE | FMOD_ACCURATETIME | FMOD_LOOP_OFF), 0, &_Stream);
		if (Error == FMOD_OK) Error = _Stream->getSubSound(static_cast<TMedioAudio *>(nMedio)->Pista -1, &_PistaCD); // no s'identifica la pista en els cds d'audio posible error en funcio Info
	}

	if (Error != FMOD_OK) { 
		Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Error, __FUNCTION__, TEXT("No se puede cargar la cancion!"));
		Stop();
	}
	else {
		Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Info, __FUNCTION__, TEXT("Cancion cargada correctamente"));
		_Estado = EnStop;
		Ret = true;
	}
	return Ret;
}



const bool CancionFMOD::Play(void) {
	FMOD_RESULT		Error	= FMOD_OK;
	bool			Pausado = false;
	float           fFreq, fVolumen, fPanning;


	if (_Estado != SinCargar) {
		_Channel->getPaused(&Pausado);
		if (Pausado == false) {
			if (_PistaCD == NULL)	{
				Error = _System->playSound(FMOD_CHANNEL_FREE, _Stream, true, &_Channel);
				_Stream->getVariations(&fFreq, &fVolumen, &fPanning);
			}
			else {
				Error = _System->playSound(FMOD_CHANNEL_FREE, _PistaCD, true, &_Channel);
			}
//			_Channel->setVolume(Sistema.App.Config.Volumen);

		}
		_Channel->setPaused(false);
//		Sistema.App.Player.MenuInicio.ActualizarSliderEQ();
		if (FMOD_OK != Error) {
			Stop();
			Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Info, __FUNCTION__, TEXT("Error al intentar reproducir la cancion!"));
			return false;
		}
		else {
			Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Info, __FUNCTION__, IDIOMA__LA_OPERACION_SE_HA_COMPLETADO_CORRECTAMENTE);
			_Estado = EnPlay;
			return true;
		}
	}
	Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Error, __FUNCTION__, TEXT("La canción no se ha cargado!"));
	return false;
}



void CancionFMOD::Pausa(void) {
	Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Info, __FUNCTION__, IDIOMA__LA_OPERACION_SE_HA_COMPLETADO_CORRECTAMENTE);
	if (_Estado != EnStop && _Estado != SinCargar) {
		_Channel->setPaused(true);
		_Estado = EnPausa;
	}
}



void CancionFMOD::Volumen(float Kual) {
	_Channel->setVolume(Kual);
}

const float CancionFMOD::Volumen(void) {
	float Ret = 0.0f;
	_Channel->getVolume(&Ret);
	return Ret;
}



void CancionFMOD::Stop(void) {
	Sistema.App.VentanaLogs.AgregarMensaje(Log_FMOD, Log_Info, __FUNCTION__, IDIOMA__LA_OPERACION_SE_HA_COMPLETADO_CORRECTAMENTE);
	_Channel->setPaused(false);
	_Channel->stop();
	_Estado = EnStop;
//	if (Sistema.App.FMod.CancionActual() != NULL) Sistema.App.FMod.CancionActual()->ReemplazarIcono();
//	for (int zz = 0; zz < 128; zz++) _ArraySpectrum[zz] = 0.0f;
}


float *CancionFMOD::PillaSpectrum(const int NumBarritas, float *ArraySpectrum) {
	if (_Estado == EnPlay) {
		_Channel->getSpectrum(_ArraySpectrumL, NumBarritas, 0,  FMOD_DSP_FFT_WINDOW_HAMMING);
		_Channel->getSpectrum(_ArraySpectrumR, NumBarritas, 1,  FMOD_DSP_FFT_WINDOW_HAMMING);
		for (int i = 0; i < NumBarritas; i++) {
			ArraySpectrum[i] = (_ArraySpectrumL[i] + _ArraySpectrumR[i]) / 2.0f; 
		}
		return ArraySpectrum;
	}
	else {
		return NULL;
	}
}


Estados_Medio CancionFMOD::ComprobarEstado(void) {
	bool Playing;
	_Channel->isPlaying(&Playing);
	if (Playing == false && _Estado != EnStop) {
		_Estado = Terminada;
	}
	return _Estado;
}



void CancionFMOD::SumarEscuchada(void) {
	unsigned int TiempoActual = 0;
	unsigned int TiempoTotal = 0;
	_Channel->getPosition(&TiempoActual, FMOD_TIMEUNIT_MS);
	if (_PistaCD == NULL)	_Stream->getLength(&TiempoTotal, FMOD_TIMEUNIT_MS);
	else					_PistaCD->getLength(&TiempoTotal, FMOD_TIMEUNIT_MS);
	if (TiempoActual >= TiempoTotal - 10) { 
		_Medio->Reproducido ++;
		if (_Medio->TipoMedio == Tipo_Medio_Audio) {
			static_cast<TMedioAudio *>(_Medio)->StrDisco->Escuchado ++;
			static_cast<TMedioAudio *>(_Medio)->StrGrupo->Escuchado ++;
			static_cast<TMedioAudio *>(_Medio)->StrGenero->Escuchado ++;
			static_cast<TMedioAudio *>(_Medio)->StrTipo->Escuchado ++;
		}
		_Medio->Tiempo = TiempoTotal;
//		_Medio->Item->Texto(2, Sistema.App.Media.TiempoStrL(TiempoTotal));
		Sistema.App.ListaC.ActualizarTodo();
   }
}


void CancionFMOD::ReemplazarIcono(const bool Error) {
	// Para la BD
	ArbolBaseDatos *BDTree = &Sistema.App.ArbolBaseDatos;
	int TotalNodos = BDTree->TotalNodosLineales();
	for (int i = TotalNodos -1; i > 0 ; i--) {
		if (BDTree->NodoLinealMedio(i)->Medio == _Medio) {
			switch (_Estado) {
				case Terminada :
				case EnStop :
					if (Error == false) {
						if		(_Medio->TipoMedio == Tipo_Medio_Audio)		BDTree->NodoLinealMedio(i)->Icono(Sistema.App.BD.MiraIconoCancion(_Medio->Reproducido), false);  
						else if (_Medio->TipoMedio == Tipo_Medio_CDAudio)	BDTree->NodoLinealMedio(i)->Icono(IDI_PISTAAUDIO, false);  
					}
					else				BDTree->NodoLinealMedio(i)->Icono(IDI_CERRAR_N, false);  
					break;
				case EnPlay :
					if (Error == false) BDTree->NodoLinealMedio(i)->Icono(IDI_PLAY, false);  
					else				BDTree->NodoLinealMedio(i)->Icono(IDI_CERRAR_N, false);  
					break;
				case EnPausa :
					if (Error == false) BDTree->NodoLinealMedio(i)->Icono(IDI_PAUSA, false);  
					else				BDTree->NodoLinealMedio(i)->Icono(IDI_CERRAR_N, false);  
					break;
			}
		}
	}
//	BDTree->FinAgregarNodos();
	BDTree->Repintar(true);
	// Para la Lista
	if (Error == false) {
		switch (_Estado) {
			case Terminada :
			case EnStop :
				if (_Medio->Item != NULL)   {
					if (_Medio->TipoMedio == Tipo_Medio_Audio)			_Medio->Item->Icono(Sistema.App.BD.MiraIconoCancion(_Medio->Reproducido), true);  
					else if (_Medio->TipoMedio == Tipo_Medio_CDAudio)	_Medio->Item->Icono(IDI_PISTAAUDIO, true);  
				}
				break;
			case EnPlay :
				if (_Medio->Item != NULL)   _Medio->Item->Icono(IDI_PLAY, true);  
				break;
			case EnPausa :
				if (_Medio->Item != NULL)   _Medio->Item->Icono(IDI_PAUSA, true);  
				break;
		}
	}
	else {
		if (_Medio->Item != NULL)   _Medio->Item->Icono(IDI_CERRAR_N, true);  
	}
}



const UINT64 CancionFMOD::TiempoActual(void) {
	unsigned int Tmp = 0;
	_Channel->getPosition(&Tmp, FMOD_TIMEUNIT_MS);
	return static_cast<UINT64>(Tmp);
}


void CancionFMOD::TiempoActual(unsigned int Act) {
	_Channel->setPosition(Act, FMOD_TIMEUNIT_MS);
}


const UINT64 CancionFMOD::TiempoTotal(void) {
	unsigned int Tmp = 0;
	if (_PistaCD == NULL)	_Stream->getLength(&Tmp, FMOD_TIMEUNIT_MS);
	else					_PistaCD->getLength(&Tmp, FMOD_TIMEUNIT_MS);
	return static_cast<UINT64>(Tmp);
}


void CancionFMOD::RateAdelanteX4(void) {
}

void CancionFMOD::RateAtrasX4(void) {
}

void CancionFMOD::RateNormal(void) {
}

#endif