#pragma once

#include <math/FGLocation.h>
#include "input_output/FGGroundCallback.h"
#include "iSimABI.h"
#include <math/FGColumnVector3.h>
#include "input_output/FGLog.h"

class xpGroundCallback :public JSBSim::FGGroundCallback{
public:
  xpGroundCallback(GroundContactCallback callback)
    : callback(callback){}
  double GetAGLevel(double t, const JSBSim::FGLocation& location,
    JSBSim::FGLocation& contact,
    JSBSim::FGColumnVector3& normal, JSBSim::FGColumnVector3& v,
    JSBSim::FGColumnVector3& w) const override;

  GroundContactCallback callback;
};