#pragma once

#include "VentanaTranslucida.h"


class VentanaTranslucidaLista : public VentanaTranslucida {
public:
					VentanaTranslucidaLista(void) { };
		           ~VentanaTranslucidaLista(void) { };
	LRESULT			Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
	LRESULT			Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
	LRESULT			Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
	void			PintarContenido(HDC hDCDestino, RECT *RC);
};


