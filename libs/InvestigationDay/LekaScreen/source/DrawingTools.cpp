#include "DrawingTools.h"

namespace leka {

uint32_t Draw::frame_buffer_start_address = LCD_FRAME_BUFFER;	// hltdc.LayerCfg[1].FBStartAdress;
uint32_t Draw::data_address				  = frame_buffer_start_address;

Draw::Draw() {}

void Draw::clear(uint32_t color)
{
	filledRectangle(0, 0, screen_dimension.width, screen_dimension.height, color);
}

void Draw::filledRectangle(uint16_t x_0, uint16_t y_0, uint16_t width, uint16_t height, uint32_t color)
{
	// x_0 and y_0 refer to left corner of the rectangle
	uint8_t n_channel = 4;	 // ARGB
	data_address	  = frame_buffer_start_address + n_channel * (screen_dimension.width * y_0 + x_0);

	LKCoreDMA2D::load(LKCoreDMA2D::Mode::R2M, color, data_address, width, height, screen_dimension.width - width);
}

}	// namespace leka