#pragma once


#define TECLA_PRESIONADA(Estados, TeclaVirtual) (Estados[TeclaVirtual] & 0x80)
//#define TECLA_SOLTADA(Estados, TeclaVirtual) (Estados[TeclaVirtual] & KF_UP)

class Tecla {
public:
			Tecla(void) { 
			};

			Tecla(const int nTeclaVirtual, const bool nControl = false, const bool nAlt = false, const bool nShift = false) {
				TeclaVirtual	= nTeclaVirtual;
				Control			= nControl;
				Alt				= nAlt;
				Shift			= nShift;
			};

		   ~Tecla(void) { 
		    };

 void		AsignarTecla(const int nTeclaVirtual, const bool nControl = false, const bool nAlt = false, const bool nShift = false) {
				TeclaVirtual	= nTeclaVirtual;
				Control			= nControl;
				Alt				= nAlt;
				Shift			= nShift;
		    };

 const bool TeclaPresionada(PBYTE Estados) {
				bool ControlPresionado	= false;
				bool AltPresionado		= false;
				bool ShiftPresionado	= false;
				
				if (TeclaVirtual > 255)
					return false;

				if (TECLA_PRESIONADA(Estados, VK_CONTROL))	ControlPresionado	= true;
				if (TECLA_PRESIONADA(Estados, VK_MENU))		AltPresionado		= true;
				if (TECLA_PRESIONADA(Estados, VK_SHIFT))	ShiftPresionado		= true;

				if (TECLA_PRESIONADA(Estados, TeclaVirtual) && Control == ControlPresionado && Alt == AltPresionado && Shift == ShiftPresionado) {
					return true;
				}
				return false;
			};
/*
 const bool TeclaSoltada(PBYTE Estados) {
				if (TECLA_SOLTADA(Estados, TeclaVirtual)) {
					return true;
				}
				if (TECLA_SOLTADA(Estados, TeclaVirtual) && TECLA_PRESIONADA(Estados, VK_CONTROL) == static_cast<int>(Control) && TECLA_PRESIONADA(Estados, VK_MENU) == static_cast<int>(Alt)) {
					return true;
				}
				return false;
			};
*/
 int		TeclaVirtual;
 bool		Control;
 bool		Alt;
 bool		Shift;
};
