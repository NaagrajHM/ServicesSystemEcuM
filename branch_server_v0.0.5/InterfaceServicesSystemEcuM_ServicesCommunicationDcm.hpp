#pragma once

enum EnumStatusEcuM{
      eStatusEcuM_Uninit = 0
// ,  eStatusEcuM_InitDrivers1
   ,  eStatusEcuM_Running
   ,  eStatusEcuM_RequestReprogramming
   ,  eStatusEcuM_RequestResetAppl
   ,  eStatusEcuM_InitShutdown
   ,  eStatusEcuM_Error
};

class InterfaceServicesSystemEcuM_ServicesCommunicationDcm{
   public:
      virtual               ~InterfaceServicesSystemEcuM_ServicesCommunicationDcm ()                            = default;
      virtual void           vSetStatusEcuM                                       (EnumStatusEcuM leStatusEcuM) = 0;
      virtual EnumStatusEcuM eGetStatusEcuM                                       (void) const                  = 0;
};

extern InterfaceServicesSystemEcuM_ServicesCommunicationDcm* InterfaceServicesSystemEcuM_ServicesCommunicationDcm_ptr;

//TBD: Remove temporary solution!
enum EnumModeEcuM{
      eModeEcuM_Boot = 0
   ,  eModeEcuM_Appl
   ,  eModeEcuM_Shutdown
   ,  eModeEcuM_Error
};
extern EnumModeEcuM ServicesSystemEcuM_eGetModeCurrent(void);
