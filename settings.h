#define DEBUG_MODE

#define Q_LOW 0
#define Q_MID 1
#define Q_HIGH 2
#define Q_HIGHEST 3

#define OPT_TEXTURE_QUALITY Q_HIGHEST

#if OPT_TEXTURE_QUALITY == Q_LOW
    #define OPT_TEX_MIN_FILTER GL_NEAREST
    #define OPT_TEX_MAG_FILTER GL_NEAREST
#endif
#if OPT_TEXTURE_QUALITY == Q_MID
    #define OPT_TEX_MIN_FILTER GL_LINEAR
    #define OPT_TEX_MAG_FILTER GL_LINEAR
#endif
#if OPT_TEXTURE_QUALITY == Q_HIGH
    #define OPT_TEX_MIN_FILTER GL_NEAREST_MIPMAP_NEAREST
    #define OPT_TEX_MAG_FILTER GL_LINEAR
#endif
#if OPT_TEXTURE_QUALITY == Q_HIGHEST
    #define OPT_TEX_MIN_FILTER GL_LINEAR_MIPMAP_LINEAR
    #define OPT_TEX_MAG_FILTER GL_LINEAR
#endif


#define OPT_PATH_TEXTURES "data/texs/"
#define OPT_PATH_MODELS "data/mdls/"
#define OPT_PATH_SOUNDS "data/snds/"
#define OPT_PATH_FONTS "data/fnts/"
#define OPT_PATH_SCRIPTS "data/luas/"

#define OPT_PATHEXT_MODELS ".obj"
#define OPT_PATHEXT_TEXTURES ".bmp"
#define OPT_PATHEXT_SOUNDS ".wav"
#define OPT_PATHEXT_FONTS ".bmp"
#define OPT_PATHEXT_SCRIPTS ".lua"

#define OPT_NUM_EXPLO_PTLS 1000
#define OPT_NUM_INITIAL_PTLS 10000
#define OPT_MAX_PTLS 100000

#define OPT_NUM_INITIAL_PRJCTLS 100
#define OPT_MAX_PRJCTLS 1000

#define SCR_W 800
#define SCR_H 600
