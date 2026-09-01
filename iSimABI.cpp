#include "iSimABI.h"
#include "FGFDMExec.h"


extern "C" {

  struct JSBSim_FDM{
    std::unique_ptr<JSBSim::FGFDMExec> fdm;
    std::shared_ptr<JSBSim::FGFCS> fcs;
  };


  JSBSIM_API void* JSBSim_Create(){
    auto* handle=new JSBSim_FDM;

    handle->fdm=std::make_unique<JSBSim::FGFDMExec>();
    handle->fcs=handle->fdm->GetFCS();

    return handle;
  }

  JSBSIM_API void JSBSim_Destroy(void* ptr){
    delete static_cast<JSBSim_FDM*>(ptr);
  }

  JSBSIM_API bool JSBSim_LoadModel(void* ptr, const char* modelPath){

    auto* JSB=static_cast<JSBSim_FDM*>(ptr);

    return JSB->fdm->LoadModel(modelPath);
  }

  JSBSIM_API bool JSBSim_FirstRun(void* ptr){
    auto* JSB=static_cast<JSBSim_FDM*>(ptr);
    return JSB->fdm->RunIC();
  }

  JSBSIM_API bool JSBSim_FlightLoop(void* ptr){
    auto* JSB=static_cast<JSBSim_FDM*>(ptr);
    return JSB->fdm->Run();
  }



}