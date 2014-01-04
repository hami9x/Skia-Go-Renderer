#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    typedef void* SkiaRenderer;
    typedef uint32_t Color;
    typedef struct Point {
        int x;
        int y;
    } Point;

    typedef struct Rect {
        Point min;
        Point max;
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
    void UpdateWindowSize(SkiaRenderer, int, int);
    void Clear(SkiaRenderer);
    void Restore(SkiaRenderer, int);
    void ClipRect(SkiaRenderer, Rect);
    void DrawRect(SkiaRenderer, Paint, Rect, Point *);
    Color ColorFromRGBA(int, int, int, int);
#ifdef __cplusplus
}
#endif