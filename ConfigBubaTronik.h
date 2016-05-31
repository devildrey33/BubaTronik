#pragma once

#ifndef CONFIG_BUBATRONIK
	#define CONFIG_BUBATRONIK
	// Archivo de configuraciones pre compilacion de BubaTronik

	// Macros de compilacion en modo debug
	#if defined _DEBUG
		// Esta macro se utiliza para ejecutar mas de una instancia del player, el objetivo es poder poner musica en un player mientras depuro otro.
		// Solo debe usarse en modo debug, ya que cada medio se abrira un reproductor para el, si lo ejecutamos desde el explorador
		#define IGNORAR_INSTANCIA_PREVIA

		// Si esta definida y el modo debug esta activo la variable que contiene el path de la aplicacion 
		// quitara el ultimo directorio deberia ser el \Debug\, y usara el directorio principal
		// NOTA activando esta opcion hay que tener en cuenta que el ejecutable debera estar un directorio por encima de la raíz
		#define SIMULAR_PATH_DEBUG				

		// Macro que se utiliza para que el player inicie con 2 argumentos predefinidos y los cargue
		//#define SIMULACION_ARGS
	#endif



	// Si esta activa el reproductor utilizara el FMOD para el audio, si no se utilizara el VLC
	#define UTILIZAR_FMOD

	// Si esta definida se mostrara un acceso para poder abrir la ventana de log
//	#define MOSTRAR_LOG

	// Si esta definida mostrara la nueva ventana que se esta diseñando para BubaTronik 0.70
//	#define MOSTRAR_VENTANAPRINCIPALEX

	// Version de la libvlc a compilar '98' para la 0.98a, '99' para la 0.9.9 y '100' para la 1.0
	//#define LIBVLC_NUM_VERSION	98				
	//#define LIBVLC_NUM_VERSION	99			
	#define LIBVLC_NUM_VERSION	115	
	#define LIBVLC_STR_VERSION TEXT("1.1.5")

	// Nombre del reproductor
	#define APP_NOMBRE				TEXT("BubaTronik")  
	#define APP_NUM_VER				0.61f
	#define APP_FECHA_COMPILACION	TEXT("26/11/2010")
//	#define APP_REVISION			__COUNTER__
//	#define APP_FECHA_COMPILACION	__TIMESTAMP__

	#define APP_IDIOMA_VER 0.5f

	#if (LIBVLC_NUM_VERSION == 120)
		// Path de las librerias
		#define PATHVLC			TEXT("LibVLC 1.2.0\\libvlc.dll")
		#define PATHVLC_CORE	TEXT("LibVLC 1.2.0\\libvlccore.dll")
		// Path de los plugins
	//	#define PATHVLC_PLUGINS "--plugin-path=.\\LibVLC 1.0\\plugins"
		#define PATHVLC_PLUGINS TEXT("LibVLC 1.2.0\\plugins")
		// Version actual del reproductor
		#if defined _DEBUG
			#define APP_STR_VERSION TEXT("0.61 (vlc 1.2.0) Debug")		
		#else
			#define APP_STR_VERSION TEXT("0.61 (vlc 1.2.0)")		
		#endif
	#endif

	#if (LIBVLC_NUM_VERSION == 115)
		// Path de las librerias
		#define PATHVLC			TEXT("libvlc.dll")
		#define PATHVLC_CORE	TEXT("libvlccore.dll")
		// Path de los plugins
		#define PATHVLC_PLUGINS TEXT("Plugins\\VLC")
		// Version actual del reproductor
		#if defined _DEBUG
			#define APP_STR_VERSION TEXT("0.61 (vlc 1.1.5) Debug")		
		#else
			#define APP_STR_VERSION TEXT("0.61 (vlc 1.1.5)")		
		#endif
	#endif

	#if (LIBVLC_NUM_VERSION == 114)
		// Path de las librerias
		#define PATHVLC			TEXT("libvlc.dll")
		#define PATHVLC_CORE	TEXT("libvlccore.dll")
		// Path de los plugins
		#define PATHVLC_PLUGINS TEXT("Plugins\\VLC")
		// Version actual del reproductor
		#if defined _DEBUG
			#define APP_STR_VERSION TEXT("0.61 (vlc 1.1.4) Debug")		
		#else
			#define APP_STR_VERSION TEXT("0.61 (vlc 1.1.4)")		
		#endif
	#endif

	// MEJOR ELECCION
	// Tiene problemas al usar SetFocus en la ventana del VLC (cuando se usa el stop, causa un deadlock total de la aplicacion)
	// Solucion DESACTIVAR VENTANA DEL VLC con EnableWindow
	#if (LIBVLC_NUM_VERSION == 105)
		// Path de las librerias
		#define PATHVLC			TEXT("LibVLC 1.0.5\\libvlc.dll")
		#define PATHVLC_CORE	TEXT("LibVLC 1.0.5\\libvlccore.dll")
		// Path de los plugins
	//	#define PATHVLC_PLUGINS "--plugin-path=.\\LibVLC 1.0\\plugins"
		#define PATHVLC_PLUGINS TEXT("LibVLC 1.0.5\\plugins")
		// Version actual del reproductor
		#if defined _DEBUG
			#define APP_STR_VERSION TEXT("0.60 (vlc 1.0.5) Debug")		
		#else
			#define APP_STR_VERSION TEXT("0.60 (vlc 1.0.5)")		
		#endif
	#endif


	/*
	// Se queda pillada al final de un mp4 al usar stop (solo cuando esta en pantalla completa)
	// Tiene problemas al usar SetFocus en la ventana del VLC (cuando se usa el stop, causa un deadlock total de la aplicacion)
	// DESCARTADA
	#if (LIBVLC_NUM_VERSION == 99)
		// Path de la libreria
		#define PATHVLC			TEXT("LibVLC 0.99\\libvlc.dll")
		#define PATHVLCORE		TEXT("LibVLC 0.99\\libvlccore.dll")
		// Path de los plugins
	//	#define PATHVLC_PLUGINS "--plugin-path=.\\LibVLC 0.98a\\plugins"
		#define PATHVLC_PLUGINS TEXT("LibVLC 0.99\\plugins")
		// Version actual del reproductor
		#if defined _DEBUG
			#define APP_STR_VERSION TEXT("0.51c (vlc 0.99) Debug")		
		#else
			#define APP_STR_VERSION TEXT("0.51c (vlc 0.99)")		
		#endif
	#endif


	// Se queda pillada al final de un mp4 al usar stop (solo cuando esta en pantalla completa)
	// Tiene problemas al usar SetFocus en la ventana del VLC (cuando se usa el stop, causa un deadlock total de la aplicacion)
	// El modo pantalla completa solo se ejecuta en el monitor principal
	// DESCARTADA
	#if (LIBVLC_NUM_VERSION == 98)
		// Path de la libreria
		#define PATHVLC			TEXT("LibVLC 0.98a\\libvlc.dll")
		#define PATHVLCORE		TEXT("LibVLC 0.98a\\libvlccore.dll")
		// Path de los plugins
	//	#define PATHVLC_PLUGINS "--plugin-path=.\\LibVLC 0.98a\\plugins"
		#define PATHVLC_PLUGINS TEXT("LibVLC 0.98a\\plugins")
		// Version actual del reproductor
		#if defined _DEBUG
			#define APP_STR_VERSION TEXT("0.51c (vlc 0.98a) Debug")		
		#else
			#define APP_STR_VERSION TEXT("0.51c (vlc 0.98a)")		
		#endif
	#endif

	*/

#endif