// STDIN.H SUPER BASICO PARA LO QUE NECESITA LA LIBVLC (Solo se usa en VC2008)
// En las propiedades de Bubatronik se ha añadido el directorio de inclusion "." que redirige el include <stdint.h> al archivo que se creo para bubatronik.
#ifndef STDINT
    #define STDINT
    typedef unsigned long int uint32_t;
    typedef __int64 int64_t;
    typedef unsigned long long int uint64_t;
#endif