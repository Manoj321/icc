icc
===

Infinite Coffee Cup

Published paper -> http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=6757120&contentType=Conference+Publications

Abstract:


Imagine you go to your favorite café house or a restaurant and the system recognizes your presence 
and alerts the waiter and caters to your favorite beverage you longed for the day. 
The main motivation to Infinite Coffee Cup is to detect the presence of a person and dynamically 
and non-Invasively estimate the amount of coffee in a circular cup on the table. 
One of the more popular devices used is Microsoft Kinect, which has cameras that capture
both RGB and Depth data. Kinect is a low cost sensing device which provides two streams of images, 
8 bit 3 channel RGB image and 11 bit single channel depth image. The system is fixed at the roof facing 
downwards and it takes a top view of the cup placed on the table. We use "libfreenect" drivers which is an 
open source project, to stream data from kinect. We employ various image processing algorithms using
Open CV (Open Source Computer Vision) which is a library of programming functions for real time computer vision. 
The algorithms are used to manipulate the data from kinect to, -Detect the circular coffee cups in the RGB image.
-Overlay and correlate the depth values of cups in the depth image to get the height and amount of coffee in the cup.
