// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0
// clang-format off

#ifndef _LEKA_SHAPES_CREATION_H_
#define _LEKA_SHAPES_CREATION_H_

#include <cstdint>
#include "shape.h"
#include <array>

using namespace leka;

using outputs_sequence_s2 = std::array<std::uint8_t, 2> ; //Size 2
using outputs_sequence_s3 = std::array<std::uint8_t, 3>;
using outputs_sequence_s4 = std::array<std::uint8_t, 4>;
using outputs_sequence_s5 = std::array<std::uint8_t, 5>;


	/****** CLASSES ******/
namespace motion {
	
	namespace tree::output {

			struct Segment {
				uint8_t 			_value;
				std::string_view 	_name_image;
			};

			namespace number {
				inline constexpr auto idle 				= 1;

				inline constexpr auto down 				= 8;
				inline constexpr auto left 				= 4;
				inline constexpr auto right 			= 0;
				inline constexpr auto up				= 7;

				inline constexpr auto down_left 		= 6;
				inline constexpr auto down_right 		= 3;
				inline constexpr auto up_left 			= 5;
				inline constexpr auto up_right 			= 2;
			}

			namespace segment {
				inline constexpr auto idle 				= Segment { number::idle, 			"Idle"		};
				
				inline constexpr auto down 				= Segment { number::down, 			"Down"		};
				inline constexpr auto left 				= Segment { number::left, 			"Left"		};
				inline constexpr auto right 			= Segment { number::right, 			"Right"		};
				inline constexpr auto up 				= Segment { number::up, 			"Up"		};

				inline constexpr auto down_left 		= Segment { number::down_left, "Down_Left"		};
				inline constexpr auto down_right 		= Segment { number::down_right,"Down_Right"		};
				inline constexpr auto up_left 			= Segment { number::up_left, 	"Up_Left"		};
				inline constexpr auto up_right 			= Segment { number::up_right, 	"Up_Right"		};
			}

			inline constexpr auto segments_array 		= std::to_array( {
														segment::idle,
														segment::down, 			segment::left, 
														segment::right, 		segment::up,
														segment::down_left, 	segment::down_right, 
														segment::up_left, 		segment::up_right
														});
		}
	

	namespace shape {

		namespace clockwise {

			namespace card {

				inline constexpr auto square					= 0x0015;	//Square
				inline constexpr auto rhombus 					= 0x0005; 	//Blue
				inline constexpr auto star 						= 0x0018;	//Star
				inline constexpr auto ortho_triangle_one 		= 0x000C; 	//4
				inline constexpr auto ortho_triangle_second 	= 0x0009; 	//2
				inline constexpr auto iso_triangle_one			= 0x0017;
				inline constexpr auto iso_triangle_second		= 0x000B;

			}
			
			inline const auto square 				= Shape<4> {card::square,
														outputs_sequence_s4 {tree::output::number::up, tree::output::number::right, tree::output::number::down, tree::output::number::left}, "Square_C"};
			inline const auto rhombus				= Shape<4>(card::rhombus,
														outputs_sequence_s4 {tree::output::number::up_right, tree::output::number::down_right, tree::output::number::down_left, tree::output::number::up_left}, "Rhombus_C");
			inline const auto star 					= Shape<5>(card::star,
														outputs_sequence_s5 {tree::output::number::up_right, tree::output::number::down_right, tree::output::number::up_left, tree::output::number::right, tree::output::number::down_left}, "Star_C");
			inline const auto ortho_triangle_one 	= Shape<3>(card::ortho_triangle_one,
														outputs_sequence_s3 {tree::output::number::left, tree::output::number::up, tree::output::number::down_right}, "Triangle_Rectangle_DL");
			inline const auto ortho_triangle_second = Shape<3>(card::ortho_triangle_second,
														outputs_sequence_s3 {tree::output::number::right, tree::output::number::down, tree::output::number::up_left}, "Triangle_Rectangle_UR");
			inline const auto iso_triangle_one 		= Shape<3>(card::iso_triangle_one,
														outputs_sequence_s3 {tree::output::number::left, tree::output::number::up_right, tree::output::number::down_right}, "Triangle_Iso_D_C");
			inline const auto iso_triangle_second 	= Shape<3>(card::iso_triangle_second,
														outputs_sequence_s3 {tree::output::number::right, tree::output::number::down_left, tree::output::number::up_left}, "Triangle_Iso_U_C");
			
		}

