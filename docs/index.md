# 3D-Engine

https://github.com/Xavierlm11/Omega-Engine

The goal of this project is to make a game engine for learning popuses
We hope you like it! :)

We are:
Xavier Casadó - https://github.com/Akage369
Xavier López - https://github.com/Xavierlm11

Website: https://xavierlm11.github.io/Omega-Engine/

## Description
There are 3 modes of rendering: Normal, with checkers view and Wireframe.
Also, there are other settings in the view like fog.

The meshes are grouped in a game object system with parents and children.
Finally, if the file has attached a texture, the engine will automatically set a meterial with it in the game object.

Omega Engine has an Assets Manager to set up the scene to your liking.

Our last feature is Physics! You can add the Physics component to the GameOjects and add them a collider, static or not. Adjust it to fit the form which you like and press the Play button to see all the magic happening. If you press Pause, the game will stop and you can resume clicking again.

You can also add cosntraints and play with a vehicle, pushing the houses out of the scene, for instance.
The Camera can shoot balls too.

When you have set up the scene correctly, you can save it and the next time you open the engine, it will be restored, so don not worry about that.


## Controls
The camera uses WS to move front and backwards, AD to move laterally and EC to move up and down.
Also, you can scroll the mouse wheel to zoom in or zoom out.
Right click to rotate the camera and Alt + left click to rotate around the selecetd game object.
With F the camera is focused on the selected GameObject.

Camera shoots with "1" key.
You can disable the colliders wireframe lines in the Game viewport with "F2" key.
Car moves with arrow keys.

## Performance
When initializing the engine, all the assets in the Assets folder will be serialized into custom format files ".chad". 
These can be located at Library/Meshes and Library/Materials folders, which are automatically generated too.

These files contain all the necessary info of the assets and are needed for the engine to work, so it's important not to remove them manually.
Don't worry, they will be removed when closing the engine.
You can also drag and drop files from any directory of your computer to the engine. These will be added to the Assets folder and a new .chad file will be generated.
We support .png, .jpg & .dds files for textures and .fbx for models.

## Features
Omega engine is capable of creating GameObjects from the assets in the Assets windows via Drag & Drop. 
If you click in the asset, you can see more information in the inspector. 
You can drop it in the Viewport or in the hierarchy, inside the root object or any other that will become its parent.
If the asset is a material, you can drop it into an existing GameObject to set it up as its texture.
Also, if the GameObject has already attached a relative path of a texture, and this is in Assets folder, the GameObject will load when being created.

You can select a GameObject clicking it in the hierarchy. Then, you can view its properties in the Inspector and modify his transform component.
You can also add new components there like the physics one. Then, you can select if it is static and choose a collider for them, adjust the form and set a constraint with another.

There is also a camera viewport which FOV and Near Plane Distance you can change.

## About Us
### Xavier Casadó Benítez
[Github](https://github.com/Akage369)
![xavi2](https://user-images.githubusercontent.com/79161102/212573462-64a9d3f8-461f-44fd-b158-d1f07d52e0bc.jpg)


### Xavier López Martín
[Github](https://github.com/Xavierlm11)
![xavi1](https://user-images.githubusercontent.com/79161102/212573490-56890018-f2d4-488a-b05e-82755a155a55.jpeg)


<iframe width="560" height="315" src="https://www.youtube.com/embed/4J9V1xe50Mg" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>





