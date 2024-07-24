#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

class CanMessage;

namespace CanPrimitives
{
	constexpr int INVALID_SOCKET = -1;

	struct Filter {
		uint32_t id;
		uint32_t mask;

		bool operator==(const Filter& other) const {
			return id == other.id && mask == other.mask;
		}
	};
}

class CanDevice
{
	public:
		CanDevice() = delete;
		explicit CanDevice(const std::string& interface);
    virtual ~CanDevice();

    virtual auto Open() -> bool = 0;
		auto Close() -> bool;
		[[nodiscard]] auto IsOpen() const -> bool;

		virtual auto Send(const CanMessage& message, int timeout_ms = -1) -> size_t = 0;
		virtual auto Read(CanMessage& message, int timeout_ms = -1) -> bool = 0;

		auto AddFilter(const CanPrimitives::Filter& filter) -> void;
		auto RemoveFilter(const CanPrimitives::Filter& filter) -> void;

	protected:
		auto ApplyFilters() const -> void;

	  int socket_fd = CanPrimitives::INVALID_SOCKET;
		const std::string interface;

	private:
		std::vector<CanPrimitives::Filter> filters;
};
