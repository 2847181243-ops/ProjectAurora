# ons-simrun run 21

sha: 06f5324112c978b5c5ad979798b49724dfe95e99
status: success
at: 2026-11-09T13:04:46Z

## engine output


## app build tail

2026-11-09 12:56:32.931 appintentsmetadataprocessor[2060:9466] Starting appintentsmetadataprocessor export
2026-11-09 12:56:32.957 appintentsmetadataprocessor[2060:9466] warning: Metadata extraction skipped. No AppIntents.framework dependency found.

AppIntentsSSUTraining (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/appintentsnltrainingprocessor --infoplist-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --temp-dir-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/ssu --bundle-id com.aurora.app --product-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app --extracted-metadata-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Metadata.appintents --metadata-file-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Aurora.DependencyMetadataFileList --source-file /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --archive-ssu-assets
2026-11-09 12:56:33.035 appintentsnltrainingprocessor[2061:9472] Parsing options for appintentsnltrainingprocessor
2026-11-09 12:56:33.037 appintentsnltrainingprocessor[2061:9472] Starting AppIntents SSU YAML Generation
2026-11-09 12:56:33.046 appintentsnltrainingprocessor[2061:9472] No AppShortcuts found - Skipping.

RegisterExecutionPolicyException /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    builtin-RegisterExecutionPolicyException /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app

Validate /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    builtin-validationUtility /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app -shallow-bundle -infoplist-subpath Info.plist

Touch /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /usr/bin/touch -c /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
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
