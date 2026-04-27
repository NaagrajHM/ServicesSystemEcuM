#include "InterfaceClientDeviceDriverEthernet.hpp"

enum EnumModeEcuM{
      eModeEcuM_Boot = 0
   ,  eModeEcuM_Appl
   ,  eModeEcuM_Shutdown
   ,  eModeEcuM_Error
};
EnumModeEcuM ServicesSystemEcuM_geModeCurrent;

#include "InterfaceServicesSystemEcuM_DeviceDriverEthernet.hpp"
#include <atomic>
using namespace std;
class ClassStatusEcuM : public InterfaceServicesSystemEcuM_DeviceDriverEthernet{
   private:
      atomic<EnumStatusEcuM> eStatusEcuM = eStatusEcuM_Uninit;
   public:
      ClassStatusEcuM(EnumStatusEcuM leStatusEcuM){
         eStatusEcuM = leStatusEcuM;
      }
      void vSetStatusEcuM(EnumStatusEcuM leStatusEcuM){
         eStatusEcuM.store(leStatusEcuM);
      }
      EnumStatusEcuM eGetStatusEcuM(void) const{
         return eStatusEcuM.load();
      }
};
ClassStatusEcuM InstanceStatusEcuM(eStatusEcuM_Uninit);
InterfaceServicesSystemEcuM_DeviceDriverEthernet* InterfaceServicesSystemEcuM_DeviceDriverEthernet_ptr = &InstanceStatusEcuM;

#include "InterfaceDeviceDriverEthernet_ServicesSystemSchM.hpp"
void ServicesSystemEcuM_vHandlerMode_Appl(void){
   InstanceStatusEcuM.vSetStatusEcuM(eStatusEcuM_Running);
   while(
         eStatusEcuM_Running
      == InstanceStatusEcuM.eGetStatusEcuM()
   ){
      InterfaceClientServicesSystemSchM_ptrDeviceDriverEthernet->vFunctionMain();
   }
   if(
         eStatusEcuM_InitShutdown
      == InstanceStatusEcuM.eGetStatusEcuM()
   ){
      InstanceStatusEcuM.vSetStatusEcuM(eStatusEcuM_Uninit);
      ServicesSystemEcuM_geModeCurrent = eModeEcuM_Shutdown;
   }
}

#include "InterfaceDeviceDriverEthernet_ServicesSystemEcuM.hpp"
int main(
      int   argc
   ,  char* argv[]
){
   if(argc < 2){
      cerr << "Error: No IP address provided." << endl;
      cout << "Usage: " << argv[0] << " <IP_ADDRESS>" << endl;
      return 1;
   }
   else{
      ServicesSystemEcuM_geModeCurrent = eModeEcuM_Boot;
      stringAddressIP = argv[1]; // Error: assignment needs attention!
      InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet->vFunctionInit();
      while(
            eModeEcuM_Shutdown
         != ServicesSystemEcuM_geModeCurrent
      ){
         ServicesSystemEcuM_vHandlerMode_Appl();
      }
      InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet->vFunctionDeInit();
      return 0;
   }
}
