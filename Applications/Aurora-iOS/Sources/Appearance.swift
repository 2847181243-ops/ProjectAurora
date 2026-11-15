// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI
import UIKit

/* Three ways to sit behind the library: a flat black, a flat white, whatever the
 * system is doing, or a picture the user picked. The picture is stored in the
 * app container so it survives reinstalls of a different build. */
enum BgStyle: String, CaseIterable, Identifiable {
    case system, black, white, custom
    var id: String { rawValue }

    var label: String {
        switch self {
        case .system: return "跟随系统"
        case .black: return "黑色"
        case .white: return "白色"
        case .custom: return "自定义"
        }
    }

    var colorScheme: ColorScheme? {
        switch self {
        case .black: return .dark
        case .white: return .light
        default: return nil
        }
    }
}

enum BackgroundStore {
    static var url: URL {
        let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
        return dir.appendingPathComponent("background.img")
    }

    static func save(_ image: UIImage) -> Bool {
        guard let data = image.jpegData(compressionQuality: 0.9) else { return false }
        return (try? data.write(to: url)) != nil
    }

    static func load() -> UIImage? {
        guard let data = try? Data(contentsOf: url) else { return nil }
        return UIImage(data: data)
    }

    static func clear() {
        try? FileManager.default.removeItem(at: url)
    }
}

struct AppBackground: View {
    let style: BgStyle

    var body: some View {
        GeometryReader { geometry in
            switch style {
            case .black:
                Color.black
            case .white:
                Color.white
            case .system:
                Color(uiColor: .systemBackground)
            case .custom:
                if let image = BackgroundStore.load() {
                    Image(uiImage: image)
                        .resizable()
                        .aspectRatio(contentMode: .fill)
                        .frame(width: geometry.size.width, height: geometry.size.height)
                        .clipped()
                        /* Darken whatever the picture is so text and covers keep
                         * their contrast; the point is the backdrop, not the photo. */
                        .overlay(Color.black.opacity(0.45))
                } else {
                    Color.black
                }
            }
        }
        .ignoresSafeArea()
    }
}

/* Frosted plate used for the gear, the player controls and the settings rows.
 * Built from materials rather than the newer glass API so it renders the same on
 * every iOS version this app targets. */
struct GlassSurface: ViewModifier {
    var cornerRadius: CGFloat = 16

    func body(content: Content) -> some View {
        content
            .background(.ultraThinMaterial, in: RoundedRectangle(cornerRadius: cornerRadius, style: .continuous))
            .overlay(
                RoundedRectangle(cornerRadius: cornerRadius, style: .continuous)
                    .strokeBorder(Color.white.opacity(0.14), lineWidth: 0.8)
            )
            .shadow(color: .black.opacity(0.28), radius: 12, y: 6)
    }
}

extension View {
    func glassSurface(cornerRadius: CGFloat = 16) -> some View {
        modifier(GlassSurface(cornerRadius: cornerRadius))
    }
}
