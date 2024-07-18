/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file main.cpp
 * \brief This is a demo which test the Skia shader
 */

#include <include/renderer/vSurface.h>
#include <glfw/glfw3.h>
#include <thread>
#include <chrono>

sk_sp<VRenderInterface> GLInterface;
sk_sp<SkRuntimeEffect> Shader;

int WIDTH  = 640;
int HEIGHT = 480;

time_t StartTime;

GLFWwindow *GLWindow;

const char *SkSLCode1 = R"(
// Source: @notargs https://twitter.com/notargs/status/1250468645030858753
uniform float3 iResolution;
uniform float iTime;
uniform float4 iMouse;

#define fs(i) (fract(sin((i)*114.514)*1919.810))
#define lofi(i,j) (floor((i)/(j))*(j))

const float PI=acos(-1.);

// CAVE 09
// ENTER THE CAVE
//
// DJ: Pinieon


float time;
float seed;

mat2 r2d(float t){
  return mat2(cos(t),sin(t),-sin(t),cos(t));
}

mat3 orthBas(vec3 z){
  z=normalize(z);
  vec3 up=abs(z.y)>.999?vec3(0,0,1):vec3(0,1,0);
  vec3 x=normalize(cross(up,z));
  return mat3(x,cross(z,x),z);
}

float random(){
  seed++;
  return fs(seed);
}

vec3 uniformLambert(vec3 n){
  float p=PI*2.*random();
  float cost=sqrt(random());
  float sint=sqrt(1.0-cost*cost);
  return orthBas(n)*vec3(cos(p)*sint,sin(p)*sint,cost);
}


vec4 tbox(vec3 ro,vec3 rd,vec3 s){
  vec3 or=ro/rd;
  vec3 pl=abs(s/rd);
  vec3 f=-or-pl;
  vec3 b=-or+pl;
  float fl=max(f.x,max(f.y,f.z));
  float bl=min(b.x,min(b.y,b.z));
  if(bl<fl||fl<0.){return vec4(1E2);}
  vec3 n=-sign(rd)*step(f.yzx,f.xyz)*step(f.zxy,f.xyz);
  return vec4(n,fl);
}

struct QTR {
  vec3 cell;
  vec3 pos;
  float len;
  float size;
  bool hole;
};

bool isHole(vec3 p){
  if(abs(p.x)<.5&&abs(p.y)<.5){return true;}
  float dice=fs(dot(p,vec3(-2,-5,7)));
  if(dice<.3){return true;}
  return false;
}

QTR qt(vec3 ro,vec3 rd){
  vec3 haha=lofi(ro+rd*1E-2,.5);
  float ha=fs(dot(haha,vec3(6,2,0)));
  ha=smoothstep(-0.2,0.2,sin(0.5*time+PI*2.*(ha-.5)));

  ro.z+=ha;

  QTR r;
  r.size=1.;
  for(int i=0;i<4;i++){
    r.size/=2.;
    r.cell=lofi(ro+rd*1E-2*r.size,r.size)+r.size/2.;
    if(isHole(r.cell)){break;}
    float dice=fs(dot(r.cell,vec3(5,6,7)));
    if(dice>r.size){break;}
  }

  vec3 or=(ro-r.cell)/rd;
  vec3 pl=abs(r.size/2./rd);
  vec3 b=-or+pl;
  r.len=min(b.x,min(b.y,b.z));

  r.pos=r.cell-vec3(0,0,ha);
  r.hole=isHole(r.cell);

  return r;
}


vec4 main( in vec2 fragCoord )
{
  vec2 uv=fragCoord.xy/iResolution.xy;
  vec2 p=uv*2.-1.;
  p.x*=iResolution.x/iResolution.y;

  time=iTime;

  seed=length(p);
  seed+=fract(time);

  float haha=time*62.0/60.0;
  float haha2=floor(haha)-.2*exp(-fract(haha));

  p=r2d(time*.2+.2*floor(haha))*p;

  vec3 ro0=vec3(0,0,1);
  ro0.z-=haha2;
  ro0+=.02*vec3(sin(time*1.36),sin(time*1.78),0);

  vec3 rd0=normalize(vec3(p,-1.));

  vec3 ro=ro0;
  vec3 rd=rd0;
  vec3 fp=ro+rd*2.;
  ro+=vec3(0.04*vec2(random(),random())*mat2(1,1,-1,1),0);
  rd=normalize(fp-ro);

  float rl=.01;
  vec3 rp=ro+rd*rl;

  vec3 col=vec3(0);
  vec3 colRem=vec3(1);
  float samples=1.;

  for(int i=0;i<200;i++){
    QTR qtr=qt(rp,rd);

    vec4 isect;
    if(qtr.hole){
      isect=vec4(1E2);
    }else{
      float size=qtr.size*.5;
      size-=.01;
      size-=.02*(.5+.5*sin(5.0*time+15.0*qtr.cell.z));
      isect=tbox(rp-qtr.pos,rd,vec3(size));
    }

    if(isect.w<1E2){
      float fog=exp(-.2*rl);
      colRem*=fog;

      rl+=isect.w;
      rp=ro+rd*rl;

      vec3 mtl=fs(cross(qtr.cell,vec3(4,8,1)));

      vec3 n=isect.xyz;

      if(mtl.x<.1){
        col+=colRem*vec3(10,1,1);
        colRem*=0.;
      }else if(mtl.x<.2){
        col+=colRem*vec3(6,8,11);
        colRem*=0.;
      }else{
        colRem*=0.3;
      }

      ro=ro+rd*rl;
      rd=mix(uniformLambert(n),reflect(rd,n),pow(random(),.3));
      rl=.01;
    } else{
      rl+=qtr.len;
      rp=ro+rd*rl;
    }

    if(colRem.x<.01){
      ro=ro0;
      rd=rd0;
      vec3 fp=ro+rd*2.;
      ro+=vec3(0.04*vec2(random(),random())*mat2(1,1,-1,1),0);
      rd=normalize(fp-ro);
      rl=.01;
      rp=ro+rd*rl;
      colRem=vec3(1);
      samples++;
    }
  }

  col=pow(col/samples,vec3(.4545));
  col*=1.0-0.4*length(p);
  col=vec3(
    smoothstep(.1,.9,col.x),
    smoothstep(.0,1.0,col.y),
    smoothstep(-.1,1.1,col.z)
  );


  return vec4(col, 1.);
}
)";
const char *SkSLCode2 = R"(
uniform float3 iResolution;
uniform float iTime;
uniform float4 iMouse;

int hexid;
vec3 hpos, point, pt;
float tcol, bcol, hitbol, hexpos, fparam=0.;

