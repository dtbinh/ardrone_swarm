# Include "ardrone/ardrone.h"

 
/ / ------------------------------------------------ --------------------------
/ / Main (Number of arguments, Value of arguments)
/ / This is the main function.
/ / Return value Success: 0 Error: -1
/ / ------------------------------------------------ --------------------------
int  main ( int  argc, char  ** argv)
{
    //AR.Drone class
    ARDrone ardrone;
 
    //Initialize
    if  (! Ardrone.Open ()) {
        printf ( "Failed to Initialize\n" );
        Return  -1;
    }
 
    //Map
    IplImage * map = cvCreateImage (cvSize (500, 500), IPL_DEPTH_8U, 3);
    cvZero (map);
 
    //Position matrix
    cv :: Mat P = cv :: Mat :: zeros (3, 1, CV_64FC1);
 
    //Main loop
    while  (! GetAsyncKeyState (VK_ESCAPE)) {
        //Update your AR.Drone
        if  (! Ardrone.Update ()) break ;
 
        // Get an image
        IplImage * image = ardrone.getImage ();
 
        // Orientation
        Double  roll Ardrone.GetRoll = ();
        Double  pitch Ardrone.GetPitch = ();
        Double  yaw Ardrone.GetYaw = ();
 
        // Velocity
        Double  vx, vy, VZ;
        Double  velocity = Ardrone.GetVelocity (& vx, vy &, & VZ);
 
        // Battery
        int  Ardrone.GetBatteryPercentage battery = ();
        printf ( "%Ardrone.Battery = D [%%] \n" , battery);
 
        // Rotation matrices
        Double  _RX [] = {1.0, 0.0, 0.0,
                                0.0, cos (roll), - sin (roll),
                                0.0, sin (roll),   cos (roll)};
        Double  _RY [] = { cos (pitch), 0.0,   sin (pitch),
                                0.0, 1.0, 0.0,
                        - sin (pitch), 0.0,   cos (pitch)};
        Double  _RZ [] = {    cos (yaw), - sin (yaw), 0.0,
                           sin (yaw),   cos (yaw), 0.0,
                                0.0, 0.0, 1.0};
        cv :: Mat RX (3, 3, CV_64FC1, _RX);
        cv :: Mat RY (3, 3, CV_64FC1, _RY);
        cv :: Mat RZ (3, 3, CV_64FC1, _RZ);
 
        // Time
        static  Double  last ArdGetTickCount = ();
        Double  dt = (ArdGetTickCount () - last) * 0.001;
        last = ardGetTickCount ();
 
        // Local movement
        Double  _M [] = {vx * dt, vy * dt, VZ * dt};
        cv :: Mat M (3, 1, CV_64FC1, _M);
 
        // Dead reckoning
        P = P + RZ * RY * RX * M;
 
        // Position (x, y, z)
        Double  pos [3] = {P.At < Double > (0,0), P.At < Double > (1,0), P.At < Double > (2,0)};
 
        // Take off / Landing
        if  (KEY_PUSH (VK_SPACE)) {
            if  (Ardrone.OnGround ()) Ardrone.Takeoff ();
            else                     Ardrone.Landing ();
        }
 
        // Move
        Double  x = 0.0, y = 0.0, z = 0.0, r = 0.0;
        if  (KEY_DOWN (VK_UP)) x = 0.5;
        if  (KEY_DOWN (VK_DOWN)) x = -0.5;
        if  (KEY_DOWN (VK_LEFT)) r = 1.0;
        if  (KEY_DOWN (VK_RIGHT)) r = -1.0;
        if  (KEY_DOWN ( 'Q' )) z = 0.5;
        if  (KEY_DOWN ( 'A' )) z = -0.5;
        ardrone.move3D (x, y, z, r);
 
        // Display the image
        cvDrawCircle (map, cvPoint (-pos [1] * 30.0 + map-> width / 2,-pos [0] * 30.0 + map-> height / 2), 2, CV_RGB (255,0,0));
        cvShowImage ( "map" , map);
        CvDrawText (image, CvPoint (20, 20), "x =% 3.2F, 3.2F y =%, z =% 3.2F" , pos [0], pos [1], pos [2]);
        cvShowImage ( "camera" , image);
        cvWaitKey (1);
    }
 
    //See you
    ardrone.close ();
    cvReleaseImage (& map);
 
    Return  0;
}