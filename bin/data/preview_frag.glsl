#version 110
uniform sampler2DRect video;
uniform sampler2DRect texture;
uniform vec2 video_size;
uniform vec2 texture_size;
uniform vec2 render_size;

void main()
{
    vec2 uv = vec2(gl_FragCoord.x, render_size.y - gl_FragCoord.y);
    vec2 vuv = vec2(uv.s * video_size.x / render_size.x, uv.t * video_size.y / render_size.y);
    vec4 v = texture2DRect(video, vuv);

    float x = v.r * 0.2989 + v.g * 0.5870 + v.b * 0.1140;
    x = x * x + 0.35 * (render_size.x - uv.s)/render_size.x;
    //x = v.r;
    gl_FragColor = vec4(x * 0.5, x, x * 0.5, 1.0);
}