#version 410 core
#ifdef VERTEX_PROGRAM
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in mat4 model;
    layout (location = 5) in vec3 normals;
    layout (location = 6) in vec2 texCoords;
    layout (location = 7) in vec4 textOffsets;
    layout (location = 8) in vec2 textinfos;
    layout (location = 9) in float texID;

    uniform mat4 view;
    uniform mat4 projection;

    out vec2 TextureCoords1;
    out vec2 TextureCoords2;
    out float blend;
    out float TextureID;

    void main()
    {

        vec2 textcoords = texCoords;
        textcoords /= textinfos.x;
        TextureCoords1 = textcoords + textOffsets.xy;
        TextureCoords2 = textcoords + textOffsets.zw;
        blend = textinfos.y;
        TextureID = texID;
        gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);

    }
#endif
#ifdef FRAGMENT_PROGRAM
    uniform sampler2D textures[32];

    in vec2 TextureCoords1;
    in vec2 TextureCoords2;
    in float blend;
    in float TextureID;
    out vec4 FragColor;
    void main()
    {
        vec4 texture1 = texture(textures[int(round(TextureID))],TextureCoords1);
        vec4 texture2 = texture(textures[int(round(TextureID))],TextureCoords2);
        FragColor = mix(texture1,texture2,blend);
    }
#endif
