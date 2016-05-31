// Clase creada a partir de la LibMetaTag originalmente creada por Pipian.
// En ve de aprovechar la libreria de Pipian he optado por crear una yo mismo a partir de esta libreria,
// y incorporando algunos cambios para que funcione mucho mejor y de forma mas agil con BubaTronik.
#include "StdAfx.h"
#include "InformacionArchivoEx.h"
#include "BaseDatos.h"
#include "SistemaVLC.h"
#include "BubaTronikApp.h"

#define TAM_BUFFER		4096



InformacionArchivoEx::InformacionArchivoEx(void) {
}

/*InformacionArchivoEx::InformacionArchivoEx(const TCHAR *Path, const bool EsVideo) {
    if (EsVideo == false)	_AbrirArchivoAudio(Path);
//	else					_AbrirArchivoAudio(Path);
}*/

InformacionArchivoEx::~InformacionArchivoEx(void) {
    _BorrarMemoria();
}




// Falta obtener el tiempo del audio
TMedio *InformacionArchivoEx::ObtenerInfo(const TCHAR *Path, TipoArchivo &TiposArchivo, Unidades_Disco &Unidades, std::vector<DatosPathBuscar> &Buscar_Paths, libvlc_instance_t *InstanciaVLC, FMOD::System *InstanciaFMOD) {
    // Lo primero es determinar si es un archivo de audio o de video
	DWL::DWLString	PathStr = Path;
	UINT64			Tiempo  = 0;
	UINT			Ancho   = 0;
	UINT			Alto    = 0;
	bool            EsVideo = false;
	DWL::DWLString  Tmp;

	// Comprobamos que exista la ruta del archivo
	TCHAR PathCorto[MAX_PATH + 1];
	ZeroMemory(PathCorto, MAX_PATH + 1);
	GetShortPathName(Path, PathCorto, MAX_PATH + 1);
	if (PathCorto[0] == 0) return NULL;


	Tipo_Medio TipoMedio = _TipoMedio(PathStr, TiposArchivo); 
	// Si es del tipo video, puede ser que dicho contenedor de video solo tenga audio dentro, por lo que se considerara dicho medio como audio a pesar de su extensión.
	if (TipoMedio == Tipo_Medio_Video || TipoMedio == Tipo_Medio_INDEFINIDO) {
		EsVideo = true;
		TipoArchivo_Datos *nDatos = TiposArchivo.BuscarTipoArchivo(Path);
		if (nDatos != NULL) {
			if (nDatos->ID == EXT_ASF || nDatos->ID == EXT_MP4 || nDatos->ID == EXT_OGG) {
				EsVideo = _AnalisisVLC(Path, Tiempo, Ancho, Alto, InstanciaVLC);
			}
		}

		if (EsVideo == false && TipoMedio != Tipo_Medio_INDEFINIDO) TipoMedio = Tipo_Medio_Audio;
	}
	if (TipoMedio == Tipo_Medio_INDEFINIDO && EsVideo == true) TipoMedio = Tipo_Medio_Video;

	//	else												EsVideo = false;

	Unidad_Disco *UD = Unidades.Buscar_Letra(Path[0]);	
	OrdenEstructuraDirectorios TipoOrden = Indefinido;
	bool                       PathAntesQueTag = false;
	_AnalizarPath(PathStr, TipoMedio, Buscar_Paths);
	switch (TipoMedio) {
		case Tipo_Medio_Subtitulos :
			TMedioSubtitulos *InfoSubtitulos;
			InfoSubtitulos = new TMedioSubtitulos;
			InfoSubtitulos->Path = PathStr;
			return InfoSubtitulos;
		case Tipo_Medio_Audio :
			// Buscamos los datos de su path buscar (si es que existen)
			for (size_t n = 0; n < Buscar_Paths.size(); n++) {
				if (Buscar_Paths[n].Path == PathStr.SubStr(0, Buscar_Paths[n].Path.Tam())) {
					TipoOrden = Buscar_Paths[n].TipoOrden;
					PathAntesQueTag = Buscar_Paths[n].PathAntesQueTag;
				}
			}
				
			TMedioPreAudio *InfoAudio;
			InfoAudio = new TMedioPreAudio;
			// Examinamos los tags y de paso obtenemos la longitud del archivo
			InfoAudio->Longitud = ObtenerMetaDatosAudio(Path, TiposArchivo); 

			// Obtengo los mejores valores posibles necesarios
			MetaDato *InfoNombre;
			InfoNombre = _BuscarNombre();
			MetaDato *InfoGenero;
			InfoGenero = _BuscarGenero();
			MetaDato *InfoGrupo;
			InfoGrupo = _BuscarGrupo();
			MetaDato *InfoDisco;
			InfoDisco = _BuscarDisco();
			MetaDato *InfoPista;
			InfoPista = _BuscarPista();

			InfoAudio->Path			= PathStr;
			InfoAudio->NombreTipo	= InfoPath.Tipo();
		
			if (PathAntesQueTag == false) { // Primero el tag
				// [Nombre] por cojones como minimo hay el nombre en el archivo por lo que nunca sera desconocido
				if (InfoNombre == NULL) {
					_Filtro(InfoPath.Nombre(), &InfoAudio->Nombre);
				}
				else {
					if (InfoNombre->Datos.Tam() > 0)	_Filtro(InfoNombre->Datos(), &InfoAudio->Nombre);
					if (InfoAudio->Nombre.Tam() == 0)	_Filtro(InfoPath.Nombre(), &InfoAudio->Nombre);
				}
		
				// [Genero]
				if (InfoGenero != NULL) _Filtro(InfoGenero->Datos(), &InfoAudio->NombreGenero);
				else					_Filtro(InfoPath.Genero(), &InfoAudio->NombreGenero);
				// - Un filtro de un string que contiene solo espacios puede retornar un nombre vacio
				if (InfoAudio->NombreGenero.Tam() == 0) InfoAudio->NombreGenero = TEXT("1024"); // El genero numerico 1024 no existe y retornara Genero desconocido

				// [Grupo]
				if (InfoGrupo == NULL) {
					if (InfoPath.Grupo.Tam() > 0)			_Filtro(InfoPath.Grupo(), &InfoAudio->NombreGrupo);
				}
				else {
					if (InfoGrupo->Datos.Tam() > 0)			_Filtro(InfoGrupo->Datos(), &InfoAudio->NombreGrupo); 
					if (InfoAudio->NombreGrupo.Tam() == 0)	_Filtro(InfoPath.Grupo(), &InfoAudio->NombreGrupo);
				}
				// - Un filtro de un string que contiene solo espacios puede retornar un nombre vacio
				if (InfoAudio->NombreGrupo.Tam() == 0)	
					InfoAudio->NombreGrupo = TEXT("?");
		
				// [Disco]
				if (InfoDisco == NULL) {
					if (InfoPath.Disco.Tam() > 0)			_Filtro(InfoPath.Disco(), &InfoAudio->NombreDisco);
				}
				else {
					if (InfoDisco->Datos.Tam() > 0)			_Filtro(InfoDisco->Datos(), &InfoAudio->NombreDisco);
					if (InfoAudio->NombreDisco.Tam() == 0)	_Filtro(InfoPath.Disco(), &InfoAudio->NombreDisco);
				}
				// - Un filtro de un string que contiene solo espacios puede retornar un nombre vacio
				if (InfoAudio->NombreDisco.Tam() == 0)	
					InfoAudio->NombreDisco = TEXT("?");

				// [Pista]
				if (InfoPista == NULL)  InfoAudio->Pista = InfoPath.Pista;
				else { // No tienen porque venir los datos de la pista solo con un valor
					DWL::DWLString PistaFinal;
					bool EsNum = true;
					for (size_t n = 0; (n < InfoPista->Datos.Tam() && EsNum == true); n++) {
						EsNum = _EsNumero(InfoPista->Datos[n]);
						if (EsNum == true) PistaFinal += InfoPista->Datos[n];
					}
					if (PistaFinal.Tam() > 0)	InfoAudio->Pista = _wtoi(PistaFinal());
					else						InfoAudio->Pista = InfoPath.Pista;
					// Si en el tag trae pista 0 o algo raro metemos la del path
					if (InfoAudio->Pista < 1)	InfoAudio->Pista = InfoPath.Pista;
				}
			}
			else { // Primero el path
				// [Nombre] por cojones como minimo hay el nombre en el archivo por lo que nunca sera desconocido
				_Filtro(InfoPath.Nombre(), &InfoAudio->Nombre);
				// [Genero]
				if (InfoPath.Genero.Tam() != 0) _Filtro(InfoPath.Genero(), &InfoAudio->NombreGenero);
				else {
					if (InfoGenero != NULL) {
						if (InfoGenero->Datos.Tam() != 0) _Filtro(InfoGenero->Datos(), &InfoAudio->NombreGenero);
					}
				}
				// - Un filtro de un string que contiene solo espacios puede retornar un nombre vacio
				if (InfoAudio->NombreGenero.Tam() == 0) InfoAudio->NombreGenero = TEXT("1024"); // El genero numerico 1024 no existe y retornara Genero desconocido
				// [Grupo]
				if (InfoPath.Grupo.Tam() != 0) {
					_Filtro(InfoPath.Grupo(), &InfoAudio->NombreGrupo);
					if (InfoAudio->NombreGrupo.Tam() == 0 && InfoGrupo->Datos.Tam() > 0) _Filtro(InfoGrupo->Datos(), &InfoAudio->NombreGrupo);
				}
				else {
					if (InfoGrupo != NULL) {
						if (InfoGrupo->Datos.Tam() > 0)	_Filtro(InfoGrupo->Datos(), &InfoAudio->NombreGrupo); 
					}
				}
				// - Un filtro de un string que contiene solo espacios puede retornar un nombre vacio
				if (InfoAudio->NombreGrupo.Tam() == 0)	
					InfoAudio->NombreGrupo = TEXT("?");
				// [Disco]
				if (InfoPath.Disco.Tam() != 0) {
					_Filtro(InfoPath.Disco(), &InfoAudio->NombreDisco);
					if (InfoAudio->NombreDisco.Tam() == 0 && InfoDisco->Datos.Tam() > 0) _Filtro(InfoDisco->Datos(), &InfoAudio->NombreDisco);
				}
				else {
					if (InfoDisco != NULL) {
						if (InfoDisco->Datos.Tam() > 0)	_Filtro(InfoDisco->Datos(), &InfoAudio->NombreDisco);
					}
				}
				// - Un filtro de un string que contiene solo espacios puede retornar un nombre vacio
				if (InfoAudio->NombreDisco.Tam() == 0)	
					InfoAudio->NombreDisco = TEXT("?");
				// [Pista]
				if (InfoPath.Pista > 0)  InfoAudio->Pista = InfoPath.Pista;
				else { // No tienen porque venir los datos de la pista solo con un valor
					DWL::DWLString PistaFinal;
					bool EsNum = true;
					if (InfoPista != NULL) {
						for (size_t n = 0; (n < InfoPista->Datos.Tam() && EsNum == true); n++) {
							EsNum = _EsNumero(InfoPista->Datos[n]);
							if (EsNum == true) PistaFinal += InfoPista->Datos[n];
						}
						if (PistaFinal.Tam() > 0)	InfoAudio->Pista = _wtoi(PistaFinal());
						else						InfoAudio->Pista = 0;
					}
					else {
						InfoAudio->Pista = 0;
					}
				}


			}
			InfoAudio->Reproducido	= 0;
			InfoAudio->Tiempo		= _ObtenerTiempoCancion(Path, InstanciaFMOD);
	//		InfoAudio->Longitud		= _ObtenerLongitud(PathStr()); // la estamos obteniendo desde ObtenerMetaDatosAudio que tambien abre el archivo
			if (UD == NULL) InfoAudio->ID_Disco		= 0; // es una unidad de red
			else			InfoAudio->ID_Disco		= UD->Numero_Serie();
			InfoAudio->Nota			= 2;
			InfoAudio->Item			= NULL;
			InfoAudio->Nodo			= NULL;
			InfoAudio->TipoMedio    = Tipo_Medio_PreAudio; // falta por analizar sus raices (generos, grupos, discos, anomalias)
			return InfoAudio;
		case Tipo_Medio_Video :
			TMedioVideo *InfoVideo;
			InfoVideo = new TMedioVideo;
			InfoVideo->Path			= PathStr;
			_Filtro(InfoPath.Nombre(), &InfoVideo->Nombre);
			InfoVideo->Reproducido	= 0;
			InfoVideo->Tiempo		= Tiempo;
			InfoVideo->Longitud		= _ObtenerLongitud(PathStr());
			if (UD == NULL) InfoVideo->ID_Disco		= 0; // es una unidad de red
			else			InfoVideo->ID_Disco		= UD->Numero_Serie();
			InfoVideo->Nota			= 2;
			InfoVideo->Item			= NULL;
			InfoVideo->Nodo			= NULL;
			InfoVideo->Pista		= InfoPath.Pista;
			InfoVideo->Alto			= Alto;
			InfoVideo->Ancho		= Ancho;
			InfoVideo->TipoMedio    = Tipo_Medio_Video;
			return InfoVideo;
		case Tipo_Medio_Lista :
			TMedioListaM3u *InfoLista;
			InfoLista = new TMedioListaM3u;
			InfoLista->Path			= PathStr;
			_Filtro(InfoPath.Nombre(), &InfoLista->Nombre);
			InfoLista->Reproducido	= 0;
			InfoLista->Tiempo		= 0;
			InfoLista->Longitud		= _ObtenerLongitud(PathStr());
			if (UD == NULL) InfoLista->ID_Disco		= 0; // es una unidad de red
			else			InfoLista->ID_Disco		= UD->Numero_Serie();
			InfoLista->Nota			= 2;
			InfoLista->Item			= NULL;
			InfoLista->Nodo			= NULL;
			InfoLista->TipoMedio    = Tipo_Medio_Lista;
			return InfoLista;
		case Tipo_Medio_CDAudio :
//			_AnalizarPathPistaAudio(PathStr);
			TMedioCDAudio *InfoCDAudio;
			InfoCDAudio = new TMedioCDAudio;
			InfoCDAudio->Path		 = PathStr;
			_Filtro(InfoPath.Nombre(), &InfoCDAudio->Nombre);
			InfoCDAudio->Reproducido = 0;
			InfoCDAudio->Tiempo		 = _ObtenerTiempoCancionCDAudio(Path, InstanciaFMOD);
			InfoCDAudio->Longitud	 = _ObtenerLongitud(PathStr());
			if (UD == NULL) InfoCDAudio->ID_Disco		= 0; // es una unidad de red
			else			InfoCDAudio->ID_Disco		= UD->Numero_Serie();
			InfoCDAudio->Nota		 = 2;
			InfoCDAudio->Item		 = NULL;
			InfoCDAudio->Nodo		 = NULL;
			InfoCDAudio->TipoMedio   = Tipo_Medio_CDAudio;
			InfoCDAudio->Pista       = InfoPath.Pista;
			return InfoCDAudio;
	}


	return NULL;
}

