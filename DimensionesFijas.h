#pragma once

#ifndef DIMENSIONES_FIJAS
	#define DIMENSIONES_FIJAS
	
	// Separacion entre controles horizontales
	#define SEPARACION_CONTROLES_X	 5
	#define	VENTANAEX_MARGEN		10

	// VentanaLog
	#define VENTANALOG_ALTURA_PARTE_SUPERIOR	29
	#define VENTANALOG_ALTURA_PARTE_INFERIOR	15

	// VentanaEx
	#define VENTANAEX_ALTURA_PARTE_SUPERIOR		63
	#define VENTANAEX_ALTURA_PARTE_INFERIOR		15

	#define VENTANAEX_BORDE						4
	#define VENTANAEX_BORDECLARO				2

	// Tamaño minimo de la ventana principal del reproductor
	#define MINIMO_PLAYER_ANCHO			 600
	#define MINIMO_PLAYER_ALTO			 390

	// Oscy
	#define X_OSCY		((X_BOTON_SHUFLE_REPEAT + (ANCHO_BOTON_SHUFLE_REPEAT * 2)) + (SEPARACION_CONTROLES_X * 2))
	#define	ALTO_OSCY	26
	#define	ANCHO_OSCY	160


	// Tiempo Actual
	#define X_TIEMPO_ACTUAL		((X_OSCY + ANCHO_OSCY) + SEPARACION_CONTROLES_X)
	#define ANCHO_TIEMPO_ACTUAL	120
	#define ALTO_TIEMPO_ACTUAL	14

	// Volumen
	#define ANCHO_VOLUMEN	100
	#define ALTO_VOLUMEN	10
	#define X_VOLUMEN		X_TIEMPO_ACTUAL + ((ANCHO_TIEMPO_ACTUAL - ANCHO_VOLUMEN) / 2)

	// Opciones Basicas
//	#define ANCHO_OPCIONES_BASICAS		140

	// Control Cerrar Maximizar Minimizar
	#define CM_ANCHO_ICO	16
	#define CM_BORDE		2
	#define CM_ANCHO		((CM_ANCHO_ICO * 3) + (CM_BORDE * 4))	// Ancho para el control en modo Minimizar Maximizar Cerrar
	#define CM_ANCHO_SC		((CM_ANCHO_ICO) + (CM_BORDE * 4))		// Ancho para el control en modo Solo Cerrar
	#define CM_ALTO			23

	// Controles en modo pantalla completa
	#define CONTROLESMP_ANCHO 400
	#define CONTROLESMP_ALTO   50

	// Boton Shufle/Repeat
	#define X_BOTON_SHUFLE_REPEAT		((X_BOTONERA + ((ANCHO_BOTON_BOTONERA + 1) * 5)) + SEPARACION_CONTROLES_X)
	#define ANCHO_BOTON_SHUFLE_REPEAT	50
	#define ALTO_BOTON_SHUFLE_REPEAT	24
	
	// Botones para el Control Presentacion
	#define ALTO_BOTON_CP           18
	// Botonera (< play pausa stop >)
	#define X_BOTONERA				VENTANAEX_MARGEN
	#define ANCHO_BOTON_BOTONERA	24
	#define ALTO_BOTONERA			24
	// Margen lateral para la escena del ControlPresentacionEx
	#define MARGEN_ESCENA		3
	// Separacion entre objetos de la escena
	#define SEPARACION_ESCENA	0

#endif