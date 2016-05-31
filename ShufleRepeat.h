#pragma once

#include <DWLGraficos.h>
#include <DWLControlEx.h>

enum Estados_ShufleRepeat {
	ShufleRepeat_Estado_Normal		=  0,
	ShufleRepeat_Estado_Resaltado	=  1,
	ShufleRepeat_Estado_Presionado	=  2,
};

#define ID_BOTON_SHUFLE	33010
#define ID_BOTON_REPEAT	33011



class ShufleRepeat : public DWL::ControlesEx::Base::DWLControlEx {
public:
							ShufleRepeat(void);
				           ~ShufleRepeat(void);
 void						CrearShufleRepeat(HWND hWndParent, const int cX, const int cY);
 void						Repintar();
 void						ShufleMarcado(const bool Marcado);
 void						RepeatMarcado(const bool Marcado);
private:
 LRESULT					Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT					Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT					Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT					Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
 LRESULT					Evento_Mouse_Saliendo(void);
// LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
 DWL::GDI::DWLRegion		Region;
 Estados_ShufleRepeat		Estado_Shufle;
 Estados_ShufleRepeat		Estado_Repeat;
 bool						Shufle_Marcado;
 bool						Repeat_Marcado;
};
