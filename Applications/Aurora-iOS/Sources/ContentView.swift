// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

import SwiftUI

/* One screen for the player: the library. Everything that exists for debugging
 * (modules, log, diagnostics) moved into Settings, because somebody opening this
 * app wants to play a game, not configure an engine. */
struct ContentView: View {
    @ObservedObject var host: AuroraHost
    @State private var showSettings = false
    @State private var playing: GameRow?

    var body: some View {
        NavigationStack {
            LibraryView(host: host, playing: $playing)
                .navigationTitle("Aurora")
                .navigationBarTitleDisplayMode(.large)
                .toolbar {
                    ToolbarItem(placement: .topBarTrailing) {
                        Button {
                            showSettings = true
                        } label: {
                            Image(systemName: "gearshape")
                        }
                        .accessibilityLabel("Settings")
                    }
                }
        }
        .tint(Theme.accent)
        .preferredColorScheme(.dark)
        .sheet(isPresented: $showSettings) {
            SettingsView(host: host)
        }
        .fullScreenCover(item: $playing) { game in
            PlayerView(host: host, game: game) {
                playing = nil
            }
        }
    }
}

enum Theme {
    static let accent = Color(red: 1.0, green: 0.61, blue: 0.22)
    static let card = Color(white: 0.12)
    static let background = Color(white: 0.05)
}
