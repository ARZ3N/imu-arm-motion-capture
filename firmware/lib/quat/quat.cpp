#include "quat.h"
#include<math.h>

quaternion q_add(const quaternion q, const quaternion p){
    quaternion a = {q.w + p.w, q.x + p.x, q.y + p.y, q.z + p.z};
    return a;
}

quaternion q_sub(const quaternion q, const quaternion p){
    quaternion d = {q.w - p.w, q.x - p.x, q.y - p.y, q.z - p.z};
    return d;
}

quaternion q_S_mul(const float m, const quaternion q){
    quaternion p = {q.w * m, q.x * m, q.y * m, q.z * m};
    return p;
}

quaternion q_V_mul(const quaternion q, const quaternion p){
    quaternion vp = {
        ((q.w*p.w) - (q.x*p.x) - (q.y*p.y) - (q.z*p.z)),
        ((q.w*p.x) + (q.x*p.w) + (q.y*p.z) - (q.z*p.y)),
        ((q.w*p.y) - (q.x*p.z) + (q.y*p.w) + (q.z*p.x)),
        ((q.w*p.z) + (q.x*p.y) - (q.y*p.x) + (q.z*p.w))
    };
    return vp;
}
quaternion q_conjugate(const quaternion q){
    quaternion c = {
        q.w,
        -1.0*q.x,
        -1.0*q.y,
        -1.0*q.z
    };
    return c;
}
float q_norm(const quaternion q){
    float nm;
    nm = sqrt(pow(q.w, 2) + pow(q.x, 2) + pow(q.y, 2) + pow(q.z, 2));
    return nm;
}
quaternion q_inv(const quaternion q){
    if((q.w == 0)&&(q.x == 0)&&(q.y == 0)&&(q.z == 0)){
        return q;
    }
    else{
        quaternion q1 = q_conjugate(q);
        float m = (1.0/pow(q_norm(q), 2));
        q1 = q_S_mul(m, q1);
        return q1;
    }    
}

quaternion q_normalize(quaternion q){
    float qmag = q_norm(q);
    if(qmag != 0){
        quaternion nq;
        nq.w = q.w/qmag;
        nq.x = q.x/qmag;
        nq.y = q.y/qmag;
        nq.z = q.z/qmag;
        return nq;
    }
    else{ return q;}
}

quaternion q_form_rq(float angle, quaternion v){
    q_normalize(v);
    quaternion q = { 
        cos(angle/2.0), 
        (v.x)*sin(angle/2.0),
        (v.y)*sin(angle/2.0),
        (v.z)*sin(angle/2.0)
    };
    return q_normalize(q);
}
//! ensure q_delta is a rotation quaternion that is normalized!!
//! <p> : Vector (3d point) to be rotated
//! <q_delta> : rotation quaternion
quaternion q_rotate(quaternion q_delta, quaternion p){
    quaternion qr;
    qr = q_V_mul(q_delta, p);
    qr = q_V_mul(qr, q_inv(q_delta));
    return qr;
}