////////////////////////////////////////////////////
// Windows "plumbing".                            //
// You can safely ignore eveorything in this file. //
////////////////////////////////////////////////////

#include "WindowClass.h"

// A class that "abstracts away" the process
// of getting a window up on the screen.
Window::Window( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight )
{
  // Save off these parameters in private instance variables.
  hInstance = hInst ;
  d3dpps.BackBufferWidth = windowWidth ;
  d3dpps.BackBufferHeight = windowHeight ;

  // Create a window.
  WNDCLASSEX window = { 0 } ;
  window.cbSize			= sizeof( WNDCLASSEX ) ;
  window.hbrBackground	= (HBRUSH)GetStockObject( WHITE_BRUSH );
  window.hCursor = LoadCursor( NULL, IDC_ARROW ) ;
  window.hIcon = LoadIcon( NULL, IDI_APPLICATION ) ;
  window.hIconSm = LoadIcon( NULL, IDI_APPLICATION ) ;
  window.hInstance = hInstance ;
  window.lpfnWndProc = WndProc ;
  window.lpszClassName = TEXT( "myWindow" ) ;  // XXXX MATCH ME XXXX
  window.lpszMenuName = NULL;
  window.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;

  if(!RegisterClassEx( &window ))
  {
    bail( "Something's wrong with the WNDCLASSEX structure you defined.. quitting" ) ;
    return ;
  }

  RECT wndSize ;
  wndSize.left = windowXPos ;
  wndSize.right = windowXPos + windowWidth ;
  wndSize.top = windowYPos ;
  wndSize.bottom = windowYPos + windowHeight ;

  AdjustWindowRectEx( &wndSize, WS_OVERLAPPEDWINDOW, NULL, 0 ) ;

  // Create the main window
  hwnd = CreateWindowEx(

    0 /*WS_EX_TOPMOST*/,  // extended window style.. if set to WS_EX_TOPMOST, for example,
    // then your window will be the topmost window all the time.  Setting it to 0
    // will make your window just another regular everyday window (not topmost or anything).

    TEXT( "myWindow" ),  // window class name.. must match XXXX MATCH ME XXXX above, exactly

    windowTitleBar,      // text in title bar of your window

    WS_OVERLAPPEDWINDOW, // window style.  Try using WS_POPUP, for example.

    wndSize.left, wndSize.top,
    wndSize.right - wndSize.left, wndSize.bottom - wndSize.top,

    // Don't worry about the next 4, not important for now.
    NULL, NULL,
    hInstance, NULL
  ) ;
  ShowWindow( hwnd, SW_SHOWNORMAL ) ;
  UpdateWindow( hwnd ) ;

  info( "Starting up Direct3D..." ) ;
  if( !initD3D() )
  {
    bail( "D3D failed to initialize. Check your set up params in initD3D() function, check your width and height are valid" ) ;
  }
  defaultSprite = new Sprite( gpu, NULL ) ; // Initialize the default sprite

  info( "Starting up rawinput devices..." ) ;
  startupRawInput();

  info( "Turning off the mouse cursor..." ) ;
  ShowCursor( FALSE ) ;

  // start up fmod
  initFMOD() ;

}

Window::~Window()
{
  // ... clean up and shut down ... 

  // D3D
  foreach( SpriteMapIter, iter, sprites )
  {
    delete iter->second ;
  }

  SAFE_RELEASE( gpu ) ;
  SAFE_RELEASE( d3d ) ;

  // FMOD
  SoundMapIter iter ;
  foreach( SoundMapIter, iter, sounds )
  {
    FMOD_ErrorCheck( FMOD_Sound_Release( iter->second ) ) ;
  }
  FMOD_ErrorCheck( FMOD_System_Release( fmodSys ) ) ;
  
}


