// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"



//

namespace leka {

uint16_t CoreAudio::_waveBuffer[512];

CoreAudio::CoreAudio(LKCoreSTM32HalBase &hal,CoreDAC &dac, CoreDACTimer &timer) : _hal(hal), _coreDac(dac) ,_coreTimer(timer), _volume(10) {}


void CoreAudio::playFile(FIL* file)
{
    WavFile wavFile(file);
    uint16_t* _waveBuffer_2 = _waveBuffer + 256;

    //fillBufferWithSinWave(_waveBuffer, 512, 220 , 44100, 0xFFF, 0);

    printf("Will Initialize CoreAudio\n");
    _initialize(wavFile.header().SamplingRate);
    printf("CoreAudio Initialized\n");

    WavReader::loadSector(&wavFile, _waveBuffer, 512);
    _scaleToVolume(_waveBuffer, 256);
    _align12bR(_waveBuffer, 256);
    //bool eof = false;
    bool eof = WavReader::loadSector(&wavFile, _waveBuffer_2, 512);
    _scaleToVolume(_waveBuffer_2, 256);
    _align12bR(_waveBuffer_2, 256);


    _coreTimer.start();
    _coreDac.start(_waveBuffer, 512);
    
    while(!eof) 
    {
        if(_coreDac.dmaFlag() == CoreDAC::Half_cpt)
        {
            eof = WavReader::loadSector(&wavFile, _waveBuffer, 512);
            _scaleToVolume(_waveBuffer, 256);
            _align12bR(_waveBuffer, 256);
            _coreDac.dmaFlag() = CoreDAC::None;
            //printf("half DMA\n");
        }
        
        if(_coreDac.dmaFlag() == CoreDAC::Cpt)
        {
            if(!eof)
            {
                eof = WavReader::loadSector(&wavFile, _waveBuffer_2, 512);
                _scaleToVolume(_waveBuffer_2, 256);
                _align12bR(_waveBuffer_2, 256);
            }
            _coreDac.dmaFlag() = CoreDAC::None;
            //printf("cpt DMA\n");
        } 
    }

    stop();  
}

void CoreAudio::pause()
{
    //TODO(samhadjes) : implement pause/resume
}

void CoreAudio::resume()
{
    //TODO(samhadjes) : implement pause/resume
}

void CoreAudio::stop()
{
    _coreTimer.stop();
    _coreDac.stop();
}
    // PRIVATE

void CoreAudio::_initialize(float frequency)
{
    printf("Initialize CoreTimer\n");
    _coreTimer.initialize(frequency);
    printf("CoreTimer Initialized\n");
    printf("Initialize CoreDAC\n");
    _coreDac.initialize();
    printf("CoreDAC Initialized\n");
}

void CoreAudio::_align12bR(uint16_t *buffer, uint16_t length)
{
    for(int i = 0; i<length; ++i)
    {
        *buffer = *buffer >> 4;
    }
}

void CoreAudio::_scaleToVolume(uint16_t *buffer, uint16_t length)
{
    for(int i = 0; i<length; ++i)
    {
        //*buffer = static_cast<double>(*buffer) * (_volume/100.F);
        *buffer = static_cast<double>(*buffer) /6.F;
        //*buffer += 0x7FFF * (1.F - _volume/100.F);
    }
}


// Fill buffer with sin wave at given frequency and sampling rate
// Values can be limited between maxValue and minValue
void CoreAudio::fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate , uint16_t maxValue, uint16_t minValue)
{
	uint32_t samplesPerPeriod = (samplingRate / frequency);

	for(uint32_t i = 0; i < bufferSize; ++i) {
		float tmp = 0.5 * sin(i * 2.0 * M_PI / samplesPerPeriod) + 0.5;
		tmp *= maxValue-minValue;
		buffer[i] = tmp + minValue;
	}
}

}	// namespace leka