mat2 rot(float a) {
    float s=sin(a),c=cos(a);
    return mat2(c,s,-s,c);
}

vec3 path(float t) {
    return vec3(sin(t*.3+cos(t*.2)*.5)*4.,cos(t*.2)*3.,t);
}

float hexagon( in vec2 p, in float r )
{
    const vec3 k = vec3(-0.866025404,0.5,0.577350269);
    p = abs(p);
    p -= 2.0*min(dot(k.xy,p),0.0)*k.xy;
    p -= vec2(clamp(p.x, -k.z*r, k.z*r), r);
    return length(p)*sign(p.y);
}

float hex(vec2 p) {
    p.x *= 0.57735*2.0;
	p.y+=mod(floor(p.x),2.0)*0.5;
	p=abs((mod(p,1.0)-0.5));
	return abs(max(p.x*1.5 + p.y, p.y*2.0) - 1.0);
}

mat3 lookat(vec3 dir) {
    vec3 up=vec3(0.,1.,0.);
    vec3 rt=normalize(cross(dir,up));
    return mat3(rt, cross(rt,dir), dir);
}

float hash12(vec2 p)
{
	p*=1000.;
	vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

float de(vec3 p) {
    pt=vec3(p.xy-path(p.z).xy,p.z);
    float h=abs(hexagon(pt.xy,3.+fparam));
    hexpos=hex(pt.yz);
    tcol=smoothstep(.0,.15,hexpos);
    h-=tcol*.1;
    vec3 pp=p-hpos;
    pp=lookat(point)*pp;
    pp.y-=abs(sin(iTime))*3.+(fparam-(2.-fparam));
    pp.yz*=rot(-iTime);
    float bola=length(pp)-1.;
    bcol=smoothstep(0.,.5,hex(pp.xy*3.));
    bola-=bcol*.1;
    vec3 pr=p;
    pr.z=mod(p.z,6.)-3.;
    float d=min(h,bola);
    if (d==bola) {
        tcol=1.;
        hitbol=1.;
    }
    else {
        hitbol=0.;
        bcol=1.;
    }
    return d*.5;
}

vec3 normal(vec3 p) {
    vec2 e=vec2(0.,.005);
    return normalize(vec3(de(p+e.yxx),de(p+e.xyx),de(p+e.xxy))-de(p));
}

vec3 march(vec3 from, vec3 dir) {
    vec3 odir=dir;
    vec3 p=from,col=vec3(0.);
    float d,td=0.;
    vec3 g=vec3(0.);
    for (int i=0; i<200; i++) {
        d=de(p);
        if (d<.001||td>200.) break;
        p+=dir*d;
        td+=d;
        g+=.1/(.1+d)*hitbol*abs(normalize(point));
    }
    float hp=hexpos*(1.-hitbol);
    p-=dir*.01;
    vec3 n=normal(p);
    if (d<.001) {
        col=pow(max(0.,dot(-dir,n)),2.)*vec3(.6,.7,.8)*tcol*bcol;
    }
    col+=float(hexid);
    vec3 pr=pt;
    dir=reflect(dir,n);
    td=0.;
    for (int i=0; i<200; i++) {
        d=de(p);
        if (d<.001||td>200.) break;
        p+=dir*d;
        td+=d;
        g+=.1/(.1+d)*abs(normalize(point));
    }
    float zz=p.z;
    if (d<.001) {
        vec3 refcol=pow(max(0.,dot(-odir,n)),2.)*vec3(.6,.7,.8)*tcol*bcol;
        p=pr;
        p=abs(.5-fract(p*.1));
        float m=100.;
        for (int i=0; i<10; i++) {
            p=abs(p)/dot(p,p)-.8;
            m=min(m,length(p));
        }
        col=mix(col,refcol,m)-m*.3;
        col+=step(.3,hp)*step(.9,fract(pr.z*.05+iTime*.5+hp*.1))*.7;
        col+=step(.3,hexpos)*step(.9,fract(zz*.05+iTime+hexpos*.1))*.3;
    }
    col+=g*.03;
	col.rb*=rot(odir.y*.5);
	return col;
}


vec4 main( in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy-.5;
    uv.x*=iResolution.x/iResolution.y;
    float t=iTime*2.;
    vec3 from=path(t);
    if (mod(iTime-10.,20.)>10.) {
        from=path(floor(t/20.)*20.+10.);
        from.x+=2.;
    }
    hpos=path(t+3.);
    vec3 adv=path(t+2.);
    vec3 dir=normalize(vec3(uv,.7));
    vec3 dd=normalize(adv-from);
    point=normalize(adv-hpos);
    point.xz*=rot(sin(iTime)*.2);
    dir=lookat(dd)*dir;
    vec3 col = march(from, dir);
	col*=vec3(1.,.9,.8);
    return vec4(col,1.0);
})";
const char *SkSLCode3 = R"(
uniform float3 iResolution;
uniform float iTime;
uniform float4 iMouse;

// Frozen wasteland
// By Dave Hoskins
// https://www.shadertoy.com/view/Xls3D2
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.


#define ITR 90
#define FAR 110.
#alias time (iTime)
#alias MOD3 (vec3(.16532,.17369,.15787))
#alias SUN_COLOUR  (vec3(1., .95, .85))

float height(in vec2 p)
{
    float h = sin(p.x*.1+p.y*.2)+sin(p.y*.1-p.x*.2)*.5;
    h += sin(p.x*.04+p.y*.01+3.0)*4.;
    h -= sin(h*10.0)*.1;
    return h;
}

float camHeight(in vec2 p)
{
    float h = sin(p.x*.1+p.y*.2)+sin(p.y*.1-p.x*.2)*.5;
    h += sin(p.x*.04+p.y*.01+3.0)*4.;
    return h;
}

float smin( float a, float b)
{
	const float k = 2.7;
	float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
	return mix( b, a, h ) - k*h*(1.0-h);
}

#alias MOD2 (vec2(.16632,.17369))
#alias MOD3 (vec3(.16532,.17369,.15787))
float tri(in float x){return abs(fract(x)-.5);}

float hash12(vec2 p)
{
	p  = fract(p * MOD2);
    p += dot(p.xy, p.yx+19.19);
    return fract(p.x * p.y);
}
float vine(vec3 p, in float c, in float h)
{
    p.y += sin(p.z*.5625+1.3)*3.5-.5;
    p.x += cos(p.z*2.)*1.;
    vec2 q = vec2(mod(p.x, c)-c/2., p.y);
    return length(q) - h*1.4 -sin(p.z*3.+sin(p.x*7.)*0.5)*0.1;
}