// Devuelve el tamaño del archivo
DWORD InformacionArchivoEx::ObtenerMetaDatosAudio(const TCHAR *Path, TipoArchivo &TiposArchivo) {
	_BorrarMemoria();
	DWL::Archivos::DWLArchivoBinario Archivo;
	int Pos = -1;

	if (Archivo.AbrirArchivoLectura(Path)) {
		TipoArchivo_Datos *Tipo = TiposArchivo.Extension(Path);
		if (Tipo == NULL) return Archivo.Tam();
		switch (Tipo->ID) {
			case EXT_MP3 : case EXT_MP2 :
				// Busco tags ID3V2
				Pos = _BuscarID3V2(Archivo);
				if (Pos > -1) _ObtenerDatosID3V2(Archivo, Pos);
				// Busco tags ID3V1
				if (_BuscarID3V1(Archivo) == 1) _ObtenerDatosID3V1(Archivo);
				break;
			case EXT_WMA :
				// Busco tags WMA
				Pos = _BuscarWMA(Archivo);
				if (Pos > -1) _ObtenerDatosWMA(Archivo, Pos);
				break;
			case EXT_ASF :
				// Busco tags WMA
				Pos = _BuscarWMA(Archivo);
				if (Pos > -1) _ObtenerDatosWMA(Archivo, Pos);
				// Busco tags Vorbis
				Pos = _BuscarVorbis(Archivo);
				if (Pos > -1) _ObtenerDatosVorbis(Archivo, Pos);
				break;
			case EXT_OGG :
				// Busco tags Vorbis
				Pos = _BuscarVorbis(Archivo);
				if (Pos > -1) _ObtenerDatosVorbis(Archivo, Pos);
				break;
		}

	}

	// Leo la longitud del archivo
	return Archivo.Tam();
}

UINT64 InformacionArchivoEx::_ObtenerTiempoCancion(const TCHAR *Path, FMOD::System *InstanciaFMOD) {
	UINT Milisegundos = 0;
	FMOD::Sound	 *Stream;
	InstanciaFMOD->createStream(reinterpret_cast<const char *>(Path), FMOD_OPENONLY | FMOD_UNICODE, 0, &Stream);
	Stream->getLength(&Milisegundos, FMOD_TIMEUNIT_MS);
	FMOD_RESULT R = Stream->release();
	return Milisegundos;
}

