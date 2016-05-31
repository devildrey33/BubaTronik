#pragma once



class AnomaliaEstandar {
public :
							AnomaliaEstandar(void) { 
								_Eleccion = 0;
							};
							// Constructor que inicia 2 anomalias
							AnomaliaEstandar(StrTp *Anomalia1, StrTp *Anomalia2) { 
								_Eleccion = 0;
								if (Anomalia2->Medios.size() > Anomalia1->Medios.size()) {
									_Anomalias.push_back(Anomalia2);
									_Anomalias.push_back(Anomalia1);
								}
								else {
									_Anomalias.push_back(Anomalia1);
									_Anomalias.push_back(Anomalia2);
								}
							};

					       ~AnomaliaEstandar(void) { 
							};
							// Funcion que determina si alguna de las dos anomalias existe
						    // y las introduce en el vector
						    // Retorna false si no existe ninguna de las 2
 const bool					MirarAnomalia(StrTp *NuevaAnomalia1, StrTp *NuevaAnomalia2) {
								unsigned int	i = 0;
								unsigned int	z = 0;
								BYTE			Existe = 0;
								unsigned int    Tam = _Anomalias.size();
								for (i = 0; i < Tam; i++) {
									if (NuevaAnomalia1->Nombre == _Anomalias[i]->Nombre) Existe += 2;
									if (NuevaAnomalia2->Nombre == _Anomalias[i]->Nombre) Existe += 4;
								}
								switch (Existe) {
									case 2 : // existe el primero
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Medios.size() <= NuevaAnomalia2->Medios.size()) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia2);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia2);
										return true;
										break;
									case 4 : // Esixte el segundo
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Medios.size() <= NuevaAnomalia1->Medios.size()) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia1);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia1);
										return true;
										break;
									case 6 : // Existen los dos...
										return true;
									default : // No existe ninguno
										return false;
								}
							};

 inline const unsigned int	TotalAnomalias(void) {
								return _Anomalias.size();
							};

 inline StrTp  	           *Anomalia(const unsigned int Pos) {
								return _Anomalias[Pos];
							};

 inline const unsigned int  Eleccion(void) {
								return _Eleccion;
							};

 inline void				Eleccion(const unsigned int nEleccion) {
								if (nEleccion < _Anomalias.size()) {
									_Eleccion = nEleccion;
								}
							};

 inline void				EliminarEntrada(const unsigned int Pos) {
								_Anomalias.erase(_Anomalias.begin() + Pos);
							};

 inline void                CambiarNombreFinal(const TCHAR *nTxt) {
								_NombreFinal = nTxt;
							};

 inline const TCHAR        *ObtenerNombreFinal(void) {
								if (_NombreFinal.Tam() == 0)	return _Anomalias[_Eleccion]->Nombre();
								else							return _NombreFinal();
							};

 inline StrTp              *AplicarNombreFinal(void) {
								if (_NombreFinal.Tam() > 0) {
									_Anomalias[_Eleccion]->Nombre = _NombreFinal;
									_NombreFinal = TEXT("");
								}
								return _Anomalias[_Eleccion];
							}
private :
 DWL::DWLString            _NombreFinal;

 std::vector<StrTp *>	   _Anomalias;

 unsigned int			   _Eleccion;
};






class AnomaliaGen {
public :
							AnomaliaGen(void) { 
								_Eleccion = 0;
							};

							AnomaliaGen(TGenero *Anomalia1, TGenero *Anomalia2) { 
								_Eleccion = 0;
								if (Anomalia2->Canciones > Anomalia1->Canciones) {
									_Anomalias.push_back(Anomalia2);
									_Anomalias.push_back(Anomalia1);
								}
								else {
									_Anomalias.push_back(Anomalia1);
									_Anomalias.push_back(Anomalia2);
								}
							};

					       ~AnomaliaGen(void) { 
							};