vec3 tri3(in vec3 p){return vec3( tri(p.z+tri(p.y)), tri(p.z+tri(p.x)), tri(p.y+tri(p.x)));}
float Noise3d(in vec3 p)
{
    float z=1.4;
	float rz = 0.;
    vec3 bp = p;
	for (float i=0.; i<= 2.; i++ )
	{
        vec3 dg = tri3(bp);
        p += (dg);

        bp *= 2.;
		z *= 1.5;
		p *= 1.3;

        rz+= (tri(p.z+tri(p.x+tri(p.y))))/z;
        bp += 0.14;
	}
	return rz;
}

//--------------------------------------------------------------------------------
float map(vec3 p)
{
    p.y += height(p.zx);
    float d = p.y+.5;

    d = smin(d, vine(p+vec3(.8,0.,0),30.,3.3) );
    d = smin(d, vine(p.zyx+vec3(0.,0,17.),33.,1.4) );
    d += Noise3d(p*.05)*(p.y*1.2);
    p.xz *=.3;
    d+= Noise3d(p*.3);
    return d;
}
float fogmap(in vec3 p, in float d)
{
    p.xz -= time*7.+sin(p.z*.3)*3.;
    p.y -= time*.5;
    return (max(Noise3d(p*.008+.1)-.1,0.0)*Noise3d(p*.1))*.3;
}

float march(in vec3 ro, in vec3 rd, out float drift, in vec2 scUV)
{
	float precis = 0.1;
    float mul = .34;
    float h;
    float d = hash12(scUV)*1.5;
    drift = 0.0;
    for( int i=0; i<ITR; i++ )
    {
        vec3 p = ro+rd*d;
        h = map(p);
        if(h < precis*(1.0+d*.05) || d > FAR) break;
        drift +=  fogmap(p, d);
        d += h*mul;
        mul+=.004;
        //precis +=.001;
	 }
    drift = min(drift, 1.0);
	return d;
}

vec3 normal( in vec3 pos, in float d )
{
	vec2 eps = vec2( d *d* .003+.01, 0.0);
	vec3 nor = vec3(
	    map(pos+eps.xyy) - map(pos-eps.xyy),
	    map(pos+eps.yxy) - map(pos-eps.yxy),
	    map(pos+eps.yyx) - map(pos-eps.yyx) );
	return normalize(nor);
}

float bnoise(in vec3 p)
{
    p.xz*=.4;
    float n = Noise3d(p*3.)*0.4;
    n += Noise3d(p*1.5)*0.2;
    return n*n*.2;
}

vec3 bump(in vec3 p, in vec3 n, in float ds)
{
    p.xz *= .4;
    //p *= 1.0;
    vec2 e = vec2(.01,0);
    float n0 = bnoise(p);
    vec3 d = vec3(bnoise(p+e.xyy)-n0, bnoise(p+e.yxy)-n0, bnoise(p+e.yyx)-n0)/e.x;
    n = normalize(n-d*10./(ds));
    return n;
}

float shadow(in vec3 ro, in vec3 rd, in float mint)
{
	float res = 1.0;

    float t = mint;
    for( int i=0; i<12; i++ )
    {
		float h = map(ro + rd*t);
        res = min( res, 4.*h/t );
        t += clamp( h, 0.1, 1.5 );
            }
    return clamp( res, 0., 1.0 );
}

vec3 Clouds(vec3 sky, vec3 rd)
{

    rd.y = max(rd.y, 0.0);
    float ele = rd.y;
    float v = (200.0)/(abs(rd.y)+.01);

    rd.y = v;
    rd.xz = rd.xz * v - time*8.0;
	rd.xz *= .0004;

	float f = Noise3d(rd.xzz*3.) * Noise3d(rd.zxx*1.3)*2.5;
    f = f*pow(ele, .5)*2.;
  	f = clamp(f-.15, 0.01, 1.0);

    return  mix(sky, vec3(1),f );
}


vec3 Sky(vec3 rd, vec3 ligt)
{
    rd.y = max(rd.y, 0.0);

    vec3 sky = mix(vec3(.1, .15, .25), vec3(.8), pow(.8-rd.y, 3.0));
    return  mix(sky, SUN_COLOUR, min(pow(max(dot(rd,ligt), 0.0), 4.5)*1.2, 1.0));
}
float Occ(vec3 p)
{
    float h = 0.0;
    h  = clamp(map(p), 0.5, 1.0);
 	return sqrt(h);
}


vec4 main( in vec2 fragCoord )
{
	vec2 p = fragCoord.xy/iResolution.xy-0.2;
    vec2 q = fragCoord.xy/iResolution.xy;
	p = -p;
	q = -q;
	p.x*=iResolution.x/iResolution.y;
    vec2 mo = iMouse.xy / iResolution.xy-.5;
    mo = (mo==vec2(-.5))?mo=vec2(-0.1,0.07):mo;
	mo.x *= iResolution.x/iResolution.y;

	vec3 ro = vec3(0.+smoothstep(0.,1.,tri(time*1.5)*.3)*1.5, smoothstep(0.,1.,tri(time*3.)*3.)*0.08, -time*3.5-130.0);
    ro.y -= camHeight(ro.zx)-.4;
    mo.x += smoothstep(0.7,1.,sin(time*.35))*.5-1.5 - smoothstep(-.7,-1.,sin(time*.35))*.5;

    vec3 eyedir = normalize(vec3(cos(mo.x),mo.y*2.-.05+sin(time*.5)*0.1,sin(mo.x)));
    vec3 rightdir = normalize(vec3(cos(mo.x+1.5708),0.,sin(mo.x+1.5708)));
    vec3 updir = normalize(cross(rightdir,eyedir));
	vec3 rd=normalize((p.x*rightdir+p.y*updir)*1.+eyedir);

    vec3 ligt = normalize( vec3(1.5, .9, -.5) );
    float fg;
	float rz = march(ro,rd, fg, fragCoord);
	vec3 sky = Sky(rd, ligt);

    vec3 col = sky;

    if ( rz < FAR )
    {
        vec3 pos = ro+rz*rd;
        vec3 nor= normal( pos, rz);
        float d = distance(pos,ro);
        nor = bump(pos,nor,d);
        float shd = (shadow(pos,ligt,.04));

        float dif = clamp( dot( nor, ligt ), 0.0, 1.0 );
        vec3 ref = reflect(rd,nor);
        float spe = pow(clamp( dot( ref, ligt ), 0.0, 1.0 ),5.)*2.;

        float fre = pow( clamp(1.+dot(rd, nor),0.0,1.0), 3. );
        col = vec3(.75);
	    col = col*dif*shd + fre*spe*shd*SUN_COLOUR +abs(nor.y)*vec3(.12, .13, .13);
        // Fake the red absorption of ice...
        d = Occ(pos+nor*3.);
        col *= vec3(d, d, min(d*1.2, 1.0));
        // Fog from ice storm...
        col = mix(col, sky, smoothstep(FAR-25.,FAR,rz));

    }
    else
    {
        col = Clouds(col, rd);
    }

    // Fog mix...
    col = mix(col, vec3(0.6, .65, .7), fg);

    // Post...
    col = mix(col, vec3(.5), -.3);
    //col = col*col * (3.0-2.0*col);
	//col = clamp(pow(col,vec3(1.5)),0.0, 1.0);

	col = sqrt(col);


    // Borders...
    float f = smoothstep(0.0, 3.0, iTime)*.5;
    col *= f+f*pow(70. *q.x*q.y*(1.0-q.x)*(1.0-q.y), .2);


	return vec4( col, 1.0 );
}
)";
const char* SkSLCode4 = R"(
uniform float3 iResolution;
uniform float iTime;
uniform float4 iMouse;


