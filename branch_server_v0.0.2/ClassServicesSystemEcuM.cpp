
#include "InterfaceServicesCommunicationDcm_ServicesSystemEcuM.hpp"
#include "InterfaceServicesCommunicationDcm_ServicesSystemSchM.hpp"
InterfaceClientServicesSystemEcuM* Init1[] = {
      InterfaceClientServicesSystemEcuM_ptrServicesCommunicationDcm
};
InterfaceClientServicesSystemSchM* ScheduleTable[] = {
      InterfaceClientServicesSystemSchM_ptrServicesCommunicationDcm
};


#include "InterfaceServicesSystemEcuM_ServicesCommunicationDcm.hpp"
#include <atomic>
using namespace std;
class ClassStatusEcuM : public InterfaceServicesSystemEcuM_ServicesCommunicationDcm{
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
InterfaceServicesSystemEcuM_ServicesCommunicationDcm* InterfaceServicesSystemEcuM_ServicesCommunicationDcm_ptr = &InstanceStatusEcuM;

#include "InterfaceDeviceDriverEthernet_ServicesSystemEcuM.hpp"
#include "InterfaceDeviceDriverEthernet_ServicesSystemSchM.hpp"
void vThreadBackground(void){
   InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet->vFunctionInit();
   while(
         eStatusEcuM_Running
      == InstanceStatusEcuM.eGetStatusEcuM()
   ){
      InterfaceClientServicesSystemSchM_ptrDeviceDriverEthernet->vFunctionMain();
   }
// InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet->vFunctionDeInit(); // TBD: Ethernet de-initialization is not working from main/background thread.
}

#include <thread>
int main(void){
   for(auto Client : Init1){
      Client->vFunctionInit();
   }
   InstanceStatusEcuM.vSetStatusEcuM(eStatusEcuM_Running);
   thread ThreadBackground(vThreadBackground);
   while(
         eStatusEcuM_Running
      == InstanceStatusEcuM.eGetStatusEcuM()
   ){
      for(auto Thread : ScheduleTable){
         Thread->vFunctionMain();
      }
   }
   for(auto Client : Init1){
      Client->vFunctionDeInit();
   }
   InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet->vFunctionDeInit(); // TBD: Ethernet de-initialization is not working from main/background thread.
   if(ThreadBackground.joinable()){
      ThreadBackground.join();
   }
   return 0;
}
