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

		static auto callback_send_data = [this](const interface::BLEService::data_to_send_handle_t &handle) {
			const auto &[h, d, s] = handle;
			write(h, d, s);
		};

		service->onDataReadyToSend(callback_send_data);
	}
	_gatt_server_event_handler.setServices(services);
}

void CoreGattServer::write(GattAttribute::Handle_t characteristic_updated, const uint8_t *data, uint16_t n_data_bytes)
{
	_gatt_server.write(characteristic_updated, data, n_data_bytes);
}