bool Window::initD3D()
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
  d3dpps.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE ; // FRAMERATE::UNBOUNDED  You need to
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
  


  hr = gpu->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | (1 << D3DFVF_TEXCOUNT_SHIFT) ) ;
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

  D3DVERTEXELEMENT9 uv ;
  uv.Usage = D3DDECLUSAGE_TEXCOORD ;
  uv.UsageIndex = 0 ;
  uv.Stream = 0 ;
  uv.Type = D3DDECLTYPE_FLOAT2 ;
  uv.Offset = cumulativeOffset ;
  uv.Method = D3DDECLMETHOD_DEFAULT ;
  cumulativeOffset += 2*sizeof(float) ;

  D3DVERTEXELEMENT9 vertexElements[] =
  {
    pos,
    col,
    uv,

    D3DDECL_END()
  } ;

  IDirect3DVertexDeclaration9 * Vdecl ;

  hr = gpu->CreateVertexDeclaration( vertexElements, &Vdecl ) ;
  DX_CHECK( hr, "CreateVertexDeclaration" ) ;

  hr = gpu->SetVertexDeclaration( Vdecl ) ;
  DX_CHECK( hr, "SetVertexDeclaration" ) ;

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


  
  D3DXCreateFont( gpu, 18, 0, FW_BOLD, 0,
    FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &id3dxFontArial12 ) ;

  D3DXCreateSprite( gpu, &id3dxSpriteRenderer ) ;

  // start the mouse in the middle of the screen
  mouseX = getWidth() / 2 ;
  mouseY = getHeight() / 2 ;

  return true ;
}

void Window::drawSprite( int id, float x, float y, float width, float height )
{
  Sprite *sprite = defaultSprite ;
  SpriteMapIter spriteEntry = sprites.find( id ) ;

  if( spriteEntry != sprites.end() )
    sprite = spriteEntry->second ;
  else
    warning( "Sprite %d not loaded, using default sprite instead", id ) ;
    
  D3DXVECTOR2 center( x - sprite->getCenterX(), y - sprite->getCenterY() ) ;
  D3DXVECTOR2 scale( sprite->getScaleXFor( width ), sprite->getScaleYFor( height ) ) ;

  D3DXVECTOR3 vec3Center( x, y, 0 ) ;
  D3DXVECTOR3 vec3Pos( x, y, 0 ) ;

  D3DXMATRIX matrix ;
  D3DXMatrixAffineTransformation2D( &matrix, 1.0f, &center, 0.0f, &center ) ;

  id3dxSpriteRenderer->Begin( D3DXSPRITE_ALPHABLEND ) ;
  id3dxSpriteRenderer->SetTransform( &matrix ) ;

  RECT rect = sprite->getRect() ;
  id3dxSpriteRenderer->Draw(
    
    sprite->getTexture(),
    &rect,
    &vec3Center,
    &vec3Pos,
    D3DCOLOR_ARGB( 255, 255, 255, 255 )
  
  ) ;
  id3dxSpriteRenderer->End();

}

// id is how you will refer to this sprite after its been loaded
// filename is just the filename on the disk drive
void Window::loadSprite( int id, char *filename,
                         D3DCOLOR backgroundColor,
                         int singleSpriteWidth, int singleSpriteHeight,
                         int numFrames, float timeBetweenFrames )
{
  Sprite *sprite = new Sprite( gpu, filename, backgroundColor,
    singleSpriteWidth, singleSpriteHeight, numFrames, timeBetweenFrames ) ;
  

  SpriteMapIter existingSpritePtr = sprites.find( id ) ;
  if( existingSpritePtr != sprites.end() )
  {
    // the sprite with that id existed.  erase it
    warning( "Sprite with id=%d already existed.  Destroying it..", id ) ;
    delete existingSpritePtr->second ;
  }

  // now add it
  sprites.insert( make_pair( id, sprite ) ) ;
}

int Window::randomSpriteId()
{
  SpriteMapIter iter = sprites.begin() ;
  advance( iter, rand()%sprites.size() ) ;

  return iter->first ;
}

