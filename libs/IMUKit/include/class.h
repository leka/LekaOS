// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_SHAPES_CREATION_H_
#define _LEKA_SHAPES_CREATION_H_

#include <cstdint>
#include "shape.h"

using namespace leka;

typedef typename std::uint8_t TreeOutput;
typedef typename std::uint16_t IDCard;
typedef typename std::array<TreeOutput, 2> outputs_sequence_s2; //Size 2
typedef typename std::array<TreeOutput, 3> outputs_sequence_s3;
typedef typename std::array<TreeOutput, 4> outputs_sequence_s4;
typedef typename std::array<TreeOutput, 5> outputs_sequence_s5;


	/****** CLASSES ******/

const TreeOutput TO_idle 							= 1;
const TreeOutput TO_down 							= 8;
const TreeOutput TO_left 							= 4;
const TreeOutput TO_right 							= 0;
const TreeOutput TO_up 								= 7;
const TreeOutput TO_diag_down_left 					= 6;
const TreeOutput TO_diag_down_right 				= 3;
const TreeOutput TO_diag_up_left 					= 5;
const TreeOutput TO_diag_up_right 					= 2;

OutputClass OC_diag_down_left						("Diagonal Down Left", TO_diag_down_left, "Down_Left");
OutputClass OC_diag_down_right						("Diagonal Down Right", TO_diag_down_right, "Down_Right");
OutputClass OC_diag_up_left							("Diagonal Up Left", TO_diag_up_left, "Up_Left");
OutputClass OC_diag_up_right						("Diagonal Up Right", TO_diag_up_right, "Up_Right");
OutputClass OC_idle									("Idle", TO_idle, "Idle");
OutputClass OC_left									("Left", TO_left, "Left");
OutputClass OC_right								("Right", TO_right, "Right");
OutputClass OC_down									("Down", TO_down, "Down");
OutputClass OC_up									("Up", TO_up, "Up");

std::array<OutputClass, 9> output_class_array 		= {OC_diag_down_left, OC_diag_down_right, OC_diag_up_left, OC_diag_up_right,
														OC_idle,
														OC_left, OC_right,OC_down, OC_up
													};

std::array<TreeOutput, 10> output_class_sequence 	= {0, 0, 0, 0, 0, 0, 0, 0 , 0, 0};


/****** SHAPES *****/
													//id Card:   which real symbol on this card:
const IDCard id_start_card 							= 0x0006;	//Start Card
const IDCard id_square_c							= 0x0015;	//Square
const IDCard id_square_cc 							= 0x0002; 	//DÉ
const IDCard id_rhombus_c 							= 0x0005; 	//Blue
const IDCard id_rhombus_cc 							= 0x0008; 	//Orange
const IDCard id_triangle_c 							= 0x0017;	//triangle
//std::uint16_t id_triangle_counter_c 				= 0x0008; 	//Orange
const IDCard id_star_c 								= 0x0018;	//Star
const IDCard id_star_cc 							= 0x0007; 	//jaune
const IDCard id_triangle_rect_up_left_c 			= 0x000A; 	//Zero
const IDCard id_triangle_rect_up_left_cc 			= 0x000B; 	//1
const IDCard id_triangle_rect_up_right_c 			= 0x000C; 	//2
const IDCard id_triangle_rect_up_right_cc 			= 0x000D; 	//3
const IDCard id_triangle_rect_down_left_c 			= 0x000E; 	//4
const IDCard id_triangle_rect_down_left_cc 			= 0x000F; 	//5
const IDCard id_triangle_rect_down_right_c 			= 0x0010; 	//6
const IDCard id_triangle_rect_down_right_cc 		= 0x0011; 	//7
const IDCard id_triangle_iso_left_c					= 0x0012;	//8
const IDCard id_triangle_iso_left_cc 				= 0x0013; 	//9
const IDCard id_triangle_iso_right_c				= 0x0004; 	//Indigo
const IDCard id_triangle_iso_right_cc 				= 0x0003; 	//Violet
const IDCard id_triangle_iso_up_c					= 0x0035;
const IDCard id_triangle_iso_up_cc					= 0x0039;
const IDCard id_triangle_iso_down_c					= 0x0040;
const IDCard id_triangle_iso_down_cc				= 0x0041;
const IDCard id_circle_c 							= 0x0016;	//Circle
const IDCard id_circle_cc 							= 0x0009;	//red


			// SHAPE								SHAPE NAME											ID CARD							SEQUENCE OF OUTPUT TO DO SHAPE																				NAME IMAGE
