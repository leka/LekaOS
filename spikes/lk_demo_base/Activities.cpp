#include "Activities.h"

#include "rtos/ThisThread.h"

#include "Assets.h"
#include "CircularQueue.h"

using namespace leka;
using namespace std::chrono_literals;

auto get_random = [](uint32_t max_excluded_value) {
	return static_cast<int>(std::rand() / ((RAND_MAX + 1u) / max_excluded_value));
};

enum class Reinforcer
{
	green_spin				= static_cast<int>(Tag::reinforcer_1_green_spin),
	violet_green_blink_spin = static_cast<int>(Tag::reinforcer_2_violet_green_blink_spin),
	fire_static				= static_cast<int>(Tag::reinforcer_3_fire_static),
	glitters_static			= static_cast<int>(Tag::reinforcer_4_glitters_static),
	rainbow_static			= static_cast<int>(Tag::reinforcer_5_rainbow_static)
};
auto default_reinforcer = Reinforcer::rainbow_static;

auto hasToGoMainMenu(leka::Tag tag_value) -> bool
{
	return tag_value == Tag::remote_standard;
}

void runReinforcer(Reinforcer reinforcer, rtos::EventFlags &event_flags_external_interaction,
				   DisplayUtils &display_utils, LedsUtils &leds_utils, MotorsUtils &motors_utils)
{
	leds_utils.turnOff(LedsRange::all);
	display_utils.displayVideo("animation-joy");

	switch (reinforcer) {
		case Reinforcer::green_spin:
			motors_utils.spin(MotorsUtils::SpinDirection::Right);
			leds_utils.runReinforcer(LedsReinforcer::green);
			break;
		case Reinforcer::violet_green_blink_spin:
			motors_utils.spin(MotorsUtils::SpinDirection::Left);
			leds_utils.runReinforcer(LedsReinforcer::violet_green_blink);
			break;
		case Reinforcer::fire_static:
			leds_utils.runReinforcer(LedsReinforcer::fire);
			break;
		case Reinforcer::glitters_static:
			leds_utils.runReinforcer(LedsReinforcer::glitters);
			break;
		case Reinforcer::rainbow_static:
			leds_utils.runReinforcer(LedsReinforcer::rainbow);
			break;
		default:
			rtos::ThisThread::sleep_for(200ms);
	}

	motors_utils.stop();
	leds_utils.turnOff(LedsRange::all);
	event_flags_external_interaction.wait_any(END_OF_VIDEO_FLAG);
	event_flags_external_interaction.set(STOP_VIDEO_FLAG);
}

void leka::displayTags(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
					   LedsUtils &leds_utils, RFIDUtils &rfid_utils)
{
	while (true) {
		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

		if (auto tag_value = rfid_utils.getTag(); !hasToGoMainMenu(rfid_utils.getTag())) {
			display_utils.displayImage(rfid_image_table.at(static_cast<uint8_t>(tag_value) - 1));
		} else {
			return;
		}
	}
}

void leka::activityRecognitionColor(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
									LedsUtils &leds_utils, MotorsUtils &motors_utils, RFIDUtils &rfid_utils)
{
	int pos = 0;

	constexpr auto amount_of_different_colors = 4;
	CircularQueue<int, amount_of_different_colors / 2> last_color_displayed {};
	last_color_displayed.push(-1);
	CircularQueue<int, object_colored_family_table.size() / 2> last_family_displayed {};
	last_family_displayed.push(-1);
	auto expected_tag_color = Tag::None;

	auto random_color  = 0;
	auto random_family = 0;

	std::string last_image = "NaN";

	while (true) {
		do {
			random_color = get_random(amount_of_different_colors);
			pos			 = 0;
		} while (last_color_displayed.hasPattern(&random_color, 1, pos));
		last_color_displayed.push(random_color);

		do {
			random_family = get_random(object_colored_family_table.size());
			pos			  = 0;
		} while (last_family_displayed.hasPattern(&random_family, 1, pos));
		last_family_displayed.push(random_family);

		switch (random_color) {
			case 0:
				last_image		   = object_colored_family_table.at(random_family) + "-red";
				expected_tag_color = Tag::color_red;
				break;
			case 1:
				last_image		   = object_colored_family_table.at(random_family) + "-blue";
				expected_tag_color = Tag::color_blue;
				break;
			case 2:
				last_image		   = object_colored_family_table.at(random_family) + "-green";
				expected_tag_color = Tag::color_green;
				break;
			case 3:
				last_image		   = object_colored_family_table.at(random_family) + "-yellow";
				expected_tag_color = Tag::color_yellow;
				break;
			default:
				break;
		}

		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		do {
			display_utils.displayImage(last_image);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);
		} while ((rfid_utils.getTag() != expected_tag_color) && !hasToGoMainMenu(rfid_utils.getTag()));

		if (hasToGoMainMenu(rfid_utils.getTag())) {
			return;
		}

		runReinforcer(default_reinforcer, event_flags_external_interaction, display_utils, leds_utils, motors_utils);
	}
}

