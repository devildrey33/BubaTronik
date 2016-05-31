
#include "StdAfx.h"
#include "BaseDatos.h"
#include "ControlPresentacionEx.h"
#include "ArbolBaseDatos.h"
#include "resource.h"
#include "AnomaliasStrTp.h"
#include "BubaTronikApp.h"
#include "InformacionArchivoEx.h"

// Version de la bd
#define PLAYER_BD_VERSION 160




BaseDatos::BaseDatos(void) {
	Revisando = false;
	for (size_t i = 0; i < 10; i++) _MediosRecientes[i] = NULL;
}

BaseDatos::~BaseDatos(void) {
}


void BaseDatos::AgregarMedioReciente(TMedio *nMedio) {
	size_t i;
	// Desplazamos la lista eliminando el ultimo TMedio
	for (i = 9; i > 0; i--) {
		_MediosRecientes[i] = _MediosRecientes[i - 1];
	}

	// Si llegamos aqui es que la lista está llena
	_MediosRecientes[0] = nMedio;
}

const bool BaseDatos::Iniciar(const TCHAR *Path) {
	UINT MediosIgnorados = 0;
	DWL::DWLString PathBD = Path; PathBD += TEXT("BD.BTR");
	ArchivoBinarioPlayer BDG;
	BDG.AbrirArchivoLectura(PathBD());
	if (BDG.EstaAbierto() == false) {
		return false;
	}
	try {
		int BDVer = 0;
		BDG.LeerInt(BDVer);
		switch (BDVer) {
			case 101 : // 0.57
				_CargarTipos(BDG);
				_CargarGeneros(BDG);
				_CargarGrupos(BDG);
				_CargarDiscos(BDG);
				MediosIgnorados = _CargarMedios057(BDG);
				break;
			case 160 : // 0.60
				_CargarTipos(BDG);
				_CargarGeneros(BDG);
				_CargarGrupos(BDG);
				_CargarDiscos(BDG);
				_CargarMedios(BDG);
				_CargarMediosRecientes(BDG);
				break;
			default :
				static VentanaMensaje_Aceptar Msg(IDIOMA__LA_VERSION_DE_LA_BASE_DE_DATOS_NO_ES_VALIDA_PARA_ESTE_REPRODUCTOR, IDIOMA__ERROR, 0);
				break;
		}
	}
	catch (...) { 
		size_t i = 0;
		for (i = 0; i < Tipos.size();      i++) delete Tipos[i];
		for (i = 0; i < Generos.size();    i++) delete Generos[i];
		for (i = 0; i < Grupos.size();     i++) delete Grupos[i];
		for (i = 0; i < Discos.size();     i++) delete Discos[i];
		for (i = 0; i < Medios.size();	   i++) delete Medios[i];
		for (i = 0; i < StrTipos.size();   i++) delete StrTipos[i];
		for (i = 0; i < StrGeneros.size(); i++) delete StrGeneros[i];
		for (i = 0; i < StrGrupos.size();  i++) delete StrGrupos[i];
		for (i = 0; i < StrDiscos.size();  i++) delete StrDiscos[i];
		Tipos.resize(0);
		Generos.resize(0);
		Grupos.resize(0);
		Discos.resize(0);
		Medios.resize(0);
		StrTipos.resize(0);
		StrGeneros.resize(0);
		StrGrupos.resize(0);
		StrDiscos.resize(0);
        VentanaMensaje_Aceptar MsgErr(IDIOMA__LA_BASE_DE_DATOS_PARECE_ESTAR_DANADA, IDIOMA__ERROR, 0);
	}
	
	if (MediosIgnorados > 0) {
		DWL::DWLString Str;
		
		Str.sprintf(IDIOMA__HAY_D_MEDIOS_QUE_NO_SE_HAN_CARGADO_DEBIDO_A_QUE_SU_INFORMACION_PODRIA_SER_INVALIDA, MediosIgnorados);
		static VentanaMensaje_Aceptar Msg(Str(), IDIOMA__IGNORADA_CARGA_DE_ALGUNOS_MEDIOS, 0);
	}

	return true;
}



StrTp *BaseDatos::AgregarStrTp(DWL::DWLString *Str, std::vector<StrTp *> &Vec) {
	unsigned int R = 0;
	for (R = 0; R < Vec.size(); R++) {
		if (DWLStrCmpi(Str->Texto(), Vec[R]->Nombre.Texto()) == 0) {
			return Vec[R];
		}
	}
	StrTp *TMP = new StrTp;
	TMP->Nombre.AgregarTexto(Str->Texto());
	TMP->Escuchado = 0;
	Vec.push_back(TMP);
	return TMP;
}


StrTp *BaseDatos::AgregarStrTp(const TCHAR *Str, std::vector<StrTp *> &Vec) {
	unsigned int R = 0;
	for (R = 0; R < Vec.size(); R++) {
		if (DWLStrCmpi(Str, Vec[R]->Nombre()) == 0) {
			return Vec[R];
		}
	}
	StrTp *TMP = new StrTp;
	TMP->Nombre.AgregarTexto(Str);
	TMP->Escuchado = 0;
	Vec.push_back(TMP);
	return TMP;
}



void BaseDatos::Guardar(const TCHAR *Path) {
	size_t TotalCanciones = Medios.size();
	if (Medios.size() == 0) {
		return;
	}
	DWL::DWLString Pt = Path; Pt += TEXT("BD.BTR");
	DeleteFile(Pt());
	ArchivoBinarioPlayer BDG;
	BDG.AbrirArchivoEscritura(Pt());
	int BDVer = PLAYER_BD_VERSION;
	BDG.GuardarInt(BDVer);
	
	_GuardarTipos(BDG); 
	_GuardarGeneros(BDG);
	_GuardarGrupos(BDG);
	_GuardarDiscos(BDG);
	_GuardarMedios(BDG);
	_GuardarMediosRecientes(BDG);
	BDG.CerrarArchivo();
}




void BaseDatos::_GuardarTipos(ArchivoBinarioPlayer &BDG) {
	size_t Tmp = StrTipos.size();
	BDG.GuardarSizeT(Tmp); // Numero de tipos
	for (size_t i = 0; i < StrTipos.size(); i++) {
		BDG.GuardarString(StrTipos[i]->Nombre);
		BDG.GuardarDword(StrTipos[i]->Escuchado); // Veces que se ha eschuchado el tipo
	} 
}


void BaseDatos::_CargarTipos(ArchivoBinarioPlayer &BDG) {
	size_t  Num = 0;
	size_t  Tam = 0;
	StrTp  *TMP = NULL;
	BDG.LeerSizeT(Num); // Leo numero de tipos
	for (size_t i = 0; i < Num; i++) {
		TMP = new StrTp;
		BDG.LeerString(TMP->Nombre);
		BDG.LeerDword(TMP->Escuchado); // Numero de veces que se ha escuchado
		StrTipos.push_back(TMP);
	}
}



void BaseDatos::_GuardarGeneros(ArchivoBinarioPlayer &BDG) {
	size_t N = StrGeneros.size();
	BDG.GuardarSizeT(N); // Numero de Generos
	for (size_t i = 0; i < StrGeneros.size(); i++) {
		BDG.GuardarString(StrGeneros[i]->Nombre);
		BDG.GuardarDword(StrGeneros[i]->Escuchado); // Numero de tipos
	} 
}


void BaseDatos::_CargarGeneros(ArchivoBinarioPlayer &BDG) {
	size_t Num = 0;
	size_t Tam = 0;
	StrTp *TMP = NULL; 
	BDG.LeerSizeT(Num); // Leo numero de generos
	for (size_t i = 0; i < Num; i++) {
		TMP = new StrTp;
		BDG.LeerString(TMP->Nombre);
		BDG.LeerDword(TMP->Escuchado); // Leo numero de veces que se ha escuchado
		StrGeneros.push_back(TMP);
	}
}


void BaseDatos::_GuardarGrupos(ArchivoBinarioPlayer &BDG) {
	size_t N = StrGrupos.size();
	BDG.GuardarSizeT(N); // Numero de Grupos
	for (size_t i = 0; i < StrGrupos.size(); i++) {
		BDG.GuardarString(StrGrupos[i]->Nombre);
		BDG.GuardarDword(StrGrupos[i]->Escuchado); // Veces que se ha escuchado
	} 
}



void BaseDatos::_CargarGrupos(ArchivoBinarioPlayer &BDG) {
	size_t Num = 0;
	size_t Tam = 0;
	StrTp *TMP = NULL; 
	BDG.LeerSizeT(Num); // Leo numero de grupos
	for (unsigned int i = 0; i < Num; i++) {
		TMP = new StrTp;
		BDG.LeerString(TMP->Nombre);// = TMP;
		BDG.LeerDword(TMP->Escuchado); // Veces que se ha escuchado
		StrGrupos.push_back(TMP);
	}
}




void BaseDatos::_GuardarDiscos(ArchivoBinarioPlayer &BDG) {
	size_t N = StrDiscos.size();
	BDG.GuardarSizeT(N); // Numero de Discos
	for (size_t i = 0; i < StrDiscos.size(); i++) {
		BDG.GuardarString(StrDiscos[i]->Nombre);
		BDG.GuardarDword(StrDiscos[i]->Escuchado); // Veces que se ha escuchado
	} 
}



void BaseDatos::_CargarDiscos(ArchivoBinarioPlayer &BDG) {
	size_t Num = 0;
	size_t Tam = 0;
	StrTp       *TMP = NULL; 
	BDG.LeerSizeT(Num); // Leo numero de discos
	for (size_t i = 0; i < Num; i++) {
		TMP = new StrTp;
		BDG.LeerString(TMP->Nombre);
		BDG.LeerDword(TMP->Escuchado); // Veces que se ha escuchado
		StrDiscos.push_back(TMP);
	}
}

void BaseDatos::_CargarMediosRecientes(ArchivoBinarioPlayer &BDG) {
	size_t TotalMediosRecientes  = 0;
	size_t PosicionMedioReciente = 0;
	size_t i                     = 0;
	// Leo el total de medios recientes y los añado al vector
	BOOL Leido = BDG.LeerSizeT(TotalMediosRecientes);
	// Ha sucedido un error leyendo el total de medios (eso es que no hay lista de medios recientes v0.59)
	if (Leido == FALSE) 
		return;
	for (i = 0; i < TotalMediosRecientes; i++) {
		BDG.LeerSizeT(PosicionMedioReciente);
		_MediosRecientes[i] = Medios[PosicionMedioReciente];
	}
}

	
void BaseDatos::_GuardarMediosRecientes(ArchivoBinarioPlayer &BDG) {
	// Guardo el numero total de medios recientes
	size_t TotalMediosRecientes = 0;
	size_t i                    = 0;
	size_t z                    = 0;
	// Cuento el total de medios recientes 
	for (i = 0; i < 10; i++) {
		if (_MediosRecientes[i] != NULL) TotalMediosRecientes ++;
	}

	BDG.GuardarSizeT(TotalMediosRecientes); // Numero de medios recientes
	// Recorro los medios recientes para obtener la posición de su medio y guardarla.
	for (i = 0; i != TotalMediosRecientes; i++) {
		for (z = 0; z < Medios.size(); z ++) {
			if (Medios[z] == _MediosRecientes[i]) {
				BDG.GuardarSizeT(z);
				break;
			}
		}
	}
}


void BaseDatos::_GuardarMedios(ArchivoBinarioPlayer &BDG) {
	size_t TotalMedios  = Medios.size();
	size_t N = 0;
	TMedioAudio   *TmpAudio = NULL;
	TMedioCDAudio *TmpCDAudio = NULL;
	TMedioVideo   *TmpVideo = NULL;
	BDG.GuardarSizeT(TotalMedios); // Numero de medios
	for (size_t i = 0; i < TotalMedios; i++) {
		BDG.Guardar(&Medios[i]->TipoMedio, sizeof(Tipo_Medio));	// Tipo de medio
		BDG.GuardarString(Medios[i]->Nombre);					// Nombre
		BDG.GuardarString(Medios[i]->Path);						// Path
		BDG.Guardar(&Medios[i]->Tiempo, sizeof(UINT64));		// Tiempo
		BDG.GuardarDword(Medios[i]->Longitud);					// Longitud
		BDG.GuardarDword(Medios[i]->Reproducido);				// Reproducido
		BDG.GuardarByte(Medios[i]->Nota);						// Nota
		BDG.GuardarDword(Medios[i]->ID_Disco);					// ID del disco donde esta almacenado el medio
		switch (Medios[i]->TipoMedio) {
			case Tipo_Medio_Audio : 
				TmpAudio = static_cast<TMedioAudio *>(Medios[i]);
				BDG.GuardarInt(TmpAudio->Pista);				// Pista
				for (N = 0; N < StrTipos.size(); N++) {
					if (StrTipos[N] == TmpAudio->StrTipo) {
						BDG.GuardarSizeT(N);					// Numero de Tipo al que corresponde
						break;
					}
				}
				for (N = 0; N < StrGeneros.size(); N++) {
					if (StrGeneros[N] == TmpAudio->StrGenero) {
						BDG.GuardarSizeT(N);					// Numero de Genero al que corresponde
						break;
					}
				}
				for (N = 0; N < StrGrupos.size(); N++) {
					if (StrGrupos[N] == TmpAudio->StrGrupo) {
						BDG.GuardarSizeT(N);					// Numero de Grupo al que corresponde
						break;
					}
				}
				for (N = 0; N < StrDiscos.size(); N++) {
					if (StrDiscos[N] == TmpAudio->StrDisco) {
						BDG.GuardarSizeT(N);					// Numero de Disco al que corresponde
						break;
					}
				}
				for (N = 0; N < 11; N ++) BDG.GuardarFloat(TmpAudio->EQ[N]); // Equalizacion
				break;
			case Tipo_Medio_Video :
				TmpVideo = static_cast<TMedioVideo *>(Medios[i]);
				BDG.GuardarInt(TmpVideo->Pista);				// Pista
				BDG.GuardarUINT(TmpVideo->Ancho);				// Ancho
				BDG.GuardarUINT(TmpVideo->Alto);				// Alto
				break;
			case Tipo_Medio_Lista : // no hay nada extra que guardar
				break;
			case Tipo_Medio_CDAudio :
				TmpCDAudio = static_cast<TMedioCDAudio *>(Medios[i]);
				BDG.GuardarInt(TmpCDAudio->Pista);				// Pista
				break;
		}
	}
}


