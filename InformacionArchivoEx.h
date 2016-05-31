#pragma once
// Clase creada a partir de la LibMetaTag originalmente creada por Pipian.
// En ve de aprovechar la libreria de Pipian he optado por crear una yo mismo a partir de esta libreria,
// y incorporando algunos cambios para que funcione mucho mejor y de forma mas agil con BubaTronik.


#include <vector>
#include <fstream>
#include <DWLString.h>
#include "..\vlc-1.1.5\include\vlc\vlc.h"
#include "TipoArchivo.h"
#include "Unidades_de_Disco.h"
#include "VentanaLog.h"
#include "ConfiguracionPlayer.h"

// Clase que contiene los datos extraidos del path
class DatosPath {
  public :
					DatosPath(void) { Pista = 0; };
	DWL::DWLString	Nombre;
	DWL::DWLString	Genero;
	DWL::DWLString	Grupo;
	DWL::DWLString	Disco;
	DWL::DWLString	Tipo;
	int             Pista;
};

// Tipo de datos
enum TipoMetaDato {
	Tipo_NO_DEFINIDO	= -1,
	Tipo_ID3V1			=  0,
	Tipo_ID3V2			=  1,
	Tipo_WMA			=  2,
	Tipo_Vorbis			=  3
};
			
// clase que contiene los datos finales
class MetaDato {
  public:
						MetaDato(void) { };
	                   ~MetaDato(void) { Tipo = Tipo_NO_DEFINIDO; };
	DWL::DWLString		Nombre; // El nombre del metadato 
	DWL::DWLString		Datos;  // Los datos del frame se guardan en formato UNICODE
	TipoMetaDato		Tipo;
};


class Meta_ID3V2_UnSync {
  public:
			Meta_ID3V2_UnSync(void) { Datos = NULL; Total = 0; };
	       ~Meta_ID3V2_UnSync(void) { };
	char   *Datos;
	UINT    Total;
};

// Clase para los datos WMA
class Meta_WMA {
  public:
							Meta_WMA(void) { };
	                       ~Meta_WMA(void) { };
	std::vector<MetaDato *>	MetaDatos;
};

// Clase para los datos ID3 V1.x
class Meta_ID3V1 {
 public:
							Meta_ID3V1(void) { SubVersion = 0; };
	                       ~Meta_ID3V1(void) { };
	unsigned short			SubVersion;
	std::vector<MetaDato *>	MetaDatos;
};

// Clase para los datos ID3 V2.x
class Meta_ID3V2 {
  public:
							Meta_ID3V2(void) { Tam = 0; SubVersion = 0; Revision = 0; UnSync = 0; Extendido = false; };
	                       ~Meta_ID3V2(void) { };
	unsigned short			SubVersion;
	unsigned short			Revision;
	bool					UnSync;
	bool					Extendido;
	UINT					Tam;
	std::vector<MetaDato *>	MetaDatos;
};

// Clase para los datos Vorbis
class Meta_Vorbis {
  public:
							Meta_Vorbis(void) { };
	                       ~Meta_Vorbis(void) { };
	DWL::DWLString			Vendor;
	std::vector<MetaDato *>	MetaDatos;
};


class TMedio;
enum Tipo_Medio;

// Clase que analiza un medio y obtiene todos sus datos
class InformacionArchivoEx {
  public :
//                                InformacionArchivoEx(const TCHAR *Path, const bool EsVideo);
                                InformacionArchivoEx(void);
                               ~InformacionArchivoEx(void);

    std::vector<MetaDato *>	    MetaDatos;

    Meta_WMA					WMA;

    Meta_ID3V2					ID3V2;

    Meta_ID3V1					ID3V1;

    Meta_Vorbis					Vorbis;

	DatosPath					InfoPath;

	TMedio                     *ObtenerInfo(const TCHAR *Path, TipoArchivo &TiposArchivo, Unidades_Disco &Unidades, std::vector<DatosPathBuscar> &Buscar_Paths, libvlc_instance_t *InstanciaVLC, FMOD::System *InstanciaFMOD);

								// Función que analiza un archivo y alamcena los metadatos que hay en el
								// Retorna el tamaño en bytes del archivo
    DWORD					    ObtenerMetaDatosAudio(const TCHAR *Path, TipoArchivo &TiposArchivo);

  private :
	MetaDato                  *_BuscarMetaDato(const TCHAR *Nombre, const TipoMetaDato Tipo = Tipo_NO_DEFINIDO);

	UINT64                     _ObtenerTiempoCancion(const TCHAR *Path, FMOD::System *InstanciaFMOD);
	UINT64                     _ObtenerTiempoCancionCDAudio(const TCHAR *Path, FMOD::System *InstanciaFMOD);

								// Función para analizar el medio con VLC para determinar si es un video o es audio solo
	const bool				   _AnalisisVLC(const TCHAR *Path, UINT64 &Tiempo, UINT &nAncho, UINT &nAlto, libvlc_instance_t *InstanciaVLC);

	const Tipo_Medio           _TipoMedio(DWL::DWLString &PathFinal, TipoArchivo &TiposArchivo);
								// Función que analiza un archivo y alamcena los metadatos que hay en el
    void					   _AnalizarPath(DWL::DWLString &Path, Tipo_Medio TipoMedio, std::vector<DatosPathBuscar> &Buscar_Paths);
    void					   _AnalizarPathPistaAudio(DWL::DWLString &Path);
	void					   _AnalizarNombre(DWL::DWLString &Nombre);
	DWORD                      _ObtenerLongitud(const TCHAR *Path);
	const bool                 _EsNumero(const TCHAR Caracter);

	//void					   _GeneroNumerico(DWL::DLWString &NombreGenero);

	const UINT                 _Filtro(const TCHAR *Origen, DWL::DWLString *Destino);

								// Función que borra la memoria almacenada en esta clase
    void					   _BorrarMemoria(void);

								// Retorna la posicion inicial del tag, -1 si no existe
    const int				   _BuscarVorbis(DWL::Archivos::DWLArchivoBinario &Archivo);
								// Retorna la posicion inicial del tag, -1 si no existe
    const int				   _BuscarWMA(DWL::Archivos::DWLArchivoBinario &Archivo);
								// Retorna 1 si exsite, -1 si no existe.
    const int				   _BuscarID3V1(DWL::Archivos::DWLArchivoBinario &Archivo);
								// Retorna la posicion inicial del tag, -1 si no existe
    const int				   _BuscarID3V2(DWL::Archivos::DWLArchivoBinario &Archivo);

	MetaDato                  *_BuscarNombre(void);
	MetaDato                  *_BuscarGenero(void);
	MetaDato                  *_BuscarGrupo(void);
	MetaDato                  *_BuscarDisco(void);
	MetaDato                  *_BuscarPista(void);

    void					   _ObtenerDatosWMA(DWL::Archivos::DWLArchivoBinario &Archivo, const int Posicion);
    void					   _ObtenerDatosVorbis(DWL::Archivos::DWLArchivoBinario &Archivo, const int Posicion);
    void					   _ObtenerDatosID3V1(DWL::Archivos::DWLArchivoBinario &Archivo);
    void					   _ObtenerDatosID3V2(DWL::Archivos::DWLArchivoBinario &Archivo, const int Posicion);
    Meta_ID3V2_UnSync	      *_ID3V2_MirarSync(char *Datos, UINT Tam);
    void					   _ID3V2_UnSync(char *Datos, char *pc);
    MetaDato		          *_ID3V2_ParsearFrame(char **pc, char *Fin);
};
