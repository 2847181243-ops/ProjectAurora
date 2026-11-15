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
 *
 * A material only reads as glass when there is something behind it to refract, so
 * the plates are drawn over the backdrop glow below. The highlight stroke is what
 * sells the edge: bright at the top-left, fading out by the bottom-right. */
struct GlassSurface: ViewModifier {
    var cornerRadius: CGFloat = 16

    func body(content: Content) -> some View {
        let shape = RoundedRectangle(cornerRadius: cornerRadius, style: .continuous)
        content
            .background(.ultraThinMaterial, in: shape)
            .background(Color.white.opacity(0.06), in: shape)
            .overlay(
                shape.strokeBorder(
                    LinearGradient(colors: [Color.white.opacity(0.55), Color.white.opacity(0.06)],
                                   startPoint: .topLeading, endPoint: .bottomTrailing),
                    lineWidth: 0.9)
            )
            .shadow(color: .black.opacity(0.35), radius: 14, y: 8)
    }
}

/* Something for the glass to sample. Without this the plates sit on flat black
 * and read as dark rectangles no matter how thin the material is. */
struct BackdropGlow: View {
    let tint: Color
    let image: UIImage?

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                if let image {
                    Image(uiImage: image)
                        .resizable()
                        .scaledToFill()
                        .frame(width: geometry.size.width, height: geometry.size.height)
                        .clipped()
                        .blur(radius: 60)
                        .opacity(0.35)
                }
                Circle()
                    .fill(tint.opacity(0.35))
                    .frame(width: geometry.size.width * 0.9)
                    .blur(radius: 120)
                    .offset(x: -geometry.size.width * 0.25, y: -geometry.size.height * 0.2)
                Circle()
                    .fill(Color.blue.opacity(0.18))
                    .frame(width: geometry.size.width * 0.7)
                    .blur(radius: 140)
                    .offset(x: geometry.size.width * 0.3, y: geometry.size.height * 0.25)
            }
        }
        .ignoresSafeArea()
        .allowsHitTesting(false)
    }
}

extension View {
    func glassSurface(cornerRadius: CGFloat = 16) -> some View {
        modifier(GlassSurface(cornerRadius: cornerRadius))
    }
}