void Window::drawAxes()
{
  
  static D3DVertex axis[] = {

    // x-axis is red
    D3DVertex( -getWidth(), 0, 0, 0, 0, 255, 0, 0 ),
    D3DVertex( +getWidth(), 0, 0, 0, 0, 255, 0, 0 ),

    // y-axis green
    D3DVertex( 0, -getHeight(), 0, 0, 0, 0, 255, 0 ),
    D3DVertex( 0, +getHeight(), 0, 0, 0, 0, 255, 0 ),

    // z-axis blue
    D3DVertex( 0, 0, -getHeight(), 0, 0, 0, 0, 255 ),
    D3DVertex( 0, 0, +getHeight(), 0, 0, 0, 0, 255 )

  } ;


  HRESULT hr = gpu->SetTexture( 0, NULL ) ;
  DX_CHECK( hr, "unset the SetTexture" ) ;
  
  hr = gpu->DrawPrimitiveUP( D3DPT_LINELIST, 3, axis, sizeof( D3DVertex ) ) ;
  DX_CHECK( hr, "DrawPrimitiveUP FAILED!" ) ;

  static float pointSize = 8.0f ;

  gpu->SetRenderState( D3DRS_POINTSIZE, CAST_AS_DWORD( pointSize ) ) ;

  // Draw points at end of axis.
  static D3DVertex points[] = {
    D3DVertex( getWidth(), 0, 0, 0, 0, 255, 0, 0 ),
    D3DVertex( 0, getHeight(), 0, 0, 0, 0, 255, 0 ),
    D3DVertex( 0, 0, getHeight(), 0, 0, 0, 0, 255 ),
  } ;

  hr = gpu->DrawPrimitiveUP( D3DPT_POINTLIST, 3, points, sizeof( D3DVertex ) ) ;
  DX_CHECK( hr, "DrawPrimitiveUP FAILED!" ) ;
}

bool Window::beginDrawing()
{
  HRESULT hr ;

  if( isDeviceLost )
  {
    //warning( "The d3d device is lost right now, not beginning to draw" ) ;
    return false ;
  }

  hr = gpu->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    D3DCOLOR_ARGB( 255, 0, 10, 45 ), 1.0f, 0 ) ;
  DX_CHECK( hr, "Clear error" ) ;

  #pragma region set up the camera
  D3DXMATRIX projx ;

  D3DXMatrixOrthoRH( &projx, getWidth(), getHeight(), 1, 100 ) ;
  //D3DXMatrixPerspectiveFovRH( &projx, PI/4, backBufferWidth/backBufferHeight, 1.0f, 1000.0f ) ;

  gpu->SetTransform( D3DTS_PROJECTION, &projx ) ;

  D3DXMATRIX viewx ;

  int x = getWidth() / 2 ;
  int y = getHeight() / 2 ;
  D3DXVECTOR3 eye( x, y, 10 ) ;
  D3DXVECTOR3 look( x, y, 0 ) ;
  D3DXVECTOR3 up( 0, 1, 0 ) ;
  D3DXMatrixLookAtRH( &viewx, &eye, &look, &up ) ;
  gpu->SetTransform( D3DTS_VIEW, &viewx ) ;
  #pragma endregion

  hr = gpu->BeginScene() ;
  DX_CHECK( hr, "BeginScene error" ) ;

  return true ;
}

void Window::endDrawing()
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

