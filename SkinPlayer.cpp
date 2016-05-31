#include "stdafx.h"
#include "SkinPlayer.h"
#include <string>
#include <fstream>
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);


// Modifica esta macro si añades algun valor mas al skin

// Hay 2 valores que no salen bien, hay que revisar el txt del skin y compararlo con los datos actuales.....
#define TOTAL_VALORES_A_OBTENER 40

// Retorna :
//				TRUE  si ha cargado correctamente el skin
//				FALSE si no encuentra el archivo skin o no tiene suficientes datos
const bool SkinPlayer::CargarSkin(const TCHAR *Path) {
	char			Linea[512];
	Tipo_Datos_Skin Tipo;
	std::ifstream	Archivo;
	std::string		StrLinea;
	DWL::DWLString  PathFinal = Sistema.App.AppPath(); PathFinal += TEXT("GRAFICOS\\"); PathFinal += Path; PathFinal += TEXT(".BubaTronik_Skin");
	int				TotalColores = 0;
	int				Inicio;
	int				Fin;
	bool			SinError = true;
	DWL::DWLString  ErrTxt;
	try { 
		Archivo.open(PathFinal(), std::ios::in);
		while (Archivo.eof() == false && Archivo.good() == true) {
			for (int z = 0; z < 511; z++) Linea[z] = '\0';
			Archivo.getline(Linea, 512);
			StrLinea = Linea;
			DesEspaciar(StrLinea);
			if (EsComentario(StrLinea) == false && StrLinea.size() > 0) {
				if (EsDefinicion(StrLinea) == true) {
					GuardarDefinicion(StrLinea);
				}
				else {
					Tipo = ObtenerTipoDatos(StrLinea, Inicio, Fin);
					switch (Tipo) {
						case TDS_Version :
							Version = ObtenerInteger(StrLinea.substr(Inicio, Fin));
							if (Version != VERSION_SKIN) {
								VentanaMensaje_Aceptar MsgErr(IDIOMA__ERROR_LA_VERSION_DEL_SKIN_NO_ES_VALIDA_PARA_ESTE_REPRODUCTOR, IDIOMA__ERROR, 0);
								Archivo.close();
								DWL::SO::DWLEstilos::Restaurar_Colores_Por_Defecto();
								return false;
							}

							break;
						default :
							if (ObtenerColor(StrLinea.substr(Inicio, Fin), Tipo) == true) TotalColores ++;
							break;
					}
				}
			}
		}
	}
	catch (...) {
	}
	Archivo.close();
	if (TOTAL_VALORES_A_OBTENER != TotalColores) {
		SinError = false;
	}
	ColDef.clear();
	if (SinError == false) {
		DWL::SO::DWLEstilos::Restaurar_Colores_Por_Defecto();
		ErrTxt.sprintf(IDIOMA__ERROR_CARGANDO_ARCHIVO_DE_SKIN_S, Path);
		VentanaMensaje_Aceptar MsgErr2(ErrTxt(), IDIOMA__ERROR, 0);
	}
	return SinError;
}


const bool SkinPlayer::EsComentario(std::string &Linea) {
	if (Linea.size() == 0)					return false;
	if (Linea[0] == '/' && Linea[1] == '/') return true;
	else								    return false;
}


const bool SkinPlayer::EsDefinicion(std::string &Linea) {
	if (Linea.size() == 0) return false;
	if (Linea[0] == '#')   return true;
	else				   return false;
}


void SkinPlayer::GuardarDefinicion(std::string &Linea) {
	
	if (Linea.size() == 0) return;
	unsigned int i;
	for (i = 0; i < Linea.size(); i++) {
		if (Linea[i] == '=') break;
	}
	std::string NombreDefine = Linea;
	NombreDefine[i] = '\0';
	DesEspaciar(NombreDefine);
	COLORREF Color;
	if (ObtenerRGB(Linea.substr(i, Linea.size() -i), Color) == true) {
		DefinicionCol Def(Color, NombreDefine.c_str());
		ColDef.push_back(Def);
	}
}



