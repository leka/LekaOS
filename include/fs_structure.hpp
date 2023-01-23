// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

// NOLINTBEGIN(modernize-concat-nested-namespaces)

namespace leka::fs {

namespace home {

	namespace vid {

		namespace system {

			inline constexpr auto robot_system_sleep_wake_up_no_eyebrows =
				std::string_view {"/fs/home/vid/system/robot-system-sleep-wake_up-no_eyebrows.avi"};
			inline constexpr auto robot_system_reinforcer_happy_no_eyebrows =
				std::string_view {"/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi"};
			inline constexpr auto robot_system_idle_looking_top_right_left_no_eyebrows =
				std::string_view {"/fs/home/vid/system/robot-system-idle-looking_top_right_left-no_eyebrows.avi"};
			inline constexpr auto robot_system_sleep_yawn_then_sleep_no_eyebrows =
				std::string_view {"/fs/home/vid/system/robot-system-sleep-yawn_then_sleep-no_eyebrows.avi"};
			inline constexpr auto robot_system_ble_connection_wink_no_eyebrows =
				std::string_view {"/fs/home/vid/system/robot-system-ble_connection-wink-no_eyebrows.avi"};

		}	// namespace system

		namespace states {

			inline constexpr auto robot_state_looking_top_right_left_no_eyebrows =
				std::string_view {"/fs/home/vid/states/robot-state-looking_top_right_left-no_eyebrows.avi"};
			inline constexpr auto robot_state_looking_center_front_winking_no_eyebrows =
				std::string_view {"/fs/home/vid/states/robot-state-looking_center_front_winking-no_eyebrows.avi"};
			inline constexpr auto robot_state_looking_center_right_no_eyebrows =
				std::string_view {"/fs/home/vid/states/robot-state-looking_center_right-no_eyebrows.avi"};
			inline constexpr auto robot_state_looking_center_left_no_eyebrows =
				std::string_view {"/fs/home/vid/states/robot-state-looking_center_left-no_eyebrows.avi"};
			inline constexpr auto robot_state_looking_top_right_no_eyebrows =
				std::string_view {"/fs/home/vid/states/robot-state-looking_top_right-no_eyebrows.avi"};

		}	// namespace states

		namespace actions {

			inline constexpr auto robot_animation_action_fly_landing_on_nose_no_eyebrows =
				std::string_view {"/fs/home/vid/actions/robot-animation-action-fly_landing_on_nose-no_eyebrows.avi"};
			inline constexpr auto robot_animation_action_blowing_bubbles_no_eyebrows =
				std::string_view {"/fs/home/vid/actions/robot-animation-action-blowing_bubbles-no_eyebrows.avi"};
			inline constexpr auto robot_animation_action_singing_colored_notes_no_eyebrows =
				std::string_view {"/fs/home/vid/actions/robot-animation-action-singing-colored_notes-no_eyebrows.avi"};
			inline constexpr auto robot_animation_action_sneezing_no_eyebrows =
				std::string_view {"/fs/home/vid/actions/robot-animation-action-sneezing-no_eyebrows.avi"};
			inline constexpr auto robot_animation_action_diving_under_water_no_eyebrows =
				std::string_view {"/fs/home/vid/actions/robot-animation-action-diving_under_water-no_eyebrows.avi"};
			inline constexpr auto robot_animation_action_yawning_no_eyebrows =
				std::string_view {"/fs/home/vid/actions/robot-animation-action-yawning-no_eyebrows.avi"};

		}	// namespace actions

		namespace emotions {

			inline constexpr auto robot_emotion_afraid_blue_forehead_sweatdrop_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-afraid_blue_forehead_sweatdrop-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_amazed_sparkly_eyes_looking_top_trembling_mouth_no_eyebrows =
				std::string_view {
					"/fs/home/vid/emotions/"
					"robot-emotion-amazed_sparkly_eyes_looking_top_trembling_mouth-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_angry_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-angry-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_angry_breathing_nose_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-angry_breathing_nose-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_amazed_sparkly_eyes_trembling_mouth_no_eyebrows = std::string_view {
				"/fs/home/vid/emotions/robot-emotion-amazed_sparkly_eyes_trembling_mouth-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_sad_trembling_mouth_cry_tears_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-sad_trembling_mouth_cry_tears-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_happy_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-happy-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_sad_no_tears_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-sad-no_tears-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_afraid_trembling_eyes_sweatdrop_no_eyebrows = std::string_view {
				"/fs/home/vid/emotions/robot-emotion-afraid_trembling_eyes_sweatdrop-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_afraid_trembling_eyes_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-afraid_trembling_eyes-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_disgusted_green_cheeks_pull_out_the_tang_no_eyebrows =
				std::string_view {
					"/fs/home/vid/emotions/robot-emotion-disgusted_green_cheeks_pull_out_the_tang-no_eyebrows.avi"};
			inline constexpr auto robot_emotion_sick_green_cheeks_no_eyebrows =
				std::string_view {"/fs/home/vid/emotions/robot-emotion-sick_green_cheeks-no_eyebrows.avi"};

		}	// namespace emotions

	}	// namespace vid

