// SPDX-License-Identifier: GPL-3.0-or-later
import Foundation
import GameController

/* Controllers for the two visual novel engines are simple: they advance on a
 * click. So the mapping is
 *
 *   touchpad        -> pointer position, press = click
 *   A / cross       -> click at the centre of the picture
 *   left stick      -> pointer position (coarse, for pads without a touchpad)
 *
 * Key events for in-game menus need the engine side to accept them; that is the
 * next step. Until then this already makes a controller usable for reading. */
final class Gamepad: ObservableObject {
    static let shared = Gamepad()

    @Published private(set) var connectedName: String = ""
    @Published private(set) var connectedBattery: String = ""

    private var ticker: Timer?
    private var wasPressed = false
    private var lastTap: TimeInterval = 0

    private init() {
        NotificationCenter.default.addObserver(forName: .GCControllerDidConnect, object: nil, queue: .main) { [weak self] _ in
            self?.refresh()
        }
        NotificationCenter.default.addObserver(forName: .GCControllerDidDisconnect, object: nil, queue: .main) { [weak self] _ in
            self?.refresh()
        }
        GCController.startWirelessControllerDiscovery(completionHandler: nil)
        refresh()
    }

    private func refresh() {
        if let controller = GCController.controllers().first {
            connectedName = controller.vendorName ?? "手柄"
            if let battery = controller.battery {
                connectedBattery = String(format: "%.0f%%", battery.batteryLevel * 100)
            } else {
                connectedBattery = ""
            }
            startTicker()
        } else {
            connectedName = ""
            connectedBattery = ""
            ticker?.invalidate()
            ticker = nil
        }
    }

    private func startTicker() {
        guard ticker == nil else { return }
        ticker = Timer.scheduledTimer(withTimeInterval: 1.0 / 60.0, repeats: true) { [weak self] _ in
            self?.poll()
        }
        RunLoop.main.add(ticker!, forMode: .common)
    }

    private func handleTouchpad(_ touchpad: GCControllerTouchpad) {
        let position = touchpad.touchSurface
        let x = Float(position.xAxis.value)
        let y = Float(1.0 - position.yAxis.value)
        if touchpad.button.isPressed {
            aurora_bridge_set_touch(1, x, y)
            wasPressed = true
        } else if wasPressed {
            aurora_bridge_set_touch(0, x, y)
            wasPressed = false
        }
    }

    private func poll() {
        guard let controller = GCController.controllers().first else { return }
        var mask: UInt32 = 0
        var axisX: Float = 0
        var axisY: Float = 0

        if let pad = controller.extendedGamepad {
            if pad.buttonA.isPressed { mask |= 1 << 0 }
            if pad.buttonB.isPressed { mask |= 1 << 1 }
            if pad.buttonX.isPressed { mask |= 1 << 2 }
            if pad.buttonY.isPressed { mask |= 1 << 3 }
            if pad.dpad.up.isPressed { mask |= 1 << 4 }
            if pad.dpad.down.isPressed { mask |= 1 << 5 }
            if pad.dpad.left.isPressed { mask |= 1 << 6 }
            if pad.dpad.right.isPressed { mask |= 1 << 7 }
            axisX = pad.leftThumbstick.xAxis.value
            axisY = pad.leftThumbstick.yAxis.value
        } else if let pad = controller.microGamepad {
            if pad.buttonA.isPressed { mask |= 1 << 0 }
            if pad.dpad.up.isPressed { mask |= 1 << 4 }
            if pad.dpad.down.isPressed { mask |= 1 << 5 }
            if pad.dpad.left.isPressed { mask |= 1 << 6 }
            if pad.dpad.right.isPressed { mask |= 1 << 7 }
            axisX = pad.dpad.xAxis.value
            axisY = pad.dpad.yAxis.value
        }

        aurora_bridge_set_pad(mask, axisX, axisY)

        /* The touchpad is the good pointer: it has a real absolute position.
         * Only the PlayStation pads have one, and each exposes it on its own
         * subclass rather than on GCController. */
        if let dual = controller.extendedGamepad as? GCDualShockGamepad {
            handleTouchpad(dual.touchpad)
        } else if let dual = controller.extendedGamepad as? GCDualSenseGamepad {
            handleTouchpad(dual.touchpad)
        }

        /* A on its own means "advance", which is a tap in the middle. */
        let aPressed = (mask & 1) != 0
        let now = Date().timeIntervalSince1970
        if aPressed, now - lastTap > 0.25 {
            lastTap = now
            aurora_bridge_set_touch(1, 0.5, 0.5)
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.05) {
                aurora_bridge_set_touch(0, 0.5, 0.5)
            }
        }
    }
}
