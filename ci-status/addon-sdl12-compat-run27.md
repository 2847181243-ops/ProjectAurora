# addon-sdl12-compat

sha: 0d08a8f9d78354b6be0b2520933a2b0152fd4fa2
status: failure
at: 2026-11-08T20:33:54Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/b/lib/sdl12_compat.xcodeproj: SDL: clang: error: linker command failed with exit code 1 (use -v to see invocation)
addon.log-
addon.log-note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'sdl12_compat')
addon.log-note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'SDL' from project 'sdl12_compat')
--
addon.log:** BUILD FAILED **
addon.log-
addon.log-
addon.log-The following build commands failed:

## tails

### addon.log
/Users/runner/work/ProjectAurora/ProjectAurora/b/lib/sdl12_compat.xcodeproj: SDL: clang: error: linker command failed with exit code 1 (use -v to see invocation)

note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'sdl12_compat')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'SDL' from project 'sdl12_compat')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'sdl12_compat')
** BUILD FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/Release-iphoneos/libSDL-1.2.1.2.79.dylib normal (in target 'SDL' from project 'sdl12_compat')
(1 failure)
b/lib/Release-iphoneos/libSDLmain.a