// Funcion que se mantiene por compatibilidad con la base de datos de la version 0.57
// Todo el tema de tags se mira usando la clase InformacionArchivoEx
void BaseDatos::_CargarTag(ArchivoBinarioPlayer &BDG) {
	size_t Total = 0; // Numero total de frames dentro del tag
	BDG.LeerSizeT(Total);
	FMOD_TAGDATATYPE TipoDatos;
	FMOD_TAGTYPE	 TipoTag;
	std::string      Nombre;
	DWL::DWLString   Datos;
	UINT             Longitud_DatosOriginal;
	for (size_t i = 0; i < Total; i++) {
		BDG.Leer(&TipoDatos, sizeof(FMOD_TAGDATATYPE));
		BDG.Leer(&TipoTag, sizeof(FMOD_TAGTYPE));
		BDG.LeerString(Nombre);							// ID del tag TCOM, TCON, TYER, etc..
		BDG.LeerUnsignedInt(Longitud_DatosOriginal);		// Tamaño original (no tiene porque ser el real...)
		BDG.LeerString(Datos);							// Texto
	}
}


// En la version 0.57 solo existia el TMedio, y todos los medios adoptaban esa estructura.
// En la version 0.60 dependiendo del medio tiene una u otra estructura.
const UINT BaseDatos::_CargarMedios057(ArchivoBinarioPlayer &BDG) {
	size_t			Num				= 0;
	DWL::DWLString	Nombre;
	DWL::DWLString	Path;
	DWL::DWLString	Extension;
	Unidad_Disco   *TmpUnidad		= NULL;
	bool			EsVideo			= false;
	bool            MedioValido     = false;
	TMedioAudio    *NuevoMedioAudio = NULL;
	TMedioVideo    *NuevoMedioVideo = NULL;
	DWORD           TmpDWORD        = 0;
	BYTE            TmpBYTE         = 0;
	int             TmpINT          = 0;
	size_t          TmpSIZET		= 0;
	float           TmpFLOAT        = 0.0f;
	UINT            MediosIgnorados = 0;
	BDG.LeerSizeT(Num); // Leo numero de medios
	for (size_t i = 0; i < Num; i++) {
		BDG.LeerString(Nombre);
		BDG.LeerString(Path);
		BDG.LeerBool(EsVideo);
		if (EsVideo == true) { // Puede ser un OGG o un ASF
			for (size_t n = Path.Tam() - 1; n != 0; n--) {
				if (Path[n] == TEXT('.')) {
					Extension = Path.SubStr(n + 1);
					if (Extension.Comparar(TEXT("ASF")) == true || Extension.Comparar(TEXT("OGG")) == true) MedioValido = false;
					break;
				}
			}
		}
		else { 
			MedioValido = true;
		}
		if (MedioValido == true && EsVideo == false) { // audio
			NuevoMedioAudio = new TMedioAudio();
			NuevoMedioAudio->Path = Path;
			NuevoMedioAudio->Nombre = Nombre;
			NuevoMedioAudio->TipoMedio = Tipo_Medio_Audio;
			BDG.LeerDword(TmpDWORD); // Leo el tiempo
			NuevoMedioAudio->Tiempo = TmpDWORD; // El tiempo se pasa a UINT64
			BDG.LeerDword(NuevoMedioAudio->Longitud); // Leo el tamaño del archivo
			BDG.LeerDword(NuevoMedioAudio->Reproducido); // Leo las veces que se ha reproducido
			BDG.LeerByte(NuevoMedioAudio->Nota); // Leo la Nota
			BDG.LeerDword(NuevoMedioAudio->ID_Disco); // Leo la ID del disco
			TmpUnidad = Sistema.App.UnidadesDisco.Buscar_Numero_Serie(NuevoMedioAudio->ID_Disco);
			if (TmpUnidad != NULL) NuevoMedioAudio->Path[0] = TmpUnidad->Letra(); // Asigno la letra de la unidad al primer caracter del path
			BDG.LeerInt(NuevoMedioAudio->Pista); // Leo la pista
			BDG.LeerSizeT(TmpSIZET); // Leo el Tipo al que pertenece
			NuevoMedioAudio->StrTipo = StrTipos[TmpSIZET];
			BDG.LeerSizeT(TmpSIZET); // Leo el Genero al que pertenece
			NuevoMedioAudio->StrGenero = StrGeneros[TmpSIZET];
			BDG.LeerSizeT(TmpSIZET); // Leo el Grupo al que pertenece
			NuevoMedioAudio->StrGrupo = StrGrupos[TmpSIZET];
			BDG.LeerSizeT(TmpSIZET); // Leo el Grupo al que pertenece
			NuevoMedioAudio->StrDisco = StrDiscos[TmpSIZET];
			for (unsigned int i2 = 0; i2 < 11; i2++) BDG.LeerFloat(NuevoMedioAudio->EQ[i2]); // equalizador

			NuevoMedioAudio->Nodo = NULL;
			NuevoMedioAudio->Item = NULL;
			
			Medios.push_back(NuevoMedioAudio);
		}
		else if (MedioValido == true && EsVideo == true) { // video
			NuevoMedioVideo = new TMedioVideo();
			NuevoMedioVideo->Path = Path;
			NuevoMedioVideo->Nombre = Nombre;
			NuevoMedioVideo->TipoMedio = Tipo_Medio_Video;
			BDG.LeerDword(TmpDWORD); // Leo el tiempo
			NuevoMedioVideo->Tiempo = TmpDWORD; // El tiempo se pasa a UINT64
			BDG.LeerDword(NuevoMedioVideo->Longitud); // Leo el tamaño del archivo
			BDG.LeerDword(NuevoMedioVideo->Reproducido); // Leo las veces que se ha reproducido
			BDG.LeerByte(NuevoMedioVideo->Nota); // Leo la Nota
			BDG.LeerDword(NuevoMedioVideo->ID_Disco); // Leo la ID del disco
			TmpUnidad = Sistema.App.UnidadesDisco.Buscar_Numero_Serie(NuevoMedioVideo->ID_Disco);
			if (TmpUnidad != NULL) NuevoMedioVideo->Path[0] = TmpUnidad->Letra(); // Asigno la letra de la unidad al primer caracter del path
			BDG.LeerInt(NuevoMedioVideo->Pista); // Leo la pista
			BDG.LeerSizeT(TmpSIZET); // Leo el Tipo al que pertenece
			BDG.LeerSizeT(TmpSIZET); // Leo el Genero al que pertenece
			BDG.LeerSizeT(TmpSIZET); // Leo el Grupo al que pertenece
			BDG.LeerSizeT(TmpSIZET); // Leo el Disco al que pertenece
			for (unsigned int i2 = 0; i2 < 11; i2++) BDG.LeerFloat(TmpFLOAT); // equalizador
			NuevoMedioVideo->Nodo = NULL;
			NuevoMedioVideo->Item = NULL;
			Medios.push_back(NuevoMedioVideo);
		}
		else { // asf - ogg (leo los datos sin introducirlos en la BD)
			BDG.LeerDword(TmpDWORD); // Leo el Tiempo
			BDG.LeerDword(TmpDWORD); // Leo el Longitud
			BDG.LeerDword(TmpDWORD); // Leo el Reproducido
			BDG.LeerByte(TmpBYTE); // Leo la Nota
			BDG.LeerDword(TmpDWORD); // Leo la ID del disco
			BDG.LeerInt(TmpINT); // Leo la pista
			BDG.LeerSizeT(TmpSIZET); // Leo el Tipo al que pertenece
			BDG.LeerSizeT(TmpSIZET); // Leo el Genero al que pertenece
			BDG.LeerSizeT(TmpSIZET); // Leo el Grupo al que pertenece
			BDG.LeerSizeT(TmpSIZET); // Leo el Disco al que pertenece
			for (unsigned int i2 = 0; i2 < 11; i2++) BDG.LeerFloat(TmpFLOAT);
			
			MediosIgnorados ++;
		}
		// Cargamos el tag 
		_CargarTag(BDG);
		//delete Tag; // Borramos el tag de memoria (en la 0.60 no se guardaran los tags en la bd)
	}
	return MediosIgnorados;
}

// version 0.60
void BaseDatos::_CargarMedios(ArchivoBinarioPlayer &BDG) {
	size_t			TotalMedios = 0;
	size_t          Pos;
	Tipo_Medio		TipoMedio   = Tipo_Medio_Audio;
	Unidad_Disco   *TmpUnidad   = NULL;
	TMedioAudio    *TmpAudio    = NULL;
	TMedioCDAudio  *TmpCDAudio  = NULL;
	TMedioVideo    *TmpVideo    = NULL;
	TMedio         *TmpMedio    = NULL;
	BDG.LeerSizeT(TotalMedios); // Leo numero de medios
	for (size_t i = 0; i < TotalMedios; i++) {
		BDG.Leer(&TipoMedio, sizeof(Tipo_Medio)); // Leo el tipo de medio
		switch (TipoMedio) {
			case Tipo_Medio_Audio :
				TmpAudio = new TMedioAudio;
				TmpAudio->TipoMedio = Tipo_Medio_Audio;
				BDG.LeerString(TmpAudio->Nombre);			// Leo el nombre
				BDG.LeerString(TmpAudio->Path);				// Leo el path
				BDG.Leer(&TmpAudio->Tiempo, sizeof(UINT64)); // Leo el Tiempo
				BDG.LeerDword(TmpAudio->Longitud);			// Leo el Longitud
				BDG.LeerDword(TmpAudio->Reproducido);		// Leo el Reproducido
				BDG.LeerByte(TmpAudio->Nota);				// Leo la Nota
				BDG.LeerDword(TmpAudio->ID_Disco);			// Leo la ID del disco
				TmpUnidad = Sistema.App.UnidadesDisco.Buscar_Numero_Serie(TmpAudio->ID_Disco);
				if (TmpUnidad != NULL) TmpAudio->Path[0] = TmpUnidad->Letra(); // Asigno la letra de la unidad al primer caracter del path
				BDG.LeerInt(TmpAudio->Pista);				// Leo la pista
				BDG.LeerSizeT(Pos);							// Leo el Tipo al que pertenece
				TmpAudio->StrTipo = StrTipos[Pos];
				BDG.LeerSizeT(Pos);							// Leo el Genero al que pertenece
				TmpAudio->StrGenero = StrGeneros[Pos];
				BDG.LeerSizeT(Pos);							// Leo el Grupo al que pertenece
				TmpAudio->StrGrupo = StrGrupos[Pos];
				BDG.LeerSizeT(Pos);							// Leo el Grupo al que pertenece
				TmpAudio->StrDisco = StrDiscos[Pos];
				for (unsigned int i2 = 0; i2 < 11; i2++) BDG.LeerFloat(TmpAudio->EQ[i2]); // Leo la equalizacion
				TmpAudio->Nodo = NULL;
				TmpAudio->Item = NULL;
				Medios.push_back(TmpAudio);
				break;
			case Tipo_Medio_Video :
				TmpVideo = new TMedioVideo;
				TmpVideo->TipoMedio = Tipo_Medio_Video;
				BDG.LeerString(TmpVideo->Nombre);			// Leo el nombre
				BDG.LeerString(TmpVideo->Path);				// Leo el path
				BDG.Leer(&TmpVideo->Tiempo, sizeof(UINT64)); // Leo el Tiempo
				BDG.LeerDword(TmpVideo->Longitud);			// Leo el Longitud
				BDG.LeerDword(TmpVideo->Reproducido);		// Leo el Reproducido
				BDG.LeerByte(TmpVideo->Nota);				// Leo la Nota
				BDG.LeerDword(TmpVideo->ID_Disco);			// Leo la ID del disco
				TmpUnidad = Sistema.App.UnidadesDisco.Buscar_Numero_Serie(TmpVideo->ID_Disco);
				if (TmpUnidad != NULL) TmpVideo->Path[0] = TmpUnidad->Letra(); // Asigno la letra de la unidad al primer caracter del path
				BDG.LeerInt(TmpVideo->Pista);				// Leo la pista
				BDG.LeerUINT(TmpVideo->Ancho);				// Leo el ancho del video
				BDG.LeerUINT(TmpVideo->Alto);				// Leo la altura del video
				TmpVideo->Nodo = NULL;
				TmpVideo->Item = NULL;
				Medios.push_back(TmpVideo);
				break;
			case Tipo_Medio_Lista :
				TmpMedio = new TMedio;
				TmpMedio->TipoMedio = Tipo_Medio_Lista;
				BDG.LeerString(TmpMedio->Nombre);			// Leo el nombre
				BDG.LeerString(TmpMedio->Path);				// Leo el path
				BDG.Leer(&TmpMedio->Tiempo, sizeof(UINT64)); // Leo el Tiempo
				BDG.LeerDword(TmpMedio->Longitud);			// Leo el Longitud
				BDG.LeerDword(TmpMedio->Reproducido);		// Leo el Reproducido
				BDG.LeerByte(TmpMedio->Nota);				// Leo la Nota
				BDG.LeerDword(TmpMedio->ID_Disco);			// Leo la ID del disco
				TmpUnidad = Sistema.App.UnidadesDisco.Buscar_Numero_Serie(TmpMedio->ID_Disco);
				if (TmpUnidad != NULL) TmpMedio->Path[0] = TmpUnidad->Letra(); // Asigno la letra de la unidad al primer caracter del path
				TmpMedio->Nodo = NULL;
				TmpMedio->Item = NULL;
				Medios.push_back(TmpMedio);
				break;
			case Tipo_Medio_CDAudio :
				TmpCDAudio = new TMedioCDAudio;
				TmpCDAudio->TipoMedio = Tipo_Medio_CDAudio;
				BDG.LeerString(TmpCDAudio->Nombre);			// Leo el nombre
				BDG.LeerString(TmpCDAudio->Path);				// Leo el path
				BDG.Leer(&TmpCDAudio->Tiempo, sizeof(UINT64)); // Leo el Tiempo
				BDG.LeerDword(TmpCDAudio->Longitud);			// Leo el Longitud
				BDG.LeerDword(TmpCDAudio->Reproducido);		// Leo el Reproducido
				BDG.LeerByte(TmpCDAudio->Nota);				// Leo la Nota
				BDG.LeerDword(TmpCDAudio->ID_Disco);			// Leo la ID del disco
				TmpUnidad = Sistema.App.UnidadesDisco.Buscar_Numero_Serie(TmpCDAudio->ID_Disco);
				if (TmpUnidad != NULL) TmpCDAudio->Path[0] = TmpUnidad->Letra(); // Asigno la letra de la unidad al primer caracter del path
				BDG.LeerInt(TmpCDAudio->Pista);				// Leo la pista
				TmpCDAudio->Nodo = NULL;
				TmpCDAudio->Item = NULL;
				Medios.push_back(TmpCDAudio);
				break;
		}
	}
}