	namespace wav {

		inline constexpr auto fur_elise = std::string_view {"/fs/home/wav/fur-elise.wav"};

	}	// namespace wav

	namespace img {

		namespace system {

			inline constexpr auto robot_battery_charging_quarter_1_orange =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_1-orange.jpg"};
			inline constexpr auto robot_face_tired = std::string_view {"/fs/home/img/system/robot-face-tired.jpg"};
			inline constexpr auto robot_misc_splash_screen_medium_300 =
				std::string_view {"/fs/home/img/system/robot-misc-splash_screen-medium-300.jpg"};
			inline constexpr auto robot_misc_splash_screen_small_200 =
				std::string_view {"/fs/home/img/system/robot-misc-splash_screen-small-200.jpg"};
			inline constexpr auto robot_battery_empty_must_be_charged =
				std::string_view {"/fs/home/img/system/robot-battery-empty-must_be_charged.jpg"};
			inline constexpr auto robot_battery_charging_full_green =
				std::string_view {"/fs/home/img/system/robot-battery-charging-full_green.jpg"};
			inline constexpr auto robot_battery_charging_quarter_2_yellow =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_2-yellow.jpg"};
			inline constexpr auto robot_face_smiling = std::string_view {"/fs/home/img/system/robot-face-smiling.jpg"};
			inline constexpr auto robot_battery_charging_quarter_4_orange =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_4-orange.jpg"};
			inline constexpr auto robot_misc_choose_activity_fr_FR =
				std::string_view {"/fs/home/img/system/robot-misc-choose_activity-fr_FR.jpg"};
			inline constexpr auto robot_face_angry = std::string_view {"/fs/home/img/system/robot-face-angry.jpg"};
			inline constexpr auto robot_battery_charging_quarter_1_red =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_1-red.jpg"};
			inline constexpr auto robot_battery_charging_quarter_1_green =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_1-green.jpg"};
			inline constexpr auto robot_face_sad = std::string_view {"/fs/home/img/system/robot-face-sad.jpg"};
			inline constexpr auto robot_battery_charging_quarter_3_orange =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_3-orange.jpg"};
			inline constexpr auto robot_face_neutral = std::string_view {"/fs/home/img/system/robot-face-neutral.jpg"};
			inline constexpr auto robot_battery_charging_quarter_3_red =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_3-red.jpg"};
			inline constexpr auto robot_misc_choose_activity_en_US =
				std::string_view {"/fs/home/img/system/robot-misc-choose_activity-en_US.jpg"};
			inline constexpr auto robot_misc_missing_resource =
				std::string_view {"/fs/home/img/system/robot-misc-missing_resource.jpg"};
			inline constexpr auto robot_battery_charging_quarter_2_red =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_2-red.jpg"};
			inline constexpr auto robot_misc_robot_misc_screen_empty_white =
				std::string_view {"/fs/home/img/system/robot-misc-robot-misc-screen_empty_white.jpg"};
			inline constexpr auto robot_face_smiling_slightly =
				std::string_view {"/fs/home/img/system/robot-face-smiling-slightly.jpg"};
			inline constexpr auto robot_battery_charging_quarter_3_yellow =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_3-yellow.jpg"};
			inline constexpr auto robot_face_smiling_smiling_eyes =
				std::string_view {"/fs/home/img/system/robot-face-smiling-smiling_eyes.jpg"};
			inline constexpr auto robot_misc_splash_screen_large_400 =
				std::string_view {"/fs/home/img/system/robot-misc-splash_screen-large-400.jpg"};
			inline constexpr auto robot_battery_charging_empty_no_color =
				std::string_view {"/fs/home/img/system/robot-battery-charging-empty_no_color.jpg"};
			inline constexpr auto robot_face_happy = std::string_view {"/fs/home/img/system/robot-face-happy.jpg"};
			inline constexpr auto robot_battery_charging_quarter_3_green =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_3-green.jpg"};
			inline constexpr auto robot_battery_charging_quarter_1_yellow =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_1-yellow.jpg"};
			inline constexpr auto robot_battery_charging_quarter_4_red =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_4-red.jpg"};
			inline constexpr auto robot_face_disgusted =
				std::string_view {"/fs/home/img/system/robot-face-disgusted.jpg"};
			inline constexpr auto robot_face_smiling_no_cheeks =
				std::string_view {"/fs/home/img/system/robot-face-smiling-no_cheeks.jpg"};
			inline constexpr auto robot_face_tired_with_tears =
				std::string_view {"/fs/home/img/system/robot-face-tired-with_tears.jpg"};
			inline constexpr auto robot_battery_charging_quarter_4_green =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_4-green.jpg"};
			inline constexpr auto robot_face_afraid = std::string_view {"/fs/home/img/system/robot-face-afraid.jpg"};
			inline constexpr auto robot_battery_charging_quarter_2_orange =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_2-orange.jpg"};
			inline constexpr auto robot_battery_charging_quarter_4_yellow =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_4-yellow.jpg"};
			inline constexpr auto robot_battery_charging_quarter_2_green =
				std::string_view {"/fs/home/img/system/robot-battery-charging-quarter_2-green.jpg"};
			inline constexpr auto robot_face_sad_with_tears =
				std::string_view {"/fs/home/img/system/robot-face-sad-with_tears.jpg"};
			inline constexpr auto robot_battery_charging_empty_red =
				std::string_view {"/fs/home/img/system/robot-battery-charging-empty_red.jpg"};
			inline constexpr auto robot_misc_robot_misc_screen_empty_black =
				std::string_view {"/fs/home/img/system/robot-misc-robot-misc-screen_empty_black.jpg"};

		}	// namespace system

