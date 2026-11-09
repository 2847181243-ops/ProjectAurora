// SPDX-License-Identifier: GPL-3.0-or-later
//
// Audio-only SMPEG stand-in. The engine hands us an SDL_RWops pointing at an MP3
// (or MPEG stream); we decode it with AudioToolbox into PCM once, then serve it
// through SMPEG_playAudio() the way SMPEG would.

#include "smpeg.h"

#include <AudioToolbox/AudioToolbox.h>
#include <cstring>
#include <string>
#include <vector>

struct SMPEG {
    std::vector<Uint8> pcm;      // interleaved S16 stereo
    size_t position = 0;         // in frames
    int volume = 100;
    bool playing = false;
    bool looping = false;
    bool failed = false;
    bool has_audio = false;
    std::string error;
    int sample_rate = 44100;
    int channels = 2;
};

namespace {

struct MemoryReader {
    const Uint8* data = nullptr;
    SInt64 size = 0;
    SInt64 offset = 0;
};

OSStatus readCallback(void* client, SInt64 position, UInt32 request_count, void* buffer, UInt32* actual_count) {
    MemoryReader* reader = static_cast<MemoryReader*>(client);
    if (position < 0) return -1;
    SInt64 remaining = reader->size - position;
    if (remaining < 0) remaining = 0;
    UInt32 count = request_count < static_cast<UInt32>(remaining) ? request_count : static_cast<UInt32>(remaining);
    if (count > 0) std::memcpy(buffer, reader->data + position, count);
    *actual_count = count;
    return noErr;
}

OSStatus lengthCallback(void* client, SInt64* length) {
    *length = static_cast<MemoryReader*>(client)->size;
    return noErr;
}

/* Decodes the whole stream to 16 bit stereo at its native sample rate. */
bool decode(const std::vector<Uint8>& encoded, SMPEG* mpeg) {
    AudioFileID file = nullptr;
    MemoryReader reader{encoded.data(), static_cast<SInt64>(encoded.size()), 0};
    OSStatus status = AudioFileOpenWithCallbacks(&reader, readCallback,
                                                (AudioFile_WriteProc)nullptr,
                                                lengthCallback,
                                                (AudioFile_SetSizeProc)nullptr,
                                                0, &file);
    if (status != noErr || file == nullptr) {
        mpeg->error = "audio file could not be opened";
        return false;
    }

    ExtAudioFileRef ext = nullptr;
    status = ExtAudioFileWrapAudioFileID(file, false, &ext);
    if (status != noErr || ext == nullptr) {
        AudioFileClose(file);
        mpeg->error = "extended audio file open failed";
        return false;
    }

    AudioStreamBasicDescription source{};
    UInt32 size = sizeof(source);
    status = ExtAudioFileGetProperty(ext, kExtAudioFileProperty_FileDataFormat, &size, &source);
    if (status != noErr) {
        ExtAudioFileDispose(ext);
        AudioFileClose(file);
        mpeg->error = "could not read the stream format";
        return false;
    }

    mpeg->sample_rate = source.mSampleRate > 0 ? static_cast<int>(source.mSampleRate) : 44100;
    mpeg->channels = 2;

    AudioStreamBasicDescription target{};
    target.mSampleRate = mpeg->sample_rate;
    target.mFormatID = kAudioFormatLinearPCM;
    target.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    target.mChannelsPerFrame = mpeg->channels;
    target.mBitsPerChannel = 16;
    target.mFramesPerPacket = 1;
    target.mBytesPerFrame = 2 * mpeg->channels;
    target.mBytesPerPacket = target.mBytesPerFrame;
    status = ExtAudioFileSetProperty(ext, kExtAudioFileProperty_ClientDataFormat, sizeof(target), &target);
    if (status != noErr) {
        ExtAudioFileDispose(ext);
        AudioFileClose(file);
        mpeg->error = "could not configure the output format";
        return false;
    }

    const UInt32 frames_per_chunk = 4096;
    std::vector<Uint8> chunk(frames_per_chunk * target.mBytesPerFrame);
    for (;;) {
        AudioBufferList list{};
        list.mNumberBuffers = 1;
        list.mBuffers[0].mNumberChannels = target.mChannelsPerFrame;
        list.mBuffers[0].mDataByteSize = static_cast<UInt32>(chunk.size());
        list.mBuffers[0].mData = chunk.data();
        UInt32 frames = frames_per_chunk;
        status = ExtAudioFileRead(ext, &frames, &list);
        if (status != noErr || frames == 0) break;
        const size_t bytes = static_cast<size_t>(frames) * target.mBytesPerFrame;
        mpeg->pcm.insert(mpeg->pcm.end(), chunk.begin(), chunk.begin() + bytes);
    }

    ExtAudioFileDispose(ext);
    AudioFileClose(file);
    mpeg->has_audio = !mpeg->pcm.empty();
    return mpeg->has_audio;
}

SMPEG* create(SDL_RWops* src) {
    SMPEG* mpeg = new SMPEG();
    if (src == nullptr) {
        mpeg->error = "no stream";
        return mpeg;
    }
    std::vector<Uint8> encoded;
    Uint8 buffer[16384];
    for (;;) {
        const size_t got = SDL_RWread(src, buffer, 1, sizeof(buffer));
        if (got == 0) break;
        encoded.insert(encoded.end(), buffer, buffer + got);
    }
    SDL_RWclose(src);
    if (!decode(encoded, mpeg)) {
        mpeg->failed = true;
    }
    return mpeg;
}

} // namespace

