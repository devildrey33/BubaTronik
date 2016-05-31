#include "StdAfx.h"
#include "ObjetoEscenaTexto.h"
#include "BubaTronikApp.h"

ObjetoEscenaTexto::ObjetoEscenaTexto(const TCHAR *nTexto, TipoFuente nTipoFuente, va_list Marker) { 
	Fuente = nTipoFuente;
	Texto(nTexto, Marker);
};


/*void ObjetoEscenaTexto::Iniciar(const TCHAR *nTexto, DWL::GDI::DWLFuente *nFuente, DWL::GDI::DWLFuente *nFuenteComando, va_list Marker) {
		_Fuente = nFuente; 
		_FuenteComando = nFuenteComando;
		Texto(nTexto, Marker); 
};*/

void ObjetoEscenaTexto::Borrar_Memoria(void) { 
	for (size_t i = 0; i < Palabras.size(); i ++) { 
		delete Palabras[i];
	} 
	Palabras.resize(0);
};


// Calculo el tamaño de las palabras y asigno las IDs para los comandos/links
void ObjetoEscenaTexto::Texto(const TCHAR *nTexto, va_list Marker) {
	Borrar_Memoria();
	ObjetoEscenaPalabra *Tmp;
	//_Texto = nTexto;
	HDC				hDC = GetDC(Sistema.App.PlayerEx.CP.hWnd());
	HFONT			FuenteVieja;
	HFONT           FuenteActual = NULL;
	
	switch (Fuente) {
		default :
		case Tahoma13 :
			FuenteActual = DWL::SO::DWLEstilos::Fuentes.Tahoma13();
			break;
		case Tahoma15b :
			FuenteActual = Sistema.App.Tahoma15b();
			break;
	}
	FuenteVieja = static_cast<HFONT>(SelectObject(hDC, FuenteActual));
	
	DWL::DWLString	Linea;
	SIZE			E;
	int				IDPalabra = 0;
	bool			FinID = false;
	//int				ObligarFuenteComando = 0;
	// Separo la frase por palabras
	for (unsigned int z = 0; z < DWLStrLen(nTexto); z++) {
		switch (nTexto[z]) {
			case TEXT(' ') :
				if (IDPalabra != 0) SelectObject(hDC, DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita());
				else                SelectObject(hDC, FuenteActual);
//				if (IDPalabra != 0)	SelectObject(hDC, _FuenteComando->Fuente());
//				else				SelectObject(hDC, _Fuente->Fuente());

				GetTextExtentPoint32(hDC, Linea(), static_cast<int>(Linea.Tam()), &E);
				Tmp = new ObjetoEscenaPalabra(Linea(), E.cx, IDPalabra);
				Palabras.push_back(Tmp);
				Linea = TEXT("");
				if (FinID == true) { 
					IDPalabra = 0;
					FinID = false;
				}
				break;
			case TEXT('#') : // Obligar a utilizar la fuente comando
				if (IDPalabra != 0) FinID = true;
				else				IDPalabra = -1;
				break;
			case TEXT('$') : // Comando
				if (IDPalabra != 0) FinID = true;
				else				IDPalabra = va_arg(Marker, int);
				break;
			default :
				Linea += nTexto[z];
				break;
		}
	}
	/*GetTextExtentPoint32(hDC, Linea(), static_cast<int>(Linea.Tam()), &E);
	Tmp = new ObjetoEscenaPalabra(Linea(), E.cx, 0);
	Palabras.push_back(Tmp);*/
	if (IDPalabra != 0) SelectObject(hDC, DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita());
	else                SelectObject(hDC, FuenteActual);

	GetTextExtentPoint32(hDC, Linea(), static_cast<int>(Linea.Tam()), &E);
	Tmp = new ObjetoEscenaPalabra(Linea(), E.cx, IDPalabra);
	Palabras.push_back(Tmp);
	if (FinID == true) { 
		IDPalabra = 0;
		FinID = false;
	}

	SelectObject(hDC, FuenteVieja);
	ReleaseDC(Sistema.App.PlayerEx.CP.hWnd(), hDC);
};