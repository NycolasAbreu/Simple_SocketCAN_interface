#include <iostream>
#include <unistd.h>
#include <memory>
#include <linux/can.h>

#include "CanMessage.h"
#include "CanDevice.h"
#include "CanDeviceFactory.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: <CAN interface> <CAN mode S send R read>" << "\n";
    return 1;
  }

  const std::string interface = argv[1];
  const std::string mode = argv[2];

  //std::unique_ptr<CanDevice> can = CanDeviceFactory::Create(CanDeviceFactory::ProtocolType::RAW, interface);
  std::unique_ptr<CanDevice> can = CanDeviceFactory::Create(CanDeviceFactory::ProtocolType::ISO_TP, interface, 0x111, 0x321);

  CanPrimitives::Filter filter {0x222, CAN_SFF_MASK};
  //can->AddFilter(filter);
  CanPrimitives::Filter filter2 {0x333, CAN_SFF_MASK};
  //can->AddFilter(filter2);

  can->Open();

  if (mode == "S" || mode == "s") {
    while (true)
    {
      const CanMessage::Data data{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
      const auto message = CanMessage(0x111, data);
      if (can->Send(message))
      {
        std::cout << "Sent CAN frame with ID: " << std::hex << message.GetCanID() << "\n";
        std::cout << "Data: ";
        for (const auto& byte : message.GetData())
        {
          std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << "\n";
      }

      sleep(2);
    }
  }
  else if (mode == "R" || mode == "r")
  {
    int i = 0;
    while (i < 10000)
    {
      auto message = CanMessage();
      can->Read(message);

      std::cout << "Read CAN frame with ID: " << std::hex << message.GetCanID() << "\n";
      std::cout << "Data: ";
      for (const auto& byte : message.GetData())
      {
        std::cout << std::hex << static_cast<int>(byte) << " ";
      }
      std::cout << "\n";

      if (i == 10)
      {
        can->RemoveFilter(filter2);
        can->RemoveFilter(filter);
      }

      i++;
    }
  }
}