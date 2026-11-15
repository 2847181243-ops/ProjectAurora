// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI

/* The library is the app. Two columns of 2:3 cards, covers read from the game
 * folder when it ships one, and a stable colour block when it does not. */
struct LibraryView: View {
    @ObservedObject var host: AuroraHost
    @Binding var playing: GameRow?
    @State private var showImporter = false

    private var columns: [GridItem] {
        [GridItem(.adaptive(minimum: 150, maximum: 220), spacing: 16)]
    }

    var body: some View {
        ScrollView {
            if host.games.isEmpty {
                emptyState
            } else {
                LazyVGrid(columns: columns, spacing: 20) {
                    ForEach(host.games) { game in
                        Button {
                            /* Only open the player when an engine actually claims
                             * this file; otherwise the screen would be a black page
                             * with no explanation. */
                            guard let row = host.rows.first(where: { $0.id == game.module }) else { return }
                            host.run(row)
                            playing = game
                        } label: {
                            GameCard(game: game)
                        }
                        .buttonStyle(.plain)
                    }
                }
                .padding(.horizontal, 20)
                .padding(.vertical, 12)
            }
        }
        .background(Theme.background.ignoresSafeArea())
        .fileImporter(isPresented: $showImporter,
                      allowedContentTypes: [.folder, .item],
                      allowsMultipleSelection: true) { result in
            if case .success(let urls) = result {
                host.importGames(from: urls)
            }
        }
    }

    private var emptyState: some View {
        VStack(spacing: 18) {
            Image(systemName: "square.stack.3d.up.slash")
                .font(.system(size: 52, weight: .light))
                .foregroundStyle(.secondary)
            Text("还没有游戏")
                .font(.title3.weight(.semibold))
            Text("把游戏文件夹放进「文件」App 的\n在我的 iPad 上 → Aurora → Games，\n或者点下面的导入。")
                .font(.footnote)
                .foregroundStyle(.secondary)
                .multilineTextAlignment(.center)
            Button {
                showImporter = true
            } label: {
                Label("导入游戏", systemImage: "tray.and.arrow.down")
                    .padding(.horizontal, 4)
            }
            .buttonStyle(.borderedProminent)
        }
        .frame(maxWidth: .infinity)
        .padding(.top, 120)
        .padding(.horizontal, 40)
    }
}

struct GameCard: View {
    let game: GameRow

    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            CoverImage(game: game)
                .aspectRatio(2.0 / 3.0, contentMode: .fit)
                .clipShape(RoundedRectangle(cornerRadius: 12, style: .continuous))
                .overlay(
                    RoundedRectangle(cornerRadius: 12, style: .continuous)
                        .strokeBorder(Color.white.opacity(0.08), lineWidth: 1)
                )
            Text(game.title)
                .font(.subheadline.weight(.medium))
                .foregroundStyle(.primary)
                .lineLimit(1)
            Text(engineName)
                .font(.caption2)
                .foregroundStyle(.secondary)
                .lineLimit(1)
        }
    }

    private var engineName: String {
        if game.module.contains("onscripter") { return "ONScripter" }
        if game.module.contains("kirikiri") { return "KiriKiri2" }
        if game.module.isEmpty { return "无法识别格式" }
        return game.module
    }
}
