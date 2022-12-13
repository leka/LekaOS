// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../Accelerometer.h"

using namespace leka::mock;

Accelerometer::Accelerometer() {}
Accelerometer::~Accelerometer() {}

#include "../Battery.h"

Battery::Battery() {}
Battery::~Battery() {}

#include "../BLEService.h"

BLEService::BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics)
	: interface::BLEService(uuid, characteristics)
{
	// nothing to do
}

BLEService::~BLEService() {}

// #include "../CGAnimation.h"

// CGAnimation::CGAnimation() {}
// CGAnimation::~CGAnimation() {}

#include "../CoreBufferedSerial.h"

CoreBufferedSerial::CoreBufferedSerial() {}
CoreBufferedSerial::~CoreBufferedSerial() {}

#include "../CoreDMA2D.h"

CoreDMA2D::CoreDMA2D() {}
CoreDMA2D::~CoreDMA2D() {}

#include "../CoreDSI.h"

CoreDSI::CoreDSI() {}
CoreDSI::~CoreDSI() {}

#include "../CoreFont.h"

CoreFont::CoreFont() {}
CoreFont::~CoreFont() {}

#include "../CoreGraphics.h"

CoreGraphics::CoreGraphics() {}
CoreGraphics::~CoreGraphics() {}

#include "../CoreI2C.h"

CoreI2C::CoreI2C() {}
CoreI2C::~CoreI2C() {}

#include "../CoreJPEG.h"

CoreJPEG::CoreJPEG() {}
CoreJPEG::~CoreJPEG() {}

#include "../CoreJPEGMode.h"

CoreJPEGMode::CoreJPEGMode() {}
CoreJPEGMode::~CoreJPEGMode() {}

#include "../CoreLCD.h"

CoreLCD::CoreLCD() {}
CoreLCD::~CoreLCD() {}

#include "../CoreLCDDriver.h"

CoreLCDDriver::CoreLCDDriver() {}
CoreLCDDriver::~CoreLCDDriver() {}

#include "../CoreLED.h"

CoreLED::CoreLED() {}
CoreLED::~CoreLED() {}

// #include "../CoreLL.h"

// CoreLL::CoreLL() {}
// CoreLL::~CoreLL() {}

#include "../CoreLTDC.h"

CoreLTDC::CoreLTDC() {}
CoreLTDC::~CoreLTDC() {}

#include "../CoreMotor.h"

CoreMotor::CoreMotor() {}
CoreMotor::~CoreMotor() {}

#include "../CoreQDAC.h"

CoreQDAC::CoreQDAC() {}
CoreQDAC::~CoreQDAC() {}

#include "../CoreRFIDReader.h"

CoreRFIDReader::CoreRFIDReader() {}
CoreRFIDReader::~CoreRFIDReader() {}

#include "../CoreSDRAM.h"

CoreSDRAM::CoreSDRAM() {}
CoreSDRAM::~CoreSDRAM() {}

#include "../CoreSTM32Hal.h"

CoreSTM32Hal::CoreSTM32Hal() {}
CoreSTM32Hal::~CoreSTM32Hal() {}

#include "../CoreTouchSensor.h"

CoreTouchSensor::CoreTouchSensor() {}
CoreTouchSensor::~CoreTouchSensor() {}

#include "../CoreVideo.h"

CoreVideo::CoreVideo() {}
CoreVideo::~CoreVideo() {}

#include "../EventFlags.h"

EventFlags::EventFlags() {}
EventFlags::~EventFlags() {}

#include "../EventQueue.h"

EventQueue::EventQueue()
{
	equeue_stub.void_ptr = &ptr;

	spy_CoreEventQueue_on_dispatch_forever_call = [] { equeue_stub.call_cb_immediately = true; };
	spy_EventQueue_on_dispatch_forever_call		= [] { equeue_stub.call_cb_immediately = true; };
}

EventQueue::~EventQueue()
{
	equeue_stub.void_ptr			= nullptr;
	equeue_stub.call_cb_immediately = false;
}

#include "../File.h"

File::File() {}
File::~File() {}

#include "../FirmwareUpdate.h"

FirmwareUpdate::FirmwareUpdate() {}
FirmwareUpdate::~FirmwareUpdate() {}

#include "../FlashManager.h"

FlashManager::FlashManager() {}
FlashManager::~FlashManager() {}

#include "../FlashMemory.h"

FlashMemory::FlashMemory() {}
FlashMemory::~FlashMemory() {}

#include "../Gyroscope.h"

Gyroscope::Gyroscope() {}
Gyroscope::~Gyroscope() {}

// #include "../IOExpander.h"

// IOExpander::IOExpander() {}
// IOExpander::~IOExpander() {}

#include "../LEDAnimation.h"

LEDAnimation::LEDAnimation() {}
LEDAnimation::~LEDAnimation() {}

#include "../LSM6DSOX.h"

LSM6DSOX::LSM6DSOX() {}
LSM6DSOX::~LSM6DSOX() {}

#include "../MCU.h"

MCU::MCU() {}
MCU::~MCU() {}

#include "../Network.h"

Network::Network() {}
Network::~Network() {}

#include "../PwmOut.h"

PwmOut::PwmOut() {}
PwmOut::~PwmOut() {}

#include "../QSPI.h"

QSPI::QSPI() {}
QSPI::~QSPI() {}

#include "../SPI.h"

SPI::SPI() {}
SPI::~SPI() {}

#include "../Ticker.h"

Ticker::Ticker() {}
Ticker::~Ticker() {}

#include "../Timeout.h"

Timeout::Timeout() {}
Timeout::~Timeout() {}

#include "../VideoKit.h"

VideoKit::VideoKit() {}
VideoKit::~VideoKit() {}
