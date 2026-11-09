# ons-simrun run 12

sha: b193e26af73e7ca15ea06be7e1cc605eba559f5c
status: success
at: 2026-11-09T09:57:29Z

## engine output


## app build tail

  "_TTF_FontAscent", referenced from:
      ONScripterLabel::drawGlyph(SDL_Surface*, Fontinfo*, SDL_Color&, char*, int*, bool, AnimationInfo*, SDL_Rect*, SDL_Rect&) in libaurora_onscripter.a[31](ONScripterLabel_text.o)
      ONScripterLabel::drawGlyph(SDL_Surface*, Fontinfo*, SDL_Color&, char*, int*, bool, AnimationInfo*, SDL_Rect*, SDL_Rect&) in libaurora_onscripter.a[31](ONScripterLabel_text.o)
  "_TTF_GlyphMetrics", referenced from:
      ONScripterLabel::drawGlyph(SDL_Surface*, Fontinfo*, SDL_Color&, char*, int*, bool, AnimationInfo*, SDL_Rect*, SDL_Rect&) in libaurora_onscripter.a[31](ONScripterLabel_text.o)
      ONScripterLabel::strpxlen(char const*, Fontinfo*) in libaurora_onscripter.a[31](ONScripterLabel_text.o)
      ONScripterLabel::init() in libaurora_onscripter.a[32](ONScripterLabel.o)
      ONScripterLabel::init() in libaurora_onscripter.a[32](ONScripterLabel.o)
      ONScripterLabel::init() in libaurora_onscripter.a[32](ONScripterLabel.o)
  "_TTF_Init", referenced from:
      ONScripterLabel::initSDL() in libaurora_onscripter.a[32](ONScripterLabel.o)
  "_TTF_OpenFont", referenced from:
      Fontinfo::openFont(char*, int, int) in libaurora_onscripter.a[9](FontInfo.o)
  "_TTF_RenderGlyph_Shaded", referenced from:
      ONScripterLabel::renderGlyph(_TTF_Font*, unsigned short) in libaurora_onscripter.a[31](ONScripterLabel_text.o)
ld: symbol(s) not found for architecture x86_64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

** BUILD FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Aurora.debug.dylib normal (in target 'Aurora' from project 'Aurora')
	Building project Aurora with scheme Aurora and configuration Debug
(2 failures)

## xcodegen tail
⚙️  Generating plists...
⚙️  Generating project...
⚙️  Writing project...
Created project at /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj

## onslib
include
lib
prefix
libaurora_onscripter.a

## simulator tail
