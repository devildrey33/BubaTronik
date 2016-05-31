#pragma once
#include <string>
#include <DWLBarraEx.h>

class EnviarEmail {
  public:
					EnviarEmail(void);
		           ~EnviarEmail(void);
    const bool		Enviar(const TCHAR *Path, DWL::ControlesEx::DWLBarraEx *Barra);
  private:
	bool           _Error;
	const bool	   _Revisar(int iStatus, char *szFunction);
	std::string   &_base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
};