leka::Shape<4> square_c								("Square_Clockwise", 								id_square_c, 					outputs_sequence_s4 {TO_up, TO_right, TO_down, TO_left}, 													"Square_C");
leka::Shape<4> square_cc							("Square_CounterClockwise", 						id_square_cc, 					outputs_sequence_s4 {TO_up, TO_left, TO_down, TO_right}, 													"Square_CC");
leka::Shape<4> rhombus_c							("Rhombus_Clockwise", 								id_rhombus_c, 					outputs_sequence_s4 {TO_diag_up_right, TO_diag_down_right, TO_diag_down_left, TO_diag_up_left},				"Rhombus_C");
leka::Shape<4> rhombus_cc							("Rhombus_CounterClockwise", 						id_rhombus_cc, 					outputs_sequence_s4 {TO_diag_up_left, TO_diag_down_left, TO_diag_down_right, TO_diag_up_right}, 			"Rhombus_CC");
leka::Shape<3> Triangle_rect_up_left_c				("Triangle_Rectangle_Up_Left_Clockwise", 			id_triangle_rect_up_left_c, 	outputs_sequence_s3 {TO_up, TO_right, TO_diag_down_left}, 													"Triangle_Rectangle_UL");
leka::Shape<3> Triangle_rect_up_left_cc				("Triangle_Rectangle_Up_Left_CounterClockwise",		id_triangle_rect_up_left_cc, 	outputs_sequence_s3 {TO_left, TO_down, TO_diag_up_right}, 													"Triangle_Rectangle_UL_CC");
leka::Shape<3> Triangle_rect_up_right_c				("Triangle_Rectangle_Up_Right_Clockwise",			id_triangle_rect_up_right_c, 	outputs_sequence_s3 {TO_right,TO_down, TO_diag_up_left}, 													"Triangle_Rectangle_UR");
leka::Shape<3> Triangle_rect_up_right_cc			("Triangle_Rectangle_Up_Right_CounterClockwise",	id_triangle_rect_up_right_cc, 	outputs_sequence_s3 {TO_up, TO_left, TO_diag_down_right}, 													"Triangle_Rectangle_UR_CC");
leka::Shape<3> Triangle_rect_down_left_c			("Triangle_Rectangle_Down_Left_Clockwise",			id_triangle_rect_down_left_c, 	outputs_sequence_s3 {TO_left, TO_up, TO_diag_down_right}, 													"Triangle_Rectangle_DL");
leka::Shape<3> Triangle_rect_down_left_cc			("Triangle_Rectangle_Down_Left_CounterClockwise",	id_triangle_rect_down_left_cc, 	outputs_sequence_s3 {TO_down, TO_right, TO_diag_up_left}, 													"Triangle_Rectangle_DL_CC");
leka::Shape<3> Triangle_rect_down_right_c			("Triangle_Rectangle_Down_Right_Clockwise",			id_triangle_rect_down_right_c, 	outputs_sequence_s3 {TO_down, TO_left, TO_diag_up_right}, 													"Triangle_Rectangle_DR" );
leka::Shape<3> Triangle_rect_down_right_cc			("Triangle_Rectangle_DOwn_Right_CounterClockwise",	id_triangle_rect_down_right_cc, outputs_sequence_s3 {TO_right, TO_up, TO_diag_down_left}, 													"Triangle_Rectangle_DR_CC");
leka::Shape<3> Triangle_iso_left_c					("Triangle_Isosceles_Left_Clockwise",				id_triangle_iso_left_c, 		outputs_sequence_s3 {TO_up, TO_diag_down_right, TO_diag_down_left}, 										"Triangle_Iso_L_C");
leka::Shape<3> Triangle_iso_left_cc					("Triangle_Isosceles_Left_CounterClockwise",		id_triangle_iso_left_cc, 		outputs_sequence_s3 {TO_down, TO_diag_up_right, TO_diag_up_left}, 											"Triangle_Iso_L_CC");
leka::Shape<3> Triangle_iso_right_c					("Triangle_Isosceles_Right_Clockwise",				id_triangle_iso_right_c, 		outputs_sequence_s3 {TO_down, TO_diag_up_left, TO_diag_up_right},											"Triangle_Iso_R_C");
leka::Shape<3> Triangle_iso_right_cc				("Triangle_Isosceles_Right_CounterClockwise",		id_triangle_iso_right_cc, 		outputs_sequence_s3 {TO_up, TO_diag_down_left, TO_diag_down_right}, 										"Triangle_Iso_R_CC");
leka::Shape<3> Triangle_iso_up_c					("Triangle_Isosceles_Up_Clockwise", 				id_triangle_iso_up_c, 			outputs_sequence_s3 {TO_right, TO_diag_down_left, TO_diag_up_left}, 										"Triangle_Iso_U_C");
leka::Shape<3> Triangle_iso_up_cc					("Triangle_Isosceles_Up_CounterClockwise",			id_triangle_iso_up_cc, 			outputs_sequence_s3 {TO_left, TO_diag_down_right, TO_diag_up_right}, 										"Triangle_Iso_U_CC");
leka::Shape<3> Triangle_iso_down_c					("Triangle_Isosceles_Down_Clockwise", 				id_triangle_iso_down_c, 		outputs_sequence_s3 {TO_left, TO_diag_up_right, TO_diag_down_left}, 										"Triangle_Iso_D_C");
leka::Shape<3> Triangle_iso_down_cc					("Triangle_Isosceles_Down_CounterClockwise",		id_triangle_iso_down_cc, 		outputs_sequence_s3 {TO_right, TO_diag_up_left, TO_diag_down_left}, 										"Triangle_Iso_D_CC");
leka::Shape<5> star_c								("Star_Clockwise", 									id_star_c, 						outputs_sequence_s5 {TO_diag_up_right, TO_diag_down_right, TO_diag_up_left, TO_right, TO_diag_down_left}, 	"Star_C");
leka::Shape<5> star_cc								("Star_CounterClockwise",							id_star_cc, 					outputs_sequence_s5 {TO_diag_up_left, TO_diag_down_left, TO_diag_up_right, TO_left, TO_diag_down_right}, 	"Star_CC");


