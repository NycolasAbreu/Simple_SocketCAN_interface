#include "RawCanDevice.h"

#include <algorithm>
#include <cstring>
#include <linux/can.h>
#include <net/if.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "CanMessage.h"
#include "CanDevice.h"

//TODO: Add error handling

RawCanDevice::RawCanDevice(const std::string& interface)
  : CanDevice(interface)
{
}

auto RawCanDevice::Open() -> bool
{
  socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (socket_fd < 0) 
  {
    return false;
  }

  ifreq interface_request {};
  std::strcpy(interface_request.ifr_name, interface.c_str());
  if (ioctl(socket_fd, SIOCGIFINDEX, &interface_request) < 0)
  {
    Close();
    return false;
  }

  sockaddr_can addr {};
  addr.can_family = AF_CAN;
  addr.can_ifindex = interface_request.ifr_ifindex;
  if (bind(socket_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
  {
    Close();
    return false;
  }

  ApplyFilters();

  return true;
}

auto RawCanDevice::Send(const CanMessage& message, int /*timeout_ms*/) -> size_t
{
  can_frame frame = {};
  frame.can_id = message.GetCanID();
  constexpr size_t MAX_DATA_LENGTH = CAN_MAX_DLC;
  frame.can_dlc = static_cast<unsigned char>(std::min(message.GetData().size(), MAX_DATA_LENGTH));
  std::copy_n(message.GetData().begin(), frame.can_dlc, frame.data);

  if (write(socket_fd, &frame, sizeof(frame)) != sizeof(frame) < 0)
  {
    return false;
  }

  return true;
}

auto RawCanDevice::Read(CanMessage& message, int timeout_ms) -> bool 
{ 
  pollfd poll_fd {};
  poll_fd.fd = socket_fd;
  poll_fd.events = POLLIN;

  if (poll(&poll_fd, 1, timeout_ms) <= 0)
  {
    return false;
  }

  can_frame frame {};
  if (read(socket_fd, &frame, sizeof(frame)) > 0)
  {
    CanMessage::Data data {};
    data.reserve(frame.can_dlc);
    std::copy_n(std::begin(frame.data), frame.can_dlc, std::back_inserter(data));
    message.SetData(data);
    message.SetCanID(frame.can_id);

    return true;
  }

  return false;
}
