#ifndef TIMEOPTIONS_H
#define TIMEOPTIONS_H

#include <basicSolverInfo.h>
#include <readNeighbours.h>
#include <basicSolverInfo.h>
#include <readBasicInfo.h>
#include <readScalarField.h>
#include <readVectorField.h>
#include <readPdfField.h>


#ifdef __cplusplus
extern "C" {
#endif

    // Elapsed time
    const double elapsed (const struct solverInfo* info);

    // Flag to enable writing
    const int writeFlag(const struct solverInfo* info);

    // Update time structure
    const int updateTime(struct solverInfo* info);
    
    
#ifdef __cplusplus
}
#endif
    

#endif // TIMEOPTIONS_H
