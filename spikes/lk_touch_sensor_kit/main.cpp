// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreEventFlags.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"
#include "TouchSensorKit.h"

using namespace leka;
using namespace std::chrono;

namespace leds {

namespace internal {

	namespace ears {

		auto spi			= CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
		constexpr auto size = 2;

	}	// namespace ears

	namespace belt {

		auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
		constexpr auto size = 20;

	}	// namespace belt

	namespace animations {

		auto event_loop = EventLoopKit {};

	}	// namespace animations

}	// namespace internal

auto ears		   = CoreLED<internal::ears::size> {internal::ears::spi};
auto belt		   = CoreLED<internal::belt::size> {internal::belt::spi};
auto random_number = uint8_t {0};
auto win		   = bool {false};

auto random8(uint8_t min, uint8_t max) -> uint8_t
{
	return min + rand() % (max - min + 1);
}
void turnOn()
{
	ears.setColor(RGB::pure_blue);
	belt.setColor(RGB::pure_blue);
	ears.show();
	belt.show();
}
void turnOff()
{
	ears.setColor(RGB::black);
	belt.setColor(RGB::black);
	ears.show();
	belt.show();
}
void turnOnEarLeft(RGB color)
{
	ears.setColorAtIndex(1, color);
	ears.show();
}
void turnOffEarLeft()
{
	ears.setColorAtIndex(1, RGB::black);
	ears.show();
}
void turnOnEarRight(RGB color)
{
	ears.setColorAtIndex(0, color);
	ears.show();
}
void turnOffEarRight()
{
	ears.setColorAtIndex(0, RGB::black);
	ears.show();
}
void turnOnBeltRightBack(RGB color)
{
	belt.setColorAtIndex(1, color);
	belt.setColorAtIndex(2, color);
	belt.setColorAtIndex(3, color);
	belt.show();
}
void turnOffBeltRightBack()
{
	belt.setColorAtIndex(1, RGB::black);
	belt.setColorAtIndex(2, RGB::black);
	belt.setColorAtIndex(3, RGB::black);
	belt.show();
}
void turnOnBeltRightFront(RGB color)
{
	belt.setColorAtIndex(6, color);
	belt.setColorAtIndex(7, color);
	belt.setColorAtIndex(8, color);
	belt.setColorAtIndex(9, color);
	belt.show();
}
void turnOffBeltRightFront()
{
	belt.setColorAtIndex(6, RGB::black);
	belt.setColorAtIndex(7, RGB::black);
	belt.setColorAtIndex(8, RGB::black);
	belt.setColorAtIndex(9, RGB::black);
	belt.show();
}
void turnOnBeltLeftBack(RGB color)
{
	belt.setColorAtIndex(10, color);
	belt.setColorAtIndex(11, color);
	belt.setColorAtIndex(12, color);
	belt.setColorAtIndex(13, color);
	belt.show();
}
void turnOffBeltLeftBack()
{
	belt.setColorAtIndex(10, RGB::black);
	belt.setColorAtIndex(11, RGB::black);
	belt.setColorAtIndex(12, RGB::black);
	belt.setColorAtIndex(13, RGB::black);
	belt.show();
}
void turnOnBeltLeftFront(RGB color)
{
	belt.setColorAtIndex(16, color);
	belt.setColorAtIndex(17, color);
	belt.setColorAtIndex(18, color);
	belt.show();
}
void turnOffBeltLeftFront()
{
	belt.setColorAtIndex(16, RGB::black);
	belt.setColorAtIndex(17, RGB::black);
	belt.setColorAtIndex(18, RGB::black);
	belt.show();
}
// Cette fonction va setter un des 6 groupes de leds en bleu
void setLedsWithRandomColors()
{
	// On considère que les leds bleues devront être touchées
	constexpr RGB targetColor = RGB::pure_blue;
	// win est une variable globale qui représente le fait de réussir à toucher un groupe de leds bleues
	win = false;
	// l'ensemble des leds sont éteintes avant de resetter un nouveau groupe de leds en bleu
	turnOff();
	// en fonction du nombre tiré au sort on sette un groupe  de leds entre 0 et 5
	random_number = random8(0, 5);
	switch (random_number) {
		case 0:
			turnOnEarLeft(targetColor);
			break;

		case 1:
			turnOnEarRight(targetColor);
			break;

		case 2:
			turnOnBeltRightBack(targetColor);
			break;

		case 3:
			turnOnBeltRightFront(targetColor);
			break;

		case 4:
			turnOnBeltLeftBack(targetColor);
			break;

		case 5:
			turnOnBeltLeftFront(targetColor);
			break;
	}
}
}	// namespace leds

auto ledkit = LedKit {leds::internal::animations::event_loop, leds::ears, leds::belt};

auto touch_sensor_kit = TouchSensorKit();

