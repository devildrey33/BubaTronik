#pragma once
#include "VentanaMensaje.h"
#include <DWLBarraEx.h>

// Clase para mostrar una ventana que descargara la actualizacion
class VentanaMensaje_Actualizacion : public VentanaMensaje_Base {
 public:
                                VentanaMensaje_Actualizacion(void);
                               ~VentanaMensaje_Actualizacion(void);
  void                          Mostrar(void);
  void                          Novedades(const char *nNovedades);
  void							ProbarInstalacionActualizacion(void);
 private:
  void                          DescargarActualizacion(void);
  LRESULT						Evento_Cerrar(void);
  LRESULT                       Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
  DWL::ControlesEx::DWLBotonEx	Boton_Aceptar;
  DWL::ControlesEx::DWLBarraEx  ProgresoDescarga;
  LRESULT CALLBACK              GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
  DWL::DWLString                TextoNovedades;
  bool                          Descargado;
};