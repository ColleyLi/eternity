#include "D3DWindow.h"

D3DWindow::D3DWindow( HINSTANCE hInst, TCHAR* windowTitleBar,
                     int windowXPos, int windowYPos,
                     int windowWidth, int windowHeight ) :
  Window( hInst, windowTitleBar,
          windowXPos, windowYPos,
          windowWidth, windowHeight )
{
  d3dpps.BackBufferWidth = windowWidth ;
  d3dpps.BackBufferHeight = windowHeight ;

  info( "Starting up Direct3D..." ) ;
  if( !initD3D() )
  {
    bail( "D3D failed to initialize. Check your set up params in initD3D() function, check your width and height are valid" ) ;
  }
}

D3DWindow::~D3DWindow()
{
  // D3D
  SAFE_RELEASE( gpu ) ;
  SAFE_RELEASE( d3d ) ;
}

bool D3DWindow::initD3D()
{
  // start by nulling out both pointers:
  d3d = 0 ;
  gpu = 0 ;

  d3d = Direct3DCreate9( D3D_SDK_VERSION ) ;

  if( d3d == NULL )
  {
    error( "D3DDevice creation FAILED" ) ;
    return false ;
  }

  info( "Direct3D9 device created successfully" ) ;


  memset( &d3dpps, 0, sizeof( D3DPRESENT_PARAMETERS ) ) ;

  d3dpps.BackBufferCount = 1 ;
  d3dpps.SwapEffect = D3DSWAPEFFECT_DISCARD  ;
  d3dpps.BackBufferFormat = D3DFMT_UNKNOWN ;
  d3dpps.EnableAutoDepthStencil = true ;
  d3dpps.AutoDepthStencilFormat = D3DFMT_D16 ;
  d3dpps.hDeviceWindow = hwnd ;
  
  d3dpps.BackBufferFormat = D3DFMT_X8R8G8B8 ;
  
  d3dpps.Windowed = true ;
  //d3dpps.BackBufferWidth =  ;  // these have already been set
  //d3dpps.BackBufferHeight =  ; // in Window() ctor
  //d3dpps.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE ; // FRAMERATE::UNBOUNDED  You need to
  // uncomment this line to make the GPU flip over really really fast

  HRESULT hr = d3d->CreateDevice(

    D3DADAPTER_DEFAULT, // primary display adapter
    D3DDEVTYPE_HAL,     // use HARDWARE rendering
    hwnd,
    D3DCREATE_HARDWARE_VERTEXPROCESSING,
    &d3dpps,
    &gpu

  ) ;


  if( !DX_CHECK( hr, "initialize Direct3D" ) )
  {
    error( "Init d3d FAILED" ) ;
    return false ;
  }


  info( "Direct3D9 GPU device creation successful" ) ;

  isDeviceLost = false ;

  gpu->GetDeviceCaps( &caps ) ;
  



  initVertexDeclaration() ;


  

  hr = gpu->SetRenderState( D3DRS_COLORVERTEX, TRUE ) ;
  DX_CHECK( hr, "SetRenderState( COLORVERTEX )" ) ;

  hr = gpu->SetRenderState( D3DRS_LIGHTING, FALSE ) ;
  DX_CHECK( hr, "Lighting off" ) ;

  int sampleType = D3DTEXF_LINEAR ;  // change to D3DTEXF_NONE to
  // make images BLOCKY and stop the "blurring"

  // D3DTEXF_LINEAR:  Blur the color between pixels nicely
  // D3DTEXF_NONE:  BLOCKY PIXELS.


  hr = gpu->SetSamplerState( 0, D3DSAMP_MAGFILTER, sampleType ) ;
  DX_CHECK( hr, "mag filter" ) ;

  // Set minification filter, 
  hr = gpu->SetSamplerState( 0, D3DSAMP_MINFILTER, sampleType ) ;
  DX_CHECK( hr, "min filter" ) ;

  // Enable mipmapping in general
  hr = gpu->SetSamplerState( 0, D3DSAMP_MIPFILTER, sampleType ) ;
  DX_CHECK( hr, "mip filter" ) ;

  hr = gpu->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ) ;
  DX_CHECK( hr, "cull mode off" ) ;

  hr = gpu->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ) ;
  DX_CHECK( hr, "alpha blending on" ) ;
  
  clearColor = D3DCOLOR_ARGB( 255, 0, 10, 45 ) ;

  // init the eye,look, up, so that
  // you're not zero'd out at prog start
  eye = D3DXVECTOR3( 4, 2, 10 ) ;
  look = D3DXVECTOR3( 0, 0, 0 ) ;
  up = D3DXVECTOR3( 0, 1, 0 ) ;

  return true ;
}