UINT64 InformacionArchivoEx::_ObtenerTiempoCancionCDAudio(const TCHAR *Path, FMOD::System *InstanciaFMOD) {
	char Letra[3]; 
	Letra[0] = static_cast<char>(Path[0]);
	Letra[1] = ':';
	Letra[2] = '\0';
	UINT Milisegundos = 0;
	FMOD::Sound	 *Stream = NULL;
	FMOD::Sound	 *PistaCD = NULL;
	FMOD_RESULT Error = InstanciaFMOD->createStream(Letra, FMOD_OPENONLY, 0, &Stream);
	if (Stream != NULL) {
		Stream->getSubSound(InfoPath.Pista - 1, &PistaCD); // no s'identifica la pista en els cds d'audio posible error en funcio Info
		if (PistaCD != NULL) {
			PistaCD->getLength(&Milisegundos, FMOD_TIMEUNIT_MS);
			PistaCD->release();
			Stream->release();
		}
	}
	return Milisegundos;
}



// retorna true si es un video, false en caso contrario
// Si es un video ademas rellena los parametros Tiempo, nAncho y nAlto
const bool InformacionArchivoEx::_AnalisisVLC(const TCHAR *Path, UINT64 &Tiempo, UINT &nAncho, UINT &nAlto, libvlc_instance_t *Instancia) {
	char	Destino[2049];
	size_t  TamnTexto = wcslen(Path);
	int		TamRes = WideCharToMultiByte(CP_UTF8, NULL, Path, TamnTexto, Destino, 2048, NULL, NULL);
	Destino[TamRes] = 0;

	libvlc_media_t			*Media			= NULL;
	libvlc_media_player_t	*nMediaPlayer	= NULL;
	Media = libvlc_media_new_path(Instancia, Destino);

	libvlc_media_parse(Media);
	libvlc_media_add_option(Media, "sout=#description:dummy");

	nMediaPlayer = libvlc_media_player_new_from_media(Media);

	libvlc_state_t Estado = libvlc_Opening;
	libvlc_media_player_play(nMediaPlayer);

    //posible deadlock
	// Esperamos hasta que empieze el play
	while (Estado != libvlc_Playing && Estado != libvlc_Ended) { 
		Estado = libvlc_media_player_get_state(nMediaPlayer);
        Sleep(100);
		if (Estado == libvlc_Error) { // Hay un error en la libvlc, salimos
			Sleep(300); // Nos aseguramos de que el log se escriba
			libvlc_media_player_stop(nMediaPlayer);
			libvlc_media_release(Media);
			libvlc_media_player_release(nMediaPlayer);
			Tiempo = 0;
			nAncho = 0;
			nAlto = 0;
			return false;
		}
	}
	
	// Miramos los streams disponibles y buscamos el de video
	libvlc_media_track_info_t *TI = NULL;
	int z = libvlc_media_get_tracks_info(Media, &TI);
	for (int n = 0; n < z; n++) {
		if (TI->i_type == libvlc_track_video) {
			libvlc_media_player_stop(nMediaPlayer);
			Tiempo = 0; //libvlc_media_player_get_length(nMediaPlayer); // NO DA BIEN EL TIEMPO..............
			nAncho = TI->u.video.i_width;
			nAlto = TI->u.video.i_height;
			libvlc_media_release(Media);
			libvlc_media_player_release(nMediaPlayer);		
			return true;
		}
		TI ++;
	}
	libvlc_media_release(Media);
	libvlc_media_player_release(nMediaPlayer);		

	// No hay streams de video retornamos false
	Tiempo	= 0;
	nAncho	= 0;
	nAlto	= 0;
	return false;
}
/*
void InformacionArchivoEx::_GeneroNumerico(DWL::DLWString &NombreGenero) {
	if (NombreGenero.Tam == 0) return;
	UINT NumGenero = 0 ;
	switch (NombreGenero[0]) {
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
			NumGenero = DWLStrToInt(NombreGenero());
			Sistema.App.Idioma_EstiloStr(NumGenero);
			break;
	}

}*/

// Funcion que filtra el texto para quitarle caracteres que pueden confundir la base de datos.
const UINT InformacionArchivoEx::_Filtro(const TCHAR *Origen, DWL::DWLString *Destino) {
	if (Sistema.App.Config.Buscar_FiltroCaracteres == false) {
		Destino->Borrar();
		Destino->AgregarTexto(Origen);
		Destino->Caracter(0) = toupper(Destino->Caracter(0));
		return Destino->Tam();
	}

	if (Origen == NULL) 
		return 0;
	size_t txtsize = DWLStrLen(Origen);
	DWL::DWLString Tmp;
	Tmp = TEXT('*'); // Filtro caracteres
	size_t i;
	for (i = 0; i < txtsize; i++) {                                                        
		switch (Origen[i]) {                                                        
			case 65279 : // caracter invisible inutil del unicode
				break;
			case TEXT(':') : case TEXT('_') : case TEXT(',') : case TEXT('.') : case 96  /*'*/ : case TEXT('(') : case TEXT(')') :
			case TEXT('!') : case TEXT('?') : case TEXT('¿') : case TEXT('^') : case TEXT('"') : case TEXT('#') : case TEXT('$') :
			case TEXT('%') : case TEXT('/') : case TEXT('\\'): case TEXT('-') : case TEXT('[') : case TEXT(']') : case TEXT('~') :
				if (Tmp[Tmp.Tam() -1] != TEXT(' ')) Tmp += TEXT(' '); // Si el caracter anterior no es un espacio, pongo uno.
				break;                                                                  
			case ' ' : 
				if (Tmp[Tmp.Tam() -1] != TEXT(' ')) Tmp += TEXT(' '); // Si el caracter anterior no es un espacio, pongo uno.                                                 
				break;
			// Elimino todas las tildes
			case TEXT('à') : case TEXT('á') : case TEXT('â') : case TEXT('ä') : case TEXT('À') : case TEXT('Á') : case TEXT('Â') : case TEXT('Ä') :	
				Tmp += TEXT('a'); 																						
				break;
			case TEXT('è') : case TEXT('é') : case TEXT('ê') : case TEXT('ë') : case TEXT('È') : case TEXT('É') : case TEXT('Ê') : case TEXT('Ë') :	
				Tmp += TEXT('e'); 																						
				break;
			case TEXT('ì') : case TEXT('í') : case TEXT('î') : case TEXT('ï') : case TEXT('Ì') : case TEXT('Í') : case TEXT('Î') : case TEXT('Ï') :	
				Tmp += TEXT('i'); 	
				break;
			case TEXT('ò') : case TEXT('ó') : case TEXT('ô') : case TEXT('ö') : case TEXT('Ò') : case TEXT('Ó') : case TEXT('Ô') : case TEXT('Ö') :	
				Tmp += TEXT('o'); 	
				break;
			case TEXT('ù') : case TEXT('ú') : case TEXT('û') : case TEXT('ü') : case TEXT('Ù') : case TEXT('Ú') : case TEXT('Û') : case TEXT('Ü') :	
				Tmp += TEXT('u'); 																						
				break;
			case TEXT('Ñ') : case TEXT('ñ') :
				Tmp += TEXT('ñ');
				break;
			case TEXT('Ç') : case TEXT('ç') :
				Tmp += TEXT('ç');
				break;
			default :                                                                   
				Tmp += static_cast<TCHAR>(tolower(Origen[i]));
				break;                                                                  
		}
	}
	i = Tmp.Tam() -1; // Elimino espacios del final
	while(Tmp[i] == TEXT(' ')) { Tmp[i] = TEXT('\0'); i--; }
	i = 1; // Busco el primer caracter que no sea un espacio
	while (Tmp[i] == TEXT(' ')) { i ++; }

//	Tmp[i] = toupper(Tmp[i]);
	Destino->Borrar();
	Destino->AgregarTexto(&Tmp[i]);
	Destino->Caracter(0) = toupper(Destino->Caracter(0));
	return Destino->Tam();
}


const bool InformacionArchivoEx::_EsNumero(const TCHAR Caracter) {
	switch (Caracter) {
		case TEXT('0') : case TEXT('1') : case TEXT('2') : case TEXT('3') : case TEXT('4') : 
		case TEXT('5') : case TEXT('6') : case TEXT('7') : case TEXT('8') : case TEXT('9') : 
			return true;
		default :
			return false;
	}
}

