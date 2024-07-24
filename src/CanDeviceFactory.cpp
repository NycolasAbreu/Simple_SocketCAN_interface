#include <CanDeviceFactory.h>

#include "RawCanDevice.h"
#include "IsoTpCanDevice.h"

std::unique_ptr<CanDevice> CanDeviceFactory::Create(
  ProtocolType type,
  const std::string& interface_name,
  uint32_t tx_id,
  uint32_t rx_id
)
{
  switch (type) {
    case ProtocolType::RAW:
      return std::make_unique<RawCanDevice>(interface_name);
    case ProtocolType::ISO_TP:
      return std::make_unique<IsoTpCanDevice>(interface_name, tx_id, rx_id);
    default:
      return nullptr;
  }
}
