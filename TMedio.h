#ifndef BUBATRONIK_TMEDIO
#define BUBATRONIK_TMEDIO

#include "TDisco.h"
#include <vector>

class NodoArbolBaseDatos;
class ItemListaMedios;

enum Estados_Medio {
	SinCargar = 0,
	EnStop    = 1,
	EnPlay    = 2,
	EnPausa   = 3,
	Terminada = 4,
	EnError   = 5
};


enum Tipo_Medio {
	Tipo_Medio_INDEFINIDO	= -1,
	Tipo_Medio_PreAudio		= 0,
	Tipo_Medio_Audio		= 1,
	Tipo_Medio_Video		= 2,
	Tipo_Medio_CDAudio		= 3,
	Tipo_Medio_Lista		= 4,
	Tipo_Medio_Subtitulos	= 5
};

typedef unsigned __int64  UINT64;

// Datos basicos de un medio (tanto audio como video)
class TMedio {
  public:
								TMedio(void)  { };
	DWL::DWLString				Path;		//
	DWL::DWLString				Nombre;		//
	Tipo_Medio					TipoMedio;
	DWORD						Reproducido;//
	UINT64						Tiempo;		//
	DWORD						Longitud;	//
	DWORD						ID_Disco;	//
	NodoArbolBaseDatos		   *Nodo;
	ItemListaMedios            *Item;
	BYTE                        Nota;		//
};


// Datos temporales para un medio con audio
class TMedioPreAudio : public TMedio {
  public :
							TMedioPreAudio(void) { TipoMedio = Tipo_Medio_PreAudio; Pista = 0; };
	int                     Pista;
	DWL::DWLString          NombreTipo; 
	DWL::DWLString          NombreGenero;
	DWL::DWLString          NombreGrupo;
	DWL::DWLString          NombreDisco;
};

// Datos para un medio con audio
class TMedioAudio : public TMedio {
  public :
							TMedioAudio(void) { TipoMedio = Tipo_Medio_Audio; Pista = 0; };
	int                     Pista;
	float                   EQ[11]; // de momento solo audio
	TTipo                  *Tipo; 
	TGenero                *Genero;
	TGrupo                 *Grupo;
	TDisco                 *Disco;
	StrTp                  *StrTipo; 
	StrTp                  *StrGenero;
	StrTp                  *StrGrupo;
	StrTp                  *StrDisco;
};

// Datos para un medio con video
class TMedioVideo : public TMedio {
  public :
					TMedioVideo(void) { TipoMedio = Tipo_Medio_Video; Pista = 0; };
	UINT			Ancho;
	UINT			Alto;
	int             Pista;
};

// Datos para una lista de medios M3u
class TMedioListaM3u : public TMedio {
  public :
								TMedioListaM3u(void) { TipoMedio = Tipo_Medio_Lista; };
	//std::vector<DWL::DWLString> Paths;
};

// Datos para la ruta de un archivo de subtitulos
class TMedioSubtitulos : public TMedio {
  public :
								TMedioSubtitulos(void) { TipoMedio = Tipo_Medio_Subtitulos; };
	//std::vector<DWL::DWLString> Paths;
};

// Datos para una pista de un CD de audio
class TMedioCDAudio : public TMedio {
  public :
			TMedioCDAudio(void) { TipoMedio = Tipo_Medio_CDAudio; Pista = 0; };
	int		Pista;
};

#endif