#pragma once

#include <memory>

class CanDevice;

class CanDeviceFactory 
{
  public:
    enum class ProtocolType 
    {
      RAW,
      ISO_TP
    };

    static std::unique_ptr<CanDevice> Create(
      ProtocolType type,
      const std::string& interface_name,
      uint32_t tx_id = 0,
      uint32_t rx_id = 0
    );
};