void Window::initFMOD()
{
  FMOD_ErrorCheck( FMOD_System_Create( &fmodSys ) ) ;

  UINT version ;
  FMOD_ErrorCheck( FMOD_System_GetVersion( fmodSys, &version ) ) ;

  if( version < FMOD_VERSION ) 
  {
    // if the version being used to run
    // this program is different from the
    // version the program was compiled with,
    // print an error
    error("FMOD Error!  You are using an old version of FMOD %08x.  This program requires %08x", version, FMOD_VERSION);
  }


  FMOD_ErrorCheck( FMOD_System_Init( fmodSys, 32, FMOD_INIT_NORMAL, NULL ) ) ;


  int numSamples = 8000 ;
  int fs = 44100 ;
  float NTs  = 1.0f/fs ; // sampling interval
  short *data = new short[ numSamples ] ;

/*
    Frequencies for equal tempered tuning
    1    2    3     4     5    6     7     8     (( OCTAVE ))
C   32   65   131   262   523  1047  2093  4186
C#  35   69   139   277   554  1109  2218  4435
D   37   73   147   294   587  1175  2349  4699
D#  39   78   156   311   622  1245  2489  4978
E   41   82   165   330   659  1319  2637  5274
F   44   87   175   349   698  1397  2794  5588
F#  46   93   185   370   740  1480  2960  5920
G   49   98   196   392   784  1568  3136  6272
G#  52   104  208   415   831  1661  3322  6645
A   55   110  220   440   880  1760  3520  7040
A#  58   117  233   466   932  1865  3729  7459
B   61   123  247   494   988  1976  3951  7902
*/

  //addSinewave( data, numSamples, 0, 65, 5000, false ) ;
  //addSinewave( data, 4000, 1000, 131, 5000, false ) ;
  //addSinewave( data, 3000, 2000, 262, 5000, false ) ;
  //addSinewave( data, 4000, 3000, 523, 5000, false ) ;

  // A footstep
  //for( int i = 60 ; i < 18000 ; i++ )
  //  addSinewave( data, 8000, 0, i, 5, false ) ;

  // A heeled footstep
  //for( int i = 60 ; i < 18000 ; i+=10 )
  //  addSinewave( data, 8000, 0, i, 10, false ) ;

  // This is that awfully annoying "spark" sound
  // that the guys in the topman stage make in megaman 3
  //for( int i = 60 ; i < 18000 ; i+=i )
  //  addSinewave( data, 8000, 0, i, i, false ) ;

  // Some kind of error
  //addSlidingSinewave( data, 8000, 0, 330, 294, 6000, 6000, false ) ;
  //addSlidingSinewave( data, 8000, 0, 587, 659, 6000, 6000, false ) ;

  /*
  // a "warf" kind of sound
  int noteLen = 500 ;
  int pos = -noteLen ;
  short amp = 12000 ;
  addSinewave( data, noteLen, pos += noteLen, 330, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 349, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 370, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 392, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 415, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 440, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 466, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 494, amp, false ) ;
  */

  // a "warf" kind of sound, c scale
  /*
  int noteLen = 500 ;
  int pos = -noteLen ;
  short amp = 12000 ;
  addSinewave( data, noteLen, pos += noteLen, 262, amp, false ) ; // C
  addSinewave( data, noteLen, pos += noteLen, 294, amp, false ) ; // D
  addSinewave( data, noteLen, pos += noteLen, 330, amp, false ) ; // E
  addSinewave( data, noteLen, pos += noteLen, 349, amp, false ) ; // F
  addSinewave( data, noteLen, pos += noteLen, 392, amp, false ) ; // G
  addSinewave( data, noteLen, pos += noteLen, 440, amp, false ) ; // A
  addSinewave( data, noteLen, pos += noteLen, 494, amp, false ) ; // B
  addSinewave( data, noteLen, pos += noteLen, 523, amp, false ) ; // C
  */

  int noteLen = 4000 ;
  int pos = -noteLen ;
  short amp = 12000 ;
  addSinewave( data, noteLen, pos += noteLen, 392, amp, false ) ; // G
  addSinewave( data, noteLen, pos += noteLen, 262, amp, false ) ; // C
  
  
  
  
  
  // This sounds like a kick drum
  //addSlidingSinewave( data, 8000, 0, 131, 62, 16000, 16000, false ) ;

  // This sounds like bumping into a solid wall in mario bros 1
  //addSlidingSquarewave( data, 8000, 0, 131, 62, 12000, 12000 ) ;
  
  // A really squeaky objection.
  //addSlidingSquarewave( data, 8000, 0, 2000, 100, 12000, 12000 ) ;

  // laser fire
  //addSlidingSquarewave( data, 8000, 0, 4000, 2000, 6000, 6000 ) ;
  //addSlidingSquarewave( data, 8000, 0, 16000, 19000, 6000, 6000 ) ;
  
  
  //addSquarewave( data, 5000, 2000, 1047, 1000 ) ;
  //addSquarewave( data, 5000, 2000, 523, 666, 1000 ) ;

  FMOD_CREATESOUNDEXINFO soundInfo = {0};
  soundInfo.cbsize = sizeof( FMOD_CREATESOUNDEXINFO ) ;
  soundInfo.length = numSamples * sizeof( short ) ;
  soundInfo.defaultfrequency = fs ;
  soundInfo.numchannels = 1 ;
  soundInfo.userdata = data ;
  soundInfo.format = FMOD_SOUND_FORMAT::FMOD_SOUND_FORMAT_PCM16 ;

  // Generate the default sound
  FMOD_ErrorCheck(
    FMOD_System_CreateSound(
      fmodSys,
      (char*)data,
      FMOD_HARDWARE | FMOD_CREATESAMPLE | FMOD_OPENUSER,
      &soundInfo, &defaultSound
    )
  ) ;

  void *ptr1, *ptr2 ;
  unsigned int bytesLocked1, bytesLocked2 ;
  FMOD_ErrorCheck(
    FMOD_Sound_Lock( defaultSound, 0, numSamples * sizeof( short ), &ptr1, &ptr2, &bytesLocked1, &bytesLocked2 ) 
  ) ;

  info( "locked %d, %d bytes", bytesLocked1, bytesLocked2 ) ;
  memcpy( ptr1, data, bytesLocked1 ) ;
  
  FMOD_ErrorCheck(
    FMOD_Sound_Unlock( defaultSound, ptr1, ptr2, bytesLocked1, bytesLocked2 ) 
  ) ;

  // erase the local copy of the generated data
  //free( data ) ;
}

