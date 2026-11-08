import SwiftUI

@main
struct AuroraApp: App {
    @StateObject private var host = AuroraHost()

    var body: some Scene {
        WindowGroup {
            ContentView(host: host)
                .onAppear { host.start() }
        }
    }
}
