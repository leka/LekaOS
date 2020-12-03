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
#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include "mbed.h"

class AudioStream {

public:

    /**
     * Get the number of channels
     *
     * @return          Number of channels in this stream
     */
    virtual uint32_t get_channels() = 0;

    /**
     * Get the size of each sample in bytes
     *
     * @return          Size of each sample in bytes
     */
    virtual uint32_t get_bytes_per_sample() = 0;

    /**
     * Get the sample rate
     *
     * @return          Sample rate in Hz
     */
    virtual uint32_t get_sample_rate() = 0;

    /**
     * Read from the audio stream
     *
     * @param data      Audio data
     * @param size      Size of audio data to read
     * @return          Size read or -1 if no more data
     */
    virtual int read(uint8_t *data, uint32_t size) = 0;

    /**
     * Close this stream
     *
     * Release the resources associated with this stream
     */
    virtual void close() = 0;

    virtual ~AudioStream() {}
};

#endif
