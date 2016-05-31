#pragma once

#include <DWLVentana.h>
#include <DWLGraficos.h>
#include "ConfigBubaTronik.h"
#include "DimensionesFijas.h"

#define TIMER_MOVIMIENTO 55



class VentanaEx : public DWL::Ventanas::DWLVentana {
public:
							VentanaEx(void);
			               ~VentanaEx(void);
   HWND						CrearVentanaEx(const TCHAR *Nombre, const int cX, const int cY, const int cAncho, const int cAlto, const int nIcono, const TCHAR *Titulo, const bool nMinimizable = true, const bool nMaximizable = true, const UINT AlturaSuperior = VENTANAEX_ALTURA_PARTE_SUPERIOR);
   virtual LRESULT			Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
   virtual void             Evento_PintarEx(HDC hDC) { };
   LRESULT					Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
   LRESULT					Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
   LRESULT					Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
   LRESULT					Evento_Temporizador(const UINT cID);
   virtual void				Evento_ValidarControles(void) { };
   void						Maximizar(const bool Maxi);
   const bool				Maximizado(void) { return _Maximizado; };
   void						PintarTitulo(const bool SoloTitulo, DWL::GDI::DWLhDC *hDCDest);
   DWL::GDI::DWLRegion		Borde;
  protected:
	UINT                   _AltoParteSuperior;
    int                    _Icono;
    DWL::DWLString		   _Titulo;
	// Variables para el redimensionado de esta ventana
	bool					Redimensionando;
	bool				   _Maximizado;
	int						NS;
	int						EO;
};
