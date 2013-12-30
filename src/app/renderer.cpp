#include "renderer.h"

#include <GL/gl.h>
#include "GrContext.h"
#include "SkCanvas.h"
#include "gl/GrGLConfig.h"
#include "gl/GrGLFunctions.h"
#include "gl/GrGLExtensions.h"
#include "gl/GrGLInterface.h"
#include "SkGpuDevice.h"

Canvas Init(int width, int height) {
    const GrGLInterface* fInterface = GrGLCreateNativeInterface();
    SkASSERT(NULL != fInterface);
    GrContext * fContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)fInterface);
    SkASSERT(NULL != fContext);
    GrBackendRenderTargetDesc desc;
    desc.fWidth = SkScalarRoundToInt(width);
    desc.fHeight = SkScalarRoundToInt(height);
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;

    GrGLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    desc.fRenderTargetHandle = buffer;

    GrRenderTarget* fRenderTarget = fContext->wrapBackendRenderTarget(desc);

    fContext->setRenderTarget(fRenderTarget);
    if (NULL != fContext && NULL != fRenderTarget) {
        SkAutoTUnref<SkBaseDevice> device(new SkGpuDevice(fContext, fRenderTarget));
        return (void *)(new SkCanvas(device));
    }
    return NULL;
}