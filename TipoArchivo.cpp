#include "StdAfx.h"
#include "TipoArchivo.h"
#include "Definiciones.h"

//std::vector<TipoArchivo_Datos *> Sistema.App.Tipos;
//UINT                             TipoArchivo::UltimaID = EXT_ULTIMO_TIPO;

TipoArchivo::TipoArchivo(void) {
	UltimaID = EXT_ULTIMO_TIPO;
}

TipoArchivo::~TipoArchivo(void) {
}

void TipoArchivo::BorrarTodosLosTipos(void) {
    for (size_t i = 0; i < Tipos.size(); i++) delete Tipos[i];
    Tipos.resize(0);
};

// Audio por mirar :  AMR, AOB, APE, AU, DTS, M4A, M4P, MKA, MPA, OGA, OMA, RMI, SND, SPX, TTA, VOC, VQF, W64, WV, XM
// DVD : IFO,
void TipoArchivo::TiposPorDefecto(void) {
    BorrarTodosLosTipos();
    AgregarTipoArchivo(TEXT("ASF")       , Tipo_Medio_Video  , EXT_ASF);
    AgregarTipoArchivo(TEXT("AMV")       , Tipo_Medio_Video  , EXT_AMV);
    AgregarTipoArchivo(TEXT("AVI")       , Tipo_Medio_Video  , EXT_AVI);
    AgregarTipoArchivo(TEXT("CDA")       , Tipo_Medio_CDAudio, EXT_CDA);
    AgregarTipoArchivo(TEXT("DIVX")      , Tipo_Medio_Video  , EXT_DIVX);
    AgregarTipoArchivo(TEXT("DV")        , Tipo_Medio_Video  , EXT_DV);
    AgregarTipoArchivo(TEXT("FLAC")      , Tipo_Medio_Audio  , EXT_FLAC);
    AgregarTipoArchivo(TEXT("FLV")       , Tipo_Medio_Video  , EXT_FLV);
    AgregarTipoArchivo(TEXT("GXF")       , Tipo_Medio_Video  , EXT_GXF);
    AgregarTipoArchivo(TEXT("IT")        , Tipo_Medio_Audio  , EXT_IT);
    AgregarTipoArchivo(TEXT("MKV")       , Tipo_Medio_Video  , EXT_MKV);
    AgregarTipoArchivo(TEXT("MID")       , Tipo_Medio_Audio  , EXT_MID);
    AgregarTipoArchivo(TEXT("MOV")       , Tipo_Medio_Video  , EXT_MOV);
    AgregarTipoArchivo(TEXT("MPEG")      , Tipo_Medio_Video  , EXT_MPEG);
    AgregarTipoArchivo(TEXT("MPG")       , Tipo_Medio_Video  , EXT_MPG);
    AgregarTipoArchivo(TEXT("MPV")       , Tipo_Medio_Video  , EXT_MPG);
    AgregarTipoArchivo(TEXT("MPEG1")     , Tipo_Medio_Video  , EXT_MPEG1);
    AgregarTipoArchivo(TEXT("MPEG2")     , Tipo_Medio_Video  , EXT_MPEG2);
    AgregarTipoArchivo(TEXT("MPEG4")     , Tipo_Medio_Video  , EXT_MPEG4);
    AgregarTipoArchivo(TEXT("MPE")       , Tipo_Medio_Video  , EXT_MPE);
    AgregarTipoArchivo(TEXT("MP2")       , Tipo_Medio_Audio  , EXT_MP2);
    AgregarTipoArchivo(TEXT("MP3")       , Tipo_Medio_Audio  , EXT_MP3);
    AgregarTipoArchivo(TEXT("MP4")       , Tipo_Medio_Video  , EXT_MP4);
    AgregarTipoArchivo(TEXT("MP4V")      , Tipo_Medio_Video  , EXT_MP4V);
    AgregarTipoArchivo(TEXT("MTS")       , Tipo_Medio_Video  , EXT_MTS);
    AgregarTipoArchivo(TEXT("MXF")       , Tipo_Medio_Video  , EXT_MXF);
    AgregarTipoArchivo(TEXT("M1V")       , Tipo_Medio_Video  , EXT_M1V);
    AgregarTipoArchivo(TEXT("M2T")       , Tipo_Medio_Video  , EXT_M2T);
    AgregarTipoArchivo(TEXT("M2TS")      , Tipo_Medio_Video  , EXT_M2TS);
    AgregarTipoArchivo(TEXT("M2V")       , Tipo_Medio_Video  , EXT_M2V);
    AgregarTipoArchivo(TEXT("M3U")       , Tipo_Medio_Lista  , EXT_M3U);
    AgregarTipoArchivo(TEXT("M4V")       , Tipo_Medio_Video  , EXT_M4V);
    AgregarTipoArchivo(TEXT("MOD")       , Tipo_Medio_Video  , EXT_MOD);
    AgregarTipoArchivo(TEXT("NSV")       , Tipo_Medio_Video  , EXT_NSV);
    AgregarTipoArchivo(TEXT("NUV")       , Tipo_Medio_Video  , EXT_NUV);
    AgregarTipoArchivo(TEXT("OGG")       , Tipo_Medio_Video  , EXT_OGG);
    AgregarTipoArchivo(TEXT("OGM")       , Tipo_Medio_Video  , EXT_OGM);
    AgregarTipoArchivo(TEXT("OGV")       , Tipo_Medio_Video  , EXT_OGV);
    AgregarTipoArchivo(TEXT("OGX")       , Tipo_Medio_Video  , EXT_OGX);
    AgregarTipoArchivo(TEXT("PS")        , Tipo_Medio_Video  , EXT_PS);
    AgregarTipoArchivo(TEXT("RM")        , Tipo_Medio_Video  , EXT_RM);
    AgregarTipoArchivo(TEXT("RMVB")      , Tipo_Medio_Video  , EXT_RMVB);
    AgregarTipoArchivo(TEXT("SDP")       , Tipo_Medio_Video  , EXT_SDP);
    AgregarTipoArchivo(TEXT("S3M")       , Tipo_Medio_Audio  , EXT_S3M);
    AgregarTipoArchivo(TEXT("TOD")       , Tipo_Medio_Video  , EXT_TOD);
    AgregarTipoArchivo(TEXT("TS")        , Tipo_Medio_Video  , EXT_TS);
    AgregarTipoArchivo(TEXT("VOB")       , Tipo_Medio_Video  , EXT_VOB);
    AgregarTipoArchivo(TEXT("VRO")       , Tipo_Medio_Video  , EXT_VRO);
    AgregarTipoArchivo(TEXT("WAV")       , Tipo_Medio_Audio  , EXT_WAV);
    AgregarTipoArchivo(TEXT("WEBM")      , Tipo_Medio_Audio  , EXT_WEBM);
    AgregarTipoArchivo(TEXT("WMA")       , Tipo_Medio_Audio  , EXT_WMA);
    AgregarTipoArchivo(TEXT("WMV")       , Tipo_Medio_Video  , EXT_WMV);
    AgregarTipoArchivo(TEXT("XM")        , Tipo_Medio_Audio  , EXT_XM);
    AgregarTipoArchivo(TEXT("CRDOWNLOAD"), Tipo_Medio_Video  , EXT_CRDOWNLOAD);
    AgregarTipoArchivo(TEXT("SRT")		 , Tipo_Medio_Subtitulos, EXT_SRT);
	
    UltimaID = EXT_ULTIMO_TIPO;
}

