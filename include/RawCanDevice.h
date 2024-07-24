#pragma once

#include <string>

#include "CanDevice.h"

class RawCanDevice : public CanDevice
{
	public:
	  RawCanDevice() = delete;
	  explicit RawCanDevice(const std::string& interface);
	  ~RawCanDevice() override = default;

	  auto Open() -> bool override;
	  auto Send(const CanMessage &message, int timeout_ms = -1) -> size_t override;
	  auto Read(CanMessage& message, int timeout_ms = -1) -> bool override;
};
