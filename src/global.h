#include <QString>

#ifndef GLOBAL_H
#define GLOBAL_H

enum class Tasks
{
    solveMetalEquation,
    solveFilmEquation,
    solveClEquation,
    solveOHEquation,
    solveLSEquation,
    solveFluidEquation,
    savingVTKOutput,
    None
};

struct Simulation
{
    QString outputDir;
    int mpiSize;
    bool vtkOutput;
    bool running;
};

#endif // GLOBAL_H
