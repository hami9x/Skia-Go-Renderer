#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    typedef void* SkiaRenderer;
    typedef uint32_t Color;

    typedef struct Rect {
        int x;
        int y;
        int w;
        int h;
    } Rect;

    typedef struct Paint {
        Color fillColor;
        Color strokeColor;
        int strokeWidth;
    } Paint;

    SkiaRenderer Init(int, int);
    void Flush(SkiaRenderer);
    void Die(SkiaRenderer);
    int Save(SkiaRenderer);
    void Clear(SkiaRenderer);
    void Restore(SkiaRenderer, int);

    void DrawRect(SkiaRenderer, Paint, Rect, int);
    Color ColorFromRGBA(int, int, int, int);
#ifdef __cplusplus
}
#endif