#pragma once

#include "BaseDatos.h"
#include "ListaCanciones.h"


class SistemaMedia {
public:
						SistemaMedia(void);
				       ~SistemaMedia(void);
 const bool				Play(void);
 const bool				Pausa(void);
 const bool				Stop(void);
 const bool				Siguiente(void);
 const bool				Anterior(void);
 const bool				AbrirMedio(ItemListaMedios *nMedio);
 const bool				AbrirPrimerMedioLista(const bool NoPlay = false);
 void					EliminarMedioActual(void);
 Estados_Medio			ComprobarEstado(void);
 void					ReemplazarIcono(const bool Error);
 void					SumarReproduccion(void);
 const UINT64			TiempoActual(void);
 const UINT64			TiempoActualMs(void);
 void					TiempoActual(const DWORD nTiempo);
 const UINT64			TiempoTotal(void);
 int					Actual;
 ItemListaMedios       *MedioActual;
 const int				Volumen(void);
 void					Volumen(const int nVolumen);
 const TCHAR		   *TiempoStrL(UINT64 TMS); // Funcion que retorna a partir de un valor en milisegundos una cadena de texto de este estilo [hh:mm:ss]
 const TCHAR		   *TiempoStr(UINT64 TMS);  // Funcion que retorna a partir de un valor en milisegundos una cadena de texto de este estilo [mm:ss]
 void					RateAdelanteX4(void);
 void					RateAtrasX4(void);
 void					RateNormal(void);
// void					ExaminarStreamVideo(UINT &nAncho, UINT &nAlto, UINT64 &Tiempo);
// void					Asignar_Subtitulos_MedioActual(const TCHAR *Path);
private:
 void					ReemplazarIconoVideo(const bool Error);
 bool				   _EnStop;
 TCHAR			       _TmpTxt[256];
 bool				   _RateModificado;
 float				   _RateOriginal;

 //TMedia	   *MedioActual;
};
