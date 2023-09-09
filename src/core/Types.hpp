#pragma once

namespace shed {
    class vec2 {
        public:
            vec2() {};
            vec2(float _x, float _y) : 
            x(_x),
            y(_y) {};
            float x;
            float y;
    };

    class vec3 {
        public:
            vec3() {};
            vec3(float _x, float _y, float _z) : 
            x(_x),
            y(_y),
            z(_z) {};
            float x;
            float y;
            float z;
    };

    class vec4 {
        public:
            vec4() {};
            vec4(float _x, float _y, float _z, float _w) : 
            x(_x),
            y(_y),
            z(_z),
            w(_w) {};
            float x;
            float y;
            float z;
            float w;
    };
}