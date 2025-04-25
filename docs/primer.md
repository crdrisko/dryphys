# Primer

## DryPhys

...

## PartitionEngine

### `PartitionEngine::Window`

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

---

### `PartitionEngine::Shader` and `PartitionEngine::ShaderProgram`

Since `PartitionEngine` is a wrapper around OpenGL and GLFW, we need a way to represent shaders. We use the `Shader` class for individual shaders and the `ShaderProgram` class for, you guessed it, shader programs. 

For shaders, we can create them explictly by passing a file and the OpenGL type:

```C++
Shader vertexShader {"path/to/vertexShader.vs", GL_VERTEX_SHADER};
Shader fragmentShader {"path/to/fragmentShader.fs", GL_FRAGMENT_SHADER};
```

The constructor handles all aspects of shader creation, from file parsing to creation to compilation. In the future we can see adding precomputed shaders and other stream inputs but for now shaders are only passed in via files.

Now that we have our two shaders, we can create a shader program from them:

```C++
ShaderProgram shaderProgram {vertexShader, fragmentShader};
```

Shaders can also be created in the constructor of the ShaderProgram as temporary rvalues and initialization of the program will proceed as expected:

```C++
ShaderProgram shaderProgram {
        Shader {"path/to/vertexShader.vs", GL_VERTEX_SHADER}, 
        Shader {"path/to/fragmentShader.fs", GL_FRAGMENT_SHADER}};
```

Construction of a `ShaderProgram` consists of three stages: creation, attachment, and linking. After this point, you will have a shader program ready to use and attach any uniforms to. Uniform attachment is simplified with a couple of templated functions. Let's look at an example.

```C++
float timeValue  = glfwGetTime();
float greenValue = std::sin(timeValue) / 2.0f + 0.5f;

shaderProgram.use();
shaderProgram.setUniform("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
```

If this looks familiar that's likely because you've seen this shader program in [LearnOpenGL](https://learnopengl.com/). Here though, we've used our own wrapper and templated our `setUniform()` function to accept a variable number of input variables. In this example we're passing four float values to the uniform named "ourColor". At compile-time, the function `glUniform4f()` is initialized with the ShaderProgram's index, the passed name, and any parameters (4 floats in this case).

We can do something similar for matrix types:

```C++
DryPhys::Matrix4D projection {0.00250, 0,    0, -1, 
                                 0, 0.00333, 0, -1, 
                                 0,    0,   -1,  0, 
                                 0,    0,    0,  1};

shaderProgram.use();
shaderProgram.setUniformMatrix("projection", projection);
```

In the same way as before, the first parameter to `setUniformMatrix()` is the name of the uniform and the second term is the matrix. Currently, shader programs in `PartitionEngine` can create uniforms for `DryPhys::Matrix3D`'s, `DryPhys::Matrix4D`'s, and `DryPhys::Transform4D`'s. Since these matrices are the transpose of the type OpenGL takes, a secondary transformation is performed internally before the uniform is passed to OpenGL.

---

### `PartitionEngine::Engine`

...
