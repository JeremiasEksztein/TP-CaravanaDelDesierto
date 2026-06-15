#include "Ranking.h"

int MostrarRankingDeJugadores()
{
    unsigned ranking = 1;
    tRegistroPartida partida;
    FILE* arch = fopen(NOMBRE_ARCHIVO_RANKING, "rb" );
    if(!arch)
    {
        return EXIT_FAILURE;
    }
    //Los puedo mostrar secuencialmente porque se guardaron ordenados
    printf("RANKING |    NOMBRE   |  MOVIMIENTOS |  PUNTAJE\n");
    while(fread(&partida,sizeof(tRegistroPartida),1,arch))
    {
        printf("%08u|%13s|%014u|%10d\n",ranking,partida.nombreCuenta,partida.cantJugadas,partida.puntaje);
        ranking++;
    }
    fclose(arch);
    return EXIT_SUCCESS;
}
int GuardarJugadorEnRanking(tJugador* jugador)
{
    
}