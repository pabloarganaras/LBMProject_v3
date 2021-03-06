#include <stdio.h>
#include <twoPhasesFields.h>
#include <timeOptions.h>
#include <syncScalarField.h>
#include <syncPdfField.h>
#include <writeScalarField.h>
#include <writeVectorField.h>
#include <writePdfField.h>
#include <lbstream.h>

int main( int argc, char **argv ) {

    int pid, world;
    
    // Initialize mpi
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);
    MPI_Comm_size(MPI_COMM_WORLD,&world);

    if(pid == 0) {
	printf("\n");
	printf("     o-----o-----o  \n");
	printf("     | -   |   - |  \n");
	printf("     |   - | -   |  \n");
	printf("     o<----o---->o       Two Phases - Lattice-Boltzmann solver\n");
	printf("     |   - | -   |  \n");
	printf("     | -   |   - |  \n");
	printf("     o-----o-----o  \n");
    }
	
    // Simulation properties
    struct solverInfo info = readBasicInfo( pid, world );

    
    // Read Fields
    struct twoPhasesFields fields;

    // Neighbours indices
    fields.nb = readNeighbours(&info);
    if(pid == 0) { printf("\nReading neighbour indices\n"); }
    
    // Order parameter
    fields.phi     = readScalarField("phi", &info);
    fields.phi_old = readScalarField("phi", &info);
    if(pid == 0) { printf("\nReading field phi\n"); }

    // Velocity
    fields.U     = readVectorField("U", &info);
    fields.U_old = readVectorField("U", &info);
    if(pid == 0) { printf("\nReading field U\n");  }

    // Navier-Stokes field
    fields.g = readPdfField("g", &info);
    fields.swp = readPdfField("g", &info);
    if(pid == 0) { printf("\nReading field g\n\n\n");  }
    


    // Synchronize phi
    unsigned int id, k;
    for( id = 0 ; id < info.lattice.nlocal ; id++) {
    	fields.phi[id] = id;
    }
    syncScalarField( &info, fields.phi );

    
    // Synchronize U
    for( id = 0 ; id < info.lattice.nlocal ; id++) {
    	for( k = 0 ; k < 3 ; k++ ) {
    	    fields.U[id][k] = id;
    	}
    }
    syncPdfField( &info, fields.U, 3 );

    // Synchronize g
    for( id = 0 ; id < info.lattice.nlocal ; id++) {
    	for( k = 0 ; k < info.lattice.Q ; k++ ) {
    	    fields.g[id][k] = id;
    	}
    }
    syncPdfField( &info, fields.g, info.lattice.Q );
    

    // Advance in time. Collide, stream, update and write
    while( updateTime(&info) ) {
		
    	// Write fields
    	if( writeFlag(&info) ) {
	    
    	    if(pid == 0) {
    		printf("Time = %.2f\n", (double)info.time.current);
    		printf("Elapsed time = %.2f seconds\n\n", elapsed(&info) );
    	    }
	    
    	    // ScalarFields
    	    writeScalarField("phi", fields.phi, &info);
    	
    	    // Vector fields
    	    writeVectorField("U", fields.U, &info);

    	    // Pdf fields
    	    writePdfField("g", fields.g, &info);
	    
    	}

    }


    
    // Print info
    if(pid == 0) {
	printf("\n  Finished in %.3f seconds \n\n", elapsed(&info) );
    }


    MPI_Finalize();
    
    return 0;
    
}
