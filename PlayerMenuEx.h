#pragma once

#include <DWLMenuEx.h>

class PlayerMenuEx : public DWL::ControlesEx::DWLMenuEx {
public:
				PlayerMenuEx(void);
			   ~PlayerMenuEx(void);
 void			Mostrar(HWND hWndParent);
};