void leka::activityRecognitionEmotions(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
									   LedsUtils &leds_utils, MotorsUtils &motors_utils, RFIDUtils &rfid_utils)
{
	int pos = 0;

	CircularQueue<int, emotions_table.size() / 2> last_emotions_displayed {};
	last_emotions_displayed.push(-1);
	auto expected_tag_emotion_child = Tag::None;
	auto expected_tag_emotion_leka	= Tag::None;

	auto random_emotion = 0;
	auto tag_value		= Tag::None;

	std::string last_image = "NaN";

	while (true) {
		do {
			random_emotion = get_random(emotions_table.size());
			pos			   = 0;
		} while (last_emotions_displayed.hasPattern(&random_emotion, 1, pos));
		last_emotions_displayed.push(random_emotion);

		last_image = emotions_table.at(random_emotion);

		switch (random_emotion) {
			case 0:
				expected_tag_emotion_child = Tag::emotion_happiness_child;
				expected_tag_emotion_leka  = Tag::emotion_happiness_leka;
				break;
			case 1:
				expected_tag_emotion_child = Tag::emotion_anger_child;
				expected_tag_emotion_leka  = Tag::emotion_anger_leka;
				break;
			case 2:
				expected_tag_emotion_child = Tag::emotion_fear_child;
				expected_tag_emotion_leka  = Tag::emotion_fear_leka;
				break;
			case 3:
				expected_tag_emotion_child = Tag::emotion_disgust_child;
				expected_tag_emotion_leka  = Tag::emotion_disgust_leka;
				break;
			case 4:
				expected_tag_emotion_child = Tag::emotion_sadness_child;
				expected_tag_emotion_leka  = Tag::emotion_sadness_leka;
				break;
			default:
				break;
		}

		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		do {
			display_utils.displayImage(last_image);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

			tag_value = rfid_utils.getTag();
		} while ((tag_value != expected_tag_emotion_child) && (tag_value != expected_tag_emotion_leka) &&
				 !hasToGoMainMenu(tag_value));

		if (hasToGoMainMenu(tag_value)) {
			return;
		}

		runReinforcer(default_reinforcer, event_flags_external_interaction, display_utils, leds_utils, motors_utils);
	}
}

void leka::activityRecognitionNumbers(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
									  LedsUtils &leds_utils, MotorsUtils &motors_utils, RFIDUtils &rfid_utils)
{
	int pos = 0;

	CircularQueue<int, numbers_table.size() / 2> last_numbers_displayed {};
	last_numbers_displayed.push(-1);
	auto expected_tag_number = Tag::None;

	auto random_number = 0;

	std::string last_image = "NaN";

	while (true) {
		do {
			random_number = get_random(numbers_table.size());
			pos			  = 0;
		} while (last_numbers_displayed.hasPattern(&random_number, 1, pos));
		last_numbers_displayed.push(random_number);

		last_image			= numbers_table.at(random_number);
		expected_tag_number = Tag(static_cast<uint8_t>(Tag::number_0_zero) + random_number);

		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		do {
			display_utils.displayImage(last_image);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);
		} while ((rfid_utils.getTag() != expected_tag_number) && !hasToGoMainMenu(rfid_utils.getTag()));

		if (hasToGoMainMenu(rfid_utils.getTag())) {
			return;
		}

		runReinforcer(default_reinforcer, event_flags_external_interaction, display_utils, leds_utils, motors_utils);
	}
}

