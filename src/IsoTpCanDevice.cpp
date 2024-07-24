#include "IsoTpCanDevice.h"

#include <cstring>
#include <linux/can.h>
#include <net/if.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "CanMessage.h"

//TODO: Add error handling
//TODO: Add options from isotp protocol

IsoTpCanDevice::IsoTpCanDevice(const std::string& interface, uint32_t tx_id, uint32_t rx_id)
  : CanDevice(interface)
  , tx_id(tx_id)
  , rx_id(rx_id)
{
}

auto IsoTpCanDevice::Open() -> bool
{
  socket_fd = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
  if (socket_fd < 0) {
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
  addr.can_addr.tp.tx_id = tx_id;
  addr.can_addr.tp.rx_id = rx_id;
  if (bind(socket_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
  {
    Close();
    return false;
  }

  ApplyFilters();

  return true;
}

auto IsoTpCanDevice::Send(const CanMessage& message, int timeout_ms) -> size_t
{
  pollfd poll_fd {};
  poll_fd.fd = socket_fd;
  poll_fd.events = POLLOUT;

  if (poll(&poll_fd, 1, timeout_ms) <= 0)
  {
    return false;
  }

  if (write(socket_fd, message.GetData().data(), message.GetData().size()) < 0)
  {
    return false;
  }

  return true;
}

//TODO: Not working correctly, maybe data format is wrong
auto IsoTpCanDevice::Read(CanMessage& message, int timeout_ms) -> bool
{
  pollfd poll_fd {};
  poll_fd.fd = socket_fd;
  poll_fd.events = POLLIN;

  if (poll(&poll_fd, 1, timeout_ms) <= 0)
  {
    return false;
  }

  constexpr uint16_t max_isotp_data_lenght = 4095;
  CanMessage::Data buffer(max_isotp_data_lenght);
  if (read(socket_fd, buffer.data(), buffer.size()) > 0)
  {
    message.SetCanID(rx_id);
    message.SetData(buffer);
    return true;
  }

  return false;
}
