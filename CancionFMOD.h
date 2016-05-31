#pragma once
#include "BaseDatos.h"

#define LineasOscyFMOD 1024

#if defined UTILIZAR_FMOD

class CancionFMOD {
public:
							CancionFMOD(void);
//							CancionFMOD(TMedio *Cancion, FMOD::System *System);
	                       ~CancionFMOD(void);
 const bool					AbrirCancion(TMedio *Cancion, FMOD::System *System);
 const bool					Play(void);
 void						Pausa(void);
 void						Stop(void);
 void						Volumen(float Vol);
 const float				Volumen(void);
 void						TiempoActual(unsigned int Pos);
 const UINT64				TiempoActual(void);
 const UINT64				TiempoTotal(void);
 float			           *PillaSpectrum(const int NumBarritas, float *ArraySpectrum);
 void                       SumarEscuchada(void);
 void                       ReemplazarIcono(const bool Error);
 void						RateAdelanteX4(void);
 void						RateAtrasX4(void);
 void						RateNormal(void);
 inline TMedio             *Medio(void) { 
								return _Medio; 
							};
 inline Estados_Medio		Estado(void) {
								return _Estado;
							};
 Estados_Medio				ComprobarEstado(void);
private:
 TMedio                   *_Medio;
 float                     _ArraySpectrumL[LineasOscyFMOD];
 float                     _ArraySpectrumR[LineasOscyFMOD];
 FMOD::Sound	     	  *_Stream;
 FMOD::Sound	     	  *_PistaCD;
 FMOD::Channel	          *_Channel;
 FMOD::System             *_System;
 Estados_Medio			   _Estado;

 friend class SistemaFMOD;
};

#endif