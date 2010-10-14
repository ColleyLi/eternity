#include "D3DWindow.h"

D3DWindow::D3DWindow( HINSTANCE hInst, TCHAR* windowTitleBar ) :
  Window( hInst, windowTitleBar, 32, 32, 800, 600 )
{
  info( "Starting up Direct3D in fullscreen exclusive mode.." ) ;
  if( !initD3D() )
  {
    bail( "D3D failed to initialize.  Maybe you should try a windowed mode instead." ) ;
  }
}

D3DWindow::D3DWindow( HINSTANCE hInst, TCHAR* windowTitleBar,
                     int windowXPos, int windowYPos,
                     int windowWidth, int windowHeight ) :
  Window( hInst, windowTitleBar,
          windowXPos, windowYPos,
          windowWidth, windowHeight )
{
  info( "Starting up Direct3D..." ) ;
  if( !initD3D( windowWidth, windowHeight ) )
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

bool D3DWindow::setupGPU()
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
  

  // here we need to set the actual window size,
  // in case there was a hard reset to the default 800x600 resolution
  Window::setSize( getWidth(), getHeight(), false ) ;

  initVertexDeclarations() ;

  setDefaultRenderStateOptions() ;

  clearColor = D3DCOLOR_ARGB( 255, 0, 10, 45 ) ;

  return true ;
}

bool D3DWindow::initD3D()
{
  return initD3D( GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) ) ;
}

bool D3DWindow::initD3D( int width, int height )
{
  memset( &d3dpps, 0, sizeof( D3DPRESENT_PARAMETERS ) ) ;

  d3dpps.BackBufferCount = 1 ;
  d3dpps.SwapEffect = D3DSWAPEFFECT_COPY ; // for screens.
  // if you discard, there's a risk that when you
  // try to copy the backbuffer it will be gone
  // (i suppose if you try to copy after Present call)
  d3dpps.BackBufferFormat = D3DFMT_UNKNOWN ;
  d3dpps.EnableAutoDepthStencil = true ;
  d3dpps.AutoDepthStencilFormat = D3DFMT_D16 ;
  d3dpps.hDeviceWindow = hwnd ;
  
  d3dpps.BackBufferFormat = D3DFMT_X8R8G8B8 ;
  
  d3dpps.Windowed = true ;
  d3dpps.BackBufferWidth = width ;
  d3dpps.BackBufferHeight = height ;
  //d3dpps.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE ; // FRAMERATE::UNBOUNDED  You need to
  // uncomment this line to make the GPU flip over really really fast

  return setupGPU() ;
}

void D3DWindow::setDefaultRenderStateOptions()
{
  HRESULT hr ;

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
  

}

void D3DWindow::screenshot()
{
  char buf[ 300 ] ;
  sprintNow( buf ) ;
  IDirect3DSurface9 *surface ;

  gpu->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface ) ;

  // svae the surfaace
  D3DXSaveSurfaceToFileA( buf,
    D3DXIFF_PNG, surface, NULL, NULL ) ;

}