void Window::loadSound( int id, char * filename, int options )
{
  FMOD_SOUND *newSound ;

  FMOD_ErrorCheck( FMOD_System_CreateSound( fmodSys, filename, FMOD_HARDWARE | options, 0, &newSound ) ) ;

  sounds.insert( make_pair( id, newSound ) ) ;
}

void Window::playSound( int id )
{
  FMOD_SOUND *sound = defaultSound ;
  SoundMapIter soundEntry = sounds.find( id ) ;
  
  if( soundEntry != sounds.end() )
    sound = soundEntry->second ;
  else
    warning( "Sound id=%d doesn't exist", id ) ;

  FMOD_System_PlaySound( fmodSys, FMOD_CHANNEL_FREE, sound, false, &fmodChannel ) ;
}

void Window::drawString( char *str, float x, float y, float width, float height, D3DCOLOR color, DWORD formatOptions )
{
  RECT rect ;
  SetRect( &rect, x, y, x + width, y + height ) ;

  id3dxFontArial12->DrawTextA( NULL, str, -1, &rect, formatOptions, color ) ;
}

int Window::getWidth()
{
  return d3dpps.BackBufferWidth ;
}

int Window::getHeight()
{
  return d3dpps.BackBufferHeight ;
}

bool Window::fullScreenInMaxResolution()
{
  return setSize( GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), true ) ;
}

bool Window::setSize( int width, int height, bool fullScreen )
{
  
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
      }
    }
  }


  // Ok, now that d3d buffer/device resize was successful,
  // resize the actual window now, if its in windowed mode

  RECT wndSize ;
  wndSize.left = 0 ;
  wndSize.right = width ;
  wndSize.top =  0 ;
  wndSize.bottom = height ;

  // We have to AdjustWindowRectEx() so the client area
  // is exactly the right size
  AdjustWindowRectEx( &wndSize, WS_OVERLAPPEDWINDOW, NULL, 0 ) ;

  SetWindowPos( hwnd, HWND_TOP, 0, 0, wndSize.right - wndSize.left, wndSize.bottom - wndSize.top, 
    SWP_NOMOVE | SWP_NOZORDER ) ;

  return true ;
}

void Window::d3dLoseDevice()
{
  isDeviceLost = true ;

  // call the onLostDevice function of every id3dx object
  DX_CHECK( id3dxFontArial12->OnLostDevice(), "font onlostdevice" ) ;
  DX_CHECK( id3dxSpriteRenderer->OnLostDevice(), "sprite renderer onlostdevice" ) ;
}

