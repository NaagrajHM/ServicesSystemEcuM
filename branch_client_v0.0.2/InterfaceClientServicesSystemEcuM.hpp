#pragma once

class InterfaceClientServicesSystemEcuM{
   public:
      virtual     ~InterfaceClientServicesSystemEcuM ()     = default;
      virtual void vFunctionInit                     (void) = 0;
      virtual void vFunctionDeInit                   (void) = 0;
};
