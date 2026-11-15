// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

import SwiftUI

/* One screen for the player: the library. Everything that exists for debugging
 * (modules, log, diagnostics) moved into Settings, because somebody opening this
 * app wants to play a game, not configure an engine. */
struct ContentView: View {
    @ObservedObject var host: AuroraHost
    @AppStorage("appearance.background") private var backgroundRaw = BgStyle.black.rawValue
    @AppStorage("appearance.glassAlpha") private var glassAlpha = 0.10
    @State private var showSettings = false
    @State private var playing: GameRow?
    @State private var path: [GameRow] = []

    /* The first shelf cover doubles as the backdrop, which is what gives the
     * glass something to show through. */
    private var leadingCover: UIImage? {
        guard let first = host.games.first(where: { !$0.module.isEmpty }) else { return nil }
        return CoverArt.image(for: first)
    }

    private var background: BgStyle {
        BgStyle(rawValue: backgroundRaw) ?? .system
    }

    var body: some View {
        ZStack {
            AppBackground(style: background)
            if background != .white {
                BackdropGlow(tint: Theme.accent, image: leadingCover)
            }

            NavigationStack(path: $path) {
                LibraryView(host: host, path: $path)
                    .navigationTitle("Aurora")
                    .navigationBarTitleDisplayMode(.large)
                    .toolbar {
                        ToolbarItem(placement: .topBarTrailing) {
                            Button {
                                showSettings = true
                            } label: {
                                Image(systemName: "gearshape")
                                    .font(.system(size: 16, weight: .semibold))
                                    .padding(9)
                                    .glassSurface(cornerRadius: 20)
                            }
                            .accessibilityLabel("设置")
                        }
                    }
                    .toolbarBackground(.hidden, for: .navigationBar)
                    .navigationDestination(for: GameRow.self) { game in
                        GameDetailView(host: host,
                                       game: game,
                                       onOpen: { picked in
                                           guard let row = host.rows.first(where: { $0.id == picked.module }) else { return }
                                           host.run(row)
                                           playing = picked
                                       },
                                       onSelect: { next in path = [next] })
                    }
            }
        }
        .environment(\.glassAlpha, glassAlpha)
        .tint(Theme.accent)
        .preferredColorScheme(background.colorScheme)
        .sheet(isPresented: $showSettings) {
            SettingsView(host: host)
        }
        /* Starting a module from anywhere should land in the player: with the
         * module list gone, the library is the only place a game can start. */
        .onChange(of: host.runningModule) { module in
            guard !module.isEmpty, playing == nil else { return }
            playing = host.games.first { $0.module == module }
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
}