#alias getNormal (getNormalHex)

//#define raymarch vanillaRayMarch
#alias raymarch (enchancedRayMarcher)

#define FAR 570.
#define INFINITY 1e32

#define FOG 1.

#define PI 3.14159265
#define TAU (2*PI)
#define PHI (1.618033988749895)

float vol = 0.;
float noise = 0.;

float hash12(vec2 p) {
	float h = dot(p,vec2(127.1,311.7));
    return fract(sin(h)*43758.5453123);
}

// 3d noise
float noise_3(in vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);
	vec3 u = f*f*(3.0-2.0*f);

    vec2 ii = i.xy + i.z * vec2(5.0);
    float a = hash12( ii + vec2(0.0,0.0) );
	float b = hash12( ii + vec2(1.0,0.0) );
    float c = hash12( ii + vec2(0.0,1.0) );
	float d = hash12( ii + vec2(1.0,1.0) );
    float v1 = mix(mix(a,b,u.x), mix(c,d,u.x), u.y);

    ii += vec2(5.0);
    a = hash12( ii + vec2(0.0,0.0) );
	b = hash12( ii + vec2(1.0,0.0) );
    c = hash12( ii + vec2(0.0,1.0) );
	d = hash12( ii + vec2(1.0,1.0) );
    float v2 = mix(mix(a,b,u.x), mix(c,d,u.x), u.y);

    return max(mix(v1,v2,u.z),0.0);
}

float fbm(vec3 x)
{
    float r = 0.0;
    float w = 1.0, s = 1.0;
    for (int i=0; i<7; i++)
    {
        w *= 0.5;
        s *= 2.0;
        r += w * noise_3(s * x);
    }
    return r;
}

vec3 fromRGB(int r, int g, int b) {
 	return vec3(float(r), float(g), float(b)) / 255.;
}

vec3
    light = vec3(0.0),
    p = vec3(0.),
    p2 = vec3(0.),
	lightDir = vec3(0.);

vec3 lightColour = normalize(vec3(1.8, 1.0, 0.3));

vec3 saturate(vec3 a) { return clamp(a, 0.0, 1.0); }
vec2 saturate(vec2 a) { return clamp(a, 0.0, 1.0); }
float saturate(float a) { return clamp(a, 0.0, 1.0); }

float smin( float a, float b, float k )
{
    float res = exp( -k*a ) + exp( -k*b );
    return -log( res )/k ;
}

void pR(inout vec2 p, float a) {
	p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

struct geometry {
    float dist;
    float materialIndex;
    float specular;
    float diffuse;
    vec3 space;
    vec3 color;
};

float fSphere(vec3 p, float r) {
	return length(p) - r;
}


geometry scene(vec3 p) {
    geometry plane;

    float localNoise = fbm(p / 10.) * 2.;;
    p.y -= localNoise * .2;
    plane.dist = p.y;
    p.y *= 3.5;

    plane.dist = smin(plane.dist, length(p) - 25., .15 + localNoise * .2);
    plane.dist = max(plane.dist, -length(p) + 29.  + localNoise);
    plane.materialIndex = 4.;
    plane.space = p;
    plane.color = vec3(1., .2, .0);
    plane.diffuse = 0.;
    plane.specular = 22.1;

    return plane;
}


float t_min = 0.001;
float t_max = FAR;
const int MAX_ITERATIONS = 90;

geometry enchancedRayMarcher(vec3 o, vec3 d, int maxI) {
    geometry mp;

    float t_min = 0.001;
    float tb = (2.1-o.y)/d.y;

    if( tb<0.0 ) { mp.dist = INFINITY; return mp; }

    t_min = tb;

    float omega = 1.3;
    float t = t_min;
    float candidate_error = INFINITY;
    float candidate_t = t_min;
    float previousRadius = 0.;
    float stepLength = 0.;
    float pixelRadius = //tan(radians (FOV) / 2.0)
        1. /350.;
    float functionSign = scene(o).dist < 0. ? -1. : +1.;


    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        if (maxI > 0 && i > maxI) break;
        mp = scene(d * t + o);
        float signedRadius = functionSign * mp.dist;
        float radius = abs(signedRadius);
        bool sorFail = omega > 1. &&
        (radius + previousRadius) < stepLength;
        if (sorFail) {
            stepLength -= omega * stepLength;
            omega = 1.;
        } else {
        	stepLength = signedRadius * omega * .8;
        }
        previousRadius = radius;
        float error = radius / t;
        if (!sorFail && error < candidate_error) {
            candidate_t = t;
            candidate_error = error;
        }
        if (!sorFail && error < pixelRadius || t > t_max) break;
        t += stepLength;
   	}

    mp.dist = candidate_t;

    if (
        (t > t_max || candidate_error > pixelRadius)
    	) mp.dist = INFINITY;


    return mp;
}

geometry vanillaRayMarch(vec3 o, vec3 d, int maxI) {
    geometry mp;

    float l = -.1;

    for (int i = 0; i < 30; i++) {
        if (abs(l) < 0.1 || l > 130.) {
            break;
        }

        mp = scene(o + d * l);
        l += mp.dist;
    }

    mp.dist = l;
    return mp;
}

float softShadow(vec3 ro, vec3 lp, float k) {
    const int maxIterationsShad = 125;
    vec3 rd = (lp - ro); // Unnormalized direction ray.


    float shade = 1.;
    float dist = 1.0;
    float end = max(length(rd), 0.01);
    float stepDist = end / float(maxIterationsShad);

    float tb = (8.0-ro.y)/normalize(rd).y;
    if( tb>0.0 ) end = min( end, tb );

    rd /= end;
    for (int i = 0; i < maxIterationsShad; i++) {
        float h = scene(ro + rd * dist).dist;
        shade = min(shade, smoothstep(0.0, 1.0, k * h / dist));
        dist += min(h, stepDist * 2.);
        if (h < 0.001 || dist > end) break;
    }
    return min(max(shade, 0.3), 1.0);
}


