#include <memory>
#include <unordered_map>
#include "iSimABI.h"
#include "FGFDMExec.h"
#include "models/FGPropulsion.h"
#include "initialization/FGInitialCondition.h"

using namespace std;

extern "C" {
  //struct for storing the FDM and its components
  struct JSBSim_FDM{
    unique_ptr<JSBSim::FGFDMExec> fdm;
    vector<SGPropertyNode_ptr> properties;
    std::unordered_map<std::string, uint32_t> propertyIDMap;
  };

  //create function that also is responsible for filling in the references to all the FDM components
  //doing it this way ensures that we have the correct instance of each component owned and controlled by our FDMExec instance
  void* JSBSim_Create(){
    auto* handle=new JSBSim_FDM;
    handle->fdm=make_unique<JSBSim::FGFDMExec>();
    return handle;
  }

  void JSBSim_Destroy(void* ptr){
    delete static_cast<JSBSim_FDM*>(ptr);
  }

  bool JSBSim_SetDirectories(void* ptr, const char* rootDir, const char* aircraftDir, const char* engineDir, const char* systemsDir){
    auto* JSB=static_cast<JSBSim_FDM*>(ptr);
    try{
      JSB->fdm->SetRootDir(SGPath(rootDir));
      JSB->fdm->SetAircraftPath(SGPath(aircraftDir));
      JSB->fdm->SetEnginePath(SGPath(engineDir));
      JSB->fdm->SetSystemsPath(SGPath(systemsDir));
      return true;
    }
    catch(int err){
      cout<<"Error setting directories: "<<err<<endl;
      return false;
    }
  }

  engineInfo JSBSim_LoadModel(void* ptr, const char* modelPath){
    try{
      auto* JSB=static_cast<JSBSim_FDM*>(ptr);
      engineInfo info;

      info.success =  JSB->fdm->LoadModel(modelPath);
      auto propulsion=JSB->fdm->GetPropulsion();
      info.numEngines=propulsion->GetNumEngines();
      info.numTanks=propulsion->GetNumTanks();
      info.engineType=propulsion->GetEngine(0)->GetType();

      return info;
    }
    catch(...){
      engineInfo info;
      info.numEngines=0;
      info.numTanks=0;
      info.engineType=0;
      info.success=false;
      return info;
    }

  }

  bool JSBSim_RunIC(void* ptr){
    auto* JSB=static_cast<JSBSim_FDM*>(ptr);
    return JSB->fdm->RunIC();
  }


  bool JSBSim_SetInitialCondition(void* ptr, double latDeg, double lonDeg, double altft, double hdgTrue, double kts){
    auto* JSB=static_cast<JSBSim_FDM*>(ptr);
    auto IC=JSB->fdm->GetIC();
    IC->SetLatitudeDegIC(latDeg);
    IC->SetLongitudeDegIC(lonDeg);
    IC->SetAltitudeASLFtIC(altft);
    IC->SetPsiDegIC(hdgTrue);
    IC->SetVcalibratedKtsIC(kts);
    return true;
  }


  bool JSBSim_FlightLoop(void* ptr){
    auto* JSB=static_cast<JSBSim_FDM*>(ptr);    
    return JSB->fdm->Run();
  }

  dllExport bool JSBSim_SetPropertyValue(void* ptr, const uint32_t propertyID, double value){
    try{
      auto* JSB=static_cast<JSBSim_FDM*>(ptr);
      if(propertyID>=JSB->properties.size()){
        return false;
      }
      JSB->properties[propertyID]->setDoubleValue(value);
      return true;
    }
    catch(int err){
      cout<<"Error setting property: "<<err<<endl;
      return false;
    }
  }

  dllExport double JSBSim_GetPropertyValue(void* ptr, const uint32_t propertyID){
    try{
      auto* JSB=static_cast<JSBSim_FDM*>(ptr);
      if(propertyID>=JSB->properties.size()){
        return UINT32_MAX;
      }
      return JSB->properties[propertyID]->getDoubleValue();

    } catch(int err){
      cout<<"Error getting property: "<<err<<endl;
      return UINT32_MAX;
    }
  }

  //check if the requested property has already been seen and if not store it in a vector of property pointers for later use.
  // This is to avoid the overhead of searching for the property every time it is requested.
  // The vector of property pointers will be stored in the JSBSim_FDM struct and will be used to retrieve the property value directly.
  dllExport uint32_t JSBSim_PreFetch(void* ptr, const char* propertyPath){
    try      {
      auto* JSB=static_cast<JSBSim_FDM*>(ptr);
      auto it=JSB->propertyIDMap.find(propertyPath);

      //if we find the property, exit we already have it stored
      if(it!=JSB->propertyIDMap.end()){
        return it->second;
      }

      auto propertyManager=JSB->fdm->GetPropertyManager();
      auto node=propertyManager->GetNode(propertyPath, false);

      if(!node){
        return UINT32_MAX;
      }

 

      uint32_t index=static_cast<uint32_t>(JSB->properties.size());
      JSB->properties.push_back(node);
      JSB->propertyIDMap[propertyPath]=index;
      return index;
    }
    catch(...){
      //jsb throws an error when the node is not found, return UINT32_MAX to indicate that the property was not found
      return UINT32_MAX;
    }
   
  }

}