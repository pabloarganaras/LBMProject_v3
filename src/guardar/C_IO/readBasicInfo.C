// Read information for c-written solvers

#include <basicSolverInfo.h>
#include <dictionary.h>
#include <LBModelCreator.h>
#include <fstream>

#ifdef __cplusplus
extern "C" {
#endif


struct solverInfo readBasicInfo() {

    struct solverInfo info;

    // Read time properties
    dictionary tp("properties/simulation");

    info.time.start = tp.lookUpEntry<double>("startTime");
    info.time.end = tp.lookUpEntry<double>("endTime");
    info.time.tstep = tp.lookUpEntry<double>("timeStep");
    info.time.writeInterval = tp.lookUpEntry<double>("writeInterval");
    info.time.current = info.time.start;
    info.time.stp = 0;

    info.time.st = time( NULL );


    // Read lattice properties
    dictionary lp("properties/latticeProperties");

    LBModelCreator creator;
    basicLBModel* lbm = creator.create( lp.lookUpEntry<string>("LBModel") );
    
    info.lattice.size = lp.lookUpEntry<double>("latticeSize");
    info.lattice.c = info.lattice.size / info.time.tstep;
    info.lattice.cs2 = info.lattice.c * info.lattice.c * lbm->cs2();
    info.lattice.d = lbm->D();
    info.lattice.Q = lbm->Q();


    // Read total number of points from neighbour file
    std::ostringstream fileName;
    fileName << "processor0" << "/" << lbm->name() << "_lattice/neighbours" ;
    
    std::ifstream inFile;
    inFile.open( fileName.str().c_str() );
    if( !inFile.is_open() ) {
	std::cout << "Unable to open file " << fileName << std::endl;
    	exit(1);
    }
    
    inFile >> info.lattice.nlocal;

    inFile.close();



    // Lattice weights
    const vector<double> omega = lbm->omega();
    info.lattice.omega = (double*)malloc( lbm->Q() * sizeof(double));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.lattice.omega[i] = omega[i];


    // Lattice velocities
    const vector<Vector3> vel = lbm->latticeVel();
    info.lattice.vel = (int**)malloc( lbm->Q() * sizeof(int*));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.lattice.vel[i] = (int*)malloc( 3 * sizeof(int));

    for(uint i = 0 ; i < lbm->Q() ; i++) {
	info.lattice.vel[i][0] = (int)vel[i].x();
	info.lattice.vel[i][1] = (int)vel[i].y();
	info.lattice.vel[i][2] = (int)vel[i].z();
    }
    
    

    // Read macroscopic properties
    dictionary mp("properties/macroscopicProperties");

    pdf Sh = mp.lookUpEntry<pdf>("h/S");

    info.fields.colMat = (double**)malloc( lbm->Q() * sizeof(double*));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.fields.colMat[i] = (double*)malloc( lbm->Q() * sizeof(double));

    Matrix SS( Sh.size() );
    for(uint i = 0 ; i < Sh.size() ; i++)
	SS[i][i] = Sh[i];

    Matrix M = lbm->invM() * SS * lbm->M();

    for(uint i = 0 ; i < lbm->Q() ; i++) {
	for(uint j = 0 ; j < lbm->Q() ; j++) {
	    info.fields.colMat[i][j] = M[i][j];
	}
    }
    

    info.fields.srcMat = (double**)malloc( lbm->Q() * sizeof(double*));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.fields.srcMat[i] = (double*)malloc( lbm->Q() * sizeof(double));    

    for(uint i = 0 ; i < Sh.size() ; i++)
	SS[i][i] = 1.0 - (0.5 * Sh[i]);

    M = lbm->invM() * SS * lbm->M();

    for(uint i = 0 ; i < lbm->Q() ; i++) {
	for(uint j = 0 ; j < lbm->Q() ; j++) {
	    info.fields.srcMat[i][j] = M[i][j];
	}
    }
    


    pdf Sg_a = mp.lookUpEntry<pdf>("g/S_a");
    
    info.fields.colMatA = (double**)malloc( lbm->Q() * sizeof(double*));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.fields.colMatA[i] = (double*)malloc( lbm->Q() * sizeof(double));

    for(uint i = 0 ; i < Sg_a.size() ; i++)
	SS[i][i] = Sg_a[i];

    M = lbm->invM() * SS * lbm->M();

    for(uint i = 0 ; i < lbm->Q() ; i++) {
	for(uint j = 0 ; j < lbm->Q() ; j++) {
	    info.fields.colMatA[i][j] = M[i][j];
	}
    }
    
    
    info.fields.srcMatA = (double**)malloc( lbm->Q() * sizeof(double*));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.fields.srcMatA[i] = (double*)malloc( lbm->Q() * sizeof(double));    

    for(uint i = 0 ; i < Sg_a.size() ; i++)
	SS[i][i] = 1.0 - (0.5 * Sg_a[i]);

    M = lbm->invM() * SS * lbm->M();

    for(uint i = 0 ; i < lbm->Q() ; i++) {
	for(uint j = 0 ; j < lbm->Q() ; j++) {
	    info.fields.srcMatA[i][j] = M[i][j];
	}
    }
    

    
    pdf Sg_b = mp.lookUpEntry<pdf>("g/S_b");

    info.fields.colMatB = (double**)malloc( lbm->Q() * sizeof(double*));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.fields.colMatB[i] = (double*)malloc( lbm->Q() * sizeof(double));

    for(uint i = 0 ; i < Sg_b.size() ; i++)
	SS[i][i] = Sg_b[i];

    M = lbm->invM() * SS * lbm->M();

    for(uint i = 0 ; i < lbm->Q() ; i++) {
	for(uint j = 0 ; j < lbm->Q() ; j++) {
	    info.fields.colMatB[i][j] = M[i][j];
	}
    }
    
    info.fields.srcMatB = (double**)malloc( lbm->Q() * sizeof(double*));
    for(uint i = 0 ; i < lbm->Q() ; i++)
	info.fields.srcMatB[i] = (double*)malloc( lbm->Q() * sizeof(double));    


    for(uint i = 0 ; i < Sg_b.size() ; i++)
	SS[i][i] = 1.0 - (0.5 * Sg_b[i]);

    M = lbm->invM() * SS * lbm->M();

    for(uint i = 0 ; i < lbm->Q() ; i++) {
	for(uint j = 0 ; j < lbm->Q() ; j++) {
	    info.fields.srcMatB[i][j] = M[i][j];
	}
    }
    

    info.fields.sigma = mp.lookUpEntry<double>("h/sigma");
    
    info.fields.D = mp.lookUpEntry<double>("h/D");
    
    info.fields.phi_A = mp.lookUpEntry<double>("Macro/phi_A");

    info.fields.phi_B = mp.lookUpEntry<double>("Macro/phi_B");

    info.fields.M_phi = mp.lookUpEntry<double>("Macro/M_phi");

    info.fields.rho_A = mp.lookUpEntry<double>("Macro/rho_A");
    
    info.fields.rho_B = mp.lookUpEntry<double>("Macro/rho_B");

    Vector3 g = mp.lookUpEntry<Vector3>("g/G");

    info.fields.gx = g.x();
    info.fields.gy = g.y();
    info.fields.gz = g.z();
    
    
    info.fields.eta = info.fields.M_phi / (   info.lattice.cs2 * info.time.tstep * (1/Sh[3] - 0.5)   );

    info.fields.kappa = 3 * info.fields.sigma * info.fields.D / (2 * (info.fields.phi_A - info.fields.phi_B) * (info.fields.phi_A - info.fields.phi_B));
    
    info.fields.beta  = 12 * info.fields.sigma / ( info.fields.D * pow(info.fields.phi_A - info.fields.phi_B, 4));    
    
    
    return info;
    
}


#ifdef __cplusplus
}
#endif
