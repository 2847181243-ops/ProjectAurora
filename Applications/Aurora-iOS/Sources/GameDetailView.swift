// SPDX-License-Identifier: GPL-3.0-or-later
import AVKit
import SwiftUI
import UIKit

/* What a console does when you pick a game: artwork on one side, the opening
 * movie on the other, and the rest of the shelf underneath. */
struct GameDetailView: View {
    @ObservedObject var host: AuroraHost
    let game: GameRow
    var onOpen: (GameRow) -> Void
    var onSelect: (GameRow) -> Void

    @State private var movie: URL?
    @State private var softwareMovie: String?
    @State private var unplayableMovie: String?

    private var others: [GameRow] {
        host.games.filter { !$0.module.isEmpty && $0.id != game.id }
    }

    var body: some View {
        VStack(spacing: 18) {
            HStack(alignment: .top, spacing: 20) {
                CoverImage(game: game)
                    .aspectRatio(2.0 / 3.0, contentMode: .fit)
                    .frame(maxWidth: 260)
                    .clipShape(RoundedRectangle(cornerRadius: 16, style: .continuous))
                    .overlay(
                        RoundedRectangle(cornerRadius: 16, style: .continuous)
                            .strokeBorder(Color.white.opacity(0.10), lineWidth: 1)
                    )

                VStack(alignment: .leading, spacing: 12) {
                    Text(game.title)
                        .font(.title2.weight(.semibold))
                        .lineLimit(2)

                    ZStack {
                        RoundedRectangle(cornerRadius: 14, style: .continuous)
                            .fill(Color.black)
                        if let softwareMovie {
                            MovieView(path: softwareMovie)
                                .clipShape(RoundedRectangle(cornerRadius: 14, style: .continuous))
                        } else if let movie {
                            VideoPlayer(player: AVPlayer(url: movie))
                                .clipShape(RoundedRectangle(cornerRadius: 14, style: .continuous))
                        } else {
                            VStack(spacing: 8) {
                                Image(systemName: "film.slash")
                                    .font(.system(size: 28, weight: .light))
                                    .foregroundStyle(.secondary)
                                Text(unplayableMovie ?? "这个游戏没有可播放的片头")
                                    .font(.caption)
                                    .foregroundStyle(.secondary)
                                    .multilineTextAlignment(.center)
                            }
                            .padding(20)
                        }
                    }
                    .aspectRatio(16.0 / 9.0, contentMode: .fit)

                    Button {
                        onOpen(game)
                    } label: {
                        Label("开始游戏", systemImage: "play.fill")
                            .frame(maxWidth: .infinity)
                            .padding(.vertical, 6)
                    }
                    .buttonStyle(.borderedProminent)
                    .disabled(game.module.isEmpty)

                    if game.module.isEmpty {
                        Text("没有引擎认领这个游戏，所以打不开。")
                            .font(.caption2)
                            .foregroundStyle(.secondary)
                    }
                }
            }
            .padding(.horizontal, 24)
            .padding(.top, 8)

            if !others.isEmpty {
                VStack(alignment: .leading, spacing: 8) {
                    Text("其他游戏")
                        .font(.footnote.weight(.medium))
                        .foregroundStyle(.secondary)
                        .padding(.horizontal, 24)
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 14) {
                            ForEach(others) { other in
                                Button {
                                    onSelect(other)
                                } label: {
                                    VStack(alignment: .leading, spacing: 6) {
                                        CoverImage(game: other)
                                            .aspectRatio(2.0 / 3.0, contentMode: .fit)
                                            .frame(width: 92)
                                            .clipShape(RoundedRectangle(cornerRadius: 8, style: .continuous))
                                        Text(other.title)
                                            .font(.caption2)
                                            .lineLimit(1)
                                            .frame(width: 92, alignment: .leading)
                                    }
                                }
                                .buttonStyle(.plain)
                            }
                        }
                        .padding(.horizontal, 24)
                    }
                }
                .padding(.bottom, 16)
            }

            Spacer(minLength: 0)
        }
        .background(Color.black.ignoresSafeArea())
        .navigationTitle(game.title)
        .navigationBarTitleDisplayMode(.inline)
        .task(id: game.path) { locateOpening() }
    }

    /* Games ship their opening movie next to the script. Only mp4/mov can be
     * handed to AVFoundation; older titles use MPEG-1 or WMV, which iOS cannot
     * decode at all, so say that instead of showing a black rectangle. */
    private func locateOpening() {
        movie = nil
        softwareMovie = nil
        unplayableMovie = nil
        var folder = URL(fileURLWithPath: game.path)
        var isDirectory: ObjCBool = false
        if FileManager.default.fileExists(atPath: folder.path, isDirectory: &isDirectory), !isDirectory.boolValue {
            folder = folder.deletingLastPathComponent()
        }
        let names = (try? FileManager.default.contentsOfDirectory(at: folder,
                                                                  includingPropertiesForKeys: nil)) ?? []
        let preferred = ["mp4", "m4v", "mov"]
        let legacy = ["mpg", "mpeg", "wmv", "avi"]
        let ordered = names.sorted { $0.lastPathComponent < $1.lastPathComponent }
        if let playable = ordered.first(where: { preferred.contains($0.pathExtension.lowercased()) }) {
            movie = playable
            return
        }
        if let mpeg = ordered.first(where: { ["mpg", "mpeg"].contains($0.pathExtension.lowercased()) }) {
            /* MPEG-1 has no system decoder on iOS at all, so hand it to the
             * software player instead of claiming it cannot be shown. */
            softwareMovie = mpeg.path
            return
        }
        if let old = ordered.first(where: { ["wmv", "avi"].contains($0.pathExtension.lowercased()) }) {
            unplayableMovie = "片头是 \(old.pathExtension.uppercased())，暂时没有解码器"
        }
    }
}
