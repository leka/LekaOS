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

		service->onDataReadyToSend([this](GattAttribute::Handle_t &&PH1, const uint8_t *&&PH2, uint16_t &&PH3) {
			write(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), std::forward<decltype(PH3)>(PH3));
		});
	}
	_gatt_server_event_handler.setServices(services);
}

void CoreGattServer::write(GattAttribute::Handle_t characteristic_updated, const uint8_t *data, uint16_t n_data_bytes)
{
	_gatt_server.write(characteristic_updated, data, n_data_bytes);
}
