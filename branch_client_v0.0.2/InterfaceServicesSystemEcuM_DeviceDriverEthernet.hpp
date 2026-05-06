enum EnumStatusEcuM{
      eStatusEcuM_Uninit = 0
// ,  eStatusEcuM_InitDrivers1
   ,  eStatusEcuM_Running
   ,  eStatusEcuM_RequestReprogramming
   ,  eStatusEcuM_RequestResetAppl
   ,  eStatusEcuM_InitShutdown
   ,  eStatusEcuM_Error
};

class InterfaceServicesSystemEcuM_DeviceDriverEthernet{
   public:
      virtual               ~InterfaceServicesSystemEcuM_DeviceDriverEthernet ()                            = default;
      virtual void           vSetStatusEcuM                                   (EnumStatusEcuM leStatusEcuM) = 0;
      virtual EnumStatusEcuM eGetStatusEcuM                                   (void) const                  = 0;
};

extern InterfaceServicesSystemEcuM_DeviceDriverEthernet* InterfaceServicesSystemEcuM_DeviceDriverEthernet_ptr;

