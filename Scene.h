#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
#include "Game.h"

//const int WindowWidth  = 800;
//const int WindowHeight = 600;

class Scene : public QGLWidget
{
   Q_OBJECT
public:
    enum                        Sizes { SCENE_WIDTH    = 800,
                                        SCENE_HEIGHT   = 600,
                                        PROJECT_WIDTH  = 300,
                                        PROJECT_HEIGHT = 300
                                      };
    enum                        ProjectsCoordinates { PROJECT_X   = SCENE_WIDTH - PROJECT_WIDTH,
                                                      PROJECT_Y_1 = 0,
                                                      PROJECT_Y_2 = PROJECT_HEIGHT,
                                                      PROJECT_Y_3 = 2 * PROJECT_HEIGHT
                                                    };
    enum                        ViewSide { FRONT_SIDE,
                                           RIGHT_SIDE,
                                           BACK_SIDE,
                                           LEFT_SIDE };
    //Stereo
    bool			mIsStereo;
    float			mRatio;
    float			mFrustumHeight;
    float			mFrustumNearPlane;
    float			mFrustumFarPlane;
    float			mFrustumAperture;
    float			mFrustumHalfWidth;
    float			mFrustumFocalLength;
    float			mFrustumEyeSep;
    float			mFrustumBottom;
    float                       mFrustumTop;
    float                       mFrustumLeft;
    float			mFrustumRight;
    bool			mIsOneSide;

                                Scene( Game* new_game, QWidget* pwgt = 0 );
                                ~Scene();
    static void                 SetViewVectors();
    void                        SetLigthOption( float ambient[ 4 ], float diffuse[ 4 ], float specular[ 4 ] );
    void                        Resize( int new_width, int new_height );
    void                        ChangeCameraPosition( float x, float y );
    int                         GetViewSide();
    void                        paintGL();
protected:
    void                        initializeGL();
    void                        resizeGL( int new_width, int new_height );
public slots :
    void                        set3D( bool );
private:
    static const unsigned int   SectorVecCnt = 4;
    static const Point3Di       mCameraShift;
    static const float          CAMERA_RADIUS;
    static const float          CAMERA_POS_CHANGE_KOEFF;
    static Point2Df             mSectorVecs[ SectorVecCnt ];

    SphericalCoor		mCameraPosition;
    const Game*                 mpGame;
    int                         mSceneWidth;
    int                         mSceneHeight;
    float                       mAmbientLight[ 4 ];
    float                       mDiffuseLight[ 4 ];
    float                       mSpecularLight[ 4 ];


    Point3Df                    GetCameraPosition() const;
    void			DrawTextInformation();
};

#endif // SCENE_H
