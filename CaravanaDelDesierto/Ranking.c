#include "Ranking.h"

int MostrarRankingDeJugadores()
{
    tRanking 
    FILE* arch = fopen(NOMBRE_ARCHIVO_RANKING, "rt" );
    if(!arch)
    {
        return EXIT_FAILURE;
    }
    while(fgets(linea,sizeof(linea),arch))
    {

    }
    fclose(arch);
    return EXIT_SUCCESS;
}