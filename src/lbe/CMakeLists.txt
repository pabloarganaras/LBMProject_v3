#------------------ Generacion de bibliotecas ------------------#
 
# Basic lattice-Boltzmann Equation library
add_library(lbe SHARED
  matVecMult.c
  scalarFieldLaplacian.c
  scalarFieldGradient.c
  density.c
  lbstream.c
  syncScalarField.c
  syncPdfField.c
  chemicalPotential.c
  pressureWithU.c
  orderParameter.c
  velocity.c
  cahnHilliardSource.c
  cahnHilliardEquilibrium.c
  cahnHilliardCollision.c
  liangEquilibrium.c
  liangCollision.c
  liangSource.c
  )