void BaseDatos::Terminar(void) {
	size_t i = 0;
	for (i = 0; i < Tipos.size();		i++) delete Tipos[i];
	for (i = 0; i < Generos.size();		i++) delete Generos[i];
	for (i = 0; i < Grupos.size();		i++) delete Grupos[i];
	for (i = 0; i < Discos.size();		i++) delete Discos[i];
	for (i = 0; i < Medios.size();		i++) {
		switch (Medios[i]->TipoMedio) {
			case Tipo_Medio_Audio :		delete static_cast<TMedioAudio *>(Medios[i]);	break;
			case Tipo_Medio_Video :		delete static_cast<TMedioVideo *>(Medios[i]);	break;
		}
	}
	for (i = 0; i < StrTipos.size();	i++) delete StrTipos[i];
	for (i = 0; i < StrGeneros.size();	i++) delete StrGeneros[i];
	for (i = 0; i < StrGrupos.size();	i++) delete StrGrupos[i];
	for (i = 0; i < StrDiscos.size();	i++) delete StrDiscos[i];
}


// Función que retorna si el path existe en la BD (utilizada por el ThreadBuscarArchivos mediante un SendMessage)
TMedio *BaseDatos::BuscarPath(const TCHAR *Path) {
	for (size_t i = 0; i < Medios.size(); i++) {
		if (Medios[i]->Path == Path) { // Si el path es el mismo, comprobamos que los numeros de serie sean el mismo (para cds de audio)
			if (Sistema.App.UnidadesDisco.Buscar_Letra(Path[0])->Numero_Serie() == Medios[i]->ID_Disco)	return Medios[i];
			else																						return NULL;
		}
	}
	return NULL;
}

// Hay que asegurarse antes de que la cancion no existe en la BD (la mejor forma es comparando los paths de los medios con este)
TMedio *BaseDatos::AgregarMedioBD(TMedio *nMedio) {
	switch (nMedio->TipoMedio) {
		// Los subtitulos no se agregan a la bd
		case Tipo_Medio_Subtitulos :
			delete static_cast<TMedioSubtitulos *>(nMedio);
			return NULL;
		// Información basica de un archivo de audio (hay que reparsearla, y añadir tipos, generos, grupos y discos a sus respectivos vectores.)
		case Tipo_Medio_PreAudio :
			TMedioPreAudio *pAudio;
			pAudio = static_cast<TMedioPreAudio *>(nMedio);
			TMedioAudio *fAudio;
			fAudio = new TMedioAudio;
			fAudio->Pista		= pAudio->Pista;
			fAudio->ID_Disco	= pAudio->ID_Disco;
			fAudio->Path		= pAudio->Path;
			fAudio->Nombre		= pAudio->Nombre;
			fAudio->Tiempo		= pAudio->Tiempo;
			fAudio->Longitud	= pAudio->Longitud;
			fAudio->Reproducido = 0;
			fAudio->Nota        = 2;
			fAudio->StrTipo		= AgregarStrTp(&pAudio->NombreTipo, StrTipos);
			switch (pAudio->NombreGenero[0]) {
				case TEXT('0'): case TEXT('1'): case TEXT('2'): case TEXT('3'): case TEXT('4'):
				case TEXT('5'): case TEXT('6'): case TEXT('7'): case TEXT('8'): case TEXT('9'): // Tiene un numero que identifica el nombre
					fAudio->StrGenero = AgregarStrTp(Sistema.App.Idioma_EstiloStr(DWLStrToInt(pAudio->NombreGenero())), StrGeneros);
					break;
				default : // Tiene nombre normal
					fAudio->StrGenero = AgregarStrTp(&pAudio->NombreGenero, StrGeneros);
					break;
			}

			
			if (pAudio->NombreGrupo == TEXT("?")) pAudio->NombreGrupo = IDIOMA__GRUPO_DESCONOCIDO;
			fAudio->StrGrupo	= AgregarStrTp(&pAudio->NombreGrupo, StrGrupos);
			if (pAudio->NombreDisco == TEXT("?")) pAudio->NombreGrupo = IDIOMA__DISCO_DESCONOCIDO;
			fAudio->StrDisco	= AgregarStrTp(&pAudio->NombreDisco, StrDiscos);
			fAudio->Tipo		= NULL;
			fAudio->Genero		= NULL;
			fAudio->Grupo		= NULL;
			fAudio->Disco		= NULL;
			fAudio->Item		= NULL;
			fAudio->Nodo		= NULL;
			for (size_t i = 0; i != 11; i++) fAudio->EQ[i] = 1.0f;

			delete pAudio;
			Medios.push_back(fAudio);
			return fAudio;
		// Informacion completa de un archivo de video
		case Tipo_Medio_Video :
			Medios.push_back(static_cast<TMedioVideo *>(nMedio));
			break;
		case Tipo_Medio_Lista :
			Medios.push_back(static_cast<TMedioListaM3u *>(nMedio));
			break;
		case Tipo_Medio_CDAudio :
			TMedioCDAudio *nPistaAudio = static_cast<TMedioCDAudio *>(nMedio);
			Medios.push_back(nPistaAudio);
			break;
	}
	return nMedio;
}



