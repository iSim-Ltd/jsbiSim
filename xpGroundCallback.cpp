#include "xpGroundCallback.h"
#include <math/FGLocation.h>
#include <math/FGColumnVector3.h>
#include "input_output/FGLog.h"

//this function is called by the FDM for EACH contact point on the aircraft (gear, wings, body etc) to determine attitude when touching the ground
//location is the location of the contact itself. XP probe data needs to be fed per location
double xpGroundCallback::GetAGLevel(double t, const JSBSim::FGLocation& location, JSBSim::FGLocation& contact, JSBSim::FGColumnVector3& normal, JSBSim::FGColumnVector3& vel, JSBSim::FGColumnVector3& angularVel) const{
 
  vel.InitMatrix();
  angularVel.InitMatrix();


  JSBSim::FGLocation l=location;
  const double a=20925646.32546;
  const double b=20855486.5951;
  l.SetEllipse(a, b);

  double latitude=l.GetGeodLatitudeRad();
  double longitude=l.GetLongitude();

  //converting xp normal vector to ECEF normal vector
  double east=*normalX;
  double up=*normalY;
  double north=-*normalZ;

  double sinLat=sin(latitude);
  double cosLat=cos(latitude);
  double sinLon=sin(longitude);
  double cosLon=cos(longitude);

  double x=-(sinLon*east) - (sinLat*cosLon*north) + (cosLat*cosLon*up);
  double y=(cosLon*east) - (sinLat*sinLon*north) + (cosLat*sinLon*up);
  double z=(cosLat*north) + (sinLat*up);

  JSBSim::FGColumnVector3 ecefNormal(x, y, z);
  double mag=ecefNormal.Magnitude();
  if(mag>1e-9) ecefNormal/=mag;  // normalize to unit vector

  normal=ecefNormal;


  //contact.SetEllipse(a, b);
  contact.SetPositionGeodetic(longitude, latitude, *terrainElevation);

  return l.GetGeodAltitude()-*terrainElevation;
}
