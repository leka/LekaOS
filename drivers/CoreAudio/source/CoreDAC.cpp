#include "CoreDAC.h"

namespace leka {

CoreDAC::CoreDAC(LKCoreSTM32HalBase &hal)
	: _hal(hal), _pCallbackCpt(nullptr), _pCallbackHlfCpt(nullptr)	 //, _dmaFlag(None)
{
	_hdac.Instance = DAC;
	_hdma.Instance = DMA1_Stream5;
}

}	// namespace leka