extern "C" {

SMPEG* SMPEG_new_rwops(SDL_RWops* src, SMPEG_Info* info, int) {
    SMPEG* mpeg = create(src);
    if (info != nullptr) SMPEG_getinfo(mpeg, info);
    return mpeg;
}

void SMPEG_delete(SMPEG* mpeg) { delete mpeg; }

char* SMPEG_error(SMPEG* mpeg) {
    if (mpeg == nullptr) return "no stream";
    return mpeg->error.empty() ? nullptr : const_cast<char*>(mpeg->error.c_str());
}

void SMPEG_getinfo(SMPEG* mpeg, SMPEG_Info* info) {
    if (mpeg == nullptr || info == nullptr) return;
    std::memset(info, 0, sizeof(*info));
    info->has_audio = mpeg->has_audio ? 1 : 0;
    info->has_video = 0;
    info->current_fps = 0;
    info->length = mpeg->sample_rate > 0 ? static_cast<double>(mpeg->pcm.size() / (2 * 2)) / mpeg->sample_rate : 0.0;
    std::strncpy(info->description, "aurora audio shim", sizeof(info->description) - 1);
}

void SMPEG_wantedSpec(SMPEG* mpeg, SDL_AudioSpec* wanted) {
    if (wanted == nullptr) return;
    std::memset(wanted, 0, sizeof(*wanted));
    wanted->freq = mpeg != nullptr ? mpeg->sample_rate : 44100;
    wanted->format = AUDIO_S16SYS;
    wanted->channels = 2;
    wanted->samples = 4096;
}

void SMPEG_actualSpec(SMPEG*, SDL_AudioSpec*) {}
void SMPEG_enableaudio(SMPEG*, int) {}
void SMPEG_enablevideo(SMPEG*, int) {}

void SMPEG_setvolume(SMPEG* mpeg, int volume) {
    if (mpeg != nullptr) mpeg->volume = volume;
}

void SMPEG_play(SMPEG* mpeg) {
    if (mpeg != nullptr && mpeg->has_audio) mpeg->playing = true;
}

void SMPEG_pause(SMPEG* mpeg) {
    if (mpeg != nullptr) mpeg->playing = false;
}

void SMPEG_stop(SMPEG* mpeg) {
    if (mpeg == nullptr) return;
    mpeg->playing = false;
    mpeg->position = 0;
}

void SMPEG_rewind(SMPEG* mpeg) {
    if (mpeg != nullptr) mpeg->position = 0;
}

void SMPEG_loop(SMPEG* mpeg, int repeat) {
    if (mpeg != nullptr) mpeg->looping = repeat != 0;
}

SMPEGstatus SMPEG_status(SMPEG* mpeg) {
    if (mpeg == nullptr || !mpeg->has_audio) return SMPEG_ERROR;
    if (!mpeg->playing) return SMPEG_STOPPED;
    return (mpeg->position * 4 >= mpeg->pcm.size()) ? SMPEG_STOPPED : SMPEG_PLAYING;
}

/* Called from the engine's music hook to fill the audio stream. */
int SMPEG_playAudio(SMPEG* mpeg, Uint8* stream, int len) {
    if (mpeg == nullptr || stream == nullptr || len <= 0) return 0;
    const size_t available = mpeg->pcm.size() - (mpeg->position * 4);
    const size_t wanted = static_cast<size_t>(len) < available ? static_cast<size_t>(len) : available;
    if (wanted == 0) {
        if (mpeg->looping && !mpeg->pcm.empty()) {
            mpeg->position = 0;
        } else {
            std::memset(stream, 0, len);
            mpeg->playing = false;
            return 0;
        }
    }

    const Uint8* source = mpeg->pcm.data() + mpeg->position * 4;
    const int gain = mpeg->volume;
    const size_t frames = wanted / 4;
    Sint16* out = reinterpret_cast<Sint16*>(stream);
    const Sint16* in = reinterpret_cast<const Sint16*>(source);
    for (size_t i = 0; i < frames * 2; ++i) {
        out[i] = static_cast<Sint16>((static_cast<int>(in[i]) * gain) / 100);
    }
    mpeg->position += frames;
    if (wanted < static_cast<size_t>(len)) std::memset(stream + wanted, 0, len - wanted);
    return static_cast<int>(wanted);
}

void SMPEG_setdisplay(SMPEG*, SDL_Surface*, SDL_mutex*, void (*)(SDL_Surface*, int, int)) {}
void SMPEG_scaleXY(SMPEG*, int, int) {}
void SMPEG_move(SMPEG*, int, int) {}

} // extern "C"
