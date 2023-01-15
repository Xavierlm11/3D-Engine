# 3D-Engine

https://github.com/Xavierlm11/Omega-Engine

The goal of this project is to make a game engine for learning popuses

We hope you like it! :)

There are 3 modes of rendering: Normal, with checkers view and Wireframe.
Also, there are other settings in the view like fog.

The meshes are grouped in a game object system with parents and children.
Finally, if the file has attached a texture, the engine will automatically set a meterial with it in the game object.

We are:
Xavier Casadó - https://github.com/Akage369
Xavier López - https://github.com/Xavierlm11

[Controls]
The camera uses WS to move front and backwards, AD to move laterally and EC to move up and down.
Also, you can scroll the mouse wheel to zoom in or zoom out.
Right click to rotate the camera and Alt + left click to rotate around the selecetd game object.
With F the camera is focused on the selected GameObject.

[Performance]
When initializing the engine, all the assets in the Assets folder will be serialized into custom format files ".chad". 
These can be located at Library/Meshes and Library/Materials folders, which are automatically generated too.

These files contain all the necessary info of the assets and are needed for the engine to work, so it's important not to remove them manually.
Don't worry, they will be removed when closing the engine.
You can also drag and drop files from any directory of your computer to the engine. These will be added to the Assets folder and a new .chad file will be generated.
We support .png, .jpg & .dds files for textures and .fbx for models.

[Features]
Omega engine is capable of creating GameObjects from the assets in the Assets windows via Drag & Drop. 
If you click in the asset, you can see more information in the inspector. 
You can drop it in the Viewport or in the hierarchy, inside the root object or any other that will become its parent.
If the asset is a material, you can drop it into an existing GameObject to set it up as its texture.
Also, if the GameObject has already attached a relative path of a texture, and this is in Assets folder, the GameObject will load when being created.

You can select a GameObject clicking it in the hierarchy. Then, you can view its properties in the Inspector and modify his transform component.

There is also a camera viewport which FOV and Near Plane Distance you can change.


<iframe width="560" height="315" src="https://www.youtube.com/embed/4J9V1xe50Mg" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>





