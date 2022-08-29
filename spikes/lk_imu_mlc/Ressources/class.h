#ifndef _LEKA_SHAPES_CREATION_H_
#define _LEKA_SHAPES_CREATION_H_

#include <cstdint>
#include "Ressources/shape.h"

using namespace leka;


	//Interrupt Value
const std::uint8_t Idle 							= 1;
const std::uint8_t Down 							= 8;
const std::uint8_t Left 							= 4;
const std::uint8_t Right 							= 0;
const std::uint8_t Up 								= 7;
const std::uint8_t DiagDownLeft 					= 6;
const std::uint8_t DiagDownRight 					= 3;
const std::uint8_t DiagUpLeft 						= 5;
const std::uint8_t DiagUpRight 						= 2;

													//id Card:   which real symbol on this card:
std::uint16_t id_square_clockwise 					= 0x0015;	//Square
std::uint16_t id_square_counter_clockwise 			= 0x0002; 	//DÉ

std::uint16_t id_rhombus_clockwise 					= 0x0005; 	//Blue
std::uint16_t id_rhombus_counter_clockwise 			= 0x0008; 	//Orange


std::uint16_t id_triangle_clockwise 				= 0x0017;	//triangle
//std::uint16_t id_triangle_counter_clockwise 		= 0x0008; 	//Orange

std::uint16_t id_star_clockwise 					= 0x0018;	//Star
std::uint16_t id_star_counter_clockwise 			= 0x0007; 	//jaune
std::uint16_t id_triangle_rect_up_left_clockwise 	= 0x000A; 	//Zero
std::uint16_t id_triangle_rect_up_left_CC 			= 0x000B; 	//1
std::uint16_t id_TriangleRectUpRightC 				= 0x000C; 	//2
std::uint16_t id_TriangleRectUpRightCC 				= 0x000D; 	//3
std::uint16_t id_TriangleRectDownLeftC 				= 0x000E; 	//4
std::uint16_t id_TriangleRectDownLeftCC 			= 0x000F; 	//5
std::uint16_t id_TriangleRectDownRightC 			= 0x0010; 	//6
std::uint16_t id_TriangleRectDownRightCC 			= 0x0011; 	//7


std::uint16_t id_TriangleIsoLeftC 					= 0x0012;	//8
std::uint16_t id_TriangleIsoLeftCC 					= 0x0013; 	//9
std::uint16_t id_TriangleIsoRightC 					= 0x0004; 	//Indigo
std::uint16_t id_TriangleIsoRightCC 				= 0x0003; 	//Violet
std::uint16_t id_Circle_C 							= 0x0016;	//Circle
std::uint16_t id_Circle_CC 							= 0x0009;	//red



leka::Shape<4> squareC					("Square_Clockwise", id_square_clockwise, std::array<std::uint8_t, 4>{Up, Right, Down, Left}, "Square_C");
leka::Shape<4> squareCC					("Square_CounterClockwise", id_square_counter_clockwise, std::array<std::uint8_t, 4>{Up, Left, Down, Right}, "Square_CC");
leka::Shape<4> rhombusC					("Rhombus_Clockwise", id_rhombus_clockwise, std::array<std::uint8_t, 4>{DiagUpRight, DiagDownRight, DiagDownLeft, DiagUpLeft},"Rhombus_C");
leka::Shape<4> rhombusCC				("Rhombus_CounterClockwise", id_rhombus_counter_clockwise, std::array<std::uint8_t, 4>{DiagUpLeft, DiagDownLeft, DiagDownRight, DiagUpRight}, "Rhombus_CC");
leka::Shape<3> TriangleRectUpLeftC		("Triangle_Rectangle_Up_Left_Clockwise",id_triangle_rect_up_left_clockwise, std::array<std::uint8_t, 3>{Up, Right, DiagDownLeft}, "Triangle_Rectangle_UL");
leka::Shape<3> TriangleRectUpLeftCC		("Triangle_Rectangle_Up_Left_CounterClockwise",id_triangle_rect_up_left_CC, std::array<std::uint8_t, 3>{Left, Down, DiagUpRight}, "Triangle_Rectangle_UL_CC");
leka::Shape<3> TriangleRectUpRightC		("Triangle_Rectangle_Up_Right_Clockwise",id_TriangleRectUpRightC, std::array<std::uint8_t, 3>{Right, Down, DiagUpLeft}, "Triangle_Rectangle_UR");
leka::Shape<3> TriangleRectUpRightCC	("Triangle_Rectangle_Up_Right_CounterClockwise",id_TriangleRectUpRightCC, std::array<std::uint8_t, 3>{Up, Left, DiagDownRight}, "Triangle_Rectangle_UR_CC");
leka::Shape<3> TriangleRectDownLeftC	("Triangle_Rectangle_Down_Left_Clockwise",id_TriangleRectDownLeftC, std::array<std::uint8_t, 3>{Left, Up, DiagDownRight}, "Triangle_Rectangle_DL");
leka::Shape<3> TriangleRectDownLeftCC	("Triangle_Rectangle_Down_Left_CounterClockwise",id_TriangleRectDownLeftCC, std::array<std::uint8_t, 3>{Down, Right, DiagUpLeft}, "Triangle_Rectangle_DL_CC");
leka::Shape<3> TriangleRectDownRightC	("Triangle_Rectangle_Down_Right_Clockwise",id_TriangleRectDownRightC, std::array<std::uint8_t, 3>{Down, Left, DiagUpRight}, "Triangle_Rectangle_DR" );
leka::Shape<3> TriangleRectDownRightCC	("Triangle_Rectangle_DOwn_Right_CounterClockwise",id_TriangleRectDownRightCC, std::array<std::uint8_t, 3>{Right, Up, DiagDownLeft}, "Triangle_Rectangle_DR_CC");
leka::Shape<3> TriangleIsoLeftC			("Triangle_Isosceles_Left_Clockwise",id_TriangleIsoLeftC, std::array<std::uint8_t, 3>{Up, DiagDownRight, DiagDownLeft}, "Triangle_Iso_L_C");
leka::Shape<3> TriangleIsoLeftCC		("Triangle_Isosceles_Left_CounterClockwise",id_TriangleIsoLeftCC, std::array<std::uint8_t, 3>{Down, DiagUpRight, DiagUpLeft}, "Triangle_Iso_L_CC");
leka::Shape<3> TriangleIsoRightC		("Triangle_Isosceles_Right_Clockwise",id_TriangleIsoRightC, std::array<std::uint8_t, 3>{Down, DiagUpLeft, DiagUpRight}, "Triangle_Iso_R_C");
leka::Shape<3> TriangleIsoRightCC		("Triangle_Isosceles_Right_CounterClockwise",id_TriangleIsoRightCC, std::array<std::uint8_t, 3>{Up, DiagDownLeft, DiagDownRight}, "Triangle_Iso_R_CC");
leka::Shape<3> TriangleIsoUpC			("Triangle_Isosceles_Up_Clockwise", 0x0035,std::array<std::uint8_t, 3>{Right, DiagDownLeft, DiagUpLeft}, "Triangle_Iso_U_C");
leka::Shape<3> TriangleIsoUpCC			("Triangle_Isosceles_Up_CounterClockwise",0x0039, std::array<std::uint8_t, 3>{Left, DiagDownRight, DiagUpRight}, "Triangle_Iso_U_CC");
leka::Shape<3> TriangleIsoDownC			("Triangle_Isosceles_Down_Clockwise", 0x0040, std::array<std::uint8_t, 3>{Left, DiagUpRight, DiagDownLeft}, "Triangle_Iso_D_C");
leka::Shape<3> TriangleIsoDownCC		("Triangle_Isosceles_Down_CounterClockwise",0x0041, std::array<std::uint8_t, 3>{Right, DiagUpLeft, DiagDownLeft}, "Triangle_Iso_D_CC");
leka::Shape<5> starC					("Star_Clockwise", id_star_clockwise, std::array<std::uint8_t, 5>{DiagUpRight, DiagDownRight, DiagUpLeft, Right, DiagDownLeft}, "Star_C");
leka::Shape<5> starCC					("Star_CounterClockwise",id_star_counter_clockwise, std::array<std::uint8_t, 5>{DiagUpLeft, DiagDownLeft, DiagUpRight, Left, DiagDownRight}, "Star_CC");