 const bool					MirarAnomalia(TGenero *NuevaAnomalia1, TGenero *NuevaAnomalia2) {
								unsigned int	i = 0;
								unsigned int    z = 0;
								BYTE			Existe = 0;
								unsigned int    Tam = _Anomalias.size();
								for (i = 0; i < Tam; i++) {
									if (NuevaAnomalia1->Nombre == _Anomalias[i]->Nombre) Existe += 2;
									if (NuevaAnomalia2->Nombre == _Anomalias[i]->Nombre) Existe += 4;
								}
								switch (Existe) {
									case 2 : // existe el primero
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Canciones <= NuevaAnomalia2->Canciones) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia2);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia2);
										return true;
										break;
									case 4 : // Esixte el segundo
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Canciones <= NuevaAnomalia1->Canciones) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia1);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia1);
										return true;
										break;
									case 6 : // Existen los dos...
										return true;
									default : // No existe ninguno
										return false;
								}
							};

 inline const unsigned int	TotalAnomalias(void) {
								return _Anomalias.size();
							};

 inline const TGenero	   *Anomalia(const unsigned int Pos) {
								return _Anomalias[Pos];
							};

 inline const unsigned int  Eleccion(void) {
								return _Eleccion;
							};

 inline void				Eleccion(const unsigned int nEleccion) {
								if (nEleccion < _Anomalias.size()) {
									_Eleccion = nEleccion;
								}
							};

 inline void				EliminarEntrada(const unsigned int Pos) {
								_Anomalias.erase(_Anomalias.begin() + Pos);
							};

 inline void                CambiarNombreFinal(const TCHAR *nTxt) {
								_NombreFinal = nTxt;
							};

 inline const TCHAR        *ObtenerNombreFinal(void) {
								if (_NombreFinal.Tam() == 0)	return _Anomalias[_Eleccion]->Nombre->Nombre();
								else							return _NombreFinal();
							};

 inline StrTp              *AplicarNombreFinal(void) {
								if (_NombreFinal.Tam() > 0) {
									_Anomalias[_Eleccion]->Nombre->Nombre = _NombreFinal;
									_NombreFinal = TEXT("");
								}
								return _Anomalias[_Eleccion]->Nombre;
							}

private :
 DWL::DWLString            _NombreFinal;

 std::vector<TGenero *>	   _Anomalias;

 unsigned int			   _Eleccion;
};



class AnomaliaGrp {
public :
							AnomaliaGrp(void) { 
								_Eleccion = 0;
							};

							AnomaliaGrp(TGrupo *Anomalia1, TGrupo *Anomalia2) { 
								_Eleccion = 0;
								if (Anomalia2->Canciones > Anomalia1->Canciones) {
									_Anomalias.push_back(Anomalia2);
									_Anomalias.push_back(Anomalia1);
								}
								else {
									_Anomalias.push_back(Anomalia1);
									_Anomalias.push_back(Anomalia2);
								}
							};

					       ~AnomaliaGrp(void) { 
							};

 const bool					MirarAnomalia(TGrupo *NuevaAnomalia1, TGrupo *NuevaAnomalia2) {
								unsigned int	i = 0;
								unsigned int    z = 0;
								BYTE			Existe = 0;
								unsigned int    Tam = _Anomalias.size();
								for (i = 0; i < Tam; i++) {
									if (NuevaAnomalia1->Nombre == _Anomalias[i]->Nombre) Existe += 2;
									if (NuevaAnomalia2->Nombre == _Anomalias[i]->Nombre) Existe += 4;
								}
								switch (Existe) {
									case 2 : // existe el primero
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Canciones <= NuevaAnomalia2->Canciones) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia2);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia2);
										return true;
										break;
									case 4 : // Esixte el segundo
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Canciones <= NuevaAnomalia1->Canciones) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia1);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia1);
										return true;
										break;
									case 6 : // Existen los dos...
										return true;
									default : // No existe ninguno
										return false;
								}
							};

 inline const unsigned int	TotalAnomalias(void) {
								return _Anomalias.size();
							};

 inline const TGrupo	   *Anomalia(const unsigned int Pos) {
								return _Anomalias[Pos];
							};

 inline const unsigned int  Eleccion(void) {
								return _Eleccion;
							};

 inline void				Eleccion(const unsigned int nEleccion) {
								if (nEleccion < _Anomalias.size()) {
									_Eleccion = nEleccion;
								}
							};

 inline void				EliminarEntrada(const unsigned int Pos) {
								_Anomalias.erase(_Anomalias.begin() + Pos);
							};

 inline void                CambiarNombreFinal(const TCHAR *nTxt) {
								_NombreFinal = nTxt;
							};

 inline const TCHAR        *ObtenerNombreFinal(void) {
								if (_NombreFinal.Tam() == 0)	return _Anomalias[_Eleccion]->Nombre->Nombre();
								else							return _NombreFinal();
							};

 inline StrTp              *AplicarNombreFinal(void) {
								if (_NombreFinal.Tam() > 0) {
									_Anomalias[_Eleccion]->Nombre->Nombre = _NombreFinal;
									_NombreFinal = TEXT("");
								}
								return _Anomalias[_Eleccion]->Nombre;
							}