// Esta funcion retorna el tipo de datos que contiene la linea
Tipo_Datos_Skin SkinPlayer::ObtenerTipoDatos(std::string &Linea, int &Inicio, int &Fin) {
	unsigned int i;
	for (Inicio = 0; Inicio < static_cast<int>(Linea.size()); Inicio++) {
		if (Linea[Inicio] == '"') break;
	}
	for (i = Inicio + 1; i < Linea.size(); i++) {
		if (Linea[i] == '"') break;
	}
	Inicio ++;
	Fin = i - Inicio;
	for (i = 0; i < Linea.size(); i++) {
		if (Linea[i] == '=') break;
	}

	std::string NombreDato = Linea.substr(0, i);
	DesEspaciar(NombreDato);
	// Datos basicos
	if (_strcmpi(NombreDato.c_str(), "Version") == 0)										return TDS_Version;
	if (_strcmpi(NombreDato.c_str(), "VentanaEx_Borde_Claro") == 0)							return TDS_VentanaEx_Borde_Claro;
	if (_strcmpi(NombreDato.c_str(), "VentanaEx_Fondo_DegradadoSuperior") == 0)				return TDS_VentanaEx_Fondo_DegradadoSuperior;
	if (_strcmpi(NombreDato.c_str(), "VentanaEx_Fondo_DegradadoInferior") == 0)				return TDS_VentanaEx_Fondo_DegradadoInferior;
	if (_strcmpi(NombreDato.c_str(), "VentanaEx_Texto_TituloNormal") == 0)					return TDS_VentanaEx_Texto_TituloNormal;
	if (_strcmpi(NombreDato.c_str(), "VentanaEx_Texto_TituloSombra") == 0)					return TDS_VentanaEx_Texto_TituloSombra;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Borde_Normal") == 0)						return TDS_ControlEx_Borde_Normal;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Borde_Claro") == 0)							return TDS_ControlEx_Borde_Claro;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Borde_FocoRaton") == 0)						return TDS_ControlEx_Borde_FocoRaton;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Fondo_DegradadoSuperior") == 0)				return TDS_ControlEx_Fondo_DegradadoSuperior;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Fondo_DegradadoInferior") == 0)				return TDS_ControlEx_Fondo_DegradadoInferior;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Fondo_DegradadoResaltado") == 0)			return TDS_ControlEx_Fondo_DegradadoResaltado;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Fondo_Seleccionado") == 0)					return TDS_ControlEx_Fondo_Seleccionado;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Texto_Normal") == 0)						return TDS_ControlEx_Texto_Normal;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Texto_Resaltado") == 0)						return TDS_ControlEx_Texto_Resaltado;
	if (_strcmpi(NombreDato.c_str(), "ControlEx_Texto_Presionado") == 0)					return TDS_ControlEx_Texto_Presionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Borde_Normal") == 0)						return TDS_ContenedorEx_Borde_Normal;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Borde_Resaltado") == 0)					return TDS_ContenedorEx_Borde_Resaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Borde_Seleccionado") == 0)				return TDS_ContenedorEx_Borde_Seleccionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Cursor") == 0)							return TDS_ContenedorEx_Cursor;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_Claro") == 0)						return TDS_ContenedorEx_Fondo_Claro;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_Normal") == 0)						return TDS_ContenedorEx_Fondo_Normal;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_Resaltado") == 0)					return TDS_ContenedorEx_Fondo_Resaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_Presionado") == 0)					return TDS_ContenedorEx_Fondo_Presionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_Seleccionado") == 0)				return TDS_ContenedorEx_Fondo_Seleccionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_SeleccionadoResaltado") == 0)		return TDS_ContenedorEx_Fondo_SeleccionadoResaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_SubSeleccionado") == 0)			return TDS_ContenedorEx_Fondo_SubSeleccionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Fondo_SubSeleccionadoResaltado") == 0)	return TDS_ContenedorEx_Fondo_SubSeleccionadoResaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Texto_Normal") == 0)						return TDS_ContenedorEx_Texto_Normal;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Texto_Resaltado") == 0)					return TDS_ContenedorEx_Texto_Resaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Texto_Presionado") == 0)					return TDS_ContenedorEx_Texto_Presionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Texto_Seleccionado") == 0)				return TDS_ContenedorEx_Texto_Seleccionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Texto_SeleccionadoResaltado") == 0)		return TDS_ContenedorEx_Texto_SeleccionadoResaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Texto_SubSeleccionado") == 0)			return TDS_ContenedorEx_Texto_SubSeleccionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Texto_SubSeleccionadoResaltado") == 0)	return TDS_ContenedorEx_Texto_SubSeleccionadoResaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Controles_Texto_Normal") == 0)			return TDS_ContenedorEx_Controles_Texto_Normal;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Controles_Texto_Resaltado") == 0)		return TDS_ContenedorEx_Controles_Texto_Resaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Controles_Texto_Presionado") == 0)		return TDS_ContenedorEx_Controles_Texto_Presionado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Controles_Fondo_Normal") == 0)			return TDS_ContenedorEx_Controles_Fondo_Normal;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Controles_Fondo_Resaltado") == 0)		return TDS_ContenedorEx_Controles_Fondo_Resaltado;
	if (_strcmpi(NombreDato.c_str(), "ContenedorEx_Controles_Fondo_Presionado") == 0)		return TDS_ContenedorEx_Controles_Fondo_Presionado;



	return TDS_ValorInvalido;
}