#define EPSILON .001
vec3 getNormalHex(vec3 pos)
{
	float d=scene(pos).dist;
	return normalize(
        vec3(
            scene(
                pos+vec3(EPSILON,0,0)).dist-d,
                scene(pos+vec3(0,EPSILON,0)).dist-d,
                scene(pos+vec3(0,0,EPSILON)).dist-d
        	)
    	);
}

float getAO(vec3 hitp, vec3 normal, float dist)
{
    vec3 spos = hitp + normal * dist;
    float sdist = scene(spos).dist;
    return clamp(sdist / dist, 0.4, 1.0);
}

vec3 Sky(in vec3 rd, bool showSun, vec3 lightDir)
{
   	float sunSize = 1.;
   	float sunAmount = max(dot(rd, lightDir), .1);
   	float v = pow(1.2 - max(rd.y, .5), 1.1);
   	vec3 sky = mix(fromRGB(255,200,100), vec3(1.1, 1.2, 1.3) / 10., v);
   	sky += lightColour * sunAmount * sunAmount * 1. + lightColour * min(pow(sunAmount, 1e4)* sunSize,1233. * sunSize);

   	return clamp(sky, 0.0, 1.0);
}

vec3 doColor( in vec3 sp, in vec3 rd, in vec3 sn, in vec3 lp, geometry obj) {
	vec3 sceneCol = vec3(0.0);
    lp = sp + lp;
    vec3 ld = lp - sp; // Light direction vector.
    float lDist = max(length(ld / 2.), 0.001); // Light to surface distance.
    ld /= lDist; // Normalizing the light vector.
	float atten = 1. / (1.0 + lDist * 0.025 + lDist * lDist * 0.2);
	float diff = max(dot(sn, ld), obj.diffuse);
    float spec = max(dot(reflect(-ld, sn), -rd), obj.specular / 2.);
	vec3 objCol = obj.color;//getObjectColor(sp, sn, obj);
	sceneCol += (objCol * (diff + .15) * spec * 0.1);// * atten;
    return sceneCol;
}


vec4 main(in vec2 fragCoord) {

    //lightColour = normalize(fromRGB(133,7,142)) * 2.;

    vec2 uv = -(fragCoord.xy / iResolution.xy - .1);

    uv.y *= 1.2;
    vol = 4.;
    float t2 = iTime;
    float
        sk = sin(-t2 * .1) * 48.0,
        ck = cos(-t2 * .1) * 48.0,

        mat = 0.;

    light = vec3(0., 7., 100.);
    lightDir = light;


    vec3
        vuv = vec3(0., 1., 0. ),
    	ro = vec3(ck, 18., sk);

    vec3
        vrp =  vec3(0., 0., 0.),

    	vpn = normalize(vrp - ro),
    	u = normalize(cross(vuv, vpn)),
    	v = cross(vpn, u),
    	vcv = (ro + vpn),
    	scrCoord = (vcv + uv.x * u * iResolution.x/iResolution.y + uv.y * v),
    	rd = normalize(scrCoord - ro),
        hit;

    vec3 sceneColor = vec3(0.);

    geometry tr = raymarch(ro, rd, 0);

    float fog = smoothstep(FAR * FOG, 0., tr.dist) * 1.;
    hit = ro + rd * tr.dist;

    vec3 sn = getNormal(hit);

    float sh = softShadow(hit, hit + light, 8.2);

    float
        ao = getAO(hit, sn, 10.2);

    noise = fbm(mod(hit + 20. + hit.yzx* 4., 40.) / 1.);
	vec3 sky = Sky(rd, true, normalize(light)) * 1.;

    if (tr.dist < FAR) {
        sceneColor = (doColor(hit, rd, sn, light, tr) * 1.) * 1.;
        sceneColor *= ao;
        sceneColor *= sh;
        sceneColor = mix(sceneColor, sky, saturate(tr.dist * 4.5 / FAR));
    } else {
        sceneColor = sky;
    }

    vec4 fragColor = vec4(clamp(sceneColor * (1. - length(uv) / 3.5), 0.0, 1.0), 1.0);
    fragColor = pow(fragColor, 1./vec4(1.2));
	return fragColor;
})";
const char* SkSLCode5 = R"(
uniform float3 iResolution;
uniform float iTime;
uniform float4 iMouse;



#define MDIST 150.0
#define STEPS 164.0
#define pi 3.1415926535
#define pmod(p,x) (mod(p,x)+0.5*(x))
#define rot(a) (mat2(cos(a),sin(a),-sin(a),cos(a)))

//this is a useless trick but it's funny
#define vmm(v,minOrMax) (minOrMax(v.x,minOrMax(v.y,v.z)))


//iq box sdf
float ebox( vec3 p, vec3 b ){
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(vmm(q,max),0.0);
}
//iq palette
vec3 pal( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d ){
    return a + b*cos(2.*pi*(c*t+d));
}
float h11 (float a) {
    return fract(sin((a)*12.9898)*43758.5453123);
}
//https://www.shadertoy.com/view/fdlSDl
vec2 tanha(vec2 x) {
  vec2 x2 = x*x;
  return clamp(x*(27.0 + x2)/(27.0+9.0*x2), -1.0, 1.0);
}
float tanha(float x) {
  float x2 = x*x;
  return clamp(x*(27.0 + x2)/(27.0+9.0*x2), -1.0, 1.0);
}

struct sdResult
{
    vec2 center;
    vec2 dim;
    float id;
    float vol;
};

