// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "animations/AfraidBlue.h"
#include "animations/AfraidRed.h"
#include "animations/AfraidRedBlue.h"
#include "animations/Amazed.h"
#include "animations/Angry.h"
#include "animations/AngryShort.h"
#include "animations/BleConnection.h"
#include "animations/BlinkGreen.h"
#include "animations/Bubbles.h"
#include "animations/Disgusted.h"
#include "animations/Fire.h"
#include "animations/Fly.h"
#include "animations/Happy.h"
#include "animations/Rainbow.h"
#include "animations/Sad.h"
#include "animations/SadCry.h"
#include "animations/Sick.h"
#include "animations/Singing.h"
#include "animations/Sleeping.h"
#include "animations/Sneeze.h"
#include "animations/SpinBlink.h"
#include "animations/Sprinkles.h"
#include "animations/Underwater.h"
#include "animations/WakeUp.h"
#include "animations/Wink.h"
#include "animations/Yawn.h"

namespace leka::led::animation {

static inline auto afraid_blue	   = led::animation::AfraidBlue {};
static inline auto afraid_red	   = led::animation::AfraidRed {};
static inline auto afraid_red_blue = led::animation::AfraidRedBlue {};
static inline auto amazed		   = led::animation::Amazed {};
static inline auto angry		   = led::animation::Angry {};
static inline auto angry_short	   = led::animation::AngryShort {};
static inline auto blink_green	   = led::animation::BlinkGreen {};
static inline auto bubbles		   = led::animation::Bubbles {};
static inline auto ble_connection  = led::animation::BleConnection {};
static inline auto disgusted	   = led::animation::Disgusted {};
static inline auto fire			   = led::animation::Fire {};
static inline auto fly			   = led::animation::Fly {};
static inline auto happy		   = led::animation::Happy {};
static inline auto rainbow		   = led::animation::Rainbow {};
static inline auto sad			   = led::animation::Sad {};
static inline auto sad_cry		   = led::animation::SadCry {};
static inline auto sick			   = led::animation::Sick {};
static inline auto singing		   = led::animation::Singing {};
static inline auto sleeping		   = led::animation::Sleeping {};
static inline auto sneeze		   = led::animation::Sneeze {};
static inline auto spin_blink	   = led::animation::SpinBlink {};
static inline auto sprinkles	   = led::animation::Sprinkles {};
static inline auto underwater	   = led::animation::Underwater {};
static inline auto wake_up		   = led::animation::WakeUp {};
static inline auto wink			   = led::animation::Wink {};
static inline auto yawn			   = led::animation::Yawn {};

};	 // namespace leka::led::animation
