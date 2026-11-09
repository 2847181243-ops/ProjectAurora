# ios run 109

sha: c105f7c41addc9b0b1f5025f8640c570d07bb973
status: failure
at: 2026-11-09T02:50:44Z

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
  "_HVF_clear_part_cache", referenced from:
      _hvf_slot_load_glyph in libfreetype.a[29](hvf.o)
      _hvf_refresh_axis_coordinates in libfreetype.a[29](hvf.o)
  "_HVF_close_part_renderer", referenced from:
      _hvf_face_done in libfreetype.a[29](hvf.o)
  "_HVF_open_part_renderer", referenced from:
      _hvf_init_renderer in libfreetype.a[29](hvf.o)
  "_HVF_part_renderer_storage_size", referenced from:
      _hvf_init_renderer in libfreetype.a[29](hvf.o)
  "_HVF_render_current_part", referenced from:
      _hvf_slot_load_glyph in libfreetype.a[29](hvf.o)
  "_HVF_render_part_axis_count", referenced from:
      _hvf_slot_load_glyph in libfreetype.a[29](hvf.o)
  "_HVF_set_axis_value", referenced from:
      _hvf_slot_load_glyph in libfreetype.a[29](hvf.o)
  "_HVF_set_render_part", referenced from:
      _hvf_slot_load_glyph in libfreetype.a[29](hvf.o)
  "_IMG_Load", referenced from:
      ONScripterLabel::initSDL() in libaurora_onscripter.a[31](ONScripterLabel.o)
  "_IMG_LoadBMP_RW", referenced from:
      _supported in libSDL_image.a[12](IMG.o)
  "_IMG_LoadCUR_RW", referenced from:
      _supported in libSDL_image.a[12](IMG.o)
  "_IMG_LoadGIF_RW", referenced from:
      _supported in libSDL_image.a[12](IMG.o)
