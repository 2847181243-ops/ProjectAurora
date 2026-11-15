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

    /* Loose files only. Safe to call while a view renders. */
    static func looseImage(for game: GameRow) -> UIImage? {
        let folder = folderURL(for: game)
        let entries = (try? FileManager.default.contentsOfDirectory(at: folder,
                                                                    includingPropertiesForKeys: nil)) ?? []
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

    static func image(for game: GameRow) -> UIImage? {
        if let loose = looseImage(for: game) { return loose }
        return packedCover(in: folderURL(for: game))
    }

    /* Extracting an archive is not free, so the result is cached against the
     * archive's size and timestamp. */
    private static func packedCover(in folder: URL) -> UIImage? {
        let archives = ((try? FileManager.default.contentsOfDirectory(at: folder, includingPropertiesForKeys: nil)) ?? [])
            .filter { $0.pathExtension.lowercased() == "xp3" }
        guard let archive = archives.first else { return nil }
        guard let attributes = try? FileManager.default.attributesOfItem(atPath: archive.path),
              let size = attributes[.size] as? Int,
              let modified = attributes[.modificationDate] as? Date else { return nil }

        let cacheDir = FileManager.default.urls(for: .cachesDirectory, in: .userDomainMask)[0]
            .appendingPathComponent("covers", isDirectory: true)
        try? FileManager.default.createDirectory(at: cacheDir, withIntermediateDirectories: true)
        let key = "(archive.lastPathComponent)-(size)-(Int(modified.timeIntervalSince1970))"
        let cached = cacheDir.appendingPathComponent(key + ".img")
        if let data = try? Data(contentsOf: cached), let image = UIImage(data: data) { return image }

        guard let image = Xp3Cover.image(for: folder.path),
              let data = image.jpegData(compressionQuality: 0.9) else { return nil }
        try? data.write(to: cached)
        return image
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
            /* Reading an XP3 archive blocks for seconds; on the main actor that
             * is exactly the black screen people saw at launch. */
            let image = await Task.detached(priority: .userInitiated) {
                CoverArt.image(for: game)
            }.value
            loaded = image
        }
    }
}
