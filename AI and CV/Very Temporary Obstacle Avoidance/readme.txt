http://digestingduck.blogspot.com/2011/02/very-temporary-obstacle-avoidance.html

https://www.youtube.com/watch?v=SE0nE8wYCrc#t=21

Descrption of the files:

--------------- game.py -----------------------

original simulation using pygame 2D Game Library.

http://www.pygame.org/news.html

--------------- test2.py ----------------------
--------------- game.cpp ----------------------


These files were used during the process of translating the code from Python to C++. I tried to maintain the same name functions
and variables for simplicity but due to the different nature of the languages, there are some differences.

--------------- main.cpp ---------------------- 

Obstacle Avoidance with Lidar integration. In order to run the code, you have to download the RPLidar SDK.

http://rplidar.robopeak.com/download.html

Once you download it, import the sdk_and_demo.sln, which includes frame_grabber, simple_grabber, ultra_simple, rplidar_driver. Once everything
is up and running, replace the main.cpp in ultra_simple with my main.cpp. Given a goal(point) the algorithm takes all the points from the Lidar
and returns where you should head to as a direction vector.