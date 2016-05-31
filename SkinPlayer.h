#pragma once
#include <DWLBotonEx.h>
#include <DWLArbolEx.h>
#include <DWLListaEx.h>
#include <DWLMarcaEx.h>
#include <DWLBarraEx.h>
#include <DWLBarraScrollEx.h>
#include <DWLMenuEx.h>
#include <DWLEdicionEx.h>
#include <DWLEdicionDesplegableEx.h>


#define VERSION_SKIN 3

enum Tipo_Datos_Skin {
	TDS_ValorInvalido								= -1,
	// Datos del skin ///////////////////////////
	TDS_Version										=  0,
	TDS_VentanaEx_Borde_Claro						=  1,
	TDS_VentanaEx_Borde_Oscuro						=  2,
	TDS_VentanaEx_Fondo_DegradadoSuperior			=  3,
	TDS_VentanaEx_Fondo_DegradadoInferior			=  4,
	TDS_VentanaEx_Texto_TituloNormal				=  5,
	TDS_VentanaEx_Texto_TituloSombra				=  6,
	TDS_ControlEx_Borde_Normal						=  7,
	TDS_ControlEx_Borde_Claro						=  8,
	TDS_ControlEx_Borde_FocoRaton					=  9,
	TDS_ControlEx_Fondo_DegradadoSuperior			= 10,
	TDS_ControlEx_Fondo_DegradadoInferior			= 11,
	TDS_ControlEx_Fondo_DegradadoResaltado			= 12,
	TDS_ControlEx_Fondo_Seleccionado				= 13,
	TDS_ControlEx_Texto_Normal						= 14,
	TDS_ControlEx_Texto_Resaltado					= 15,
	TDS_ControlEx_Texto_Presionado					= 16,
	TDS_ContenedorEx_Borde_Normal					= 17,
	TDS_ContenedorEx_Borde_Resaltado				= 18,
	TDS_ContenedorEx_Borde_Seleccionado				= 19,
	TDS_ContenedorEx_Cursor							= 20,
	TDS_ContenedorEx_Fondo_Claro					= 21,
	TDS_ContenedorEx_Fondo_Normal					= 22,
	TDS_ContenedorEx_Fondo_Resaltado				= 23,
	TDS_ContenedorEx_Fondo_Presionado				= 24,
	TDS_ContenedorEx_Fondo_Seleccionado				= 25,
	TDS_ContenedorEx_Fondo_SeleccionadoResaltado	= 26,
	TDS_ContenedorEx_Fondo_SubSeleccionado			= 27,
	TDS_ContenedorEx_Fondo_SubSeleccionadoResaltado	= 28,
	TDS_ContenedorEx_Texto_Normal					= 29,
	TDS_ContenedorEx_Texto_Resaltado				= 30,
	TDS_ContenedorEx_Texto_Presionado				= 31,
	TDS_ContenedorEx_Texto_Seleccionado				= 32,
	TDS_ContenedorEx_Texto_SeleccionadoResaltado	= 33,
	TDS_ContenedorEx_Texto_SubSeleccionado			= 34,
	TDS_ContenedorEx_Texto_SubSeleccionadoResaltado = 35,
	TDS_ContenedorEx_Controles_Texto_Normal			= 36,
	TDS_ContenedorEx_Controles_Texto_Resaltado		= 37,
	TDS_ContenedorEx_Controles_Texto_Presionado		= 38,
	TDS_ContenedorEx_Controles_Fondo_Normal			= 39,
	TDS_ContenedorEx_Controles_Fondo_Resaltado		= 40,
	TDS_ContenedorEx_Controles_Fondo_Presionado		= 41
};