// òptimizar la parte final en la que se añaden canciones, paara que mire los videos y m3us tambien
void BaseDatos::ActualizarArbol(void) {
	size_t Contador = 0;
	size_t Total = 0;
	#define MAX_NOMBRE_CANCION 512
	TCHAR  TMP[MAX_NOMBRE_CANCION];
	ArbolBaseDatos *Arbol = &Sistema.App.ArbolBaseDatos;
	Arbol->EliminarTodosLosNodos();
	if (Medios.size() == 0) {
		Arbol->ActualizarTodo();
		return;
	}

	size_t						i			= 0;
	size_t						Punto		= 0;
	DWL::DWLString				Tmp;
	NodoArbolBaseDatos         *Nodo		= NULL;
	NodoArbolBaseDatos         *Nodo2		= NULL;
	
	
	NodoArbolBaseDatos *nAudio  = Arbol->AgregarNodoMedio(NULL, IDIOMA__AUDIO, IDI_BDA, NULL, NULL, NULL, NULL, NULL, false);
	NodoArbolBaseDatos *nVideo  = Arbol->AgregarNodoMedio(NULL, IDIOMA__VIDEO, IDI_BDV, NULL, NULL, NULL, NULL, NULL, true);
	NodoArbolBaseDatos *nListas = Arbol->AgregarNodoMedio(NULL, IDIOMA__LISTAS, IDI_LISTA_MODIFICARPISTA, NULL, NULL, NULL, NULL, NULL, true);

	// Audio
	TMedioAudio *MedioAudio = NULL;
	switch (Sistema.App.Config.OrdenBD) {
		case Orden_Tipo_Genero_Grupo_Disco :
			ConstruyeTGGD(); // Creo las relaciones entre tipos generos grupos discos y canciones
			Total = Tipos.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				Tipos[Contador]->Nodo = Arbol->AgregarNodoMedio(nAudio, Tipos[Contador]->Nombre->Nombre(), IDI_TIPO, Tipos[Contador], NULL, NULL, NULL, NULL, false);
			}
			Total = Generos.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				Generos[Contador]->Nodo = Arbol->AgregarNodoMedio(Generos[Contador]->Tipo->Nodo, Generos[Contador]->Nombre->Nombre(), IDI_GENERO, Generos[Contador]->Tipo, Generos[Contador], NULL, NULL, NULL, false);
			}
			Total = Grupos.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				if (Grupos[Contador]->Discos > 0) Grupos[Contador]->Nodo = Arbol->AgregarNodoMedio(Grupos[Contador]->Genero->Nodo, Grupos[Contador]->Nombre->Nombre(), IDI_GRUPO, Grupos[Contador]->Tipo, Grupos[Contador]->Genero, Grupos[Contador], NULL, NULL, false);
			}
			Total = Discos.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				if (Discos[Contador]->Canciones > 0) Discos[Contador]->Nodo = Arbol->AgregarNodoMedio(Discos[Contador]->Grupo->Nodo, Discos[Contador]->Nombre->Nombre(), IDI_DISCO, Discos[Contador]->Tipo, Discos[Contador]->Genero, Discos[Contador]->Grupo, Discos[Contador], NULL, false);
			}
			Total = Medios.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				if (Medios[Contador]->TipoMedio == Tipo_Medio_Audio) {
					MedioAudio = static_cast<TMedioAudio *>(Medios[Contador]);
					if (MedioAudio->Disco != NULL) {
						if (MedioAudio->Pista > 0)	swprintf(TMP, MAX_NOMBRE_CANCION, TEXT("%.2d %s"), MedioAudio->Pista, Medios[Contador]->Nombre());
						else						wcscpy_s(TMP, MAX_NOMBRE_CANCION, Medios[Contador]->Nombre());
						Medios[Contador]->Nodo = Arbol->AgregarNodoMedio(MedioAudio->Disco->Nodo, TMP, MiraIconoCancion(Medios[Contador]->Reproducido), MedioAudio->Tipo, MedioAudio->Genero, MedioAudio->Grupo, MedioAudio->Disco, Medios[Contador], false);
					}
				}
			}
			break;

		case Orden_Genero_Grupo_Disco :
			ConstruyeGGD(); // Creo las relaciones entre generos grupos discos y canciones
			Total = Generos.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				if (Generos[Contador]->Grupos > 0) Generos[Contador]->Nodo = Arbol->AgregarNodoMedio(nAudio, Generos[Contador]->Nombre->Nombre(), IDI_GENERO, NULL, Generos[Contador], NULL, NULL, NULL, false);
			}
			Total = Grupos.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				if (Grupos[Contador]->Discos > 0) Grupos[Contador]->Nodo = Arbol->AgregarNodoMedio(Grupos[Contador]->Genero->Nodo, Grupos[Contador]->Nombre->Nombre(), IDI_GRUPO, NULL, Grupos[Contador]->Genero, Grupos[Contador], NULL, NULL, false);
			}
			Total = Discos.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				if (Discos[Contador]->Canciones > 0) Discos[Contador]->Nodo = Arbol->AgregarNodoMedio(Discos[Contador]->Grupo->Nodo, Discos[Contador]->Nombre->Nombre(), IDI_DISCO, NULL, Discos[Contador]->Genero, Discos[Contador]->Grupo, Discos[Contador], NULL, false);
			}
			Total = Medios.size();
			for (Contador = 0; Contador < Total; Contador ++) {
				if (Medios[Contador]->TipoMedio == Tipo_Medio_Audio) {
					MedioAudio = static_cast<TMedioAudio *>(Medios[Contador]);
					if (MedioAudio->Disco != NULL) {
						if (MedioAudio->Pista > 0)	swprintf(TMP, MAX_NOMBRE_CANCION, TEXT("%.2d %s"), MedioAudio->Pista, Medios[Contador]->Nombre());
						else						wcscpy_s(TMP, MAX_NOMBRE_CANCION, Medios[Contador]->Nombre());
						Medios[Contador]->Nodo = Arbol->AgregarNodoMedio(MedioAudio->Disco->Nodo, TMP, MiraIconoCancion(Medios[Contador]->Reproducido), NULL, MedioAudio->Genero, MedioAudio->Grupo, MedioAudio->Disco, Medios[Contador], false);
					}
				}
			}
			break;

		case Orden_Grupo_Disco :
			ConstruyeGD(); // Creo las relaciones entre grupos discos y canciones
			Total = Grupos.size();
			for (Contador = 0; Contador < Total; Contador++) {
				if (Grupos[Contador]->Discos > 0) Grupos[Contador]->Nodo = Arbol->AgregarNodoMedio(nAudio, Grupos[Contador]->Nombre->Nombre(), IDI_GRUPO, NULL, NULL, Grupos[Contador], NULL, NULL, false);
			}
			Total = Discos.size();
			for (Contador = 0; Contador < Total; Contador++) {
				if (Discos[Contador]->Canciones > 0) Discos[Contador]->Nodo = Arbol->AgregarNodoMedio(Discos[Contador]->Grupo->Nodo, Discos[Contador]->Nombre->Nombre(), IDI_DISCO, NULL, NULL, Discos[Contador]->Grupo, Discos[Contador], NULL, false);
			}
			Total = Medios.size();
			for (Contador = 0; Contador < Total; Contador++) {
				if (Medios[Contador]->TipoMedio == Tipo_Medio_Audio) {
					MedioAudio = static_cast<TMedioAudio *>(Medios[Contador]);
					if (MedioAudio->Disco != NULL) {
						if (MedioAudio->Pista > 0)	swprintf(TMP, MAX_NOMBRE_CANCION, TEXT("%.2d %s"), MedioAudio->Pista, Medios[Contador]->Nombre());
						else						wcscpy_s(TMP, MAX_NOMBRE_CANCION, Medios[Contador]->Nombre());
						Medios[Contador]->Nodo = Arbol->AgregarNodoMedio(MedioAudio->Disco->Nodo, TMP, MiraIconoCancion(Medios[Contador]->Reproducido), NULL, NULL, MedioAudio->Grupo, MedioAudio->Disco, Medios[Contador], false);
					}
				}
			}
			break;

		case Orden_Disco :
			ConstruyeD(); // Creo las relaciones entre discos y canciones
			Total = Discos.size();
			for (Contador = 0; Contador < Total; Contador++) {
				if (Discos[Contador]->Canciones > 0) Discos[Contador]->Nodo = Arbol->AgregarNodoMedio(nAudio, Discos[Contador]->Nombre->Nombre(), IDI_DISCO, NULL, NULL, NULL, Discos[Contador], NULL, false);
			}
			Total = Medios.size();
			for (Contador = 0; Contador < Total; Contador++) {
				if (Medios[Contador]->TipoMedio == Tipo_Medio_Audio) {
					MedioAudio = static_cast<TMedioAudio *>(Medios[Contador]);
					if (MedioAudio->Disco != NULL) {
						if (MedioAudio->Pista > 0)	swprintf(TMP, MAX_NOMBRE_CANCION, TEXT("%.2d %s"), MedioAudio->Pista, Medios[Contador]->Nombre());
						else						wcscpy_s(TMP, MAX_NOMBRE_CANCION, Medios[Contador]->Nombre());
						Medios[Contador]->Nodo = Arbol->AgregarNodoMedio(MedioAudio->Disco->Nodo, TMP, MiraIconoCancion(Medios[Contador]->Reproducido), NULL, NULL, NULL, MedioAudio->Disco, Medios[Contador], false);
					}
				}
			}
			break;
	}

	// Videos
	
	for (Contador = 0; Contador < Medios.size(); Contador ++) {
		if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[Contador]->ID_Disco) != NULL) {
			if (Medios[Contador]->TipoMedio == Tipo_Medio_Video) {
				Punto = 0;
				Nodo = nVideo;
				for (i = 0; i < Medios[Contador]->Path.Tam(); i++) {
					if (Medios[Contador]->Path[i] == TEXT('\\')) {
						Tmp = Medios[Contador]->Path.SubStr(Punto, i - Punto).Texto();
						Nodo2 = Arbol->BuscarNodoR(Tmp(), Nodo);
						if (Nodo2 == NULL)	Nodo = Arbol->AgregarNodoMedio(Nodo, IDI_BD_SELECCIONAR, Tmp(), NULL);
						else				Nodo = Nodo2;
						Punto = i + 1;
					}
				}
				Arbol->AgregarNodoMedio(Nodo, MiraIconoVideo(Medios[Contador]->Reproducido), Medios[Contador]->Nombre(), Medios[Contador]);
			}
		}
	}

	// listas M3U
	for (Contador = 0; Contador < Medios.size(); Contador ++) {
		if (Medios[Contador]->TipoMedio == Tipo_Medio_Lista) {
			if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[Contador]->ID_Disco) != NULL) {
				_ArbolBD_AgregarM3U(nListas, Medios[Contador]);
			}
		}
	}

	// CDs de audio
    WIN32_FIND_DATA		FindInfoPoint;
	HANDLE				hFind						= NULL;
	InformacionArchivoEx Info;
	NodoArbolBaseDatos *nCDAudio = NULL;
	DWL::Archivos::DWLArchivoBinario Archivo;
	DWL::DWLString Tmp2;
	Tmp = TEXT("?:\\");
	TMedio *MedioPistaAudio = NULL;
	DWORD MascaraBitsUnidades = GetLogicalDrives();
	SetErrorMode(SEM_FAILCRITICALERRORS);
	for (TCHAR i2 = 0; i2 < 32; i2++) {
		if (MascaraBitsUnidades & (1 << i2)) {
			Tmp[0] = i2 + 'A';
			if (GetDriveType(Tmp()) == DRIVE_CDROM) { // Es un cdrom
				Tmp2 = Tmp;
				Tmp2 += TEXT("Track01.cda");
				Archivo.AbrirArchivoLectura(Tmp2());
				
				if (Archivo.EstaAbierto() == true) { // es un cd de audio
					Tmp2.sprintf(IDIOMA__CD_DE_AUDIO_S, Tmp());
					nCDAudio = Arbol->AgregarNodoMedio(NULL, Tmp2(), IDI_CDAUDIO, NULL, NULL, NULL, NULL, NULL, true);
					// Buscamos las pistas
					Tmp2 = Tmp; Tmp2 += TEXT("*.cda");
					hFind = FindFirstFile(Tmp2(), &FindInfoPoint);
					do {
						if (FindInfoPoint.cFileName[0] != TEXT('.')) { 
							Tmp2 = Tmp; Tmp2 += FindInfoPoint.cFileName;
							MedioPistaAudio = BuscarPath(Tmp2());
							if (MedioPistaAudio == NULL) {
								MedioPistaAudio = Info.ObtenerInfo(Tmp2(), Sistema.App.Tipos, Sistema.App.UnidadesDisco, Sistema.App.Config.Buscar_Paths, Sistema.App.VLC.InstanciaVLC(), Sistema.App.FMod.Sistema_FMOD());
								MedioPistaAudio = AgregarMedioBD(MedioPistaAudio);
							}
							swprintf(TMP, 512, TEXT("%.2d %s"), static_cast<TMedioCDAudio *>(MedioPistaAudio)->Pista, MedioPistaAudio->Nombre());
							MedioPistaAudio->Nodo = Arbol->AgregarNodoMedio(nCDAudio, TMP, IDI_PISTAAUDIO, NULL, NULL, NULL, NULL, MedioPistaAudio, false);
						}
					} while (FindNextFile(hFind, &FindInfoPoint) != 0);
					FindClose(hFind);
				}
			}
		}
	}
	SetErrorMode(0);
	Arbol->ActualizarTodo(true);
}


void BaseDatos::_ArbolBD_AgregarM3U(NodoArbolBaseDatos *nListas, TMedio *MedioM3U) {
	char							   *ArchivoLeido;
	int									i = 0;
	size_t								z = 0;
	DWL::DWLString						Linea;
	DWL::DWLString						PathAudio;
	DWL::DWLString						DirectorioM3U;
	DWL::Archivos::DWLArchivoBinario	ArchivoM3U;
	ArbolBaseDatos                     *Arbol = &Sistema.App.ArbolBaseDatos;
	bool                                Encontrado = false;

	ArchivoM3U.AbrirArchivoLectura(MedioM3U->Path());
	// Busco la primera antibarra del path para obtener el directorio absoluto donde esta ubicado el M3U
	for (i = MedioM3U->Path.Tam() - 1; i > -1; i--) {
		if (MedioM3U->Path[i] == '\\') {
			DirectorioM3U = MedioM3U->Path.SubStr(0, i + 1);
			break;
		}
	}

	DWORD TamArchivo = ArchivoM3U.Tam();
	ArchivoLeido = new char[TamArchivo +1];
	ArchivoM3U.Leer(ArchivoLeido, TamArchivo);
	
	// Añado el nodo que hace de lista en el arbol
	NodoArbolBaseDatos *ListaM3U = NULL;
	

	MedioM3U->Longitud = 0;
	MedioM3U->Tiempo = 0;

	for (i = 0; i < static_cast<int>(TamArchivo); i++) {
		if (ArchivoLeido[i] == TEXT('\n') || i == TamArchivo) {
			Encontrado = false;
			// No es un comentario
			if (Linea[0] != TEXT('#')) {
				if (Linea[1] == TEXT(':')) { // ruta absoluta
					PathAudio = Linea;
				}
				else { // ruta relativa (ARREGLAR RUTA RELATIVA, puede contener \..\)
					PathAudio = DirectorioM3U;
					PathAudio += Linea;
				}
				// Busco el medio que corresponde al path
				for (z = 0; z < Medios.size(); z++) {
					if (Medios[z]->Path == PathAudio) { // Encontrado el medio
						// Añado el nodo principal de esta lista, si no lo he añadido aun.
						if (ListaM3U == NULL) ListaM3U = Arbol->AgregarNodoMedio(nListas, IDI_M3U, MedioM3U->Nombre(), MedioM3U);
						// Añado el medio dentro de la lista
						Arbol->AgregarNodoMedioM3u(ListaM3U, IDI_AUDIO, Medios[z]->Nombre(), Medios[z]);
						MedioM3U->Longitud += Medios[z]->Longitud;
						MedioM3U->Tiempo += Medios[z]->Tiempo;
						Encontrado = true;
						break;
					}
				}
				// No se ha encontrado el medio en la base de datos
			}
			Linea.Borrar();
			//i ++; // sumo uno para saltar el caracter de despues del intro (SOLO WINDOWS)
		}
		else {
			Linea += ArchivoLeido[i];
		}
	}

	delete [] ArchivoLeido;
}

TTipo *BaseDatos::_BuscaTipo(StrTp *Tp) {										// Funcion para buscar un Tipo
	for (size_t i = Tipos.size(); i > 0; i--) {									// Del tamaño de los tipos a 0
		if (DWLStrCmpi(Tipos[i - 1]->Nombre->Nombre(), Tp->Nombre()) == 0) {	// Comparo el tipo
			return Tipos[i - 1];												// Retorno el Tipo
		}																		//
	}																			//
	TTipo *Org = new TTipo;														// Creo tipo para agregarlo al vector
	Org->Nombre = Tp;															// Nombre
	Tipos.push_back(Org);														// Añado el tipo al vector
	return Org;																	// Retorno la ultima posicion del Vector
}																				// 



TGenero *BaseDatos::_BuscaGenero(StrTp *Gen, TTipo *Tp, StrTp *StrTipo) {										// Funcion para buscar un genero
	for (size_t i = Generos.size(); i > 0; i --) {																// Del tope de Generos a 0
		if (DWLStrCmpi(Generos[i - 1]->Nombre->Nombre(), Gen->Nombre()) == 0 && Generos[i - 1]->Tipo == Tp) {	// Comparo el Genero
			return Generos[i - 1];																				// Retorno el Genero
		}																										//
	}																											//
	TGenero *Org = new TGenero;																					// Nuevo Genero para agregarlo al vector
	Org->Nombre = Gen;																							// Asigno el nombre Nombre
	Org->Tipo = Tp;																								// Tipo
	Org->StrTipo = StrTipo;
	Generos.push_back(Org);																						// Añado el Genero
	return Org;																									// Retorno el ultimo Genero
}																												//




TGrupo *BaseDatos::_BuscaGrupo(StrTp *Grp, TTipo *Tp, TGenero *Gen, StrTp *StrTipo, StrTp *StrGenero) {										// Funcion para buscar un Grupo
	for (size_t i = Grupos.size(); i > 0; i--) {																							// Del tope de Grupos a 0
		if (DWLStrCmpi(Grupos[i - 1]->Nombre->Nombre(), Grp->Nombre()) == 0 && Grupos[i - 1]->Tipo == Tp && Grupos[i - 1]->Genero == Gen) {	// Comparo el Grupo
			return Grupos[i - 1];																											// Retorno el Grupo
		}																																	//
	}																																		//
	TGrupo *Org = new TGrupo;																												// Nuevo Gurpo a añadir en el vector
	Org->Nombre = Grp;																														// Nombre
	Org->Tipo = Tp;																															// Tipo
	Org->Genero = Gen;																														// Genero
	Org->StrTipo = StrTipo;
	Org->StrGenero = StrGenero;
	Grupos.push_back(Org);																													// Añado el Grupo
	return Org;																																// Retorno el Ultimo Gruo
}																																			// 



TDisco *BaseDatos::_BuscaDisco(StrTp *Dsk, TTipo *Tp, TGenero *Gen, TGrupo *Grp, StrTp *StrTipo, StrTp *StrGenero, StrTp *StrGrupo) {                                       // Funcion para buscar un Disco
	for (size_t i = Discos.size(); i > 0; i--) {																															// Del tope de Discos a 0
		if (DWLStrCmpi(Discos[i - 1]->Nombre->Nombre(), Dsk->Nombre()) == 0 && Discos[i - 1]->Tipo == Tp && Discos[i - 1]->Genero == Gen && Discos[i -1]->Grupo == Grp) {	// Comparo el Disco
			return Discos[i - 1];																																			// Retorno el Disco
		}																																									//
	}																																										//
	TDisco *Org = new TDisco;																																				// Nuevo Disco para añadir en el Vector
	Org->Nombre = Dsk;																																						// Nombre
	Org->Tipo = Tp;																																							// Tipo
	Org->Genero = Gen;																																						// Genero
	Org->Grupo = Grp;																																						// Grupo
	Org->StrTipo = StrTipo;
	Org->StrGenero = StrGenero;
	Org->StrGrupo = StrGrupo;
	Discos.push_back(Org);																																					// Añado el Disco
	return Org;																																								// Retorno el ultimo Disco del vector
}																																											//



