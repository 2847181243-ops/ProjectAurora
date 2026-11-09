# ons-simrun run 20

sha: 1b9daf956751b57274d240159d2216e56df6e8c8
status: success
at: 2026-11-09T12:40:11Z

## engine output


## app build tail

Undefined symbols for architecture x86_64:
  "aurora::ons::initEngine(aurora::ons::Config const&)", referenced from:
      (anonymous namespace)::startInstance(void*) in onscripter_module.o
  "aurora::ons::requestStop()", referenced from:
      (anonymous namespace)::stopInstance(void*) in onscripter_module.o
      (anonymous namespace)::releaseInstance(void*) in onscripter_module.o
  "aurora::ons::describeVideo()", referenced from:
      (anonymous namespace)::startInstance(void*) in onscripter_module.o
  "aurora::ons::shutdownEngine()", referenced from:
      (anonymous namespace)::startInstance(void*)::$_0::operator()() const in onscripter_module.o
  "aurora::ons::engineAvailable()", referenced from:
      (anonymous namespace)::startInstance(void*) in onscripter_module.o
  "aurora::ons::runLoop()", referenced from:
      (anonymous namespace)::startInstance(void*)::$_0::operator()() const in onscripter_module.o
ld: symbol(s) not found for architecture x86_64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** BUILD FAILED **


The following build commands failed:
	Ld /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/build/Build/Products/Debug-iphonesimulator/Aurora.app/Aurora.debug.dylib normal (in target 'Aurora' from project 'Aurora')
	Building project Aurora with scheme Aurora and configuration Debug
(2 failures)

## xcodegen tail
⚙️  Generating plists...
⚙️  Generating project...
⚙️  Writing project...
Created project at /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj

## undefined symbols
Undefined symbols for architecture x86_64:
  "aurora::ons::initEngine(aurora::ons::Config const&)", referenced from:
      (anonymous namespace)::startInstance(void*) in onscripter_module.o
  "aurora::ons::requestStop()", referenced from:
      (anonymous namespace)::stopInstance(void*) in onscripter_module.o
      (anonymous namespace)::releaseInstance(void*) in onscripter_module.o
  "aurora::ons::describeVideo()", referenced from:
      (anonymous namespace)::startInstance(void*) in onscripter_module.o
  "aurora::ons::shutdownEngine()", referenced from:
      (anonymous namespace)::startInstance(void*)::$_0::operator()() const in onscripter_module.o
  "aurora::ons::engineAvailable()", referenced from:
      (anonymous namespace)::startInstance(void*) in onscripter_module.o
  "aurora::ons::runLoop()", referenced from:
      (anonymous namespace)::startInstance(void*)::$_0::operator()() const in onscripter_module.o
ld: symbol(s) not found for architecture x86_64
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj: Aurora: clang++: error: linker command failed with exit code 1 (use -v to see invocation)

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** BUILD FAILED **



## onslib
include
lib
prefix
libaurora_onscripter.a

## simulator tail