// Nombres de pista validos : 1, 01, 1x01, 1-12, 20x21
// Nunca se hace caso a 4 digitos seguidos o mas (ya que son o el año o otra cosa.. pero la pista seguro que no)
// Se analiza de izquierda a derecha (lo mas normal es que la pista salga a la izquieda nada mas empezar)
void InformacionArchivoEx::_AnalizarNombre(DWL::DWLString &Nombre) { 
	// Quitamos la extension
	DWL::DWLString TmpStr = Nombre;
	size_t i;
	for (i = TmpStr.Tam() - 1; i != 0; i--) {
		if (TmpStr[i] == TEXT('.')) {
			InfoPath.Tipo = TmpStr.SubStr(i + 1);
			Nombre = TmpStr.SubStr(0, i);
			break;
		}
	}
	bool PistaEncontrada = false;
	InfoPath.Pista = 0;
	BOOL C1 = -1, C2 = -1, C3 = -1, C4 = -1, C5 = -1;
	for (i = 0; i < Nombre.Tam(); i++) {
		
		if (_EsNumero(Nombre[i]) == true) { // Si encontramos un numero escaneamos los 4 siguientes caracteres
			C1 = FALSE;	C2 = FALSE;	C3 = FALSE;	C4 = FALSE;	C5 = FALSE;
			if (Nombre.Tam() > i + 1) C1 = _EsNumero(Nombre[i + 1]);
			if (Nombre.Tam() > i + 2) C2 = _EsNumero(Nombre[i + 2]);
			if (Nombre.Tam() > i + 3) C3 = _EsNumero(Nombre[i + 3]);
			if (Nombre.Tam() > i + 4) C4 = _EsNumero(Nombre[i + 4]);
			if (Nombre.Tam() > i + 5) C5 = _EsNumero(Nombre[i + 5]);
			// 01?01? (si el separador es un espacio no se tomara como pista doble)
			if (C1 == TRUE && C2 == FALSE && Nombre[i + 2] != TEXT(' ') && C3 == TRUE && C4 == TRUE && C5 == FALSE) { // Pista doble, se multiplicara por 100 la primera parte de la pista
				InfoPath.Pista = _wtoi(Nombre.SubStr(i, 2).Texto()) * 100;
				InfoPath.Pista += _wtoi(Nombre.SubStr(i + 3, 2).Texto());
				if (i != 0) InfoPath.Nombre = Nombre.SubStr(0, i);
				InfoPath.Nombre += Nombre.SubStr(i + 5, Nombre.Tam() - (i + 5));
				return;
			}
			// 1?01? (si el separador es un espacio no se tomara como pista doble)
			if (C1 == FALSE && Nombre[i + 1] != TEXT(' ') && C2 == TRUE && C3 == TRUE && C4 == FALSE) { // Pista doble, se multiplicara por 100 la primera parte de la pista
				InfoPath.Pista = _wtoi(Nombre.SubStr(i, 1).Texto()) * 100;
				InfoPath.Pista += _wtoi(Nombre.SubStr(i + 2, 2).Texto());
				if (i != 0) InfoPath.Nombre = Nombre.SubStr(0, i);
				InfoPath.Nombre += Nombre.SubStr(i + 4, Nombre.Tam() - (i + 4));
				return;
			}
			// 001?
			if (C1 == TRUE && C2 == TRUE && C3 == FALSE) { // Pista unica tres digitos
				InfoPath.Pista = _wtoi(Nombre.SubStr(i, 3).Texto());
				if (i != 0) InfoPath.Nombre = Nombre.SubStr(0, i);
				InfoPath.Nombre += Nombre.SubStr(i + 3, Nombre.Tam() - (i + 3));
				return;
			}
			// 01?
			if (C1 == TRUE && C2 == FALSE) { // Pista unica dos digitos
				InfoPath.Pista = _wtoi(Nombre.SubStr(i, 2).Texto());
				if (i != 0) InfoPath.Nombre = Nombre.SubStr(0, i);
				InfoPath.Nombre += Nombre.SubStr(i + 2, Nombre.Tam() - (i + 2));
				return;
			}
			// 1?
			if (C1 == FALSE) { // Pista unica un digito
				InfoPath.Pista = _wtoi(Nombre.SubStr(i, 1).Texto());
				if (i != 0) InfoPath.Nombre = Nombre.SubStr(0, i);
				InfoPath.Nombre += Nombre.SubStr(i + 1, Nombre.Tam() - (i + 1));
				return;
			}
		}
	}
	// No se ha encontrado nada parecido a una pista
	InfoPath.Nombre = Nombre;
}

/*void InformacionArchivoEx::_AnalizarPathPistaAudio(DWL::DWLString &Path) {
	InfoPath.Nombre.CopiarTexto(Path(), 5);
	InfoPath.Pista = 
}*/

void InformacionArchivoEx::_AnalizarPath(DWL::DWLString &Path, Tipo_Medio TipoMedio,  std::vector<DatosPathBuscar> &Buscar_Paths) {
	OrdenEstructuraDirectorios Orden = Indefinido;
	DWL::DWLString             PathBuscar;
	for (size_t n = 0; n < Buscar_Paths.size(); n++) {
		if (Buscar_Paths[n].Path == Path.SubStr(0, Buscar_Paths[n].Path.Tam())) {
			Orden = Buscar_Paths[n].TipoOrden;
			PathBuscar.Borrar();
			PathBuscar.AgregarTexto(Buscar_Paths[n].Path(), Buscar_Paths[n].Path.Tam() - 1);
			break;
		}
	}

	
	size_t UltimaPos = Path.Tam();
	DWL::DWLString *TmpStr;
	std::vector <DWL::DWLString *> Paths;
	size_t i;
	switch (TipoMedio) {
		case Tipo_Medio_Audio   :
			// Separo el path por las antibarras
			for (i = UltimaPos; i > PathBuscar.Tam(); i--) {
				if (Path[i - 1] == TEXT('\\')) {
					TmpStr = new DWL::DWLString(Path.SubStr(i, (UltimaPos - i)));
					Paths.insert(Paths.begin(), TmpStr);
					UltimaPos = i -1;
				}
			}
			// El nombre siempre esta al final
			_AnalizarNombre(*Paths[Paths.size() - 1]);
			if (Paths.size() == 2) { // solo hay un nombre y un disco
				InfoPath.Disco = Paths[0]->Texto();
			}
			if (Paths.size() == 3) { // hay un nombre un disco y un grupo
				InfoPath.Grupo = Paths[0]->Texto();
				InfoPath.Disco = Paths[1]->Texto();
			}
			if (Paths.size() > 3) { // puede haber carpetas de discos dobles (c:\mp3\grupo\disco\cd1\01 pista)
				switch (Orden) {
					case GeneroGrupoDisco :
						InfoPath.Genero = Paths[0]->Texto();
						InfoPath.Grupo = Paths[1]->Texto();
						InfoPath.Disco = Paths[2]->Texto();
						break;
					case GrupoDisco :
						InfoPath.Grupo = Paths[0]->Texto();
						InfoPath.Disco = Paths[1]->Texto();
						break;
					case Disco :
						InfoPath.Disco = Paths[0]->Texto();
						break;
					case Indefinido :
						InfoPath.Grupo = Paths[Paths.size() - 3]->Texto();
						InfoPath.Disco = Paths[Paths.size() - 2]->Texto();
						break;
				}
			}
			// Borro los paths de memoria
			for (i = 0; i < Paths.size(); i++) delete Paths[i];
			break;
		case Tipo_Medio_CDAudio :
		case Tipo_Medio_Video   :
			for (size_t i2 = UltimaPos; i2 != -1; i2--) {
				if (Path[i2] == TEXT('\\')) {
					_AnalizarNombre(Path.SubStr(i2 + 1, UltimaPos - i2));
					return;
				}
			}
			break;
		case Tipo_Medio_INDEFINIDO :
		case Tipo_Medio_Lista   :
			for (size_t i2 = UltimaPos; i2 != -1; i2--) {
				if (Path[i2] == TEXT('\\')) {
					InfoPath.Nombre = Path.SubStr(i2 + 1, UltimaPos - i2);
					return;
				}
			}
			break;
	}

}


