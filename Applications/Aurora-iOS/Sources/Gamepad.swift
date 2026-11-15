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

    /* Virtual pointer: the D-pad moves it, A clicks where it sits. That gives
     * menu navigation on any game that is driven by a mouse, with no engine
     * changes at all. */
    @Published private(set) var pointer = CGPoint(x: 0.5, y: 0.5)

    private var ticker: Timer?
    private var wasPressed = false
    private var lastTap: TimeInterval = 0
    private var lastMove: TimeInterval = 0

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

        /* Touchpad support is parked: neither GCController nor the DualShock /
         * DualSense subclasses expose it in this SDK as .touchpad, so the pointer
         * currently comes from the stick and the A button. */
        _ = controller;

        /* Touchpad, if this SDK exposes one through the physical profile. */
        let profile = controller.physicalInputProfile
        if let surface = profile.dpads["Touchpad"], let button = profile.buttons["Touchpad Button"] {
            let x = CGFloat(surface.xAxis.value)
            let y = CGFloat(1.0 - surface.yAxis.value)
            if button.isPressed {
                pointer = CGPoint(x: x, y: y)
                aurora_bridge_set_touch(1, Float(x), Float(y))
                wasPressed = true
            } else if wasPressed {
                aurora_bridge_set_touch(0, Float(x), Float(y))
                wasPressed = false
            }
        }

        let now = Date().timeIntervalSince1970
        /* D-pad or stick moves the pointer on a slow repeat so a held direction
         * steps instead of racing. */
        let step: CGFloat = 0.02
        let hold: TimeInterval = 0.22
        var dx: CGFloat = 0
        var dy: CGFloat = 0
        if mask & (1 << 6) != 0 { dx -= 1 }
        if mask & (1 << 7) != 0 { dx += 1 }
        if mask & (1 << 4) != 0 { dy -= 1 }
        if mask & (1 << 5) != 0 { dy += 1 }
        if dx == 0 && dy == 0 {
            if abs(axisX) > 0.6 { dx = axisX > 0 ? 1 : -1 }
            if abs(axisY) > 0.6 { dy = axisY > 0 ? 1 : -1 }
        }
        if (dx != 0 || dy != 0), now - lastMove > hold {
            lastMove = now
            pointer = CGPoint(x: min(1, max(0, pointer.x + dx * step)),
                              y: min(1, max(0, pointer.y + dy * step)))
        }

        /* A clicks wherever the pointer sits. */
        let aPressed = (mask & 1) != 0
        if aPressed, now - lastTap > 0.25 {
            lastTap = now
            let tap = CGPoint(x: pointer.x, y: pointer.y)
            aurora_bridge_set_touch(1, Float(tap.x), Float(tap.y))
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.05) {
                aurora_bridge_set_touch(0, Float(tap.x), Float(tap.y))
            }
        }
    }
}
