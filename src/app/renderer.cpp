#include "skia.h"

#include <GL/gl.h>
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkRect.h"
#include "SkRRect.h"
#include "SkRegion.h"
#include "gl/GrGLConfig.h"
#include "gl/GrGLFunctions.h"
#include "gl/GrGLExtensions.h"
#include "gl/GrGLInterface.h"
#include "SkGpuDevice.h"
#include "SkGraphics.h"
#include "SkPoint.h"

#include <iostream>

class Renderer {
public:
    SkCanvas * canvas;
    GrContext * context;
    GrRenderTarget * renderTarget;
    int a;

    Renderer(int width, int height) {
        canvas = NULL; context = NULL; renderTarget = NULL;
        //Initialize
        SkGraphics::Init();
        const GrGLInterface* interface = GrGLCreateNativeInterface();
        SkASSERT(NULL != interface);
        context = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)(interface));
        SkASSERT(NULL != context);

        updateRenderTarget(width, height);

        // if (NULL != context && NULL != renderTarget) {
        //     SkAutoTUnref<SkBaseDevice> device(new SkGpuDevice(context, renderTarget));
        //     canvas = new SkCanvas(device);
        // }
    }

    void updateRenderTarget(int width, int height) {
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

        if (renderTarget != NULL) {
            SkSafeUnref(renderTarget);
        }
        renderTarget = context->wrapBackendRenderTarget(desc);
        context->setRenderTarget(renderTarget);
        if (NULL != context && NULL != renderTarget) {
            SkAutoTUnref<SkBaseDevice> device(new SkGpuDevice(context, renderTarget));
            if (canvas != NULL) SkSafeUnref(canvas);
            canvas = new SkCanvas(device);
        }
    }

    void updateCanvas(int width, int height) {
        // canvas = new SkCanvas(canvas->createCompatibleDevice(SkBitmap::kARGB_8888_Config, width, height, false));
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
        SkSafeUnref(canvas);
        SkSafeUnref(context);
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
    return SkRect::MakeXYWH(rect.min.x, rect.min.y, rect.max.x-rect.min.x, rect.max.y-rect.min.y);
}

Color ColorFromRGBA(int r, int g, int b, int a) {
    return SkColorSetARGB(a, r, g, b);
}

void DrawRect(SkiaRenderer _r, Paint _paint, Rect _rect, Point * raddis) {
    Renderer * r = Renderer::fromPtr(_r);
    SkVector skRads[4];
    for (int i=0; i<4; ++i) {
        skRads[i] = SkPoint::Make(raddis[i].x, raddis[i].y);
    }
    SkRect rect = toSkRect(_rect);
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(_paint.fillColor);
    paint.setStyle(SkPaint::kFill_Style);
    SkRRect rr;
    rr.setRectRadii(rect, skRads);
    r->canvas->drawRRect(rr, paint);
    paint.setColor(_paint.strokeColor);
    paint.setStrokeWidth(_paint.strokeWidth);
    paint.setStyle(SkPaint::kStroke_Style);
    r->canvas->drawRRect(rr, paint);
}

void ClipRect(SkiaRenderer _r, Rect _rect) {
    Renderer * r = Renderer::fromPtr(_r);
    SkRect rect = toSkRect(_rect);
    r->canvas->clipRect(rect, SkRegion::kIntersect_Op, true);
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

void UpdateWindowSize(SkiaRenderer _r, int width, int height) {
    Renderer * r = Renderer::fromPtr(_r);
    r->updateRenderTarget(width, height);
    r->updateCanvas(width, height);
}