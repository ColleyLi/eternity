#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "helperFunctions.h"

enum CamMode
{
  Grounded,
  Fly
} ;

/// A camera has 3 important vectors:
///   RIGHT, UP, FORWARD
/// THIS WORKS DIFFERENTLY THAN
/// THE NORMAL D3D "EYE", "LOOK", "UP" vectors.
/// 
/// Eye look and up are great for knowing where
/// you're aiming etc, but they're difficult to
/// work with when moving about in space.
/// 
/// Maintaining 3 unit vectors:
///   RIGHT - always straight to the right
///   UP    - if NOT in fly mode, (grounded) always 0,1,0
///   FORWARD - unit vector pointing IN THE DIRECTION YOU'RE LOOKING.
///
/// You then construct a VIEW MATRIX as follows:
///
/// [ RIGHT.X   RIGHT.Y   RIGHT.Z ]
/// [ UP.X      UP.Y      UP.Z    ]
/// [ FWD.X     FWD.Y     FWD.Z   ]
/// 
/// This will be your view matrix and it will actually
/// rotate you exactly to pointing in the direction
/// these vectors specify,
/// almost magical, really..
/// 
/// http://www.fastgraph.com/makegames/3drotation/
/// has an article on this,
/// and 
/// http://social.msdn.microsoft.com/Forums/en-US/xnaframework/thread/f4d80d25-c947-440e-832c-fdae6cfc8b51/
/// has a question on it -
/// What we'll do is return a D3DXMATRIX
/// containing your VIEWING TRANSFORMATION
/// you should use, this class also takes
/// care of MOTION of the RIGHT, UP and FORWARD
class Camera3D
{
  /// For a sense of where
  /// it is in space
  D3DXVECTOR3 eye ; // look ; // look vector not necessary
  // since it is simply (eye + forward)
  // get it using the getter function

  D3DXVECTOR3 right, up, forward ;
  D3DXMATRIX viewMatrix ;

  /// Fly mode means the up vector
  /// is free to change direction.  In
  /// "grounded" mode the camera's
  /// UP vector CANNOT CHANGE from (0,1,0)
  CamMode camMode ;

public:
  Camera3D()
  {
    // ASSUMES RIGHT-HANDED COORDINATE SYSTEM
    right = D3DXVECTOR3( 1, 0, 0 ) ;
    up = D3DXVECTOR3( 0, 1, 0 ) ;
    forward = D3DXVECTOR3( 0, 0, -1 ) ;

    eye = D3DXVECTOR3( 4, 2, 25 ) ;


    trackFwdSpeed = trackRotSpeed = trackPitchSpeed = 0.01f ;

    // Zero out the view matrix
    memset( &viewMatrix, 0, sizeof( D3DXMATRIX ) ) ;

    camMode = Fly ;
  }

  #pragma region vector get/set
  // Access
  D3DXVECTOR3 getEye()
  {
    return eye ;
  }

  void setEye( D3DXVECTOR3 iEye )
  {
    eye = iEye ;
  }

  D3DXVECTOR3 getLook()
  {
    return (eye + forward) ;
  }

  void setLook( D3DXVECTOR3 iLook )
  {
    // setting the look point means
    // you're setting where (eye + forward) is,
    // but (forward) is a unit vector, so
    // we'll just normalize it
    forward = iLook - eye ;

    D3DXVec3Normalize( &forward, &forward ) ;
  }

  D3DXVECTOR3 getUp()
  {
    return up ;
  }

  void setUp( D3DXVECTOR3 iUp )
  {
    up = iUp ;
  }
  #pragma endregion

  D3DXMATRIX getView()
  {
    // construct the view matrix
    // as simply combination of right, up, fwd vectors

    // First row is set by RIGHT
    D3DXMatrixSetRow( &viewMatrix, 0, &right ) ;
    D3DXMatrixSetRow( &viewMatrix, 1, &up ) ;
    D3DXMatrixSetRow( &viewMatrix, 2, &forward ) ;
  }

  void renormalizeVectors()
  {
    // Right, up, forward vectors
    // should always be normalized
    D3DXVec3Normalize( &right, &right ) ;
    D3DXVec3Normalize( &up, &up ) ;
    D3DXVec3Normalize( &forward, &forward ) ;
  }

  void stepForward( float increment )
  {
    eye += increment*forward ;
  }

  void stepSide( float increment )
  {
    // Step towards the right
    eye += increment*right ;
  }

