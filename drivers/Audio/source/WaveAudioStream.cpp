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

#include "WaveAudioStream.h"

// Define to printf for debugging
#define debug_printf(...)

//Riff structures

typedef struct {
    uint32_t identifier;
    uint32_t size;
} riff_chunk_t;

typedef struct {
    uint32_t identifier;
    uint32_t size;
    uint32_t format;
} riff_riff_t;

//Wave structures

typedef struct {
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
} wave_format_t;

/**
 * round the number up so it is even
 *
 * @param val   value to round up
 * @return      rounded up value
 */
static uint32_t round_up_even(uint32_t val) {
    return (val & 1 ? val + 1 : val);
}

//read chunk info, return 0 on success
/**
 * Read chunk info or return an error
 *
 * @param file          File to read from
 * @param chunk_info    Chunk structure to fill
 * @return      0 on success otherwise non-zero
 */
static int read_chunk_info(File *file, riff_chunk_t* chunk_info) {
    int length = 0;

    //read data

    length = file->read(chunk_info, sizeof(riff_chunk_t));
    if (length < 0) {
        debug_printf("read_chunk_info error: fread failed\n");
        return length;
    }
    if (length != sizeof(riff_chunk_t)) {
        debug_printf("read_chunk_info error: only able to read %i bytes\r\n", length);
        return 1;
    }

    return 0;
}

/**
 * read chunks until the specified one is found, return 0 on success
 * 
 * this function must be called when file position is at the start of a chunk
 *
 * @param file             File to read from
 * @param chunk_info       Chunk structure to fill
 * @param name             Name of the chunk
 * @return                 0 on success, otherwise non-zero
 */
static int read_chunk_info_skip(File *file, riff_chunk_t* chunk_info, const char *name) {
    riff_chunk_t current_chunk;
    int result;

    //chunk name must be 4 bytes
    if (strlen(name) != 4) {
        debug_printf("read_chunk_info_skip: Input string not long enough\n");
        return -1;
    }

    //read initial chunk
    result = read_chunk_info(file,&current_chunk);
    if (result != 0) {
        debug_printf("read_chunk_info_skip: read_chunk_info problem\r\n");
        return -1;
    }
    
    //search until chunk is found
    while (memcmp(&(current_chunk.identifier),name,4) != 0) {

        result = file->seek(round_up_even(current_chunk.size), SEEK_CUR);
        if (result < 0) {
            debug_printf("read_chunk_info_skip: Problem Seeking\n");
            return -1;
        }

        result = read_chunk_info(file,&current_chunk);
        if (result != 0) {
            debug_printf("read_chunk_info_skip: read_chunk_info problem\r\n");
            return -1;
        }
    }
    *chunk_info = current_chunk;

    return 0;
}

WaveAudioStream::WaveAudioStream(File *file)
    : _file(file), _valid(false), _channels(0), _bytes_per_sample(0),
      _sample_rate(0), _audio_size(0), _size_used(0)
{
    // Read file header
    riff_riff_t file_header;
    int length = file->read(&file_header, sizeof(file_header));
    if (length < 0) {
        debug_printf("Error reading from file\r\n");
        return;
    }
    if (length != sizeof(file_header)) {
        debug_printf("File not large enough\r\n");
        return;
    }
    if (memcmp(&(file_header.identifier), "RIFF", 4) != 0) {
        debug_printf("Incorrect header\r\n");
        return;
    }
    if (memcmp(&(file_header.format), "WAVE", 4) != 0) {
        debug_printf("File is not a wave\r\n");
        return;
    }
    debug_printf("RIFF data size = %lu\r\n",file_header.size);


    // Read chunks until format chunk is found
    riff_chunk_t current_chunk;
    int result = read_chunk_info_skip(file, &current_chunk, "fmt ");
    if (result != 0) {
        debug_printf("Problem reading \"fmt \" chunk\r\n");
        return;
    }
    
    if (current_chunk.size < sizeof(wave_format_t)) {
        debug_printf("\"fmt \" chunk has wrong size\r\n");
        return;
    }

    // Read format chunk
    wave_format_t format;
    length = file->read(&format, sizeof(format));
    if (length < 0) {
        debug_printf("Error reading from file\r\n");
        return;
    }
    if (length != sizeof(format)) {
        debug_printf("Error reading format\r\n");
        return;
    }
    if (format.audio_format == 1) {
        debug_printf("File is PCM type\r\n");
    }
    else if (format.audio_format == 0xFFFE){
        debug_printf("File is extensible type\r\n");
    }
    else{
        debug_printf("File is not PCM or Extensible type\r\n");
        return;
    }
    // Seek past the rest of the fmt chunk to the next chunk header
    file->seek(current_chunk.size - length, SEEK_CUR);
    
    // Print out file info
    debug_printf("Bits Per Sample = %i\r\n", (int)format.bits_per_sample);
    debug_printf("Block Align = %i\r\n", (int)format.block_align);
    debug_printf("Byte Rate = %i\r\n", (int)format.byte_rate);
    debug_printf("Number of Channels = %i\r\n", (int)format.num_channels);
    debug_printf("Sample Rate = %i\r\n", (int)format.sample_rate);

    // Read the data chunk
    result = read_chunk_info_skip(file, &current_chunk, "data");
    if (result != 0) {
        debug_printf("Problem reading \"data\" chunk\r\n");
        return;
    }
    debug_printf("Data chunk size (subchunk2size)= %lu\r\n",current_chunk.size);

    _channels = format.num_channels;
    _bytes_per_sample = (format.bits_per_sample + 7) / 8;
    _sample_rate = format.sample_rate;
    _audio_size = current_chunk.size;
    _valid = true;
}

uint32_t WaveAudioStream::get_channels()
{
    MBED_ASSERT(_valid);
    return _channels;
}

uint32_t WaveAudioStream::get_bytes_per_sample()
{
    MBED_ASSERT(_valid);
    return _bytes_per_sample;
}

uint32_t WaveAudioStream::get_sample_rate()
{
    MBED_ASSERT(_valid);
    return _sample_rate;
}

bool WaveAudioStream::get_valid()
{
    return _valid;
}

int WaveAudioStream::read(uint8_t *data, uint32_t size)
{
    MBED_ASSERT(_valid);
    uint32_t size_left = _audio_size - _size_used;
    uint32_t read_size = size < size_left ? size : size_left;

    if (read_size == 0) {
        return -1;
    }

    int result = _file->read(data, read_size);
    if (result < 0) {
        return -1;
    }
    _size_used += result;
    return result;
}

void WaveAudioStream::close()
{
    if (_file != NULL) {
        _file->close();
        _file = NULL;
    }
}

WaveAudioStream::~WaveAudioStream()
{
    close();
}

