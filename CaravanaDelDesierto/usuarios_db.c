#include "usuarios_db.h"

static void _accionGrabarEnArch(void *data, void *usuario)
{
    FILE *fp = (FILE *)usuario;
    fwrite(data, sizeof(tEntradaIndice), 1, fp);
}

static void _guardarIndice(tTabla *t)
{
    FILE *fp;
    if (!t || !t->indice) return;
    fp = fopen(t->pathIndice, "wb");
    if (!fp) return;
    arbolBinBusqVisitar(&t->indice, E_INORDEN, _accionGrabarEnArch, fp);
    fclose(fp);
    t->indiceSucio = FALSE;
}

int jugadorIndiceCmp(const void *a, const void *b)
{
    const tEntradaIndice *ea = (const tEntradaIndice *)a;
    const tEntradaIndice *eb = (const tEntradaIndice *)b;
    return strcmp(ea->clave, eb->clave);
}

int jugadorLeerClave(void *out, const void *in)
{
    const tRegistroJugador *reg = (const tRegistroJugador *)in;
    strncpy((char *)out, reg->nombre, INDICE_CLAVE_LEN - 1);
    ((char *)out)[INDICE_CLAVE_LEN - 1] = '\0';
    return OK;
}

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

    if(regTam == 0) {
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

    while(fread(&tmp, sizeof(tEntradaIndice), 1, fpIndice) == 1) {
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
    } else {
        /* Reconstruir índice desde archivo de datos */
        long offset = 0;
        char buf[256];
        tEntradaIndice entrada;

        fseek(t->archDatos, 0, SEEK_SET);
        while (fread(buf, t->regTam, 1, t->archDatos) == 1) {
            t->leerClave(entrada.clave, buf);
            entrada.offset = offset++;
            entrada.id = ((tRegistroJugador *)buf)->id;
            arbolBinBusqPoner(&t->indice, &entrada,
                              sizeof(tEntradaIndice), t->cmp);
        }
        _guardarIndice(t);
    }

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
    entrada.id = ((tRegistroJugador *)reg)->id;

    if(arbolBinBusqPoner(&t->indice, &entrada, sizeof(tEntradaIndice), t->cmp) == ERR) {
        return ERR;
    }

    fwrite(reg, t->regTam, 1, t->archDatos);

    _guardarIndice(t);

    return OK;
}

int tablaBuscar(tTabla *t, const void *clave, void *buf)
{
    tEntradaIndice entrada, claveBusqueda;

    if(!t || !clave || !buf || !t->archDatos) {
        return ERR;
    }

    t->leerClave(claveBusqueda.clave, clave);

    if(arbolBinBusqBuscar(&t->indice, &claveBusqueda, &entrada, sizeof(tEntradaIndice), t->cmp) == OK) {
        fseek(t->archDatos, entrada.offset * t->regTam, SEEK_SET);
        fread(buf, t->regTam, 1, t->archDatos);
    } else {
        return ERR;
    }


    return OK;
}

int tablaActualizar(tTabla *t, const void *clave, const void *reg)
{
    tEntradaIndice entrada;

    if(!t || !clave || !reg) {
        return ERR;
    }

    t->leerClave(entrada.clave, clave);

    if(arbolBinBusqBuscar(&t->indice, &entrada, &entrada, sizeof(tEntradaIndice), t->cmp) == OK) {
        fseek(t->archDatos, entrada.offset * t->regTam, SEEK_SET);
        fwrite(reg, t->regTam, 1, t->archDatos);
        fflush(t->archDatos);
    } else {
        return ERR;
    }


    return OK;
}

int tablaRecorrer(tTabla *t, fnAccion accion, void *usuario)
{
    char buf[256];

    if(!t || !accion || !t->archDatos) {
        return ERR;
    }

    fseek(t->archDatos, 0, SEEK_SET);

    while(fread(buf, t->regTam, 1, t->archDatos) == 1) {
        accion(buf, usuario);
    }

    return OK;
}

int tablaCerrar(tTabla *t)
{
    FILE *fpIndice;

    if(!t) {
        return ERR;
    }

if (t->archDatos) {
        fflush(t->archDatos);
        fclose(t->archDatos);
    }
	t->archDatos = NULL;

    if(t->indiceSucio) {
        fpIndice = fopen(t->pathIndice, "wb");

        if(!fpIndice) {
            return ERR;
        }

        arbolBinBusqVisitar(&t->indice, E_PREORDEN, _accionGrabarEnArch, fpIndice);

        fclose(fpIndice);
    }

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

int tablaProximoId(tTabla *t)
{
    long sz;

    if(!t || !t->archDatos) {
        return 0;
    }

    fseek(t->archDatos, 0, SEEK_END);
    sz = ftell(t->archDatos);
    return (int)(sz / t->regTam);
}