		namespace id {

			inline constexpr auto _0071 = std::string_view {"/fs/home/img/id/0071.jpg"};
			inline constexpr auto _0065 = std::string_view {"/fs/home/img/id/0065.jpg"};
			inline constexpr auto _00A6 = std::string_view {"/fs/home/img/id/00A6.jpg"};
			inline constexpr auto _0059 = std::string_view {"/fs/home/img/id/0059.jpg"};
			inline constexpr auto _006C = std::string_view {"/fs/home/img/id/006C.jpg"};
			inline constexpr auto _006B = std::string_view {"/fs/home/img/id/006B.jpg"};
			inline constexpr auto _0058 = std::string_view {"/fs/home/img/id/0058.jpg"};
			inline constexpr auto _007F = std::string_view {"/fs/home/img/id/007F.jpg"};
			inline constexpr auto _00A7 = std::string_view {"/fs/home/img/id/00A7.jpg"};
			inline constexpr auto _0064 = std::string_view {"/fs/home/img/id/0064.jpg"};
			inline constexpr auto _0070 = std::string_view {"/fs/home/img/id/0070.jpg"};
			inline constexpr auto _00AA = std::string_view {"/fs/home/img/id/00AA.jpg"};
			inline constexpr auto _0066 = std::string_view {"/fs/home/img/id/0066.jpg"};
			inline constexpr auto _00B9 = std::string_view {"/fs/home/img/id/00B9.jpg"};
			inline constexpr auto _00AC = std::string_view {"/fs/home/img/id/00AC.jpg"};
			inline constexpr auto _0072 = std::string_view {"/fs/home/img/id/0072.jpg"};
			inline constexpr auto _00A5 = std::string_view {"/fs/home/img/id/00A5.jpg"};
			inline constexpr auto _007D = std::string_view {"/fs/home/img/id/007D.jpg"};
			inline constexpr auto _0099 = std::string_view {"/fs/home/img/id/0099.jpg"};
			inline constexpr auto _0098 = std::string_view {"/fs/home/img/id/0098.jpg"};
			inline constexpr auto _00A4 = std::string_view {"/fs/home/img/id/00A4.jpg"};
			inline constexpr auto _007E = std::string_view {"/fs/home/img/id/007E.jpg"};
			inline constexpr auto _006A = std::string_view {"/fs/home/img/id/006A.jpg"};
			inline constexpr auto _00AB = std::string_view {"/fs/home/img/id/00AB.jpg"};
			inline constexpr auto _00B8 = std::string_view {"/fs/home/img/id/00B8.jpg"};
			inline constexpr auto _0073 = std::string_view {"/fs/home/img/id/0073.jpg"};
			inline constexpr auto _0067 = std::string_view {"/fs/home/img/id/0067.jpg"};
			inline constexpr auto _006E = std::string_view {"/fs/home/img/id/006E.jpg"};
			inline constexpr auto _007A = std::string_view {"/fs/home/img/id/007A.jpg"};
			inline constexpr auto _00A0 = std::string_view {"/fs/home/img/id/00A0.jpg"};
			inline constexpr auto _00C8 = std::string_view {"/fs/home/img/id/00C8.jpg"};
			inline constexpr auto _0063 = std::string_view {"/fs/home/img/id/0063.jpg"};
			inline constexpr auto _0077 = std::string_view {"/fs/home/img/id/0077.jpg"};
			inline constexpr auto _00AF = std::string_view {"/fs/home/img/id/00AF.jpg"};
			inline constexpr auto _0088 = std::string_view {"/fs/home/img/id/0088.jpg"};
			inline constexpr auto _0089 = std::string_view {"/fs/home/img/id/0089.jpg"};
			inline constexpr auto _0076 = std::string_view {"/fs/home/img/id/0076.jpg"};
			inline constexpr auto _00C9 = std::string_view {"/fs/home/img/id/00C9.jpg"};
			inline constexpr auto _0062 = std::string_view {"/fs/home/img/id/0062.jpg"};
			inline constexpr auto _00A1 = std::string_view {"/fs/home/img/id/00A1.jpg"};
			inline constexpr auto _006D = std::string_view {"/fs/home/img/id/006D.jpg"};
			inline constexpr auto _00A3 = std::string_view {"/fs/home/img/id/00A3.jpg"};
			inline constexpr auto _007B = std::string_view {"/fs/home/img/id/007B.jpg"};
			inline constexpr auto _0048 = std::string_view {"/fs/home/img/id/0048.jpg"};
			inline constexpr auto _006F = std::string_view {"/fs/home/img/id/006F.jpg"};
			inline constexpr auto _00AE = std::string_view {"/fs/home/img/id/00AE.jpg"};
			inline constexpr auto _0074 = std::string_view {"/fs/home/img/id/0074.jpg"};
			inline constexpr auto _0060 = std::string_view {"/fs/home/img/id/0060.jpg"};
			inline constexpr auto _0061 = std::string_view {"/fs/home/img/id/0061.jpg"};
			inline constexpr auto _00AD = std::string_view {"/fs/home/img/id/00AD.jpg"};
			inline constexpr auto _0075 = std::string_view {"/fs/home/img/id/0075.jpg"};
			inline constexpr auto _00A2 = std::string_view {"/fs/home/img/id/00A2.jpg"};
			inline constexpr auto _0049 = std::string_view {"/fs/home/img/id/0049.jpg"};
			inline constexpr auto _007C = std::string_view {"/fs/home/img/id/007C.jpg"};
			inline constexpr auto _00D9 = std::string_view {"/fs/home/img/id/00D9.jpg"};
			inline constexpr auto _0012 = std::string_view {"/fs/home/img/id/0012.jpg"};
			inline constexpr auto _0006 = std::string_view {"/fs/home/img/id/0006.jpg"};
			inline constexpr auto _001D = std::string_view {"/fs/home/img/id/001D.jpg"};
			inline constexpr auto _000A = std::string_view {"/fs/home/img/id/000A.jpg"};
			inline constexpr auto _001E = std::string_view {"/fs/home/img/id/001E.jpg"};
			inline constexpr auto _0007 = std::string_view {"/fs/home/img/id/0007.jpg"};
			inline constexpr auto _00D8 = std::string_view {"/fs/home/img/id/00D8.jpg"};
			inline constexpr auto _0013 = std::string_view {"/fs/home/img/id/0013.jpg"};
			inline constexpr auto _0005 = std::string_view {"/fs/home/img/id/0005.jpg"};
			inline constexpr auto _0011 = std::string_view {"/fs/home/img/id/0011.jpg"};
			inline constexpr auto _000C = std::string_view {"/fs/home/img/id/000C.jpg"};
			inline constexpr auto _0039 = std::string_view {"/fs/home/img/id/0039.jpg"};
			inline constexpr auto _001F = std::string_view {"/fs/home/img/id/001F.jpg"};
			inline constexpr auto _0038 = std::string_view {"/fs/home/img/id/0038.jpg"};
			inline constexpr auto _000B = std::string_view {"/fs/home/img/id/000B.jpg"};
			inline constexpr auto _0010 = std::string_view {"/fs/home/img/id/0010.jpg"};
			inline constexpr auto _0004 = std::string_view {"/fs/home/img/id/0004.jpg"};
			inline constexpr auto _000F = std::string_view {"/fs/home/img/id/000F.jpg"};
			inline constexpr auto _0028 = std::string_view {"/fs/home/img/id/0028.jpg"};
			inline constexpr auto _001B = std::string_view {"/fs/home/img/id/001B.jpg"};
			inline constexpr auto _0014 = std::string_view {"/fs/home/img/id/0014.jpg"};
			inline constexpr auto _0015 = std::string_view {"/fs/home/img/id/0015.jpg"};
			inline constexpr auto _0001 = std::string_view {"/fs/home/img/id/0001.jpg"};
			inline constexpr auto _001C = std::string_view {"/fs/home/img/id/001C.jpg"};
			inline constexpr auto _0029 = std::string_view {"/fs/home/img/id/0029.jpg"};
			inline constexpr auto _001A = std::string_view {"/fs/home/img/id/001A.jpg"};
			inline constexpr auto _000E = std::string_view {"/fs/home/img/id/000E.jpg"};
			inline constexpr auto _0017 = std::string_view {"/fs/home/img/id/0017.jpg"};
			inline constexpr auto _0003 = std::string_view {"/fs/home/img/id/0003.jpg"};
			inline constexpr auto _0002 = std::string_view {"/fs/home/img/id/0002.jpg"};
			inline constexpr auto _0016 = std::string_view {"/fs/home/img/id/0016.jpg"};
			inline constexpr auto _000D = std::string_view {"/fs/home/img/id/000D.jpg"};
			inline constexpr auto _0033 = std::string_view {"/fs/home/img/id/0033.jpg"};
			inline constexpr auto _0027 = std::string_view {"/fs/home/img/id/0027.jpg"};
			inline constexpr auto _003E = std::string_view {"/fs/home/img/id/003E.jpg"};
			inline constexpr auto _002A = std::string_view {"/fs/home/img/id/002A.jpg"};
			inline constexpr auto _00D0 = std::string_view {"/fs/home/img/id/00D0.jpg"};
			inline constexpr auto _00D1 = std::string_view {"/fs/home/img/id/00D1.jpg"};
			inline constexpr auto _003D = std::string_view {"/fs/home/img/id/003D.jpg"};
			inline constexpr auto _0026 = std::string_view {"/fs/home/img/id/0026.jpg"};
			inline constexpr auto _0032 = std::string_view {"/fs/home/img/id/0032.jpg"};
			inline constexpr auto _0024 = std::string_view {"/fs/home/img/id/0024.jpg"};
			inline constexpr auto _0030 = std::string_view {"/fs/home/img/id/0030.jpg"};
			inline constexpr auto _00D3 = std::string_view {"/fs/home/img/id/00D3.jpg"};
			inline constexpr auto _002B = std::string_view {"/fs/home/img/id/002B.jpg"};
			inline constexpr auto _0018 = std::string_view {"/fs/home/img/id/0018.jpg"};
			inline constexpr auto _003F = std::string_view {"/fs/home/img/id/003F.jpg"};
			inline constexpr auto _00D2 = std::string_view {"/fs/home/img/id/00D2.jpg"};
			inline constexpr auto _0019 = std::string_view {"/fs/home/img/id/0019.jpg"};
			inline constexpr auto _002C = std::string_view {"/fs/home/img/id/002C.jpg"};
			inline constexpr auto _0031 = std::string_view {"/fs/home/img/id/0031.jpg"};
			inline constexpr auto _0025 = std::string_view {"/fs/home/img/id/0025.jpg"};
			inline constexpr auto _00D6 = std::string_view {"/fs/home/img/id/00D6.jpg"};
			inline constexpr auto _0009 = std::string_view {"/fs/home/img/id/0009.jpg"};
			inline constexpr auto _003C = std::string_view {"/fs/home/img/id/003C.jpg"};
			inline constexpr auto _0021 = std::string_view {"/fs/home/img/id/0021.jpg"};
			inline constexpr auto _0035 = std::string_view {"/fs/home/img/id/0035.jpg"};
			inline constexpr auto _0034 = std::string_view {"/fs/home/img/id/0034.jpg"};
			inline constexpr auto _0020 = std::string_view {"/fs/home/img/id/0020.jpg"};
			inline constexpr auto _00DA = std::string_view {"/fs/home/img/id/00DA.jpg"};
			inline constexpr auto _003B = std::string_view {"/fs/home/img/id/003B.jpg"};
			inline constexpr auto _0008 = std::string_view {"/fs/home/img/id/0008.jpg"};
			inline constexpr auto _002F = std::string_view {"/fs/home/img/id/002F.jpg"};
			inline constexpr auto _00D7 = std::string_view {"/fs/home/img/id/00D7.jpg"};
			inline constexpr auto _00D5 = std::string_view {"/fs/home/img/id/00D5.jpg"};
			inline constexpr auto _002D = std::string_view {"/fs/home/img/id/002D.jpg"};
			inline constexpr auto _0036 = std::string_view {"/fs/home/img/id/0036.jpg"};
			inline constexpr auto _0022 = std::string_view {"/fs/home/img/id/0022.jpg"};
			inline constexpr auto _00DB = std::string_view {"/fs/home/img/id/00DB.jpg"};
			inline constexpr auto _0023 = std::string_view {"/fs/home/img/id/0023.jpg"};
			inline constexpr auto _0037 = std::string_view {"/fs/home/img/id/0037.jpg"};
			inline constexpr auto _00D4 = std::string_view {"/fs/home/img/id/00D4.jpg"};
			inline constexpr auto _002E = std::string_view {"/fs/home/img/id/002E.jpg"};
			inline constexpr auto _003A = std::string_view {"/fs/home/img/id/003A.jpg"};
			inline constexpr auto _0050 = std::string_view {"/fs/home/img/id/0050.jpg"};
			inline constexpr auto _00CA = std::string_view {"/fs/home/img/id/00CA.jpg"};
			inline constexpr auto _00BE = std::string_view {"/fs/home/img/id/00BE.jpg"};
			inline constexpr auto _0044 = std::string_view {"/fs/home/img/id/0044.jpg"};
			inline constexpr auto _005F = std::string_view {"/fs/home/img/id/005F.jpg"};
			inline constexpr auto _00C7 = std::string_view {"/fs/home/img/id/00C7.jpg"};
			inline constexpr auto _00B3 = std::string_view {"/fs/home/img/id/00B3.jpg"};
			inline constexpr auto _0078 = std::string_view {"/fs/home/img/id/0078.jpg"};
			inline constexpr auto _004B = std::string_view {"/fs/home/img/id/004B.jpg"};
			inline constexpr auto _0093 = std::string_view {"/fs/home/img/id/0093.jpg"};
			inline constexpr auto _0087 = std::string_view {"/fs/home/img/id/0087.jpg"};
			inline constexpr auto _009E = std::string_view {"/fs/home/img/id/009E.jpg"};
			inline constexpr auto _008A = std::string_view {"/fs/home/img/id/008A.jpg"};
			inline constexpr auto _009D = std::string_view {"/fs/home/img/id/009D.jpg"};
			inline constexpr auto _0086 = std::string_view {"/fs/home/img/id/0086.jpg"};
			inline constexpr auto _0092 = std::string_view {"/fs/home/img/id/0092.jpg"};
			inline constexpr auto _00B2 = std::string_view {"/fs/home/img/id/00B2.jpg"};
			inline constexpr auto _004C = std::string_view {"/fs/home/img/id/004C.jpg"};
			inline constexpr auto _0079 = std::string_view {"/fs/home/img/id/0079.jpg"};
			inline constexpr auto _00C6 = std::string_view {"/fs/home/img/id/00C6.jpg"};
			inline constexpr auto _00BD = std::string_view {"/fs/home/img/id/00BD.jpg"};
			inline constexpr auto _0045 = std::string_view {"/fs/home/img/id/0045.jpg"};
			inline constexpr auto _0051 = std::string_view {"/fs/home/img/id/0051.jpg"};
			inline constexpr auto _0047 = std::string_view {"/fs/home/img/id/0047.jpg"};
			inline constexpr auto _00BF = std::string_view {"/fs/home/img/id/00BF.jpg"};
			inline constexpr auto _00CB = std::string_view {"/fs/home/img/id/00CB.jpg"};
			inline constexpr auto _0053 = std::string_view {"/fs/home/img/id/0053.jpg"};
			inline constexpr auto _004A = std::string_view {"/fs/home/img/id/004A.jpg"};
			inline constexpr auto _00B0 = std::string_view {"/fs/home/img/id/00B0.jpg"};
			inline constexpr auto _00C4 = std::string_view {"/fs/home/img/id/00C4.jpg"};
			inline constexpr auto _005E = std::string_view {"/fs/home/img/id/005E.jpg"};
			inline constexpr auto _0084 = std::string_view {"/fs/home/img/id/0084.jpg"};
			inline constexpr auto _0090 = std::string_view {"/fs/home/img/id/0090.jpg"};
			inline constexpr auto _008B = std::string_view {"/fs/home/img/id/008B.jpg"};
			inline constexpr auto _009F = std::string_view {"/fs/home/img/id/009F.jpg"};
			inline constexpr auto _008C = std::string_view {"/fs/home/img/id/008C.jpg"};
			inline constexpr auto _0091 = std::string_view {"/fs/home/img/id/0091.jpg"};
			inline constexpr auto _0085 = std::string_view {"/fs/home/img/id/0085.jpg"};
			inline constexpr auto _00C5 = std::string_view {"/fs/home/img/id/00C5.jpg"};
			inline constexpr auto _005D = std::string_view {"/fs/home/img/id/005D.jpg"};
			inline constexpr auto _00B1 = std::string_view {"/fs/home/img/id/00B1.jpg"};
			inline constexpr auto _00CC = std::string_view {"/fs/home/img/id/00CC.jpg"};
			inline constexpr auto _0052 = std::string_view {"/fs/home/img/id/0052.jpg"};
			inline constexpr auto _0046 = std::string_view {"/fs/home/img/id/0046.jpg"};
			inline constexpr auto _00B5 = std::string_view {"/fs/home/img/id/00B5.jpg"};
			inline constexpr auto _004D = std::string_view {"/fs/home/img/id/004D.jpg"};
			inline constexpr auto _00C1 = std::string_view {"/fs/home/img/id/00C1.jpg"};
			inline constexpr auto _00BC = std::string_view {"/fs/home/img/id/00BC.jpg"};
			inline constexpr auto _00A9 = std::string_view {"/fs/home/img/id/00A9.jpg"};
			inline constexpr auto _0042 = std::string_view {"/fs/home/img/id/0042.jpg"};
			inline constexpr auto _0056 = std::string_view {"/fs/home/img/id/0056.jpg"};
			inline constexpr auto _009C = std::string_view {"/fs/home/img/id/009C.jpg"};
			inline constexpr auto _0081 = std::string_view {"/fs/home/img/id/0081.jpg"};
			inline constexpr auto _0095 = std::string_view {"/fs/home/img/id/0095.jpg"};
			inline constexpr auto _0094 = std::string_view {"/fs/home/img/id/0094.jpg"};
			inline constexpr auto _0080 = std::string_view {"/fs/home/img/id/0080.jpg"};
			inline constexpr auto _009B = std::string_view {"/fs/home/img/id/009B.jpg"};
			inline constexpr auto _008F = std::string_view {"/fs/home/img/id/008F.jpg"};
			inline constexpr auto _0057 = std::string_view {"/fs/home/img/id/0057.jpg"};
			inline constexpr auto _00CF = std::string_view {"/fs/home/img/id/00CF.jpg"};
			inline constexpr auto _00A8 = std::string_view {"/fs/home/img/id/00A8.jpg"};
			inline constexpr auto _00BB = std::string_view {"/fs/home/img/id/00BB.jpg"};
			inline constexpr auto _0043 = std::string_view {"/fs/home/img/id/0043.jpg"};
			inline constexpr auto _005A = std::string_view {"/fs/home/img/id/005A.jpg"};
			inline constexpr auto _00C0 = std::string_view {"/fs/home/img/id/00C0.jpg"};
			inline constexpr auto _00B4 = std::string_view {"/fs/home/img/id/00B4.jpg"};
			inline constexpr auto _004E = std::string_view {"/fs/home/img/id/004E.jpg"};
			inline constexpr auto _00C2 = std::string_view {"/fs/home/img/id/00C2.jpg"};
			inline constexpr auto _005C = std::string_view {"/fs/home/img/id/005C.jpg"};
			inline constexpr auto _0069 = std::string_view {"/fs/home/img/id/0069.jpg"};
			inline constexpr auto _00B6 = std::string_view {"/fs/home/img/id/00B6.jpg"};
			inline constexpr auto _00CD = std::string_view {"/fs/home/img/id/00CD.jpg"};
			inline constexpr auto _0055 = std::string_view {"/fs/home/img/id/0055.jpg"};
			inline constexpr auto _0041 = std::string_view {"/fs/home/img/id/0041.jpg"};
			inline constexpr auto _008D = std::string_view {"/fs/home/img/id/008D.jpg"};
			inline constexpr auto _0096 = std::string_view {"/fs/home/img/id/0096.jpg"};
			inline constexpr auto _0082 = std::string_view {"/fs/home/img/id/0082.jpg"};
			inline constexpr auto _0083 = std::string_view {"/fs/home/img/id/0083.jpg"};
			inline constexpr auto _0097 = std::string_view {"/fs/home/img/id/0097.jpg"};
			inline constexpr auto _008E = std::string_view {"/fs/home/img/id/008E.jpg"};
			inline constexpr auto _009A = std::string_view {"/fs/home/img/id/009A.jpg"};
			inline constexpr auto _0040 = std::string_view {"/fs/home/img/id/0040.jpg"};
			inline constexpr auto _00BA = std::string_view {"/fs/home/img/id/00BA.jpg"};
			inline constexpr auto _00CE = std::string_view {"/fs/home/img/id/00CE.jpg"};
			inline constexpr auto _0054 = std::string_view {"/fs/home/img/id/0054.jpg"};
			inline constexpr auto _004F = std::string_view {"/fs/home/img/id/004F.jpg"};
			inline constexpr auto _00B7 = std::string_view {"/fs/home/img/id/00B7.jpg"};
			inline constexpr auto _00C3 = std::string_view {"/fs/home/img/id/00C3.jpg"};
			inline constexpr auto _0068 = std::string_view {"/fs/home/img/id/0068.jpg"};
			inline constexpr auto _005B = std::string_view {"/fs/home/img/id/005B.jpg"};

		}	// namespace id

	}	// namespace img

}	// namespace home

namespace usr {