sdResult subdiv(vec2 p,float seed){
    vec2 dMin = vec2(-10.);
    vec2 dMax = vec2(10.);
    float t = iTime*0.6;
    float t2 = iTime;
    vec2 dim = dMax - dMin;
    float id = 0.;
    const float ITERS = 6.;

    float MIN_SIZE = 0.1;
    float MIN_ITERS = 1.;

    //big thanks to @0b5vr for letting me use his cleaner subdiv implementation
    //https://www.shadertoy.com/view/NsKGDy
    vec2 diff2 = vec2(1);
    for(float i = 0.;i<ITERS;i++){
        vec2 divHash=tanha(vec2(sin(t2*pi/3.+id+i*t2*0.05),cos(t2*pi/3.+h11(id)*100.+i*t2*0.05))*3.)*0.35+0.5;
        //divHash=vec2(sin(t*pi/3.+id),cos(t*pi/3.+h11(id)*100.))*0.5+0.5;
        //if(iMouse.z>0.5){divHash = mix(divHash,M,0.9);}
        divHash = mix(vec2(0.5),divHash,tanha(sin(t*0.8)*5.)*0.2+0.4);
        vec2 divide = divHash * dim + dMin;
        divide = clamp(divide, dMin + MIN_SIZE+0.01, dMax - MIN_SIZE-0.01);
        vec2 minAxis = min(abs(dMin - divide), abs(dMax - divide));
        float minSize = min( minAxis.x, minAxis.y);
        bool smallEnough = minSize < MIN_SIZE;
        if (smallEnough && i + 1. > MIN_ITERS) { break; }
        dMax = mix( dMax, divide, step( p, divide ));
        dMin = mix( divide, dMin, step( p, divide ));
        diff2 =step( p, divide)-
        vec2(h11(diff2.x+seed)*10.,h11(diff2.y+seed)*10.);
        id = length(diff2)*100.0;
        dim = dMax - dMin;
    }
    vec2 center = (dMin + dMax)/2.0;
    sdResult result;
    result.center = center;
    result.id = id;
    result.dim = dim;
    result.vol = dim.x*dim.y;
    return result;
}
vec3 rdg = vec3(0);
float dibox(vec3 p,vec3 b,vec3 rd){
    vec3 dir = sign(rd)*b;
    vec3 rc = (dir-p)/rd;
    return min(rc.x,rc.z)+0.01;
}
bool traverse = true;
vec3 map(vec3 p){
    float seed = sign(p.y)-0.3;
    seed = 1.;
    //p.y = abs(p.y)-4.;

    vec2 a = vec2(99999,1);
    vec2 b = vec2(2);

    a.x = p.y-2.0;
    float id = 0.;
    if(a.x<0.1||!traverse){
        float t = iTime;
        sdResult sdr = subdiv(p.xz,seed);
        vec3 centerOff = vec3(sdr.center.x,0,sdr.center.y);
        vec2 dim = sdr.dim;

        float rnd = 0.05;
        float size = min(dim.y,dim.x)*1.;
        //size = 1.;
        size+=(sin((centerOff.x+centerOff.z)*0.6+t*4.5)*0.5+0.5)*2.;
        size = min(size,4.0);
        a.x = ebox(p-centerOff-vec3(0,0,0),vec3(dim.x,size,dim.y)*0.5-rnd)-rnd;
        if(traverse){
            b.x = dibox(p-centerOff,vec3(dim.x,1,dim.y)*0.5,rdg);
            a = (a.x<b.x)?a:b;
        }
        id = sdr.id;
    }
    return vec3(a,id);
}
vec3 norm(vec3 p){
    vec2 e = vec2(0.01,0.);
    return normalize(map(p).x-vec3(
    map(p-e.xyy).x,
    map(p-e.yxy).x,
    map(p-e.yyx).x));
}
vec4 main( in vec2 fragCoord ){
	fragCoord = fragCoord;
    vec2 R = iResolution.xy;
    vec2 uv = (fragCoord-0.5*R.xy)/R.y;
	uv = 0. - uv;
    vec3 col = vec3(0);

    vec3 ro = vec3(0,6.,-12)*1.2;

    ro.xz*=rot(0.35);
    vec3 lk = vec3(-1,-3,0.5);
    if(iMouse.z>0.){
       ro*=2.;
       lk = vec3(0);
       ro.yz*=rot(2.0*(iMouse.y/iResolution.y-0.5));
       ro.zx*=rot(-9.0*(iMouse.x/iResolution.x-0.5));
    }
    vec3 f = vec3(normalize(lk-ro));
    vec3 r = normalize(cross(vec3(0,1,0),f));
    vec3 rd = normalize(f*(1.8)+r*uv.x+uv.y*cross(f,r));
    rdg = rd;
    vec3 p = ro;
    float dO =0.;
    vec3 d;
    bool hit = false;

    for(float i = 0.; i<STEPS; i++){
        p = ro+rd*dO;
        d = map(p);
        dO+=d.x;
        if(d.x<0.005){
            hit = true;
            break;
        }
        if(dO>MDIST)break;
    }

    if(hit&&d.y!=2.0){
        traverse = false;
        vec3 n = norm(p);
        vec3 r = reflect(rd,n);
        vec3 e = vec3(0.5);
        vec3 al = pal(fract(d.z)*0.35-0.8,e*1.2,e,e*2.0,vec3(0,0.33,0.66));
        col = al;
        vec3 ld = normalize(vec3(0,45,0)-p);

        //sss from nusan
        float sss=0.1;
        const float sssteps = 10.;
        for(float i=1.; i<sssteps; ++i){
            float dist = i*0.2;
            sss += smoothstep(0.,1.,map(p+ld*dist).x/dist)/(sssteps*1.5);
        }
        sss = clamp(sss,0.0,1.0);

        float diff = max(0.,dot(n,ld))*0.7+0.3;
        float amb = dot(n,ld)*0.45+0.55;
        float spec = pow(max(0.,dot(r,ld)),13.0);
        //blackle ao
        #define AO(a,n,p) (smoothstep(-a,a,map(p+n*a).x))
        float ao = AO(0.1,n,p)*AO(.2,n,p)*AO(.3,n,p);

        spec = smoothstep(0.,1.,spec);
        col = vec3(0.204,0.267,0.373)*
        mix(vec3(0.169,0.000,0.169),vec3(0.984,0.996,0.804),mix(amb,diff,0.75))
        +spec*0.3;
        col+=sss*al;
        col*=mix(ao,1.,0.65);
        col = pow(col,vec3(0.85));
    }
    else{
    	col = mix(vec3(0.373,0.835,0.988),vec3(0.424,0.059,0.925),length(uv));
    }

    col *=1.0-0.5*pow(length(uv*vec2(0.8,1.)),2.7);
    vec3 col2 = smoothstep(vec3(0.0, 0.0, 0.0), vec3(1.1, 1.1, 1.3), col);
    col = mix(col,col2,0.5)*1.05;

    return vec4(col,1.0);
})";
const char* SkSLCode6= R"(
uniform float3 iResolution;
uniform float iTime;
uniform float4 iMouse;

float arms, time,
g = 0.;

struct Hit {
float d; // SDF distance.
int id; // Material ID.
vec2 t; // uv texture coords.
};

float n21(vec2 p) {
const vec3 s = vec3(7, 157, 0);
vec2 h,
   ip = floor(p);
p = fract(p);
p = p * p * (3. - 2. * p);
h = s.zy + dot(ip, s.xy);
h = mix(fract(sin(h) * 43.5453), fract(sin(h + s.x) * 43.5453), p.x);
return mix(h.x, h.y, p.y);
}