using namespace leds;
// Chaque level a plusieurs manches. Cette fonction renvoie les signaux lumineux relatifs à une manche
void update_touched_colors()
{
	// lorsque l'enfant touche la bonne led elle devra s'allumer en vert
	constexpr RGB win_color = RGB::pure_green;
	// Si ce n'est pas la bonne led elle doit s'allumer en rouge
	constexpr RGB lose_color = RGB::pure_red;
	// Si on a touché l'oreille gauche et que le random_number vaut 0 ( ds le switch oreille gauche)
	// Elle se colore en vert et il a gagné (win = true)
	if (touch_sensor_kit.ear_left_touched()) {
		if (random_number == 0) {
			turnOnEarLeft(win_color);
			win = true;
			// Sinon c'est que random_number correspond à un autre groupe de leds, on a touché le mauvais
			//  On allume l'oreille gauche de la couleur rouge
		} else {
			turnOnEarLeft(lose_color);
		}
	}

	if (touch_sensor_kit.ear_right_touched()) {
		if (random_number == 1) {
			turnOnEarRight(win_color);
			win = true;
		} else {
			turnOnEarRight(lose_color);
		}
	}

	if (touch_sensor_kit.belt_right_back_touched()) {
		if (random_number == 2) {
			turnOnBeltRightBack(win_color);
			win = true;
		} else {
			turnOnBeltRightBack(lose_color);
		}
	}

	if (touch_sensor_kit.belt_right_front_touched()) {
		if (random_number == 3) {
			turnOnBeltRightFront(win_color);
			win = true;
		} else {
			turnOnBeltRightFront(lose_color);
		}
	}
	if (touch_sensor_kit.belt_left_back_touched()) {
		if (random_number == 4) {
			turnOnBeltLeftBack(win_color);
			win = true;
		} else {
			turnOnBeltLeftBack(lose_color);
		}
	}

	if (touch_sensor_kit.belt_left_front_touched()) {
		if (random_number == 5) {
			turnOnBeltLeftFront(win_color);
			win = true;
		} else {
			turnOnBeltLeftFront(lose_color);
		}
	}
}
// renvoie uniquement une variable globale win qui n'est modifiée que par update_touched_colors si l'enfant a touché le
// bon groupe de leds
auto is_target_touched() -> bool
{
	return win;
}
// Ici on controle le temps autorisé pour une manche si il est dépassé c'est de toute façon perdu
// inf 6000  si on veut perdre( 6 capteurs, 1capteur/s touché selon sleep du do du dessous)
auto playOnce(int maximum_duration) -> bool
{
	// On sette un groupe de leds en bleu de façon aléatoire
	leds::setLedsWithRandomColors();
	// ce touched_flags va setter groupe de leds il est au max = à 2^5 ce qui correspond au sixième groupe de leds
	uint8_t touched_flags = 1;
	// on récupère la valeur actuelle du temps
	auto start = rtos::Kernel::Clock::now();
	// Cette variable stockera l'écart entre le start et le moment oû l'enfant joue
	int delta;
	do {
		rtos::ThisThread::sleep_for(1s);
		// Le premier capteur sera considéré comme touché au départ de la manche (il correspond à 2^0 )
		touch_sensor_kit.fakeUpdateState(touched_flags);
		touch_sensor_kit.resetByPowerMode();
		// En fnction de random_number on met à jour les couleurs
		update_touched_colors();
		rtos::ThisThread::sleep_for(1s);
		// On multiplie touched flags par 2 afin de lui donner la valeur de la prochaine puissance de 2 ainsi cela
		// permettra de mettre à jour le prochain belt_touched
		touched_flags = touched_flags * 2;
		// on récupère le temps restant dans la manche
		delta = static_cast<int>((rtos::Kernel::Clock::now() - start).count());
		// On le fait tant que le bon groupe de leds n'est pas touché ou qu'il reste du temps
	} while (!(is_target_touched()) && (delta < maximum_duration));
	// On récupère la valuer de win à travers cette fonction, cette valeur a été mise à jour grâce à
	// update_touched_colors
	return is_target_touched();
}
// Cette fonction permet de jouer 5 manches d'un level
void playLevel(uint8_t level)
{	// durée pour toucher le bon groupe de leds doit être de plus en plus petite à mesure que le niveau augmente. Il est
	// au maximum à 6000 ms.
	// PK ? dans la fonction du dessus on simule l'allumage d'un groupe de leds toutes les secondes en 6000 ms tous les
	// groupes sont allumés en fait on gagne forcément le premier niveau. Mais pas forcément les autres
	int maximum_duration = 6000 - 1000 * level;
	uint8_t score		 = 0;
	// Si le score est inférieure à 5
	while (score < 5) {
		// Si on réussit une manche on augmente le score
		if (playOnce(maximum_duration)) {
			++score;
		} else {
			// sinon on revient à 0. On ne sort de la fonction que tant qu'on a réussit un level en entier (5 manches
			// réussies d'affilé)
			score = 0;
		}
	}

	log_info("animation::rainbow");
	ledkit.start(&LedKit::animation::rainbow);
	rtos::ThisThread::sleep_for(4s);
	touch_sensor_kit.printState();
	log_info("\n\n");
}
auto main() -> int
{
	logger::init();
	leds::turnOff();

	HelloWorld hello;
	hello.start();

	log_info("Hello, World!\n\n");

	ledkit.init();

	touch_sensor_kit.setup();

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	auto t = rtos::Kernel::Clock::now() - start;
	log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
			 int(t.count() / 1000));

	touch_sensor_kit.adjust_sensivity(0x00);
	rtos::ThisThread::sleep_for(2s);

	uint8_t level = 0;
	while (true) {
		// playlevel renvoie void se finit quand le score est incrémenté à 5
		playLevel(level);
		++level;
		if (level == 4) {
			level = 0;
		}
	}
}
