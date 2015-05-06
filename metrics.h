#ifndef __H_METRICS
#define __H_METRICS

#include <gl\gl.h>
#include <vector>
using std::vector;

class cVertex {
    public:
        cVertex() {};
        cVertex(double x, double y, double z) : x(x), y(y), z(z) {};
        double x;
        double y;
        double z;
};

typedef cVertex cNormal;

class cVertex2d {
    public:
        cVertex2d() {};
        cVertex2d(double x, double y) : x(x), y(y) {};

        union
        {
            double x;
            double w;
            double u;
        };
        union
        {
            double y;
            double h;
            double v;
        };
};

typedef cVertex2d cUVCoord;

class cQuadFace {
    public:
        cQuadFace() {}
        cQuadFace(  cVertex v1, cVertex v2, cVertex v3, cVertex v4,
                    cUVCoord uv1, cUVCoord uv2, cUVCoord uv3, cUVCoord uv4,
                    cNormal n);
        cVertex v[4];
        cUVCoord uv[4];
        cNormal n;
        GLuint texid;
};

class cFace {
    public:
        cFace() {}
        cFace(vector<cVertex> &vertices, vector<cUVCoord> &UVCoords, cNormal &normal, GLuint texId);

        vector<cVertex> v;
        vector<cUVCoord> uv;
        cNormal n;
        GLuint texId;
        unsigned int vCount;
};

class cRect {
    public:
        cRect() {}
        cRect(float x1, float y1, float x2, float y2);

        float x1;
        float y1;
        float x2;
        float y2;
};

class pos2f {
    //funcs
    public:
        pos2f() : x(0.0), y(0.0) {};
        pos2f(float x, float y) : x(x), y(y) {};

    //vars:
    public:
        float x;
        float y;
};

class rect4f {
    //funcs
    public:
        rect4f() : x1(0.0), y1(0.0), x2(0.0), y2(0.0) {};
        rect4f(float x1, float y1, float x2, float y2) : x1(x1), y1(y1), x2(x2), y2(y2) {};

    //vars:
    public:
        float x1;
        float y1;
        float x2;
        float y2;
};

enum {
    LEFT, RIGHT
};

typedef pos2f vector2f;



#endif
