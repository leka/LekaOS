// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <events/mbed_events.h>
#include <mbed.h>

#include "ble/BLE.h"

#include "PrettyPrinter.h"

bool TEST_DIFFERENT_MOVEMENTS	   = false;
bool TEST_PROGRESSIVE_ACCELERATION = false;

static EventQueue event_queue(/* event count */ 10 * EVENTS_EVENT_SIZE);

const static char DEVICE_NAME[] = "LekaDev";

class MotorsService
{
  public:
	const static uint16_t MOTORS_SERVICE_UUID										= 0xA000;
	const static uint16_t MOTORS_DIFFERENT_MOVEMENTS_STATE_CHARACTERISTIC_UUID		= 0xA001;
	const static uint16_t MOTORS_PROGRESSIVE_ACCELERATION_STATE_CHARACTERISTIC_UUID = 0xA002;

	MotorsService(BLE &_ble, bool initialValueForMotorsCharacteristics)
		: ble(_ble),
		  motors_different_movements_state(MOTORS_DIFFERENT_MOVEMENTS_STATE_CHARACTERISTIC_UUID,
										   &initialValueForMotorsCharacteristics),
		  motors_progressive_acceleration_state(MOTORS_PROGRESSIVE_ACCELERATION_STATE_CHARACTERISTIC_UUID,
												&initialValueForMotorsCharacteristics)
	{
		GattCharacteristic *charTable[] = {&motors_different_movements_state, &motors_progressive_acceleration_state};
		GattService motorsService(MOTORS_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

		ble.gattServer().addService(motorsService);
	}

	GattAttribute::Handle_t getDifferentMovementsValueHandle() const
	{
		return motors_different_movements_state.getValueHandle();
	}
	GattAttribute::Handle_t getProgressiveAccelerationValueHandle() const
	{
		return motors_progressive_acceleration_state.getValueHandle();
	}

  private:
	BLE &ble;
	ReadWriteGattCharacteristic<bool> motors_different_movements_state;
	ReadWriteGattCharacteristic<bool> motors_progressive_acceleration_state;
};

class MotorsBLEProgram : ble::Gap::EventHandler
{
  public:
	MotorsBLEProgram(BLE &ble, events::EventQueue &event_queue)
		: _ble(ble),
		  _event_queue(event_queue),
		  _motors_service_uuid(MotorsService::MOTORS_SERVICE_UUID),
		  _adv_data_builder(_adv_buffer)
	{
	}

	virtual ~MotorsBLEProgram() = default;

	void start()
	{
		_ble.gap().setEventHandler(this);

		_ble.init(this, &MotorsBLEProgram::onInitComplete);

		// _event_queue.call_every(500, this, &MotorsBLEProgram::blink);

		// _event_queue.dispatch_forever();
	}

	bool motorsShouldRun() { return _should_run; }

  private:
	/** Callback triggered when the ble initialization process has finished */
	void onInitComplete(BLE::InitializationCompleteCallbackContext *params)
	{
		if (params->error != BLE_ERROR_NONE) {
			printf("Ble initialization failed.");
			return;
		}

		_motors_service = make_unique<MotorsService>(_ble, false);

		_ble.gattServer().onDataWritten(this, &MotorsBLEProgram::onDataWritten);

		leka::ble::printMacAddress();

		startAdvertising();
	}

	void startAdvertising()
	{
		/* Create advertising parameters and payload */

		ble::AdvertisingParameters adv_parameters(ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
												  ble::adv_interval_t(ble::millisecond_t(1000)));

		_adv_data_builder.setFlags();
		_adv_data_builder.setLocalServiceList(mbed::make_Span(&_motors_service_uuid, 1));
		_adv_data_builder.setName(DEVICE_NAME);

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

	/**
	 * This callback allows the MotorsService to receive updates to the motors_different_movements_state Characteristic.
	 *
	 * @param[in] params Information about the characterisitc being updated.
	 */
	void onDataWritten(const GattWriteCallbackParams *params)
	{
		if ((params->handle == _motors_service->getDifferentMovementsValueHandle()) && (params->len == 1)) {
			_should_run				 = *(params->data);
			TEST_DIFFERENT_MOVEMENTS = _should_run;
		}
		if ((params->handle == _motors_service->getProgressiveAccelerationValueHandle()) && (params->len == 1)) {
			_should_run					  = *(params->data);
			TEST_PROGRESSIVE_ACCELERATION = _should_run;
		}
	}

	/* Event handler */

	void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &) final
	{
		_ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
	}

	BLE &_ble;
	events::EventQueue &_event_queue;

	bool _should_run = false;

	UUID _motors_service_uuid;
	unique_ptr<MotorsService> _motors_service;

	uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
	ble::AdvertisingDataBuilder _adv_data_builder;
};
