#version 330

// ping pong inputs
uniform sampler2DRect particles0;
uniform sampler2DRect particles1;
uniform sampler2DRect texStartPos;

uniform vec3 attractor;
uniform vec3 gravity;
uniform float radiusSquared;
uniform float elapsed;
uniform float attFrc;
uniform float homing;
uniform float dampening;

in vec2 texCoordVarying;

layout(location = 0) out vec4 posOut;
layout(location = 1) out vec4 velOut;

vec3 getAttractionForce(in float _distSq, in float _rSq, in vec3 _dir){
    float mag = attFrc * (1.0-_distSq/_rSq);
    vec3 frc = step(_distSq, _rSq)*mag*_dir;
    return frc;
}

void applyGravity(inout vec3 _v){_v+=gravity;}

void accelerate(inout vec3 _vel, in vec3 _frc){_vel+=elapsed*_frc;}
void move(inout vec3 _pos, inout vec3 _vel){_pos+=elapsed*_vel;}

bool updateAttractor(in vec3 _posAttractor, in vec3 _pos, float _rSq, out vec3 _frc){
    vec3 diff = _posAttractor - _pos;
    float distSq = dot(diff, diff);
    if(distSq<_rSq){
        _frc = getAttractionForce(distSq, _rSq, normalize(diff));
        applyGravity(_frc);
        return true;
    }else{
        return false;
    }
}

void main()
{
    vec3 pos = texture(particles0, texCoordVarying.st).xyz;
    vec3 vel = texture(particles1, texCoordVarying.st).xyz;
    
    vec3 force;
    if(!updateAttractor(attractor, pos.xyz, radiusSquared, force)){
        vec3 posHome = texture(texStartPos, texCoordVarying.st).xyz;
        vec3 diff = posHome - pos;
        vec3 dir = normalize(diff);
        force = dir*homing;
    }
    
    accelerate(vel, force);
    vel *= dampening;
    move(pos, vel);
    
    posOut = vec4(pos, 1.0);
    velOut = vec4(vel, 0.0);
}