# ios run 238

sha: acecea18e63c7cb8fc9d6b05446421f2c6c339d0
status: failure
at: 2026-11-15T07:59:30Z

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
struct GameDetailView: View {
                            ^
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Sources/GameDetailView.swift:145:5: error: Attribute 'private' can only be used in a non-local scope
    private func locateOpening() {
    ^ (in target 'Aurora' from project 'Aurora')
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Sources/GameDetailView.swift:176:1: error: Expected '}' in struct

^ (in target 'Aurora' from project 'Aurora')
/Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Sources/GameDetailView.swift:8:29: note: To match this opening '{'
struct GameDetailView: View {
                            ^ (in target 'Aurora' from project 'Aurora')

note: Run script build phase 'embed SDL2 dylib' will be run during every build because the option to run the script phase "Based on dependency analysis" is unchecked. (in target 'Aurora' from project 'Aurora')
** ARCHIVE FAILED **


The following build commands failed:
	SwiftDriver Aurora normal arm64 com.apple.xcode.tools.swift.compiler (in target 'Aurora' from project 'Aurora')
	Archiving project Aurora with scheme Aurora
(2 failures)

### Applications/Aurora-iOS/project.log
⚙️  Generating plists...
⚙️  Generating project...
⚙️  Writing project...
Created project at /Users/runner/work/ProjectAurora/ProjectAurora/Applications/Aurora-iOS/Aurora.xcodeproj

## archive tail
