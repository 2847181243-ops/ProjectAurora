# ons-simrun run 24

sha: 8721ec31d8c972a025eadad21820a3bf66944cdc
status: success
at: 2026-11-09T13:51:53Z

## engine output

[onscripter] script directory: /Users/runner/Library/Developer/CoreSimulator/Devices/98A0BFA3-CEC3-4FD6-9916-5A2149774DBF/data/Containers/Data/Application/A6750875-8B7C-4D75-A6FA-283C66CEAE82/Library/Application Support/AuroraData/Games/minimal-ons
Adding path: /Users/runner/Library/Developer/CoreSimulator/Devices/98A0BFA3-CEC3-4FD6-9916-5A2149774DBF/data/Containers/Data/Application/A6750875-8B7C-4D75-A6FA-283C66CEAE82/Library/Application Support/AuroraData/RuntimeModules/com.aurora.runtime.onscripter/Saves/
[onscripter] video driver: uikit surface: 640x480
[onscripter] engine thread started
2026-11-09 13:51:09.927 Aurora[4566:18488] [aurora] autostart com.aurora.runtime.onscripter with /Users/runner/Library/Developer/CoreSimulator/Devices/98A0BFA3-CEC3-4FD6-9916-5A2149774DBF/data/Containers/Data/Application/A6750875-8B7C-4D75-A6FA-283C66CEAE82/Library/Application Support/AuroraData/Games/minimal-ons
	48  Aurora.debug.dylib                  0x000000010af47077 _ZN6aurora3ons7runLoopEv + 39
com.aurora.app: 4566

## app build tail

2026-11-09 13:47:52.837 appintentsmetadataprocessor[1769:8602] Starting appintentsmetadataprocessor export
2026-11-09 13:47:52.847 appintentsmetadataprocessor[1769:8602] warning: Metadata extraction skipped. No AppIntents.framework dependency found.

AppIntentsSSUTraining (in target 'Aurora' from project 'Aurora')
    cd /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS
    /Applications/Xcode_26.6.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/appintentsnltrainingprocessor --infoplist-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --temp-dir-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/ssu --bundle-id com.aurora.app --product-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app --extracted-metadata-path /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Metadata.appintents --metadata-file-list /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Intermediates.noindex/Aurora.build/Debug-iphonesimulator/Aurora.build/Aurora.DependencyMetadataFileList --source-file /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Info.plist --archive-ssu-assets
2026-11-09 13:47:52.882 appintentsnltrainingprocessor[1771:8605] Parsing options for appintentsnltrainingprocessor
2026-11-09 13:47:52.882 appintentsnltrainingprocessor[1771:8605] Starting AppIntents SSU YAML Generation
2026-11-09 13:47:52.887 appintentsnltrainingprocessor[1771:8605] No AppShortcuts found - Skipping.

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
	34  libSDL2-2.0.0.dylib                 0x000000010ad86386 UIKit_DestroyWindow + 390
	35  libSDL2-2.0.0.dylib                 0x000000010ad6c284 SDL_DestroyWindow + 308
	36  Aurora.debug.dylib                  0x000000010afb4e40 EndVidModeCreate + 192
	37  Aurora.debug.dylib                  0x000000010af9cad7 SDL_QuitSubSystem + 279
	38  Aurora.debug.dylib                  0x000000010af74ae9 _ZL16SDL_Quit_Wrapperv + 9
	39  libsystem_c.dylib                   0x000000010dc10d24 __cxa_finalize_ranges + 423
	40  libsystem_c.dylib                   0x000000010dc11046 exit + 22
	41  Aurora.debug.dylib                  0x000000010af4db24 _ZN15ONScripterLabel10endCommandEv + 52
	42  Aurora.debug.dylib                  0x000000010af5e184 _ZN15ONScripterLabel12runEventLoopEv + 1716
	43  Aurora.debug.dylib                  0x000000010af5dab5 _ZN15ONScripterLabel12waitEventSubEi + 293
	44  Aurora.debug.dylib                  0x000000010af5e243 _ZN15ONScripterLabel9waitEventEi + 35
	45  Aurora.debug.dylib                  0x000000010af55c98 _ZN15ONScripterLabel12clickCommandEv + 168
	46  Aurora.debug.dylib                  0x000000010af7a985 _ZN15ONScripterLabel9parseLineEv + 453
	47  Aurora.debug.dylib                  0x000000010af7a64c _ZN15ONScripterLabel12executeLabelEv + 668
	48  Aurora.debug.dylib                  0x000000010af47077 _ZN6aurora3ons7runLoopEv + 39
	49  Aurora.debug.dylib                  0x000000010aefa319 _ZZN12_GLOBAL__N_113startInstanceEPvENK3$_0clEv + 25
	50  Aurora.debug.dylib                  0x000000010aefa2d5 _ZNSt3__18__invokeB9dee210106IJZN12_GLOBAL__N_113startInstanceEPvE3$_0EEENS_20__invoke_result_implIvJDpT_EE4typeEDpOS5_ + 21
	51  Aurora.debug.dylib                  0x000000010aefa22d _ZNSt3__116__thread_executeB9dee210106INS_10unique_ptrINS_15__thread_structENS_14default_deleteIS2_EEEEZN12_GLOBAL__N_113startInstanceEPvE3$_0JEJEEEvRNS_5tupleIJT_T0_DpT1_EEENS_15__tuple_indicesI	52  Aurora.debug.dylib                  0x000000010aef9f2c _ZNSt3__114__thread_proxyB9dee210106INS_5tupleIJNS_10unique_ptrINS_15__thread_structENS_14default_deleteIS3_EEEEZN12_GLOBAL__N_113startInstanceEPvE3$_0EEEEES8_S8_ + 92
	53  libsystem_pthread.dylib             0x000000010e4e8d49 _pthread_start + 115
	54  libsystem_pthread.dylib             0x000000010e4e482f thread_start + 15
)
libc++abi: terminating due to uncaught exception of type NSException
com.aurora.app: 4566
getpwuid_r did not find a match for uid 501
Timestamp               Ty Process[PID:TID]
