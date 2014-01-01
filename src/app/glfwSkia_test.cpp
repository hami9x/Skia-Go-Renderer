#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <GLFW/glfw3.h>

#include "GrContext.h"
#include "GrRenderTarget.h"
#include "gl/GrGLInterface.h"
#include "SkGpuDevice.h"
#include "SkCanvas.h"
#include "SkGraphics.h"

int main( int ac,char** av )
{
    GLFWwindow* window;
    if (!glfwInit())
        exit(EXIT_FAILURE);

    SkAutoGraphics ag; 
    int window_width = 800,
         window_height = 600;
    //*
    if( glfwInit() == GL_FALSE )
    {
        printf( "Could not initialize GLFW. Aborting.\n" );
        exit( 0 );
    }

    // This function calls wglMakeCurrent somewhere in its execution, so make sure that (or its platform specific equivalent) it is called when porting to other frameworks.
    window = glfwCreateWindow(window_width, window_height, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    const GrGLInterface* fGL = GrGLCreateNativeInterface();
    GrContext* fGrContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)(fGL));
    if( fGrContext == 0x0 )
    {
        printf("\nfGrContext was null");
        exit( 0 );
    }


    GrRenderTarget* fGrRenderTarget;

    GrBackendRenderTargetDesc desc;
    desc.fWidth = window_width;
    desc.fHeight = window_height;
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    desc.fSampleCnt = 0; // Number of samples in the color/stencil attachments
    desc.fStencilBits = 8;
    
    GrGLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    desc.fRenderTargetHandle = buffer;

    // I had some serious trouble with segmentation faults and failures arising from this function. Perhaps some more robust error checking is in order?
    fGrRenderTarget = fGrContext->wrapBackendRenderTarget(desc);

    // Canvi (Canvasses?) only really care about the device they contain, so you can ignore the SkGpuCanvas.
    SkCanvas* gpuCanvas = new SkCanvas(new SkGpuDevice(fGrContext,fGrRenderTarget));

    while( !glfwWindowShouldClose(window) )
    {
        // Draw a red background with a gray, semi-transparent circle in the top left corner.
        gpuCanvas->drawColor( SK_ColorRED );
        SkRect r(SkRect::MakeWH(200,200));
        SkPaint p;
        p.setARGB( 200,100,100,100 );
        gpuCanvas->drawOval( r,p );

        glfwSwapBuffers(window);
        glfwPollEvents();
        fGrContext->flush(false);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 1;
}
