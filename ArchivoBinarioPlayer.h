#pragma once

#include "FMOD.hpp"
#include <DWLArchivoBinario.h>
#include "Teclado.h"

/*enum CRITERIO_BUSQUEDA {
	Primero_Path = 0,
	Primero_Tag  = 1
};*/

enum RespuestaVentanaMensaje {
	Respuesta_NADA					= -1,
	Respuesta_Aceptar				= 0,
	Respuesta_Cancelar				= 1,
	Respuesta_Aceptar_NoMostrar		= 2,
	Respuesta_Cancelar_NoMostrar	= 3
};


enum ORDEN_BASE_DATOS {
	Orden_Tipo_Genero_Grupo_Disco	= 0,
	Orden_Genero_Grupo_Disco		= 1,
	Orden_Grupo_Disco				= 2,
	Orden_Disco						= 3
};

enum TIPOS_OSCY {
	Oscy_Tres_Pixeles =  0,
	Oscy_Dos_Pixeles  =  1,
	Oscy_Un_Pixel     =  2
};

enum TIPOS_REPEAT {
	Repeat_Desactivado			=  0,
	Repeat_Normal				=  1,
	Repeat_Shufle				=  2,
	Repeat_Genero				=  4,
	Repeat_Grupo				=  5,
	Repeat_Disco				=  6,
	Repeat_Canciones			=  7,
	Repeat_Apagar_Equipo		=  9,
	Repeat_Apagar_Reproductor	= 10
};

enum TIPOS_INICIO {
	Inicio_Genero_Aleatorio			= 0,
	Inicio_Grupo_Aleatorio			= 1,
	Inicio_Disco_Aleatorio			= 2,
	Inicio_Canciones_Aleatorias		= 3,
//	Inicio_Canciones_MasEscuchadas	= 4,
	Inicio_Cualquier_Cosa			= 5,
	Inicio_Nada						= 6
};


// Clase para leer / almacenar valores en un archivo binario

class ArchivoBinarioPlayer : public DWL::Archivos::DWLArchivoBinario {
public:

					ArchivoBinarioPlayer(void) { 
					};

					ArchivoBinarioPlayer(const TCHAR *Path, const bool Abrir_si_no_existe = true) {
						AbrirArchivo(Path, Abrir_si_no_existe);
					};

				   ~ArchivoBinarioPlayer(void) { 
						CerrarArchivo();
				    };

/* 
 ///////////////////// Tipos FMOD
 inline void		LeerFMODOutputType(FMOD_OUTPUTTYPE &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(FMOD_OUTPUTTYPE));
					};

 inline void		GuardarFMODOutputType(FMOD_OUTPUTTYPE &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(FMOD_OUTPUTTYPE));
					};

 inline void		LeerFMODTagType(FMOD_TAGTYPE &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(FMOD_TAGTYPE));
					};

 inline void		GuardarFMODTagType(FMOD_TAGTYPE &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(FMOD_TAGTYPE));
					};

 inline void		LeerFMODTagDataType(FMOD_TAGDATATYPE &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(FMOD_TAGDATATYPE));
					};

 inline void		GuardarFMODTagDataType(FMOD_TAGDATATYPE &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(FMOD_TAGDATATYPE));
					};*/
 ///////////////////// Tipos PLAYER
 inline void		LeerTecla(Tecla &Val) {
						int TeclaV = 0;
						bool Control = false;
						bool Alt = false;
						LeerInt(TeclaV);
						LeerBool(Control);
						LeerBool(Alt);
//						_BDG.read(reinterpret_cast<char *>(&TeclaV), sizeof(int));
//						_BDG.read(reinterpret_cast<char *>(&Control), sizeof(bool));
//						_BDG.read(reinterpret_cast<char *>(&Alt), sizeof(bool));
						Val.AsignarTecla(TeclaV, Control, Alt);
					};

 inline void		GuardarTecla(Tecla &Val) { 
						GuardarInt(Val.TeclaVirtual);
						GuardarBool(Val.Control);
						GuardarBool(Val.Alt);
//						_BDG.write(reinterpret_cast<char *>(&Val.TeclaVirtual), sizeof(int));
//						_BDG.write(reinterpret_cast<char *>(&Val.Control), sizeof(bool));
//						_BDG.write(reinterpret_cast<char *>(&Val.Alt), sizeof(bool));
					};
/*
 inline void		LeerOrdenBaseDatos(ORDEN_BASE_DATOS &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(ORDEN_BASE_DATOS));
					};

 inline void		GuardarOrdenBaseDatos(ORDEN_BASE_DATOS &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(ORDEN_BASE_DATOS));
					};

 inline void		LeerTiposOscy(TIPOS_OSCY &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(TIPOS_OSCY));
					};

 inline void		GuardarTiposOscy(TIPOS_OSCY &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(TIPOS_OSCY));
					};

 inline void		LeerTiposRepeat(TIPOS_REPEAT &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(TIPOS_REPEAT));
					};

 inline void		GuardarTiposRepeat(TIPOS_REPEAT &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(TIPOS_REPEAT));
					};

 inline void		LeerTiposInicio(TIPOS_INICIO &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(TIPOS_INICIO));
					};

 inline void		GuardarTiposInicio(TIPOS_INICIO &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(TIPOS_INICIO));
					};

 inline void		LeerRespuestaVentanaMensaje(RespuestaVentanaMensaje &Val) {
						_BDG.read(reinterpret_cast<char *>(&Val), sizeof(RespuestaVentanaMensaje));
					};

 inline void		GuardarRespuestaVentanaMensaje(RespuestaVentanaMensaje &Val) { 
						_BDG.write(reinterpret_cast<char *>(&Val), sizeof(RespuestaVentanaMensaje));
					};*/

};
