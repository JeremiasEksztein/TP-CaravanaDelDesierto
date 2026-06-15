#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED

#define MENU_TEXTO "==========================\n"\
                    "Bienvenido a la Caravana del Desierto\n"\
                    "\n1. JUGAR\n"\
                    "\n2. VER RANKING\n"\
                    "\n3. Salir\n"\
                    "\n==========================\n"
#define OPCIONES_MENU "123"
#define SALIR '3'
#define JUGAR '1'
#define RANKING '2'

char CrearMenuInicial(const char* menuTexto, const char* opciones);

#endif
