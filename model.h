#ifndef __MODEL_H
#define __MODEL_H

#include <string>
#include <vector>
#include "metrics.h"
#include "gl/gl.h"

using std::string;
using std::vector;


class cModel {
    public:
        cModel();

        enum {
            OPT_DRAWONBUILD
        };

        bool loadFile(string filename);
        GLuint loadTexture(string filename);
        void setOpt(int opt, bool value);

        void draw();

        void addFace(vector<cVertex> &faceVertices, vector<cUVCoord> faceUVCoords, GLuint textureIdx);
        void clearFaces();

    private:
        cNormal calcNormal(vector<cVertex> &vertices);

        static GLuint nextTexId;
        GLuint displayList;
        bool displayListMade;
        vector<cFace> faces;

        bool optDrawOnBuild;
};

#endif /* ifndef __MODEL_H */
