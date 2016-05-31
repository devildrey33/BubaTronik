#pragma once

#include <DWLString.h>
#include <DWLFuente.h>
#include <vector>

enum TipoFuente {
	Tahoma13 = 0,
//	Tahoma13b = 1,
	Tahoma15b = 1
};

// Clase que contendra una palabra con su ancho y su ID.
// Si la ID no es 0, es que esa palabra es un comando.
class ObjetoEscenaPalabra {
public :
					ObjetoEscenaPalabra(void) { };
					ObjetoEscenaPalabra(const TCHAR *nPalabra, const UINT nAncho, const int nID = 0) { Palabra = nPalabra; Ancho = nAncho; ID = nID; };
				   ~ObjetoEscenaPalabra(void) { };
 DWL::DWLString		Palabra;
 UINT				Ancho;
 int				ID;
};

class ObjetoEscenaPalabraPulsable {
public:
						ObjetoEscenaPalabraPulsable(void) { };
						ObjetoEscenaPalabraPulsable(const int left, const int top, const int right, const int bottom, ObjetoEscenaPalabra *nPalabra) { 
							Espacio.left = left;
							Espacio.top = top;
							Espacio.right = right;
							Espacio.bottom = bottom;
							Palabra = nPalabra;
							Estado = 0;
							UEstado = 0;
						};

					   ~ObjetoEscenaPalabraPulsable(void) { };
 RECT					Espacio;
 UINT                   Estado;
 UINT                   UEstado;
 ObjetoEscenaPalabra   *Palabra;
// DWL::GDI::DWLFuente   *Fuente;
};

class ObjetoEscenaTexto {
public:
									ObjetoEscenaTexto(void) { }; // por eliminar
									ObjetoEscenaTexto(const TCHAR *nTexto, TipoFuente nTipoFuente, va_list Marker);
								   ~ObjetoEscenaTexto(void) { Borrar_Memoria(); };
// void								Iniciar(const TCHAR *nTexto, DWL::GDI::DWLFuente *nFuente, DWL::GDI::DWLFuente *nFuenteComando, va_list Marker); // por eliminar
 void								Borrar_Memoria(void);
 void								Texto(const TCHAR *nTexto, va_list Marker = NULL);
 std::vector<ObjetoEscenaPalabra *>	Palabras;
 TipoFuente							Fuente;

/* DWL::GDI::DWLFuente		   *Fuente(void)										{ return _Fuente; };
 void						   *Fuente(DWL::GDI::DWLFuente *nFuente)				{ _Fuente = nFuente; };
 DWL::GDI::DWLFuente		   *FuenteComando(void)									{ return _FuenteComando; };
 void						   *FuenteComando(DWL::GDI::DWLFuente *nFuenteComando)	{ _Fuente = nFuenteComando; };
protected:
 DWL::GDI::DWLFuente		  *_Fuente;
 DWL::GDI::DWLFuente		  *_FuenteComando;*/
};