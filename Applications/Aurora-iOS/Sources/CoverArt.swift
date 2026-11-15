// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI
import UIKit

/* Game folders usually ship their own key art, and that beats anything we could
 * invent: title.bmp in an ONScripter game, logo.png in a KiriKiri one. Look for
 * the common names, fall back to a stable colour block so the grid still reads. */
enum CoverArt {
    private static let names = [
        "cover", "thumbnail", "thumb", "title", "logo", "icon", "package", "banner",
    ]
    private static let extensions = ["png", "jpg", "jpeg", "webp", "bmp"]

    static func image(for game: GameRow) -> UIImage? {
        let folder = folderURL(for: game)
        let fileManager = FileManager.default
        let entries = (try? fileManager.contentsOfDirectory(at: folder,
                                                            includingPropertiesForKeys: nil)) ?? []
        /* Match case-insensitively: doujin packages are wildly inconsistent. */
        for base in names {
            for ext in extensions {
                let wanted = "\(base).\(ext)"
                if let hit = entries.first(where: { $0.lastPathComponent.lowercased() == wanted }) {
                    if let image = UIImage(contentsOfFile: hit.path) { return image }
                }
            }
        }
        return nil
    }

    private static func folderURL(for game: GameRow) -> URL {
        var url = URL(fileURLWithPath: game.path)
        var isDirectory: ObjCBool = false
        if FileManager.default.fileExists(atPath: url.path, isDirectory: &isDirectory), !isDirectory.boolValue {
            url = url.deletingLastPathComponent()
        }
        return url
    }

    /* Same title, same colour, every launch: a hash is stable where randomness
     * would make the grid flicker between sessions. */
    static func tint(for title: String) -> Color {
        var hash: UInt32 = 2166136261
        for byte in title.utf8 {
            hash = (hash ^ UInt32(byte)) &* 16777619
        }
        let hue = Double(hash % 360) / 360.0
        return Color(hue: hue, saturation: 0.35, brightness: 0.45)
    }
}

struct CoverImage: View {
    let game: GameRow
    @State private var loaded: UIImage?

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                if let loaded {
                    /* Fit, not fill: a cover is artwork and cropping it loses the
                     * composition. The blurred copy behind keeps the frame full. */
                    Image(uiImage: loaded)
                        .resizable()
                        .scaledToFill()
                        .blur(radius: 20)
                        .opacity(0.5)
                    Image(uiImage: loaded)
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                } else {
                    LinearGradient(colors: [CoverArt.tint(for: game.title),
                                            CoverArt.tint(for: game.title).opacity(0.45)],
                                   startPoint: .topLeading, endPoint: .bottomTrailing)
                    Text(String(game.title.prefix(1)).uppercased())
                        .font(.system(size: min(geometry.size.width, geometry.size.height) * 0.42,
                                      weight: .semibold, design: .rounded))
                        .foregroundStyle(.white.opacity(0.85))
                }
            }
            .frame(width: geometry.size.width, height: geometry.size.height)
            .clipped()
        }
        .task(id: game.path) {
            loaded = CoverArt.image(for: game)
        }
    }
}