void leka::activityRecognitionShapes(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
									 LedsUtils &leds_utils, MotorsUtils &motors_utils, RFIDUtils &rfid_utils)
{
	int pos = 0;

	CircularQueue<int, shapes_table.size() / 2> last_shapes_displayed {};
	last_shapes_displayed.push(-1);
	auto expected_tag_shape = Tag::None;

	auto random_shape = 0;

	std::string last_image = "NaN";

	while (true) {
		do {
			random_shape = get_random(shapes_table.size());
			pos			 = 0;
		} while (last_shapes_displayed.hasPattern(&random_shape, 1, pos));
		last_shapes_displayed.push(random_shape);

		last_image		   = shapes_table.at(random_shape);
		expected_tag_shape = Tag(static_cast<uint8_t>(Tag::shape_circle_plain_line) + random_shape);

		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		do {
			display_utils.displayImage(last_image);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);
		} while ((rfid_utils.getTag() != expected_tag_shape) && !hasToGoMainMenu(rfid_utils.getTag()));

		if (hasToGoMainMenu(rfid_utils.getTag())) {
			return;
		}

		runReinforcer(default_reinforcer, event_flags_external_interaction, display_utils, leds_utils, motors_utils);
	}
}

void leka::displayEmotions(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
						   LedsUtils &leds_utils, RFIDUtils &rfid_utils)
{
	while (true) {
		display_utils.displayImage("robot-emotion-neutral");

		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

		auto tag_value = rfid_utils.getTag();
		if (hasToGoMainMenu(tag_value)) {
			return;
		}
		switch (tag_value) {
			case Tag::emotion_happiness_leka:
				display_utils.displayImage("robot-emotion-happy");
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::emotion_sadness_leka:
				display_utils.displayImage("robot-emotion-sad_tears");
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::emotion_fear_leka:
				display_utils.displayImage("robot-emotion-affraid");
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::emotion_anger_leka:
				display_utils.displayImage("robot-emotion-angry");
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::emotion_disgust_leka:
				display_utils.displayImage("robot-emotion-disgusted");
				rtos::ThisThread::sleep_for(4s);
				break;
			default:
				rtos::ThisThread::sleep_for(200ms);
		}
	}
}

void leka::activityColorLeka1(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
							  LedsUtils &leds_utils, RFIDUtils &rfid_utils)
{
	auto updateFace = [&]() { display_utils.displayImage("robot-emotion-happy"); };

	while (true) {
		display_utils.displayImage("robot-emotion-neutral");
		leds_utils.turnOff(LedsRange::all);

		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

		auto tag_value = rfid_utils.getTag();
		if (hasToGoMainMenu(tag_value)) {
			return;
		}

		switch (tag_value) {
			case Tag::color_red:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Red);
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::color_green:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Green);
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::color_blue:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Blue);
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::color_yellow:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Yellow);
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::color_white:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::White);
				rtos::ThisThread::sleep_for(4s);
				break;
			case Tag::color_black:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Black);
				rtos::ThisThread::sleep_for(4s);
				break;
			default:
				rtos::ThisThread::sleep_for(200ms);
		}
	}
}

void leka::activityColorLeka2(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
							  LedsUtils &leds_utils, RFIDUtils &rfid_utils)
{
	auto is_happy = true;
	display_utils.displayImage("robot-emotion-neutral");

	auto updateFace = [&display_utils, &is_happy]() {
		if (is_happy) {
			display_utils.displayImage("robot-emotion-happy");
		} else {
			display_utils.displayImage("robot-emotion-neutral");
		}
		is_happy = !is_happy;
	};

	while (true) {
		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
		event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

		auto tag_value = rfid_utils.getTag();
		if (hasToGoMainMenu(tag_value)) {
			return;
		}
		switch (tag_value) {
			case Tag::color_red:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Red);
				break;
			case Tag::color_green:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Green);
				break;
			case Tag::color_blue:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Blue);
				break;
			case Tag::color_yellow:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Yellow);
				break;
			case Tag::color_white:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::White);
				break;
			case Tag::color_black:
				updateFace();
				leds_utils.turnOn(LedsRange::all, CRGB::Black);
				break;
			default:
				break;
		}

		rtos::ThisThread::sleep_for(500ms);
	}
}

void leka::permanentRun(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
						LedsUtils &leds_utils, RFIDUtils &rfid_utils)
{
	display_utils.displayVideo("animation-idle");

	while (true) {
		event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

		if (hasToGoMainMenu(rfid_utils.getTag())) {
			event_flags_external_interaction.set(STOP_VIDEO_FLAG);
			return;
		}

		leds_utils.pulsation();
	}
}

