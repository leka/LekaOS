#include "CoreGattServer.h"

using namespace leka;
using namespace std::chrono_literals;

void CoreGattServer::setEventHandler()
{
	_gatt_server.setEventHandler(&_gatt_server_event_handler);
}

void CoreGattServer::setServices(std::span<interface::BLEService *> services)
{
	for (auto &service: services) {
		_gatt_server.addService(*service);

		service->onDataReadyToSend([this](const interface::BLEService::data_to_send_handle_t &handle) {
			const auto &[h, d] = handle;
			write(h, d);
		});
	}

	_gatt_server_event_handler.setServices(services);
}

void CoreGattServer::write(GattAttribute::Handle_t characteristic_updated, std::span<const uint8_t> data)
{
	_gatt_server.write(characteristic_updated, data.data(), static_cast<uint16_t>(std::size(data)));
}