std::array<leka::AbstractField*, 22> shapes = {&squareC, &squareCC,
										&rhombusC, &rhombusCC,
										&TriangleRectUpLeftC, &TriangleRectUpLeftCC, &TriangleRectUpRightC, &TriangleRectUpRightCC,
										&TriangleRectDownLeftC, &TriangleRectDownLeftCC, &TriangleRectDownRightC, &TriangleRectDownRightCC,
										&TriangleIsoLeftC, &TriangleIsoLeftCC, &TriangleIsoRightC, &TriangleIsoRightCC,
										&TriangleIsoUpC, &TriangleIsoUpCC, &TriangleIsoDownC, &TriangleIsoDownCC,
										&starC, &starCC
								};


//Interrupts
OutputClass inter_DiagDownLeft		("Diagonal Down Left", DiagDownLeft, "Down_Left");
OutputClass inter_DiagDownRight		("Diagonal Down Right", DiagDownRight, "Down_Right");
OutputClass inter_DiagUpLeft		("Diagonal Up Left", DiagUpLeft, "Up_Left");
OutputClass inter_DiagUpRight		("Diagonal Up Right", DiagUpRight, "Up_Right");
OutputClass inter_Idle				("Idle", Idle, "Idle");
OutputClass inter_Left				("Left", Left, "Left");
OutputClass inter_Right				("Right", Right, "Right");
OutputClass inter_Down				("Down", Down, "Down");
OutputClass inter_Up				("Up", Up, "Up");

std::array<OutputClass, 9> interrupts_array = {inter_DiagDownLeft, inter_DiagDownRight, inter_DiagUpLeft, inter_DiagUpRight,
												inter_Idle,
												inter_Left, inter_Right,inter_Down, inter_Up
											};

std::array<std::uint8_t, 10> interrupts_sequence = {0, 0, 0, 0, 0, 0, 0, 0 , 0, 0};




	//Circle
/*Interrupt inter_ArcDownRight("Arc Down Right", ArcDownRight, "Arc_Down_Right");
Interrupt inter_ArcUpRight("Arc Up Right", ArcUpRight, "Arc_Up_Right");
Interrupt inter_ArcDownLeft("Arc Down Left", ArcDownLeft, "Arc_Down_Left");
Interrupt inter_ArcUpLeft("Arc Up Left", ArcUpLeft, "Arc_Up_Left");*/

//leka::Shape<2> circleC("Circle_Clockwise", id_Circle_C, std::array<std::uint8_t, 2>{ArcDownRight, ArcUpLeft}, "Circle_C");
//leka::Shape<2> circleCC("Circle_CounterClockwise", id_Circle_CC, std::array<std::uint8_t, 2>{ArcUpLeft, ArcDownRight}, "Circle_CC");



#endif
