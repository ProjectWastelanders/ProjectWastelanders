#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 normals;
	layout (location = 2) in vec2 textCoords;
	
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	
	uniform vec2 texOffset1;
	uniform vec2 texOffset2;
	uniform vec2 texInfo;
	
	out vec2 TextureCoords1;
	out vec2 TextureCoords2;
	out float blend;

	void main()
	{
	
		vec2 textcoords = textCoords;
		textcoords /= texInfo.x;
		TextureCoords1 = textcoords + texOffset1;
		TextureCoords2 = textcoords + texOffset2;
		blend = texInfo.y;
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
