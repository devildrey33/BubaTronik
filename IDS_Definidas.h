#pragma once

// IDS de controles y temporizadores

#define ID_HACKVLC					2000	// Timer para el temporizador del hack a la ventana del vlc
#define ID_TECLADO_GLOBAL			2001	// Timer para obtener el teclado
#define ID_ESCONDER_MOUSE			2002	// Timer para esconder el raton en videos a pantalla completa

#define ID_CONTROL_PRESENTACION		33001
#define ID_ARBOL_BASEDATOS			33002
#define ID_MEDIA_PLAYER				33004
#define ID_MEDIA_PLAYER_PC			33005
#define ID_LISTA_CANCIONES			33006
#define ID_SLIDER_TIEMPO			33007
#define ID_CONTROL_VOLUMEN			33008
#define ID_CM						33009
#define ID_OSCY						33010

#define WM_PARAMETROS WM_USER + 2132	// Mensaje que notifica la ejecucion de nuevos medios desde el explorador


#define ID_BARRA_BUSCAR						101
#define ID_RUTA_BUSCAR						103
#define ID_BOTON_BUSCAR2					104
#define ID_FILTRO_CARACTERES				105
#define ID_FILTRO_COINCIDENCIAS				106
#define ID_ARBOL_GENEROS					107
#define ID_ARBOL_GRUPOS						108
#define ID_ARBOL_DISCOS						109
#define ID_LISTA_ANOMALIAS_GENEROS			110
#define ID_MARCA_TAG						111
#define ID_MARCA_PATH						112
#define ID_COMBO_ORDEN_DIR					113	
//#define ID_BOTON_ANALIZARDISCOS				111
//#define ID_BOTON_BUSCARTERMINAR				112
//#define ID_OMITIR_TAGS1						113
//#define ID_BOTONERACP						114
//#define ID_APLICARGEN						115
//#define ID_RESTAURARGEN						116
//#define ID_APLICARGRP						117
//#define ID_RESTAURARGRP						118
//#define ID_APLICARDSK						119
//#define ID_RESTAURARDSK						120
#define ID_ASOCIAR_PREGUNTAR				121
#define ID_ASOCIAR_SIEMPRE					122
#define ID_ASOCIAR_NUNCA					123
#define ID_ASOCIAR_ADD						124
#define ID_ASOCIAR_REP						125
#define ID_EXCLUIR_NOTA						126
#define ID_BOTON_REVISAR					127
#define ID_BARRA_REVISAR					128
#define ID_OPCIONES_FUENTE					129
#define ID_LISTA_UP							130
#define ID_LISTA_DOWN						131
#define ID_BUSCAR_ACTUALIZACIONES			132
#define ID_LISTA_IDIOMAS					133
#define ID_LISTA_INICIO						134
#define ID_OBTENER_TECLADO					135
#define ID_SIEMPRE_DELANTE					136
#define ID_LISTA_ELIMINAR					137
#define ID_OPCIONES_SKIN					138
#define ID_OPCIONES_OSCY					139
#define	ID_OPCIONES_MOSTRAR_TAG				140
#define ID_OPCIONES_MOSTRAR_VIDEOS			141
#define ID_OPCIONES_OCULTAR_MOUSE			142
#define ID_OPCIONES_UTILIZAR_BARRA_WIN		143
#define ID_LISTA_TIPOS                      144
#define ID_BOTON_ELIMINAR_TIPOS             145
#define ID_BOTON_RESTAURAR_TIPOS            146
#define ID_BOTONERA_TIPOS                   147
#define ID_BOTON_ELIMINAR_TIPOS_AUDIO       148
#define ID_BOTON_ELIMINAR_TIPOS_VIDEO       149
#define ID_DIRECTORIOS_BUSCAR               150
#define ID_LISTA_MEDIOS_ENCONTRADOS         151
#define ID_BOTON_ELIMINAR_DIRECTORIO        152
#define ID_GENERO_FINAL                     153
#define ID_GENERO_ACCION                    154
#define ID_BOTON_AGREGAR_DIR                155
#define ID_COMBO_DIR						156
#define ID_GENERAR_LISTA					157
#define ID_TIPOS_LISTA                      158
#define ID_ICONO_BD							159
#define ID_ICONO_LISTA                      160
#define ID_ICONO_GENERO                     161
#define ID_ICONO_GRUPO                      162
#define ID_ICONO_DISCO                      163
#define ID_EDITLOG							164
#define ID_OPCIONES_TECLA_PLAY              165
#define ID_OPCIONES_TECLA_STOP              166
#define ID_OPCIONES_TECLA_VOLUMEN_MAS       167
#define ID_OPCIONES_TECLA_VOLUMEN_MENOS     168
#define ID_OPCIONES_TECLA_SIGUIENTE         169
#define ID_OPCIONES_TECLA_ANTERIOR          170
#define ID_OPCIONES_TECLA_ADELANTE          171
#define ID_OPCIONES_TECLA_ATRAS             172
#define ID_LISTA_GUARDAR					173

