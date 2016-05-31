#pragma once

#include <DWLString.h>
#include <DWLFuente.h>
#include <vector>

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

class Palabra_Pulsable {
public:
						Palabra_Pulsable(void) { };
						Palabra_Pulsable(const int left, const int top, const int right, const int bottom, ObjetoEscenaPalabra *nPalabra, DWL::GDI::DWLFuente *nFuente) { 
							Espacio.left = left;
							Espacio.top = top;
							Espacio.right = right;
							Espacio.bottom = bottom;
							Palabra = nPalabra;
							Fuente = nFuente;
							Estado = 0;
							UEstado = 0;
						};
					   ~Palabra_Pulsable(void) { };
 RECT					Espacio;
 UINT                   Estado;
 UINT                   UEstado;
 ObjetoEscenaPalabra        *Palabra;
 DWL::GDI::DWLFuente   *Fuente;
};

class ObjetoEscenaTexto {
public:
								ObjetoEscenaTexto(void) { };// por eliminar
								ObjetoEscenaTexto(const TCHAR *nTexto, ) { };
							   ~ObjetoEscenaTexto(void) { Borrar_Memoria(); };
 void							Iniciar(const TCHAR *nTexto, DWL::GDI::DWLFuente *nFuente, DWL::GDI::DWLFuente *nFuenteComando, va_list Marker);
 void							Borrar_Memoria(void);
 void							Texto(const TCHAR *nTexto, va_list Marker = NULL);
 std::vector<ObjetoEscenaPalabra *>	Palabras;

 DWL::GDI::DWLFuente		   *Fuente(void)										{ return _Fuente; }; // por eliminar
 void						   *Fuente(DWL::GDI::DWLFuente *nFuente)				{ _Fuente = nFuente; }; // por eliminar
 DWL::GDI::DWLFuente		   *FuenteComando(void)									{ return _FuenteComando; }; // por eliminar
 void						   *FuenteComando(DWL::GDI::DWLFuente *nFuenteComando)	{ _Fuente = nFuenteComando; }; // por eliminar
protected:
 DWL::GDI::DWLFuente		  *_Fuente; // por eliminar
 DWL::GDI::DWLFuente		  *_FuenteComando; // por eliminar
};