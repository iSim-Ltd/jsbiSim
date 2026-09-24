#pragma once

#include <math/FGLocation.h>
#include "input_output/FGGroundCallback.h"

class xpGroundCallback :public JSBSim::FGGroundCallback{
public:
  xpGroundCallback(const double* aglElev, const double* nX, const double* nY, const double* nZ) : terrainElevation(aglElev), normalX(nX), normalY(nY), normalZ(nZ){}
  double GetAGLevel(double t, const JSBSim::FGLocation& location,
    JSBSim::FGLocation& contact,
    JSBSim::FGColumnVector3& normal, JSBSim::FGColumnVector3& v,
    JSBSim::FGColumnVector3& w) const override;

private:
  const double* terrainElevation;
  const double* normalX;
  const double* normalY;
  const double* normalZ;
};