void SkinPlayer::DesEspaciar(std::string &Linea) {
	size_t i = 0;
	char Tmp[512];
	// Mientras linea[i] sea un espacio o una tabulaion
	if (Linea.size() == 0) return;
	while (Linea[i] == ' ' || Linea[i] == '	') i++;
	strcpy_s(Tmp, 512, &Linea[i]);
	for (i = strlen(Tmp) -1; i > 0; i--) {
		if (Tmp[i] == ' ' || Tmp[i] == '	') Tmp[i] = '\0';
		else               break;
	}
	Linea = Tmp;
}


const bool SkinPlayer::BuscarColor(std::string &Nombre, COLORREF &Color) {
	for (unsigned int i = 0; i < ColDef.size(); i++) {
		if (_strcmpi(ColDef[i].Nombre.c_str(), Nombre.c_str()) == 0) {
			Color = ColDef[i].Color;
			return true;
		}
	}
	return false;
}


const bool SkinPlayer::ObtenerColor(std::string &Linea, Tipo_Datos_Skin Tipo) {
	unsigned int	i = 0;
	COLORREF		Color = 0;
	bool			Ok = false;
	if (Linea[i] == '#')	Ok = BuscarColor(Linea, Color);		
	else					Ok = ObtenerRGB(Linea, Color);
	if (Ok == false) 
		return false;
	switch (Tipo) {
		case TDS_VentanaEx_Borde_Claro							: DWL::SO::DWLEstilos::Colores.VentanaEx_Borde_Claro						= Color;						return true;
		case TDS_VentanaEx_Borde_Oscuro							: DWL::SO::DWLEstilos::Colores.VentanaEx_Borde_Oscuro						= Color;						return true;
		case TDS_VentanaEx_Fondo_DegradadoSuperior				: DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior			= Color;						return true;
		case TDS_VentanaEx_Fondo_DegradadoInferior				: DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior			= Color;						return true;
		case TDS_VentanaEx_Texto_TituloNormal					: DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloNormal					= Color;						return true;
		case TDS_VentanaEx_Texto_TituloSombra					: DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloSombra					= Color;						return true;
		case TDS_ControlEx_Borde_Normal							: DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal						= Color;						return true;
		case TDS_ControlEx_Borde_Claro							: DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro						= Color;						return true;
		case TDS_ControlEx_Borde_FocoRaton						: DWL::SO::DWLEstilos::Colores.ControlEx_Borde_FocoRaton					= Color;						return true;
		case TDS_ControlEx_Fondo_DegradadoSuperior				: DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior			= Color;						return true;
		case TDS_ControlEx_Fondo_DegradadoInferior				: DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior			= Color;						return true;
		case TDS_ControlEx_Fondo_DegradadoResaltado				: DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoResaltado			= Color;						return true;
		case TDS_ControlEx_Fondo_Seleccionado					: DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_Seleccionado					= Color;						return true;
		case TDS_ControlEx_Texto_Normal							: DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Normal						= Color;						return true;
		case TDS_ControlEx_Texto_Resaltado						: DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Resaltado					= Color;						return true;
		case TDS_ControlEx_Texto_Presionado						: DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Presionado					= Color;						return true;
		case TDS_ContenedorEx_Borde_Normal						: DWL::SO::DWLEstilos::Colores.ContenedorEx_Borde_Normal					= Color;						return true;
		case TDS_ContenedorEx_Borde_Resaltado					: DWL::SO::DWLEstilos::Colores.ContenedorEx_Borde_Resaltado					= Color;						return true;
		case TDS_ContenedorEx_Borde_Seleccionado				: DWL::SO::DWLEstilos::Colores.ContenedorEx_Borde_Seleccionado				= Color;						return true;
		case TDS_ContenedorEx_Cursor							: DWL::SO::DWLEstilos::Colores.ContenedorEx_Cursor							= Color;						return true;
		case TDS_ContenedorEx_Fondo_Claro						: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Claro						= Color;						return true;
		case TDS_ContenedorEx_Fondo_Normal						: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal					= Color;						return true;
		case TDS_ContenedorEx_Fondo_Resaltado					: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Resaltado					= Color;						return true;
		case TDS_ContenedorEx_Fondo_Presionado					: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Presionado				= Color;						return true;
		case TDS_ContenedorEx_Fondo_Seleccionado				: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Seleccionado				= Color;						return true;
		case TDS_ContenedorEx_Fondo_SeleccionadoResaltado		: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_SeleccionadoResaltado		= Color;						return true;
		case TDS_ContenedorEx_Fondo_SubSeleccionado				: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_SubSeleccionado			= Color;						return true;
		case TDS_ContenedorEx_Fondo_SubSeleccionadoResaltado	: DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_SubSeleccionadoResaltado	= Color;						return true;
		case TDS_ContenedorEx_Texto_Normal						: DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Normal					= Color;						return true;
		case TDS_ContenedorEx_Texto_Resaltado					: DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Resaltado					= Color;						return true;
		case TDS_ContenedorEx_Texto_Presionado					: DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Presionado				= Color;						return true;
		case TDS_ContenedorEx_Texto_Seleccionado				: DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Seleccionado				= Color;						return true;
		case TDS_ContenedorEx_Texto_SeleccionadoResaltado		: DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_SeleccionadoResaltado		= Color;						return true;
		case TDS_ContenedorEx_Texto_SubSeleccionado				: DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_SubSeleccionado			= Color;						return true;
		case TDS_ContenedorEx_Texto_SubSeleccionadoResaltado	: DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_SubSeleccionadoResaltado	= Color;						return true;
		case TDS_ContenedorEx_Controles_Texto_Normal			: DWL::SO::DWLEstilos::Colores.ContenedorEx_Controles_Texto_Normal			= Color;						return true;
		case TDS_ContenedorEx_Controles_Texto_Resaltado			: DWL::SO::DWLEstilos::Colores.ContenedorEx_Controles_Texto_Resaltado		= Color;						return true;
		case TDS_ContenedorEx_Controles_Texto_Presionado		: DWL::SO::DWLEstilos::Colores.ContenedorEx_Controles_Texto_Presionado		= Color;						return true;
		case TDS_ContenedorEx_Controles_Fondo_Normal			: DWL::SO::DWLEstilos::Colores.ContenedorEx_Controles_Fondo_Normal			= Color;						return true;
		case TDS_ContenedorEx_Controles_Fondo_Resaltado			: DWL::SO::DWLEstilos::Colores.ContenedorEx_Controles_Fondo_Resaltado		= Color;						return true;
		case TDS_ContenedorEx_Controles_Fondo_Presionado		: DWL::SO::DWLEstilos::Colores.ContenedorEx_Controles_Fondo_Presionado		= Color;						return true;
	}
	return false;
}



