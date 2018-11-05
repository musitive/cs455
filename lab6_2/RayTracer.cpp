#include "RayTracer.h"

double RayTracer::jitter(int i, double scale) {
    uniform_real_distribution<double> unif(0,scale);
    default_random_engine re;
    return i + unif(re);
}

Ray RayTracer::computeRay(double i, double j, double angle, double aspectratio, Environment* env) {
    double xx = (2 * (i * 1/env->width) - 1) * angle * aspectratio;
    double yy = (1 - 2 * (j * 1/env->height)) * angle;
    Position look_from = env->from;
    Position look_at = Position(xx,yy,0) + env->at;

    Ray ray = Ray(look_from, look_at);
    return ray;
}

Ray RayTracer::jitter(Ray ray) {
    uniform_real_distribution<double> unif(0,0.1);
    default_random_engine re;
    ray.rd = Direction(Position(ray.rd.x+unif(re),ray.rd.y+unif(re),ray.rd.z+unif(re)));
    return ray;
}

Colori RayTracer::trace(Ray ray, Environment* env, Object* current, const int& depth) {
    if (depth >= MAX_DEPTH)
        return Colori(255);

    Position p = Position(numeric_limits<double>::infinity());
    Object* closest = NULL;

    for(Object* o: env->env) {
        if (o != current) {
            Position np = o->findIntersect(ray);
            if(length(np - ray.r0) < length(p - ray.r0)) {
                closest = o;
                p = np;
            }
        }
    }
    
    if (!closest)
        return Colori(env->background.x*255,env->background.y*255,env->background.z*255);
    else {
        bool blocked = false;
        Light light = { env->light_color, env->light_position };
        Direction l = light.position - p;
        Ray toLight(p, l);
        double distance = length(light.position - p);
        for (Object* o: env->env) {
            if (o != closest) {
                if (length(light.position - o->findIntersect(toLight, false)) < distance) {
                    blocked = true;
                    break;
                }
            }
        }
        if (closest->getMaterial()->isReflective) {
            Direction n = closest->computeNormal(p);
            double dt = dot(ray.rd, n);
            Direction r = n * dt * 2 - ray.rd;
            Ray newRay(p, -r);
            Colori ci = closest->computeColor(ray.r0, p, light, false, env->ambient_light);
            Colord cd = Colord(ci.x/255.,ci.y/255.,ci.z/255.);
            Colori ct = Colori();
            for(int y = 0; y < 4; y++) {
                ct = ct + trace(jitter(newRay), env, closest, depth+1);
            }
            ct = ct / 4;
            return Colori(ct.x*cd.x,ct.y*cd.y,ct.z*cd.z);
        }
        else
            return closest->computeColor(ray.r0, p, light, blocked, env->ambient_light);
    }
}

vector<Colori> RayTracer::subdivide(int i, int j, double scale, double angle, double aspectratio, Environment* env) {
    vector<Colori> colors = vector<Colori>();
    for(int y = 0; y < scale; y++) {
        for(int x = 0; x < scale; x++) {
            colors.push_back(trace(computeRay(jitter(i, 1 / scale) + x / scale, jitter(j, 1 / scale) + y / scale, angle, aspectratio, env), env, NULL, 0));
        }
    }
    return colors;
}