void BaseDatos::ConstruyeTGGD(void) {
	size_t i,ge,gr,ds;
	for (i = 0; i < Tipos.size(); i++)   delete Tipos[i];
	for (i = 0; i < Generos.size(); i++) delete Generos[i];
	for (i = 0; i < Grupos.size(); i++)  delete Grupos[i];
	for (i = 0; i < Discos.size(); i++)  delete Discos[i];
	Tipos.clear();
	Generos.clear();
	Grupos.clear();
	Discos.clear();
	TMedioAudio *MedioAudio = NULL;
	for (i = 0; i < Medios.size(); i++) {

		if (Medios[i]->ID_Disco == 0 || Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
			if ( Medios[i]->TipoMedio == Tipo_Medio_Audio) {
//			if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL && Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				MedioAudio = static_cast<TMedioAudio *>(Medios[i]);
				MedioAudio->Tipo = _BuscaTipo(MedioAudio->StrTipo);
				MedioAudio->Genero = _BuscaGenero(MedioAudio->StrGenero, MedioAudio->Tipo, MedioAudio->StrTipo);
				MedioAudio->Grupo = _BuscaGrupo(MedioAudio->StrGrupo, MedioAudio->Tipo, MedioAudio->Genero, MedioAudio->StrTipo, MedioAudio->StrGenero);
				MedioAudio->Disco = _BuscaDisco(MedioAudio->StrDisco, MedioAudio->Tipo, MedioAudio->Genero, MedioAudio->Grupo, MedioAudio->StrTipo, MedioAudio->StrGenero, MedioAudio->StrGrupo);

				MedioAudio->Tipo->Canciones ++;
				MedioAudio->Tipo->Longitud += MedioAudio->Longitud;
				MedioAudio->Tipo->Tiempo += MedioAudio->Tiempo;
				MedioAudio->Genero->Canciones ++;
				MedioAudio->Genero->Longitud += MedioAudio->Longitud;
				MedioAudio->Genero->Tiempo += MedioAudio->Tiempo;
				MedioAudio->Grupo->Canciones ++;
				MedioAudio->Grupo->Longitud += MedioAudio->Longitud;
				MedioAudio->Grupo->Tiempo += MedioAudio->Tiempo;
				MedioAudio->Disco->Canciones ++;
				MedioAudio->Disco->Longitud += MedioAudio->Longitud;
				MedioAudio->Disco->Tiempo += MedioAudio->Tiempo;
			}
		}
	}

	for (ge = 0; ge < Generos.size(); ge++) {
		Generos[ge]->Tipo->Generos ++;
	}
	for (gr = 0; gr < Grupos.size(); gr++) {
		Grupos[gr]->Tipo->Grupos ++;  
		Grupos[gr]->Genero->Grupos ++;
	}
	for (ds = 0; ds < Discos.size(); ds++) {
		Discos[ds]->Tipo->Discos ++;  
		Discos[ds]->Genero->Discos ++;
		Discos[ds]->Grupo->Discos ++;
	}

}


void BaseDatos::ConstruyeGGD(void) {
	size_t i;
	size_t gr;
	for (i = 0; i < Tipos.size(); i++) delete Tipos[i];
	for (i = 0; i < Generos.size(); i++) delete Generos[i];
	for (i = 0; i < Grupos.size(); i++) delete Grupos[i];
	for (i = 0; i < Discos.size(); i++) delete Discos[i];
	Tipos.clear();
	Generos.clear();
	Grupos.clear();
	Discos.clear();
	TMedioAudio *MedioAudio = NULL;
	for (i = 0; i < Medios.size(); i++) {
		if (Medios[i]->ID_Disco == 0 || Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
			if ( Medios[i]->TipoMedio == Tipo_Medio_Audio) {
//		if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL && Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				MedioAudio = static_cast<TMedioAudio *>(Medios[i]);
				MedioAudio->Tipo = NULL;
				MedioAudio->Genero = _BuscaGenero(MedioAudio->StrGenero, NULL, MedioAudio->StrTipo);
				MedioAudio->Grupo = _BuscaGrupo(MedioAudio->StrGrupo, NULL, MedioAudio->Genero, MedioAudio->StrTipo, MedioAudio->StrGenero);
				MedioAudio->Disco = _BuscaDisco(MedioAudio->StrDisco, NULL, MedioAudio->Genero, MedioAudio->Grupo, MedioAudio->StrTipo, MedioAudio->StrGenero, MedioAudio->StrGrupo);

				MedioAudio->Genero->Canciones ++;
				MedioAudio->Genero->Longitud += MedioAudio->Longitud;
				MedioAudio->Genero->Tiempo += MedioAudio->Tiempo;
				MedioAudio->Grupo->Canciones ++;
				MedioAudio->Grupo->Longitud += MedioAudio->Longitud;
				MedioAudio->Grupo->Tiempo += MedioAudio->Tiempo;
				MedioAudio->Disco->Canciones ++;
				MedioAudio->Disco->Longitud += MedioAudio->Longitud;
				MedioAudio->Disco->Tiempo += MedioAudio->Tiempo;
			}
		}
	}
	for (gr = 0; gr < Grupos.size(); gr++) {
		Grupos[gr]->Genero->Grupos ++;
	}
	for (i = 0; i < Discos.size(); i++) {
		Discos[i]->Genero->Discos ++;
		Discos[i]->Grupo->Discos ++;
	}
}


void BaseDatos::ConstruyeGD(void) {
	size_t i;
	for (i = 0; i < Tipos.size(); i++) delete Tipos[i];
	for (i = 0; i < Generos.size(); i++) delete Generos[i];
	for (i = 0; i < Grupos.size(); i++) delete Grupos[i];
	for (i = 0; i < Discos.size(); i++) delete Discos[i];
	Tipos.clear();
	Generos.clear();
	Grupos.clear();
	Discos.clear();
	TMedioAudio *MedioAudio = NULL;
	for (i = 0; i < Medios.size(); i++) {
		if (Medios[i]->ID_Disco == 0 || Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
			if ( Medios[i]->TipoMedio == Tipo_Medio_Audio) {
//		if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL && Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				MedioAudio = static_cast<TMedioAudio *>(Medios[i]);
				MedioAudio->Tipo = NULL;
				MedioAudio->Genero = NULL;
				MedioAudio->Grupo = _BuscaGrupo(MedioAudio->StrGrupo, NULL, NULL, MedioAudio->StrTipo, MedioAudio->StrGenero);
				MedioAudio->Disco = _BuscaDisco(MedioAudio->StrDisco, NULL, NULL, MedioAudio->Grupo, MedioAudio->StrTipo, MedioAudio->StrGenero, MedioAudio->StrGrupo);

				MedioAudio->Grupo->Canciones ++;
				MedioAudio->Grupo->Longitud += MedioAudio->Longitud;
				MedioAudio->Grupo->Tiempo += MedioAudio->Tiempo;
				MedioAudio->Disco->Canciones ++;
				MedioAudio->Disco->Longitud += MedioAudio->Longitud;
				MedioAudio->Disco->Tiempo += MedioAudio->Tiempo;
			}
		}
	}

	for (i = 0; i < Discos.size(); i++) {
		Discos[i]->Grupo->Discos ++;
	}
}


void BaseDatos::ConstruyeD(void) {
	size_t i;
	for (i = 0; i < Tipos.size(); i++) delete Tipos[i];
	for (i = 0; i < Generos.size(); i++) delete Generos[i];
	for (i = 0; i < Grupos.size(); i++) delete Grupos[i];
	for (i = 0; i < Discos.size(); i++) delete Discos[i];
	Tipos.clear();
	Generos.clear();
	Grupos.clear();
	Discos.clear();
	TMedioAudio *MedioAudio = NULL;
	for (i = 0; i < Medios.size(); i++) {
		if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL && Medios[i]->TipoMedio == Tipo_Medio_Audio) {
			MedioAudio = static_cast<TMedioAudio *>(Medios[i]);
			MedioAudio->Tipo = NULL;
			MedioAudio->Genero = NULL;
			MedioAudio->Disco = _BuscaDisco(MedioAudio->StrDisco, NULL, NULL, NULL, MedioAudio->StrTipo, MedioAudio->StrGenero, MedioAudio->StrGrupo);

			MedioAudio->Disco->Canciones ++;
			MedioAudio->Disco->Longitud += MedioAudio->Longitud;
			MedioAudio->Disco->Tiempo += MedioAudio->Tiempo;
		}
	}
}



const int BaseDatos::MiraIconoCancion(const int Escuchada) {
	int R;
	if (Escuchada == 0) { // Sin escuchar
		R = IDI_AUDIO;
	}
	else {
		if (Escuchada < 25) { // Escuchada de 1 a 24 veces
			R = IDI_AUDIO;
		}
		else {
			if (Escuchada < 50) { // Escuchada de 25 a 49 veces
				R = IDI_AUDIO;
			}
			else { // Escuchada 50 o mas veces
				R = IDI_AUDIO;
			}
		}
	}
	return R;
}


const int BaseDatos::MiraIconoVideo(const int Reproducido) {
	int R;
	if (Reproducido == 0) { // Sin Reproducir
		R = IDI_VIDEO;
	}
	else {
		if (Reproducido < 25) { // Reproducido de 1 a 24 veces
			R = IDI_VIDEO;
		}
		else {
			if (Reproducido < 50) { // Reproducido de 25 a 49 veces
				R = IDI_VIDEO;
			}
			else { // Reproducido 50 o mas veces
				R = IDI_VIDEO;
			}
		}
	}
	return R;
}


void BaseDatos::CargarListaInicio(void) {
	int Intentos = 0;
	switch (Sistema.App.Config.Inicio) {
		case Inicio_Genero_Aleatorio		: Intentos = SeleccionAleatoria(0);		break;
		case Inicio_Grupo_Aleatorio			: Intentos = SeleccionAleatoria(1);		break;
		case Inicio_Disco_Aleatorio			: Intentos = SeleccionAleatoria(2);		break;
		case Inicio_Canciones_Aleatorias	: Intentos = SeleccionAleatoria(3);		break;
//		case Inicio_Canciones_MasEscuchadas	: Intentos = SeleccionAleatoria(4);		break;
		case Inicio_Cualquier_Cosa			: Intentos = SeleccionAleatoria(-1);	break;
	}
}


const int BaseDatos::SeleccionAleatoria(const int Al) {
	if (Medios.size() == 0) return 0;
	int Sw             = Al;
	int	Intentos	   = 0;
	int TotalCanciones = 0;
	if (Al == -1) Sw = DWLRand(4);
	while (TotalCanciones == 0)  {
		Intentos ++;
		switch (Sw) {
			case 0 : TotalCanciones = GeneroAleatorio();			break;
			case 1 : TotalCanciones = GrupoAleatorio();				break;
			case 2 : TotalCanciones = DiscoAleatorio();				break;
			case 3 : TotalCanciones = CancionesAleatorias(50);		break;
//			case 4 : TotalCanciones = CancionesMasEscuchadas(50);   break;
		}
		if (Intentos == 10) break;
	}
	Sistema.App.Media.AbrirPrimerMedioLista();
	return Intentos;
}


void BaseDatos::GeneroSeleccionado(const int Pos) {
	if (StrGeneros.size() == 0) return;
	bool NL = false;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	for (unsigned int i = 0; i < Medios.size(); i++) {
		if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
			if (static_cast<TMedioAudio *>(Medios[i])->StrGenero == StrGeneros[Pos]) {
				if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1)	{ }
				else																	{ Sistema.App.ListaC.AgregarMedio(Medios[i]); }
			}
		}
	}
	
	
	Sistema.App.ListaC.ActualizarTodo();
	if (NL == true) Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__GENERO_SELECCIONADO_S, StrGeneros[Pos]->Nombre());
	else            Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
}


const int BaseDatos::GeneroAleatorio(StrTp *StrGenero) {
	if (StrGeneros.size() == 0) return 0;
	bool NL                 = false;
	int  CancionesAgregadas = 0;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	size_t Sw = DWLRand(StrGeneros.size());

	StrTp *AG = NULL;
	if (StrGenero == NULL) AG = StrGeneros[Sw];
	else                   AG = StrGenero;
	for (unsigned int i = 0; i < Medios.size(); i++) {
		if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
			if (static_cast<TMedioAudio *>(Medios[i])->StrGenero == AG) {
				if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1) { }
				else {
					if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
						Sistema.App.ListaC.AgregarMedio(Medios[i]);
						CancionesAgregadas ++;
					}
				}
			}
		}
	}
	
	Sistema.App.ListaC.ActualizarTodo();
	if (NL == true) {
		if (StrGenero == NULL)	Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__GENERO_ALEATORIO_S, AG->Nombre());
		else					Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__GENERO_SUGERIDO_S, AG->Nombre());
	}
	else						Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	return CancionesAgregadas;
}