const bool TipoArchivo::AgregarTipoArchivo(const TCHAR *nExtension, const Tipo_Medio nTipo, const int nID) {
    for (size_t i = 0; i < Tipos.size(); i++) {
        if (Tipos[i]->ID == nID || Tipos[i]->Extension.Comparar(nExtension) == true)
            return false;
    }
    TipoArchivo_Datos *Tipo = new TipoArchivo_Datos(nExtension, nTipo, nID);
    Tipos.push_back(Tipo);
    return true;
}

TipoArchivo_Datos *TipoArchivo::BuscarTipoArchivo(const TCHAR *nExtension) {
	DWL::DWLString ExtensionFinal = nExtension;
	for (size_t n = DWLStrLen(nExtension); n > 0; n--) {
		if (nExtension[n - 1] == TEXT('.')) {
			ExtensionFinal = &nExtension[n];
			break;
		}
	}
    for (size_t i = 0; i < Tipos.size(); i++) {
        if (Tipos[i]->Extension.Comparar(ExtensionFinal()) == true) return Tipos[i];
    }
    return NULL;
 }

void TipoArchivo::CopiarTipos(TipoArchivo &Origen) {
	BorrarTodosLosTipos();
	for (size_t i = 0; i< Origen.Tipos.size(); i++) {
		AgregarTipoArchivo(Origen.Tipos[i]->Extension(), Origen.Tipos[i]->Tipo, Origen.Tipos[i]->ID);
		Tipos[i]->Buscar_Asociar = Origen.Tipos[i]->Buscar_Asociar;
	}
}
/*
const bool TipoArchivo::BorrarTipoArchivo(const TCHAR *nExtension) {
    for (size_t i = Tipos.size() -1; i > -1; i--) {
        if (Tipos[i]->Extension.Comparar(nExtension) == true) {
            delete Tipos[i];
            Tipos.erase(Tipos.begin() + i);
            return true;
        }
    }
    return false;
}*/

