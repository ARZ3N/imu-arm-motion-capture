#ifndef QUAT_H
#define QUAT_H

typedef struct {
    float w;
    float x;
    float y;
    float z;
}quaternion;

quaternion q_add(const quaternion q, const quaternion p);
quaternion q_sub(const quaternion q, const quaternion p);
quaternion q_S_mul(const float m, const quaternion q);
quaternion q_V_mul(const quaternion q, const quaternion v);
quaternion q_conjugate(const quaternion );
float q_norm(const quaternion q);
quaternion q_inv(const quaternion q);
quaternion q_normalize(quaternion q);
quaternion q_form_rq(float angle, quaternion v);
quaternion q_rotate(quaternion q_delta, quaternion p);

#endif