const int BaseDatos::GrupoAleatorio(StrTp *StrGrupo) {
	if (StrGrupos.size() == 0) return 0;
	size_t Sw	            = DWLRand(StrGrupos.size());
	bool NL                 = false;
	int  CancionesAgregadas = 0;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;

	StrTp *AG = NULL;
	if (StrGrupo == NULL) AG = StrGrupos[Sw];
	else                  AG = StrGrupo;
//	DWLStdString Tmp = Sistema.App.Idioma(31); Tmp += TEXT(" ("); Tmp += StrGrupos[Sw]->Nombre(); Tmp += TEXT(')'); // Grupo aleatorio (Grupo)
	for (unsigned int i = 0; i < Medios.size(); i++) {
		if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
			if (static_cast<TMedioAudio *>(Medios[i])->StrGrupo == AG) {
				if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1) { }
				else {
					if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
						Sistema.App.ListaC.AgregarMedio(Medios[i]);
						CancionesAgregadas ++;
					}
				}
			}
		}
	}
	Sistema.App.ListaC.ActualizarTodo();
	if (NL == true) {
		if (StrGrupo == NULL)	Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__GRUPO_ALEATORIO_S, AG->Nombre());
		else					Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__GRUPO_SUGERIDO_S, AG->Nombre());
	}
	else						Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	return CancionesAgregadas;
}


const int BaseDatos::DiscoAleatorio(StrTp *StrDisco) {
	if (StrDiscos.size() == 0) return 0;
	size_t  Sw                 = DWLRand(StrDiscos.size());
	int		CancionesAgregadas = 0;
	bool NL                 = false;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;

	StrTp *AD = NULL;
	if (StrDisco == NULL) AD = StrDiscos[Sw];
	else                  AD = StrDisco;
	for (unsigned int i = 0; i < Medios.size(); i++) {
		if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
			if (static_cast<TMedioAudio *>(Medios[i])->StrDisco == AD) {
				if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1) { }
				else {
					if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
						Sistema.App.ListaC.AgregarMedio(Medios[i]);
						CancionesAgregadas ++;
					}
				}
			}
		}
	}
	Sistema.App.ListaC.ActualizarTodo();
	if (NL == true) {
		if (StrDisco == NULL)	Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__DISCO_ALEATORIO_S, AD->Nombre());
		else					Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__DISCO_SUGERIDO_S, AD->Nombre());
	}
	else            Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
//	if (NL == true) Sistema.App.BDList.NombreLista = Tmp;
//	else            Sistema.App.BDList.NombreLista = Sistema.App.Idioma(61);
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	return CancionesAgregadas;
}



const int BaseDatos::CancionesAleatorias(const int Num) {
	if (Medios.size() == 0) return 0;
	size_t		  Maximo             = static_cast<size_t>(Num);
	int           CancionesAgregadas = 0;
	bool          NL                 = false;
	size_t        CanAct             = DWLRand(Medios.size());
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	if (Maximo > Medios.size()) Maximo = Medios.size();
	for (Maximo = Maximo; Maximo > 0; Maximo --) {
		CanAct = DWLRand(Medios.size());
		if (Medios[CanAct]->TipoMedio == Tipo_Medio_Audio) {
			if (Sistema.App.Config.ExcluirNota1 == true && Medios[CanAct]->Nota == 1) {  }
			else { 
                if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[CanAct]->ID_Disco) != NULL) {
				    Sistema.App.ListaC.AgregarMedio(Medios[CanAct]);
				    CancionesAgregadas ++;
                }
			}
		}
	}
	
	Sistema.App.ListaC.ActualizarTodo();
	if (NL == true) Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__D_CANCIONES_ALEATORIAS, Sistema.App.ListaC.TotalItems());
	else            Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	return CancionesAgregadas;
}


void BaseDatos::Estrellas5(void) {
	bool         NL     = false;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	for (unsigned int i = 0; i < Medios.size(); i++) {
        if (Medios[i]->Nota == 5) {
            if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
                Sistema.App.ListaC.AgregarMedio(Medios[i]);
            }
        }
	}
	
	Sistema.App.ListaC.ActualizarTodo();
	if (NL == true) Sistema.App.ListaC.NombreLista = IDIOMA___CANCIONES_CON_5_ESTRELLAS_;
	else			Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
}


const int BaseDatos::CancionesMasEscuchadas(const int Num) { 
	unsigned int Maximo = static_cast<unsigned int>(Num);
	unsigned int J      = 0;
	unsigned int N      = 0;
	bool         NL     = false;
	unsigned int i;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	if (Maximo > Medios.size()) Maximo = Medios.size();
	std::vector<TMedio *> Top;
	Top.resize(Maximo);
	for (i = 0; i < Maximo; i++) Top[i] = Medios[i];
	for (i = Maximo; i < Medios.size(); i++) {
		for (J = 0; J < Maximo; J++) {
			if (Medios[i]->Reproducido > Top[J]->Reproducido) {
				if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
					if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1) {  }
					else {
                        if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
		    				for (N = Maximo -1; N > J; N--) Top[N] = Top[N-1];
			    			Top[J] = Medios[i];
                        }
					}
					break;
				}
			}
		}
	}
	for (i = 0; i < Maximo; i++) { 
        if (Top[i]->Reproducido > 0) Sistema.App.ListaC.AgregarMedio(Top[i]);
	}
	Sistema.App.ListaC.ActualizarTodo();
	if (NL == true) Sistema.App.ListaC.NombreLista.sprintf(IDIOMA___LAS_D_CANCIONES_MAS_ESCUCHADAS_, Sistema.App.ListaC.TotalItems());
	else            Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	return Sistema.App.ListaC.TotalItems();
}


void BaseDatos::GeneroMasEscuchado(void) {
	DWORD Maximo = 0;
	int   Pos    = -1;
	bool  NL     = false;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	unsigned int i;
	for (i = 0; i < StrGeneros.size(); i++) { // Calculo el genero
		if (StrGeneros[i]->Escuchado > Maximo) {
			Maximo = StrGeneros[i]->Escuchado;
			Pos = i;
		}
	}
	if (Pos != -1) { // Si existe alguno lo añado
//		DWLStdString Tmp = SO.Sistema.App.Idioma(91); Tmp += TEXT(" ("); Tmp += StrGeneros[Pos]->Nombre.c_str(); Tmp += TEXT(')'); // Genero (Genero)
		for (i = 0; i < Medios.size(); i++) {
			if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				if (static_cast<TMedioAudio *>(Medios[i])->StrGenero == StrGeneros[Pos]) { 
					if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1) { }
					else {
						if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
							Sistema.App.ListaC.AgregarMedio(Medios[i]); 
						}
					}
				}
			}
		}
		Sistema.App.ListaC.ActualizarTodo();
		
		if (NL == true) Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__GENERO_MAS_ESCUCHADO_S, StrGeneros[Pos]->Nombre());
		else            Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
		if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	}
}


void BaseDatos::GrupoMasEscuchado(void) {
	DWORD Maximo = 0;
	int   Pos    = -1;
	bool  NL     = false;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	unsigned int i;
	for (i = 0; i < StrGrupos.size(); i++) { // Calculo el genero
		if (StrGrupos[i]->Escuchado > Maximo) {
			Maximo = StrGrupos[i]->Escuchado;
			Pos = i;
		}
	}
	if (Pos != -1) { // Si existe alguno lo añado
		//DWLStdString Tmp = SO.Sistema.App.Idioma(90); Tmp += TEXT(" ("); Tmp += StrGrupos[Pos]->Nombre.c_str(); Tmp += TEXT(')'); // Genero (Genero)
		for (i = 0; i < Medios.size(); i++) {
			if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				if (static_cast<TMedioAudio *>(Medios[i])->StrGrupo == StrGrupos[Pos]) { 
					if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1) { }
					else {
						if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
							Sistema.App.ListaC.AgregarMedio(Medios[i]); 
						}
					}
				}
			}
		}
		Sistema.App.ListaC.ActualizarTodo();
		if (NL == true) Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__GRUPO_MAS_ESCUCHADO_S, StrGrupos[Pos]->Nombre());
		else            Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
		if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	}
}


void BaseDatos::DiscoMasEscuchado(void) {
	DWORD Maximo = 0;
	int   Pos    = -1;
	bool  NL     = false;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	unsigned int i;
	for (i = 0; i < StrDiscos.size(); i++) { // Calculo el genero
		if (StrDiscos[i]->Escuchado > Maximo) {
			Maximo = StrDiscos[i]->Escuchado;
			Pos = i;
		}
	}
	if (Pos != -1) { // Si existe alguno lo añado
		//DWLStdString Tmp = SO.Sistema.App.Idioma(89); Tmp += TEXT(" ("); Tmp += StrDiscos[Pos]->Nombre.c_str(); Tmp += TEXT(')'); // Genero (Genero)
		for (i = 0; i < Medios.size(); i++) {
			if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				if (static_cast<TMedioAudio *>(Medios[i])->StrDisco == StrDiscos[Pos]) { 
					if (Sistema.App.Config.ExcluirNota1 == true && Medios[i]->Nota == 1) { }
					else {
						if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
							Sistema.App.ListaC.AgregarMedio(Medios[i]); 
						}
					}
				}
			}
		}
		Sistema.App.ListaC.ActualizarTodo();
		if (NL == true) Sistema.App.ListaC.NombreLista.sprintf(IDIOMA__DISCO_MAS_ESCUCHADO_S, StrDiscos[Pos]->Nombre());
		else            Sistema.App.ListaC.NombreLista = IDIOMA___MODIFICADA_;
		if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	}
}


const int BaseDatos::RevisarBD(DWL::ControlesEx::DWLBarraEx *BarraProgreso) {
	FILE *Archivo = NULL;
	BarraProgreso->BarraEx_Estilos.Colores.ReiniciarColores();
	BarraProgreso->Maximo(Medios.size());
	BarraProgreso->Valor(0);
	int MediosEliminados = 0;
	UINT TotalBarra = Medios.size();
	if (Sistema.App.Config.Buscar_FiltroCoincidencias == true) {
		TotalBarra += (StrGeneros.size() + StrGrupos.size() + StrDiscos.size());
	}
	Revisando = true;
	bool Cancelado = false;
	int i = 0;
	for (i = static_cast<int>(Medios.size()) - 1; i > -1; i--) {
		if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(Medios[i]->ID_Disco) != NULL) {
			_wfopen_s(&Archivo, Medios[i]->Path(), TEXT("r"));
			if (Archivo == NULL) { // NO EXISTE
				BorrarMedio(i);
				MediosEliminados ++;
			}
			else {
				fclose(Archivo);
			}
		}
		Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Valor(Medios.size() - i, TotalBarra);
		BarraProgreso->Valor_SumarValor(1);
		Sistema.App.Eventos_Mirar();
		if (Revisando == false) {
			Cancelado = true;
			break;
		}
	}

	if (MediosEliminados != 0) ActualizarArbol();

	if (Revisando == true) Sistema.App.BD.RepararAnomaliasMediosAudio(BarraProgreso);
	Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Ocultar();
	Sistema.App.PlayerEx.BarraTareas.Resaltar();
	Revisando = false;

/*	DWL::DWLString Str;
	if (Cancelado == false) Str.sprintf(TEXT("Revision terminada, se han eliminado un total de %d entradas no validas"), MediosEliminados);
	else					Str.sprintf(TEXT("Revision cancelada!, se han eliminado un total de %d entradas no validas"), MediosEliminados);
	static VentanaMensaje_Aceptar MsgErr;
	MsgErr.MostrarMensaje(Str(), TEXT("Revision de la Base de Datos"), 0);*/
	return MediosEliminados;
}


void BaseDatos::BorrarMedio(TMedio *C) {
	unsigned int i;
	for (i = 0; i < Medios.size(); i++) {
		if (Medios[i] == C) {
			if (Medios[i]->Item != NULL) Sistema.App.ListaC.EliminarMedio(Medios[i]->Item);
			delete Medios[i];
			Medios.erase(Medios.begin() + i);
			return;
		}
	}
}

void BaseDatos::BorrarMedio(const UINT C) {
	if (C > 0 && C < Medios.size()) {
		if (Medios[C]->Item != NULL) Sistema.App.ListaC.EliminarMedio(Medios[C]->Item);
		delete Medios[C];
		Medios.erase(Medios.begin() + C);
	}
}


StrTp *BaseDatos::GeneroMasEscuchadoStrTp(void) {
	int		GME = -1;
	DWORD	Maximo = 0;
	for (size_t i = 0; i < StrGeneros.size(); i++) {
		if (StrGeneros[i]->Escuchado > Maximo) {
			Maximo = StrGeneros[i]->Escuchado;
			GME = i;
		}
	}
	if (GME == -1)	return NULL;
	return StrGeneros[GME];
}


StrTp *BaseDatos::GrupoMasEscuchadoStrTp(void) {
	int		GME = -1;
	DWORD	Maximo = 0;
	for (size_t i = 0; i < StrGrupos.size(); i++) {
		if (StrGrupos[i]->Escuchado > Maximo) {
			Maximo = StrGrupos[i]->Escuchado;
			GME = i;
		}
	}
	if (GME == -1)	return NULL;
	return StrGrupos[GME];
}


StrTp *BaseDatos::DiscoMasEscuchadoStrTp(void) {
	int		GME = -1;
	DWORD	Maximo = 0;
	for (size_t i = 0; i < StrDiscos.size(); i++) {
		if (StrDiscos[i]->Escuchado > Maximo) {
			Maximo = StrDiscos[i]->Escuchado;
			GME = i;
		}
	}
	if (GME == -1) return NULL;
	return StrDiscos[GME];
}


void BaseDatos::CrearSugerenciasGenero(void) {
	_SugerenciaGenero1 = _CrearSugerenciaGenero();
	_SugerenciaGenero2 = _CrearSugerenciaGenero();
}

void BaseDatos::CrearSugerenciasGrupo(void) {
	_SugerenciaGrupo1 = _CrearSugerenciaGrupo();
	_SugerenciaGrupo2 = _CrearSugerenciaGrupo();
}