// Funcion que retorna el tamaño en bytes del archivo especificado.
DWORD InformacionArchivoEx::_ObtenerLongitud(const TCHAR *Path) {
	HANDLE				Archivo;
	DWORD				Ret = 0;
/*	SECURITY_ATTRIBUTES Sec;
	Sec.nLength = sizeof(SECURITY_ATTRIBUTES);
	Sec.lpSecurityDescriptor = NULL;
	Sec.bInheritHandle = FALSE;*/
	Archivo = CreateFile(Path, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (GetLastError() == 0) {
		if(Archivo == INVALID_HANDLE_VALUE) 	throw; 
		Ret = GetFileSize(Archivo, NULL); 
		CloseHandle(Archivo);
//		SOLO_DEBUG(Sistema.App.LogBuscarArchivos.GuardarLog(TEXT("Longitud en bytes = %d"), Ret));
	}
	else {
		CloseHandle(Archivo);	
//		SOLO_DEBUG(Sistema.App.LogBuscarArchivos.GuardarLog(TEXT("Archivo no legible, Longitud 0"), Ret));
	}
	return Ret;
}


MetaDato *InformacionArchivoEx::_BuscarMetaDato(const TCHAR *Nombre, const TipoMetaDato Tipo) {
	for (size_t i = 0; i < MetaDatos.size(); i++) {
		if (MetaDatos[i]->Nombre.Comparar(Nombre) == true) { 
			if (Tipo == Tipo_NO_DEFINIDO || Tipo == MetaDatos[i]->Tipo)	return MetaDatos[i];
		}
	}
	return NULL;
}

MetaDato *InformacionArchivoEx::_BuscarGenero(void) {
	MetaDato *Ret = _BuscarMetaDato(TEXT("TCON"));
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("WM/Genre"), Tipo_WMA);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("GENRE"), Tipo_Vorbis);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("GENRE"), Tipo_ID3V1);
	return Ret;
}

MetaDato *InformacionArchivoEx::_BuscarGrupo(void) {
	MetaDato *Ret = _BuscarMetaDato(TEXT("TPE2"));
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("AUTHOR"), Tipo_WMA);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("ARTIST"), Tipo_Vorbis);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("ARTIST"), Tipo_ID3V1);
	return Ret;
}

MetaDato *InformacionArchivoEx::_BuscarDisco(void) {
	MetaDato *Ret = _BuscarMetaDato(TEXT("TALB"));
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("WM/AlbumTitle"), Tipo_WMA);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("ALBUM"), Tipo_Vorbis);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("ALBUM"), Tipo_ID3V1);
	return Ret;
}

MetaDato *InformacionArchivoEx::_BuscarNombre(void) {
	MetaDato *Ret = _BuscarMetaDato(TEXT("TIT2"));
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("TITLE"), Tipo_WMA);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("TITLE"), Tipo_Vorbis); // ni idea
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("TITLE"), Tipo_ID3V1);
	return Ret;
}

MetaDato *InformacionArchivoEx::_BuscarPista(void) {
	MetaDato *Ret = _BuscarMetaDato(TEXT("TRCK"));
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("WM/TrackNumber"), Tipo_WMA);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("TRACKNUMBER"), Tipo_Vorbis);
	if (Ret == NULL) Ret = _BuscarMetaDato(TEXT("TRACK"), Tipo_ID3V1);
	return Ret;
}


const Tipo_Medio InformacionArchivoEx::_TipoMedio(DWL::DWLString &PathFinal, TipoArchivo &TiposArchivo) {
	DWL::DWLString Extension;
    size_t         i = 0;
    // Obtenemos la extensión
    for (i = PathFinal.Tam() -1; i > 0; i--) {
        if (PathFinal[i] == TEXT('.')) {
            Extension = PathFinal.SubStr(i + 1);
            break;
        }
    }
    // No tiene extension.
    if (Extension.Tam() == 0) return Tipo_Medio_INDEFINIDO;
	TipoArchivo_Datos *TPD = TiposArchivo.BuscarTipoArchivo(Extension());
	if (TPD == NULL) return Tipo_Medio_INDEFINIDO;
	return TPD->Tipo;
}



// Función que borra la memoria almacenada en esta clase
void InformacionArchivoEx::_BorrarMemoria(void) {
	for (size_t i = 0; i < MetaDatos.size(); i++) delete MetaDatos[i];
	MetaDatos.resize(0);
	WMA.MetaDatos.resize(0);
	ID3V2.MetaDatos.resize(0);
	ID3V2.Tam			= 0; 
	ID3V2.SubVersion	= 0;
	ID3V2.Revision		= 0;
	ID3V2.UnSync		= 0;
	ID3V2.Extendido		= false;
	ID3V1.MetaDatos.resize(0);
	Vorbis.MetaDatos.resize(0);
	Vorbis.Vendor.Borrar();
};


// Retorna la posicion inicial del tag, -1 si no existe
const int InformacionArchivoEx::_BuscarVorbis(DWL::Archivos::DWLArchivoBinario &Archivo) { 
	char TagID[5] = "";
	char *pc = NULL;
	unsigned char Tipo;
	Archivo.Posicion(0);
//	Archivo.seekp(0, std::ios::beg);
	Archivo.Leer(TagID, 4);
	if (strcmp(TagID, "OggS"))	return -1;
//	char TagBuf[23];
	char *TagBuf = new char[23];
	int Status = 0;
	int Posicion = -1;
	Archivo.Leer(TagBuf, 23);
	pc = TagBuf;
	while (Status == 0) {
		unsigned char Segments;
		char *Enlace;
		unsigned int Pagina = 0, i;
		pc += 22;
		Segments = static_cast<unsigned char>(*pc);
		if (Segments == 0) {
			delete [] TagBuf;
			return -1; // tamaño invalido
		}
		Enlace = new char [Segments];
		Archivo.Leer(Enlace, Segments);
		for (i = 0; i < Segments; i++) Pagina += static_cast<unsigned char>(Enlace[i]);
		delete [] TagBuf;
		if (Pagina == 0) { 
			delete [] TagBuf;
			delete [] Enlace;
			return -1; // tamaño invalido
		}
		TagBuf = new char [Pagina];
		Archivo.Leer(TagBuf, Pagina);
		pc = TagBuf;
		for (i = 0; i < Segments && Status == 0;) {
			if (strncmp(pc + 1, "vorbis", 6) == 0) {
				Tipo = static_cast<unsigned char>(*pc);
				if (Tipo == 0x03) {
					Posicion = static_cast<int>(Archivo.Posicion()) - Pagina + (pc - TagBuf);
					Status = 1;
				}
			}
			pc += static_cast<unsigned char>(Enlace[i++]);
		}
		if (Status == 1 || Archivo.FinalDelArchivo()) {
			delete [] Enlace;
			break;
		}
		delete [] TagBuf;
		TagBuf = new char [27];
		Archivo.Leer(TagBuf, 27);
		pc = TagBuf + 4;
		delete [] Enlace;
	}
	delete [] TagBuf;
	if (Archivo.FinalDelArchivo())	return -1;
	else							return Posicion;
};

// Retorna la posicion inicial del tag, -1 si no existe
const int InformacionArchivoEx::_BuscarWMA(DWL::Archivos::DWLArchivoBinario &Archivo) {
	unsigned char WMA_GUID[16]			= {	0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C };
	unsigned char WMA_CONTENT_GUID[16]	= {	0x33, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C };
	char TagBuffer[TAM_BUFFER];
	char *pc = NULL;
	Archivo.Posicion(0);
//	Archivo.seekp(0, std::ios::beg);
	Archivo.Leer(TagBuffer, TAM_BUFFER);
	pc = TagBuffer;
	if (memcmp(pc, WMA_GUID, 16))			return -1;
	pc += 30;
	if (memcmp(pc, WMA_CONTENT_GUID, 16))	return -1;
	return pc - TagBuffer + 16;
};

// Retorna 1 si exsite, -1 si no existe.
const int InformacionArchivoEx::_BuscarID3V1(DWL::Archivos::DWLArchivoBinario &Archivo) {
	Archivo.Posicion(-128, true);
//	Archivo.seekp(-128, std::ios::end);
	char TagID[4] = "";
	Archivo.Leer(TagID, 3);
	if (strncmp(TagID, "TAG", 3) == 0)	return 1;
	else								return -1;
};