//void TipoArchivo::EliminarTipoArchivo(const TCHAR *Extension) {
//}

const bool TipoArchivo::CargarTipos(ArchivoBinarioPlayer &Archivo) {
    BorrarTodosLosTipos();
    size_t TotalTipos = 0;
    TipoArchivo_Datos *nTipo = NULL;
    bool Error = false;
    if (Archivo.LeerSizeT(TotalTipos) == FALSE)	return false;
    for (size_t i = 0; i < TotalTipos; i++) {
        nTipo = new TipoArchivo_Datos();
        if (Archivo.LeerString(nTipo->Extension) == FALSE)					Error = true;
        if (Archivo.LeerInt(nTipo->ID) == FALSE)                            Error = true;
        if (Archivo.Leer(&nTipo->Tipo, sizeof(Tipo_Medio)) == FALSE)  Error = true;
        if (Archivo.LeerBool(nTipo->Buscar_Asociar) == FALSE)               Error = true;

        if (Error == true) {
            delete nTipo;
            return false;
        }
        Tipos.push_back(nTipo);
    }
    // Ultima ID Valida
    Archivo.LeerUINT(UltimaID);
    return true;
}

void TipoArchivo::GuardarTipos(ArchivoBinarioPlayer &Archivo) {
    size_t TotalTipos = Tipos.size();
    Archivo.GuardarSizeT(TotalTipos);
    for (size_t i = 0; i < TotalTipos; i++) {
        Archivo.GuardarString(Tipos[i]->Extension);
        Archivo.GuardarInt(Tipos[i]->ID);
        Archivo.Guardar(&Tipos[i]->Tipo, sizeof(Tipo_Medio));
        Archivo.GuardarBool(Tipos[i]->Buscar_Asociar);
    }
    Archivo.GuardarUINT(UltimaID);
}


TipoArchivo_Datos *TipoArchivo::Extension(const TCHAR *Path) {
	DWL::DWLString	Tmp;
	bool			Punto	= false;
	int				TP		= 0;
	size_t			z		= 0;
	TP = DWLStrLen(Path);
//	else				TP = TamPath;
	for (z = TP; z > 0; z--) {
		if (Path[z] == TEXT('.')) {
			Punto = true;
			Tmp = &Path[z + 1];
			break;
		}
	}
	if (Punto == true) {
        for (size_t i = 0; i < Tipos.size(); i++) {
			if (Tipos[i]->Extension.Comparar(Tmp()) == TRUE) return Tipos[i];
        }
    }
    return NULL;
}