bool Window::d3dResetDevice( D3DPRESENT_PARAMETERS & pps )
{
  HRESULT hr ;

  hr = gpu->Reset( &d3dpps ) ;
  bool succeeded = DX_CHECK( hr, "reset gpu device" ) ; // this still might fail.
  // If it does fail, then the device is STILL lost then.

  // This is written out this way to make it clear
  // how the program logic goes.
  if( !succeeded )
    isDeviceLost = true ;
  else
    isDeviceLost = false ;

  // If the previous call to Reset the GPU _worked_, then
  // we should reset all the id3dx objects
  if( !isDeviceLost )
  {
    DX_CHECK( id3dxFontArial12->OnResetDevice(), "font onlostdevice" ) ;
    DX_CHECK( id3dxSpriteRenderer->OnResetDevice(), "sprite renderer onlostdevice" ) ;

    info( "GPU reset complete" ) ;
  }
  else
  {
    error( "I could not reset the gpu" ) ;
  }

  return succeeded ;
}

void Window::d3dDeviceCheck()
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

void Window::d3dShutdown()
{
  SAFE_RELEASE( id3dxSpriteRenderer ) ;
  SAFE_RELEASE( id3dxFontArial12 ) ;
  SAFE_RELEASE( gpu ) ;
  SAFE_RELEASE( d3d ) ;
}

//!! fix this out
bool Window::d3dSupportsNonPowerOf2Textures()
{
  bool conditionallySupportsNonPow2Tex = caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL ;

  return conditionallySupportsNonPow2Tex ;
}

Graphics* Window::createPrimitiveSurface()
{
  // Create a surface and return the Graphics* object

  // FOR NOW ITS A TEST SURFACE AND
  // WE'll JUST MAKE IT TEXTURE 1000

  int width = 128 ;
  int height = 128 ;

  // create a texture
  IDirect3DTexture9 *tex ;
  DX_CHECK(
    D3DXCreateTexture( gpu, width, height,
    1, 0, D3DFMT_X8R8G8B8,
    D3DPOOL_MANAGED, &tex ), "Create texture" ) ;
  
  // Get its surface
  IDirect3DSurface9 *surface ;
  DX_CHECK( tex->GetSurfaceLevel( 0, &surface ), "Get surface" ) ;

  HDC texDC ;
  DX_CHECK( surface->GetDC( &texDC ), "Get dc of texture" ) ;

  
  Pen bluePen( Color( 100, 0, 0, 255 ) ) ;
  SolidBrush blueBrush( Color( 200, 0, 0, 255 ) ) ;
  SolidBrush redBrush( Color( 128, 255, 0, 0 ) ) ;

  Graphics* g = new Graphics( texDC ) ;
  g->FillRectangle( &redBrush, 0, 0, 4, 4 ) ;
  


// test gif
  Image i( TEXT( "sprites/Astos.gif" ) ) ;
  g->DrawImage( &i, 0,0, 64, 64 ) ;


  surface->ReleaseDC( texDC ) ;

  







  // K now i'm going to hack a copy
  // of the texture because it doesn't let you use
  // A8R8G8B8 when you use a DC.  For some silly reason.
  // Hopefully the X8 will preserve the alpha values
  D3DLOCKED_RECT lockedRectTex, lockedRectCpyA8 ;




  // copy texture with A8
  IDirect3DTexture9 *cpyA8 ;

  DX_CHECK( D3DXCreateTexture(
    gpu, width, height,
    1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
    &cpyA8 ), "Create texture from file in memory" ) ;
  


  int dataLen = width*height*sizeof(int) ; // a8r8g8b8!

  // lock both
  DX_CHECK( tex->LockRect( 0, &lockedRectTex, NULL, 0 ), "Lock rect tex" ) ;
  DX_CHECK( cpyA8->LockRect( 0, &lockedRectCpyA8, NULL, 0 ), "Lock rect cpyA8" ) ;

  /*
  // YES the alpha channel data IS preserved in X8
  for( int i = 0 ; i < width; i++ )
  {
    for( int j = 0 ; j < height ; j++ )
    {
      printf( "%8x ", ((int*)lockedRect.pBits)[ i + width*j ] ) ;
    }
    puts("");
  }
  */
  memcpy( lockedRectCpyA8.pBits, lockedRectTex.pBits, dataLen ) ;

  DX_CHECK( cpyA8->UnlockRect( 0 ), "Unlock rect cpyA8" ) ;
  DX_CHECK( tex->UnlockRect( 0 ), "Unlock rect tex" ) ;

  Sprite *sprite= new Sprite( width, height, cpyA8 ) ;
  sprites.insert( make_pair( 1000, sprite ) ) ;
  
  
  return NULL ;

}


