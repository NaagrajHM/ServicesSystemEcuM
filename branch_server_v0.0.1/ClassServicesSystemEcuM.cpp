
#include "InterfaceDeviceDriverEthernet_ServicesSystemEcuM.hpp"
#include "InterfaceServicesCommunicationDcm_ServicesSystemEcuM.hpp"

#include "InterfaceDeviceDriverEthernet_ServicesSystemSchM.hpp"
#include "InterfaceServicesCommunicationDcm_ServicesSystemSchM.hpp"

#define FOR_EVER_AND_EVER 1

InterfaceClientServicesSystemEcuM* Init1[] = {
      InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet
   ,  InterfaceClientServicesSystemEcuM_ptrServicesCommunicationDcm
};

InterfaceClientServicesSystemSchM* ScheduleTable[] = {
      InterfaceClientServicesSystemSchM_ptrDeviceDriverEthernet
   ,  InterfaceClientServicesSystemSchM_ptrServicesCommunicationDcm
};

int main(void){
   for(auto Client : Init1){
      Client->vFunctionInit();
   }

   while(
      FOR_EVER_AND_EVER
   ){
      for(auto Thread : ScheduleTable){
         Thread->vFunctionMain();
      }
   }

   return 0;
}
