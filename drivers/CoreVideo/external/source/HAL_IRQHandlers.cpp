#include "CoreVideo.hpp"

extern "C" {

namespace display::internal {
	extern leka::CoreVideo corevideo;
}

void DMA2D_IRQHandler()
{
	HAL_DMA2D_IRQHandler(&display::internal::corevideo.getDMA2DHandle());
}

void LTDC_IRQHandler()
{
	HAL_LTDC_IRQHandler(&display::internal::corevideo.getLTDCHandle());
}

void JPEG_IRQHandler()
{
	HAL_JPEG_IRQHandler(&display::internal::corevideo.getJPEGHandle());
}

void DMA2_Stream0_IRQHandler()
{
	HAL_DMA_IRQHandler(display::internal::corevideo.getJPEGHandle().hdmain);
}

void DMA2_Stream1_IRQHandler()
{
	HAL_DMA_IRQHandler(display::internal::corevideo.getJPEGHandle().hdmaout);
}

}	// extern "C"