void leka::twoNumbersAdditionCalculator(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
										LedsUtils &leds_utils, MotorsUtils &motors_utils, RFIDUtils &rfid_utils)
{
	int pos = 0;

	CircularQueue<int, numbers_table.size() / 2> last_numbers_displayed {};
	last_numbers_displayed.push(-1);

	auto convertToRealNumber = [](Tag tag_number) {
		return static_cast<int>(tag_number) - static_cast<int>(Tag::number_0_zero);
	};

	auto random_number = 0;

	std::string last_image = "NaN";

	rtos::ThisThread::sleep_for(2s);

	while (true) {
		auto actual_tag_number_1 = Tag::None;
		auto actual_tag_number_2 = Tag::None;

		do {
			random_number = get_random(numbers_table.size() - 1) + 1;
			pos			  = 0;
		} while (last_numbers_displayed.hasPattern(&random_number, 1, pos));
		last_numbers_displayed.push(random_number);

		last_image = numbers_table.at(random_number);
		leds_utils.turnOff(LedsRange::all);

		while (convertToRealNumber(actual_tag_number_1) < 0 || convertToRealNumber(actual_tag_number_1) > 10) {
			display_utils.displayImage(last_image);
			event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

			actual_tag_number_1 = rfid_utils.getTag();
			if (hasToGoMainMenu(actual_tag_number_1)) {
				return;
			}
		}
		leds_utils.turnOn(LedsRange::front_right | LedsRange::back_right | LedsRange::ear_right, CRGB::White);

		rtos::ThisThread::sleep_for(2s);

		while (convertToRealNumber(actual_tag_number_2) < 0 || convertToRealNumber(actual_tag_number_2) > 10) {
			display_utils.displayImage(last_image);
			leds_utils.turnOn(LedsRange::front_right | LedsRange::back_right | LedsRange::ear_right, CRGB::White);
			event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

			actual_tag_number_2 = rfid_utils.getTag();
			if (hasToGoMainMenu(actual_tag_number_2)) {
				return;
			}
		}
		leds_utils.turnOn(LedsRange::front_left | LedsRange::back_left | LedsRange::ear_left, CRGB::White);

		rtos::ThisThread::sleep_for(1500ms);

		auto sum = convertToRealNumber(actual_tag_number_1) + convertToRealNumber(actual_tag_number_2);

		if (sum == random_number) {
			runReinforcer(default_reinforcer, event_flags_external_interaction, display_utils, leds_utils,
						  motors_utils);
		}
	}
}

void leka::selectReinforcer(rtos::EventFlags &event_flags_external_interaction, DisplayUtils &display_utils,
							LedsUtils &leds_utils, MotorsUtils &motors_utils, RFIDUtils &rfid_utils)
{
	auto convertToReinforcerID = [](Tag tag_number) {
		return static_cast<int>(tag_number) - static_cast<int>(Tag::reinforcer_1_green_spin) + 1;
	};
	auto tag_value		= Tag::None;
	auto last_tag_value = tag_value;

	while (true) {
		do {
			display_utils.displayImage("robot-emotion-neutral");
			leds_utils.turnOff(LedsRange::all);

			event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);

			tag_value = rfid_utils.getTag();
			if (hasToGoMainMenu(tag_value)) {
				return;
			}
		} while (convertToReinforcerID(tag_value) < convertToReinforcerID(Tag::reinforcer_1_green_spin) ||
				 convertToReinforcerID(tag_value) > convertToReinforcerID(Tag::reinforcer_5_rainbow_static));

		runReinforcer(Reinforcer(tag_value), event_flags_external_interaction, display_utils, leds_utils, motors_utils);

		if (last_tag_value == tag_value) {
			default_reinforcer = Reinforcer(tag_value);
			display_utils.displayImage(rfid_image_table.at(static_cast<uint8_t>(tag_value) - 1));

			rtos::ThisThread::sleep_for(4s);
		}
		last_tag_value = tag_value;
	}
}