// Retorna la posicion inicial del tag, -1 si no existe
const int InformacionArchivoEx::_BuscarID3V2(DWL::Archivos::DWLArchivoBinario &Archivo) {
	char TagBuffer[TAM_BUFFER];
	char *pc = TagBuffer;
	int Estado = 0; // Estado 1 encontrado ID3
	int CaracteresLeidos = 0;
	int Busqueda = -1; // -1 Principio, -2 Final
	int Pos = 0;
	Archivo.Posicion(0);
//								Archivo.seekp(0, std::ios::beg);
	CaracteresLeidos = Archivo.Leer(TagBuffer, 10);
	while (Archivo.FinalDelArchivo() == false && Estado == 0) {
		if (Busqueda == -1) {
			if ((strncmp(pc, "ID3", 3) == 0 || strncmp(pc, "3DI", 3) == 0))	{ 
				Estado = 1;
			}
			else { // el tag no esta al principio
				Archivo.Posicion(-3, true);
				CaracteresLeidos = Archivo.Leer(TagBuffer, 3);
				Busqueda = -2;
			}
		}
		else {
			if (Busqueda == -2) { // Iniciamos busqueda por el final
				pc = TagBuffer;
				Pos = static_cast<int>(Archivo.Posicion());
				if ((strncmp(pc, "ID3", 3) == 0 || strncmp(pc, "3DI", 3) == 0)) Estado = 1;
				Busqueda = 1;
			}
			if (Estado != 1) {
				//Pos = static_cast<int>(Archivo.Posicion()) - TAM_BUFFER;
				Archivo.Posicion(Pos - TAM_BUFFER);
				CaracteresLeidos = Archivo.Leer(TagBuffer, TAM_BUFFER);
				pc = TagBuffer;
				for (int i = 0; (i < CaracteresLeidos - 3 && Estado == 0); i++) {
					pc ++;
					if ((strncmp(pc, "ID3", 3) == 0 || strncmp(pc, "3DI", 3) == 0))
						Estado = 1;
				}
				if (Estado == 1) 
					Pos += pc - TagBuffer;
				Pos -= TAM_BUFFER - 9;
				
//				if ((Pos < -TAM_BUFFER + 9 || Archivo.fail()) && Estado != 1) Estado = -1; // que coño es el fail
				if ((Pos < -TAM_BUFFER + 9) && Estado != 1) 
					Estado = -1;						
//				Pos += 9;
			}
		}
		// Miro si coincide el resto del header, o en caso contrario asigno estado a 0
		if (Estado == 1 && *(pc + 3) < 0xFF && *(pc + 4) < 0xFF &&	*(pc + 6) < 0x80 && *(pc + 7) < 0x80 &&	*(pc + 8) < 0x80 && *(pc + 9) < 0x80)	
			Estado = 1;
		else if (Estado != -1)
			Estado = 0;

		//if (Busqueda == 0)	Busqueda = -1;
	}
	if (Estado < 0 || Archivo.FinalDelArchivo())	return -1;
	else											return Pos;
};

void InformacionArchivoEx::_ObtenerDatosID3V1(DWL::Archivos::DWLArchivoBinario &Archivo) {
//	char *pc = TagV1;

	char TagV1[30];
	// Nombre
	Archivo.Leer(TagV1, 30);
	MetaDato *TmpMeta;
	TmpMeta = new MetaDato;
	TmpMeta->Tipo = Tipo_ID3V1;
	TmpMeta->Datos.AgregarTexto(TagV1, 29);
	TmpMeta->Nombre = TEXT("Title");
	if (TmpMeta->Datos.Tam() == 0) {
		delete TmpMeta;
	}
	else {							
		MetaDatos.push_back(TmpMeta);
		ID3V1.MetaDatos.push_back(TmpMeta);
	}

	// Grupo
	Archivo.Leer(TagV1, 30);
	TmpMeta = new MetaDato;
	TmpMeta->Tipo = Tipo_ID3V1;
	TmpMeta->Datos.AgregarTexto(TagV1, 29);
	TmpMeta->Nombre = TEXT("Artist");
	if (TmpMeta->Datos.Tam() == 0) {
		delete TmpMeta;
	}
	else {							
		MetaDatos.push_back(TmpMeta);
		ID3V1.MetaDatos.push_back(TmpMeta);
	}

	// Disco
	Archivo.Leer(TagV1, 30);
	TmpMeta = new MetaDato;
	TmpMeta->Tipo = Tipo_ID3V1;
	TmpMeta->Datos.AgregarTexto(TagV1, 29);
	TmpMeta->Nombre = TEXT("Album");
	if (TmpMeta->Datos.Tam() == 0) {
		delete TmpMeta;
	}
	else {							
		MetaDatos.push_back(TmpMeta);
		ID3V1.MetaDatos.push_back(TmpMeta);
	}

	// Año
	Archivo.Leer(TagV1, 4);
	TmpMeta = new MetaDato;
	TmpMeta->Tipo = Tipo_ID3V1;
	TmpMeta->Datos.AgregarTexto(TagV1, 4);
	TmpMeta->Datos += TEXT('\0');
	TmpMeta->Nombre = TEXT("Year");
	if (TmpMeta->Datos.Tam() == 0) {
		delete TmpMeta;
	}
	else {							
		MetaDatos.push_back(TmpMeta);
		ID3V1.MetaDatos.push_back(TmpMeta);
	}

	// Comentario
	Archivo.Leer(TagV1, 28);
	TmpMeta = new MetaDato;
	TmpMeta->Tipo = Tipo_ID3V1;
	TmpMeta->Datos.AgregarTexto(TagV1, 28);
	TmpMeta->Nombre = TEXT("Comment");
	if (TmpMeta->Datos.Tam() == 0) {
		delete TmpMeta;
	}
	else {							
		MetaDatos.push_back(TmpMeta);
		ID3V1.MetaDatos.push_back(TmpMeta);
	}
		
	// Pista
	Archivo.Leer(TagV1, 2);
	if (TagV1[0] == '\0' && TagV1[1] != '\0') {
		TmpMeta = new MetaDato;
		TmpMeta->Tipo = Tipo_ID3V1;
		TmpMeta->Datos = static_cast<int>(TagV1[1]);
		TmpMeta->Nombre = TEXT("Track");
		if (TmpMeta->Datos.Tam() == 0) {
			delete TmpMeta;
		}
		else {
			ID3V1.MetaDatos.push_back(TmpMeta);
			MetaDatos.push_back(TmpMeta);
			ID3V1.SubVersion = 1;
		}
	}

	// Genero
	Archivo.Leer(TagV1, 1);
	TmpMeta = new MetaDato;
	TmpMeta->Tipo = Tipo_ID3V1;
	// genero numerico de 1 byte
	TmpMeta->Datos = static_cast<unsigned char>(TagV1[0]);
	TmpMeta->Nombre = TEXT("Genre");
	if (TmpMeta->Datos.Tam() == 0) {
		delete TmpMeta;
	}
	else {							
		MetaDatos.push_back(TmpMeta);
		ID3V1.MetaDatos.push_back(TmpMeta);
	}				
};

// La cabecera del ID3TAGV2 son 10 bytes distribuidos de la siguiente manera :
// CCCSROLLLL
// 0123456789
//
// C  = Identificador de 3 bytes "ID3" para el principio o "3DI" si esta al final
// SR = Version de 2 bytes, el primero es la subversion y el segundo es la revision
// O  = Opciones de 1 byte [0x80 Unsync] [0x40 Extendido] 
// L  = Longitud del tag 4 bytes
void InformacionArchivoEx::_ObtenerDatosID3V2(DWL::Archivos::DWLArchivoBinario &Archivo, const int Posicion) {
	char TmpBuf[10];
	char *TagBuf;
	char *pc = TmpBuf;
	bool AlFinal = false;
	MetaDato *TmpMeta = NULL;
	Archivo.Posicion(Posicion);
	Archivo.Leer(TmpBuf, 10); // Leo toda la cabecera
	if (TmpBuf[0] == '3' && TmpBuf[1] == 'D' && TmpBuf[2] == 'I')	AlFinal = true;
//					// Obtengo el tamaño del tag y su version
	ID3V2.SubVersion = TmpBuf[3];
	ID3V2.Revision   = TmpBuf[4];
	if (ID3V2.SubVersion > 4) return; // subversion no soportada 
	if ((TmpBuf[5] & 0x80) == 0x80) ID3V2.UnSync = true;
	if ((TmpBuf[5] & 0x40) == 0x40 && ID3V2.SubVersion > 0x02) ID3V2.Extendido = true;
	ID3V2.Tam = ((static_cast<unsigned char>(TmpBuf[6]) << 21) | (static_cast<unsigned char>(TmpBuf[7]) << 14) | (static_cast<unsigned char>(TmpBuf[8]) << 7) | (static_cast<unsigned char>(TmpBuf[9]) << 0));
	if (AlFinal == true) Archivo.Posicion(Archivo.Posicion() + (-10 - ID3V2.Tam));
	// Leo el resto del tag en un buffer
	// Los tags no pueden superar los 268435456 bytes (256MB), de todas formas lo limitare a 10MB ya que es una burrada, y asi me evito validaciones fallidas de la cabecera del ID3V2
	if (ID3V2.Tam > 10000000) return;
	TagBuf = new char[ID3V2.Tam];
	BOOL CaracteresLeidos = Archivo.Leer(TagBuf, ID3V2.Tam);
	if (ID3V2.Tam != CaracteresLeidos) { // No coincide el buffer con los caracteres leidos, el buscador ha encontrado una coincidencia falsa
//		_ASSERT(0);
		delete [] TagBuf;
		return;
	}
	pc = TagBuf;
	if (ID3V2.Extendido == true) { // Si existe un tag extendido lo omitimos
		memcpy(TmpBuf, pc, 4);
		pc += 4;
		if (ID3V2.SubVersion == 0x03 && ID3V2.UnSync == true) _ID3V2_UnSync(TmpBuf, pc);

		if (ID3V2.SubVersion == 0x03)	pc += ((static_cast<unsigned char>(TmpBuf[0]) << 21) | (static_cast<unsigned char>(TmpBuf[1]) << 14) | (static_cast<unsigned char>(TmpBuf[2]) << 7) | (static_cast<unsigned char>(TmpBuf[3]) << 0));
		else							pc += ((static_cast<unsigned char>(TmpBuf[3]) << 0) | (static_cast<unsigned char>(TmpBuf[2]) << 8) | (static_cast<unsigned char>(TmpBuf[1]) << 16) | (static_cast<unsigned char>(TmpBuf[0]) << 24));
	}
	// leer frames
	char *Fin = TagBuf + ID3V2.Tam;
	while (pc < Fin) {
		if (*pc == '\0') break;
		TmpMeta = _ID3V2_ParsearFrame(&pc, Fin);
		if (TmpMeta != NULL) {
			if (TmpMeta->Datos.Tam() == 0)	{
				delete TmpMeta;
			}
			else {
				ID3V2.MetaDatos.push_back(TmpMeta);
				MetaDatos.push_back(TmpMeta);
			}
		}
	}

	delete [] TagBuf;
};