void D3DWindow::initVertexDeclaration()
{
  HRESULT hr ;
  // w/ texture
  //hr = gpu->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | (1 << D3DFVF_TEXCOUNT_SHIFT) ) ;

  hr = gpu->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE ) ;
  DX_CHECK( hr, "SetFVF" ) ;

  int cumulativeOffset = 0 ;
  D3DVERTEXELEMENT9 pos ;
  pos.Usage = D3DDECLUSAGE_POSITION ;
  pos.UsageIndex = 0 ;
  pos.Stream = 0 ;
  pos.Type = D3DDECLTYPE_FLOAT3 ;
  pos.Offset = cumulativeOffset ;
  pos.Method = D3DDECLMETHOD_DEFAULT ; 
  cumulativeOffset += 3*sizeof(float) ;

  D3DVERTEXELEMENT9 col;
  col.Usage = D3DDECLUSAGE_COLOR ;
  col.UsageIndex = 0 ;
  col.Stream = 0 ;
  col.Type = D3DDECLTYPE_D3DCOLOR ;
  col.Offset = cumulativeOffset ;
  col.Method = D3DDECLMETHOD_DEFAULT ;
  cumulativeOffset += sizeof( D3DCOLOR ) ;

  /*
  D3DVERTEXELEMENT9 uv ;
  uv.Usage = D3DDECLUSAGE_TEXCOORD ;
  uv.UsageIndex = 0 ;
  uv.Stream = 0 ;
  uv.Type = D3DDECLTYPE_FLOAT2 ;
  uv.Offset = cumulativeOffset ;
  uv.Method = D3DDECLMETHOD_DEFAULT ;
  cumulativeOffset += 2*sizeof(float) ;
  */

  D3DVERTEXELEMENT9 vertexElements[] =
  {
    pos,
    col,

    D3DDECL_END()
  } ;

  IDirect3DVertexDeclaration9 * Vdecl ;

  hr = gpu->CreateVertexDeclaration( vertexElements, &Vdecl ) ;
  DX_CHECK( hr, "CreateVertexDeclaration" ) ;

  hr = gpu->SetVertexDeclaration( Vdecl ) ;
  DX_CHECK( hr, "SetVertexDeclaration" ) ;
}

void D3DWindow::d3dLoseDevice()
{
  isDeviceLost = true ;

  foreach( vector<ID3DXSprite*>::iterator, iter, registeredSprites )
  {
    DX_CHECK( (*iter)->OnLostDevice(), "vector: sprite renderer OnLostDevice" ) ;
  }

  foreach( vector<ID3DXFont*>::iterator, iter, registeredFonts )
  {
    DX_CHECK( (*iter)->OnLostDevice(), "vector: font OnLostDevice" ) ;
  }

  foreach( vector<ID3DXLine*>::iterator, iter, registeredLines )
  {
    DX_CHECK( (*iter)->OnLostDevice(), "vector: line OnLostDevice" ) ;
  }
}

bool D3DWindow::d3dResetDevice( D3DPRESENT_PARAMETERS & pps )
{
  HRESULT hr ;

  hr = gpu->Reset( &d3dpps ) ;
  bool succeeded = DX_CHECK( hr, "reset gpu device" ) ; // this still might fail.
  // If it does fail, then the device is STILL lost then.

  // This is written out this way to make it clear
  // how the program logic goes.
  if( succeeded )
  {
    isDeviceLost = false ;

    info( "GPU reset complete" ) ;

    // If the previous call to Reset the GPU _worked_, then
    // we should reset all the id3dx objects
    foreach( vector<ID3DXSprite*>::iterator, iter, registeredSprites )
    {
      DX_CHECK( (*iter)->OnResetDevice(), "vector: sprite renderer OnResetDevice" ) ;
    }

    foreach( vector<ID3DXFont*>::iterator, iter, registeredFonts )
    {
      DX_CHECK( (*iter)->OnResetDevice(), "vector: font OnResetDevice" ) ;
    }

    foreach( vector<ID3DXLine*>::iterator, iter, registeredLines )
    {
      DX_CHECK( (*iter)->OnResetDevice(), "vector: line OnResetDevice" ) ;
    }
  }
  else
  {
    isDeviceLost = true ;
    error( "I could not reset the gpu" ) ;
  }

  return succeeded ;
}

