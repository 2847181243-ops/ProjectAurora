# ios run 217

sha: 5d37caf3e1b725b1dac3e72db51f6e17729697e1
status: failure
at: 2026-11-15T01:57:06Z

## errors


## tails

### gen.log
wrote Applications/Aurora-iOS/Generated/AuroraBuiltinModules.cpp with 5 module(s)

### toolchain.log
[32m==>[0m [1mDownloading bottle manifests[0m
✔︎ Bottle Manifest xcodegen (2.46.0)
[32m==>[0m [1mFetching downloads for: [32mxcodegen[39m[0m
✔︎ Bottle xcodegen (2.46.0)
[34m==>[0m [1mPouring xcodegen--2.46.0.arm64_tahoe.bottle.1.tar.gz[0m
🍺  /opt/homebrew/Cellar/xcodegen/2.46.0: 38 files, 7.4MB

### Applications/Aurora-iOS/archive.log
    
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c -ivfsstatcache /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/SDKStatCaches.noindex/iphoneos26.5-23F81a-688ef53f1462e2c8f657fdc38a81448fee66e23052a0157c865c76b268046726.sdkstatcache -target arm64-apple-ios16.0 -fmessage-length\=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit\=0 -fno-color-diagnostics -fmodules-prune-interval\=86400 -fmodules-prune-after\=345600 -fbuild-session-file\=/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/ModuleCache.noindex/Session.modulevalidation -fmodules-validate-once-per-build-session -Wnon-modular-include-in-framework-module -Werror\=non-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Werror\=return-type -Wdocumentation -Wunreachable-code -Wquoted-include-in-framework-header -Werror\=deprecated-objc-isa-usage -Werror\=objc-root-class -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wshorten-64-to-32 -Wpointer-sign -Wno-newline-eof -Wno-implicit-fallthrough -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -fstrict-aliasing -Wdeprecated-declarations -Wno-sign-conversion -Winfinite-recursion -Wcomma -Wblock-capture-autoreleasing -Wstrict-prototypes -Wno-semicolon-before-method-body -Wunguarded-availability @/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/7187679823f38a2a940e0043cdf9d637-common-args.resp -MMD -MT dependencies -MF /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/aurora_runtime_api.d --serialize-diagnostics /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/aurora_runtime_api.dia -c /Users/runner/work/ProjectAurora/ProjectAurora/SDK/AuroraRuntimeAPI/src/aurora_runtime_api.c -o /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/aurora_runtime_api.o

Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/InstallationBuildProductsLocation/Applications/Aurora.app/Aurora normal (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -Xlinker -reproducible -target arm64-apple-ios16.0 -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -Os -L/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/EagerLinkingTBDs/Release-iphoneos -L/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/BuildProductsPath/Release-iphoneos -L/Users/runner/work/ProjectAurora/ProjectAurora/krkrlib -L/Users/runner/work/ProjectAurora/ProjectAurora/onslib/lib -L/Users/runner/work/ProjectAurora/ProjectAurora/onslib/prefix/lib -F/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/EagerLinkingTBDs/Release-iphoneos -F/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/BuildProductsPath/Release-iphoneos -filelist /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora.LinkFileList -Xlinker -rpath -Xlinker /usr/lib/swift -Xlinker -rpath -Xlinker @executable_path -Xlinker -dead_strip -Xlinker -object_path_lto -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora_lto.o -Xlinker -final_output -Xlinker /Applications/Aurora.app/Aurora -Xlinker -dependency_info -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora_dependency_info.dat -stdlib\=libc++ -fobjc-arc -fobjc-link-runtime -L/Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift/iphoneos -L/usr/lib/swift -Xlinker -add_ast_path -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora.swiftmodule @/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora-linker-args.resp -lc++ -laurora_onscripter -lSDL-1.2 -lSDL2 -lSDLmain -lSDL_image -lSDL_ttf -lSDL_mixer -lpng -ljpeg -lfreetype -logg -lvorbis -lvorbisfile -lkrkrell -lsqlite3 -lresolv -lbz2 -lz -liconv -framework AVFoundation -framework AudioToolbox -framework CoreAudio -framework CoreFoundation -framework CoreGraphics -framework CoreMotion -framework GameController -framework OpenGLES -framework QuartzCore -framework UIKit -framework Security -framework SystemConfiguration -framework ImageIO -framework CoreVideo -framework CoreText -framework Accelerate -framework IOSurface -framework CoreMedia -framework VideoToolbox -framework CoreImage -framework CoreServices -framework Metal -framework MetalKit -o /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/InstallationBuildProductsLocation/Applications/Aurora.app/Aurora
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: ignoring duplicate libraries: '-lc++'
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: search path '/Users/runner/work/ProjectAurora/ProjectAurora/onslib/lib' not found
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: search path '/Users/runner/work/ProjectAurora/ProjectAurora/onslib/prefix/lib' not found
ld: library 'aurora_onscripter' not found
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
