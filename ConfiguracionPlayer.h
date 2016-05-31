#pragma once

#include "ArchivoBinarioPlayer.h"
#include <vector>
//#include "Teclado.h"

enum OrdenEstructuraDirectorios {
	GeneroGrupoDisco = 1,
	GrupoDisco       = 2,
	Disco            = 3,
	Indefinido       = 4
};

class DatosPathBuscar {
  public :
								DatosPathBuscar(void) { };
								DatosPathBuscar(const TCHAR *nPath, const OrdenEstructuraDirectorios nTipoOrden, const bool nPathAntesQueTag) : Path(nPath), TipoOrden(nTipoOrden), PathAntesQueTag(nPathAntesQueTag) { };
								DatosPathBuscar(const DatosPathBuscar &Datos) { Path = Datos.Path; TipoOrden = Datos.TipoOrden; PathAntesQueTag = Datos.PathAntesQueTag; };
	DWL::DWLString				Path;
	OrdenEstructuraDirectorios	TipoOrden;
	bool                        PathAntesQueTag;
};

class ConfiguracionPlayer {
public:
											ConfiguracionPlayer(void);
		                                   ~ConfiguracionPlayer(void);
 void										ConfiguracionPorDefecto(void);
 void                                       DirectoriosBuscarPorDefecto(void);
 const bool                                 AgregarPathBuscar(const TCHAR *nPath, const OrdenEstructuraDirectorios TipoOrden, const bool PathAntesQueTag);
 const bool                                 EliminarPathBuscar(const TCHAR *nPath);
 void										Cargar(const TCHAR *Path);
 void										CargarFuentes(void);
 void										Guardar(const TCHAR *Path);
 int										Volumen;							// Volumen
 int                                        FMod_Buffer;
 FMOD_OUTPUTTYPE                            FMod_Driver;
 int                                        FMod_TargetaSonido;
// CRITERIO_BUSQUEDA							Criterio_Busqueda;
 ORDEN_BASE_DATOS							OrdenBD;							// Orden Genero - Grupo - Disco - Canciones
 TIPOS_OSCY									TipoOscy;							// Tipo de oscyloscope, 3 - 2 - 1 pixeles por barrita de ancho
 bool										EQActivado;							// Equalizador
 TIPOS_REPEAT								Repeat;								// Repetir lista
 bool										Shufle;								// Mezclar lista
 bool                                       ExcluirNota1;						// Excluir canciones con nota de 1 estrella
 TIPOS_INICIO								Inicio;								// Lista de inicio
 int										X;									// X de la ventana principal (sin maximizar)
 int										Y;									// Y de la ventana principal (sin maximizar)
 int										Ancho;								// Ancho de la ventana principal (sin maximizar)
 int										Alto;								// Alto de la ventana principal (sin maximizar)
 int										XLog;								// X de la ventana de log (sin maximizar)
 int										YLog;								// Y de la ventana de log (sin maximizar)
 int										AnchoLog;							// Ancho de la ventana de log (sin maximizar)
 int										AltoLog;							// Alto de la ventana de log (sin maximizar)
 bool										Buscar_FiltroCaracteres;			// Flitro de caracteres para el buscador
 bool										Buscar_FiltroCoincidencias;			// Filtro de coincidencias para el buscador
 bool										Buscar_OmitirTagsV1;				// Omitir tags version 1
// DWL::DWLString								Buscar_Path;						// Path del ultimo directorio donde se ha buscado
 DWL::DWLString								Abrir_Path;							// Path del ultimo directorio donde se ha usado abrir
 DWL::DWLString								Idioma_Path;						// Path de la dll del idioma
 RespuestaVentanaMensaje					AsociarArchivos;					// Preguntar si quieres asociar los archivos
 Tecla										Tecla_PlayPausa;					//
 Tecla										Tecla_Stop;							//
 Tecla										Tecla_VolumenMas;					//
 Tecla										Tecla_VolumenMenos;					//
 Tecla										Tecla_Adelante;						//
 Tecla										Tecla_Atras;						//
 Tecla										Tecla_Siguiente;					//
 Tecla										Tecla_Anterior;						//
 bool										TecladoGlobal;						//

 bool										BuscarActualizaciones;				// Permitir buscar actualizaciones en la web
 SYSTEMTIME									BuscarActualizaciones_UltimaVez;	// Ultima fecha en la que se miro si existian actualizaciones

 
 
 
 bool										MostrarTagsEnToolTips;	// Mostrar info dels tags en tooltips
 int										OcultarMouseEnVideo;	// Ocultar el mouse automaticament en milisegons (-1 NO OCULTAR)
 int										TamFuente;				// Tamaño predeterminado de la fuente normal

 bool                                       SiempreDelante;			// Mostrar siempre delante
 bool										MostrarSoloAudio;		// Mostrar en la BD nomes el audio
 DWL::DWLString								Skin_Path;				// Path para el skin
 
 // 0.56+
 bool										UtilizarBarraTituloWindows; // Utilizar la barra de titulo de windows

 // 0.60
//std::vector<DatosPathBuscar>              Buscar_Paths;
 // 0.61+                                   
 std::vector<DatosPathBuscar>               Buscar_Paths;
 char                                       Combo_Inicio_Generar_Lista;
};
