/**
******************************************************************************
* @file    LekaBLE.h
* @author  Yann Locatelli
* @brief   BLE operational test code
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __LEKABLE_H__
#define __LEKABLE_H__

#include "mbed.h"
#include <events/mbed_events.h>

#include "ble/BLE.h"

#include "PrettyPrinter.h"

const static char DEVICE_NAME[] = "LekaBeacon";
static events::EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

/** Schedule processing of events from the BLE middleware in the event queue. */
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
	event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

class BeaconDemo : ble::Gap::EventHandler
{
  public:
	BeaconDemo(BLE &ble, events::EventQueue &event_queue)
		: _ble(ble), _event_queue(event_queue), _adv_data_builder(_adv_buffer)
	{
	}

	void start()
	{
		_ble.gap().setEventHandler(this);

		_ble.init(this, &BeaconDemo::on_init_complete);

		_event_queue.dispatch_forever();
	}

  private:
	/**
	 * iBeacon payload builder.
	 *
	 * This data structure contains the payload of an iBeacon. The payload is
	 * built at construction time and application code can set up an iBeacon by
	 * injecting the raw field into the GAP advertising payload as a
	 * GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA.
	 */
	union Payload {
		/**
		 * Raw data of the payload.
		 */
		uint8_t raw[25];
		struct {
			/**
			 * Beacon manufacturer identifier.
			 */
			uint16_t companyID;

			/**
			 * Packet ID; Equal to 2 for an iBeacon.
			 */
			uint8_t ID;

			/**
			 * Length of the remaining data presents in the payload.
			 */
			uint8_t len;

			/**
			 * Beacon UUID.
			 */
			uint8_t proximityUUID[16];

			/**
			 * Beacon Major group ID.
			 */
			uint16_t majorNumber;

			/**
			 * Beacon minor ID.
			 */
			uint16_t minorNumber;

			/**
			 * Tx power received at 1 meter; in dBm.
			 */
			uint8_t txPower;
		};

		/**
		 * Assemble an iBeacon payload.
		 *
		 * @param[in] uuid Beacon network ID. iBeacon operators use this value
		 * to group their iBeacons into a single network, a single region and
		 * identify their organization among others.
		 *
		 * @param[in] majNum Beacon major group ID. iBeacon exploitants may use
		 * this field to divide the region into subregions, their network into
		 * subnetworks.
		 *
		 * @param[in] minNum Identifier of the Beacon in its subregion.
		 *
		 * @param[in] transmitPower Measured transmit power of the beacon at 1
		 * meter. Scanners use this parameter to approximate the distance
		 * to the beacon.
		 *
		 * @param[in] companyIDIn ID of the beacon manufacturer.
		 */
		Payload(const uint8_t *uuid, uint16_t majNum, uint16_t minNum, uint8_t transmitPower, uint16_t companyIDIn)
			: companyID(companyIDIn),
			  ID(0x02),
			  len(0x15),
			  majorNumber(__REV16(majNum)),
			  minorNumber(__REV16(minNum)),
			  txPower(transmitPower)
		{
			memcpy(proximityUUID, uuid, 16);
		}
	};

	/** Callback triggered when the ble initialization process has finished */
	void on_init_complete(BLE::InitializationCompleteCallbackContext *params)
	{
		if (params->error != BLE_ERROR_NONE) {
			printf("Ble initialization failed.");
			return;
		}

		leka::ble::printMacAddress();

		start_advertising();
	}

	void start_advertising()
	{
		/* Create advertising parameters and payload */

		ble::AdvertisingParameters adv_parameters(ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
												  ble::adv_interval_t(ble::millisecond_t(1000)));

		_adv_data_builder.setFlags();
		_adv_data_builder.setName(DEVICE_NAME);

		/**
		 * The Beacon payload has the following composition:
		 * 128-Bit / 16byte UUID = E2 0A 39 F4 73 F5 4B C4 A1 2F 17 D1 AD 07 A9 61
		 * Major/Minor  = 0x1122 / 0x3344
		 * Tx Power     = 0xC8 = 200, 2's compliment is 256-200 = (-56dB)
		 *
		 * Note: please remember to calibrate your beacons TX Power for more accurate results.
		 */
		static const uint8_t uuid[] = {0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4,
									   0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61};
		uint16_t major_number		= 1122;
		uint16_t minor_number		= 3344;
		uint16_t tx_power			= 0xC8;
		uint16_t comp_id			= 0x004C;

		Payload ibeacon(uuid, major_number, minor_number, tx_power, comp_id);

		_adv_data_builder.setManufacturerSpecificData(ibeacon.raw);

		/* Setup advertising */

		ble_error_t error = _ble.gap().setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, adv_parameters);

		if (error) {
			leka::ble::printError(error, "_ble.gap().setAdvertisingParameters() failed");
			return;
		}

		error =
			_ble.gap().setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE, _adv_data_builder.getAdvertisingData());

		if (error) {
			leka::ble::printError(error, "_ble.gap().setAdvertisingPayload() failed");
			return;
		}

		/* Start advertising */

		error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

		if (error) {
			leka::ble::printError(error, "_ble.gap().startAdvertising() failed");
			return;
		}
	}

	/* Event handler */

	void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &) final
	{
		_ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
	}

	BLE &_ble;
	events::EventQueue &_event_queue;

	uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
	ble::AdvertisingDataBuilder _adv_data_builder;
};

#endif	 // __LEKABLE_H__
