# ons-simrun run 16

sha: 836c560a90e7c12f15ccbb68820ee193e85b2896
status: success
at: 2026-11-09T11:21:14Z

## engine output


## app build tail

    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/appintentsmetadataprocessor --toolchain-dir /var/run/com.apple.security.cryptexd/mnt/com.apple.MobileAsset.MetalToolchain-v17.6.109.0.JCF52Q/Metal.xctoolchain --module-name Aurora --sdk-root /Applications/Xcode_26.6.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator26.5.sdk --xcode-version 17F113 --platform-family iOS --deployment-target 16.0 --bundle-identifier com.aurora.app --output /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app --target-triple x86_64-apple-ios16.0-simulator --binary-file /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Aurora --dependency-file /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora_dependency_info.dat --stringsdata-file /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/ExtractedAppShortcutsMetadata.stringsdata --source-file-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora.SwiftFileList --metadata-file-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Aurora.DependencyMetadataFileList --static-metadata-file-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Aurora.DependencyStaticMetadataFileList --swift-const-vals-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Objects-normal/x86_64/Aurora.SwiftConstValuesFileList --compile-time-extraction --deployment-aware-processing --validate-assistant-intents --no-app-shortcuts-localization
2026-11-09 11:14:19.879 appintentsmetadataprocessor[2488:10602] Starting appintentsmetadataprocessor export
2026-11-09 11:14:19.903 appintentsmetadataprocessor[2488:10602] warning: Metadata extraction skipped. No AppIntents.framework dependency found.

AppIntentsSSUTraining (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/appintentsnltrainingprocessor --infoplist-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --temp-dir-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/ssu --bundle-id com.aurora.app --product-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app --extracted-metadata-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Metadata.appintents --metadata-file-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Aurora.DependencyMetadataFileList --source-file /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --archive-ssu-assets
2026-11-09 11:14:20.058 appintentsnltrainingprocessor[2491:10625] Parsing options for appintentsnltrainingprocessor
2026-11-09 11:14:20.060 appintentsnltrainingprocessor[2491:10625] Starting AppIntents SSU YAML Generation
2026-11-09 11:14:20.073 appintentsnltrainingprocessor[2491:10625] No AppShortcuts found - Skipping.

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