	namespace test {

		inline constexpr auto vid_2_ok				  = std::string_view {"/fs/usr/test/vid-2-ok.avi"};
		inline constexpr auto img_9_ok				  = std::string_view {"/fs/usr/test/img-9-ok.jpg"};
		inline constexpr auto file_1_ok				  = std::string_view {"/fs/usr/test/file-1-ok.txt"};
		inline constexpr auto img_1_ok				  = std::string_view {"/fs/usr/test/img-1-ok.jpg"};
		inline constexpr auto vid_0_ok_pre_image	  = std::string_view {"/fs/usr/test/vid-0-ok-pre_image.jpg"};
		inline constexpr auto img_3_ok				  = std::string_view {"/fs/usr/test/img-3-ok.jpg"};
		inline constexpr auto file_2_ko_empty		  = std::string_view {"/fs/usr/test/file-2-ko-empty.txt"};
		inline constexpr auto vid_1_ok				  = std::string_view {"/fs/usr/test/vid-1-ok.avi"};
		inline constexpr auto img_4_ko_broken_content = std::string_view {"/fs/usr/test/img-4-ko-broken_content.jpg"};
		inline constexpr auto img_6_ko_empty		  = std::string_view {"/fs/usr/test/img-6-ko-empty.jpg"};
		inline constexpr auto file_3_ko_need_reboot	  = std::string_view {"/fs/usr/test/file-3-ko-need_reboot"};
		inline constexpr auto vid_4_ko_empty		  = std::string_view {"/fs/usr/test/vid-4-ko-empty.avi"};
		inline constexpr auto img_5_ko_size_800x481	  = std::string_view {"/fs/usr/test/img-5-ko-size_800x481.jpg"};
		inline constexpr auto vid_5_ok				  = std::string_view {"/fs/usr/test/vid-5-ok.avi"};
		inline constexpr auto conf					  = std::string_view {"/fs/usr/test/conf"};
		inline constexpr auto vid_3_ko_broken_content = std::string_view {"/fs/usr/test/vid-3-ko-broken_content.avi"};
		inline constexpr auto img_2_ok				  = std::string_view {"/fs/usr/test/img-2-ok.jpg"};

	}	// namespace test

