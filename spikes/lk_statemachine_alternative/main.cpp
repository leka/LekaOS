// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "EventFlags.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "PointerStateMachine.h"
#include "SingletonStateMachine.h"
#include "VariantStateMachine.h"

using namespace leka;
using namespace std::chrono;

auto thread_pointer_state_machine	= rtos::Thread {};
auto thread_singleton_state_machine = rtos::Thread {};
auto thread_variant_state_machine	= rtos::Thread {};

auto event_flags_pointer_state_machine	 = rtos::EventFlags {};
auto event_flags_singleton_state_machine = rtos::EventFlags {};
auto event_flags_variant_state_machine	 = rtos::EventFlags {};

auto pointer_state_machine	 = PointerStateMachine {event_flags_pointer_state_machine};
auto singleton_state_machine = SingletonStateMachine {event_flags_singleton_state_machine};
auto variant_state_machine	 = VariantStateMachine {event_flags_variant_state_machine};

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	thread_pointer_state_machine.start({&pointer_state_machine, &PointerStateMachine::StateLoop});
	thread_singleton_state_machine.start({&singleton_state_machine, &SingletonStateMachine::StateLoop});
	thread_variant_state_machine.start({&variant_state_machine, &VariantStateMachine::StateLoop});

	while (true) {
		rtos::ThisThread::sleep_for(10s);

		event_flags_pointer_state_machine.set(static_cast<uint32_t>(Events::TIMEOUT));
		event_flags_singleton_state_machine.set(static_cast<uint32_t>(Events::TIMEOUT));
		event_flags_variant_state_machine.set(static_cast<uint32_t>(Events::TIMEOUT));

		log_info("Pointer state: %s", pointer_state_machine.getState());
		log_info("Singleton state: %s", singleton_state_machine.getState().data());
		log_info("Variant state: %s", variant_state_machine.getState().data());

		rtos::ThisThread::sleep_for(3s);

		event_flags_pointer_state_machine.set(static_cast<uint32_t>(Events::WAKEUP));
		event_flags_singleton_state_machine.set(static_cast<uint32_t>(Events::WAKEUP));
		event_flags_variant_state_machine.set(static_cast<uint32_t>(Events::WAKEUP));

		log_info("Pointer state: %s", pointer_state_machine.getState());
		log_info("Singleton state: %s", singleton_state_machine.getState().data());
		log_info("Variant state: %s", variant_state_machine.getState().data());
	}
}
