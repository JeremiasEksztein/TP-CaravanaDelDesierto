#include "usuarios_db.h"

int tablaCrear(
    tTabla *t, 
    const char *pathDatos, 
    const char *pathIndice, 
    fnCmp cmp,
    fnLeerClave leer,
    unsigned regTam, 
    unsigned claveTam
) {
    if(!t || !pathDatos || !pathIndice || !cmp || !leer) {
        return ERR;
    }

    if(claveTam > regTam) {
        return ERR;
    }

    t->archDatos = NULL;
    arbolBinBusqCrear(&t->indice);
    strncpy(t->pathDatos, pathDatos, ARCHPATHLEN);
    strncpy(t->pathIndice, pathIndice, ARCHPATHLEN);
    t->regTam = regTam;
    t->claveTam = claveTam;
    t->cmp = cmp;
    t->leerClave = leer;
    t->indiceSucio = FALSE;

    return OK;
}

static int _cargarIndice(tTabla *t, FILE *fpIndice)
{
    tEntradaIndice tmp;
    
    while(!feof(fpIndice)) {
        fread(&tmp, sizeof(tEntradaIndice), 1, fpIndice);

        if(arbolBinBusqPoner(&t->indice, &tmp, sizeof(tmp), t->cmp) != OK) {
            return ERR;
        }
    }

    return OK;
}

int tablaAbrir(tTabla *t)
{
    FILE *fpIndice;

    if(!t) {
        return ERR;
    }

    t->archDatos = fopen(t->pathDatos, "rb+");

    if(!t->archDatos) {
        t->archDatos = fopen(t->pathDatos, "wb+");

        if(!t->archDatos) {
            return ERR;
        }
    }

    fpIndice = fopen(t->pathIndice, "rb");

    if(fpIndice) {
        _cargarIndice(t, fpIndice);
        fclose(fpIndice);
    }

    /*
    IF t->pathDatos NOT EXISTS
        CREATE t->pathDatos
        OPEN t->pathDatos
    ENDIF

    IF t->pathIndice EXISTS
        OPEN t->pathIndice
        LOAD t->pathIndice INTO t->indice
    ELSE
        CREATE t->pathIndice
    ENDIF
    */

    return OK;
}

int tablaIngresar(tTabla *t, const void *reg)
{
    long ultimoOffset;
    tEntradaIndice entrada;

    if(!t || !reg || !t->archDatos) {
        return ERR;
    }

    fseek(t->archDatos, 0, SEEK_END);
    ultimoOffset = ftell(t->archDatos) / t->regTam;
    
    t->leerClave(entrada.clave, reg);
    entrada.offset = ultimoOffset;
    
    if(arbolBinBusqPoner(&t->indice, &entrada, sizeof(tEntradaIndice), t->cmp) == ERR) {
        return ERR;
    }
    
    fwrite(reg, t->regTam, 1, t->archDatos);
    
    t->indiceSucio = TRUE;

    /*
        APPEND reg INTO t->archDatos

        INSERT t->leer(reg) INTO t->indice
    
        SET t->indiceSucio TO TRUE
    */

    return OK;
}

int tablaBuscar(tTabla *t, const void *clave, void *buf)
{
    tEntradaIndice entrada, claveBusqueda;

    if(!t || !clave || !buf || !t->archDatos) {
        return ERR;
    }

    t->leerClave(claveBusqueda.clave, clave);

    if(arbolBinBusqBuscar(&t->indice, &claveBusqueda, &entrada, sizeof(tEntradaIndice), t->cmp)) {
        fseek(t->archDatos, entrada.offset * t->regTam, SEEK_SET);
        fread(buf, t->regTam, 1, t->archDatos);
    } else {
        return ERR;
    }

    /*
        IF clave IN t->indice 
            READ FROM t->archDatos IN OFFSET GIVEN
            WRITE TO buf
        ELSE
            RETURN ERROR
    */

    return OK;
}

int tablaActualizar(tTabla *t, const void *clave, const void *reg)
{
    tEntradaIndice entrada;

    if(!t || !clave || !reg) {
        return ERR;
    }

    t->leerClave(entrada.clave, clave);

    if(arbolBinBusqBuscar(&t->indice, &entrada, &entrada, sizeof(tEntradaIndice), t->cmp)) {
        fseek(t->archDatos, entrada.offset * t->regTam, SEEK_SET);
        fwrite(reg, t->regTam, 1, t->archDatos);
        fflush(t->archDatos);
    } else {
        return ERR;
    }

    /*
        IF clave IN t->indice
            WRITE TO t->archivos IN OFFSET GIVEN reg
        ELSE
            RETURN ERROR
    */

    return OK;
}

int tablaRecorrer(tTabla *t, fnAccion accion, void *usuario)
{
    if(!t || !accion) {
        return ERR;
    }



    /*
        FOR clave IN t->indice
            READ INTO tmp FROM t->archDatos[clave]
            accion(tmp, usuario)
    */

    return OK;
}

int tablaCerrar(tTabla *t)
{
    if(!t) {
        return ERR;
    }

    fflush(t->archDatos);
    fclose(t->archDatos);
    t->archDatos = NULL;

    if(t->indiceSucio) {
        t->archDatos = fopen(t->pathIndice, "wb");

        if(!t->archDatos) {
            return ERR;
        }

        arbolBinBusqVisitar(&t->indice, E_PREORDEN, _accionGrabarEnArch, t);

        fclose(t->archDatos);
        t->archDatos = NULL;
    }


    /*
        CLOSE t->archDatos

        VISIT PREORDER t->indice
            WRITE TO t->pathIndice
    */

    return OK;
}

void tablaDestruir(tTabla *t)
{
    if(!t) {
        return;
    }

    if(t->archDatos) {
        fclose(t->archDatos);
        t->archDatos = NULL;
    }

    arbolBinBusqDestruir(&t->indice);
}
