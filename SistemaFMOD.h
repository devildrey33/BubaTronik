#pragma once

#include "FMOD.hpp"
#include "FMOD_ERRORS.h"
#include "CancionFMOD.h"

#if defined UTILIZAR_FMOD
	#pragma comment(lib, "fmodexp_vc.lib")


		// Clase destinada a contener todas las funciones del sistema del FMOD, 
		// es decir todas aquellas que no esten relacionadas con la cancion en si.

		class SistemaFMOD {
		public:	
		 // Constructor y destructor
								SistemaFMOD(void);
							   ~SistemaFMOD(void);

		 // Funciones de inicio y fin necesarias para el FMOD
		 const bool				Iniciar(const bool Reconfigurando = false);
		 void                   Terminar(void);

		 // Funciones basicas para el Reproductor
		 const bool				Play(void);
		 const bool				Pausa(void);
		 const bool				Stop(void);

		 const bool	            AbrirAudio(TMedio *nCancion);

		 inline CancionFMOD    *CancionActual(void)		{ 
									return _CancionActual; 
								};


		 void					Volumen(float fVolumen);
		 const float			Volumen(void);

		 float                 *PillaSpectrum(const int NumBarritas, float *ArraySpectrum);

		 const UINT64			TiempoActual(void);
		 void					TiempoActual(unsigned int Act);
		 const UINT64			TiempoTotal(void);


		 void					MostrarError(FMOD_RESULT FmodErrorCode);
		 
		 const TCHAR		   *Version(void);

		 inline FMOD::System   *Sistema_FMOD(void) {
									return _System;
								};

		 inline void            EliminarCancionActual(void) {
		//							this->Stop();
									if (_CancionActual != NULL) delete _CancionActual;
									_CancionActual = NULL;
								};

		 // Miembros privados
		private: 
		 // Cancion Actual en la lista
		 float                 _EQ[11];
		// int                   _Actual;
		 CancionFMOD          *_CancionActual;
		// CancionFMOD          *_CancionSiguiente;
		 FMOD::System         *_System;
		 FMOD::DSP	          *_Dsp[11];

		 TCHAR                 _TmpTxt[2048];	// no es muy bonito... pero resulta mas facil ademas esta clase en un futuro desaparecera...
												// y trasladare la chapuza a SistemaMedia...
												// Tambien podria hacer que la funcion requiriera el TCHAR en ve de retornarlo.. pero 
												// no vendra de [256 * sizeof(TCHAR)] de memoria y resulta mas comodo usar funciones que retornen un puntero a TCHAR
												// que no hacer que reciban la variable.

//		friend class BDListView;
		};

#endif