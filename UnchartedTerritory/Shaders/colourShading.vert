#version 130

//Input data from the vbo
in vec2 vertexPosition;
in vec4 vertexColour;
in vec2 vertexUV;

out vec4 fragmentColour;
out vec2 fragmentPosition;
out vec2 fragmentUV;

void main(){
	//Set the x, y position on the screen
	gl_Position.xy = vertexPosition;
	
	//Set z to 0 since 2D
	gl_Position.z = 0.0;
	
	//Indicate that the coordinates are normalized
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;

	fragmentColour = vertexColour;

	//Flip the Y to make the image appear normal
	fragmentUV = vec2(vertexUV.x, 1 - vertexUV.y);
}



