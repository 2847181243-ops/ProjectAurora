# ios run 178

sha: 3d0a55067fc05226efbea521a94af3e6474e26e1
status: failure
at: 2026-11-12T18:44:17Z

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
    
    Using response file: /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/82b82416624d2658e5098eb0a28c15c5-common-args.resp
    
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -ivfsstatcache /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/SDKStatCaches.noindex/iphoneos26.5-23F81a-688ef53f1462e2c8f657fdc38a81448fee66e23052a0157c865c76b268046726.sdkstatcache -target arm64-apple-ios16.0 -fmessage-length\=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit\=0 -fno-color-diagnostics -fmodules-prune-interval\=86400 -fmodules-prune-after\=345600 -fbuild-session-file\=/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/ModuleCache.noindex/Session.modulevalidation -fmodules-validate-once-per-build-session -Wnon-modular-include-in-framework-module -Werror\=non-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Werror\=return-type -Wdocumentation -Wunreachable-code -Wquoted-include-in-framework-header -Werror\=deprecated-objc-isa-usage -Werror\=objc-root-class -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wno-c++11-extensions -Wno-implicit-fallthrough -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -fstrict-aliasing -Wdeprecated-declarations -Winvalid-offsetof -Wno-sign-conversion -Winfinite-recursion -Wmove -Wcomma -Wblock-capture-autoreleasing -Wstrict-prototypes -Wrange-loop-analysis -Wno-semicolon-before-method-body -Wunguarded-availability @/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/82b82416624d2658e5098eb0a28c15c5-common-args.resp -MMD -MT dependencies -MF /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/json.d --serialize-diagnostics /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/json.dia -c /Users/runner/work/ProjectAurora/ProjectAurora/SDK/AuroraModuleSDK/src/json.cpp -o /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/json.o

CompileC /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/input_profile.o /Users/runner/work/ProjectAurora/ProjectAurora/SDK/AuroraInputAPI/src/input_profile.cpp normal arm64 c++ com.apple.compilers.llvm.clang.1_0.compiler (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    
    Using response file: /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/82b82416624d2658e5098eb0a28c15c5-common-args.resp
    
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -ivfsstatcache /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/SDKStatCaches.noindex/iphoneos26.5-23F81a-688ef53f1462e2c8f657fdc38a81448fee66e23052a0157c865c76b268046726.sdkstatcache -target arm64-apple-ios16.0 -fmessage-length\=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit\=0 -fno-color-diagnostics -fmodules-prune-interval\=86400 -fmodules-prune-after\=345600 -fbuild-session-file\=/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/ModuleCache.noindex/Session.modulevalidation -fmodules-validate-once-per-build-session -Wnon-modular-include-in-framework-module -Werror\=non-modular-include-in-framework-module -Wno-trigraphs -Wno-missing-field-initializers -Wno-missing-prototypes -Werror\=return-type -Wdocumentation -Wunreachable-code -Wquoted-include-in-framework-header -Werror\=deprecated-objc-isa-usage -Werror\=objc-root-class -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wunused-function -Wno-unused-label -Wno-unused-parameter -Wunused-variable -Wunused-value -Wempty-body -Wuninitialized -Wconditional-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wconstant-conversion -Wint-conversion -Wbool-conversion -Wenum-conversion -Wno-float-conversion -Wnon-literal-null-conversion -Wobjc-literal-conversion -Wshorten-64-to-32 -Wno-newline-eof -Wno-c++11-extensions -Wno-implicit-fallthrough -isysroot /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS26.5.sdk -fstrict-aliasing -Wdeprecated-declarations -Winvalid-offsetof -Wno-sign-conversion -Winfinite-recursion -Wmove -Wcomma -Wblock-capture-autoreleasing -Wstrict-prototypes -Wrange-loop-analysis -Wno-semicolon-before-method-body -Wunguarded-availability @/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/82b82416624d2658e5098eb0a28c15c5-common-args.resp -MMD -MT dependencies -MF /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/input_profile.d --serialize-diagnostics /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/input_profile.dia -c /Users/runner/work/ProjectAurora/ProjectAurora/SDK/AuroraInputAPI/src/input_profile.cpp -o /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/input_profile.o

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** ARCHIVE FAILED **


The following build commands failed:
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/krkr_engine.o /Users/runner/work/ProjectAurora/ProjectAurora/Modules/KiriKiri/src/krkr_engine.cpp normal arm64 c++ com.apple.compilers.llvm.clang.1_0.compiler (in target 'Aurora' from project 'Aurora')
	Archiving project Aurora with scheme Aurora
(2 failures)

### Applications/Aurora-iOS/project.log
⚙️  Generating plists...
⚙️  Generating project...
⚙️  Writing project...
Created project at /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj

## archive tail