		namespace counterclockwise {

			namespace card {

				inline constexpr auto square 					= 0x0002; 	//DÉ
				inline constexpr auto rhombus 					= 0x0008; 	//Orange
				inline constexpr auto star 						= 0x0007; 	//jaune
				inline constexpr auto ortho_triangle_one 		= 0x000D; 	//5
				inline constexpr auto ortho_triangle_second 	= 0x0003; 	//3
				inline constexpr auto iso_triangle_one			= 0x0004;	//indigo
				inline constexpr auto iso_triangle_second		= 0x0016; 	// cercle

			}
			
			inline const auto square 				= Shape<4>(card::square,
														outputs_sequence_s4 {tree::output::number::up, tree::output::number::left, tree::output::number::down, tree::output::number::right}, "Square_CC");
			inline const auto rhombus 				= Shape<4>(card::rhombus,
														outputs_sequence_s4 {tree::output::number::up_left, tree::output::number::down_left, tree::output::number::down_right, tree::output::number::up_right}, "Rhombus_CC");
			inline const auto star 					= Shape<5>(card::star,
														outputs_sequence_s5 {tree::output::number::up_left, tree::output::number::down_left, tree::output::number::up_right, tree::output::number::left, tree::output::number::down_right}, "Star_CC");
			inline const auto ortho_triangle_one 	= Shape<3>(card::ortho_triangle_one,
														outputs_sequence_s3 {tree::output::number::down, tree::output::number::right, tree::output::number::up_left}, "Triangle_Rectangle_DL_CC");
			inline const auto ortho_triangle_second = Shape<3>(card::ortho_triangle_second,
														outputs_sequence_s3 {tree::output::number::up, tree::output::number::left, tree::output::number::down_right}, "Triangle_Rectangle_UR_CC");
			inline const auto iso_triangle_one		= Shape<3>(card::iso_triangle_one,
														outputs_sequence_s3 {tree::output::number::right, tree::output::number::up_left, tree::output::number::down_left}, "Triangle_Iso_D_CC");
			inline const auto iso_triangle_second 	= Shape<3>(card::iso_triangle_second,
														outputs_sequence_s3 {tree::output::number::left, tree::output::number::down_right, tree::output::number::up_right}, "Triangle_Iso_U_CC");
			
		}	
		//std::to_array to use
		const int SHAPE_NUMBER = 14;
		std::array<const motion::AbstractShape *, SHAPE_NUMBER> shapes_array = {
													&clockwise::square, 				&counterclockwise::square,
													&clockwise::rhombus, 				&counterclockwise::rhombus,
													&clockwise::star, 					&counterclockwise::star,
													&clockwise::ortho_triangle_one, 	&counterclockwise::ortho_triangle_one,
													&clockwise::ortho_triangle_second, 	&counterclockwise::ortho_triangle_second,
													&clockwise::iso_triangle_one, 		&counterclockwise::iso_triangle_one,
													&clockwise::iso_triangle_second, 	&counterclockwise::iso_triangle_second,
	};

	}

	namespace game {

		namespace card {

			inline constexpr auto start 	= 0x0006;

		}

		const int SEQUENCE_SIZE = 10;
		std::array<uint8_t, SEQUENCE_SIZE> output_class_sequence 	= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


		void translate_array(std::span<uint8_t> array)
		{
			for (int i = 0; i < motion::game::SEQUENCE_SIZE - 1; i++) {
				array[i] = array[i + 1];
			}
		}	
	}

}

#endif
// clang-format on
