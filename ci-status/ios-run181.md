# ios run 181

sha: 1f6f943337a65646650414d7ca50e2bcb01ff597
status: failure
at: 2026-11-12T19:20:12Z

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
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: ignoring duplicate libraries: '-lc++'
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: Could not find or use auto-linked framework 'UIUtilities': framework 'UIUtilities' not found
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: ld: warning: Could not parse or use implicit file '/Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/System/Library/Frameworks/SwiftUICore.framework/SwiftUICore.tbd': cannot link directly with 'SwiftUICore' because product being built is not an allowed client of it
Undefined symbols for architecture arm64:
  "_TVPRegisterKrkrLive2DPluginAnchor", referenced from:
      _engine_create in libkrkrell.a[2](engine_api.cpp.o)
      (anonymous namespace)::OpenGameCore(engine_handle_s*, engine_handle_s*, char const*) in libkrkrell.a[2](engine_api.cpp.o)
  "_g_live2dRenderTarget", referenced from:
      (anonymous namespace)::GLESModule::copyLayerCb(TJS::tTJSVariant*, int, TJS::tTJSVariant**, (anonymous namespace)::GLESModule*) in libkrkrell.a[27](krkrgles.cpp.o)
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
  "_TVPRegisterKrkrLive2DPluginAnchor", referenced from:
      _engine_create in libkrkrell.a[2](engine_api.cpp.o)
      (anonymous namespace)::OpenGameCore(engine_handle_s*, engine_handle_s*, char const*) in libkrkrell.a[2](engine_api.cpp.o)
  "_g_live2dRenderTarget", referenced from:
      (anonymous namespace)::GLESModule::copyLayerCb(TJS::tTJSVariant*, int, TJS::tTJSVariant**, (anonymous namespace)::GLESModule*) in libkrkrell.a[27](krkrgles.cpp.o)
ld: symbol(s) not found for architecture arm64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** ARCHIVE FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/InstallationBuildProductsLocation/Applications/Aurora.app/Aurora normal (in target 'Aurora' from project 'Aurora')
	Archiving project Aurora with scheme Aurora
(2 failures)
