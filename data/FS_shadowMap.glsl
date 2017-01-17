#version 330


// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out float  out_fragDepth;

void main()
{
    // Not really needed, OpenGL does it anyway
//    out_fragDepth =  gl_FragCoord.z;
}