const bool SkinPlayer::ObtenerRGB(std::string &Linea, COLORREF &Color) {
	unsigned int i = 0;
	char Col[4];
	Col[3] = 0;
	int Cols[3];
	int Contador = 0;
	int Posicion = 0;
	int TMP = 0;
	while (Contador != 3) {
		if (i == Linea.size()) 
			return false;
		switch (Linea[i]) {
			case ' ' :
				if (Posicion != 0) {
					Col[Posicion] = '\0';
					Cols[Contador] = atoi(Col);
					Contador ++;
					Posicion = 0;
					TMP = 0;
				}
				break;
			case '0' :	case '1' :	case '2' :	case '3' :	case '4' : 
			case '5' :	case '6' :	case '7' :	case '8' :	case '9' :
				Col[Posicion] = Linea[i];
				Posicion ++;
				if (Posicion == 3 || Linea.size() == (i + 1)) {
					Col[Posicion] = '\0';
					Cols[Contador] = atoi(Col);
					Contador ++;
					Posicion = 0;
					TMP = 0;
				}
				break;
		}
		i++;
		if (Contador == 3) break;
	}
	Color = RGB(Cols[0], Cols[1], Cols[2]);
	return true;
}



const int SkinPlayer::ObtenerInteger(std::string &Linea) {
	int Posicion = 0;
	char Pos[10];
	int TMP = 0;
	for (unsigned int i = 0; i < Linea.size(); i++) {
		switch (Linea[i]) {
			case '0' :	case '1' :	case '2' :	case '3' :	case '4' : 
			case '5' :	case '6' :	case '7' :	case '8' :	case '9' :
				Pos[Posicion] = Linea[i];	
				Posicion ++;
				break;
		}
	}
	Pos[Posicion] = '\0';
	TMP = atoi(Pos);
	return TMP;
}


