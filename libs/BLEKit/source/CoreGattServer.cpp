#include "CoreGattServer.h"

using namespace leka;
using namespace std::chrono_literals;

void CoreGattServer::setServer()
{
	setEventHandler();
	registerServerWrite();
	setServices();
}

void CoreGattServer::setEventHandler()
{
	_gatt_server.setEventHandler(&_gatt_server_event_handler);
}

void CoreGattServer::registerServerWrite()
{
	// _gatt_server_event_handler.registerServerWrite(std::bind(&CoreGattServer::serverWrite, this,
	// 														 std::placeholders::_1,
	// 														 std::placeholders::_2,
	// 														 std::placeholders::_3));

	_gatt_server_event_handler.registerServerWrite(
		[this](GattAttribute::Handle_t &&PH1, const uint8_t *&&PH2, uint16_t &&PH3) {
			serverWrite(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2),
						std::forward<decltype(PH3)>(PH3));
		});
}

void CoreGattServer::setServices()
{
	for (auto &service: _services) {
		_gatt_server.addService(*service);
	}
	_gatt_server_event_handler.setServices(_services);
}

void CoreGattServer::updateData()
{
	_gatt_server_event_handler.updateData();
}

void CoreGattServer::serverWrite(GattAttribute::Handle_t characteristic_updated, const uint8_t *data,
								 uint16_t n_data_bytes)
{
	_gatt_server.write(characteristic_updated, data, n_data_bytes);
}
