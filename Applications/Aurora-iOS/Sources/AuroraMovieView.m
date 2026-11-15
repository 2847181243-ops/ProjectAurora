// SPDX-License-Identifier: GPL-3.0-or-later
#import "AuroraMovieView.h"

#include <vector>

#include "pl_mpeg.h"

@interface AuroraMovieView () {
    plm_t *_mpeg;
    std::vector<uint8_t> _rgba;
    CGImageRef _currentFrame;
    double _clock;          /* seconds of playback we have advanced through */
    CFTimeInterval _lastTick;
    NSTimer *_timer;
}
@property (nonatomic, strong) CALayer *videoLayer;
@end

@implementation AuroraMovieView

+ (Class)layerClass { return [CALayer class]; }

- (instancetype)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
        self.backgroundColor = [UIColor blackColor];
        self.layer.masksToBounds = YES;
        _videoLayer = [CALayer layer];
        _videoLayer.contentsGravity = kCAGravityResizeAspect;
        _videoLayer.frame = self.bounds;
        [self.layer addSublayer:_videoLayer];
    }
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    _videoLayer.frame = self.bounds;
}

- (BOOL)hasVideo { return _mpeg != NULL; }
- (BOOL)playing { return _timer != nil; }
- (double)duration { return _mpeg != NULL ? plm_get_duration(_mpeg) : 0.0; }
- (double)position { return _mpeg != NULL ? plm_get_time(_mpeg) : 0.0; }

- (BOOL)openPath:(NSString *)path {
    [self pause];
    if (_mpeg != NULL) { plm_destroy(_mpeg); _mpeg = NULL; }
    if (_currentFrame != NULL) { CGImageRelease(_currentFrame); _currentFrame = NULL; }
    _videoLayer.contents = nil;

    _mpeg = plm_create_with_filename(path.fileSystemRepresentation);
    if (_mpeg == NULL) return NO;
    plm_set_audio_enabled(_mpeg, 0);
    plm_set_loop(_mpeg, 0);
    if (!plm_has_headers(_mpeg) || plm_get_width(_mpeg) <= 0) {
        plm_destroy(_mpeg);
        _mpeg = NULL;
        return NO;
    }
    _clock = 0.0;
    plm_frame_t *first = plm_decode_video(_mpeg);
    if (first != NULL) [self renderFrame:first];
    return YES;
}

- (void)play {
    if (_mpeg == NULL || _timer != nil) return;
    _lastTick = CACurrentMediaTime();
    __weak AuroraMovieView *weakSelf = self;
    _timer = [NSTimer scheduledTimerWithTimeInterval:1.0 / 30.0 repeats:YES block:^(NSTimer *timer) {
        [weakSelf tick];
    }];
}

- (void)pause {
    [_timer invalidate];
    _timer = nil;
}

- (void)tick {
    if (_mpeg == NULL) return;
    const CFTimeInterval now = CACurrentMediaTime();
    _clock += now - _lastTick;
    _lastTick = now;

    plm_frame_t *latest = NULL;
    int guard = 0;
    while (guard++ < 8) {
        const double stamp = plm_get_time(_mpeg);
        if (stamp > _clock) break;
        plm_frame_t *frame = plm_decode_video(_mpeg);
        if (frame == NULL) {
            /* Loop the opening: a title screen reel, not a one-shot. */
            plm_rewind(_mpeg);
            _clock = 0.0;
            break;
        }
        latest = frame;
    }
    if (latest != NULL) [self renderFrame:latest];
}

- (void)renderFrame:(plm_frame_t *)frame {
    const int width = (int)frame->width;
    const int height = (int)frame->height;
    if (width <= 0 || height <= 0) return;
    _rgba.resize((size_t)width * (size_t)height * 4);
    plm_frame_to_rgba(frame, _rgba.data(), width * 4);

    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    CGContextRef ctx = CGBitmapContextCreate(_rgba.data(), width, height, 8, (size_t)width * 4, space,
                                             kCGImageAlphaNoneSkipLast | kCGBitmapByteOrderDefault);
    if (ctx != NULL) {
        CGImageRef image = CGBitmapContextCreateImage(ctx);
        CGContextRelease(ctx);
        if (image != NULL) {
            if (_currentFrame != NULL) CGImageRelease(_currentFrame);
            _currentFrame = image;
            _videoLayer.contents = (__bridge id)image;
        }
    }
    CGColorSpaceRelease(space);
}

- (void)dealloc {
    [self pause];
    if (_mpeg != NULL) plm_destroy(_mpeg);
    if (_currentFrame != NULL) CGImageRelease(_currentFrame);
}

@end
