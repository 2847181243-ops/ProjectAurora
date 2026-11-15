# ios run 231

sha: e6596dc3e42f2c3465b58be5ccd4476348818158
status: failure
at: 2026-11-15T02:26:19Z

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
PrecompileModule /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/ExplicitPrecompiledModules/_Builtin_stdarg-24IZX3NM2IFZ3LZFKZ1S48C28.scan
    cd /
    builtin-precompileModule /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/ExplicitPrecompiledModules/_Builtin_stdarg-24IZX3NM2IFZ3LZFKZ1S48C28.scan

PrecompileModule /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/ExplicitPrecompiledModules/_AvailabilityInternal-7H17UP5FK32F38BT8D1AHJNS0.scan
    cd /
    builtin-precompileModule /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/ExplicitPrecompiledModules/_AvailabilityInternal-7H17UP5FK32F38BT8D1AHJNS0.scan

PrecompileModule /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/ExplicitPrecompiledModules/ptrcheck-1HIG6GVM2H7WM5DGHV3UE9SIT.scan
    cd /
    builtin-precompileModule /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/ExplicitPrecompiledModules/ptrcheck-1HIG6GVM2H7WM5DGHV3UE9SIT.scan

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** ARCHIVE FAILED **


The following build commands failed:
	CompileC /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/ArchiveIntermediates/Aurora/IntermediateBuildFilesPath/Aurora.build/Release-iphoneos/Aurora.build/Objects-normal/arm64/pl_mpeg_impl.o /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Sources/pl_mpeg_impl.c normal arm64 c com.apple.compilers.llvm.clang.1_0.compiler (in target 'Aurora' from project 'Aurora')
	Archiving project Aurora with scheme Aurora
(2 failures)

### Applications/Aurora-iOS/project.log
⚙️  Generating plists...
⚙️  Generating project...
⚙️  Writing project...
Created project at /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj

## archive tail
