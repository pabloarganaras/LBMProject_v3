#ifndef WRITEVECTORFIELD_H
#define WRITEVECTORFIELD_H

#include <basicSolverInfo.h>

#ifdef __cplusplus
extern "C" {
#endif

    void writeVectorField( char* fname, c_scalar** field, struct solverInfo* info);
    
#ifdef __cplusplus
}
#endif
    

#endif // WRITEVECTORFIELD_H
