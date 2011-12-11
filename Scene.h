#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
#include "Game.h"

class Scene : public QGLWidget
{
   Q_OBJECT
public:
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
    //Stereo
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

                                        Scene( Game* new_game, QWidget* pwgt = 0 );
                                        ~Scene();
    static void                         SetViewVectors();
    void                                SetLigthOption( float ambient[ 4 ], float diffuse[ 4 ], float specular[ 4 ] );
    void                                Resize( int new_width, int new_height );
    void                                ChangeCameraPosition( float x, float y );
    int                                 GetViewSide();
    void                                paintGL();
    void                                AddMessage( Game :: Messages aNewMessage );
    void                                ChangeShowHelp();
protected:
    void                                initializeGL();
    void                                resizeGL( int new_width, int new_height );
public slots :
    void                                set3D( bool );
private:
    static const unsigned int           SectorVecCnt = 4;
    static const Point3Di               mCameraShift;
    static const float                  CAMERA_RADIUS;
    static const float                  CAMERA_POS_CHANGE_KOEFF;
    static Point2Df                     mSectorVecs[ SectorVecCnt ];

    int                                 mWidth;
    int                                 mHeight;
    void                                SizeNormilize();

    SphericalCoor                       mCameraPosition;
    const Game*                         mpGame;
    float                               mAmbientLight[ 4 ];
    float                               mDiffuseLight[ 4 ];
    float                               mSpecularLight[ 4 ];


    Point3Df                            GetCameraPosition() const;

    Game :: Messages                    mRenderMessages[ RENDER_MESSAGES_CNT ];
    void                                DrawTextInformation();
};

#endif // SCENE_H
