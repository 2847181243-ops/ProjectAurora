/* SPDX-License-Identifier: GPL-3.0-or-later
 *
 * A small stand-in for the ONScripter SMPEG fork.
 *
 * ONScripter calls a patched SMPEG that takes a destination surface straight from
 * SMPEG_setdisplay() and positions it with SMPEG_scaleXY()/SMPEG_move(). Neither
 * icculus/smpeg nor SMPEG2 expose that API, and the engine only ever uses it for
 * MP3 background music and MPEG movies. This header declares exactly what the
 * engine calls; aurora_smpeg.mm implements the audio half with AudioToolbox and
 * treats video as unsupported.
 */
#ifndef AURORA_SMPEG_H
#define AURORA_SMPEG_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SMPEG SMPEG;

typedef enum {
    SMPEG_ERROR = -1,
    SMPEG_STOPPED,
    SMPEG_PLAYING
} SMPEGstatus;

typedef struct SMPEG_Info {
    int has_audio;
    int has_video;
    int width;
    int height;
    int current_frame;
    int current_fps;
    double length;
    char description[256];
} SMPEG_Info;

SMPEG* SMPEG_new_rwops(SDL_RWops* src, SMPEG_Info* info, int sdl_audio);
void SMPEG_delete(SMPEG* mpeg);

char* SMPEG_error(SMPEG* mpeg);
void SMPEG_getinfo(SMPEG* mpeg, SMPEG_Info* info);

void SMPEG_wantedSpec(SMPEG* mpeg, SDL_AudioSpec* wanted);
void SMPEG_actualSpec(SMPEG* mpeg, SDL_AudioSpec* spec);

void SMPEG_enableaudio(SMPEG* mpeg, int enable);
void SMPEG_enablevideo(SMPEG* mpeg, int enable);
void SMPEG_setvolume(SMPEG* mpeg, int volume);

void SMPEG_play(SMPEG* mpeg);
void SMPEG_pause(SMPEG* mpeg);
void SMPEG_stop(SMPEG* mpeg);
void SMPEG_rewind(SMPEG* mpeg);
void SMPEG_loop(SMPEG* mpeg, int repeat);

SMPEGstatus SMPEG_status(SMPEG* mpeg);
int SMPEG_playAudio(SMPEG* mpeg, Uint8* stream, int len);

/* Video positioning: accepted and ignored, movies are not rendered yet. */
void SMPEG_setdisplay(SMPEG* mpeg, SDL_Surface* surface, SDL_mutex* lock,
                      void (*callback)(SDL_Surface*, int, int));
void SMPEG_scaleXY(SMPEG* mpeg, int width, int height);
void SMPEG_move(SMPEG* mpeg, int x, int y);

#ifdef __cplusplus
}
#endif

#endif /* AURORA_SMPEG_H */
