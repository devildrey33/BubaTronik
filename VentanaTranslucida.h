#pragma once

#include <DWL.h>

class VentanaTranslucida {
public:
					VentanaTranslucida(void) { Presionada = false; PuntoPresion.x = 0; PuntoPresion.y = 0; };
		           ~VentanaTranslucida(void) { };
    void			Crear(const int nTipoVentana, const int nOrdenZ, const int Pos);
	void			Pintar(HDC hDCDestino);
	virtual LRESULT	Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) { return 0; };
	virtual LRESULT	Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) { return 0; };
	virtual LRESULT	Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) { return 0; };
	virtual void    PintarContenido(HDC hDCDestino, RECT *RC) { };
	RECT			Espacio;
	int				OrdenZ; // OrdenZ 0 es la ventana activa
	int				TipoVentana;
	bool			Presionada;
	POINT			PuntoPresion;
};


