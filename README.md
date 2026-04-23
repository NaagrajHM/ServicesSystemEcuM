# ServicesSystemEcuM
Repository for EcuM.

# 2026.04.22: branch_server_v0.0.5
Feature:
      #3 Bootloader gets application-like personality.
   ,  #4 Driver initialization lists separated for application and bootloader.
   ,  #5 Separate schedule table for application and bootloader.
   ,  #6 Implement state-machine to switch between application and bootloader without disconnecting ethernet.

# 2026.04.21: branch_server_v0.0.4
Feature: #2 Introduce power-on reset timeout in bootloader.

# 2026.04.20: branch_server_v0.0.3
Feature: #1 Introduce dummy bootloader.

# 2026.04.03: branch_server_v0.0.2
Feature:
      EcuM status for shutdown request implementation 
   ,  Multi-thread implementation for ethernet rx driver
   ,  De-Initialize base software modules before initiating shutdown request.

Known issue:
      Exit application upon shutdown request completion.

# 2026.04.02: branch_server_v0.0.1
Feature: modular development.

