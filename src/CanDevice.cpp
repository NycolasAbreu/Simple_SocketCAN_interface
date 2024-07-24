#include "CanDevice.h"

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>

CanDevice::CanDevice(const std::string& interface)
  : interface(interface)
{
}

CanDevice::~CanDevice()
{
  close(socket_fd);
}

auto CanDevice::Close() -> bool
{
  if (socket_fd == CanPrimitives::INVALID_SOCKET)
  {
    return true;
  }

  if (close(socket_fd) == 0)
  {
    socket_fd = CanPrimitives::INVALID_SOCKET;
    return true;
  }

  return false;
}

auto CanDevice::IsOpen() const -> bool
{
  return socket_fd != CanPrimitives::INVALID_SOCKET;
}

auto CanDevice::AddFilter(const CanPrimitives::Filter &filter) -> void
{
  filters.push_back(filter);
  ApplyFilters();
}

auto CanDevice::RemoveFilter(const CanPrimitives::Filter &filter) -> void
{
  const auto itr = std::find(filters.begin(), filters.end(), filter);
  filters.erase(itr);
  ApplyFilters();
}

auto CanDevice::ApplyFilters() const -> void
{
  if (IsOpen()) {
    if (filters.empty()) {
      constexpr CanPrimitives::Filter all_pass_filter = {0, 0};
      setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &all_pass_filter, sizeof(all_pass_filter));
    }
    else if (setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FILTER, filters.data(), sizeof(filters[0]) * filters.size()) == -1) {
      // TODO: Manage error
    }
  }
}
