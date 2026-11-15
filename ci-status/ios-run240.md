# ios run 240

sha: 78a748705cbeb19bb85acfa112f5651a755ec580
status: failure
at: 2026-11-15T08:03:41Z

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
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -Xlinker -reproducible -target arm64-apple-ios16.0 -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -Os -L/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/EagerLinkingTBDs/Release-iphoneos -L/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/BuildProductsPath/Release-iphoneos -L/Users/runner/work/ProjectAurora/ProjectAurora/krkrlib -L/Users/runner/work/ProjectAurora/ProjectAurora/onslib/lib -L/Users/runner/work/ProjectAurora/ProjectAurora/onslib/prefix/lib -F/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/EagerLinkingTBDs/Release-iphoneos -F/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/BuildProductsPath/Release-iphoneos -filelist /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora.LinkFileList -Xlinker -rpath -Xlinker /usr/lib/swift -Xlinker -rpath -Xlinker @executable_path -Xlinker -dead_strip -Xlinker -object_path_lto -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora_lto.o -Xlinker -final_output -Xlinker /Applications/Aurora.app/Aurora -Xlinker -dependency_info -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora_dependency_info.dat -stdlib\=libc++ -fobjc-arc -fobjc-link-runtime -L/Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift/iphoneos -L/usr/lib/swift -Xlinker -add_ast_path -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora.swiftmodule @/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/Aurora-linker-args.resp -lc++ -laurora_onscripter -lSDL-1.2 -lSDL2 -lSDLmain -lSDL_image -lSDL_ttf -lSDL_mixer -lpng -ljpeg -lfreetype -logg -lvorbis -lvorbisfile -lkrkrell -lsqlite3 -lresolv -lbz2 -lz -liconv -framework AVFoundation -framework AudioToolbox -framework CoreAudio -framework CoreFoundation -framework CoreGraphics -framework CoreMotion -framework GameController -framework OpenGLES -framework QuartzCore -framework UIKit -framework Security -framework SystemConfiguration -framework ImageIO -framework CoreVideo -framework CoreText -framework Accelerate -framework IOSurface -framework CoreMedia -framework VideoToolbox -framework CoreImage -framework CoreServices -framework Metal -framework MetalKit -o /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/InstallationBuildProductsLocation/Applications/Aurora.app/Aurora
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: ignoring duplicate libraries: '-lc++'
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: Could not find or use auto-linked framework 'CoreAudioTypes': framework 'CoreAudioTypes' not found
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: Could not find or use auto-linked framework 'UIUtilities': framework 'UIUtilities' not found
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: Could not parse or use implicit file '/Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/System/Library/Frameworks/SwiftUICore.framework/SwiftUICore.tbd': cannot link directly with 'SwiftUICore' because product being built is not an allowed client of it
Undefined symbols for architecture arm64:
  "aurora::ons::screenSize(int&, int&)", referenced from:
      (anonymous namespace)::tickInstance(void*, double) in onscripter_module.o
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
  "aurora::ons::screenSize(int&, int&)", referenced from:
      (anonymous namespace)::tickInstance(void*, double) in onscripter_module.o
ld: symbol(s) not found for architecture arm64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** ARCHIVE FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/InstallationBuildProductsLocation/Applications/Aurora.app/Aurora normal (in target 'Aurora' from project 'Aurora')
	Archiving project Aurora with scheme Aurora
(2 failures)
