// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <chrono>

#include "events/mbed_events.h"

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/services/HeartRateService.h"

#include "LogKit.h"
#include "PrettyPrinter.h"

const static auto DEVICE_NAME = std::array<char, 10> {"Heartrate"};
static events::EventQueue event_queue(/* event count */ 32 * EVENTS_EVENT_SIZE);

class HeartrateDemo : ble::Gap::EventHandler
{
  public:
	HeartrateDemo(BLE &ble, events::EventQueue &event_queue)
		: _ble(ble),
		  _event_queue(event_queue),
		  //   _led1(LED1, 1),
		  _connected(false),
		  _hr_uuid(GattService::UUID_HEART_RATE_SERVICE),
		  _hr_counter(100),
		  _hr_service(ble, _hr_counter, HeartRateService::LOCATION_FINGER),
		  _adv_data_builder(_adv_buffer.data(), std::size(_adv_buffer))
	{
	}

	void start()
	{
		_ble.gap().setEventHandler(this);

		_ble.init(this, &HeartrateDemo::onInitComplete);

		// _event_queue.call_every(500ms, this, &HeartrateDemo::blink);
		_event_queue.call_every(std::chrono::milliseconds(1000), this, &HeartrateDemo::update_sensor_value);

		// _event_queue.dispatch_forever();
		// printf("dispatch_forever\n");
	}

  private:
	/** Callback triggered when the ble initialization process has finished */
	void onInitComplete(BLE::InitializationCompleteCallbackContext *params)
	{
		if (params->error != BLE_ERROR_NONE) {
			log_error("Ble initialization failed.");
			return;
		}

		leka::ble::printMacAddress();

		startAdvertising();
	}

	void startAdvertising()
	{
		/* Create advertising parameters and payload */

		ble::AdvertisingParameters adv_parameters(ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
												  ble::adv_interval_t(ble::millisecond_t(1000)));

		_adv_data_builder.setFlags();
		_adv_data_builder.setAppearance(ble::adv_data_appearance_t::GENERIC_HEART_RATE_SENSOR);
		_adv_data_builder.setLocalServiceList(mbed::make_Span(&_hr_uuid, 1));
		_adv_data_builder.setName(DEVICE_NAME.data());

		/* Setup advertising */

		ble_error_t error = _ble.gap().setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, adv_parameters);

		if (error) {
			printf("_ble.gap().setAdvertisingParameters() failed\r\n");
			return;
		}

		error =
			_ble.gap().setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE, _adv_data_builder.getAdvertisingData());

		if (error) {
			printf("_ble.gap().setAdvertisingPayload() failed\r\n");
			return;
		}

		/* Start advertising */

		error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

		if (error) {
			printf("_ble.gap().startAdvertising() failed\r\n");
			return;
		}
	}

	void update_sensor_value()
	{
		if (_connected) {
			// Do blocking calls or whatever is necessary for sensor polling.
			// In our case, we simply update the HRM measurement.
			_hr_counter++;

			//  100 <= HRM bps <=175
			if (_hr_counter == 175) {
				_hr_counter = 100;
			}

			_hr_service.updateHeartRate(_hr_counter);
		}
	}

	// void blink(void) { _led1 = !_led1; }

  private:
	/* Event handler */

	void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &) override
	{
		_ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
		_connected = false;
	}

	void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override
	{
		if (event.getStatus() == BLE_ERROR_NONE) {
			_connected = true;
		}
	}

  private:
	BLE &_ble;
	events::EventQueue &_event_queue;

	bool _connected;
	UUID _hr_uuid;
	uint8_t _hr_counter;
	HeartRateService _hr_service;

	std::array<uint8_t, ble::LEGACY_ADVERTISING_MAX_SIZE> _adv_buffer;
	ble::AdvertisingDataBuilder _adv_data_builder;
};
