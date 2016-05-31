#pragma once

#include "ObjetoEscenaTexto.h"
#include <DWLBotonEx.h>
#include <DWLArbolEx.h>
#include <DWLListaEx.h>
#include <DWLBarraEx.h>
#include "ControlMarca.h"
#include <DWLEdicionDesplegableEx.h>
#include "DimensionesFijas.h"
#include "ControlTeclaRapida.h"


enum TipoObjetoEscena {
	ObjetoEscena_NULL				= -1,
	ObjetoEscena_Separacion			=  0,  // Separacion entre objetos
	ObjetoEscena_Texto				=  1,  // Objeto de texto / link
	ObjetoEscena_Boton				=  2,  // Objeto boton
	ObjetoEscena_Lista				=  3,  // Objeto lista report
	ObjetoEscena_Arbol				=  4,  // Objeto arbol
	ObjetoEscena_Barra				=  5,  // Objeto barra progreso
	ObjetoEscena_ComboBox			=  6,  // Objeto EdicionExpandible
	ObjetoEscena_Marca				=  7,  // Objeto marca
	ObjetoEscena_Icono  			=  8,  // Objeto para iconos
	ObjetoEscena_FrameV			    =  9,  // columnas verticales
	ObjetoEscena_FrameH			    =  10, // filas horizontales

	ObjetoEscena_TeclaRapida		=  11 // desaparece
};

enum AlineacionObjetoEscena {
	Alineacion_Centrado   = 0, // se centra en su ancho
	Alineacion_Izquierda  = 1, // se alinea a la izquierda
	Alineacion_Derecha    = 2, // se alinea a la derecha
	Alineacion_Expandible = 3  // se expande a todo el ancho
};


class Escena;
class ObjetoEscena;


class ObjetoEscenaIconoPulsable { 
  public :
			ObjetoEscenaIconoPulsable(const int nIDIcono, const UINT nIDComando, const UINT cX, const UINT cY, const UINT cAncho, const UINT cAlto) { 
				Estado = 0;
				UEstado = 0;
				IDIcono = nIDIcono;
				IDComando = nIDComando;
				Espacio.left = cX;
				Espacio.top = cY;
				Espacio.right = cX + cAncho;
				Espacio.bottom = cY + cAlto;
			};
	UINT	Estado;
	UINT	UEstado;
	int     IDIcono;
	UINT    IDComando;
	RECT    Espacio;
};

class ObjetoEscenaIcono {
  public :
		 ObjetoEscenaIcono(const int nIDIcono, const UINT cX = 0, const UINT cY = 0, const UINT nIDComando = 0) : IDIcono(nIDIcono), X(cX), Y(cY), IDComando(nIDComando) { };
	UINT X;
	UINT Y;
	int  IDIcono;
	UINT IDComando;
};


// Lista de objetos vertical o horizontal
class ObjetoEscenaFrame {
  public:
													ObjetoEscenaFrame(const AlineacionObjetoEscena nAlineacion = Alineacion_Izquierda) : _Alineacion(nAlineacion), _Alto(0), _Ancho(0) { };
												   ~ObjetoEscenaFrame(void);

	void											AgregarTexto13(const TCHAR *Texto, const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible, const UINT MaxAncho = 0, ...);
	void											AgregarTexto15b(const TCHAR *Texto, const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible, const UINT MaxAncho = 0, ...);
													// Texto / iconos / separacion
//	void											AgregarTexto(const TCHAR *Texto, DWL::GDI::DWLFuente *Fuente, DWL::GDI::DWLFuente *FuenteComando, ...);
	ObjetoEscenaIcono							   *AgregarIcono(const int IDImagen, const UINT cAncho, const UINT cAlto, const UINT PosX = 0, const UINT PosY = 0, const UINT IDComando = 0);
	void											AgregarSeparacion(const int nPixels = MARGEN_ESCENA); // Funcion que desplaza la Y actual para el siguiente objeto
													// Frames
	ObjetoEscenaFrame                              *AgregarFrameV(const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible);
	ObjetoEscenaFrame                              *AgregarFrameH(const AlineacionObjetoEscena nAlineacion = Alineacion_Centrado);
													// Controles
	DWL::ControlesEx::DWLBotonEx				   *AgregarBoton(const TCHAR *Texto, const int cAncho, const int cAlto, const int nID, const bool Activado = true, const AlineacionObjetoEscena nAlineacion = Alineacion_Centrado);
	ControlMarca								   *AgregarMarca(const TCHAR *Texto, const int nID, const bool nMarcado = true, const AlineacionObjetoEscena nAlineacion = Alineacion_Izquierda);
	DWL::ControlesEx::DWLListaEx				   *AgregarLista(const int cAlto, const int nID, const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible);
	DWL::ControlesEx::DWLArbolEx				   *AgregarArbol(const int cAlto, const int nID, const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible);
	DWL::ControlesEx::DWLBarraEx				   *AgregarBarraProgreso(const int nMinimo, const int nMaximo, const int nValor, const int nID, const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible);
	DWL::ControlesEx::DWLEdicionDesplegableEx      *AgregarEdicionDesplegable(const TCHAR *Texto, const int nID, const int nIcono = 0, const bool BuscarDirectorios = false, const UINT AltoLista = 87, const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible);
													// Controles BubaTronik
	ControlTeclaRapida							   *AgregarTeclaRapida(const TCHAR *nTexto, const int nID, Tecla *nTecla, const AlineacionObjetoEscena nAlineacion = Alineacion_Expandible);

													// Buscadores de controles
	DWL::ControlesEx::DWLBotonEx				   *BuscarBoton(const int bID);
	ControlMarca								   *BuscarMarca(const int bID);
	DWL::ControlesEx::DWLListaEx				   *BuscarLista(const int bID);
	DWL::ControlesEx::DWLArbolEx				   *BuscarArbol(const int bID);
	DWL::ControlesEx::DWLBarraEx				   *BuscarBarraProgreso(const int bID);
	DWL::ControlesEx::DWLEdicionDesplegableEx      *BuscarEdicionDesplegable(const int bID);
													// Funciones de pintado
	const UINT                                      PintarV(HDC hDCDest, const UINT cX, const UINT cY, const UINT cAncho, const UINT cAlto); // columna
	const UINT                                      PintarH(HDC hDCDest, const UINT cX, const UINT cY, const UINT cAncho, const UINT cAlto); // fila
													// Alineacion
	inline const AlineacionObjetoEscena             Alineacion(void) { return _Alineacion; };

	                                                // Funciones para calcular la altura
	const UINT                                      CalcularAltoV(const UINT AnchoDisponible);
	const UINT                                      CalcularAltoH(const UINT AnchoDisponible);
//	const UINT                                      CalcularH(void);
//	const UINT                                      CalcularV(void);


	std::vector<ObjetoEscena *>                     Objetos;
  protected:
	const UINT									   _ObtenerAltoTexto(ObjetoEscena *Txt, const UINT AnchoDisponible);
    const UINT							           _PintarTexto(HDC hDCDest, ObjetoEscena *Txt, const int cX, const int cY, const UINT AnchoEscena);

	AlineacionObjetoEscena						   _Alineacion;
	UINT                                           _Alto;
	UINT                                           _Ancho;
//	ObjetoEscena                                  *_Padre;
};