void D3DWindow::d3dDeviceCheck()
{
  // check if the device is "lost"
  // if it is, we should not draw.

  HRESULT hr ;
  hr = gpu->TestCooperativeLevel() ;

  switch( hr )
  {
  case D3DERR_DRIVERINTERNALERROR:
    error( "Hmm, the driver experienced an internal error.  This is unusual." ) ;

    // Try and re-initialize d3d.  If it fails, quit.
    if( !initD3D() )
    {
      bail( "Experienced D3DERR_DRIVERINTERNALERROR" ) ;
    }
    break ;

  case D3DERR_DEVICELOST:
    if( !isDeviceLost )  // if the device wasn't already lost..
    {
      warning( "The device has just been lost" ) ;
      d3dLoseDevice() ;
    }
    break ;

  case D3DERR_DEVICENOTRESET:
    if( isDeviceLost )
    {
      // The device was lost, but now we have the chance
      // to reset it.
      info( "Resetting the gpu device.." ) ;
      d3dResetDevice( d3dpps ) ;
    }
    else
    {
      // This should not happen, but if it does, we want to know about it
      error( "Device wasn't lost, yet we were given D3DERR_DEVICENOTRESET" ) ;
    }
    break ;
  
  case D3D_OK:
  default:
    // Device is ok, so don't do anything here
    break ;
  }
}

void D3DWindow::d3dShutdown()
{
  SAFE_RELEASE( gpu ) ;
  SAFE_RELEASE( d3d ) ;
}

//!! fix this out
bool D3DWindow::d3dSupportsNonPowerOf2Textures()
{
  bool conditionallySupportsNonPow2Tex = caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL ;

  return conditionallySupportsNonPow2Tex ;
}



void D3DWindow::d3dWindowStep()
{
  // check the d3ddevice, in case its been lost
  d3dDeviceCheck() ;
}

void D3DWindow::project2D()
{
  HRESULT hr ;

  D3DXMATRIX projx ;
  D3DXMatrixOrthoRH( &projx, getWidth(), getHeight(), 1, 100 ) ;
  hr = gpu->SetTransform( D3DTS_PROJECTION, &projx ) ;
  DX_CHECK( hr, "setting projection xform" ) ;

  D3DXMATRIX viewx ;
  int x = getWidth() / 2 ;
  int y = getHeight() / 2 ;
  D3DXVECTOR3 eye( x, y, 10 ) ;
  D3DXVECTOR3 look( x, y, 0 ) ;
  D3DXVECTOR3 up( 0, 1, 0 ) ;
  D3DXMatrixLookAtRH( &viewx, &eye, &look, &up ) ;
  hr = gpu->SetTransform( D3DTS_VIEW, &viewx ) ;
  DX_CHECK( hr, "setting view xform" ) ;

}

void D3DWindow::project3D()
{
  HRESULT hr ;

  // Projection transformation
  D3DXMATRIX projx ;
  D3DXMatrixPerspectiveFovRH( &projx, (float)(PI/4),
    (float)getWidth()/getHeight(), 1.0f, 1000.0f ) ;
  hr = gpu->SetTransform( D3DTS_PROJECTION, &projx ) ;
  DX_CHECK( hr, "setting projection xform" ) ;

  // Viewing transformation
  D3DXMATRIX viewx ;
  D3DXMatrixLookAtRH( &viewx, &eye, &look, &up ) ;
  hr = gpu->SetTransform( D3DTS_VIEW, &viewx ) ;
  DX_CHECK( hr, "setting view xform" ) ;
}