std::array<leka::AbstractField*, 22> shapes 		= {&square_c, &square_cc,
														&rhombus_c, &rhombus_cc,
														&Triangle_rect_up_left_c, &Triangle_rect_up_left_cc, &Triangle_rect_up_right_c, &Triangle_rect_up_right_cc,
														&Triangle_rect_down_left_c, &Triangle_rect_down_left_cc, &Triangle_rect_down_right_c, &Triangle_rect_down_right_cc,
														&Triangle_iso_left_c, &Triangle_iso_left_cc, &Triangle_iso_right_c, &Triangle_iso_right_cc,
														&Triangle_iso_up_c, &Triangle_iso_up_cc, &Triangle_iso_down_c, &Triangle_iso_down_cc,
														&star_c, &star_cc
													};





/*
	//Circle
leka::Shape<2> circle_c								("Circle_Clockwise", id_Circle_C, std::array<std::uint8_t, 2>{ArcDownRight, ArcUpLeft}, "Circle_C");
leka::Shape<2> circle_cc							("Circle_CounterClockwise", id_Circle_CC, std::array<std::uint8_t, 2>{ArcUpLeft, ArcDownRight}, "Circle_CC");

OutputClass OC_arc_down_right						("Arc Down Right", ArcDownRight, "Arc_Down_Right");
OutputClass OC_arc_up_right							("Arc Up Right", ArcUpRight, "Arc_Up_Right");
OutputClass OC_arc_down_left						("Arc Down Left", ArcDownLeft, "Arc_Down_Left");
OutputClass OC_arc_up_left							("Arc Up Left", ArcUpLeft, "Arc_Up_Left");

*/



#endif
