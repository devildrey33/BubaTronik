#include "StdAfx.h"
#include "ControlVolumen.h"
#include "BubaTronikApp.h"


HWND ControlVolumen::Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const UINT cID) {
	CrearBarraEx(WS_CHILD | WS_VISIBLE, hWndParent, cX, cY, cAncho, cAlto, cID, DWL_BARRAEX_DESPLAZAMIENTO_HORIZONTAL, 0, 100);
	Valor(Sistema.App.Config.Volumen);
	BarraEx_Estilos.Colores.DegradadoSuperior = RGB(static_cast<int>(static_cast<float>(_BarraEx_Valor) * 2.00f), 255 - static_cast<int>(static_cast<float>(_BarraEx_Valor) * 2.00f), 0);
	BarraEx_Estilos.Colores.DegradadoInferior = RGB(static_cast<int>(static_cast<float>(_BarraEx_Valor) * 1.30f), 255 - static_cast<int>(static_cast<float>(_BarraEx_Valor) * 1.30f), 0);
	BarraEx_Estilos.Colores.DegradadoResaltado = RGB(static_cast<int>(static_cast<float>(_BarraEx_Valor) * 2.55f), 255 - static_cast<int>(static_cast<float>(_BarraEx_Valor) * 2.55f), 0);
	return _hWnd;
}


LRESULT ControlVolumen::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	if (_BarraEx_Estado == DWL_Enum_BarraEx_Estados_Presionado) {
		RECT RC;
		GetClientRect(_hWnd, &RC);
		_BarraEx_Valor = BarraEx_MirarValor(&RC, cX, cY);
	    Sistema.App.Media.Volumen(static_cast<int>(_BarraEx_Valor)); 
	}
	DWL::ControlesEx::DWLBarraEx::Evento_Mouse_Movimiento(cX, cY, Param);
    return 0;
}


LRESULT ControlVolumen::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	RECT RC;
	GetClientRect(_hWnd, &RC);
	_BarraEx_Valor = BarraEx_MirarValor(&RC, cX, cY);
	DWL::ControlesEx::DWLBarraEx::Evento_Mouse_BotonPresionado(Boton, cX, cY, Param);
    Sistema.App.Media.Volumen(static_cast<int>(_BarraEx_Valor)); 
    return 0;
}


LRESULT ControlVolumen::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	RECT RC;
	GetClientRect(_hWnd, &RC);
	_BarraEx_Valor = BarraEx_MirarValor(&RC, cX, cY);
	DWL::ControlesEx::DWLBarraEx::Evento_Mouse_BotonSoltado(Boton, cX, cY, Param);
    return 0;
}


void ControlVolumen::Valor(const long nValor) {
	BarraEx_Estilos.Colores.DegradadoSuperior = RGB(static_cast<int>(static_cast<float>(nValor) * 2.00f), 255 - static_cast<int>(static_cast<float>(nValor) * 2.00f), 0);
	BarraEx_Estilos.Colores.DegradadoInferior = RGB(static_cast<int>(static_cast<float>(nValor) * 1.50f), 255 - static_cast<int>(static_cast<float>(nValor) * 1.50f), 0);
	BarraEx_Estilos.Colores.DegradadoResaltado = RGB(static_cast<int>(static_cast<float>(nValor) * 2.55f), 255 - static_cast<int>(static_cast<float>(nValor) * 2.55f), 0);
	DWL::ControlesEx::DWLBarraEx::Valor(nValor);
}