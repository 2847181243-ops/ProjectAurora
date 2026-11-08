# addon-SDL_mixer

sha: 0d76e71d9f97f75cbd902189b19f6c54e69fa362
status: failure
at: 2026-11-08T18:19:35Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/b/lib/SDL2_mixer.xcodeproj: wavpack: clang: error: linker command failed with exit code 1 (use -v to see invocation)
addon.log-
addon.log-CompileC /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/build/opus.build/Release-iphoneos/Objects-normal/arm64/tables_pulses_per_block.o /Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib/external/opus/silk/tables_pulses_per_block.c normal arm64 c com.apple.compilers.llvm.clang.1_0.compiler (in target 'opus' from project 'SDL2_mixer')
addon.log-    cd /Users/runner/work/ProjectAurora/ProjectAurora/third_party/lib
--
addon.log:** BUILD FAILED **
addon.log-
addon.log-
addon.log-The following build commands failed:

## tails

### addon.log
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'xmp_shared' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'wavpack' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'opusfile' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'opus' from project 'SDL2_mixer')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ogg' from project 'SDL2_mixer')
** BUILD FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/external/wavpack/Release-iphoneos/libwavpack.1.2.5.dylib normal (in target 'wavpack' from project 'SDL2_mixer')
(1 failure)
