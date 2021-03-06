// Write field for c-written solvers

#include <writeVectorField.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif


void writeVectorField( char* fname, c_scalar** field, struct solverInfo* info) {

    // Create folder if it does not exist
    std::ostringstream fileName;
    fileName << "processor" << info->parallel.pid << "/" << info->time.current << "/";
    system( ("mkdir -p " + fileName.str()).c_str() );
    
    // Open file
    std::ofstream outFile;
    fileName << fname;
    outFile.open( fileName.str().c_str() );
    if( !outFile.is_open() ) {
    	std::cout << "Unable to open file " << fileName << std::endl;
    	exit(1);
    }    

    
    // Total number of points
    outFile << info->lattice.nlocal + info->parallel.nghosts << std::endl;
    
    // Write elements
    for(uint i = 0 ; i < info->lattice.nlocal + info->parallel.nghosts ; i++) {
	outFile << field[i][0] << " ";
	outFile << field[i][1] << " ";
	outFile << field[i][2] << std::endl;
    }
    
    // Close file
    outFile.close();

}


#ifdef __cplusplus
}
#endif
