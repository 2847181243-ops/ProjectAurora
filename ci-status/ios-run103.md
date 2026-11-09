# ios run 103

sha: accda144a3581debf343872a9274cbfae84d6385
status: failure
at: 2026-11-09T01:51:26Z

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
  "_ov_info", referenced from:
      _Mix_InitOgg in libSDL_mixer.a[8](dynamic_ogg.o)
  "_ov_open_callbacks", referenced from:
      _Mix_InitOgg in libSDL_mixer.a[8](dynamic_ogg.o)
  "_ov_pcm_total", referenced from:
      _Mix_InitOgg in libSDL_mixer.a[8](dynamic_ogg.o)
  "_ov_read", referenced from:
      _Mix_InitOgg in libSDL_mixer.a[8](dynamic_ogg.o)
  "_ov_time_seek", referenced from:
      _Mix_InitOgg in libSDL_mixer.a[8](dynamic_ogg.o)
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
  "_FT_Done_Face", referenced from:
      _TTF_CloseFont in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Done_Glyph", referenced from:
      _Find_Glyph in libSDL_ttf.a[2](SDL_ttf.o)
      _Find_Glyph in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Get_Char_Index", referenced from:
      _Find_Glyph in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Get_Glyph", referenced from:
      _Find_Glyph in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Glyph_Stroke", referenced from:
      _Find_Glyph in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Glyph_To_Bitmap", referenced from:
      _Find_Glyph in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Init_FreeType", referenced from:
      _TTF_Init in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Load_Glyph", referenced from:
      _Find_Glyph in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_MulFix", referenced from:
      _TTF_OpenFontIndexDPIRW in libSDL_ttf.a[2](SDL_ttf.o)
      _TTF_OpenFontIndexDPIRW in libSDL_ttf.a[2](SDL_ttf.o)
      _TTF_OpenFontIndexDPIRW in libSDL_ttf.a[2](SDL_ttf.o)
      _TTF_OpenFontIndexDPIRW in libSDL_ttf.a[2](SDL_ttf.o)
      _TTF_OpenFontIndexDPIRW in libSDL_ttf.a[2](SDL_ttf.o)
  "_FT_Open_Face", referenced from:
      _TTF_OpenFontIndexDPIRW in libSDL_ttf.a[2](SDL_ttf.o)
