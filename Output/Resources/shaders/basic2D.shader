#version 410 core
#ifdef VERTEX_PROGRAM
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 textCoords;

uniform mat4 model;

out vec2 TextureCoords;

void main()
{
    gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    TextureCoords = textCoords;
}
#endif
#ifdef FRAGMENT_PROGRAM
out vec4 FragColor;

	in vec2 TextureCoords;
	uniform vec2 normalizedPosition;
	uniform vec2 normalizedSize;
	uniform float limit;
	uniform int opacityDir;
	uniform float totalOpacity; // Opacity applied on non-ignored fragments.

	uniform sampler2D diffuseTexture;

	void main()
	{
		float threshold = 1.0;
		switch (opacityDir)
		{
			case 0: // LEFT_TO_RIGHT
				threshold = (normalizedPosition.x + normalizedSize.x) - (limit * normalizedSize.x);
				if (gl_FragCoord.x < threshold)
				{
					FragColor = vec4(1.0,0.0,0.0,0.0);
					return;
				}
				break;
			case 1: // RIGHT_TO_LEFT
				threshold = normalizedPosition.x + (limit * normalizedSize.x);
				if (gl_FragCoord.x > threshold)
				{
					FragColor = vec4(1.0,0.0,0.0,0.0);
					return;
				}
				break;
			case 2: // UP_TO_DOWN
				threshold = (normalizedPosition.y + normalizedSize.y) - (limit * normalizedSize.y);
				if (gl_FragCoord.y < threshold)
				{
					FragColor = vec4(1.0,0.0,0.0,0.0);
					return;
				}
				break;
			case 3: // DOWN_TO_UP
				threshold = normalizedPosition.y + (limit * normalizedSize.y);
				if (gl_FragCoord.y > threshold)
				{
					FragColor = vec4(1.0,0.0,0.0,0.0);
					return;
				}
				break;
		}
		FragColor = texture(diffuseTexture, TextureCoords);
		FragColor.a *= totalOpacity;
	}
#endif