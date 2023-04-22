VARYING vec4 vCustomData;
VARYING vec3 vGlobalPosition;
VARYING vec3 vNormal;

void MAIN()
{
    METALNESS = 0.0;
    ROUGHNESS = 1.0;
    FRESNEL_POWER = 5.0;

    vec3 n = abs(vNormal.xyz) + max(vNormal.zxy, 0.0);

    bool roof = n.x > 0 && n.z > 0;
    bool bottom = n.y > 0 && n.z > 0;
    bool right = n.x > 0 && n.y > 0;
    bool top = !bottom && !right && n.y > 0;
    bool left = !right && !roof && n.x > 0;
    bool floor = !roof && !bottom && n.z > 0;

    // gradient on roof, floor and current
    float c = 1.0 - 0.25 * (UV0.x*UV0.x + UV0.y*UV0.y);

    if (roof) {
	bool isVisited = false;
	bool isCurrent = false;
	if(vCustomData.x > 0.3) {
	    // 0.32 -> both true
	    isCurrent = true;
	    isVisited = true;
	} else if(vCustomData.x > 0.2) {
	    // 0.21 -> current
	    isCurrent = true;
	} else if(vCustomData.x > 0.1) {
	    // 0.11 -> visited
	    isVisited = true;
	} // else both false

	if(isCurrent == true) {
	    BASE_COLOR = vec4(0, c, 0, 1.0);
	}
	else if(isVisited == true) {
	    discard;
	} else {
	    BASE_COLOR = vec4(0, 0, c, 1.0);
	}
    } else if(left || top) {
	bool isLeft = false;
	bool isTop = false;
	if(vCustomData.y > 0.3) {
	    // 0.32 -> both true
	    isLeft = true;
	    isTop = true;
	} else if(vCustomData.y > 0.2) {
	    // 0.21 -> top
	    isTop = true;
	} else if(vCustomData.y > 0.1) {
	    // 0.11 -> left
	    isLeft = true;
	} // else both false
	if(left && !isLeft) {
	    discard;
	} else if(top && !isTop)
	    discard;
	else
	    BASE_COLOR = vec4(1.0);
    } else if(right) {
	if(vCustomData.z > 0) {
	    BASE_COLOR = vec4(1.0);
	} else {
	    discard;
	}
    } else if(bottom) {
	if(vCustomData.w > 0) {
	    BASE_COLOR = vec4(1.0);
	} else {
	    discard;
	}
    }
    else {
	BASE_COLOR = vec4(c, c, 0, 1.0);
    }
}