// 3d
void D3DWindow::setCamera( D3DXVECTOR3 &newEye, D3DXVECTOR3 &newLook, D3DXVECTOR3 &newUp )
{
  HRESULT hr ;

  eye = newEye ;
  look = newLook ;
  up = newUp ;
  D3DXMATRIX viewx ;
  D3DXMatrixLookAtRH( &viewx, &eye, &look, &up ) ;
  hr = gpu->SetTransform( D3DTS_VIEW, &viewx ) ;

  DX_CHECK( hr, "setCamera, setting viewing xform" ) ;
}

void D3DWindow::setDrawingMode( DrawingMode dm )
{
  if( dm == D2 )
  {
    // Set up the camera
    project2D() ;
  }
  else if( dm == D3 )
  {
    project3D() ;
  }
  else
  {
    warning( "Invalid drawing mode selected, nothing changed" ) ;
  }

  // Save off drawing mode..
  drawingMode = dm ;
}

DrawingMode D3DWindow::getDrawingMode()
{
  return drawingMode ;
}

bool D3DWindow::beginDrawing()
{
  HRESULT hr ;

  if( isDeviceLost )
  {
    //warning( "The d3d device is lost right now, not beginning to draw" ) ;
    return false ;
  }

  hr = gpu->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    clearColor, 1.0f, 0 ) ;
  DX_CHECK( hr, "Clear error" ) ;

  hr = gpu->BeginScene() ;
  DX_CHECK( hr, "BeginScene error" ) ;

  // By __default__, put you in
  // 2d mode
  if( drawingMode == D3 )
    setDrawingMode( D3 ) ;
  else
    setDrawingMode( D2 ) ;

  return true ;
}

void D3DWindow::endDrawing()
{
  HRESULT hr ;

  if( isDeviceLost )
  {
    //warning( "The d3d device is lost right now, not ending the draw" ) ;
    return ;
  }

  hr = gpu->EndScene() ;
  DX_CHECK( hr, "EndScene FAILED!" ) ;

  // And finally, PRESENT what we drew to the backbuffer
  hr = gpu->Present( 0, 0, 0, 0 ) ;
  DX_CHECK( hr, "Present FAILED!" ) ;
}


void D3DWindow::setBackgroundColor( D3DCOLOR color )
{
  clearColor = color ;
}

//////////////
// Windowing
int D3DWindow::getWidth()
{
  return d3dpps.BackBufferWidth ;
}

int D3DWindow::getHeight()
{
  return d3dpps.BackBufferHeight ;
}

bool D3DWindow::setSize( int width, int height, bool fullScreen )
{
  info( "D3DWindow changing resolution to width=%d, height=%d", width, height ) ;

  // Before trying to setSize(), remember the old (default) present parameters
  D3DPRESENT_PARAMETERS oldd3dpps = d3dpps ;

  // Now update with new ones.
  d3dpps.BackBufferWidth = width ;
  d3dpps.BackBufferHeight = height ;
  d3dpps.Windowed = !fullScreen ;

  // to change, we need to lose the old device,
  // and reset with the new parameters

  d3dLoseDevice() ;
  if( !d3dResetDevice( d3dpps ) )
  {
    // if the change fails, reset with old options
    warning( "Could not put device into resolution width=%d height=%d fullscreen=%d", width, height, fullScreen ) ;
    d3dpps = oldd3dpps ;
    
    warning( "Reverting to old settings" ) ;
    if( !d3dResetDevice( d3dpps ) )
    {
      // if THAT fails, then you have a serious problem
      error( "Serious problem resetting the device" ) ;
      
      // try and completely reset d3d to stock options
      d3dShutdown() ;
      if( !initD3D() )
      {
        bail( "Couldn't even re-initialize d3d" ) ;
        return false ;
      }
    }
  }


  
  Window::setSize( width, height, fullScreen ) ;
  return true ;
}

bool D3DWindow::fullScreenInMaxResolution()
{
  return setSize( GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), true ) ;
}


void D3DWindow::registerFont( ID3DXFont* font )
{
  registeredFonts.push_back( font ) ;
}

void D3DWindow::registerSpriteRenderer( ID3DXSprite* spriteRenderer )
{
  registeredSprites.push_back( spriteRenderer ) ;
}

void D3DWindow::registerLine( ID3DXLine* line )
{
  registeredLines.push_back( line ) ;
}