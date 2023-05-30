#include "zerr.h"

#include <stdlib.h>

zerr *zerr_new(sys_config *config) {
    zerr *z = malloc(sizeof(zerr));
    if (!z) return NULL;
    post("Hello world !!");
    // memcpy(&z->cfg, config, sizeof(zerr_config));

    // g->cfg.mgr = control_manager_new();
    // if (!g->cfg.mgr) return NULL;

    // g->vd = vd_new(g->cfg.sample_rate);
    // if (!g->vd) return NULL;

    // g->modbank = modulator_bank_new(&g->cfg, g->vd);
    // if (!g->modbank) return NULL;

    // g->gran = granular_new();
    // if (!g->gran) return NULL;

    // g->schdur = scheduler_new(&g->cfg);
    // if (!g->schdur) return NULL;

    return z;
}

void zerr_free(zerr *z) {
    // granular_free(g->gran);
    // vd_free(g->vd);
    // scheduler_free(g->schdur);
    // modulator_bank_free(g->modbank);
    // control_manager_free(g->cfg.mgr);
    free(z);
}

void zerr_perform(zerr *z, float *in, float *out, int n) {
    // post("Hello world !!");
    // control_manager_perform(g->cfg.mgr, in, n);
    // vd_perform(g->vd, in, n);
    // scheduler_perform(g->schdur, n);
    // granular_perform(g->gran, g->schdur, g->vd, in, out, n); // update the buffer and manipulate the DelayLine
}