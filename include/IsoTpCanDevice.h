#pragma once

#include <string>

#include "CanDevice.h"

class IsoTpCanDevice : public CanDevice
{
  public:
    IsoTpCanDevice() = delete;
    explicit IsoTpCanDevice(const std::string& interface, uint32_t tx_id, uint32_t rx_id);
    ~IsoTpCanDevice() override = default;

    auto Open() -> bool override;
    auto Send(const CanMessage &message, int timeout_ms = -1) -> size_t override;
    auto Read(CanMessage& message, int timeout_ms = -1) -> bool override;

  private:
    uint32_t tx_id;
    uint32_t rx_id;
};
