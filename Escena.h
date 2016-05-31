#pragma once


#include "ObjetoEscena.h"
#include <DWLGraficos.h>

class ControlPresentacionEx;

// Clase que maneja todos los objetos que salen en la escena, y los pinta
class Escena : public ObjetoEscenaFrame {
public :
												Escena(void) { /*EstadoPalabraPulsable = 0;*/ };
												Escena(ControlPresentacionEx *CPEX);
												// POR ELIMINAR
// ControlTeclaRapida							   *AgregarTeclaRapida(const TCHAR *nTexto, const int nID, Tecla *nTecla);

 void											Evento_Mouse_Movimiento(const int cX, const int cY, WPARAM wParam, const UINT BarraScrollEx_ValorV);
 void											Evento_Mouse_BotonPresionado(const int Boton, const int cX, const int cY, WPARAM wParam, const UINT BarraScrollEx_ValorV);
 void											Evento_Mouse_BotonSoltado(const int Boton, const int cX, const int cY, WPARAM wParam, const UINT BarraScrollEx_ValorV);

 void											Pintar(HDC hDC, const int cX = 0, const int cY = 0);	// Funcion que pinta la escena en el DC especificado
 const UINT										PintarEscena(void); // Funcion que calcula la escena y la pinta en su back buffer
 void											Pintar_PalabrasPulsables(const UINT BarraScrollEx_ValorV);
 void											Pintar_IconosPulsables(const UINT BarraScrollEx_ValorV);

 void											BorrarPulsables(void) {
													for (size_t i = 0; i < PalabrasPulsables.size(); i++) delete PalabrasPulsables[i];
													PalabrasPulsables.resize(0);
													for (size_t i = 0; i < IconosPulsables.size(); i++) delete IconosPulsables[i];
													IconosPulsables.resize(0);
												};
// const UINT										ObtenerAltoTexto(ObjetoEscenaTexto *Txt);
// const UINT                                     ObtenerAlto(void);
 static std::vector <ObjetoEscenaPalabraPulsable *>		PalabrasPulsables;
 static std::vector <ObjetoEscenaIconoPulsable *>			IconosPulsables;
// int											EstadoPalabraPulsable;
 int											UltimaIDResaltada;
 UINT											UltimaIDIcono;
protected:
 ObjetoEscenaPalabraPulsable                              *UPP;
 ControlPresentacionEx						   *CP;
 DWL::GDI::DWLhDC								Buffer;

};		
