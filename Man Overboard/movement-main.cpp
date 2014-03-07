#pragma warning (disable:4786)

// based on code provided by Matt Buckland, Programming Game AI by Example

#include <windows.h>
#include <time.h>

#include "constants.h"
#include "utils.h"
#include "GameWorld.h"
#include "Cgdi.h"
#include "WindowUtils.h"
#include "PrecisionTimer.h"

//forward declaration of message handling function
LRESULT CALLBACK WindowProc (HWND   hwnd,
                             UINT   msg,
                             WPARAM wParam,
                             LPARAM lParam);

//--------------------------------- Globals ------------------------------
//
//------------------------------------------------------------------------

char*   g_szApplicationName = "Man Overboard";
char*	g_szWindowClassName = "MyWindowClass";

GameWorld* g_GameWorld;


//-------------------------------- WinMain -------------------------------
//
//	The entry point of the windows program
//------------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     szCmdLine, 
                    int       iCmdShow)
{
  //handle to our window
  HWND						hWnd;
    
  //our window class structure  NOT a C++ class!
  WNDCLASSEX     winclass;
		 
  // first fill in the window class stucture
  winclass.cbSize        = sizeof(WNDCLASSEX);         // size of structure
  winclass.style         = CS_HREDRAW | CS_VREDRAW;    // Winodwstyle - can also be set to 0
  winclass.lpfnWndProc   = WindowProc;                 // callback for all messages
  winclass.cbClsExtra    = 0;                          // extra data for this class
  winclass.cbWndExtra    = 0;                          // extradata for instances of the class
  winclass.hInstance     = hInstance;                  // parent applicaiton (paramenter fromWinMain)
  winclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);  // icon used in ALT+TAB switchin
  winclass.hCursor       = LoadCursor(NULL, IDC_ARROW); // cursor
  winclass.hbrBackground = NULL;                        // background Brush for colouring window
  winclass.lpszMenuName  = NULL;                        // menu resource naem fro this class
  winclass.lpszClassName = g_szWindowClassName;         // name for this class
  winclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);  //taskbar icon for htis class

  //register the window class
  if (!RegisterClassEx(&winclass))
  {
    MessageBox(NULL, "Registration Failed!", "Error", 0);

    //exit the application
    return 0;
  }

  //create the window and assign its ID to hwnd    
  hWnd = CreateWindowEx (NULL,                 // extended style
                         g_szWindowClassName,  // window class name
                         g_szApplicationName,  // window caption
                         WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,  // window styles
                         GetSystemMetrics(SM_CXSCREEN)/2 - constWindowWidth/2,  // position centrally
                         GetSystemMetrics(SM_CYSCREEN)/2 - constWindowHeight/2,                    
                         constWindowWidth,     // initial x size
                         constWindowHeight,    // initial y size
                         NULL,                 // parent window handle
                         NULL,                 // window menu handle
                         hInstance,            // program instance handle
                         NULL);                // creation parameters

  //make sure the window creation has gone OK
  if(!hWnd)
  {
    MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
  }

     
  //make the window visible
  ShowWindow (hWnd, iCmdShow);
  UpdateWindow (hWnd);

  // Enter the message loop
  bool bDone = false;

  PrecisionTimer timer;
  timer.Start();

  MSG msg;

  while(!bDone)
  {		
    while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
    {
      if( msg.message == WM_QUIT  || msg.message == WM_CLOSE) 
      {
        //stop loop if it's a quit message
	      bDone = true;
      } 
      else 
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
    }

    if (msg.message != WM_QUIT && msg.message != WM_CLOSE)
    {      
	  // update
	  g_GameWorld->Update(timer.TimeElapsed());
      //render
      RedrawWindow(hWnd, false);

      Sleep(4);
    }
   					
  }//end while

  delete g_GameWorld;

  UnregisterClass( g_szWindowClassName, winclass.hInstance );

  return msg.wParam;
}


//---------------------------- WindowProc ---------------------------------
//	
//	This is the callback function which handles all the windows messages
//-------------------------------------------------------------------------

LRESULT CALLBACK WindowProc (HWND   hwnd,
                             UINT   msg,
                             WPARAM wParam,
                             LPARAM lParam)
{
   //these hold the dimensions of the client window area
	 static int cxClient, cyClient; 

	 //used to create the back buffer
   static HDC		hdcBackBuffer;
   static HBITMAP	hBitmap;
   static HBITMAP	hOldBitmap;

    switch (msg)
    {
	
		//A WM_CREATE msg is sent when your application window is first
		//created
    case WM_CREATE:
      {
         //to get get the size of the client window first we need  to create
         //a RECT and then ask Windows to fill in our RECT structure with
         //the client window size. Then we assign to cxClient and cyClient 
         //accordingly
			   RECT rect;

			   GetClientRect(hwnd, &rect);

			   cxClient = rect.right;
			   cyClient = rect.bottom;

         //seed random number generator
         srand((unsigned) time(NULL));  
         
         //---------------create a surface to render to(backbuffer)
         //create a memory device context
         hdcBackBuffer = CreateCompatibleDC(NULL);

         //get the DC for the front buffer
         HDC hdc = GetDC(hwnd);

         hBitmap = CreateCompatibleBitmap(hdc,
                                          cxClient,
                                          cyClient);
			  
         //select the bitmap into the memory device context
	     hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

         //don't forget to release the DC
         ReleaseDC(hwnd, hdc); 
         
         g_GameWorld = new GameWorld(cxClient, cyClient);      
      } // end WM_CREATE

      break;

    case WM_COMMAND:
    {
      g_GameWorld->HandleMenuItems(wParam, hwnd); 
    }

    break;

    case WM_KEYUP:
      {
        switch(wParam)
        {
           case VK_ESCAPE:
            {             
              SendMessage(hwnd, WM_DESTROY, NULL, NULL);
            }
          
            break;

          case 'R':
            {
               delete g_GameWorld;
           
               g_GameWorld = new GameWorld(cxClient, cyClient);
            }

            break;
           

        }//end switch

        //handle any others
        g_GameWorld->HandleKeyPresses(wParam);
        
      }//end WM_KEYUP

      break;
    
    case WM_PAINT:
      { 		       
         PAINTSTRUCT ps;
          
         BeginPaint (hwnd, &ps);

        //fill our backbuffer with white
         BitBlt(hdcBackBuffer,
                0,
                0,
                cxClient,
                cyClient,
                NULL,
                NULL,
                NULL,
                WHITENESS);
		          
         gdi->StartDrawing(hdcBackBuffer);
         
         g_GameWorld->Render();

         gdi->StopDrawing(hdcBackBuffer);

         //now blit backbuffer to front
		 BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY); 
          
         EndPaint (hwnd, &ps);
      } // end WM_PAINT

      break;
        
	case WM_DESTROY:
	 {

         //clean up our backbuffer objects
         SelectObject(hdcBackBuffer, hOldBitmap);

         DeleteDC(hdcBackBuffer);
         DeleteObject(hBitmap); 

         
         
         // kill the application, this sends a WM_QUIT message  
		 PostQuitMessage (0);
	 }

       break;

     }//end switch

     //this is where all the messages not specifically handled by our 
		 //winproc are sent to be processed
		 return DefWindowProc (hwnd, msg, wParam, lParam);
}