  /// Step horizontal yaw
  /// + is left, - is right.
  void stepYaw( float increment )
  {
    static D3DXMATRIX rotMat ;

    // positive is to the __right__
    // So we rotate BOTH the FORWARD
    // and RIGHT vectors about the UP
    // axis.

    ///!! THIS LOCKS THE HORIZONTAL ROTATIONS
    /// TO "SWEEP" THE GROUND PLANE
    //static D3DXVECTOR3 yAxis( 0, 1, 0 ) ;
    //D3DXMatrixRotationAxis( &rotMat, &yAxis, increment ) ;

    D3DXMatrixRotationAxis( &rotMat, &up, increment ) ;

    // Use the rotation matrix to
    // rotate BOTH fwd and right
    D3DXVec3TransformNormal( &forward, &forward, &rotMat ) ;
    D3DXVec3TransformNormal( &right, &right, &rotMat ) ;

    // renormalize these vectors
    // to make sure they stay unit vectors
    renormalizeVectors() ;
  }

  /// Step vertical pitch
  /// by an increment.  + is up, - is down.
  void stepPitch( float increment )
  {
    static float EPS = 0.05f ; //!! margin large
    // EPS is the margin by which you cannot
    // approach straight up or straight down with
    // the camera

    static D3DXMATRIX rotMat ;

    // The rotation axis is now the RIGHT
    // vector.
    D3DXMatrixRotationAxis( &rotMat, &right, increment ) ;

    // We must prevent the camera from "flipping over" --
    // if the "forward" vector CROSSES the
    // UP vector then this is bad news.
    // The UP vector must never be "BELOW" the
    // FORWARD vector.. i.e. the angle between
    // the FORWARD vector and the UP vector
    // (measured from the FORWARD vector as
    // a starting point), MUST BE LESS THAN
    // 180 degrees.
    // It jumps from 0.00001 degrees to 359.9999 degrees
    // when you "look up too far",
    // and it jumps from 179.99999 degrees to 180.00001 degrees
    // when you "look down too far"


    float dotProduct = D3DXVec3Dot( &up, &forward ) ;


    // You're only allowed to "look more up"
    // if the dot product between UP and
    // FORWARD isn't too close to 1
    // (FORWARD is LINING UP with UP completely)
    if( increment > 0 )
    {
      // Trying to look UP
      if( dotProduct < (1 - EPS) )
      {
        // only allowed here

        // This rotates the FORWARD vector..
        D3DXVec3TransformNormal( &forward, &forward, &rotMat ) ;
      }
    }
    else
    {
      // Trying to look down

      // and you're only allowed to "look more down"
      // if the dot product between UP and
      // FORWARD isnt' too close to -1
      // (FORWARD is nearly 180 degrees from UP)
      if( dotProduct > (-1 + EPS) )
      {
        // Only allowed here

        // This rotates the FORWARD vector..
        D3DXVec3TransformNormal( &forward, &forward, &rotMat ) ;
      }
    }

    // So, you have to be careful that INCREMENT
    // isn't too large or you'll jump these limits.

    


    // IF WE ARE IN FLY MODE,
    // then rotations affect UP.
    // (if not, then they don't)
    if( camMode == Fly )
    {
      //!! ALLOWS CAMERA TO TILT, BAD
      //D3DXVec3TransformNormal( &up, &up, &rotMat ) ;
    }

    renormalizeVectors() ;
  }

  /// Sets the TRACK (automotion) speeds
  /// This doesn't affect the eye position
  /// changes that occur due to the step* functions
  void setTrackSpeeds( float iFwd, float iRot, float iPitch )
  {
    trackFwdSpeed = iFwd ;
    trackRotSpeed = iRot ;
    trackPitchSpeed = iPitch ;
  }

  void setCamMode( CamMode newCamMode )
  {
    if( newCamMode != Grounded && newCamMode != Fly )
    {
      warning( "Invalid CamMode specified, nothing changed" ) ;
      return ;
    }
    else if( camMode != Grounded && newCamMode == Grounded )
    {
      // IF the mode is ground mode,
      // then the UP axis is ALWAYS the
      // y-axis.

      // reset the UP vector to being
      // (0,1,0)
      up.x = 0 ;
      up.y = 1 ;
      up.z = 0 ;
    }
    else if( camMode != Fly && newCamMode == Fly )
    {
      // Go to fly mode now
      
    }

    camMode = newCamMode ;
  }







  #pragma region tracking

private:
  /// The forward speed of the camera
  /// when moving forward per time step.
  float trackFwdSpeed ;

  /// Rotation and pitch speeds for
  /// the camera.  Usually the same.
  float trackRotSpeed ;
  float trackPitchSpeed ;


public:
  /// Steps you towards pt,
  /// gradually
  void toPoint( D3DXVECTOR3 pt )
  {
    
  }

  /// Follows pts one after the other,
  /// interpolating in between using
  /// catmull rom splines
  void toPoints( D3DXVECTOR3 *eyePts )
  {
  }

  /// Moves you along this track interpolating
  /// in between and also interpolating the
  /// look as well
  void toPoints( D3DXVECTOR3 *eyePts, D3DXVECTOR3 *lookPts )
  {
  }

  #pragma endregion


} ;

#endif // CAMERA3D_H