void D3DWindow::initVertexDeclarations()
{
  HRESULT hr ;
  
  int cOffset = 0 ;
  D3DVERTEXELEMENT9 pos ;
  pos.Usage = D3DDECLUSAGE_POSITION ;
  pos.UsageIndex = 0 ;
  pos.Stream = 0 ;
  pos.Type = D3DDECLTYPE_FLOAT3 ;
  pos.Offset = cOffset ;
  pos.Method = D3DDECLMETHOD_DEFAULT ; 
  cOffset += 3*sizeof(float) ;

  D3DVERTEXELEMENT9 color ;
  color.Usage = D3DDECLUSAGE_COLOR ;
  color.UsageIndex = 0 ;
  color.Stream = 0 ;
  color.Type = D3DDECLTYPE_D3DCOLOR ;
  color.Offset = cOffset ;
  color.Method = D3DDECLMETHOD_DEFAULT ;
  cOffset += sizeof( D3DCOLOR ) ;

  D3DVERTEXELEMENT9 tex ;
  tex.Usage = D3DDECLUSAGE_TEXCOORD ;
  tex.UsageIndex = 0 ;
  tex.Stream = 0 ;
  tex.Type = D3DDECLTYPE_FLOAT2 ;
  tex.Offset = cOffset ;
  tex.Method = D3DDECLMETHOD_DEFAULT ;
  cOffset += 2*sizeof(float);

  D3DVERTEXELEMENT9 norm ;
  norm.Usage = D3DDECLUSAGE_NORMAL ;
  norm.UsageIndex = 0 ;
  norm.Stream = 0 ;
  norm.Type = D3DDECLTYPE_FLOAT3 ;
  norm.Offset = cOffset ;
  norm.Method = D3DDECLMETHOD_DEFAULT ; 
  cOffset += 3*sizeof(float);
  
  D3DVERTEXELEMENT9 end = D3DDECL_END() ;


  ///
  // VC  Vertex + Color
  D3DVERTEXELEMENT9 *vertexElementsVC = new D3DVERTEXELEMENT9[3] ;
  vertexElementsVC[0] = pos ;
  color.Offset = 3*sizeof(float);
  vertexElementsVC[1] = color ;
  vertexElementsVC[2] = end ;
  
  hr = gpu->CreateVertexDeclaration( vertexElementsVC, &vc ) ;
  DX_CHECK( hr, "CreateVertexDeclaration vc" ) ;
  DESTROY_ARRAY( vertexElementsVC ) ;
  
  ///
  // V:  Vertices only.  Uses a material for color.
  D3DVERTEXELEMENT9 *vertexElementsV = new D3DVERTEXELEMENT9[2] ;
  vertexElementsV[ 0 ] = pos ;
  vertexElementsV[ 1 ] = end ;
  // CREATE THE DECLARATION
  DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsV, &v ), "CreateVertexDeclaration FAILED v!" ) ;
  DESTROY_ARRAY( vertexElementsV ) ;

  // VT
  // Vertices and textures.
  D3DVERTEXELEMENT9 *vertexElementsVT = new D3DVERTEXELEMENT9[3] ;
  vertexElementsVT[ 0 ] = pos ;
  tex.Offset = 3*sizeof(float) ;
  vertexElementsVT[ 1 ] = tex ;
  vertexElementsVT[ 2 ] = end ;
  DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsVT, &vt ), "CreateVertexDeclaration FAILED vt!" ) ;
  DESTROY_ARRAY( vertexElementsVT ) ;


  // VTC
  // Vertex, texture, color.
  D3DVERTEXELEMENT9 *vertexElementsVTC = new D3DVERTEXELEMENT9[4] ;
  vertexElementsVTC[ 0 ] = pos ;

  tex.Offset = 3*sizeof(float) ;
  vertexElementsVTC[ 1 ] = tex ;

  color.Offset = tex.Offset + 2*sizeof(float) ;
  vertexElementsVTC[ 2 ] = color ;
  vertexElementsVTC[ 3 ] = end ;
  DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsVTC, &vtc ),
    "CreateVertexDeclaration FAILED vtc!" ) ;
  DESTROY_ARRAY( vertexElementsVTC ) ;


  // VTN
  D3DVERTEXELEMENT9 *vertexElementsVTN = new D3DVERTEXELEMENT9[4] ;
  vertexElementsVTN[ 0 ] = pos ;

  tex.Offset = 3*sizeof(float);
  vertexElementsVTN[ 1 ] = tex ;

  norm.Offset = tex.Offset + 2*sizeof(float);
  vertexElementsVTN[ 2 ] = norm ;
  vertexElementsVTN[ 3 ] = end ;
  DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsVTN, &vtn ), "CreateVertexDeclaration FAILED vtn!" ) ;
  DESTROY_ARRAY( vertexElementsVTN ) ;

  // VN
  D3DVERTEXELEMENT9 *vertexElementsVN = new D3DVERTEXELEMENT9[3] ;
  vertexElementsVN[ 0 ] = pos ;
  norm.Offset = 3*sizeof(float);  // This needed to be updated
  vertexElementsVN[ 1 ] = norm ;
  vertexElementsVN[ 2 ] = end ;
  DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsVN, &vn ), "CreateVertexDeclaration FAILED vn!" ) ;
  DESTROY_ARRAY( vertexElementsVN ) ;

  // After constructing it, set it as the one to use by default
  setVertexDeclaration( VertexType::PositionColor ) ;
}

void D3DWindow::setVertexDeclaration( VertexType vType )
{
  IDirect3DVertexDeclaration9 * vdecl = NULL ;

  switch( vType )
  {
  case Position:
    vdecl = v ;
    break;

  case PositionTexture:
    vdecl = vt ;
    break;

  case PositionTextureColor:
    vdecl = vtc ;
    break;

  case PositionNormal:
    vdecl = vn ;
    break;

  case PositionTextureNormal:
    vdecl = vtn ;
    break;

  case PositionColor:
  default:
    vdecl = vc ;
  }

  DX_CHECK( gpu->SetVertexDeclaration( vdecl ), "setDefaultVertexDeclaration" ) ;
  
}

void D3DWindow::setLight( int index, D3DLIGHT9* light )
{
  DX_CHECK( gpu->SetLight( index, light ), "SetLight" ) ;
  DX_CHECK( gpu->LightEnable( index, TRUE ), "LightEnable" ) ;
}

void D3DWindow::setLighting( BOOL on )
{
  DX_CHECK( gpu->SetRenderState( D3DRS_LIGHTING, on ), "Lighting state" ) ;
}

void D3DWindow::setMaterial( D3DMATERIAL9* material )
{
  DX_CHECK( gpu->SetMaterial( material ), "Set material" ) ;
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
    if( !initD3D( getWidth(), getHeight() ) )
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
  return DX_CHECK( hr, "BeginScene error" ) ;
}

bool D3DWindow::endDrawing()
{
  HRESULT hr ;

  if( isDeviceLost )
  {
    //warning( "The d3d device is lost right now, not ending the draw" ) ;
    return false ;
  }

  hr = gpu->EndScene() ;
  DX_CHECK( hr, "EndScene FAILED!" ) ;

  // And finally, PRESENT what we drew to the backbuffer
  hr = gpu->Present( 0, 0, 0, 0 ) ;
  return DX_CHECK( hr, "Present FAILED!" ) ;
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

      // DEFAULT to an 800x600 resolution,
      if( !initD3D( 800, 600 ) )
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