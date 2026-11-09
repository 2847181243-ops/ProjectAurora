# ios run 101

sha: b895f0406c6f9d574e6b8d1023cdb51e6f70b984
status: failure
at: 2026-11-09T01:24:35Z

## errors


## tails

### gen.log
wrote Applications/Aurora-iOS/Generated/AuroraBuiltinModules.cpp with 4 module(s)

### toolchain.log
[32m==>[0m [1mDownloading bottle manifests[0m
✔︎ Bottle Manifest xcodegen (2.46.0)
[32m==>[0m [1mFetching downloads for: [32mxcodegen[39m[0m
✔︎ Bottle xcodegen (2.46.0)
[34m==>[0m [1mPouring xcodegen--2.46.0.arm64_tahoe.bottle.1.tar.gz[0m
🍺  /opt/homebrew/Cellar/xcodegen/2.46.0: 38 files, 7.4MB

### Applications/Aurora-iOS/archive.log
      ONScripterLabel::init() in libaurora_onscripter.a[31](ONScripterLabel.o)
      ONScripterLabel::init() in libaurora_onscripter.a[31](ONScripterLabel.o)
  "_TTF_Init", referenced from:
      ONScripterLabel::initSDL() in libaurora_onscripter.a[31](ONScripterLabel.o)
  "_TTF_OpenFont", referenced from:
      Fontinfo::openFont(char*, int, int) in libaurora_onscripter.a[9](FontInfo.o)
  "_TTF_RenderGlyph_Shaded", referenced from:
      ONScripterLabel::renderGlyph(_TTF_Font*, unsigned short) in libaurora_onscripter.a[30](ONScripterLabel_text.o)
  "getResource(char const*)", referenced from:
      ONScripterLabel::initSDL() in libaurora_onscripter.a[31](ONScripterLabel.o)
ld: symbol(s) not found for architecture arm64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

** ARCHIVE FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/InstallationBuildProductsLocation/Applications/Aurora.app/Aurora normal (in target 'Aurora' from project 'Aurora')
	Archiving project Aurora with scheme Aurora
(2 failures)

### Applications/Aurora-iOS/project.log
⚙️  Generating plists...
⚙️  Generating project...
⚙️  Writing project...
Created project at /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj

## archive tail
Undefined symbols for architecture arm64:
  "_IMG_Load", referenced from:
      ONScripterLabel::initSDL() in libaurora_onscripter.a[31](ONScripterLabel.o)
  "_IMG_LoadJPG_RW", referenced from:
      loadImage(char*, bool*, SDL_Surface*, BaseReader*) in libaurora_onscripter.a[14](Layer.o)
      ONScripterLabel::createSurfaceFromFile(char*, int*) in libaurora_onscripter.a[27](ONScripterLabel_image.o)
  "_IMG_Load_RW", referenced from:
      loadImage(char*, bool*, SDL_Surface*, BaseReader*) in libaurora_onscripter.a[14](Layer.o)
      ONScripterLabel::createSurfaceFromFile(char*, int*) in libaurora_onscripter.a[27](ONScripterLabel_image.o)
      ONScripterLabel::initSDL() in libaurora_onscripter.a[31](ONScripterLabel.o)
  "_Mix_AllocateChannels", referenced from:
      ONScripterLabel::openAudio(int, unsigned short, int) in libaurora_onscripter.a[31](ONScripterLabel.o)
  "_Mix_ChannelFinished", referenced from:
      ONScripterLabel::openAudio(int, unsigned short, int) in libaurora_onscripter.a[31](ONScripterLabel.o)
  "_Mix_CloseAudio", referenced from:
      ONScripterLabel::resetCommand() in libaurora_onscripter.a[19](ONScripterLabel_command.o)
      ONScripterLabel::defineresetCommand() in libaurora_onscripter.a[19](ONScripterLabel_command.o)
      ONScripterLabel::playOGG(int, unsigned char*, long, bool, int) in libaurora_onscripter.a[29](ONScripterLabel_sound.o)
      ONScripterLabel::playMP3() in libaurora_onscripter.a[29](ONScripterLabel_sound.o)
      ONScripterLabel::playMPEG(char const*, bool, bool, int, int, int, int) in libaurora_onscripter.a[29](ONScripterLabel_sound.o)
      ONScripterLabel::playMPEG(char const*, bool, bool, int, int, int, int) in libaurora_onscripter.a[29](ONScripterLabel_sound.o)
  "_Mix_FreeChunk", referenced from:
      ONScripterLabel::wavestopCommand() in libaurora_onscripter.a[19](ONScripterLabel_command.o)
      ONScripterLabel::loopbgmstopCommand() in libaurora_onscripter.a[19](ONScripterLabel_command.o)
      ONScripterLabel::loopbgmstopCommand() in libaurora_onscripter.a[19](ONScripterLabel_command.o)
      ONScripterLabel::flushEventSub(SDL_Event&) in libaurora_onscripter.a[24](ONScripterLabel_event.o)