// Menus 
// -Menu Base de Datos Audio
#define IDMBDA_AGREGAR_LISTA       2000
#define IDMBDA_AGREGAR_NUEVA_LISTA 2001
#define IDMBDA_ELIMINAR            2002
#define IDMBDA_MODIFICAR           2003
#define IDMBDA_NOTA                2004
#define IDMBDA_N1                  2011
#define IDMBDA_N2                  2012
#define IDMBDA_N3                  2013
#define IDMBDA_N4                  2014
#define IDMBDA_N5                  2015
#define IDMBDA_ORDEN               2016
#define IDMBDA_TGGD                2020
#define IDMBDA_GGD                 2021
#define IDMBDA_GD                  2022
#define IDMBDA_D                   2023
#define IDMBDA_PROPIS              2029
#define IDMBDA_ABRIRDIR			   2030
// -Menu Base de Datos Video
//#define IDMBDV_AGREGAR_LISTA       2050
//#define IDMBDV_AGREGAR_NUEVA_LISTA 2051
//#define IDMBDV_ELIMINAR            2052
//#define IDMBDV_MODIFICAR           2053
//#define IDMBDV_NOTA                2054
//#define IDMBDV_N1                  2055
//#define IDMBDV_N2                  2056
//#define IDMBDV_N3                  2057
//#define IDMBDV_N4                  2058
//#define IDMBDV_N5                  2059
//#define IDMBDV_PROPIS              2060


// -Menu Boton BD
#define IDMBBD_BUSCAR             2100
#define IDMBBD_SELECIONAR         2101
#define IDMBBD_REVISAR            2102  
#define IDMBBD_FINBUSCAR          2103
// -Menu Repeat
#define IDMR_DESACTIVADO          3000
#define IDMR_NORMAL               3001
#define IDMR_NORMALSH             3002
#define IDMR_GENERO               3004
#define IDMR_GRUPO                3005
#define IDMR_DISCO                3006
#define IDMR_CANCION              3007
#define IDMR_APAGAR               3009
#define IDMR_APAGARR              3010
// -Menu Lista
#define IDMLI_ELIMINAR            3049
#define IDMLI_NOTA                3050
#define IDMLI_N1                  3051 
#define IDMLI_N2                  3052 
#define IDMLI_N3                  3053 
#define IDMLI_N4                  3054 
#define IDMLI_N5                  3055 
#define IDMLI_PROPIS              3060
#define IDMLI_MPISTA              3061
#define IDMLI_MNOMBRE             3062
#define IDMLI_COPIAR              3063
#define IDMLI_BUSCARBD			  3064
#define IDMLI_ABRIRDIR            3065
// -Menu Boton Lista
#define IDMBL_NUEVALISTA          3100
#define IDMBL_NARCHIVO            3101
#define IDMBL_LISTAVACIA          3102
#define IDMBL_NGEN                3103
#define IDMBL_NGRP                3104
#define IDMBL_NDSK                3105
#define IDMBL_NCAN                3106
#define IDMBL_N5E                 3107
#define IDMBL_NMAS_ESCUCHADAS	  3108
#define IDMBL_ALISTA              3109
#define IDMBL_ARCHIVO             3110
#define IDMBL_GEN                 3111
#define IDMBL_GRP                 3112
#define IDMBL_DSK                 3113
#define IDMBL_CAN                 3114
#define IDMBL_5E                  3115
#define IDMBL_MAS_ESCUCHADAS	  3116
#define IDMBL_GUARDAR             3117
// -Menu Video
#define IDMV_SUBTITULOS			  3120
#define IDMV_MODO_VIDEO			  3121

