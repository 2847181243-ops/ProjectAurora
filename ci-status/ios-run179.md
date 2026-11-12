# ios run 179

sha: 16d43d6cbbb36effe5c61e9fe2f3003f68845a11
status: failure
at: 2026-11-12T18:53:55Z

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
      _vtenc_init in libkrkrell.a[2173](videotoolboxenc.o)
  "_kVTProfileLevel_H264_Main_3_2", referenced from:
      _vtenc_init in libkrkrell.a[2173](videotoolboxenc.o)
  "_kVTProfileLevel_H264_Main_4_0", referenced from:
      _vtenc_init in libkrkrell.a[2173](videotoolboxenc.o)
  "_kVTProfileLevel_H264_Main_4_1", referenced from:
      _vtenc_init in libkrkrell.a[2173](videotoolboxenc.o)
  "_kVTProfileLevel_H264_Main_5_0", referenced from:
      _vtenc_init in libkrkrell.a[2173](videotoolboxenc.o)
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
  "_CMBlockBufferCopyDataBytes", referenced from:
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
  "_CMBlockBufferCreateWithMemoryBlock", referenced from:
      _videotoolbox_common_end_frame in libkrkrell.a[2172](videotoolbox.o)
  "_CMSampleBufferCreate", referenced from:
      _videotoolbox_common_end_frame in libkrkrell.a[2172](videotoolbox.o)
  "_CMSampleBufferGetDataBuffer", referenced from:
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
  "_CMSampleBufferGetDecodeTimeStamp", referenced from:
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
  "_CMSampleBufferGetFormatDescription", referenced from:
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
      _vtenc_output_callback in libkrkrell.a[2173](videotoolboxenc.o)
  "_CMSampleBufferGetPresentationTimeStamp", referenced from:
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
  "_CMSampleBufferGetSampleAttachmentsArray", referenced from:
      _vtenc_frame in libkrkrell.a[2173](videotoolboxenc.o)
  "_CMSampleBufferGetTotalSampleSize", referenced from:
      _OUTLINED_FUNCTION_30 in libkrkrell.a[2173](videotoolboxenc.o)
  "_CMTimeMake", referenced from:
