#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 normals;
	layout (location = 2) in vec2 textCoords;

	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;

	out vec2 TextureCoords;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		TextureCoords = textCoords;
	}
#endif
#ifdef FRAGMENT_PROGRAM
	out vec4 FragColor;

	in vec2 TextureCoords;

	uniform sampler2D diffuseTexture;

	void main()
	{
		FragColor = texture(diffuseTexture, TextureCoords);
	}
#endif