// SPDX-License-Identifier: GPL-3.0-or-later
import Foundation
import MetalKit
import SwiftUI

/* Video output path.
 *
 * The platform hands the module a plain RGBA buffer; this view uploads it to a
 * Metal texture and draws it as one quad. The CPU never scales or converts the
 * pixels, which is the point: a real emulator will write 480x272 PSP frames here
 * at 60 Hz and the per-frame cost stays one texture upload.
 */
final class FrameRenderer: NSObject, MTKViewDelegate {
    private let device: MTLDevice
    private let queue: MTLCommandQueue
    private var pipeline: MTLRenderPipelineState?
    private var texture: MTLTexture?
    private var uploadedSequence: Int32 = -1
    private var frameSize = CGSize(width: 0, height: 0)

    init(device: MTLDevice) {
        self.device = device
        self.queue = device.makeCommandQueue()!
        super.init()
        buildPipeline()
    }

    private func buildPipeline() {
        let source = """
        #include <metal_stdlib>
        using namespace metal;

        struct VertexOut {
            float4 position [[position]];
            float2 uv;
        };

        vertex VertexOut vertex_main(uint vertexID [[vertex_id]]) {
            const float2 positions[4] = {
                float2(-1.0, -1.0), float2(1.0, -1.0),
                float2(-1.0,  1.0), float2(1.0,  1.0)
            };
            const float2 uvs[4] = {
                float2(0.0, 1.0), float2(1.0, 1.0),
                float2(0.0, 0.0), float2(1.0, 0.0)
            };
            VertexOut out;
            out.position = float4(positions[vertexID], 0.0, 1.0);
            out.uv = uvs[vertexID];
            return out;
        }

        fragment float4 fragment_main(VertexOut in [[stage_in]],
                                      texture2d<float> frame [[texture(0)]]) {
            constexpr sampler nearest(coord::normalized, filter::nearest,
                                      address::clamp_to_edge);
            return frame.sample(nearest, in.uv);
        }
        """
        guard let library = try? device.makeLibrary(source: source, options: nil),
              let vertexFunction = library.makeFunction(name: "vertex_main"),
              let fragmentFunction = library.makeFunction(name: "fragment_main") else {
            return
        }
        let descriptor = MTLRenderPipelineDescriptor()
        descriptor.vertexFunction = vertexFunction
        descriptor.fragmentFunction = fragmentFunction
        descriptor.colorAttachments[0].pixelFormat = .bgra8Unorm
        pipeline = try? device.makeRenderPipelineState(descriptor: descriptor)
    }

    private func refreshTexture(_ view: MTKView) {
        guard let pixels = aurora_bridge_frame_data() else { return }
        let width = Int(aurora_bridge_frame_width())
        let height = Int(aurora_bridge_frame_height())
        guard width > 0, height > 0 else { return }

        let sequence = aurora_bridge_frames_presented()
        if texture == nil || texture!.width != width || texture!.height != height {
            let descriptor = MTLTextureDescriptor.texture2DDescriptor(
                pixelFormat: .rgba8Unorm, width: width, height: height, mipmapped: false)
            descriptor.usage = [.shaderRead]
            texture = device.makeTexture(descriptor: descriptor)
            frameSize = CGSize(width: width, height: height)
            uploadedSequence = -1
        }
        guard let texture = texture, sequence != uploadedSequence else { return }
        texture.replace(region: MTLRegionMake2D(0, 0, width, height),
                        mipmapLevel: 0,
                        withBytes: pixels,
                        bytesPerRow: width * 4)
        uploadedSequence = sequence
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {}

    func draw(in view: MTKView) {
        refreshTexture(view)
        guard let pipeline = pipeline,
              let drawable = view.currentDrawable,
              let descriptor = view.currentRenderPassDescriptor,
              let commandBuffer = queue.makeCommandBuffer(),
              let encoder = commandBuffer.makeRenderCommandEncoder(descriptor: descriptor) else {
            return
        }

        /* Letterbox inside the drawable so a 4:3 module is not stretched on a
         * 4:3-ish iPad screen. */
        let drawableSize = view.drawableSize
        if frameSize.width > 0 {
            let frameAspect = frameSize.width / frameSize.height
            let viewAspect = drawableSize.width / drawableSize.height
            var width = drawableSize.width
            var height = drawableSize.height
            if frameAspect > viewAspect {
                height = drawableSize.width / frameAspect
            } else {
                width = drawableSize.height * frameAspect
            }
            var x: Int32 = 0
            var y: Int32 = 0
            var targetWidth: Int32 = 0
            var targetHeight: Int32 = 0
            aurora_bridge_compute_viewport(Int32(frameSize.width), Int32(frameSize.height),
                                           Int32(drawableSize.width), Int32(drawableSize.height),
                                           0, &x, &y, &targetWidth, &targetHeight)
            if targetWidth > 0 && targetHeight > 0 {
                encoder.setViewport(MTLViewport(originX: Double(x), originY: Double(y),
                                                width: Double(targetWidth), height: Double(targetHeight),
                                                znear: 0.0, zfar: 1.0))
            }
        }

        if let texture = texture {
            encoder.setRenderPipelineState(pipeline)
            encoder.setFragmentTexture(texture, index: 0)
            encoder.drawPrimitives(type: .triangleStrip, vertexStart: 0, vertexCount: 4)
        }
        encoder.endEncoding()
        commandBuffer.present(drawable)
        commandBuffer.commit()
    }
}

/* Touches have to be forwarded by hand: the platform only knows what this view
 * tells it, and ONScripter advances a dialogue on a click. Without this the panel
 * draws frames but ignores the finger entirely. */
final class TouchMTKView: MTKView {
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesBegan(touches, with: event)
        forward(touches, active: 1)
    }

    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesMoved(touches, with: event)
        forward(touches, active: 1)
    }

    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesEnded(touches, with: event)
        forward(touches, active: 0)
    }

    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        super.touchesCancelled(touches, with: event)
        forward(touches, active: 0)
    }

    private func forward(_ touches: Set<UITouch>, active: Int32) {
        guard let touch = touches.first else { return }
        guard bounds.width > 0, bounds.height > 0 else { return }
        let point = touch.location(in: self)
        let x = Float(max(0, min(1, point.x / bounds.width)))
        let y = Float(max(0, min(1, point.y / bounds.height)))
        aurora_bridge_set_touch(active, x, y)
    }
}

struct MetalFrameView: UIViewRepresentable {
    let framesPresented: Int32

    final class Coordinator {
        var renderer: FrameRenderer?
    }

    func makeCoordinator() -> Coordinator { Coordinator() }

    func makeUIView(context: Context) -> MTKView {
        let view = TouchMTKView()
        if let device = MTLCreateSystemDefaultDevice() {
            view.device = device
            view.colorPixelFormat = .bgra8Unorm
            view.framebufferOnly = true
            view.enableSetNeedsDisplay = true
            view.isPaused = true
            view.clearColor = MTLClearColorMake(0, 0, 0, 1)
            let renderer = FrameRenderer(device: device)
            context.coordinator.renderer = renderer
            view.delegate = renderer
        }
        return view
    }

    func updateUIView(_ view: MTKView, context: Context) {
        view.setNeedsDisplay()
    }
}
