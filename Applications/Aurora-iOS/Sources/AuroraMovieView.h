// SPDX-License-Identifier: GPL-3.0-or-later
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/* Plays MPEG-1 video (the .mpg openings most of these games ship). iOS has no
 * system decoder for that format on any version, so this decodes in software and
 * hands frames straight to a CALayer. Audio is not decoded yet. */
@interface AuroraMovieView : UIView

- (BOOL)openPath:(NSString *)path;
- (void)play;
- (void)pause;

@property (nonatomic, readonly) BOOL playing;
@property (nonatomic, readonly) double duration;
@property (nonatomic, readonly) double position;
@property (nonatomic, readonly) BOOL hasVideo;

@end

NS_ASSUME_NONNULL_END
