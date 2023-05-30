#include "zerr.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

zerr_pd *zerr_new(sys_config *config) {
    zerr_pd *z = (zerr_pd *) malloc(sizeof(zerr_pd));
    if (!z) return NULL;
    z->n_outlet = 6;
    memcpy(&z->cfg, config, sizeof(sys_config));

    return z;
}

void zerr_free(zerr_pd *z) {
    free(z);
}

void zerr_perform(zerr_pd *z, float *in, float *out, int n) {
    post("Hello world !!");
}

#ifdef __cplusplus
}
#endif
