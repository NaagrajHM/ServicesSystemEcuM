
#include "InterfaceServicesCommunicationDcm_ServicesSystemEcuM.hpp"
#include "InterfaceServicesCommunicationDcm_ServicesSystemSchM.hpp"
InterfaceClientServicesSystemEcuM* Init1_Boot[] = {
      InterfaceClientServicesSystemEcuM_ptrServicesCommunicationDcm
};
InterfaceClientServicesSystemEcuM* Init1_Appl[] = {
      InterfaceClientServicesSystemEcuM_ptrServicesCommunicationDcm
};
InterfaceClientServicesSystemSchM* ScheduleTable_Boot[] = {
      InterfaceClientServicesSystemSchM_ptrServicesCommunicationDcm
};
InterfaceClientServicesSystemSchM* ScheduleTable_Appl[] = {
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

enum EnumReasonResetEcuM{
      eReasonResetEcuM_RequestReprogramming = 0
   ,  eReasonResetEcuM_PowerOn
   ,  eReasonResetEcuM_Software
};
EnumReasonResetEcuM ServicesSystemEcuM_geReasonReset;
EnumModeEcuM        ServicesSystemEcuM_geModeCurrent;
EnumModeEcuM ServicesSystemEcuM_eGetModeCurrent(void){return ServicesSystemEcuM_geModeCurrent;}

#include <chrono>
chrono::steady_clock::time_point ServicesSystemEcuM_gTimeStartSystem;
void boot_vInit(void){
   ServicesSystemEcuM_gTimeStartSystem = chrono::steady_clock::now();
   ServicesSystemEcuM_geReasonReset    = eReasonResetEcuM_PowerOn;
   ServicesSystemEcuM_geModeCurrent    = eModeEcuM_Boot;
}

constexpr chrono::duration<double> ServicesSystemEcuM_gTimeout{10.0}; // 10s
bool ServicesSystemEcuM_gbIsValidApplication = true;
void ServicesSystemEcuM_vHandlerMode_Boot(void){
   for(auto Client : Init1_Boot){
      Client->vFunctionInit();
   }
   while(
         (
               (
                     eReasonResetEcuM_PowerOn
                  == ServicesSystemEcuM_geReasonReset
               )
            && (
                     ServicesSystemEcuM_gTimeout
                  >= (
                           chrono::steady_clock::now()
                        -  ServicesSystemEcuM_gTimeStartSystem
                     )
               )
         )
      || (
               (
                     eReasonResetEcuM_RequestReprogramming
                  == ServicesSystemEcuM_geReasonReset
               )
            && (
                     eStatusEcuM_RequestReprogramming
                  == InstanceStatusEcuM.eGetStatusEcuM()
               )
         )
      || (
               false
            == ServicesSystemEcuM_gbIsValidApplication
         )
   ){
      for(auto Thread : ScheduleTable_Boot){
         Thread->vFunctionMain();
      }
   }
   for(auto Client : Init1_Appl){
      Client->vFunctionDeInit();
   }
   if(
         (
               eReasonResetEcuM_PowerOn
            == ServicesSystemEcuM_geReasonReset
         )
      && (
               ServicesSystemEcuM_gTimeout
            <  (
                     chrono::steady_clock::now()
                  -  ServicesSystemEcuM_gTimeStartSystem
               )
         )
   ){
      ServicesSystemEcuM_geModeCurrent = eModeEcuM_Appl;
   }
   else if(
         (
               eReasonResetEcuM_RequestReprogramming
            == ServicesSystemEcuM_geReasonReset
         )
      && (
               eStatusEcuM_RequestResetAppl
            == InstanceStatusEcuM.eGetStatusEcuM()
         )
   ){
      InstanceStatusEcuM.vSetStatusEcuM(eStatusEcuM_Uninit);
      ServicesSystemEcuM_geReasonReset = eReasonResetEcuM_Software;
      ServicesSystemEcuM_geModeCurrent = eModeEcuM_Appl;
   }
}

void ServicesSystemEcuM_vHandlerMode_Appl(void){
   for(auto Client : Init1_Appl){
      Client->vFunctionInit();
   }
   InstanceStatusEcuM.vSetStatusEcuM(eStatusEcuM_Running);
   while(
         eStatusEcuM_Running
      == InstanceStatusEcuM.eGetStatusEcuM()
   ){
      for(auto Thread : ScheduleTable_Appl){
         Thread->vFunctionMain();
      }
   }
   for(auto Client : Init1_Appl){
      Client->vFunctionDeInit();
   }
   if(
         eStatusEcuM_RequestReprogramming
      == InstanceStatusEcuM.eGetStatusEcuM()
   ){
      ServicesSystemEcuM_geReasonReset = eReasonResetEcuM_RequestReprogramming;
      ServicesSystemEcuM_geModeCurrent = eModeEcuM_Boot;
   }
   else if(
         eStatusEcuM_InitShutdown
      == InstanceStatusEcuM.eGetStatusEcuM()
   ){
      InstanceStatusEcuM.vSetStatusEcuM(eStatusEcuM_Uninit);
      ServicesSystemEcuM_geModeCurrent = eModeEcuM_Shutdown;
   }
}

#include "InterfaceDeviceDriverEthernet_ServicesSystemEcuM.hpp"
#include "InterfaceDeviceDriverEthernet_ServicesSystemSchM.hpp"
void vThreadBackground(void){
   InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet->vFunctionInit();
   while(
         eModeEcuM_Shutdown
      != ServicesSystemEcuM_geModeCurrent
   ){
      InterfaceClientServicesSystemSchM_ptrDeviceDriverEthernet->vFunctionMain();
   }
}

#include <thread>
int main(void){
   thread ThreadBackground(vThreadBackground);
   boot_vInit();
   while(
         eModeEcuM_Shutdown
      != ServicesSystemEcuM_geModeCurrent
   ){
      switch(
         ServicesSystemEcuM_geModeCurrent
      ){
         case eModeEcuM_Boot: ServicesSystemEcuM_vHandlerMode_Boot(); break;
         case eModeEcuM_Appl: ServicesSystemEcuM_vHandlerMode_Appl(); break;
         default:                                                     break;
      }
   }
   InterfaceClientServicesSystemEcuM_ptrDeviceDriverEthernet->vFunctionDeInit(); // TBD: Ethernet de-initialization is not working from main/background thread.
   if(ThreadBackground.joinable()){
      ThreadBackground.join();
   }
   return 0;
}
