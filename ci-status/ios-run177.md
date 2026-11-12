# ios run 177

sha: d471ff94a3ee1f3657dd2d2a0511f216224e5745
status: failure
at: 2026-11-12T18:29:45Z

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
      (anonymous namespace)::tickInstance(void*, double) in krkr_module.o
  "aurora::krkr::create(aurora::krkr::Config const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>&)", referenced from:
      (anonymous namespace)::startInstance(void*) in krkr_module.o
  "aurora::krkr::destroy()", referenced from:
      (anonymous namespace)::startInstance(void*) in krkr_module.o
      (anonymous namespace)::stopInstance(void*) in krkr_module.o
      (anonymous namespace)::releaseInstance(void*) in krkr_module.o
  "aurora::krkr::openGame(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>&)", referenced from:
      (anonymous namespace)::startInstance(void*) in krkr_module.o
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
  "aurora::krkr::engineAvailable()", referenced from:
      (anonymous namespace)::startInstance(void*) in krkr_module.o
  "aurora::krkr::tick(unsigned int, aurora::krkr::Frame&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>&)", referenced from:
      (anonymous namespace)::tickInstance(void*, double) in krkr_module.o
  "aurora::krkr::create(aurora::krkr::Config const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>&)", referenced from:
      (anonymous namespace)::startInstance(void*) in krkr_module.o
  "aurora::krkr::destroy()", referenced from:
      (anonymous namespace)::startInstance(void*) in krkr_module.o
      (anonymous namespace)::stopInstance(void*) in krkr_module.o
      (anonymous namespace)::releaseInstance(void*) in krkr_module.o
  "aurora::krkr::openGame(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>&)", referenced from:
      (anonymous namespace)::startInstance(void*) in krkr_module.o
ld: symbol(s) not found for architecture arm64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** ARCHIVE FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/InstallationBuildProductsLocation/Applications/Aurora.app/Aurora normal (in target 'Aurora' from project 'Aurora')
	Archiving project Aurora with scheme Aurora
(2 failures)
