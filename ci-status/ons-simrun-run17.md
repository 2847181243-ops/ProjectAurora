# ons-simrun run 17

sha: 3f661c5ff6c178bda9bc8ebaba87bbd4f2e2f1af
status: success
at: 2026-11-09T11:39:39Z

## engine output

com.aurora.app: 4310

## app build tail

    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    builtin-swiftStdLibTool --copy --verbose --scan-executable /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Aurora.debug.dylib --scan-folder /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Frameworks --scan-folder /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/PlugIns --scan-folder /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/SystemExtensions --scan-folder /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Extensions --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/AVFoundation.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/AudioToolbox.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/CoreAudio.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/CoreFoundation.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/CoreGraphics.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/CoreMotion.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/GameController.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/OpenGLES.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/QuartzCore.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/UIKit.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/Metal.framework --scan-folder /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk/System/Library/Frameworks/MetalKit.framework --platform iphonesimulator --toolchain /var/run/com.apple.security.cryptexd/mnt/com.apple.MobileAsset.MetalToolchain-v17.6.109.0.kXa9On/Metal.xctoolchain --toolchain /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain --destination /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Frameworks --strip-bitcode --strip-bitcode-tool /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/bitcode_strip --emit-dependency-info /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/SwiftStdLibToolInputDependencies.dep --filter-for-swift-os --back-deploy-swift-span
Ignoring --strip-bitcode because --sign was not passed

AppIntentsSSUTraining (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/appintentsnltrainingprocessor --infoplist-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --temp-dir-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/ssu --bundle-id com.aurora.app --product-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app --extracted-metadata-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Metadata.appintents --metadata-file-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Aurora.DependencyMetadataFileList --source-file /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --archive-ssu-assets
2026-11-09 11:35:35.989 appintentsnltrainingprocessor[1993:9476] Parsing options for appintentsnltrainingprocessor
2026-11-09 11:35:35.990 appintentsnltrainingprocessor[1993:9476] Starting AppIntents SSU YAML Generation
2026-11-09 11:35:35.999 appintentsnltrainingprocessor[1993:9476] No AppShortcuts found - Skipping.

RegisterExecutionPolicyException /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    builtin-RegisterExecutionPolicyException /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app

Validate /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    builtin-validationUtility /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app -shallow-bundle -infoplist-subpath Info.plist

Touch /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /usr/bin/touch -c /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app

** BUILD SUCCEEDED **


## xcodegen tail
⚙️  Generating plists...
⚙️  Generating project...
⚙️  Writing project...
Created project at /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj

## undefined symbols

## onslib
include
lib
prefix
libaurora_onscripter.a

## simulator tail
[sdl12-compat] sdl12-compat: Failed loading SDL2 library.
com.aurora.app: 4310
