# ios run 180

sha: 67c98f66bdf4250de627053fad821fd789731a8a
status: failure
at: 2026-11-12T19:06:59Z

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
Command line invocation:
    /Applications/Xcode_26.6.app/Contents/Developer/usr/bin/xcodebuild -project Aurora.xcodeproj -scheme Aurora -configuration Release -sdk iphoneos -destination generic/platform=iOS -derivedDataPath build CODE_SIGNING_ALLOWED=NO CODE_SIGNING_REQUIRED=NO CODE_SIGN_IDENTITY= DEVELOPMENT_TEAM= archive -archivePath build/Aurora.xcarchive

Build settings from command line:
    CODE_SIGN_IDENTITY = 
    CODE_SIGNING_ALLOWED = NO
    CODE_SIGNING_REQUIRED = NO
    DEVELOPMENT_TEAM = 
    SDKROOT = iphoneos26.5

2026-11-12 19:06:55.302 xcodebuild[2682:11772] Writing error result bundle to /var/folders/h5/t85nd63d71x27_8gl5gw3l_w0000gn/T/ResultBundle_2026-12-11_19-06-0055.xcresult
xcodebuild: error: 'Aurora.xcodeproj' does not exist.

### Applications/Aurora-iOS/project.log
Spec validation error: Target "Aurora" has the dependency "VideoToolbox.framework" multiple times

## archive tail