void minH(inout Hit a, Hit b) { if (b.d < a.d) a = b; }

mat2 rot(float a) {
float c = cos(a),
	s = sin(a);
return mat2(c, s, -s, c);
}

float sdBox(vec3 p, vec3 b) {
vec3 q = abs(p) - b;
return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
}

float sdCyl(vec3 p, vec2 hr) {
vec2 d = abs(vec2(length(p.zy), p.x)) - hr;
return min(max(d.x, d.y), 0.) + length(max(d, 0.));
}

float sdTaper(vec2 p, vec3 r) {
p.x = abs(p.x);
p.y = -p.y;
float b = (r.x - r.y) / r.z,
	a = sqrt(1. - b * b),
	k = dot(p, vec2(-b, a));
if (k > a * r.z) return length(p - vec2(0, r.z)) - r.y;
return (k < 0. ? length(p) : dot(p, vec2(a, b))) - r.x;
}

float sdArm(vec3 p, float c) {
return max(max(sdTaper(p.yz, vec3(.5, .2, 2)), abs(abs(p.x) - 1.55)) - .1,  // Main arm.
(p.z + 1.5 + sin(p.y * 7.) * .1) * c);
}

// The sine wave applied to the dirt track.
float trk(float z) { return sin(z * .2 - time); }

// Map the scene using SDF functions.
Hit map(vec3 p) {
// Floor.
Hit h = Hit(length(p.y), 1, p.xz);

// Walls.
minH(h, Hit(min(dot(p, vec3(-.707, .707, 0)) + 9., dot(p, vec3(-1, 0, 0)) + 20.), 0, p.yz));

// Wall lights.
float f, b,
	d = sdBox(vec3(p.xy, mod(p.z - time * 8., 40.)) - vec3(20, 7.25, 20), vec3(.1, .1, 8));
g += .01 / (.01 + d * d);
minH(h, Hit(d - .7, 4, p.xy));
p.x += trk(-.9);
p.xz *= rot(trk(7.) * .2);

// Ball.
minH(h, Hit(length(p - vec3(0, 1, 0)) - 1., 2, p.xy));
p.yz *= rot(-arms);
p.y -= 1.7;
minH(h, Hit(sdBox(p, vec3(1.2, .9, .8 + cos((p.y + 5.1) * 1.33) * .5)) - .2, 3, p.xy)); // Lower body.
vec3 op = p;

// Arms/Hands.
p.y -= .5;
p.yz *= rot(-arms);
d = sdArm(p, -1.);
p.z += arms + cos(time * 15.) * .1;
minH(h, Hit(min(d, sdArm(p, 1.)), 0, op.xy));

// Brush.
b = .2 + .8 * abs(sin(p.x * 18.)) * .1;
minH(h, Hit(sdCyl(p + vec3(0, 0, 2), vec2(.1 + b, 1.4)), 2, p.xy));

// Arm extenders.
p.x = abs(p.x) - 1.55;
p.z++;
minH(h, Hit(sdBox(p, vec3(.08, .22, .6)), 2, p.xz));

// Head.
p = op;
p.yz *= rot(arms * -.7);
p.xz *= rot(trk(0.) * .1);
p.y -= 2.;
p.z += .5;
f = cos(p.y + .8);
minH(h, Hit(max(sdBox(p, vec3(mix(1.2, 1.4, f), .8, mix(.9, 1.2, f))),  // Head.
2.5 - length(p.yz + vec2(2.5, -.8)) // Rear cut-out.
) - .2, 6, p.xy));

// Light - Top.
p.y -= mix(.96, 2.5, arms);
minH(h, Hit(sdBox(p, vec3(.5, .04, .5)), 3, p.xy));
minH(h, Hit(sdBox(p + vec3(0, .3, 0), vec3(.45, .3, .45)), 5, p.xy));
return h;
}

vec3 calcN(vec3 p) {
const vec2 e = vec2(.024, -.024);
return normalize(e.xyy * map(p + e.xyy).d + e.yyx * map(p + e.yyx).d + e.yxy * map(p + e.yxy).d + e.xxx * map(p + e.xxx).d);
}

float calcShadow(vec3 p, vec3 ld) {
float h,
	s = 1.,
	t = .1;
for (int i = 0; i < 20; i++) {
  h = map(p + ld * t).d;
  s = min(s, 15. * h / t);
  t += h;
  if (s < .001 || t > 2.7) break;
}

return clamp(s, 0., 1.);
}

// Quick ambient occlusion.
float ao(vec3 p, vec3 n, float h) { return map(p + h * n).d / h; }

/**********************************************************************************/
vec3 vig(vec3 c, vec2 fc) {
vec2 q = fc.xy / iResolution.xy;
c *= .5 + .5 * pow(16. * q.x * q.y * (1. - q.x) * (1. - q.y), .4);
return c;
}

// Calculate the floor normal vector.
vec2 flrN(vec2 t) {
return n21(vec2(t.x * 1.2, t.y)) * .3 +  // Surface texture.
smoothstep(0., .04, abs(sin(t * .5)));
}

vec3 mat(Hit h, vec3 p, inout vec3 n) {
if (h.id == 1) {
  // Floor
  vec2 d,
	   t = h.t + vec2(trk(p.z), time * -5.);
  n.xz += flrN(vec2(h.t.x, t.y));
  n = normalize(n);
  float mm = n21(mod(t * 10., 20.));
  d = abs(vec2(abs(abs(t.x) - .8) - .3, mod(t.y, .4) - .1)) - vec2(.2, .1);
  return vec3(.3, .4, .5) * mix(1., mm, (1. - step(0., min(max(d.x, d.y), 0.))) * step(p.z, -2.5));
}

if (h.id == 2) return vec3(.1); // Ball, brush
if (h.id == 3) return vec3(1. - step(abs(h.t.y + .7), .15) * .9); // White body
if (h.id == 4) return vec3(25); // Wall light.
if (h.id == 5) return vec3(1, 0, 0);
if (h.id == 6) {
  // Face
  vec2 t = vec2(abs(h.t.x), h.t.y);
  if (t.y < -.3 && t.x < 1.) {
	  t.x += arms * .4;
	  float l = .3 + .7 * abs(sin(t.y * 50.));
	  t *= rot(.6 * arms);
	  return .01 + vec3(1.5, 1.5, 0) * step(abs(t.x - .3), .25) * step(abs(t.y + .7), .1 - arms * .1) * l;
  }
}

return vec3(1);
}

