// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI

/* Hosts the software MPEG-1 decoder. iOS has no system decoder for .mpg on any
 * version, so the opening movies these games ship need their own. */
struct MovieView: UIViewRepresentable {
    let path: String

    final class Coordinator {
        var opened: String = ""
    }

    func makeCoordinator() -> Coordinator { Coordinator() }

    func makeUIView(context: Context) -> AuroraMovieView {
        let view = AuroraMovieView()
        view.openPath(path)
        context.coordinator.opened = path
        view.play()
        return view
    }

    func updateUIView(_ view: AuroraMovieView, context: Context) {
        guard context.coordinator.opened != path else { return }
        view.openPath(path)
        context.coordinator.opened = path
        view.play()
    }

    static func dismantleUIView(_ view: AuroraMovieView, coordinator: Coordinator) {
        view.pause()
    }
}
