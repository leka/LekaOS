/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef WAVE_AUDIO_STREAM_H
#define WAVE_AUDIO_STREAM_H

#include "mbed.h"
#include "AudioStream.h"

class WaveAudioStream : public AudioStream {

public:

    WaveAudioStream(File *file);

    /**
     * Get the number of channels
     *
     * @return          Number of channels in this stream
     * @note The behavior of this function is undefined if
     * the file is not valid.
     */
    virtual uint32_t get_channels();

    /**
     * Get the size of each sample in bytes
     *
     * @return          Size of each sample in bytes
     * @note The behavior of this function is undefined if
     * the file is not valid.
     */
    virtual uint32_t get_bytes_per_sample();

    /**
     * Get the sample rate
     *
     * @return          Sample rate in Hz
     * @note The behavior of this function is undefined if
     * the file is not valid.
     */
    virtual uint32_t get_sample_rate();

    /**
     * Check if the file is valid
     *
     * @return true if the file is a valid wav file, false otherwise
     * @note If this file is not valid then the only other function that
     * is safe to use is close.
     */
    virtual bool get_valid();

    /**
     * Read from the wave file
     *
     * @param data      Audio data
     * @param size      Size of audio data to read
     * @return          Size read or -1 if no more data
     * @note The behavior of this function is undefined if
     * the file is not valid.
     */
    virtual int read(uint8_t *data, uint32_t size);

    /**
     * Close the wave file
     *
     * Free any internal buffers and close the wave file
     */
    virtual void close();

    virtual ~WaveAudioStream();

protected:

    File *_file;
    bool _valid;
    uint32_t _channels;
    uint32_t _bytes_per_sample;
    uint32_t _sample_rate;
    uint32_t _audio_size;
    uint32_t _size_used;

};

#endif