Meta_ID3V2_UnSync *InformacionArchivoEx::_ID3V2_MirarSync(char *Datos, UINT Tam) {
	Meta_ID3V2_UnSync  *Sync	= NULL;
	UINT				i		= 0;
	UINT				z		= 0;
	Sync = new Meta_ID3V2_UnSync;
	Sync->Datos = Datos;
	Sync->Total = 0;
	if (Tam == 0) Tam = strlen(Sync->Datos);
	for (i = 0; i < Tam; i++) {
		if (Sync->Datos[i] == 0xFF && Sync->Datos[i + 1] == 0x00) {
			for (z = i + 1; z < Tam -1; z++) Datos[z] = Datos[z + 1];
			Sync->Datos[z] = '\0';
			Sync->Total ++;
		}
	}
	return Sync;
};

void InformacionArchivoEx::_ID3V2_UnSync(char *Datos, char *pc) {
	Meta_ID3V2_UnSync  *UnSync		= NULL;
	char			   *UnSyncTmp	= NULL;
	UINT				i			= 0;
	UnSync = _ID3V2_MirarSync(Datos, 0);
	while (UnSync->Total > 0) {
		if (UnSyncTmp != NULL) delete [] UnSyncTmp;
		UnSyncTmp = new char[UnSync->Total];
		memcpy(UnSyncTmp, pc, UnSync->Total);
		pc += UnSync->Total;
		for (i = 0; i < UnSync->Total; i++) {
			Datos[4 - UnSync->Total + i] = UnSyncTmp[i];
		}
		delete UnSync;
		UnSync = _ID3V2_MirarSync(Datos, 0);
	}
	delete UnSync;
	delete [] UnSyncTmp;
};

MetaDato *InformacionArchivoEx::_ID3V2_ParsearFrame(char **pc, char *Fin) {
	char FrameID[5];
	char OpcionesFrame[2] = "";
	char TmpChar[5];
	char *Ptr, *Datos = NULL, *Utf = NULL;
	int  TamFrame;
	MetaDato *DatosFrame = NULL;
	switch (ID3V2.SubVersion) {
		case 2 : // Version 2.2
			if (Fin - *pc < 6) return NULL;
			memcpy(FrameID, *pc, 3);
			FrameID[3] = 0;
			*pc += 3;
			memcpy(TmpChar, *pc, 3);
			if (ID3V2.UnSync == true) _ID3V2_UnSync(TmpChar, *pc);
			TamFrame = ((static_cast<unsigned char>(TmpChar[2]) << 0) | (static_cast<unsigned char>(TmpChar[1]) << 8) | (static_cast<unsigned char>(TmpChar[0]) << 16));
			*pc += 3;
			break;
		case 3 : // Version 2.3
			if (Fin - *pc < 10) return NULL;
			memcpy(FrameID, *pc, 4);
			FrameID[4] = 0;
			*pc += 4;
			memcpy(TmpChar, *pc, 4);
			if (ID3V2.UnSync == true) _ID3V2_UnSync(TmpChar, *pc);
			TamFrame = ((static_cast<unsigned char>(TmpChar[3]) << 0) | (static_cast<unsigned char>(TmpChar[2]) << 8) | (static_cast<unsigned char>(TmpChar[1]) << 16) | (static_cast<unsigned char>(TmpChar[0]) << 24));
			*pc += 4;
			memcpy(OpcionesFrame, *pc, 2);
			*pc += 2;
			break;
		case 4 : // Version 2.4
			if (Fin - *pc < 10) return NULL;
			memcpy(FrameID, *pc, 4);
			FrameID[4] = 0;
			*pc += 4;
			memcpy(TmpChar, *pc, 4);
			TamFrame = ((static_cast<unsigned char>(TmpChar[0]) << 21) | (static_cast<unsigned char>(TmpChar[1]) << 14) | (static_cast<unsigned char>(TmpChar[2]) << 7) | (static_cast<unsigned char>(TmpChar[3]) << 0));
			*pc += 4;
			break;
		default :
			return NULL;
	}
	if (Fin - *pc > 0) {
		if (TamFrame > (Fin - *pc)) return NULL;
	}
	else { // 
		return NULL;
	}
	DatosFrame = new MetaDato;
	DatosFrame->Nombre = FrameID;
	DatosFrame->Tipo = Tipo_ID3V2;
	if (ID3V2.UnSync == true) OpcionesFrame[1] |= 0x02;
	if (ID3V2.SubVersion == 0x04) {
		if ((OpcionesFrame[1] & 0x08) == 0x08 || (OpcionesFrame[1] & 0x01) == 0x01) {
			*pc += 4;
			TamFrame -= 4;
		}
		if ((OpcionesFrame[1] & 0x04) == 0x04) {
			*pc ++;
			TamFrame --;
		}
		if ((OpcionesFrame[1] & 0x40) == 0x40) {
			*pc++;
			TamFrame --;
		}
	}
	else if (ID3V2.SubVersion == 0x03) {
		if ((OpcionesFrame[1] & 0x80) == 0x80) {
			char tmp[4];
			memcpy(tmp, *pc, 4);
			*pc += 4;
			if ((OpcionesFrame[1] & 0x02) == 0x02)	_ID3V2_UnSync(tmp, *pc);
			TamFrame -= 4;
		}
		if ((OpcionesFrame[1] & 0x40) == 0x40)	{
			*pc ++;
			TamFrame --;
		}
		if((OpcionesFrame[1] & 0x20) == 0x20) {
			*pc ++;
			TamFrame --;
		}
	}

//	if (FrameID[0] == 'T' && strcmp(FrameID, "TXXX") && strcmp(FrameID, "TXX")) {	
	if (TamFrame < 2) return DatosFrame; 
	// Miramos si es un frame de lyrics (des-sincronizados)
	Ptr = *pc;
	UINT Contador = 0 ;
	UINT Codificado = *(Ptr++);
	// USLT KISS THE VERY BEST OF KISS -> e n g -1 -2 0 0 -1 -2 UTF16
	if (DatosFrame->Nombre == TEXT("USLT")) {
//		Ptr = *pc;
		//*pc += 3; // saltamos el idioma
		//TamFrame -=4;
		do {
			Contador ++;
		} while (Ptr[Contador] != 0);
		Ptr += Contador + 2;
		TamFrame -= Contador + 2;
	}

	Datos = new char[TamFrame];
	Datos[TamFrame - 1] = '\0'; //*(Datos + TamFrame -1) = '\0'; // Datos[TamFrame - 1] = 0;
	memcpy(Datos, Ptr, TamFrame -1);
	if ((OpcionesFrame[1] & 0x02) == 0x02) {
		Meta_ID3V2_UnSync *UnSync = _ID3V2_MirarSync(Datos, TamFrame);
		TamFrame -= UnSync->Total;
		delete UnSync;
	}
	DatosFrame->Datos.Borrar();
	switch (Codificado) {
		case 0x00 : // iso 88591
			DatosFrame->Datos.AgregarTexto(Datos);
			break;
		case 0x01 : // utf16
			DatosFrame->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(Datos), ((TamFrame * sizeof(char)) / sizeof(wchar_t)) );
			break;
		case 0x02 : // utf16 be
			DatosFrame->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(Datos), ((TamFrame * sizeof(char)) / sizeof(wchar_t)));
			break;
		case 0x03 : // ASCII
			DatosFrame->Datos.AgregarTexto(Datos);
			break;
//			default :
//			_ASSERT(0);
	//		break;
	}
	delete [] Datos;
