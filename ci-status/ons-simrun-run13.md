# ons-simrun run 13

sha: 55c1f1e0b9d41843bedac7d0fcc6a4212e0a6d69
status: success
at: 2026-11-09T10:15:55Z

## engine output


## app build tail

Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Aurora.debug.dylib normal (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -Xlinker -reproducible -target x86_64-apple-ios16.0-simulator -dynamiclib -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk -O0 -L/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/EagerLinkingTBDs/Debug-iphonesimulator -L/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator -L/Users/runner/work/ProjectAurora/ProjectAurora/onslib/lib -L/Users/runner/work/ProjectAurora/ProjectAurora/onslib/prefix/lib -F/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/EagerLinkingTBDs/Debug-iphonesimulator -F/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator -filelist /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora.LinkFileList -install_name @rpath/Aurora.debug.dylib -Xlinker -rpath -Xlinker /usr/lib/swift -Xlinker -rpath -Xlinker @executable_path/Frameworks -Xlinker -dead_strip -Xlinker -object_path_lto -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora_lto.o -rdynamic -Xlinker -no_deduplicate -Xlinker -objc_abi_version -Xlinker 2 -Xlinker -dependency_info -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora_dependency_info.dat -stdlib\=libc++ -fobjc-arc -fobjc-link-runtime -L/Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift/iphonesimulator -L/usr/lib/swift -Xlinker -add_ast_path -Xlinker /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora.swiftmodule @/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora-linker-args.resp -lc++ -laurora_onscripter -lSDL-1.2 -lSDL2 -lSDLmain -lSDL_image -lSDL_ttf -lSDL_mixer -lpng -ljpeg -lfreetype -logg -lvorbis -lvorbisfile -lbz2 -lz -liconv -Xlinker -alias -Xlinker _main -Xlinker ___debug_main_executable_dylib_entry_point -framework AVFoundation -framework AudioToolbox -framework CoreAudio -framework CoreFoundation -framework CoreGraphics -framework CoreMotion -framework GameController -framework OpenGLES -framework QuartzCore -framework UIKit -framework Metal -framework MetalKit -o /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Aurora.debug.dylib
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: ignoring duplicate libraries: '-lc++'
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: Could not find or use auto-linked framework 'UIUtilities': framework 'UIUtilities' not found
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: Could not parse or use implicit file '/Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk/System/Library/Frameworks/SwiftUICore.framework/SwiftUICore.tbd': cannot link directly with 'SwiftUICore' because product being built is not an allowed client of it
Undefined symbols for architecture x86_64:
  "_IMG_InitTIF", referenced from:
      _IMG_Init in libSDL_image.a[11](IMG.o)
  "_IMG_InitWEBP", referenced from:
      _IMG_Init in libSDL_image.a[11](IMG.o)
  "_IMG_QuitTIF", referenced from:
      _IMG_Quit in libSDL_image.a[11](IMG.o)
  "_IMG_QuitWEBP", referenced from:
      _IMG_Quit in libSDL_image.a[11](IMG.o)
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
