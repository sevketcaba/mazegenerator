VARYING vec3 vGlobalPosition;
VARYING vec3 vNormal;
void MAIN()
{
    vNormal = NORMAL;

    vec4 pos = MODEL_MATRIX * vec4(VERTEX, 1.0);
    vGlobalPosition = pos.xyz;
}