vec3 lights(vec3 p, vec3 rd, Hit h) {
vec3 n = calcN(p),
   ld = normalize(vec3(6, 3, -10) - p);
return mat(h, p, n) // Material color.
* ((max(0., .1 + .9 * dot(ld, n)) // Primary light.
+ max(0., .1 + .9 * dot(ld * vec3(-1, 0, -1), n)) // Bounce light.
) * mix(.3, .6, calcShadow(p, ld)) // Shadows.
* mix(ao(p, n, .3), ao(p, n, 2.), .7) // Ambient occlusion.
+ pow(max(0., dot(rd, reflect(ld, n))), 30.) // Specular.
) * vec3(2, 1.8, 1.7);
}

vec3 march(vec3 ro, vec3 rd) {
// Raymarch.
vec3 p, n, c;
float d = .01;
Hit h;
for (int i = 0; i < 110; i++) {
  p = ro + rd * d;
  h = map(p);
  if (abs(h.d) < .0015) break;
  d += h.d;
}

// Calculate pixel color.
c = lights(p, rd, h) + g;
if (h.id == 1) {
  // Ray hit the floor - Apply reflection.
  n = calcN(p);
  n.xz -= flrN(p.xz - vec2(0, time * 5.)) * .024;
  rd = reflect(rd, normalize(n));
  d = .5;
  ro = p;
  for (int i = 0; i < 90; i++) {
	  p = ro + rd * d;
	  h = map(p);
	  if (abs(h.d) < .0015) break;
	  d += h.d;
  }

  if (abs(h.d) < .0015) c = mix(c, mat(h, p, n), .01);
}

return c;
}

vec4 main(vec2 fc) {
time = mod(iTime, 30.) - 1.;
arms = (time < 0. ? smoothstep(-1., 0., time) : abs(sin(time * 10.) * .1) + .9) * .38;
time = max(0., time);
vec3 ro = vec3(-7, 4, -7. - sin(time * .3)),
   col = vec3(0);
for (float dx = 0.; dx <= 1.; dx++) {
  for (float dy = 0.; dy <= 1.; dy++) {
	  vec2 uv = (fc + vec2(dx, dy) * .5 - .5 * iResolution.xy) / iResolution.y;
  	  uv = -uv;
	  vec3 f = normalize(vec3(0, 3, -4) - ro),
		   r = normalize(cross(vec3(0, 1, 0), f));
	  col += march(ro, normalize(f + r * uv.x + cross(f, r) * uv.y));
  }
}

col /= 4.;
return vec4(vig(pow(col, vec3(.45)), fc), 1.);
})";

const char* SkSLCodes[6] = {
	SkSLCode1, SkSLCode2, SkSLCode3, SkSLCode4, SkSLCode5, SkSLCode6
};

int CodeIndex = 0;

/**
 * Init OpenGL interface object
 */
void InitGLInterface();
/**
 * Create GLFW window
 */
void InitWindow();
/**
 * The frame buffer call back function
 */
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height);
/**
 * The call back for key in glfw
 * @param Window
 * @param Key
 * @param Scancode
 * @param Action
 * @param Mod
 */
void KeyCallBack(GLFWwindow *Window, int Key, int Scancode, int Action, int Mod);
/**
 * Parse the SkSL code
 */
void ParseShader();
/**
 * Create the custom shader object
 * @return The SkShader Object
 */
sk_sp<SkShader> MakeShader();
/**
 * The error call back function of GLFW
 * @param Error The error code
 * @param Description The description string of the error
 */
void ErrorCallBack(int Error, const char *Description);
/**
 * Call Skia API to draw context
 */
void Draw(int Width, int Height);
/**
 * Init the uniform variable of Skia
 */
void InitUniform();

int main() {
	InitWindow();
	InitGLInterface();
	InitUniform();
	ParseShader();

	Draw(WIDTH, HEIGHT);
	time_t step = clock();
	using namespace std::chrono;
	while (!glfwWindowShouldClose(GLWindow)) {
		if (clock() - step >= 8) {
			step = clock();
			Draw(WIDTH, HEIGHT);
		}
		glfwPollEvents();

		std::this_thread::sleep_for(1ms);
	}

	return 0;
}

void InitUniform() {
	StartTime = clock();
}
void InitGLInterface() {
	GLInterface = sk_make_sp<VRenderInterface>();
}
void InitWindow() {
	glfwSetErrorCallback(ErrorCallBack);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	GLWindow = glfwCreateWindow(WIDTH, HEIGHT, "Skia Tester", nullptr, nullptr);
	if (!GLWindow) {
		printf("Failed to create GLFW window!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetFramebufferSizeCallback(GLWindow, FrameBufferCallBack);
	glfwSetKeyCallback(GLWindow, KeyCallBack);

	glfwMakeContextCurrent(GLWindow);
}
void KeyCallBack(GLFWwindow *Window, int Key, int Scancode, int Action, int Mod) {
	if (Key == GLFW_KEY_LEFT && Action == GLFW_RELEASE) {
		if (CodeIndex - 1 >= 0) {
			--CodeIndex;

			InitUniform();
			ParseShader();
		}
	}
	if (Key == GLFW_KEY_RIGHT && Action == GLFW_RELEASE) {
		if (CodeIndex + 1 < 6) {
			++CodeIndex;

			InitUniform();
			ParseShader();
		}
	}
}
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height) {
	Draw(Width, Height);
}
void Draw(int Width, int Height) {
	WIDTH = Width;
	HEIGHT = Height;

	sk_sp<VRenderTarget> glRenderTarget =
		sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = Width, .Height = Height, .X = 0, .Y = 0});
	sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(GLInterface);
	sk_sp<VSurface>		  glSurface =
		sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);
	SkPaint shaderPlayground;

	shaderPlayground.setShader(MakeShader());

	auto canvas = glSurface->GetNativeSurface()->getCanvas();

	canvas->drawPaint(shaderPlayground);
	canvas->flush();
	glContext->GetNativeContext()->flushAndSubmit();

	glfwSwapBuffers(GLWindow);
}
void ErrorCallBack(int Error, const char *Description) {
	fputs(Description, stderr);
}

sk_sp<SkShader> MakeShader() {
	float iResolution[8] = { static_cast<float>(WIDTH), static_cast<float>(WIDTH), static_cast<float>(HEIGHT), static_cast<float>(float(clock() - StartTime) / 1000.f),
							0.f, 0.f, 0.f, 0.f };

	sk_sp<SkData> uniform = SkData::MakeWithCopy(iResolution, sizeof(float) * 8);

	return Shader->makeShader(uniform, {});
}
void ParseShader() {
	auto result = SkRuntimeEffect::MakeForShader(SkString(SkSLCodes[CodeIndex]));
	Shader = result.effect;
	if (!result.errorText.isEmpty()) {
		printf("%s", result.errorText.c_str());
		_flushall();
		exit(EXIT_FAILURE);
	}
}