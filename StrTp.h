#ifndef BUBATRONIK_STRTP
#define BUBATRONIK_STRTP

#include <DWLString.h>
#include <vector>

class TMedioAudio;
// clase para guardar los tipos string iniciales (tipos , generos , grupos y discos)
class StrTp {
  public :
								StrTp(void) { 
									Escuchado = 0;
			//						TotalCanciones = 0;
//									Medio = NULL;
								};

	DWL::DWLString				Nombre;
	DWORD						Escuchado;
//	UINT            TotalCanciones;
	std::vector<TMedioAudio *>	Medios;
};

#endif