void leka::activityRecognitionColoredForms(rtos::EventFlags &event_flags_external_interaction,
										   DisplayUtils &display_utils, LedsUtils &leds_utils,
										   MotorsUtils &motors_utils, RFIDUtils &rfid_utils)
{
	int pos = 0;

	auto tagIsAColor = [](Tag tag) { return (tag >= Tag::color_red) && (tag <= Tag::color_yellow); };
	auto tagIsAShape = [](Tag tag) { return (tag <= Tag::shape_star_plain_line); };

	constexpr auto amount_of_different_colors = 4;
	CircularQueue<int, amount_of_different_colors / 2> last_color_displayed {};
	last_color_displayed.push(-1);
	CircularQueue<int, shapes_to_color_table.size() / 2> last_shape_to_color_displayed {};
	last_shape_to_color_displayed.push(-1);

	auto expected_tag_color = Tag::None;
	auto expected_tag_shape = Tag::None;
	auto actual_tag_color	= Tag::None;
	auto actual_tag_shape	= Tag::None;
	auto actual_tag			= Tag::None;

	auto random_color		   = 0;
	auto random_shape_to_color = 0;

	std::string last_image = "NaN";

	while (true) {
		do {
			random_color = get_random(amount_of_different_colors);
			pos			 = 0;
		} while (last_color_displayed.hasPattern(&random_color, 1, pos));
		last_color_displayed.push(random_color);

		do {
			random_shape_to_color = get_random(shapes_to_color_table.size());
			pos					  = 0;
		} while (last_shape_to_color_displayed.hasPattern(&random_shape_to_color, 1, pos));
		last_shape_to_color_displayed.push(random_shape_to_color);

		expected_tag_shape = Tag(random_shape_to_color + static_cast<int>(Tag::shape_circle_plain_line));
		switch (random_color) {
			case 0:
				last_image		   = shapes_to_color_table.at(random_shape_to_color) + "-red";
				expected_tag_color = Tag::color_red;
				break;
			case 1:
				last_image		   = shapes_to_color_table.at(random_shape_to_color) + "-blue";
				expected_tag_color = Tag::color_blue;
				break;
			case 2:
				last_image		   = shapes_to_color_table.at(random_shape_to_color) + "-green";
				expected_tag_color = Tag::color_green;
				break;
			case 3:
				last_image		   = shapes_to_color_table.at(random_shape_to_color) + "-yellow";
				expected_tag_color = Tag::color_yellow;
				break;
			default:
				break;
		}
		leds_utils.turnOff(LedsRange::all);

		event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
		do {
			display_utils.displayImage(last_image);
			event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
			event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);
			actual_tag = rfid_utils.getTag();
			// } while ((rfid_utils.getTag() != expected_tag_color) && !hasToGoMainMenu(rfid_utils.getTag()));
		} while (!tagIsAColor(actual_tag) && !tagIsAShape(actual_tag) && !hasToGoMainMenu(actual_tag));

		if (hasToGoMainMenu(actual_tag)) {
			return;
		}

		leds_utils.turnOn(LedsRange::front_right | LedsRange::back_right | LedsRange::ear_right, CRGB::White);

		rtos::ThisThread::sleep_for(100ms);

		if (tagIsAColor(actual_tag)) {
			actual_tag_color = actual_tag;

			event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
			do {
				display_utils.displayImage(last_image);
				leds_utils.turnOn(LedsRange::front_right | LedsRange::back_right | LedsRange::ear_right, CRGB::White);
				event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
				event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);
				actual_tag = rfid_utils.getTag();
			} while (!tagIsAShape(actual_tag) && !hasToGoMainMenu(actual_tag));
		} else if (tagIsAShape(actual_tag)) {
			actual_tag_shape = actual_tag;

			event_flags_external_interaction.clear(NEW_RFID_TAG_FLAG);
			do {
				display_utils.displayImage(last_image);
				leds_utils.turnOn(LedsRange::front_right | LedsRange::back_right | LedsRange::ear_right, CRGB::White);
				event_flags_external_interaction.wait_any(NEW_RFID_TAG_FLAG);
				event_flags_external_interaction.set(KICK_TURN_OFF_FLAG | KICK_WAITING_FLAG);
				actual_tag = rfid_utils.getTag();
			} while (!tagIsAColor(actual_tag) && !hasToGoMainMenu(actual_tag));
		}

		if (hasToGoMainMenu(actual_tag)) {
			return;
		}

		if (tagIsAColor(actual_tag)) {
			actual_tag_color = actual_tag;
		} else if (tagIsAShape(actual_tag)) {
			actual_tag_shape = actual_tag;
		}

		leds_utils.turnOn(LedsRange::front_left | LedsRange::back_left | LedsRange::ear_left, CRGB::White);

		rtos::ThisThread::sleep_for(2s);

		if ((expected_tag_color == actual_tag_color) && (expected_tag_shape == actual_tag_shape)) {
			runReinforcer(default_reinforcer, event_flags_external_interaction, display_utils, leds_utils,
						  motors_utils);
		}
	}
}
