#pragma once
#include <DWLVentana.h>
#include "VentanaTranslucidaLista.h"

#define TIPO_VENTANA_INICIO			0
#define TIPO_VENTANA_LISTA_MEDIOS	1



class VentanaPrincipalEx : public DWL::Ventanas::DWLVentana {
public:
										VentanaPrincipalEx(void);
								       ~VentanaPrincipalEx(void);
	void								Crear(void);
	void								CrearVentanaTranslucida(const int TipoVentana);
	void								Pintar(HDC hDCDestino = NULL);
	VentanaTranslucida				   *VentanaEnPunto(const int cX, const int cY);
	void								VentanaActiva(VentanaTranslucida *nVentana);
	const bool							MirarBordeVentanas(const int cX, const int cY, int &Punto);
	void								CambiarMouse(void);
	void								Repintar(void);
	LRESULT								Evento_Cerrar(void);
	LRESULT								Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
	LRESULT								Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
	LRESULT								Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
	LRESULT								Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
   private:
//	LRESULT CALLBACK					GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VentanaTranslucida				  *_VentanaPresionada;
	std::vector<VentanaTranslucida *>  _Ventanas;
										// Solo para el borde de la ventana
	bool							   _MousePresionado;
	bool							   _RedimensionandoVentana;
	int								   _PuntoRedimension;
};