private :
 DWL::DWLString            _NombreFinal;

 std::vector<TGrupo *>	   _Anomalias;

 unsigned int			   _Eleccion;
};





class AnomaliaDsk {
public :
							AnomaliaDsk(void) { 
								_Eleccion = 0;
								_DiscoMultiple = false;
							};

							AnomaliaDsk(TDisco *Anomalia1, TDisco *Anomalia2, const bool DiscoMultiple) { 
								_Eleccion = 0;
								_DiscoMultiple = DiscoMultiple;
								if (Anomalia2->Canciones > Anomalia1->Canciones) {
									_Anomalias.push_back(Anomalia2);
									_Anomalias.push_back(Anomalia1);
								}
								else {
									_Anomalias.push_back(Anomalia1);
									_Anomalias.push_back(Anomalia2);
								}
							};

					       ~AnomaliaDsk(void) { 
							};

 const bool					MirarAnomalia(TDisco *NuevaAnomalia1, TDisco *NuevaAnomalia2, const bool DiscoMultiple) {
								unsigned int	i = 0;
								unsigned int	z = 0;
								BYTE			Existe = 0;
								unsigned int    Tam = _Anomalias.size();
								for (i = 0; i < Tam; i++) {
									if (NuevaAnomalia1->Nombre == _Anomalias[i]->Nombre) Existe += 2;
									if (NuevaAnomalia2->Nombre == _Anomalias[i]->Nombre) Existe += 4;
								}
								switch (Existe) {
									case 2 : // existe el primero
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Canciones <= NuevaAnomalia2->Canciones) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia2);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia2);
										_DiscoMultiple = DiscoMultiple;
										return true;
										break;
									case 4 : // Esixte el segundo
										for (z = 0; z < Tam; z++) {
											if (_Anomalias[z]->Canciones <= NuevaAnomalia1->Canciones) {
												_Anomalias.insert(_Anomalias.begin() +z, NuevaAnomalia1);
												return true;
											}
										}
										_Anomalias.push_back(NuevaAnomalia1);
										_DiscoMultiple = DiscoMultiple;
										return true;
										break;
									case 6 : // Existen los dos...
										_DiscoMultiple = DiscoMultiple;
										return true;
									default : // No existe ninguno
										return false;
								}
							};

 inline const unsigned int	TotalAnomalias(void) {
								return _Anomalias.size();
							};

 inline const TDisco	   *Anomalia(const unsigned int Pos) {
								return _Anomalias[Pos];
							};

 inline const unsigned int  Eleccion(void) {
								return _Eleccion;
							};

 inline void				Eleccion(const unsigned int nEleccion) {
								if (nEleccion < _Anomalias.size()) {
									_Eleccion = nEleccion;
								}
							};

 inline void				EliminarEntrada(const unsigned int Pos) {
								_Anomalias.erase(_Anomalias.begin() + Pos);
							};

 inline const bool          DiscoMultiple(void) {
								return _DiscoMultiple;
							};

 inline void                CambiarNombreFinal(const TCHAR *nTxt) {
								_NombreFinal = nTxt;
							};

 inline const TCHAR        *ObtenerNombreFinal(void) {
								if (_NombreFinal.Tam() == 0)	return _Anomalias[_Eleccion]->Nombre->Nombre();
								else							return _NombreFinal();
							};

 inline StrTp              *AplicarNombreFinal(void) {
								if (_NombreFinal.Tam() > 0) {
									_Anomalias[_Eleccion]->Nombre->Nombre = _NombreFinal;
									_NombreFinal = TEXT("");
								}
								return _Anomalias[_Eleccion]->Nombre;
							}
private :
 DWL::DWLString            _NombreFinal;

 std::vector<TDisco *>	   _Anomalias;

 unsigned int			   _Eleccion;

 bool					   _DiscoMultiple;
};
