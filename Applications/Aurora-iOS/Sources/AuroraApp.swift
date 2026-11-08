// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

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