/*	// BotonEx //////////////////////////////////
	TDS_Boton_DegradadoSuperior				=  1,
	TDS_Boton_DegradadoInferior				=  2,
	TDS_Boton_DegradadoResaltado			=  3,
	TDS_Boton_TextoNormal					=  4,	
	TDS_Boton_TextoResaltado				=  5,	
	TDS_Boton_TextoDesactivado				=  6,
	TDS_Boton_Borde							=  7,
	// MenuEx ///////////////////////////////////
	TDS_Menu_DegradadoSuperior				=  8,
	TDS_Menu_DegradadoInferior				=  9,
	TDS_Menu_TextoNormal					= 10,
	TDS_Menu_TextoResaltado					= 11,
	TDS_Menu_TextoDesactivado				= 12,
	TDS_Menu_FlechaNormal					= 13,
	TDS_Menu_FlechaResaltada				= 14,
	TDS_Menu_FlechaDesactivada				= 15,
	TDS_Menu_FondoSeleccion					= 16,
	TDS_Menu_Borde							= 17,
	// CheckBoxEx ///////////////////////////////
	TDS_Option_Fondo						= 18,
	TDS_Option_FondoMarcaNormal				= 19,
	TDS_Option_FondoMarcaResaltado			= 20,
	TDS_Option_FondoMarcaPresionado			= 21,
	TDS_Option_BordeRectangulo				= 22,
	TDS_Option_TextoNormal					= 23,
	TDS_Option_TextoResaltado				= 24,
	TDS_Option_TextoPresionado				= 25,
	// ScrolBarEx ///////////////////////////////
	TDS_BarraScroll_DegradadoSuperior		= 26,
	TDS_BarraScroll_DegradadoInferior		= 27,
	TDS_BarraScroll_DegradadoResaltado		= 28,
	TDS_BarraScroll_FlechaNormal			= 29,
	TDS_BarraScroll_FlechaResaltada			= 30,
	TDS_BarraScroll_FlechaPresionada		= 31,
	TDS_BarraScroll_FondoNormal				= 32,
	TDS_BarraScroll_FondoResaltado			= 33,
	TDS_BarraScroll_FondoPresionado			= 34,
	TDS_BarraScroll_Borde					= 35,
	// ListaEx //////////////////////////////////
	TDS_Lista_TextoNormal					= 36,
	TDS_Lista_TextoResaltado				= 37, 
	TDS_Lista_TextoSeleccionado				= 38,
	TDS_Lista_TextoSeleccionadoResaltado	= 39,
	TDS_Lista_TextoPresionado				= 40,
	TDS_Lista_FondoNormal					= 41,
	TDS_Lista_FondoResaltado				= 42,
	TDS_Lista_FondoSeleccionado				= 43,
	TDS_Lista_FondoSeleccionadoResaltado	= 44,
	TDS_Lista_FondoPresionado				= 45,
	TDS_Lista_BrodeSeleccion				= 46,
	// ArbolEx //////////////////////////////////
	TDS_BD_FondoNormal						= 47, 
	TDS_BD_FondoResaltado					= 48,
	TDS_BD_FondoPresionado					= 49,
	TDS_BD_FondoSeleccionado				= 50,
	TDS_BD_FondoSeleccionadoResaltado		= 51,
	TDS_BD_FondoSubSeleccionado				= 52,
	TDS_BD_FondoSubSeleccionadoResaltado	= 53,
	TDS_BD_TextoNormal						= 54, 
	TDS_BD_TextoResaltado					= 55,
	TDS_BD_TextoPresionado					= 56,
	TDS_BD_TextoSeleccionado				= 57,
	TDS_BD_TextoSeleccionadoResaltado		= 58,
	TDS_BD_TextoSubSeleccionado				= 59,
	TDS_BD_TextoSubSeleccionadoResaltado	= 60,
	TDS_BD_BordeSeleccion					= 61,
	TDS_BD_Lineas							= 62,
	TDS_BD_PMTextoNormal					= 63,
	TDS_BD_PMTextoResaltado					= 64,
	TDS_BD_PMTextoPresionado				= 65, 
	TDS_BD_PMFondoNormal					= 66,
	TDS_BD_PMFondoResaltado					= 67,
	TDS_BD_PMFondoPresionado				= 68,
	// BarraEx //////////////////////////////////
	TDS_Barra_DegradadoSuperior				= 69,
	TDS_Barra_DegradadoInferior				= 70,
	TDS_Barra_DegradadoResaltado			= 71,
	TDS_Barra_Fondo							= 72,
	TDS_Barra_Borde							= 73,
	TDS_Barra_FondoVentanaPadre				= 74,
	// EditBox //////////////////////////////////
	TDS_EdicionTextoEx_Fondo				= 75,
	TDS_EdicionTextoEx_FondoVentanaPadre	= 76,
	TDS_EdicionTextoEx_FondoSeleccionado	= 77,
	TDS_EdicionTextoEx_Texto				= 78,
	TDS_EdicionTextoEx_TextoSeleccionado	= 79,
	TDS_EdicionTextoEx_TextoDesactivado		= 80,
	TDS_EdicionTextoEx_Borde				= 81,
	TDS_EdicionTextoEx_Cursor				= 82,
	// ToolTip /////////////////////////////////
	TDS_ToolTipEx_Borde						= 83,
//	TDS_ToolTipEx_BordeClaro				= 84,
	TDS_ToolTipEx_DegradadoInferior			= 84,
	TDS_ToolTipEx_DegradadoSuperior			= 85,
	TDS_ToolTipEx_TextoNormal				= 86,
	TDS_ToolTipEx_TextoSombra				= 87,
//	TDS_EditBox_TextoNormal					= 71,
//	TDS_EditBox_TextoDesactivado			= 72,
//	TDS_EditBox_Fondo						= 73,
	// Otras opciones ////////////////////////////
//	TDS_Mascara_Normal						= 100,
//	TDS_Mascara_Peque						= 101,
//	TDS_Mascara_Propiedades					= 102,
//	TDS_Titulo_X							= 103,
//	TDS_Titulo_Y							= 104,
//	TDS_Controles_X							= 105,
//	TDS_Controles_Y							= 106,
//	TDS_EnmarcarVentanaPadre                = 107,
	// Colores de la ventana /////////////////////
//	TDS_Ventana_TituloNormal				= 200,
//	TDS_Ventana_TituloResaltado				= 201,
//	TDS_Ventana_TituloPresionado			= 202,
	TDS_Ventana_TextoNormal					= 203,
	TDS_Ventana_TextoComando				= 204,
	TDS_Ventana_TextoResaltado				= 205,
	TDS_Ventana_TextoPresionado			    = 206,
	TDS_Ventana_Fondo						= 207,
	TDS_Ventana_Borde						= 208,
	TDS_Ventana_BordeClaro					= 209,
	TDS_Ventana_DegradadoSuperior			= 210,
	TDS_Ventana_DegradadoInferior			= 211,
	TDS_Ventana_DegradadoResaltado			= 212,
	TDS_Ventana_Texto_Msg					= 213,
	TDS_Ventana_Sombra_Msg					= 214*/


class DefinicionCol {
public :
			    DefinicionCol(void) { };
			    DefinicionCol(COLORREF Col, const char *Txtn) { Color = Col; Nombre = Txtn; };
		       ~DefinicionCol(void) { };
 COLORREF		Color;
 std::string	Nombre;
};



class SkinPlayer {
public :
																	SkinPlayer(void) { 
																		Version = 0; 
																	};
																   ~SkinPlayer(void) { 
																	};

 const bool															CargarSkin(const TCHAR *Path);

	
 BOOL																EnmarcarVentanaPadre;
private :
 const bool															EsComentario(std::string &Linea);
 const bool															EsDefinicion(std::string &Linea);
 const bool															ObtenerColor(std::string &Linea, Tipo_Datos_Skin Tipo);
 const bool															ObtenerRGB(std::string &Linea, COLORREF &Color);
 const int															ObtenerInteger(std::string &Linea);
 void																GuardarDefinicion(std::string &Linea);
 Tipo_Datos_Skin													ObtenerTipoDatos(std::string &Linea, int &Inicio, int &Fin);
 void																DesEspaciar(std::string &Linea);
 const bool															BuscarColor(std::string &Nombre, COLORREF &Color);


 std::vector<DefinicionCol>											ColDef;
 int																Version;
};