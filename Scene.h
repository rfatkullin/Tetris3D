#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
#include "Game.h"

class Scene : public QGLWidget
{
   Q_OBJECT
public slots :
    void                                set3D( bool );

public:
                                        Scene( Game* apNewGame, QWidget* apPwgt = 0 );
                                        ~Scene();
    static void                         SetViewVectors();
    int                                 GetViewSide();
    void                                SetLigthOption( float aAmbient[ 4 ], float aDiffuse[ 4 ], float aSpecular[ 4 ] );
    void                                Resize( int aNewWidth, int aNewHeight );
    void                                ChangeCameraPosition( float aX, float aY );
    void                                AddMessage( Game :: Messages aNewMessage );
    void                                ChangeShowHelp();
    void                                Draw();
protected:
    void                                paintGL();
    void                                initializeGL();
    void                                resizeGL( int aNewWidth, int aNewHeight );

private:
    static const int                    RENDER_MESSAGES_CNT = 4;
    static const int                    WIDTH_RATIO         = 4;
    static const int                    HEIGHT_RATIO        = 3;
    static const int                    RESOL_FIND_STEP     = 50;

    enum                                Sizes { STANDART_SCENE_WIDTH  = 800,
                                                STANDART_SCENE_HEIGHT = 600,
                                                INFO_X_OFFSET = 170,
                                                INFO_Y_OFFSET = 50,
                                                HELP_X_OFFSET = 170,
                                                HELP_Y_OFFSET = 150
                                              };

    enum                                ViewSide { FRONT_SIDE,
                                                   RIGHT_SIDE,
                                                   BACK_SIDE,
                                                   LEFT_SIDE };

    static const unsigned int           SECTOR_VEC_CNT = 4;
    static const float                  CAMERA_RADIUS;
    static const float                  CAMERA_POS_CHANGE_KOEFF;

    static const Point3Di               mCameraShift;
    static Point2Df                     mSectorVecs[ SECTOR_VEC_CNT ];

    int                                 mWidth;
    int                                 mHeight;

    SphericalCoor                       mCameraPosition;
    const Game*                         mpGame;
    float                               mAmbientLight[ 4 ];
    float                               mDiffuseLight[ 4 ];
    float                               mSpecularLight[ 4 ];

    bool                                mIsStereo;
    bool                                mShowHelp;
    float                               mRatio;
    float                               mFrustumHeight;
    float                               mFrustumNearPlane;
    float                               mFrustumFarPlane;
    float                               mFrustumAperture;
    float                               mFrustumHalfWidth;
    float                               mFrustumFocalLength;
    float                               mFrustumEyeSep;
    float                               mFrustumBottom;
    float                               mFrustumTop;
    float                               mFrustumLeft;
    float                               mFrustumRight;

    Game :: Messages                    mRenderMessages[ RENDER_MESSAGES_CNT ];

    Point3Df                            GetCameraPosition() const;
    void                                DrawTextInformation();
};

#endif // SCENE_H
