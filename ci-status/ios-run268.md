# ios run 268

sha: b00add871d03c58220ade1697d064128450abfef
status: failure
at: 2026-11-15T15:16:25Z

## errors


## tails

### gen.log
wrote Applications/Aurora-iOS/Generated/AuroraBuiltinModules.cpp with 6 module(s)

### toolchain.log
[32m==>[0m [1mDownloading bottle manifests[0m
✔︎ Bottle Manifest xcodegen (2.46.0)
[32m==>[0m [1mFetching downloads for: [32mxcodegen[39m[0m
✔︎ Bottle xcodegen (2.46.0)
[34m==>[0m [1mPouring xcodegen--2.46.0.arm64_tahoe.bottle.1.tar.gz[0m
🍺  /opt/homebrew/Cellar/xcodegen/2.46.0: 38 files, 7.4MB

### Applications/Aurora-iOS/archive.log
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_input_poll", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_input_state", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_video_refresh", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_unload_game", referenced from:
      aurora::psp::stopEngine() in psp_engine_libretro.o
ld: symbol(s) not found for architecture arm64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
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
  "_retro_deinit", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
      aurora::psp::stopEngine() in psp_engine_libretro.o
  "_retro_get_system_info", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_init", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_load_game", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_run", referenced from:
      aurora::psp::runFrame() in psp_engine_libretro.o
  "_retro_set_audio_sample", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_audio_sample_batch", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_environment", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_input_poll", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_input_state", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_set_video_refresh", referenced from:
      aurora::psp::startEngine(aurora::psp::Config const&, aurora::psp::Host const&) in psp_engine_libretro.o
  "_retro_unload_game", referenced from:
      aurora::psp::stopEngine() in psp_engine_libretro.o
