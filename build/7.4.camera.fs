#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 ourAPos;

uniform float normalizedScale;
float red = 0.0;
float green = 0.0;
float blue = 0.0;

void main()
{

	if(normalizedScale >= (4.5f/6.0f)){
	    red = 1.0;
	}else if(normalizedScale >= (3.5/6.0f) &&normalizedScale < (4.5/6.0f)){
	    red = normalizedScale*6 -3.5;
	}
	if(normalizedScale < (1/6.0f)){
	    blue = normalizedScale*6+0.1f;
	}else if(normalizedScale >= (1/6.0f) && normalizedScale < (1/3.0f)){
	    blue = 1.0;
	}else if(normalizedScale >= (2.5/6.0f) && normalizedScale < (3.5/6.0f)){
	    blue = normalizedScale*(-6) + 3.5;
	}
	if(normalizedScale >= (1.5/6.0f) && normalizedScale < (2.5/6.0f)){
	    green = normalizedScale*6 - 1.5;
	}else if(normalizedScale >= (2.5/6.0f) && normalizedScale < (4.5/6.0f)){
	    green = 1.0;
	}else if(normalizedScale >= (4.5/6.0f) && normalizedScale < (5.5/6.0f)){
	    green = normalizedScale*(-6) + 5.5;
	}

	FragColor = vec4(red, green, blue, 1.0f);
	// FragColor = ourColor;
}