// -Menu Extras
//#define IDME_ACERCADE             3200
//#define IDME_BDATAG               3201
//#define IDME_CDAMP3               3202
//#define IDME_WDREY                3203
//#define IDME_WPPLAYER             3204
// -Menu ListView CDDA
//#define IDMCDDA_PISTA             3220
//#define IDMCDDA_NOMBRE            3221
// -Menu Oscyloscope (Nombre Cancion)
#define IDMOT_BUSCARBD            3230
#define IDMOT_BUSCARLISTA         3231
#define IDMOT_PROPIEDADES         3232
#define IDMOT_ELIMINAR            3233
#define IDMOT_ELIMINARBD          3234
#define IDMOT_ELIMINARLISTA       3235
#define IDMOT_NOTA                3236
#define IDMOT_NOTA1               3241
#define IDMOT_NOTA2               3242
#define IDMOT_NOTA3               3243
#define IDMOT_NOTA4               3244
#define IDMOT_NOTA5               3245
// -Menu del sistema (VentanaPlayer)
#define IDSM_ADELANTE             3250
#define IDSM_ATRAS                3251
#define IDSM_PLAY                 3252
#define IDSM_PAUSA                3253
#define IDSM_STOP                 3254
// -Menu General
#define IDMG_SHUFLE               3260
#define IDMG_REPEAT               3261
// -Menu Shufle
#define IDMS_SHUFLESI             3270
#define IDMS_SHUFLENO             3271
// -Menu Revisar BD
//#define IDMREV_MODIFICAR		  3280
//#define IDMREV_ELIMINAR_ANOMALIA  3281
//#define IDMREV_ELIMINAR_ENTRADA   3282
//#define IDMREV_ELEGIR_ENTRADA	  3283


#define TIEMPO_TOOLTIP 2200// milisegundos

#define MENUGEN  4000
#define MENUNGEN 5000


/*#define CP_GENERO     1600
#define CP_GRUPO      1601
#define CP_DISCO      1602
#define CP_TOP50      1603
#define CP_CAN        1604
#define CP_5ESTRELLAS 1605*/
#define CP_GENERO_MAS 1611
#define CP_GRUPO_MAS  1612
#define CP_DISCO_MAS  1613

#define CP_SUGERENCIA_GENERO1 1614
#define CP_SUGERENCIA_GRUPO1  1615
#define CP_SUGERENCIA_DISCO1  1616
#define CP_SUGERENCIA_GENERO2 1617
#define CP_SUGERENCIA_GRUPO2  1618
#define CP_SUGERENCIA_DISCO2  1619

#define CP_MEDIO_RECIENTE1    1620
#define CP_MEDIO_RECIENTE2    1621
#define CP_MEDIO_RECIENTE3    1622
#define CP_MEDIO_RECIENTE4    1623
#define CP_MEDIO_RECIENTE5    1624
#define CP_MEDIO_RECIENTE6    1625
#define CP_MEDIO_RECIENTE7    1626
#define CP_MEDIO_RECIENTE8    1627
#define CP_MEDIO_RECIENTE9    1628
#define CP_MEDIO_RECIENTE10   1629

#define CP_URL_FMOD           1630
#define CP_URL_LIBVLC         1631
#define CP_URL_DEVILDREY33    1632
#define CP_URL_MALEIT		  1633
#define CP_URL_BUBATRONIK     1634
#define CP_URL_DWL			  1635
#define CP_URL_FORO           1636

// Temporizadores
#define TIMER_LISTA					1002
#define TIMER_BARRATIEMPO			1003
#define TIMER_BLINK					1004
//#define TIMER_ANALISIS				1005
