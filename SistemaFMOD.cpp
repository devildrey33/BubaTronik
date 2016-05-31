#include "stdafx.h"
#include "SistemaFMOD.h"

#if defined UTILIZAR_FMOD

#include <time.h>
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);

SistemaFMOD::SistemaFMOD(void) {
	_CancionActual = NULL;
//	_CancionSiguiente = NULL;
}


SistemaFMOD::~SistemaFMOD(void) {
}


void SistemaFMOD::Terminar(void) {
	if (_CancionActual != NULL)		delete _CancionActual;
	_CancionActual = NULL;
	_System->close();
	_System->release();
}


const bool SistemaFMOD::Iniciar(const bool Reconfigurando) {
	FMOD_RESULT FmodErrorCode = FMOD_OK;
	_System->setDriver(Sistema.App.Config.FMod_TargetaSonido);
	_System->setOutput(Sistema.App.Config.FMod_Driver);
	if (FmodErrorCode != FMOD_OK) MostrarError(FmodErrorCode);

	FmodErrorCode = FMOD::System_Create(&_System);
	if (FmodErrorCode != FMOD_OK) MostrarError(FmodErrorCode);
	// Obtengo la version del fmod
	unsigned int VersionFMOD = 0;
	_System->getVersion(&VersionFMOD);
	// Asigno el buffer de la cancion
	FMOD_RESULT R = _System->setStreamBufferSize(Sistema.App.Config.FMod_Buffer, FMOD_TIMEUNIT_MS); // suposo k sera el Stream_SetBuifferSize	

	// Obtengo el path de los pluguins
	DWL::DWLString CodecsPath = Sistema.App.AppPath(); CodecsPath += TEXT("Plugins\\FMOD");
	// Paso el path de unicode a ascii
	char CodecsPathAscii[MAX_PATH + 1] = "";
	WideCharToMultiByte(CP_OEMCP, WC_NO_BEST_FIT_CHARS, CodecsPath(), static_cast<int>(CodecsPath.Tam()) + 1, CodecsPathAscii, MAX_PATH + 1, NULL, NULL);
	FmodErrorCode = _System->setPluginPath(CodecsPathAscii);
	if (FmodErrorCode != FMOD_OK) MostrarError(FmodErrorCode);
	// Inicio el sonido con 16 canales
	FmodErrorCode = _System->init(16, FMOD_INIT_NORMAL, NULL);
	if (FmodErrorCode != FMOD_OK) MostrarError(FmodErrorCode);

	// Inicio el Equalizador
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[0]);
    _Dsp[0]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 80.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[1]);
    _Dsp[1]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 170.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[2]);
    _Dsp[2]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 320.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[3]);
    _Dsp[3]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 600.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[4]);
    _Dsp[4]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 1000.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[5]);
    _Dsp[5]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 3000.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[6]);
    _Dsp[6]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 6000.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[7]);
    _Dsp[7]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 12000.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[8]);
    _Dsp[8]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 14000.0f);
	_System->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &_Dsp[9]);
    _Dsp[9]->setParameter(FMOD_DSP_PARAMEQ_CENTER, 16000.0f);
	

	if (Reconfigurando == false) Sistema.App.Media.Actual = -1;

	if (FMOD_OK != FmodErrorCode) {
		return false;
	}
	return true;
}


const TCHAR *SistemaFMOD::Version(void) {
	TCHAR TmpTxt[2048];
	unsigned int VersionFMOD = 0;
	_System->getVersion(&VersionFMOD);
	swprintf_s(TmpTxt, 2047, TEXT("%x"), VersionFMOD);
    swprintf_s(_TmpTxt, 2047, TEXT("%c.%c%c.%c%c"), TmpTxt[0], TmpTxt[1], TmpTxt[2], TmpTxt[3], TmpTxt[4]);
	return _TmpTxt;
}



void SistemaFMOD::MostrarError(FMOD_RESULT FmodErrorCode) {
	wchar_t TxtW[2048];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, FMOD_ErrorString(FmodErrorCode), -1, TxtW, 2048);
	DWL::DWLString ErrTxt;
	ErrTxt.sprintf(TEXT("Error iniciando la libreria FMOD : (%s)\nEs muy probable que la libreria FMOD.DLL no se encuentre en el directorio raiz de este reproductor.\nEste error es grave, y el reproductor debe terminar la ejecucion.\nReinstala el reproductor para solucionar el problema."), TxtW);
	VentanaMensaje_Aceptar MsgErr(ErrTxt(), TEXT("ERROR Iniciando el sonido!!"), 0);
}



const bool SistemaFMOD::Play(void) {
	if (_CancionActual != NULL) {
		return _CancionActual->Play();
	}
	return false;
}



const bool SistemaFMOD::Pausa(void) {
	if (_CancionActual != NULL) {
		_CancionActual->Pausa();
		return true;
	}
	return false;
}



const bool SistemaFMOD::Stop(void) {
	if (_CancionActual != NULL) {
		_CancionActual->Stop();
		return true;
	}
	return false;
}




void SistemaFMOD::Volumen(float fVolumen) {
	if (_CancionActual != NULL) {
		_CancionActual->Volumen(fVolumen);
	}
}

const float SistemaFMOD::Volumen(void) {
	if (_CancionActual != NULL) {
		return _CancionActual->Volumen();
	}
	return 0.0f;
}


const UINT64 SistemaFMOD::TiempoActual(void) {
	unsigned int Tmp = 0;
	if (_CancionActual != NULL) _CancionActual->_Channel->getPosition(&Tmp, FMOD_TIMEUNIT_MS);
	return static_cast<UINT64>(Tmp);
}

void SistemaFMOD::TiempoActual(unsigned int Act) {
	if (_CancionActual != NULL) _CancionActual->_Channel->setPosition(Act, FMOD_TIMEUNIT_MS);
}

const UINT64 SistemaFMOD::TiempoTotal(void) {
	UINT64 Tmp = 0;
	if (_CancionActual != NULL) {
		Tmp = _CancionActual->TiempoTotal();
	}
	return Tmp;
}


const bool SistemaFMOD::AbrirAudio(TMedio *nCancion) {
	bool Ret = false;
	WaitForSingleObject(Sistema.App.PlayerEx.Oscy.Mutex, INFINITE);
	if (_CancionActual != NULL) delete _CancionActual;
	_CancionActual = new CancionFMOD;
	Ret = _CancionActual->AbrirCancion(nCancion, _System);
	Sistema.App.SliderTiempo.Maximo(static_cast<long>(_CancionActual->TiempoTotal()));
	Sistema.App.SliderTiempo.Valor(0);

	Sistema.App.ControlesMP.SliderTiempo.Maximo(static_cast<long>(_CancionActual->TiempoTotal()));
	Sistema.App.ControlesMP.SliderTiempo.Valor(0);

	ReleaseMutex(Sistema.App.PlayerEx.Oscy.Mutex);
	return Ret;
}


float *SistemaFMOD::PillaSpectrum(const int NumBarritas, float *ArraySpectrum) {
	if (_CancionActual != NULL) return _CancionActual->PillaSpectrum(NumBarritas, ArraySpectrum);
	else						return NULL;
}


#endif