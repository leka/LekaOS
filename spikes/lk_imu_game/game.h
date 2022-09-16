// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "LogKit.h"
#include "motion.h"

using namespace std::chrono;

namespace motion {
namespace game {
	auto index_			 = 0;
	auto game_launch	 = false;
	const auto MAX_SCORE = 5;
	const auto MIN_SCORE = 0;
	auto score			 = 0;
	const auto MAX_PV	 = 3;
	const auto MIN_PV	 = 0;
	auto lives			 = MAX_PV;
	auto interrupt_flag	 = false;
	std::string name_curr_pic_shape;
	std::string desired_shape_pic;

	auto displayValidatedShape(auto index_, auto *videokit) -> void
	{
		videokit->displayImage("/fs/home/img/shapesRecognition/Game/Shape_did_validated_plus.jpg");
		videokit->displayImage("/fs/home/img/shapesRecognition/Shapes/" +
							   static_cast<std::string>(motion::shape::shapes_array.at(index_)->getImageName()) +
							   ".jpg");
		rtos::ThisThread::sleep_for(2s);
	}

	auto displayNotValidatedShape(auto name_curr_shape_, auto *videokit) -> void
	{
		videokit->displayImage("/fs/home/img/shapesRecognition/Game/Shape_did_not_validated.jpg");
		videokit->displayImage("/fs/home/img/shapesRecognition/Shapes/" + name_curr_shape_ + ".jpg");
		rtos::ThisThread::sleep_for(2s);
	}

	auto displayScoreImage(auto score, auto *videokit) -> void
	{
		std::array<std::string, MAX_SCORE> score_img_name = {"Score_one", "Score_one", "Score_three", "Score_four",
															 "Score_five"};

		if (score == MAX_SCORE) {
			videokit->displayImage("/fs/home/img/shapesRecognition/Game/" + score_img_name[score] + ".jpg");
			rtos::ThisThread::sleep_for(2s);
			videokit->displayImage("/fs/home/img/shapesRecognition/Game/congratulations.jpg");
		} else {
			videokit->displayImage("/fs/home/img/shapesRecognition/Game/" + score_img_name.at(score) + ".jpg");
		}

		rtos::ThisThread::sleep_for(2s);
	}

	auto displayHeartsImage(auto lives, auto videokit) -> void
	{
		std::array<std::string, 4> lives_img_name = {"zero_hearts", "one_hearts", "two_hearts", "three_hearts"};

		if (lives == 0) {
			videokit->displayImage("/fs/home/img/shapesRecognition/Game/" + lives_img_name[0] + ".jpg");
			rtos::ThisThread::sleep_for(2s);
			videokit->displayImage("/fs/home/img/shapesRecognition/Game/game_over.jpg");
		} else {
			videokit->displayImage("/fs/home/img/shapesRecognition/Game/" + lives_img_name.at(lives) + ".jpg");
		}

		rtos::ThisThread::sleep_for(2s);
	}

	auto successShape(auto *leds, auto color, auto *corevideo, auto &videokit, auto &id_card) -> void
	{
		log_debug("felicitation");
		interrupt_flag = false;
		score++;
		leds->setColor(color);
		leds->show();
		corevideo->clearScreen();
		displayValidatedShape(index_, &videokit);
		displayScoreImage(score, &videokit);
		id_card = 0;   // reset id_card
		videokit.displayImage("/fs/home/img/shapesRecognition/Game/select_shape_to_do.jpg");
		rtos::ThisThread::sleep_for(2s);
	}

	auto failedShape(auto *leds, auto color, auto *corevideo, auto &videokit) -> void
	{
		leds->setColor(color);
		leds->show();
		lives--;
		corevideo->clearScreen();
		displayNotValidatedShape(name_curr_pic_shape, &videokit);
		displayHeartsImage(lives, &videokit);
		rtos::ThisThread::sleep_for(2s);
	}

	auto resetGame(auto &id_card)
	{
		interrupt_flag = false;
		game_launch	   = false;
		score		   = MIN_SCORE;
		lives		   = MAX_PV;
		id_card		   = 0;
	}
}	// namespace game
}	// namespace motion