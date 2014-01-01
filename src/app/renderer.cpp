#include "skia.h"

#include <GL/gl.h>
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkRect.h"
#include "SkRRect.h"
#include "gl/GrGLConfig.h"
#include "gl/GrGLFunctions.h"
#include "gl/GrGLExtensions.h"
#include "gl/GrGLInterface.h"
#include "SkGpuDevice.h"
#include "SkGraphics.h"

#include <iostream>

class Renderer {
public:
    SkCanvas * canvas;
    GrContext * context;
    int a;

    Renderer(int width, int height) {
        //Initialize
        a = 9;
        SkGraphics::Init();
        const GrGLInterface* interface = GrGLCreateNativeInterface();
        SkASSERT(NULL != interface);
        context = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)(interface));
        SkASSERT(NULL != context);
        GrBackendRenderTargetDesc desc;
        desc.fWidth = SkScalarRoundToInt(width);
        desc.fHeight = SkScalarRoundToInt(height);
        desc.fConfig = kSkia8888_GrPixelConfig;
        desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
        desc.fSampleCnt = 0; // Number of samples in the color/stencil attachments
        desc.fStencilBits = 8;

        GrGLint buffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
        desc.fRenderTargetHandle = buffer;

        GrRenderTarget* renderTarget = context->wrapBackendRenderTarget(desc);

        context->setRenderTarget(renderTarget);
        if (NULL != context && NULL != renderTarget) {
            SkAutoTUnref<SkBaseDevice> device(new SkGpuDevice(context, renderTarget));
            canvas = new SkCanvas(device);
        }
    }

    static Renderer * fromPtr(SkiaRenderer r) {
        Renderer * ret = static_cast<Renderer *>(r);
        return ret;
    }

    SkiaRenderer toPtr() {
        return (SkiaRenderer)(this);
    }

    void flush() {
        context->flush();
    }

    ~Renderer() {
        delete canvas;
        delete context;
    }
};

SkiaRenderer Init(int width, int height) {
    return (SkiaRenderer)(new Renderer(width, height));
}

void Flush(SkiaRenderer _r) {
    Renderer * r = Renderer::fromPtr(_r);
    r->flush();
}

void Die(SkiaRenderer _r) {
    Renderer * r = Renderer::fromPtr(_r);
    delete(r);
}

SkRect toSkRect(Rect rect) {
    return SkRect::MakeXYWH(rect.x, rect.y, rect.w, rect.h);
}

Color ColorFromRGBA(int r, int g, int b, int a) {
    return SkColorSetARGB(a, r, g, b);
}

void DrawRect(SkiaRenderer _r, Paint _paint, Rect _rect, int radii = 0) {
    Renderer * r = Renderer::fromPtr(_r);

    SkRect rect = toSkRect(_rect);
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(_paint.fillColor);
    paint.setStyle(SkPaint::kFill_Style);
    SkRRect rr;
    rr.setRectXY(rect, radii, radii);
    r->canvas->drawRRect(rr, paint);
    paint.setColor(_paint.strokeColor);
    paint.setStyle(SkPaint::kStroke_Style);
    r->canvas->drawRRect(rr, paint);
}

void Clear(SkiaRenderer _r) {
    Renderer * r = Renderer::fromPtr(_r);
    r->canvas->drawColor(SK_ColorWHITE);
}

int Save(SkiaRenderer _r) {
    Renderer * r = Renderer::fromPtr(_r);
    return r->canvas->saveLayer(NULL, NULL, SkCanvas::kARGB_ClipLayer_SaveFlag);
}

void Restore(SkiaRenderer _r, int count=1) {
    Renderer * r = Renderer::fromPtr(_r);
    r->canvas->restoreToCount(count);
}