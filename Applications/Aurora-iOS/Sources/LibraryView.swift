// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI

/* The library is the app. Two columns of 2:3 cards, covers read from the game
 * folder when it ships one, and a stable colour block when it does not. */
struct LibraryView: View {
    @ObservedObject var host: AuroraHost
    @Binding var path: [GameRow]
    @State private var showImporter = false
    @State private var unclaimed: GameRow?

    /* Only things an engine claimed are games. A stray file in the Games folder
     * is not something the player can open, so the library does not list it. */
    private var onlyGames: [GameRow] {
        host.games.filter { !$0.module.isEmpty }
    }

    /* Fixed column counts read better than an adaptive grid: two on a phone,
     * three on a tablet, so the covers stay large instead of shrinking to fit
     * however many happen to be on the shelf. */
    private func columns(for width: CGFloat) -> [GridItem] {
        /* Covers stay small so the shelf fills the width: a tablet should show a
         * row of games, not two posters. */
        let spacing: CGFloat = 16
        let target: CGFloat = 200
        let fits = Int((width + spacing) / (target + spacing))
        let count = min(6, max(2, fits))
        let total = width - spacing * CGFloat(count + 1)
        let cardWidth = max(110, min(target * 1.2, total / CGFloat(count)))
        return Array(repeating: GridItem(.fixed(cardWidth), spacing: spacing), count: count)
    }

    var body: some View {
        GeometryReader { geo in
        ScrollView {
            if onlyGames.isEmpty {
                emptyState
            } else {
                LazyVGrid(columns: columns(for: geo.size.width), spacing: 12) {
                    ForEach(onlyGames) { game in
                        if game.module.isEmpty {
                            Button {
                                unclaimed = game
                            } label: {
                                GameCard(game: game)
                            }
                            .buttonStyle(.plain)
                        } else {
                            /* Picking a game opens its page; starting it is a
                             * deliberate second tap. */
                            NavigationLink(value: game) {
                                GameCard(game: game)
                            }
                            .buttonStyle(.plain)
                        }
                    }
                }
                .padding(.horizontal, 18)
                .padding(.vertical, 8)
            }
        }
        .alert("无法识别这个游戏", isPresented: Binding(
            get: { unclaimed != nil },
            set: { if !$0 { unclaimed = nil } }
        )) {
            Button("好", role: .cancel) { unclaimed = nil }
        } message: {
            Text(reasonFor(unclaimed))
        }
        .fileImporter(isPresented: $showImporter,
                      allowedContentTypes: [.folder, .item],
                      allowsMultipleSelection: true) { result in
            if case .success(let urls) = result {
                host.importGames(from: urls)
            }
        }
        }
    }

    private func reasonFor(_ game: GameRow?) -> String {
        guard let game else { return "" }
        if game.detail.isEmpty {
            return "这个文件夹里没有找到任何引擎认识的入口文件（ONScripter 的 nscript.dat / 0.utf，KiriKiri 的 startup.tjs / data.xp3）。"
        }
        return game.detail
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
        VStack(alignment: .leading, spacing: 6) {
            /* Wider than a 2:3 poster: the shelf reads better on a tablet, and
             * the artwork is cropped rather than the card growing taller. */
            CoverImage(game: game)
                .aspectRatio(4.0 / 5.0, contentMode: .fill)
                .frame(maxWidth: .infinity)
                .clipped()
                .clipShape(RoundedRectangle(cornerRadius: 12, style: .continuous))
                .overlay(
                    RoundedRectangle(cornerRadius: 12, style: .continuous)
                        .strokeBorder(Color.white.opacity(0.08), lineWidth: 1)
                )
                .layoutPriority(1)
            /* The player does not pick an engine, so the card never names one.
             * A game nothing claims still shows up, marked so it is obvious why
             * it will not start. */
            HStack(spacing: 6) {
                Text(game.title)
                    .font(.subheadline.weight(.medium))
                    .lineLimit(1)
                Spacer(minLength: 0)
                if game.module.isEmpty {
                    Image(systemName: "questionmark.circle")
                        .font(.caption)
                        .foregroundStyle(.orange)
                }
            }
            .padding(.horizontal, 10)
            .padding(.vertical, 7)
            .frame(maxWidth: .infinity, alignment: .leading)
            .glassSurface(cornerRadius: 10)
        }
    }

}
