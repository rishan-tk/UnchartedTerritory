#version 130 
//Fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColour;
in vec2 fragmentUV;

//This is a float vector that gets outputted to the 
//screen for each pixel
out vec4 colour;

//Represents time and variable will change accordingly
uniform float time;


//A variable to hold the texture
uniform sampler2D myTexture;

void main(){

	vec4 textureColour = texture(myTexture, fragmentUV);

	colour = textureColour * fragmentColour;
	
}