void Window::step()
{
  // check the d3ddevice, in case its been lost
  d3dDeviceCheck() ;

  FMOD_System_Update( fmodSys ) ; // call this once per "tick"


  // Advance sprite animations.
  foreach( SpriteMapIter, iter, sprites )
  {
    iter->second->advance( timer.time_since_last_frame ) ;
  }

  // Copy over "current" states to "previous" states
  memcpy( keyPreviousStates, keyCurrentStates, 256 ) ;

  // Grab all keystates, to know what the user is currently pushing down.
  if( !GetKeyboardState( keyCurrentStates ) )
  {
    printWindowsLastError( "GetKeyboardState()" ) ;
  }

  
  //timer.lock( 60 ) ; // // ^^Leave as last line: YES, RECOMMENDED.  Use this line to LOCK FRAMERATE
  // at 60 fps max.  This will "waste" any idle time at the end of
  // processing a frame.
  
  timer.update();  // NOT RECOMMENDED.  Use this line to SIMPLY UPDATE
  // the frame counter WITHOUT frame limiting.
  
  // This mode lets the game run AS FAST AS IT POSSIBLY CAN
  // on this machine, and you might see frame rates of
  // 300fps or so.  This means your game will vary in
  // speed though depending on how much "stuff" is on
  // the screen.  NOT RECOMMENDED.
  
  // NOTE:  YOU MUST ALSO FIND AND UNCOMMENT THE LINE
  // that says FRAMERATE::UNBOUNDED



}

bool Window::isSlow()
{
  return timer.frames_per_second < 55 ;
}

float Window::getTimeElapsedSinceLastFrame()
{
  return timer.time_since_last_frame ;
}

void Window::drawMouseCursor(int id)
{
  drawSprite( id, mouseX, mouseY, 10, 10 ) ;

  char buf[ 300 ] ;
  sprintf( buf, "mouse\n(%d, %d)", mouseX, mouseY ) ;
  drawString( buf, mouseX, mouseY, 80, 135, Color::Blue, DT_LEFT | DT_TOP ) ;
}

// Show fps
void Window::drawFrameCounter()
{
  static char buf[ 60 ];
  sprintf( buf, "FPS:  %.1f", timer.frames_per_second ) ;

  drawString( buf, getWidth() - 100, 20, 80, 100, Color::Blue, DT_TOP | DT_RIGHT ) ;
}


// Some may argue that you could expose the "paused"
// variable as a public member.. but using member functions
// like this allows you to actually perform "clean-up" type
// stuff on pause or unpause.  For example, here we're calling
// FMOD's pause function.  We get the opportunity to do it
// once, immediately as the pause() function is called.
// See?  OOP isn't all that bad!
void Window::pause()
{
  // If the game wasn't already paused..
  if( !paused )
  {
    // ..then pause everything

    paused = true ;  // this member is checked
    // in Window::step()
  }
}
void Window::unpause()
{
  paused = false ;
}
bool Window::isPaused()
{
  return paused ;
}


