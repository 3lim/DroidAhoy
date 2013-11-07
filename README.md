# Android A'hoy
Fluid simulation based survival game.

# Setup
I'm a bit amazed at how easy it was. Follow these steps:

0. Get git & cmake

    
    sudo apt-get intall git
    sudo apt-get install cmake
    

1. Clone glfw to a local folder

    
    git clone https://github.com/glfw/glfw glfw3

2. CARE: Install required libraries
> Take special care at this step since it is possible to break the GUI
> when installing dev packages. I recommend running the second command
> to ensure that you still have the required GUI packages.

    
    sudo apt-get install cmake xorg-dev libglu1-mesa-dev
    sudo apt-get install ubuntu-desktop xorg

3. Build glfw for Unix
   
     
    cmake -G "Unix Makefiles"

4. Make glfw
   
     
    make

5. Install :D
   
         
    sudo make install
    
6. Now get the latest branch of the project and build it
    
      
    make


7. Run the demo!
    
    
    /run_demo