void BaseDatos::CrearSugerenciasDisco(void) {
	_SugerenciaDisco1 = _CrearSugerenciaDisco();
	_SugerenciaDisco2 = _CrearSugerenciaDisco();
}


StrTp *BaseDatos::_CrearSugerenciaGenero(void) {
	if (StrGeneros.size() == 0) return NULL;
	UINT Canciones = 0;
	size_t Sw = 0;
	size_t i = 0;
	UINT Intentos = 10;
	while (Canciones == 0 && Intentos != 0) {
		Sw = DWLRand(StrGeneros.size());
		for (i = 0; i < Medios.size(); i++) {
			if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				if (static_cast<TMedioAudio *>(Medios[i])->StrGenero == StrGeneros[Sw]) { 
					Canciones ++;
					break;
				}
			}
		}
		Intentos --;
	}
	if (Intentos > 0) return StrGeneros[Sw];
	return NULL;
}

StrTp *BaseDatos::_CrearSugerenciaGrupo(void) {
	if (StrGrupos.size() == 0) return NULL;

	UINT Canciones = 0;
	size_t Sw = 0;
	size_t i = 0;
	UINT Intentos = 10;
	while (Canciones == 0 && Intentos != 0) {
		Sw = DWLRand(StrGrupos.size());
		for (i = 0; i < Medios.size(); i++) {
			if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				if (static_cast<TMedioAudio *>(Medios[i])->StrGrupo == StrGrupos[Sw]) { 
					Canciones ++;
					break;
				}
			}
		}
		Intentos --;
	}
	if (Intentos != 0) return StrGrupos[Sw];
	return NULL;
}

StrTp *BaseDatos::_CrearSugerenciaDisco(void) {
	if (StrDiscos.size() == 0) return NULL;

	UINT Canciones = 0;
	size_t Sw = 0;
	size_t i = 0;
	UINT Intentos = 10;
	while (Canciones == 0 && Intentos != 0) {
		Sw = DWLRand(StrDiscos.size());
		for (i = 0; i < Medios.size(); i++) {
			if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
				if (static_cast<TMedioAudio *>(Medios[i])->StrDisco == StrDiscos[Sw]) { 
					Canciones ++;
					break;
				}
			}
		}
		Intentos --;
	}
	if (Intentos != 0) return StrDiscos[Sw];
	return NULL;
}

const int BaseDatos::BuscarStrTp(const TCHAR *Nombre, std::vector<StrTp *> &Vec) {
	for (int i = 0; i < static_cast<int>(Vec.size()); i++) {
		if (Vec[i]->Nombre.Comparar(Nombre) == true) return i;
//		if (DWLStrCmpi(Nombre, Vec[i]->Nombre()) == 0) return i;
	}
	return -1;
}


// Tipos : 0 Genero, 1 Grupo, 2 Disco
const int BaseDatos::_CompararStrTp(StrTp &Origen, StrTp &Destino, const int Tipo) {

	int Distancia = _Distancia(Origen.Nombre, Destino.Nombre);
	int Distancia2 = 0;
	if (Distancia != 1 && Tipo == 1) { // Si la comprobación no es válida falta ver si solapando los dos strings se obtiene resultado
		// Parte 7.1 especial para los Grupos
		// Esta parte compara las dos entradas para ver si la entrada mas pequeña corresponde con la parte inicial de la entrada mas grande
		// Un ejemplo seria "Iron Maiden" y "Iron Maiden LP tal"
		//                   ===========     ===========
		// Para generos y discos parece ser bastante inutil, pero en gurpos algunas veces he encontrado cosas que podrian eliminarse
		// Una pequeña cosa interesante a añadir es que a la palabra mas pequeña se le añadira un espacio al final para asi hacer una comparación 
		// de palabras enteras.
		bool			PrimeraComprobacion = false;
		DWL::DWLString	Temp;
		if (Origen.Nombre.Tam() > Destino.Nombre.Tam()) {
			Temp = Destino.Nombre(); Temp += TEXT(" ");	// Añado un espacio al final del nombre corto "Nombre "
			Distancia2 = _Distancia(Origen.Nombre.SubStr(0, Temp.Tam()), Temp);
			if (Distancia2 == 0 || Distancia2 == 1)
//			if (Distancia2 == 1 || Distancia2 == 2)
				PrimeraComprobacion = true;
//			if (Origen.Nombre.SubStr(0, Temp.Tam()) == Temp)   PrimeraComprobacion = true;
		}
		else {
			Temp = Origen.Nombre(); Temp += TEXT(" ");		// Añado un espacio al final del nombre corto "Nombre "
			Distancia2 = _Distancia(Destino.Nombre.SubStr(0, Temp.Tam()), Temp);
			if (Distancia2 == 0 || Distancia2 == 1)
				PrimeraComprobacion = true;
		}

		// Si la comprobación ha dado valida miraremos los directorios de penultimo nivel a ver si coinciden
		// c:\mp3\rage\disc\can.mp3							5
		// c:\mp3\rage against the machine\disc\can.mp3		5
		// 0   1              2              3     4
		// En el caso de que cualquiera de los dos no tenga mas de 4 
		if (PrimeraComprobacion == true) {
			DWL::DWLTexto_Separado SepOrigenGrp(Origen.Medios[0]->Path, TEXT('\\'));
			DWL::DWLTexto_Separado SepDestinoGrp(Destino.Medios[0]->Path, TEXT('\\'));
			if (SepOrigenGrp.TotalTextos() > 3 && SepDestinoGrp.TotalTextos() > 3) {
/*				DWL::DWLDebug::ImprimirDebug(SepOrigenGrp[SepOrigenGrp.TotalTextos() - 3].Texto());
				DWL::DWLDebug::ImprimirDebug(TEXT(" "));
				DWL::DWLDebug::ImprimirDebug(SepDestinoGrp[SepDestinoGrp.TotalTextos() - 3].Texto());
				DWL::DWLDebug::ImprimirDebug(TEXT("\n"));*/
				if (SepOrigenGrp[SepOrigenGrp.TotalTextos() - 3].Comparar(SepDestinoGrp[SepDestinoGrp.TotalTextos() - 3].Texto()) == true) { // los dos directorios de penultimo nivel son iguales
					return 1;
				}
			}
		}
	}

	else if (Distancia == 1 && Tipo == 2) { // Si la comprobación es valida y el tipo es disco, miramos si no es un disco doble
		// Parte 7.2 especial para discos
		// En el caso de que la comprobacion sea positiva y estemos analizando un disco, debemos tener en cuenta que puede ser un disco doble.
		// Un ejemplo de disco doble seria el sigueinte :
		//  c:\mp3\iron maiden\disc\disc 1\01 can.mp3       -> "disc 1", "cd 1", "CD I", "disk I", y miles mas..
		//  c:\mp3\iron maiden\disc\disc 2\01 can.mp3       -> "disc 2", "cd 2", "CD II", "disk II", y miles mas..
		// Si realmente existe una anomalia, lo mas logico es que dicha anomalia se encuentre en la misma carpeta, por lo que habra que compararlas

		// Escaneamos todas las anomalias para ver si tienen pista 01
		UINT z1 = 0;
		UINT z2 = 0;
		bool Pista1Origen = false;
		bool Pista1Destino = false;
		for (z1 = 0; z1 < Origen.Medios.size() && Pista1Origen == false; z1 ++) {
			if (Origen.Medios[z1]->Pista == 1) Pista1Origen = true;
		}
		for (z2 = 0; z2 < Destino.Medios.size() && Pista1Destino == false; z2 ++) {
			if (Destino.Medios[z2]->Pista == 1) Pista1Destino = true;
		}
		// Existen canciones con la pista 01 en las dos anomalias, es un disco doble.
		if (Pista1Origen == true && Pista1Destino == true) return 100;

		for (z1 = Origen.Medios[0]->Path.Tam() -1; z1 > 0; z1--) {
			if (Origen.Medios[0]->Path[z1] == TEXT('\\')) break;
		}
		for (z2 = Destino.Medios[0]->Path.Tam() -1; z2 > 0; z2--) {
			if (Destino.Medios[0]->Path[z2] == TEXT('\\')) break;
		}

		// Los directorios son iguales, no es un disco doble
		if (Origen.Medios[0]->Path.SubStr(0, z1) == Destino.Medios[0]->Path.SubStr(0, z2))	return 1;
		else																				return 100;
//		if (DirectoriosIguales == true) return 1;
//		if (DirectoriosIguales == false) return 100;
	}

	return Distancia;
}

const int BaseDatos::_Distancia(DWL::DWLString &Origen, DWL::DWLString &Destino) {
	// Step 1
	const int n = Origen.Tam();
	const int m = Destino.Tam();

	if (n < 5) return n + m; // Si origen es mas pequeño que 4 caracteres
	if (m < 5) return m + n; // Si destino es mas pequeño que 4 caracteres
//	if (n < 3) return n; // Si origen es mas pequeño que 2 caracteres
//	if (m < 3) return m; // Si destino es mas pequeño que 2 caracteres

	typedef std::vector< std::vector<int> > Tmatrix; 
	Tmatrix matrix(n + 1);

	// Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
	// allow for allocation on declaration of 2.nd dimension of vec of vec
	for (int i = 0; i <= n; i++) matrix[i].resize(m+1);

	// Step 2
	for (int i = 0; i <= n; i++) matrix[i][0]=i;
	for (int j = 0; j <= m; j++) matrix[0][j]=j;
	// Si se llevan mas de dos caracteres de tamaño no hace falta comprobar nada
	if (n - m < 2 && n - m > -2) {
		// Step 3
		for (int i = 1; i <= n; i++) {
			const TCHAR s_i = Origen[i-1];
			// Step 4
			for (int j = 1; j <= m; j++) {
				const TCHAR t_j = Destino[j-1];

				// Step 5
				int cost;
				if (s_i == t_j)  cost = 0;
				else		     cost = 1;

				// Step 6
				const int above = matrix[i - 1][j];
				const int left  = matrix[i][j - 1];
				const int diag  = matrix[i - 1][j - 1];
				int       cell  = min(above + 1, min(left + 1, diag + cost));

				// Step 6A: Cover transposition, in addition to deletion,
				// insertion and substitution. This step is taken from:
				// Berghel, Hal ; Roach, David : "An Extension of Ukkonen's 
				// Enhanced Dynamic Programming ASM Algorithm"
				// (http://www.acm.org/~hlb/publications/asm/asm.html)

				if (i > 2 && j > 2) {
					int trans = matrix[i - 2][j - 2] + 1;
					if (Origen[i - 2] != t_j)	trans++;
					if (s_i != Destino[j - 2])	trans++;
					if (cell > trans)			cell = trans;
				}
				matrix[i][j] = cell;
			}
		}
	}

	if (n - m < 2 && n - m > -2)	return matrix[n][m];
	else							return 100;
}



