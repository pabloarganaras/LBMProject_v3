#include <cahnHilliardEquilibrium.h>

void cahnHilliardEquilibrium(struct twoPhasesFields* fields, const struct solverInfo* info, double* eq, const unsigned int id) {

    eq[0] = fields->phi[id] + (info->lattice.omega[0] - 1) * info->fields.eta * fields->muPhi[id];

    unsigned int k;
    double dot;

    for( k = 1 ; k < info->lattice.Q ; k++) {
	dot = info->lattice.vel[k][0] * fields->U[id][0]   +   info->lattice.vel[k][1] * fields->U[id][1]   +   info->lattice.vel[k][2] * fields->U[id][2];
	eq[k] = info->lattice.omega[k] * info->fields.eta * fields->muPhi[id]   +   info->lattice.c * dot * info->lattice.omega[k] * fields->phi[id] / info->lattice.cs2;
    }

}
