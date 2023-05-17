#version 410 core
#ifdef VERTEX_PROGRAM
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in mat4 model;
    layout (location = 5) in vec3 normals;
    layout (location = 6) in vec2 texCoords;
    layout (location = 7) in vec4 textOffsets;
    layout (location = 8) in vec2 textinfos;

    uniform mat4 view;
    uniform mat4 projection;

    out vec2 TextureCoords1;
    out vec2 TextureCoords2;
    out float blend;

    void main()
    {

        vec2 textcoords = texCoords;
        textcoords /= textinfos.x;
        TextureCoords1 = textcoords + textOffsets.xy;
        TextureCoords2 = textcoords + textOffsets.zw;
        blend = textinfos.y;
        gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);

    }
#endif
#ifdef FRAGMENT_PROGRAM
    uniform sampler2D texture_albedo;

    in vec2 TextureCoords1;
    in vec2 TextureCoords2;
    in float blend;
    out vec4 FragColor;
    void main()
    {
        vec4 texture1 = texture(texture_albedo,TextureCoords1);
        vec4 texture2 = texture(texture_albedo,TextureCoords2);
        FragColor = mix(texture1,texture2,blend);
    }
#endif