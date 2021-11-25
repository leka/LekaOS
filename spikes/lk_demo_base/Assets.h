// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ASSETS_H_
#define _LEKA_OS_ASSETS_H_

#include <array>
#include <lstd_array>
#include <string>

namespace leka {

constexpr auto emotions_table =
	lstd::to_array<const char *>({"robot-emotion-happy", "robot-emotion-angry", "robot-emotion-affraid",
								  "robot-emotion-disgusted", "robot-emotion-sad_tears"});

constexpr auto video_table =
	lstd::to_array<const char *>({"animation-idle", "animation-joy", "animation-perplexity",
								  "animation-face-state-waiting", "animation-face-action-wink"});

const auto object_colored_family_table = lstd::to_array<std::string>(
	{"animal-fish", "clothes-socks_pair", "clothes-tshirt", "object-balloon", "object-beach_bucket", "object-house",
	 "plant-flower-5_petals", "vehicule-aerial-hot_air_balloon"});

constexpr auto numbers_table = lstd::to_array<const char *>(
	{"number-0_zero", "number-1_one", "number-2_two", "number-3_three", "number-4_four", "number-5_five",
	 "number-6_six", "number-7_seven", "number-8_eight", "number-9_nine", "number-10_ten"});

const auto shapes_table = lstd::to_array<const char *>(
	{"shape-circle-plain_line", "shape-square-plain_line", "shape-triangle-plain_line", "shape-star-plain_line",
	 "shape-circle-dotted_line", "shape-square-dotted_line", "shape-triangle-dotted_line", "shape-star-dotted_line"});

const auto shapes_to_color_table = lstd::to_array<std::string>(
	{"shape-circle-filled", "shape-square-filled", "shape-triangle-filled", "shape-star-filled"});

constexpr auto rfid_image_table = lstd::to_array<const char *>({"shape-circle-plain_line",
																"shape-square-plain_line",
																"shape-triangle-plain_line",
																"shape-star-plain_line",
																"shape-circle-dotted_line",
																"shape-square-dotted_line",
																"shape-triangle-dotted_line",
																"shape-star-dotted_line",
																"number-0_zero",
																"number-1_one",
																"number-2_two",
																"number-3_three",
																"number-4_four",
																"number-5_five",
																"number-6_six",
																"number-7_seven",
																"number-8_eight",
																"number-9_nine",
																"number-10_ten",
																"color-red",
																"color-blue",
																"color-green",
																"color-yellow",
																"color-black",
																"color-white",
																"activity-magic_objects",
																"activity-dance_freeze",
																"activity-hide_and_seek",
																"activity-music_colored_board",
																"activity-music_quest",
																"activity-color_quest",
																"activity-colors_and_sounds",
																"activity-super_simon",
																"remote-colored_arrows",
																"remote-standard",
																"reinforcer-1-green-spin",
																"reinforcer-2-violet_green_blink-spin",
																"reinforcer-3-fire-static",
																"reinforcer-4-glitters-static",
																"reinforcer-5-rainbow-static",
																"emotion-happiness-child",
																"emotion-sadness-child",
																"emotion-anger-child",
																"emotion-fear-child",
																"emotion-disgust-child",
																"robot-emotion-happy",
																"robot-emotion-sad",
																"robot-emotion-angry",
																"robot-emotion-affraid",
																"robot-emotion-disgusted"});

}	// namespace leka

#endif	 // _LEKA_OS_ASSETS_H_
