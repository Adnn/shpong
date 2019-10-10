#pragma once

namespace ad {

static const GLchar* gVertexShader = R"#(
    #version 400

    layout(location=0) in vec4 in_VertexPosition;
    layout(location=1) in ivec2 in_UV;
    layout(location=2) in vec4 in_InstanceRect;
    layout(location=3) in vec3 in_InstanceColor;

    uniform ivec2 in_BufferResolution;

    out vec3 ex_Color;

    void main(void)
    {
        vec2 bufferSpacePosition = in_InstanceRect.xy
                                   + in_VertexPosition.xy * in_InstanceRect.zw;
        gl_Position = vec4(2 * bufferSpacePosition / in_BufferResolution - vec2(1.0, 1.0),
                           0.0, 1.0);
        ex_Color = in_InstanceColor;
    }
)#";


static const GLchar* gFragmentShader = R"#(
    #version 400

    in vec3 ex_Color;
    out vec4 out_Color;

    void main(void)
    {
        out_Color = vec4(ex_Color, 1.0);
    }
)#";

} // namespace ad