void Window::moveMouseX( int diffX )
{
  mouseX += diffX ;
  
  // x-clamp to screen bounds
  if( mouseX > getWidth() ) // if its too far right
    mouseX = getWidth() ;   // slam it to the right edge of the screen
  else if( mouseX < 0 )     // if its too far left
    mouseX = 0 ;            // slam it to the left edge of the screen
}
void Window::moveMouseY( int diffY )
{
  mouseY += diffY ;

  // y clamp
  if( mouseY > getHeight() )
    mouseY = getHeight() ;
  else if( mouseY < 0 )
    mouseY = 0 ;
}
int Window::getMouseX()
{
  return mouseX ;
}
int Window::getMouseY()
{
  return mouseY ;
}

// Returns true if key is DOWN this frame
// and was UP previous frame.
bool Window::justPressed( int vkCode )
{
  return KEY_IS_DOWN( keyCurrentStates, vkCode ) &&  // Down this frame, &&
         KEY_IS_UP( keyPreviousStates, vkCode ) ;     // AND up previous frame
  
  // bitwise AND logical AND's used here.

  // Two important bits of information from
  // GetKeyboardState() documentation on MSDN:
  // "If the high-order bit is 1,
  //  the key is down;
  //  otherwise, it is up."

  // "The low-order bit is meaningless for non-toggle keys."

}

// Tells you if a key is BEING HELD DOWN
bool Window::isPressed( int vkCode )
{
  return KEY_IS_DOWN( keyCurrentStates, vkCode ) ;
}

// Returns true if a key was JUST let go of.
// A complimentary function to justPressed()
bool Window::justReleased( int vkCode )
{
  return KEY_IS_DOWN( keyPreviousStates, vkCode ) &&  // Key __WAS__ down AND
         KEY_IS_UP( keyCurrentStates, vkCode ) ;      // KEY IS UP NOW
}


/////////////// PRIVATE ///////////////////
void Window::startupRawInput()
{
  // After the window has been created, 
  // register raw input devices
  RAWINPUTDEVICE Rid[2] ;
        
  Rid[0].usUsagePage = 0x01 ;
  Rid[0].usUsage = 0x02 ;
  
  ////Rid[0].dwFlags = 0 ; // (use this if you DO NOT WANT to capture mouse)
  Rid[0].dwFlags = RIDEV_CAPTUREMOUSE | RIDEV_NOLEGACY ;  // (use this to CAPTURE MOUSE)

  // RIDEV_CAPTUREMOUSE makes it so we
  // SEIZE UP THE MOUSE from the rest of the system.
  // This makes it so the user cannot accidently "click away"
  // from your app, which is good.

  // If you want the mouse to be "captured",
  // just set Rid[0].dwFlags=0;
  // You should also comment out the line
  // that says ShowCursor( FALSE ) ; if you
  // want the normal windows white mouse cursor
  // to show

  // To use this mode we must also specify RIDEV_NOLEGACY mode.
  // RIDEV_NOLEGACY makes it so we WILL NOT
  // get WM_LBUTTONDOWN ("old-school" aka 'legacy') messages.
  // Instead we will just get WM_INPUT messages.
  Rid[0].hwndTarget = hwnd ;

  // We don't really need raw input for the keyboard
  // but it is nice to have it hooked up because
  // 
  Rid[1].usUsagePage = 0x01 ;
  Rid[1].usUsage = 0x06 ;
  Rid[1].dwFlags = 0 ; // RIDEV_NOHOTKEYS ;  // use the RIDEV_NOHOTKEYS
  // option to turn off WINKEY
  
  // Also, for the keyboard, DO NOT specify RIDEV_NOLEGACY.
  // If you do, you will no longer be able to "hear"
  // WM_CHAR messages.  WM_CHAR messages are the best and
  // easiest way to get correct typing keystrokes
  // with UpPerCasEd aNd LowErCaseD letters as the user typed them.
  Rid[1].hwndTarget = hwnd ;

  if( !RegisterRawInputDevices( Rid, 2, sizeof(Rid[0]) ) )
  {
    //registration failed. Check your Rid structs above.
    printWindowsLastError( "RegisterRawInputDevices" ) ;
    bail( "Could not register raw input devices. Check your Rid structs, please." ) ;
  }


  // clear keystates
  memset( keyCurrentStates, 0, 256 ) ;
  memset( keyPreviousStates, 0, 256 ) ;
}