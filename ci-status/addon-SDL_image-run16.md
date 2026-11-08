# addon-SDL_image

sha: 0d76e71d9f97f75cbd902189b19f6c54e69fa362
status: failure
at: 2026-11-08T18:19:44Z

## errors

addon.log:/Users/runner/work/ProjectAurora/ProjectAurora/b/lib/SDL2_image.xcodeproj: webpdemux: clang: error: no such file or directory: '/Users/runner/work/ProjectAurora/ProjectAurora/b/lib/external/libwebp/Release-iphoneos/libwebp.1.0.3.dylib'
addon.log-
addon.log-note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'SDL2_image')
addon.log-note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'SDL2_image')
--
addon.log:** BUILD FAILED **
addon.log-
addon.log-
addon.log-The following build commands failed:

## tails

### addon.log

note: Run script build phase 'Generate CMakeFiles/ZERO_CHECK' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ZERO_CHECK' from project 'SDL2_image')
note: Run script build phase 'Generate CMakeFiles/ALL_BUILD' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'ALL_BUILD' from project 'SDL2_image')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'webpdemux' from project 'SDL2_image')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'webp' from project 'SDL2_image')
note: Run script build phase 'CMake PostBuild Rules' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'tiff' from project 'SDL2_image')
** BUILD FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/b/lib/external/libwebp/Release-iphoneos/libwebpdemux.1.0.3.dylib normal (in target 'webpdemux' from project 'SDL2_image')
(1 failure)
