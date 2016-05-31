#pragma once
#include "ConfigBubaTronik.h"
#include <vector>

extern "C" {

	#if (LIBVLC_VERSION == 120)
		#include "libvlc_structures.h"
		#include "libvlc_events.h"
//		#include "mediacontrol_structures.h"
	#endif


	// VERSION 0.98a
	#if (LIBVLC_VERSION == 98)
		typedef __int64 int64_t;

		// libvlc_exception_raised
		typedef struct libvlc_exception_t {
			int		b_raised;
			int		i_code;
			char   *psz_message;
		} libvlc_exception_t;

/*		typedef enum libvlc_state_t { // doxigen 0.98a
			libvlc_NothingSpecial = 0,
			libvlc_Opening, 
			libvlc_Buffering, 
			libvlc_Playing,
			libvlc_Paused, 
			libvlc_Stopped, 
			libvlc_Ended, 
			libvlc_Error 
		};*/

		typedef enum libvlc_state_t { // source code 0.98a
			libvlc_NothingSpecial=0,
			libvlc_Opening,
			libvlc_Buffering,
			libvlc_Playing,
			libvlc_Paused,
			libvlc_Stopped,
			libvlc_Forward, 
			libvlc_Backward,
			libvlc_Ended,
			libvlc_Error  
		};

		typedef struct	libvlc_instance_t		libvlc_instance_t;
		typedef struct	libvlc_media_player_t	libvlc_media_player_t;
		typedef struct	libvlc_media_t			libvlc_media_t;

		/** This structure is opaque. It represents a libvlc log instance */
		typedef struct libvlc_log_t libvlc_log_t;

		/** This structure is opaque. It represents a libvlc log iterator */
		typedef struct libvlc_log_iterator_t libvlc_log_iterator_t;

		typedef struct libvlc_log_message_t
		{
			unsigned    sizeof_msg;   /* sizeof() of message structure, must be filled in by user */
			int         i_severity;   /* 0=INFO, 1=ERR, 2=WARN, 3=DBG */
			const char *psz_type;     /* module type */
			const char *psz_name;     /* module name */
			const char *psz_header;   /* optional header */
			const char *psz_message;  /* message */
		} libvlc_log_message_t;

	// FIN Version 0.98a
	#endif

	typedef int		libvlc_drawable_t;
	typedef struct vlc_object_t vlc_object_t;

	#define IMPORT  _declspec(dllimport)

	typedef int64_t libvlc_time_t;
	// Log
	typedef IMPORT unsigned						(*LIBVLC_LOG_COUNT)						(const libvlc_log_t *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_LOG_CLEAR)						(libvlc_log_t *, libvlc_exception_t *);
	typedef IMPORT libvlc_log_iterator_t	   *(*LIBVLC_LOG_GET_ITERATOR)				(const libvlc_log_t *, libvlc_exception_t *);
	typedef IMPORT void						    (*LIBVLC_LOG_ITERATOR_FREE)				(libvlc_log_iterator_t *, libvlc_exception_t *);
	typedef IMPORT void						    (*LIBVLC_LOG_CLOSE)						(libvlc_log_t *, libvlc_exception_t *);
	typedef IMPORT libvlc_log_t				   *(*LIBVLC_LOG_OPEN)						(libvlc_instance_t *, libvlc_exception_t *);
	typedef IMPORT int							(*LIBVLC_LOG_ITERATOR_HAS_NEXT)			(const libvlc_log_iterator_t *, libvlc_exception_t *);
	typedef IMPORT libvlc_log_message_t		   *(*LIBVLC_LOG_ITERATOR_NEXT)				(libvlc_log_iterator_t *, libvlc_log_message_t *, libvlc_exception_t *);
	// Excepciones
	typedef IMPORT int							(*LIBVLC_EXCEPTION_RAISED)				(const libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_EXCEPTION_INIT)				(libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_EXCEPTION_CLEAR)				(libvlc_exception_t *);
	typedef IMPORT const char 				   *(*LIBVLC_EXCEPTION_GET_MESSAGE)			(const libvlc_exception_t *);
	// Base
	typedef IMPORT const char				   *(*LIBVLC_GET_VERSION)					(void);
	typedef IMPORT vlc_object_t				   *(*LIBVLC_GET_VLC_INSTANCE)				(libvlc_instance_t *);
	typedef IMPORT libvlc_instance_t		   *(*LIBVLC_NEW)							(int, const char *const *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_RELEASE)						(libvlc_instance_t *);
	typedef IMPORT int						    (*LIBVLC_GET_FULLSCREEN)				(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_SET_FULLSCREEN)				(libvlc_media_player_t *, int, libvlc_exception_t *);
	// Audio
	typedef IMPORT int							(*LIBVLC_AUDIO_GET_VOLUME)			  	(libvlc_instance_t *, libvlc_exception_t *); 	
	typedef IMPORT void							(*LIBVLC_AUDIO_SET_VOLUME)			  	(libvlc_instance_t *, int, libvlc_exception_t *); 	
	// Media
	typedef IMPORT libvlc_media_t		       *(*LIBVLC_MEDIA_NEW)						(libvlc_instance_t *, const char *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_MEDIA_ADD_OPTION)				(libvlc_media_t *, const char *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_MEDIA_RELEASE)					(libvlc_media_t *);
	typedef IMPORT libvlc_time_t		        (*LIBVLC_MEDIA_GET_DURATION)			(libvlc_media_t *, libvlc_exception_t *);
	// Media Player
	#if (LIBVLC_VERSION < 100)
	    typedef IMPORT void						(*LIBVLC_MEDIA_PLAYER_SET_DRAWABLE)		(libvlc_media_player_t *, libvlc_drawable_t, libvlc_exception_t *);
	#endif
	typedef IMPORT void					     	(*LIBVLC_MEDIA_PLAYER_SET_HWND)			(libvlc_media_player_t *, void *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_MEDIA_PLAYER_RELEASE)			(libvlc_media_player_t *);
	typedef IMPORT void							(*LIBVLC_MEDIA_PLAYER_STOP)				(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_MEDIA_PLAYER_PLAY)				(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_MEDIA_PLAYER_PAUSE)			(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT libvlc_media_player_t	   *(*LIBVLC_MEDIA_PLAYER_NEW)				(libvlc_instance_t *, libvlc_exception_t *);
	typedef IMPORT libvlc_media_player_t	   *(*LIBVLC_MEDIA_PLAYER_NEW_FROM_MEDIA)	(libvlc_media_t *, libvlc_exception_t *);
	typedef IMPORT libvlc_time_t			    (*LIBVLC_MEDIA_PLAYER_GET_LENGTH)		(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT libvlc_time_t			    (*LIBVLC_MEDIA_PLAYER_GET_TIME)			(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT float					    (*LIBVLC_MEDIA_PLAYER_GET_POSITION)		(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT void							(*LIBVLC_MEDIA_PLAYER_SET_POSITION)		(libvlc_media_player_t *, float, libvlc_exception_t *);
	typedef IMPORT float					    (*LIBVLC_MEDIA_PLAYER_GET_RATE)			(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT void						    (*LIBVLC_MEDIA_PLAYER_SET_RATE)			(libvlc_media_player_t *, float, libvlc_exception_t *);
	typedef IMPORT libvlc_state_t				(*LIBVLC_MEDIA_PLAYER_GET_STATE)		(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT int							(*LIBVLC_MEDIA_PLAYER_HAS_VOUT)			(libvlc_media_player_t *, libvlc_exception_t *);

	typedef IMPORT char						   *(*LIBVLC_VIDEO_GET_ASPECT_RATIO)		(libvlc_media_player_t *, libvlc_exception_t *);
	typedef IMPORT int						    (*LIBVLC_VIDEO_SET_SUBTITLE_FILE)		(libvlc_media_player_t *, const char *, libvlc_exception_t *);
	typedef IMPORT char						   *(*LIBVLC_MEDIA_GET_META)				(libvlc_media_t *, libvlc_meta_t, libvlc_exception_t *);

	// VLC Core
	typedef IMPORT unsigned						(*LIBVLC_CORE_VLC_OBJECT_RELEASE)		(vlc_object_t *);

	// VLC_PUBLIC_API void libvlc_media_player_set_hwnd ( libvlc_media_player_t *p_mi, void *drawable, libvlc_exception_t *p_e );


	//void libvlc_set_fullscreen  	(  	libvlc_media_player_t *, int, libvlc_exception_t *) 	

	// PARTE DEL MEDIA CONTROL POR COMPLETAR (NO FUNCIONA)
/*	typedef enum  {
		mediacontrol_AbsolutePosition,
		mediacontrol_RelativePosition,
		mediacontrol_ModuloPosition
	} mediacontrol_PositionOrigin;

	typedef enum {
		mediacontrol_ByteCount,
		mediacontrol_SampleCount,
		mediacontrol_MediaTime
	} mediacontrol_PositionKey;


	struct mediacontrol_Instance {
		 libvlc_instance_t		*p_instance;
		 libvlc_media_player_t	*p_media_player;
	};

	struct mediacontrol_Position {
		mediacontrol_PositionOrigin 	origin;
		mediacontrol_PositionKey 		key;
	//	__int64  						value;
		int64_t							value;
	};

	typedef struct {
		int		code;
		char   *message;
	} mediacontrol_Exception;*/


//	typedef mediacontrol_Instance  *(*_mediacontrol_new_from_instance)		(libvlc_instance_t *, mediacontrol_Exception *);
//	typedef void					(*_mediacontrol_display_text)			(mediacontrol_Instance *, const char *,	const mediacontrol_Position *,	const mediacontrol_Position *, mediacontrol_Exception *);
//	typedef mediacontrol_Exception *(*_mediacontrol_exception_create)		(void);
	// FIN MEDIA CONTROL

} // FIN EXTERN "C"

class VLC_BASE {
public:
										VLC_BASE(void) { 
											LibVLC = NULL;
											LibVLC_Core = NULL;
										};

									   ~VLC_BASE(void) { 
										   DescargarLIBVLC_DLL(); 
									    };

 virtual const bool						funcionFalla(const TCHAR *Txt) { 
											return false; 
										};
										// Retorna true si se ha cargado, false en caso contrario.
 const bool								CargarLIBVLC_DLL(const TCHAR *PathDll, const TCHAR *PathDll_Core) {
											LibVLC = NULL;
											LibVLC = LoadLibraryEx(PathDll, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);  
//											LibVLC_Core = LoadLibraryEx(PathDll_Core, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);  
											if (LibVLC == NULL) {
												/*DWL::DWLString ErrTxt;
												ErrTxt.sprintf(TEXT("Error cargando (%s)\nEs muy probable que uno o varios archivos de esta libreria no existan o esten dañados.\nEste error es grave, y el reproductor debe terminar la ejecucion.\nReinstala el reproductor para solucionar el problema"), PathDll);

												*/
												return false;
											}
											SOLO_DEBUG(DWORD D = GetLastError());
										//	SOLO_DEBUG(App.Log.GuardarLog(TEXT("Cargando funciones.."), PathDll));


											if ((_libvlc_get_vlc_instance				= (LIBVLC_GET_VLC_INSTANCE)					GetProcAddress(LibVLC, "libvlc_get_vlc_instance")) == NULL)
												return funcionFalla(TEXT("libvlc_get_vlc_instance"));
											if ((_libvlc_get_version					= (LIBVLC_GET_VERSION)						GetProcAddress(LibVLC, "libvlc_get_version")) == NULL)
												return funcionFalla(TEXT("libvlc_get_version"));
											if ((_libvlc_exception_raised				= (LIBVLC_EXCEPTION_RAISED)					GetProcAddress(LibVLC, "libvlc_exception_raised")) == NULL)
												return funcionFalla(TEXT("libvlc_exception_raised"));
											if ((_libvlc_exception_clear				= (LIBVLC_EXCEPTION_CLEAR)					GetProcAddress(LibVLC, "libvlc_exception_clear")) == NULL)
												return funcionFalla(TEXT("libvlc_exception_clear"));
											if ((_libvlc_exception_init					= (LIBVLC_EXCEPTION_INIT)					GetProcAddress(LibVLC, "libvlc_exception_init")) == NULL)
												return funcionFalla(TEXT("libvlc_exception_init"));
											if ((_libvlc_exception_get_message			= (LIBVLC_EXCEPTION_GET_MESSAGE)			GetProcAddress(LibVLC, "libvlc_exception_get_message")) == NULL)
												return funcionFalla(TEXT("libvlc_exception_get_message"));
											if ((_libvlc_new							= (LIBVLC_NEW)								GetProcAddress(LibVLC, "libvlc_new")) == NULL)
												return funcionFalla(TEXT("libvlc_new"));
											if ((_libvlc_release						= (LIBVLC_RELEASE)							GetProcAddress(LibVLC, "libvlc_release")) == NULL)
												return funcionFalla(TEXT("libvlc_release"));
											if ((_libvlc_get_fullscreen					= (LIBVLC_GET_FULLSCREEN)					GetProcAddress(LibVLC, "libvlc_get_fullscreen")) == NULL)
												return funcionFalla(TEXT("libvlc_get_fullscreen"));
											if ((_libvlc_set_fullscreen					= (LIBVLC_SET_FULLSCREEN)					GetProcAddress(LibVLC, "libvlc_set_fullscreen")) == NULL)
												return funcionFalla(TEXT("libvlc_set_fullscreen"));
											if ((_libvlc_audio_get_volume				= (LIBVLC_AUDIO_GET_VOLUME)					GetProcAddress(LibVLC, "libvlc_audio_get_volume")) == NULL)
												return funcionFalla(TEXT("libvlc_audio_get_volume"));
											if ((_libvlc_audio_set_volume				= (LIBVLC_AUDIO_SET_VOLUME)					GetProcAddress(LibVLC, "libvlc_audio_set_volume")) == NULL)
												return funcionFalla(TEXT("libvlc_audio_set_volume"));

											if ((_libvlc_log_count						= (LIBVLC_LOG_COUNT)						GetProcAddress(LibVLC, "libvlc_log_count")) == NULL)
												return funcionFalla(TEXT("libvlc_log_count"));
											if ((_libvlc_log_clear						= (LIBVLC_LOG_CLEAR)						GetProcAddress(LibVLC, "libvlc_log_clear")) == NULL)
												return funcionFalla(TEXT("libvlc_log_clear"));
											if ((_libvlc_log_get_iterator				= (LIBVLC_LOG_GET_ITERATOR)					GetProcAddress(LibVLC, "libvlc_log_get_iterator")) == NULL)
												return funcionFalla(TEXT("libvlc_log_get_iterator"));
											if ((_libvlc_log_iterator_free				= (LIBVLC_LOG_ITERATOR_FREE)				GetProcAddress(LibVLC, "libvlc_log_iterator_free")) == NULL)
												return funcionFalla(TEXT("libvlc_log_iterator_free"));
											if ((_libvlc_log_close						= (LIBVLC_LOG_CLOSE)						GetProcAddress(LibVLC, "libvlc_log_close")) == NULL)
												return funcionFalla(TEXT("libvlc_log_close"));
											if ((_libvlc_log_open						= (LIBVLC_LOG_OPEN)							GetProcAddress(LibVLC, "libvlc_log_open")) == NULL)
												return funcionFalla(TEXT("libvlc_log_open"));
											if ((_libvlc_log_iterator_has_next			= (LIBVLC_LOG_ITERATOR_HAS_NEXT)			GetProcAddress(LibVLC, "libvlc_log_iterator_has_next")) == NULL)
												return funcionFalla(TEXT("libvlc_log_iterator_has_next"));
											if ((_libvlc_log_iterator_next				= (LIBVLC_LOG_ITERATOR_NEXT)				GetProcAddress(LibVLC, "libvlc_log_iterator_next")) == NULL)
												return funcionFalla(TEXT("libvlc_log_iterator_next"));

											if ((_libvlc_media_new						= (LIBVLC_MEDIA_NEW)						GetProcAddress(LibVLC, "libvlc_media_new")) == NULL)
												return funcionFalla(TEXT("libvlc_media_new"));
											if ((_libvlc_media_add_option				= (LIBVLC_MEDIA_ADD_OPTION)					GetProcAddress(LibVLC, "libvlc_media_add_option")) == NULL)
												return funcionFalla(TEXT("libvlc_media_add_option"));
											if ((_libvlc_media_release					= (LIBVLC_MEDIA_RELEASE)					GetProcAddress(LibVLC, "libvlc_media_release")) == NULL)
												return funcionFalla(TEXT("libvlc_media_release"));
											if ((_libvlc_media_get_duration				= (LIBVLC_MEDIA_GET_DURATION)				GetProcAddress(LibVLC, "libvlc_media_get_duration")) == NULL)
												return funcionFalla(TEXT("libvlc_media_get_duration"));
											
											if ((_libvlc_media_player_set_hwnd			= (LIBVLC_MEDIA_PLAYER_SET_HWND)			GetProcAddress(LibVLC, "libvlc_media_player_set_hwnd")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_set_hwnd"));

											#if (LIBVLC_VERSION < 100)
											    if ((_libvlc_media_player_set_drawable		= (LIBVLC_MEDIA_PLAYER_SET_DRAWABLE)		GetProcAddress(LibVLC, "libvlc_media_player_set_drawable")) == NULL)
												    return funcionFalla(TEXT("libvlc_media_player_set_drawable"));
											#endif
											if ((_libvlc_media_player_play				= (LIBVLC_MEDIA_PLAYER_PLAY)				GetProcAddress(LibVLC, "libvlc_media_player_play")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_play"));
											if ((_libvlc_media_player_pause				= (LIBVLC_MEDIA_PLAYER_PAUSE)				GetProcAddress(LibVLC, "libvlc_media_player_pause")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_pause"));	
											if ((_libvlc_media_player_stop				= (LIBVLC_MEDIA_PLAYER_STOP)				GetProcAddress(LibVLC, "libvlc_media_player_stop")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_stop"));
											if ((_libvlc_media_player_new				= (LIBVLC_MEDIA_PLAYER_NEW)					GetProcAddress(LibVLC, "libvlc_media_player_new")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_new"));
											if ((_libvlc_media_player_new_from_media	= (LIBVLC_MEDIA_PLAYER_NEW_FROM_MEDIA)		GetProcAddress(LibVLC, "libvlc_media_player_new_from_media")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_new_from_media"));
											if ((_libvlc_media_player_release			= (LIBVLC_MEDIA_PLAYER_RELEASE)				GetProcAddress(LibVLC, "libvlc_media_player_release")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_release"));
											if ((_libvlc_media_player_get_length		= (LIBVLC_MEDIA_PLAYER_GET_LENGTH)			GetProcAddress(LibVLC, "libvlc_media_player_get_length")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_get_length"));
											if ((_libvlc_media_player_get_time		= (LIBVLC_MEDIA_PLAYER_GET_TIME)				GetProcAddress(LibVLC, "libvlc_media_player_get_time")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_get_time"));
											if ((_libvlc_media_player_get_position		= (LIBVLC_MEDIA_PLAYER_GET_POSITION)		GetProcAddress(LibVLC, "libvlc_media_player_get_position")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_get_position"));
											if ((_libvlc_media_player_set_position		= (LIBVLC_MEDIA_PLAYER_SET_POSITION)		GetProcAddress(LibVLC, "libvlc_media_player_set_position")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_set_position"));
											if ((_libvlc_media_player_get_rate			= (LIBVLC_MEDIA_PLAYER_GET_RATE)			GetProcAddress(LibVLC, "libvlc_media_player_get_rate")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_get_rate"));
											if ((_libvlc_media_player_set_rate			= (LIBVLC_MEDIA_PLAYER_SET_RATE)			GetProcAddress(LibVLC, "libvlc_media_player_set_rate")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_set_rate"));
											if ((_libvlc_media_player_get_state			= (LIBVLC_MEDIA_PLAYER_GET_STATE)			GetProcAddress(LibVLC, "libvlc_media_player_get_state")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_get_state"));
											if ((_libvlc_media_player_has_vout			= (LIBVLC_MEDIA_PLAYER_HAS_VOUT)			GetProcAddress(LibVLC, "libvlc_media_player_has_vout")) == NULL)
												return funcionFalla(TEXT("libvlc_media_player_has_vout"));

											if ((_libvlc_video_get_aspect_ratio			= (LIBVLC_VIDEO_GET_ASPECT_RATIO)			GetProcAddress(LibVLC, "libvlc_video_get_aspect_ratio")) == NULL)
												return funcionFalla(TEXT("libvlc_video_get_aspect_ratio"));
											if ((_libvlc_video_set_subtitle_file		= (LIBVLC_VIDEO_SET_SUBTITLE_FILE)			GetProcAddress(LibVLC, "libvlc_video_set_subtitle_file")) == NULL)
												return funcionFalla(TEXT("libvlc_video_get_aspect_ratio"));
											if ((_libvlc_media_get_meta					= (LIBVLC_MEDIA_GET_META)					GetProcAddress(LibVLC, "libvlc_media_get_meta")) == NULL)
												return funcionFalla(TEXT("libvlc_media_get_meta"));

											// LIBVLC Core
											//if ((_libvlc_core_vlc_object_release		= (LIBVLC_CORE_VLC_OBJECT_RELEASE)			GetProcAddress(LibVLC_Core, "__vlc_object_release")) == NULL)
											//	return funcionFalla(TEXT("libvlc_core_vlc_object_release"));


											return true;
										};

 void									DescargarLIBVLC_DLL(void) {
											if (LibVLC_Core != NULL) FreeLibrary(LibVLC_Core);
											LibVLC_Core = NULL;
											if (LibVLC != NULL) FreeLibrary(LibVLC);
											LibVLC = NULL;
										};

 ///////////////////////////////////////// Volumen
 inline void							libvlc_audio_set_volume(libvlc_instance_t *Instancia_LibVLC, int nVolumen, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_audio_set_volume)(Instancia_LibVLC, nVolumen, Excepciones_LibVLC);
										};

 inline int								libvlc_audio_get_volume(libvlc_instance_t *Instancia_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_audio_get_volume)(Instancia_LibVLC, Excepciones_LibVLC);
										};

 ///////////////////////////////////////// Log
 unsigned		 						libvlc_log_count(const libvlc_log_t *Log_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_log_count)(Log_LibVLC, Excepciones_LibVLC);
										};

 inline void						    libvlc_log_clear(libvlc_log_t *Log_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_log_clear)(Log_LibVLC, Excepciones_LibVLC);
										};

 inline libvlc_log_iterator_t		   *libvlc_log_get_iterator(const libvlc_log_t *Log_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_log_get_iterator)(Log_LibVLC, Excepciones_LibVLC);
										};

 inline void						    libvlc_log_iterator_free(libvlc_log_iterator_t *LogIterator_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_log_iterator_free)(LogIterator_LibVLC, Excepciones_LibVLC);
										};

 inline void						    libvlc_log_close(libvlc_log_t *Log_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_log_close)(Log_LibVLC, Excepciones_LibVLC);
										};

 inline libvlc_log_t				   *libvlc_log_open(libvlc_instance_t *Instancia_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_log_open)(Instancia_LibVLC, Excepciones_LibVLC);
										};

 inline int							    libvlc_log_iterator_has_next(const libvlc_log_iterator_t *LogIterator_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_log_iterator_has_next)(LogIterator_LibVLC, Excepciones_LibVLC);
										};

 inline libvlc_log_message_t		   *libvlc_log_iterator_next(libvlc_log_iterator_t *LogIterator_LibVLC,	libvlc_log_message_t *BufferMensaje, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_log_iterator_next)(LogIterator_LibVLC, BufferMensaje, Excepciones_LibVLC);
										};

 ///////////////////////////////////////// Excepciones
 inline int 							libvlc_exception_raised(libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_exception_raised)(Excepciones_LibVLC);
										};

 inline void 							libvlc_exception_clear(libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_exception_clear)(Excepciones_LibVLC);
										};

 inline void							libvlc_exception_init(libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_exception_init)(Excepciones_LibVLC);
										};

 inline const char 					   *libvlc_exception_get_message(libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_exception_get_message)(Excepciones_LibVLC);
										};

 ///////////////////////////////////////// Base
 inline libvlc_instance_t			   *libvlc_new(int NumArgs, const char *const *Args, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_new)(NumArgs, Args, Excepciones_LibVLC);
										};

 inline void							libvlc_release(libvlc_instance_t *Instancia_LibVLC) {
											(_libvlc_release)(Instancia_LibVLC);
										};

										// Warning:
										// Remember to release the object with vlc_object_release().
 vlc_object_t						   *libvlc_get_vlc_instance(libvlc_instance_t *Instancia_LibVLC) {
											return (_libvlc_get_vlc_instance)(Instancia_LibVLC);
										};

 int									libvlc_get_fullscreen(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_get_fullscreen)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline void							libvlc_set_fullscreen(libvlc_media_player_t *MediaPlayer_LibVLC, int BoolFullScreen, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_set_fullscreen)(MediaPlayer_LibVLC, BoolFullScreen, Excepciones_LibVLC);
										};

 inline const char					   *libvlc_get_version(void) {
											return (_libvlc_get_version)();
										};

 ///////////////////////////////////////// Medio
 inline libvlc_media_t				   *libvlc_media_new(libvlc_instance_t *Instancia_LibVLC, const char *PathMedio, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_new)(Instancia_LibVLC, PathMedio, Excepciones_LibVLC);
										};

 inline void						    libvlc_media_add_option(libvlc_media_t *Medio_LibVLC, const char *Opciones, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_add_option)(Medio_LibVLC, Opciones, Excepciones_LibVLC);
										};

 inline	void							libvlc_media_release(libvlc_media_t *Medio_LibVLC) {
											(_libvlc_media_release)(Medio_LibVLC);
										};

 inline libvlc_time_t				    libvlc_media_get_duration(libvlc_media_t *Medio_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_get_duration)(Medio_LibVLC, Excepciones_LibVLC);
										};

 ///////////////////////////////////////// Reproductor de medios								
 inline libvlc_media_player_t	 	   *libvlc_media_player_new(libvlc_instance_t *Instancia_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
//											MediaPlayer = (_libvlc_media_player_new)(Instancia_LibVLC, Excepciones_LibVLC);
											return (_libvlc_media_player_new)(Instancia_LibVLC, Excepciones_LibVLC);
										};

 inline libvlc_media_player_t	 	   *libvlc_media_player_new_from_media(libvlc_media_t *Medio_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_player_new_from_media)(Medio_LibVLC, Excepciones_LibVLC);;
										};

 inline libvlc_time_t				    libvlc_media_player_get_length(libvlc_media_player_t *MediaPlayer_LibVLC,  libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_player_get_length)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline libvlc_time_t				    libvlc_media_player_get_time(libvlc_media_player_t *MediaPlayer_LibVLC,  libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_player_get_time)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline float						    libvlc_media_player_get_position(libvlc_media_player_t *MediaPlayer_LibVLC,  libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_player_get_position)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline void						    libvlc_media_player_set_position(libvlc_media_player_t *MediaPlayer_LibVLC, float NuevaPosicion, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_player_set_position)(MediaPlayer_LibVLC, NuevaPosicion, Excepciones_LibVLC);
										};

 inline float						    libvlc_media_player_get_rate(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_player_get_rate)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline void						    libvlc_media_player_set_rate(libvlc_media_player_t *MediaPlayer_LibVLC, float NuevoRate, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_player_set_rate)(MediaPlayer_LibVLC, NuevoRate, Excepciones_LibVLC);
										};

 #if (LIBVLC_VERSION < 100)
  inline void 							libvlc_media_player_set_drawable(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_drawable_t hWndDestino, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_player_set_drawable)(MediaPlayer_LibVLC, hWndDestino, Excepciones_LibVLC);
										};
 #endif

 inline void 							libvlc_media_player_set_hwnd(libvlc_media_player_t *MediaPlayer_LibVLC, void *hWndDestino, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_player_set_hwnd)(MediaPlayer_LibVLC, hWndDestino, Excepciones_LibVLC);
										};

 inline void							libvlc_media_player_release(libvlc_media_player_t *MediaPlayer_LibVLC) {
											(_libvlc_media_player_release)(MediaPlayer_LibVLC);
										};

 inline void							libvlc_media_player_play(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_player_play)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline void							libvlc_media_player_pause(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_player_pause)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline void							libvlc_media_player_stop(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											(_libvlc_media_player_stop)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 inline libvlc_state_t				    libvlc_media_player_get_state(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_player_get_state)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 int								    libvlc_media_player_has_vout(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_player_has_vout)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 char								   *libvlc_video_get_aspect_ratio(libvlc_media_player_t *MediaPlayer_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_video_get_aspect_ratio)(MediaPlayer_LibVLC, Excepciones_LibVLC);
										};

 int								    libvlc_video_set_subtitle_file(libvlc_media_player_t *MediaPlayer_LibVLC, const char *PathSubtitulos, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_video_set_subtitle_file)(MediaPlayer_LibVLC, PathSubtitulos, Excepciones_LibVLC);
										};

 char								   *libvlc_media_get_meta(libvlc_media_t *Media_LibVLC, libvlc_meta_t TipoMeta_LibVLC, libvlc_exception_t *Excepciones_LibVLC) {
											return (_libvlc_media_get_meta)(Media_LibVLC, TipoMeta_LibVLC, Excepciones_LibVLC);
										};

 ///////////////////////////////////////// VLC Core
 void								    libvlc_core_vlc_object_release(vlc_object_t *Objeto_VLC) {
											(_libvlc_core_vlc_object_release)(Objeto_VLC);
										};
/*
 ///////////////////////////////////////// Media Control
 inline mediacontrol_Exception		   *VLC_MediaControl_Exception_Create(void) {
											ExcepcionesMediaControl = (LIBVLC_MediaControl_Exception_Create)();
											return ExcepcionesMediaControl;
										};

 inline mediacontrol_Instance		   *VLC_MediaControl_New_From_Instance(void) {
											return (LIBVLC_MediaControl_New_From_Instance)(Instancia, ExcepcionesMediaControl);
										};

 inline void							VLC_MediaControl_Display_Text(const char *message, const mediacontrol_Position *begin, const mediacontrol_Position *end) {
											(LIBVLC_MediaControl_Display_Text)(InstanciaMediaControl, message, begin, end, ExcepcionesMediaControl);
										};*/
 /////////////////////////////////////////
 HINSTANCE								LibVLC;
 HINSTANCE								LibVLC_Core;

protected :
 /*libvlc_exception_t						Excepciones;
 //mediacontrol_Exception				   *ExcepcionesMediaControl;
 libvlc_instance_t					   *Instancia;
 libvlc_instance_t					   *InstanciaAnalisis;
// mediacontrol_Instance				   *InstanciaMediaControl;
// libvlc_media_t						   *Media;
 libvlc_media_player_t				   *MediaPlayer;
 libvlc_log_t						   *Log;
 libvlc_log_iterator_t				   *IteratorLog;

// bool								   _RateModificado;*/

 // Parte estatica de la clase
// static bool									LIBVLC_DLL_Cargada;

 LIBVLC_GET_VERSION					   _libvlc_get_version;
 LIBVLC_GET_VLC_INSTANCE			   _libvlc_get_vlc_instance;
 LIBVLC_GET_FULLSCREEN				   _libvlc_get_fullscreen;
 LIBVLC_EXCEPTION_RAISED			   _libvlc_exception_raised;
 LIBVLC_EXCEPTION_CLEAR				   _libvlc_exception_clear;
 LIBVLC_EXCEPTION_INIT				   _libvlc_exception_init;
 LIBVLC_EXCEPTION_GET_MESSAGE		   _libvlc_exception_get_message;
 LIBVLC_NEW							   _libvlc_new;
 LIBVLC_RELEASE						   _libvlc_release;
 LIBVLC_SET_FULLSCREEN				   _libvlc_set_fullscreen;
 LIBVLC_AUDIO_GET_VOLUME			   _libvlc_audio_get_volume;
 LIBVLC_AUDIO_SET_VOLUME			   _libvlc_audio_set_volume;

 LIBVLC_LOG_COUNT					   _libvlc_log_count;
 LIBVLC_LOG_CLEAR				       _libvlc_log_clear;
 LIBVLC_LOG_GET_ITERATOR			   _libvlc_log_get_iterator;
 LIBVLC_LOG_ITERATOR_FREE			   _libvlc_log_iterator_free;
 LIBVLC_LOG_CLOSE					   _libvlc_log_close;
 LIBVLC_LOG_OPEN					   _libvlc_log_open;
 LIBVLC_LOG_ITERATOR_HAS_NEXT		   _libvlc_log_iterator_has_next;
 LIBVLC_LOG_ITERATOR_NEXT			   _libvlc_log_iterator_next;

 LIBVLC_MEDIA_NEW					   _libvlc_media_new;
 LIBVLC_MEDIA_ADD_OPTION			   _libvlc_media_add_option;
 LIBVLC_MEDIA_RELEASE				   _libvlc_media_release;
 LIBVLC_MEDIA_GET_DURATION			   _libvlc_media_get_duration;

 #if (LIBVLC_VERSION < 100)
 LIBVLC_MEDIA_PLAYER_SET_DRAWABLE	   _libvlc_media_player_set_drawable;
 #endif
 LIBVLC_MEDIA_PLAYER_SET_HWND	       _libvlc_media_player_set_hwnd;
 LIBVLC_MEDIA_PLAYER_PLAY			   _libvlc_media_player_play;
 LIBVLC_MEDIA_PLAYER_PAUSE			   _libvlc_media_player_pause;
 LIBVLC_MEDIA_PLAYER_STOP			   _libvlc_media_player_stop;
 LIBVLC_MEDIA_PLAYER_RELEASE		   _libvlc_media_player_release;
 LIBVLC_MEDIA_PLAYER_NEW			   _libvlc_media_player_new;
 LIBVLC_MEDIA_PLAYER_NEW_FROM_MEDIA	   _libvlc_media_player_new_from_media;
 LIBVLC_MEDIA_PLAYER_GET_LENGTH		   _libvlc_media_player_get_length;
 LIBVLC_MEDIA_PLAYER_GET_TIME		   _libvlc_media_player_get_time;
 LIBVLC_MEDIA_PLAYER_GET_POSITION	   _libvlc_media_player_get_position;
 LIBVLC_MEDIA_PLAYER_SET_POSITION	   _libvlc_media_player_set_position;
 LIBVLC_MEDIA_PLAYER_GET_RATE		   _libvlc_media_player_get_rate;
 LIBVLC_MEDIA_PLAYER_SET_RATE		   _libvlc_media_player_set_rate;
 LIBVLC_MEDIA_PLAYER_GET_STATE		   _libvlc_media_player_get_state;
 LIBVLC_MEDIA_PLAYER_HAS_VOUT		   _libvlc_media_player_has_vout;

 LIBVLC_VIDEO_GET_ASPECT_RATIO		   _libvlc_video_get_aspect_ratio;
 LIBVLC_VIDEO_SET_SUBTITLE_FILE		   _libvlc_video_set_subtitle_file;
 LIBVLC_MEDIA_GET_META				   _libvlc_media_get_meta;

 // VLC Core
 LIBVLC_CORE_VLC_OBJECT_RELEASE		   _libvlc_core_vlc_object_release;

// _mediacontrol_new_from_instance		LIBVLC_MediaControl_New_From_Instance;
// _mediacontrol_display_text				LIBVLC_MediaControl_Display_Text;
// _mediacontrol_exception_create			LIBVLC_MediaControl_Exception_Create;
};




// Clase para crear un array de chars con los argumentos que requiera el VLC, se ha diseñado de forma que resulte facil agregar nuevos argumentos
class ArgumentosVLC {
public :
								ArgumentosVLC(void) { 
									Argumentos = NULL; 
								};

							   ~ArgumentosVLC(void) { 
								    if (Argumentos != NULL) delete Argumentos;
									for (size_t i = 0; i < Args.size(); i++) {
										delete Args[i];
									}
							    };

								// Lo ideal es pasar un char a esta funcion ya que al final el vlc mira los parametros como char
								// De todas formas se han implementado funciones para pasar los argumentos directamente con wchar_t y DWL::DWLString
 void							AgregarArgumento(const char *Argumento) {
									char *Str = new char[512];
									strcpy_s(Str, 512, Argumento);
									Args.push_back(Str);
								};

 void							AgregarArgumento(const wchar_t *Argumento) {
									char *Str = new char[512];
									int	TamRes = WideCharToMultiByte(CP_UTF8, NULL, Argumento, wcslen(Argumento), Str, 512, NULL, NULL);
									Str[TamRes] = 0; // Termino el string, ya que en algunas compilaciones la funcion WideCharToMultiByte no pone el terminador.
									Args.push_back(Str);
								};

 /*void							AgregarArgumento(DWL::DWLString &Argumento) {
									char *Str = new char[512];
									int	TamRes = WideCharToMultiByte(CP_UTF8, NULL, Argumento(), Argumento.Tam(), Str, 512, NULL, NULL);
									Str[TamRes] = 0; // Termino el string, ya que en algunas compilaciones la funcion WideCharToMultiByte no pone el terminador.
									Args.push_back(Str);
								};*/

 const char					  **ObtenerArgumentos(void) {
									if (Argumentos != NULL) delete Argumentos;
									Argumentos = new const char * [Args.size()];
									for (size_t i = 0; i < Args.size(); i++) {
										Argumentos[i] = Args[i];
									}
									return Argumentos;
								};

 const char					  **operator () (void) {
									return ObtenerArgumentos();
								};

 const int						TotalArgumentos(void) {
									return static_cast<int>(Args.size());
								}
private :
 const char					  **Argumentos;
 std::vector<char *>			Args;
};


