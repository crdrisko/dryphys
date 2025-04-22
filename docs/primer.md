# Primer

## The PartitionEngine::Window()

1.  Our first step is to provide access to the PartitionEngine library. The files are wrapped in a convient header file that provides this access. Similarly to avoid long namespace calls we can declare our usage at the top of our `.cpp` file.

    ```C++
    #include <partitionEngine/partitionEngine.hpp>

    using namespace PartitionEngine;
    ```

2. Next, we can create/initialize a `Window(...)` object. This encapsulates a `GFLWwindow` and initializes OpenGL and Glad. If using ImGui, that will also be initialized.

    ```C++
    #include <partitionEngine/partitionEngine.hpp>

    using namespace PartitionEngine;

    int main()
    {
        Window window {800, 600, "PartitionEngine Demo"};
    }
    ```

    *Note:* If we wanted to defer window creation, we can just call the default Window constructor and call the `create(...)` method later on.

3. To open and maintain a `Window(...)` object we use the following loop with four distinct functions.

    - `isOpen()`: determines wether or not the function should continue to be rendered, querying the `glfwWindowShouldClose()` function.
    - `pollEvents()`: handles the glfw events and ImGui frame info.
    - `clear()`: clears the current buffer with whatever color is passed to it, default is black.
    - `display()`: swaps the buffers and renders the ImGui state (if needed).

    ```C++
    #include <partitionEngine/partitionEngine.hpp>

    using namespace PartitionEngine;
    
    int main()
    {
        Window window {800, 600, "PartitionEngine Demo"};

        while (window.isOpen())
        {
            window.pollEvents();

            window.clear(/* ColorRGBA {51, 76, 76, 255} */);

            window.display();
        }
    }
    ```

    *Note:* Try uncommenting the clear color if you're following along with [LearnOpenGL](https://learnopengl.com/).

## The PartitionEngine::Engine()