	namespace certs {

		namespace ca {

			inline constexpr auto DigiCert_SHA2_High_Assurance_Server_CA_04E1E7A4DC5CF2F36DC02B42B85D159F =
				std::string_view {
					"/fs/usr/certs/ca/DigiCert_SHA2_High_Assurance_Server_CA-04E1E7A4DC5CF2F36DC02B42B85D159F.txt"};
			inline constexpr auto
				DigiCert_High_Assurance_TLS_Hybrid_ECC_SHA256_2020_CA1_0667035BBB14FD63AFC0D6A8534EFE16 =
					std::string_view {
						"/fs/usr/certs/ca/"
						"DigiCert_High_Assurance_TLS_Hybrid_ECC_SHA256_2020_CA1-0667035BBB14FD63AFC0D6A8534EFE16.txt"};

		}	// namespace ca

	}	// namespace certs

	namespace os {

		inline constexpr auto LekaOS_factory = std::string_view {"/fs/usr/os/LekaOS-factory.bin"};

	}	// namespace os

	namespace share {

		inline constexpr auto global_reboots_counter = std::string_view {"/fs/usr/share/global_reboots_counter"};
		inline constexpr auto factory_reset_counter	 = std::string_view {"/fs/usr/share/factory_reset_counter"};

	}	// namespace share

}	// namespace usr

namespace etc {

	inline constexpr auto bootloader_battery_hysteresis = std::string_view {"/fs/etc/bootloader-battery_hysteresis"};
	inline constexpr auto bootloader_reboots_limit		= std::string_view {"/fs/etc/bootloader-reboots_limit"};

}	// namespace etc

namespace var {

}	// namespace var

namespace sys {

	inline constexpr auto date_of_test		 = std::string_view {"/fs/sys/date_of_test"};
	inline constexpr auto os_version		 = std::string_view {"/fs/sys/os-version"};
	inline constexpr auto bootloader_version = std::string_view {"/fs/sys/bootloader-version"};
	inline constexpr auto hardware_version	 = std::string_view {"/fs/sys/hardware-version"};

}	// namespace sys

}	// namespace leka::fs

// NOLINTEND(modernize-concat-nested-namespaces)