void BaseDatos::RepararAnomaliasMediosAudio(DWL::ControlesEx::DWLBarraEx *Barra) {
	SOLO_DEBUG(
		DWORD			TiempoMS = GetTickCount();
		DWL::DWLString	TmpStr;
	);

	Revisando = true;

	// El primer paso es enumerar cuantas canciones tiene cada strtp (para los tipos no es necesario enumerar ni reparar nada)
	// De paso tambien nos quedaremos con el primer medio encontrado de cada strtp
	TMedioAudio *TmpAudio = NULL; 
	size_t       i = 0;
	for (i = 0; i < Medios.size(); i++) {
		if (Medios[i]->TipoMedio == Tipo_Medio_Audio) {
			TmpAudio = static_cast<TMedioAudio *>(Medios[i]);
			TmpAudio->StrGenero->Medios.push_back(TmpAudio);
			TmpAudio->StrGrupo->Medios.push_back(TmpAudio);
			TmpAudio->StrDisco->Medios.push_back(TmpAudio);
		}
	}

	// Revisamos que no existan StrTps sin canciones... (en la version 0.57 puede ocurrir)
	for (i = StrGeneros.size(); i != 0; i--) {
		if (StrGeneros[i -1]->Medios.size() == 0) {
			delete StrGeneros[i -1];
			StrGeneros.erase(StrGeneros.begin() + (i -1));
		}
	}
	for (i = StrGrupos.size(); i != 0; i--) {
		if (StrGrupos[i -1]->Medios.size() == 0) {
			delete StrGrupos[i -1];
			StrGrupos.erase(StrGrupos.begin() + (i -1));
		}
	}
	for (i = StrDiscos.size(); i != 0; i--) {
		if (StrDiscos[i -1]->Medios.size() == 0) {
			delete StrDiscos[i -1];
			StrDiscos.erase(StrDiscos.begin() + (i -1));
		}
	}

	if (Sistema.App.Config.Buscar_FiltroCoincidencias == false) {
		Revisando = false;
		return;
	}
	UINT TotalBarra = (Medios.size() + StrGeneros.size() + StrGrupos.size() + StrDiscos.size());

	Barra->Maximo(StrGeneros.size() + StrGrupos.size() + StrDiscos.size());
	Barra->Valor(0);
	Barra->BarraEx_Estilos.Colores.DegradadoSuperior = RGB(0, 200, 0);
	Barra->BarraEx_Estilos.Colores.DegradadoInferior = RGB(0, 160, 0);
	Barra->BarraEx_Estilos.Colores.DegradadoResaltado = RGB(0, 220, 0);

	// Creamos un vector de anomalias de generos
	// Se creara una matriz con anomalias, dichas anomalias constaran de todos aquellos generos que se parezcan entre si
	// Para determinar si un genero se parece a otro, se utilizara el algoritmo de distancia entre palabras con la precisión a 1 caracter.
	// En ultimo lugar para determinar que anomalia será la decisión correcta nos basaremos en la que tenga mas canciones.
	UINT							Tam = StrGeneros.size();
	UINT							Tam2 = 0;
	std::vector<AnomaliaEstandar *>	Anomalias;
	AnomaliaEstandar               *NuevaAnomalia;
	size_t							i2 = 0;
	size_t							i3 = 0;
	int								TDistancia = 0;
	bool                            Existe = false;
	for (i = 0; i < Tam; i++) {
		if (Revisando == false) break;
		for (i2 = 0; i2 < Tam; i2++) {
			if (i != i2) {
				TDistancia = _CompararStrTp(*StrGeneros[i], *StrGeneros[i2], 0);
				if (TDistancia == 1) {
					Existe = false;
					Tam2 = Anomalias.size();
					for (i3 = 0; i3 < Anomalias.size(); i3 ++) {
						if (Anomalias[i3]->MirarAnomalia(StrGeneros[i], StrGeneros[i2]) == true) {
							Existe = true;
							break;
						}
					}
					if (Existe == false) {
						// No existe ninguna de las 2 anomalias, agrego una nueva entrada para las 2
						NuevaAnomalia = new AnomaliaEstandar(StrGeneros[i], StrGeneros[i2]);
						Anomalias.push_back(NuevaAnomalia);
					}
				}
			}
		}
		Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Valor(Medios.size() + i, TotalBarra);
		Barra->Valor_SumarValor(1);
		Sistema.App.Eventos_Mirar();
	}

	// Para ver las anomalias en el debug
	#if defined _DEBUG
		DWL::DWLDebug::ImprimirDebug(TEXT("Anomalias Generos %d\n"), Anomalias.size());
		for (i = 0; i < Anomalias.size(); i++) {
			TmpStr = Anomalias[i]->Anomalia(Anomalias[i]->Eleccion())->Nombre;
			TmpStr += TEXT(" = [");
			for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2++) {
				TmpStr += TEXT(' ');
				TmpStr += Anomalias[i]->Anomalia(i2)->Nombre;
			}
			TmpStr += TEXT(" ]\n");
			DWL::DWLDebug::ImprimirDebug(TmpStr());
		}
	#endif

	// Aplicamos los cambios de los generos a los medios
	for (i = 0; i < Anomalias.size(); i ++) { // de 0 al tope de anomalias
		if (Revisando == false) break;
		for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2++) { // de 0 al tope de entradas
			if (i2 != Anomalias[i]->Eleccion()) {	// Si la entrada no es la elegida
				for (i3 = 0; i3 < Medios.size(); i3++) {
					if (Medios[i3]->TipoMedio == Tipo_Medio_Audio) { 
						TmpAudio = static_cast<TMedioAudio *>(Medios[i3]);
						if (TmpAudio->StrGenero == Anomalias[i]->Anomalia(i2)) {
							TmpAudio->StrGenero = Anomalias[i]->AplicarNombreFinal();
						}
					}
				}
			}
		}
	}
	// Borramos los StrGeneros que sobran
	for (i = 0; i < Anomalias.size(); i ++) { // de 0 al tope de anomalias
		if (Revisando == false) break;
		for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2 ++) { // de 0 al tope de anomalias
			if (i2 != Anomalias[i]->Eleccion()) {	// Si la entrada no es la elegida
				for (i3 = StrGeneros.size(); i3 != 0; i3 --) {
					if (StrGeneros[i3 - 1] == Anomalias[i]->Anomalia(i2)) {
						delete StrGeneros[i3 - 1];
						StrGeneros.erase(StrGeneros.begin() + (i3 - 1));
						break;
					}
				}
			}
		}
	}

	// Elimino anomalias sobre generos de la memoria
	for (i = 0; i < Anomalias.size(); i++) delete Anomalias[i];
	Anomalias.resize(0);

	// Creamos un vector de anomalias de grupos
	// Se creara una matriz con anomalias, dichas anomalias constaran de todos aquellos generos que se parezcan entre si
	// Para determinar si un grupo se parece a otro, se utilizara el algoritmo de distancia entre palabras con la precisión a 1 caracter. 
	// Ademas añadiremos una comprobación solapando los dos nombres. Hay probabilidades de que ciertos nombres de grupo vengan de la siguiente forma :
	//  Str1 = "Motorhead" 
	//  Str2 = "Motorhead BY PINGU"
	// En este caso hay que mirar si los paths de las canciones de esos grupos tienen relación, por ejemplo :
	//  Path1 = "C:\mp3\Motörhead\Motorhead 1977 (LP Motorhead Completo) by EL MAG\Motörhead - 01 - Motorhead.mp3"
	//  Path2 = "C:\mp3\Motörhead\Motorhead 1979 (LP Overkill Completo) by EL MAG\Motörhead - 04 - I'll Be Your Sister.mp3"
	//  Path1 y Path2 coinciden en el penultimo nivel de directorio "c:\mp3\Motörhead\" dicho nivel es considerado como el grupo en el analizador. Por ello seria una anomalia válida.
	// Nos podemos encontrar el caso de que aunque solapados coincidan no sean el mismo grupo, como un buen ejemplo :
	//  Str1 = "Rage"
	//  Str2 = "Rage against the machine"
	//  Path1 = "C:\mp3\Rage\[2006] Speak Of The Dead\01 - Mortituri Te Salutant  (instrumental).mp3"
	//  Path2 = "C:\mp3\Rage against the machine\Evil empire\01 PEOPLE OF THE SUN.mp3"
	//  Path1 y Path2 no coinciden en el penultimo nivel de directorio, uno es "c:\mp3\Rage\", y el otro es "c:\mp3\Rage against the machine\". La anomalia no es válida.
	// En ultimo lugar para determinar que anomalia será la decisión correcta nos basaremos en la que tenga mas canciones.
	Tam = StrGrupos.size();
	Tam2 = 0;
	for (i = 0; i < Tam; i++) {
		if (Revisando == false) break;
		for (i2 = 0; i2 < Tam; i2++) {
			if (i != i2) {
				TDistancia = _CompararStrTp(*StrGrupos[i], *StrGrupos[i2], 1);
				if (TDistancia == 1) {
					Existe = false;
					Tam2 = Anomalias.size();
					for (i3 = 0; i3 < Anomalias.size(); i3 ++) {
						if (Anomalias[i3]->MirarAnomalia(StrGrupos[i], StrGrupos[i2]) == true) {
							Existe = true;
							break;
						}
					}
					if (Existe == false) {
						// No existe ninguna de las 2 anomalias, agrego una nueva entrada para las 2
						NuevaAnomalia = new AnomaliaEstandar(StrGrupos[i], StrGrupos[i2]);
						Anomalias.push_back(NuevaAnomalia);
					}
				}
			}
		}
		Sistema.App.Eventos_Mirar();
		Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Valor((Medios.size() + StrGeneros.size()) + i, TotalBarra);
		Barra->Valor_SumarValor(1);
	}
	// Para ver las anomalias en el debug
	#if defined _DEBUG
		DWL::DWLDebug::ImprimirDebug(TEXT("Anomalias Grupos %d\n"), Anomalias.size());
		for (i = 0; i < Anomalias.size(); i++) {
			TmpStr = Anomalias[i]->Anomalia(Anomalias[i]->Eleccion())->Nombre;
			TmpStr += TEXT(" = [");
			for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2++) {
				TmpStr += TEXT(' ');
				TmpStr += Anomalias[i]->Anomalia(i2)->Nombre;
			}
			TmpStr += TEXT(" ]\n");
			DWL::DWLDebug::ImprimirDebug(TmpStr());
		}
	#endif
	// Aplicamos los cambios de los grupos a los medios
	for (i = 0; i < Anomalias.size(); i ++) { // de 0 al tope de anomalias
		if (Revisando == false) break;
		for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2++) { // de 0 al tope de entradas
			if (i2 != Anomalias[i]->Eleccion()) {	// Si la entrada no es la elegida
				for (i3 = 0; i3 < Medios.size(); i3++) {
					if (Medios[i3]->TipoMedio == Tipo_Medio_Audio) { 
						TmpAudio = static_cast<TMedioAudio *>(Medios[i3]);
						if (TmpAudio->StrGrupo == Anomalias[i]->Anomalia(i2)) {
							TmpAudio->StrGrupo = Anomalias[i]->AplicarNombreFinal();
						}
					}
				}
			}
		}
	}
	// Borramos los StrGrupos que sobran
	for (i = 0; i < Anomalias.size(); i ++) { // de 0 al tope de anomalias
		if (Revisando == false) break;
		for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2 ++) { // de 0 al tope de anomalias
			if (i2 != Anomalias[i]->Eleccion()) {	// Si la entrada no es la elegida
				for (i3 = StrGrupos.size(); i3 != 0; i3 --) {
					if (StrGrupos[i3 - 1] == Anomalias[i]->Anomalia(i2)) {
						delete StrGrupos[i3 - 1];
						StrGrupos.erase(StrGrupos.begin() + (i3 - 1));
						break;
					}
				}
			}
		}
	}
	// Elimino anomalias sobre grupos de la memoria
	for (i = 0; i < Anomalias.size(); i++) delete Anomalias[i];
	Anomalias.resize(0);

	// Creamos un vector de anomalias de discos
	// Se creara una matriz con anomalias, dichas anomalias constaran de todos aquellos generos que se parezcan entre si
	// Para determinar si un disco se parece a otro, se utilizara el algoritmo de distancia entre palabras con la precisión a 1 caracter.
	// El algoritmo de distancia comprobara que no se trate de un disco doble
	// En ultimo lugar para determinar que anomalia será la decisión correcta nos basaremos en la que tenga mas canciones.
	Tam = StrDiscos.size();
	Tam2 = 0;
	for (i = 0; i < Tam; i++) {
		if (Revisando == false) break;
		for (i2 = 0; i2 < Tam; i2++) {
			if (i != i2) {
				TDistancia = _CompararStrTp(*StrDiscos[i], *StrDiscos[i2], 2);
				if (TDistancia == 1) {
					Existe = false;
					Tam2 = Anomalias.size();
					for (i3 = 0; i3 < Anomalias.size(); i3 ++) {
						if (Anomalias[i3]->MirarAnomalia(StrDiscos[i], StrDiscos[i2]) == true) {
							Existe = true;
							break;
						}
					}
					if (Existe == false) {
						// No existe ninguna de las 2 anomalias, agrego una nueva entrada para las 2
						NuevaAnomalia = new AnomaliaEstandar(StrDiscos[i], StrDiscos[i2]);
						Anomalias.push_back(NuevaAnomalia);
					}
				}
			}
		}
		Sistema.App.Eventos_Mirar();
		Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Valor((Medios.size() + StrGeneros.size() + StrGrupos.size()) + i, TotalBarra);
		Barra->Valor_SumarValor(1);
	}
	// Para ver las anomalias en el debug
	#if defined _DEBUG
		DWL::DWLDebug::ImprimirDebug(TEXT("Anomalias Discos %d\n"), Anomalias.size());
		for (i = 0; i < Anomalias.size(); i++) {
			TmpStr = Anomalias[i]->Anomalia(Anomalias[i]->Eleccion())->Nombre;
			TmpStr += TEXT(" = [");
			for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2++) {
				TmpStr += TEXT(' ');
				TmpStr += Anomalias[i]->Anomalia(i2)->Nombre;
			}
			TmpStr += TEXT(" ]\n");
			DWL::DWLDebug::ImprimirDebug(TmpStr());
		}
	#endif
	// Aplicamos los cambios de los discos a los medios
	for (i = 0; i < Anomalias.size(); i ++) { // de 0 al tope de anomalias
		if (Revisando == false) break;
		for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2++) { // de 0 al tope de entradas
			if (i2 != Anomalias[i]->Eleccion()) {	// Si la entrada no es la elegida
				for (i3 = 0; i3 < Medios.size(); i3++) {
					if (Medios[i3]->TipoMedio == Tipo_Medio_Audio) { 
						TmpAudio = static_cast<TMedioAudio *>(Medios[i3]);
						if (TmpAudio->StrDisco == Anomalias[i]->Anomalia(i2)) {
							TmpAudio->StrDisco = Anomalias[i]->AplicarNombreFinal();
						}
					}
				}
			}
		}
	}
	// Borramos los StrDiscos que sobran
	for (i = 0; i < Anomalias.size(); i ++) { // de 0 al tope de anomalias
		if (Revisando == false) break;
		for (i2 = 0; i2 < Anomalias[i]->TotalAnomalias(); i2 ++) { // de 0 al tope de anomalias
			if (i2 != Anomalias[i]->Eleccion()) {	// Si la entrada no es la elegida
				for (i3 = StrDiscos.size(); i3 != 0; i3 --) {
					if (StrDiscos[i3 - 1] == Anomalias[i]->Anomalia(i2)) {
						delete StrDiscos[i3 - 1];
						StrDiscos.erase(StrDiscos.begin() + (i3 - 1));
						break;
					}
				}
			}
		}
	}
	// Elimino anomalias sobre discos de la memoria
	for (i = 0; i < Anomalias.size(); i++) delete Anomalias[i];
	Anomalias.resize(0);

	// Elimino datos sobrantes de los StrTp
	for (i = 0; i < StrGeneros.size(); i++) StrGeneros[i]->Medios.resize(0);
	for (i = 0; i < StrGrupos.size(); i++) StrGrupos[i]->Medios.resize(0);
	for (i = 0; i < StrDiscos.size(); i++) StrDiscos[i]->Medios.resize(0);
	
	// Comprobación para ver cuanto tarda en hacerse la función
	SOLO_DEBUG(TiempoMS = GetTickCount() - TiempoMS);

	Revisando = false;
}
