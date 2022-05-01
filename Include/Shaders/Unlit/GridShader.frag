R"(
#version 330 core

//-------------------------------------------------------------------------//
// reference for this code:                                                //
// http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/    //
//-------------------------------------------------------------------------//

// input data
in vec3 vertexForNearPanel;
in vec3 vertexForFarPanel;
in mat4 projectionView;

// uniform input
uniform float far;
uniform float near;
uniform float transparency;
uniform int GridAxis = 1; // x: 0, y: 1, z: 2


// output data
out vec4 outColour;


float CalculateDepth(vec3 position3D)
{
    vec4 res = projectionView * vec4(position3D, 1.0);

    return res.z / res.w;
}

float calculateLinearDepth(vec3 pos)
{
    float depth = CalculateDepth(pos);

    float clip_space_depth = ((depth + 1.0) /2.0) * (far-near);

    float transparent = 1.0;

    // grid will become transparent if its depth is over the threshold
    float threshold = (far-near) *  0.995;

    if(clip_space_depth > threshold)
    {
        // calculate the transparent value
        float linearDepth =( clip_space_depth - threshold )/ (far-threshold-near);
        transparent = 1.0- linearDepth;
    }

    return transparent;
}

vec4 GetGrid(float lineWidth, vec2 coordOnPanel, float fading)
{

    vec2 coord = coordOnPanel * (lineWidth);

    vec2 gradient = fwidth(coord);

    // use periodic function to check which pixel is on the grid
    vec2 grid = abs(fract(coordOnPanel * 0.1 - 0.5)-0.5) /gradient; // 0.1 for tile size (size is 1/0.1)

    float line = min(grid.x, grid.y);

    vec4 color = vec4(0.0, 0.0, 0.0, (1.0 - (min(line, 1.0))) * fading);

    return color;
}

void main()
{
    float valueOnGridAxis_NearPanel, valueOnGridAxis_FarPanel;
    vec2 coordOnPanel;

    if(GridAxis == 0) // x
    {
        valueOnGridAxis_NearPanel = vertexForNearPanel.x;
        valueOnGridAxis_FarPanel  = vertexForFarPanel.x;
    }
    else if(GridAxis == 1) // y
    {
        valueOnGridAxis_NearPanel = vertexForNearPanel.y;
        valueOnGridAxis_FarPanel  = vertexForFarPanel.y;
    }
    else // z
    {
        valueOnGridAxis_NearPanel = vertexForNearPanel.z;
        valueOnGridAxis_FarPanel  = vertexForFarPanel.z;
    }

    float t = (-valueOnGridAxis_NearPanel) / (valueOnGridAxis_FarPanel - valueOnGridAxis_NearPanel);

    vec3 panel3DPos = vertexForNearPanel + t * (vertexForFarPanel - vertexForNearPanel);


    if(GridAxis == 0) // x
    {
        coordOnPanel = panel3DPos.yz;
    }
    else if(GridAxis == 1) // y
    {
        coordOnPanel = panel3DPos.xz;
    }
    else // z
    {
        coordOnPanel = panel3DPos.xy;
    }

    // only when t is bigger than 0, the grid is inside the frustum
    if(t>0.00001)
    {

        // coefficient for fading effect
        float fading = max(0, (calculateLinearDepth(panel3DPos)));

        // calculate the color for this pixel
        outColour = GetGrid(0.1, coordOnPanel, fading * transparency);
    }
    else
    {
        outColour = vec4(0.0, 0.0, 0.0, 0.0);
    }


    // update depth of the pixel
    gl_FragDepth = CalculateDepth(panel3DPos);

}

)"