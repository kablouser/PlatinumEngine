R"(
#version 330 core

//-------------------------------------------------------------------------//
// reference for this code:                                                //
// http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/    //
//-------------------------------------------------------------------------//

// input data
layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTextureCoordinate;

// output data
out vec3 vertexForNearPanel;
out vec3 vertexForFarPanel;
out mat4 projectionView;

// matrices
uniform mat4 view;
uniform mat4 projection;


// function for unproject a vertex into 3D space
vec3 UnProject(vec3 vertexInNDC)
{
    vec4 res = inverse(projectionView) * vec4(vertexInNDC, 1.0);

    return res.xyz / res.w;
}



void main()
{
    //* unproject the vertices on the near face and the far face of the normalized cube space
    //* to find a panel that can cover the whole camera vision

    // get matrix for inversed transformation
    projectionView = projection * view;

    // get the unprojected vertices in world coordinate, output to fragment shader
    vertexForNearPanel = UnProject(vec3(inVertex.x, inVertex.y, 0.0));
    vertexForFarPanel  = UnProject(vec3(inVertex.x, inVertex.y, 1.0));

    // update vertex position
    gl_Position = vec4(inVertex, 1.0);
}
)"