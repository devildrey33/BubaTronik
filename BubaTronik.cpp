// MAIN BubaTronik
//

#include "stdafx.h"
#include "BubaTronikApp.h"



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    DWL::DWL_Iniciar<BubaTronikApp>			Aplicacion;
    return Aplicacion.Ejecutar();
}