//	}
	/*else { // UnSync
		Ptr = *pc;
		Datos = new char[TamFrame];
		memcpy(Datos, Ptr, TamFrame);
		if ((OpcionesFrame[1] & 0x02) == 0x02) {
			Meta_ID3V2_UnSync *UnSync = ID3V2_MirarSync(Datos, TamFrame);
			TamFrame -= UnSync->Total;
			delete UnSync;
		}
		DatosFrame->Datos = Datos;
	}*/

	*pc += TamFrame;
	return DatosFrame;
};


void InformacionArchivoEx::_ObtenerDatosWMA(DWL::Archivos::DWLArchivoBinario &Archivo, const int Posicion) { 
	unsigned char	WMA_EXTENDED_GUID[16] = {	0x40, 0xA4, 0xD0, 0xD2, 0x07, 0xE3, 0xD2, 0x11, 0x97, 0xF0, 0x00, 0xA0, 0xC9, 0x5E, 0xA8, 0x50 };
	MetaDato       *TmpMeta = NULL;
	char			TmpBuf[8];
	char           *TagBuf;
	char           *pc = NULL;
	int				TotalItems = 0;
	
	Archivo.Posicion(Posicion);
	Archivo.Leer(TmpBuf, 8); // Leo el tamaño de la cabecera (64 bits)
	UINT Tam = ((static_cast<unsigned char>(TmpBuf[0]) << 0) | (static_cast<unsigned char>(TmpBuf[1]) << 8) | (static_cast<unsigned char>(TmpBuf[2]) << 16) | (static_cast<unsigned char>(TmpBuf[3]) << 24));
	if (Tam == 0) return; // tamaño invalido
	TagBuf = new char [Tam];
	Archivo.Leer(TagBuf, Tam - 24); // Leo el tag base en memoria (faltara por leer la parte extendida)
	pc = TagBuf;
	int Tam_Titulo = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
	pc += 2;
	int Tam_Autor = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
	pc += 2;
	int Tam_Copyright = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
	pc += 2;
	int Tam_Descripcion = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
	pc += 2;
	int Tam_Nota = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
	pc += 2;
	if (Tam_Titulo > 0) {
		TmpMeta = new MetaDato;
		TmpMeta->Nombre = TEXT("WM/Title");
		TmpMeta->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Titulo);
		TmpMeta->Tipo = Tipo_WMA;
		MetaDatos.push_back(TmpMeta);
		WMA.MetaDatos.push_back(TmpMeta);
		pc += Tam_Titulo;
		TotalItems ++;
	}
	if (Tam_Autor > 0) {
		TmpMeta = new MetaDato;
		TmpMeta->Nombre = TEXT("WM/Author");
		TmpMeta->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Autor);
		TmpMeta->Tipo = Tipo_WMA;
		MetaDatos.push_back(TmpMeta);
		WMA.MetaDatos.push_back(TmpMeta);
		pc += Tam_Autor;
		TotalItems ++;
	}
	if (Tam_Copyright > 0) {
		TmpMeta = new MetaDato;
		TmpMeta->Nombre = TEXT("WM/Copyright");
		TmpMeta->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Copyright);
		TmpMeta->Tipo = Tipo_WMA;
		MetaDatos.push_back(TmpMeta);
		WMA.MetaDatos.push_back(TmpMeta);
		pc += Tam_Copyright;
		TotalItems ++;
	}
	if (Tam_Descripcion > 0) {
		TmpMeta = new MetaDato;
		TmpMeta->Nombre = TEXT("WM/Description");
		TmpMeta->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Descripcion);
		TmpMeta->Tipo = Tipo_WMA;
		MetaDatos.push_back(TmpMeta);
		WMA.MetaDatos.push_back(TmpMeta);
		pc += Tam_Descripcion;
		TotalItems ++;
	}
	if (Tam_Nota > 0) {
		TmpMeta = new MetaDato;
		TmpMeta->Nombre = TEXT("WM/Rating");
		TmpMeta->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Nota);
		TmpMeta->Tipo = Tipo_WMA;
		MetaDatos.push_back(TmpMeta);
		WMA.MetaDatos.push_back(TmpMeta);
		pc += Tam_Nota;
		TotalItems ++;
	}
	// Parte extendida del tag
	Archivo.Leer(TagBuf, 16); // Leo la cabecera del tag extendido
	if (memcmp(TagBuf, WMA_EXTENDED_GUID, 16)) { 
		delete [] TagBuf;
		return; // No hay tag extendido salgo.
	}
	Archivo.Leer(TmpBuf, 8); // Leo el tamaño del tag extendido
	Tam = ((static_cast<unsigned char>(TmpBuf[0]) << 0) | (static_cast<unsigned char>(TmpBuf[1]) << 8) | (static_cast<unsigned char>(TmpBuf[2]) << 16) | (static_cast<unsigned char>(TmpBuf[3]) << 24));
	delete [] TagBuf;
	if (Tam == 0) return; // tamaño invalido
	TagBuf = new char[Tam];
	Archivo.Leer(TagBuf, Tam); // Leo el tag extendido en memoria
	pc = TagBuf;
	int NumItems = TotalItems + ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
	pc += 2;
	int Tipo = 0;
	for (int i = TotalItems; i < NumItems; i++) {
		Tam = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8)); // Tamaño del nombre
		pc += 2;
		TmpMeta = new MetaDato;
		TmpMeta->Tipo = Tipo_WMA;
		TmpMeta->Nombre.AgregarTexto(reinterpret_cast<wchar_t *>(pc), Tam);
		pc += Tam;
		Tipo = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8)); // Tipo de datos
		pc += 2;
		Tam = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8)); // Tamaño de los datos
		pc += 2;
		switch (Tipo) {
			case 0 : // UTF16 Little Endian
				TmpMeta->Datos.AgregarTexto(reinterpret_cast<wchar_t *>(pc), Tam);
				break;
			case 1 : // Binario
				break;
			case 2 : // Bool
				break;
			case 3 : // int32
				break;
			case 4 : // double
				break;
			default :
				TmpMeta->Datos.AgregarTexto(pc, Tam);
				break;
		}
		pc += Tam;
		if (TmpMeta->Datos.Tam() > 0) {
			MetaDatos.push_back(TmpMeta);
			WMA.MetaDatos.push_back(TmpMeta);
		}
		else {
			delete TmpMeta;
		}
	}
	delete [] TagBuf;
};

// C:\mp3\into eternity\Into Eternity - Into Eternity (2000) [bh] 128 vbr ogg\Into Eternity - 01 - Torn.ogg (ejemplo ogg)
void InformacionArchivoEx::_ObtenerDatosVorbis(DWL::Archivos::DWLArchivoBinario &Archivo, const int Posicion) { 
	char Tmp[4];
	char *Tmp2;
	int i;
	MetaDato *TmpMeta;
	Archivo.Posicion(Posicion + 7);
//	Archivo.seekp(Posicion + 7, std::ios::beg);
	Archivo.Leer(Tmp, 4);
	int Tam = ((static_cast<unsigned char>(Tmp[0]) << 0) | (static_cast<unsigned char>(Tmp[1]) << 8) | (static_cast<unsigned char>(Tmp[2]) << 16) | (static_cast<unsigned char>(Tmp[3]) << 24));
	if (Tam == 0) return; // tamaño invalido
	Tmp2 = new char [Tam];
	Archivo.Leer(Tmp2, Tam);
	Vorbis.Vendor.Borrar();
	Vorbis.Vendor.AgregarTexto(Tmp2, 29);
	delete [] Tmp2;
	Archivo.Leer(Tmp, 4);
	int TotalItems = ((static_cast<unsigned char>(Tmp[0]) << 0) | (static_cast<unsigned char>(Tmp[1]) << 8) | (static_cast<unsigned char>(Tmp[2]) << 16) | (static_cast<unsigned char>(Tmp[3]) << 24));

	for (i = 0; i < TotalItems; i++) {
		char *data, *pc;
		Archivo.Leer(Tmp, 4);
		Tam = ((static_cast<unsigned char>(Tmp[0]) << 0) | (static_cast<unsigned char>(Tmp[1]) << 8) | (static_cast<unsigned char>(Tmp[2]) << 16) | (static_cast<unsigned char>(Tmp[3]) << 24));
		data = new char [Tam];
		Archivo.Leer(data, Tam);
		pc = strchr(data, '=');
		*pc = '\0';
		pc++;
		TmpMeta = new MetaDato;
		TmpMeta->Tipo = Tipo_Vorbis;
		TmpMeta->Nombre = data;
		TmpMeta->Datos.AgregarTexto(pc, (Tam - TmpMeta->Nombre.Tam()) -1);
		MetaDatos.push_back(TmpMeta);
		Vorbis.MetaDatos.push_back(TmpMeta);
		delete [] data;
	}
};


