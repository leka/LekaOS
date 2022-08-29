#ifndef _LEKA_SHAPES_CREATION_H_
#define _LEKA_SHAPES_CREATION_H_

#include "shape.h"

using namespace leka;

const std::uint8_t DiagDownLeft = 0;
const std::uint8_t DiagDownRight = 1;
const std::uint8_t DiagUpLeft = 2;
const std::uint8_t DiagUpRight = 3;
const std::uint8_t Down = 4;
const std::uint8_t Idle = 5;
const std::uint8_t Left = 6;
const std::uint8_t Right = 7;
const std::uint8_t Up = 8;


//Créer une structure pour stocker ?
leka::Shape<4> squareC("Square_Clockwise", std::array<std::uint8_t, 4>{Up, Right, Down, Left});
leka::Shape<4> squareCC("Square_CounterClockwise", std::array<std::uint8_t, 4>{Up, Left, Down, Right});

leka::Shape<4> rhombusC("Rhombus_Clockwise", std::array<std::uint8_t, 4>{DiagUpRight, DiagDownRight, DiagDownLeft, DiagUpLeft});
leka::Shape<4> rhombusCC("Rhombus_CounterClockwise", std::array<std::uint8_t, 4>{DiagUpLeft, DiagDownLeft, DiagDownRight, DiagUpRight});

leka::Shape<3> TriangleRectUpLeftC("Triangle_Rectangle_Up_Left_Clockwise", std::array<std::uint8_t, 3>{Up, Right, DiagDownLeft});
leka::Shape<3> TriangleRectUpLeftCC("Triangle_Rectangle_Up_Left_CounterClockwise", std::array<std::uint8_t, 3>{Left, Down, DiagUpRight});
leka::Shape<3> TriangleRectUpRightC("Triangle_Rectangle_Up_Right_Clockwise", std::array<std::uint8_t, 3>{Right, Down, DiagUpLeft});
leka::Shape<3> TriangleRectUpRightCC("Triangle_Rectangle_Up_Right_CounterClockwise", std::array<std::uint8_t, 3>{Up, Left, DiagDownRight});
leka::Shape<3> TriangleRectDownLeftC("Triangle_Rectangle_Down_Left_Clockwise", std::array<std::uint8_t, 3>{Left, Up, DiagDownRight});
leka::Shape<3> TriangleRectDownLeftCC("Triangle_Rectangle_Down_Left_CounterClockwise", std::array<std::uint8_t, 3>{Down, Right, DiagUpLeft});
leka::Shape<3> TriangleRectDownRightC("Triangle_Rectangle_Down_Right_Clockwise", std::array<std::uint8_t, 3>{Down, Left, DiagUpRight});
leka::Shape<3> TriangleRectDownRightCC("Triangle_Rectangle_DOwn_Right_CounterClockwise", std::array<std::uint8_t, 3>{Right, Up, DiagDownLeft});

leka::Shape<3> TriangleIsoLeftC("Triangle_Isosceles_Left_Clockwise", std::array<std::uint8_t, 3>{Up, DiagDownRight, DiagDownLeft});
leka::Shape<3> TriangleIsoLeftCC("Triangle_Isosceles_Left_CounterClockwise", std::array<std::uint8_t, 3>{Down, DiagUpRight, DiagUpLeft});
leka::Shape<3> TriangleIsoRightC("Triangle_Isosceles_Right_Clockwise", std::array<std::uint8_t, 3>{Down, DiagUpLeft, DiagUpRight});
leka::Shape<3> TriangleIsoRightCC("Triangle_Isosceles_Right_CounterClockwise", std::array<std::uint8_t, 3>{Up, DiagDownLeft, DiagDownRight});
leka::Shape<3> TriangleIsoUpC("Triangle_Isosceles_Up_Clockwise", std::array<std::uint8_t, 3>{Right, DiagDownLeft, DiagUpLeft});
leka::Shape<3> TriangleIsoUpCC("Triangle_Isosceles_Up_CounterClockwise", std::array<std::uint8_t, 3>{Left, DiagDownRight, DiagUpRight});
leka::Shape<3> TriangleIsoDownC("Triangle_Isosceles_Down_Clockwise", std::array<std::uint8_t, 3>{Left, DiagUpRight, DiagDownLeft});
leka::Shape<3> TriangleIsoDownCC("Triangle_Isosceles_Down_CounterClockwise", std::array<std::uint8_t, 3>{Right, DiagUpLeft, DiagDownLeft});

leka::Shape<5> starC("Star_Clockwise", std::array<std::uint8_t, 5>{DiagUpRight, DiagDownRight, DiagUpLeft, Right, DiagDownLeft});
leka::Shape<5> starCC("Star_CounterClockwise", std::array<std::uint8_t, 5>{DiagUpLeft, DiagDownLeft, DiagUpRight, Left, DiagDownRight});

//Vector à changer
std::array<leka::AbstractField*, 22> shapes = {&squareC, &squareCC,
										&rhombusC, &rhombusCC,
										&TriangleRectUpLeftC, &TriangleRectUpLeftCC, &TriangleRectUpRightC, &TriangleRectUpRightCC,
										&TriangleRectDownLeftC, &TriangleRectDownLeftCC, &TriangleRectDownRightC, &TriangleRectDownRightCC,
										&TriangleIsoLeftC, &TriangleIsoLeftCC, &TriangleIsoRightC, &TriangleIsoRightCC,
										&TriangleIsoUpC, &TriangleIsoUpCC, &TriangleIsoDownC, &TriangleIsoDownCC,
										&starC, &starCC
								};



#endif
