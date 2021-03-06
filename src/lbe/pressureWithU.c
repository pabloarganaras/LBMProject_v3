#include <pressureWithU.h>
#include <scalarFieldGradient.h>
#include <stdlib.h>


void pressureWithU( struct twoPhasesFields* fields, struct solverInfo* info, c_scalar* fld ) {

    unsigned int id, k;

    // Move over points
    for( id = 0 ; id < info->lattice.nlocal ; id++ ) {

	// Pdf reduction, excluding first element
	c_scalar red = 0;
	for( k = 1 ; k < info->lattice.Q ; k++ ) {

	    red += fields->g[id][k];

	}

	// Dot product: U * grad(rho)
	c_scalar* rhoGrad;
	rhoGrad = scalarFieldGradient( fields, info, fields->rho, id );
	c_scalar dot = fields->U[id][0] * rhoGrad[0]   +   fields->U[id][1] * rhoGrad[1]   +   fields->U[id][2] * rhoGrad[2];

	// Equilibrium source term
	c_scalar Umag  = fields->U[id][0] * fields->U[id][0]  +  fields->U[id][1] * fields->U[id][1]  +  fields->U[id][2] * fields->U[id][2];
	c_scalar UdotC = info->lattice.c * ( fields->U[id][0] * info->lattice.vel[0][0]   +   fields->U[id][1] * info->lattice.vel[0][1]   +   fields->U[id][2] * info->lattice.vel[0][2] );
	c_scalar s_eq  = info->lattice.omega[0]  *  (  UdotC / info->lattice.cs2   +   UdotC * UdotC * 0.5 / (info->lattice.cs2 * info->lattice.cs2)   -  Umag * 0.5 / info->lattice.cs2 );	
	
	// Pressure at node
	fld[id] = ( info->lattice.cs2 / (1 - info->lattice.omega[0]) )  *  ( red   +  dot  * 0.5 * info->time.tstep   +   fields->rho[id] * s_eq );
	

	// Deallocate memory
	free(rhoGrad);
    }
    
}
