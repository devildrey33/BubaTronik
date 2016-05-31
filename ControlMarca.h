#pragma once

#include <DWLMarcaEx.h>

class ControlMarca : public DWL::ControlesEx::DWLMarcaEx {
public:
				ControlMarca(void);
			   ~ControlMarca(void);
 void			MarcaEx_Evento_PintarMarca(HDC hDC, RECT *Espacio, const int nEstado);
};


class ControlMarcaMsg : public ControlMarca {
public:
				ControlMarcaMsg(void);
			   ~ControlMarcaMsg(void);
 void			MarcaEx_Evento_PintarFondo(HDC hDC, RECT *Espacio);
};
